/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "authentication.hpp"

#include <nlohmann/json.hpp>
#include <vector>

#include "amf_config.hpp"
#include "bstrlib.h"
#include "nas_algorithms.hpp"
#include "output_wrapper.hpp"
#include "sha256.hpp"
#include "utils.hpp"

extern "C" {
#include "OCTET_STRING.h"
}

using namespace amf_application;
using namespace oai::config;
extern std::unique_ptr<oai::config::amf_config> amf_cfg;

// Static variables
uint8_t authentication::no_random_delta = 0;

authentication::authentication() {
  db_desc         = {};
  db_desc.db_conn = nullptr;
}

//------------------------------------------------------------------------------
bool authentication::authentication_vectors_generator_in_ausf(
    std::shared_ptr<nas_context>& nc) {
  // Note: ref A.5@3GPP TS 33.501
  Logger::authentication().debug(
      "Generate Authentication Vectors in AUSF (locally in AMF)");
  uint8_t inputString[MAX_5GS_AUTH_VECTORS][40];
  for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
    memcpy(&inputString[i][0], nc->_5g_he_av[i].rand, 16);
    memcpy(&inputString[i][16], nc->_5g_he_av[i].xresStar, 16);
    unsigned char sha256Out[Sha256::DIGEST_SIZE];
    apply_sha256(
        (unsigned char*) inputString[i], AUTH_VECTOR_LENGTH_OCTETS, sha256Out);
    for (int j = 0; j < 16; j++)
      nc->_5g_av[i].hxresStar[j] = (uint8_t) sha256Out[j];
    memcpy(nc->_5g_av[i].rand, nc->_5g_he_av[i].rand, 16);
    memcpy(nc->_5g_av[i].autn, nc->_5g_he_av[i].autn, 16);
    uint8_t kseaf[AUTH_VECTOR_LENGTH_OCTETS];
    Authentication_5gaka::derive_kseaf(
        nc->serving_network, nc->_5g_he_av[i].kausf, kseaf);
    memcpy(nc->_5g_av[i].kseaf, kseaf, AUTH_VECTOR_LENGTH_OCTETS);
  }
  return true;
}

//------------------------------------------------------------------------------
bool authentication::authentication_vectors_generator_in_udm(
    std::shared_ptr<nas_context>& nc) {
  Logger::authentication().debug(
      "Generate Authentication Vectors in UDM (locally in AMF)");
  _5G_HE_AV_t* vector = nc->_5g_he_av;

  if (!connect_to_mysql()) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return false;
  }
  Logger::authentication().debug("Connected to MySQL successfully");

  mysql_auth_info_t mysql_resp = {};
  if (!get_mysql_auth_info(nc->imsi, mysql_resp)) {
    Logger::authentication().error("Failed to fetch user data from MySQL");
    return false;
  }

  Logger::authentication().debug(
      "Received information from MySQL for IMSI %s", nc->imsi.c_str());

  // Use the Base SQN from the DB
  uint8_t base_sqn[SQN_LENGTH] = {};
  memcpy(base_sqn, mysql_resp.sqn, SQN_LENGTH);

  uint8_t* auts = (uint8_t*) bdata(nc->auts);
  if (auts) {
    Logger::authentication().debug("AUTS present, deriving SQN_MS");
    // AUTS = SQN_MS xor AK (6 octets) || MAC-S (8 octets), TS 33.102 6.3.3.
    // Reject a short AUTS before touching it.
    if (blength(nc->auts) < SQN_LENGTH + MAC_S_LENGTH) {
      Logger::authentication().error(
          "AUTS too short (%d octets, expected %d)", blength(nc->auts),
          SQN_LENGTH + MAC_S_LENGTH);
      // Clear AUTS for the next round
      oai::utils::utils::bdestroy_wrapper(&nc->auts);
      return false;
    }
    // SQN_MS derivation
    uint8_t* sqn_ms = Authentication_5gaka::sqn_ms_derive(
        mysql_resp.opc, mysql_resp.key, auts, vector[0].rand);
    if (sqn_ms) {
      memcpy(base_sqn, sqn_ms, SQN_LENGTH);
      oai::utils::utils::free_wrapper((void**) &sqn_ms);
    } else {
      Logger::authentication().warn(
          "AUTS present but SQN_MS could not be verified; using the stored "
          "SQN");
    }
    // Clear AUTS for next round
    oai::utils::utils::bdestroy_wrapper(&nc->auts);
    auts = nullptr;
  } else {
    Logger::authentication().debug("No AUTS ...");
  }

  // Increment SQN: the vector is built with base + 32, never with the stored
  // value itself.
  uint8_t current_sqn[SQN_LENGTH] = {};
  increment_sqn(base_sqn, current_sqn);

  for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
    generate_random(vector[i].rand, RAND_LENGTH);
    oai::utils::output_wrapper::print_buffer(
        "authentication", "Generated random rand (5G HE AV)", vector[i].rand,
        16);
    generate_5g_he_av_in_udm(
        mysql_resp.opc, nc->imsi, mysql_resp.key, current_sqn,
        nc->serving_network, vector[i], mysql_resp.amf);
  }

  // Increment SQN: store base + 64 for the next round and store in the DB.
  uint8_t next_sqn[SQN_LENGTH] = {};
  increment_sqn(current_sqn, next_sqn);
  if (!mysql_write_sqn(nc->imsi, next_sqn)) {
    Logger::authentication().error(
        "Failed to update sequenceNumber in MySQL for ueid %s",
        nc->imsi.c_str());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void authentication::generate_random(uint8_t* random_p, ssize_t length) {
  gmp_randinit_default(random_state.state);
  gmp_randseed_ui(random_state.state, time(NULL));
  if (amf_cfg->auth_para.random) {
    Logger::authentication().debug("Database config random -> true");
    random_t random_nb;
    mpz_init(random_nb);
    mpz_init_set_ui(random_nb, 0);
    pthread_mutex_lock(&random_state.lock);
    mpz_urandomb(random_nb, random_state.state, 8 * length);
    pthread_mutex_unlock(&random_state.lock);
    mpz_export(random_p, NULL, 1, length, 0, 0, random_nb);
    int r = 0, mask = 0, shift;
    for (int i = 0; i < length; i++) {
      if ((i % sizeof(i)) == 0) r = rand();
      shift       = 8 * (i % sizeof(i));
      mask        = 0xFF << shift;
      random_p[i] = (r & mask) >> shift;
    }
  } else {
    Logger::authentication().error("Database config random -> false");
    pthread_mutex_lock(&random_state.lock);
    for (int i = 0; i < length; i++) {
      random_p[i] = i + no_random_delta;
    }
    no_random_delta += 1;
    pthread_mutex_unlock(&random_state.lock);
  }
}

//------------------------------------------------------------------------------
void authentication::generate_5g_he_av_in_udm(
    const uint8_t opc[16], const std::string& imsi, uint8_t key[16],
    uint8_t sqn[6], std::string& serving_network, _5G_HE_AV_t& vector,
    const uint8_t amf[AMF_LENGTH]) {
  Logger::authentication().debug("Generate 5g_he_av as in UDM");
  uint8_t mac_a[8];
  uint8_t ck[16];
  uint8_t ik[16];
  uint8_t ak[6];
  uint64_t _imsi = oai::utils::utils::fromString<uint64_t>(imsi);

  Authentication_5gaka::f1(
      opc, key, vector.rand, sqn, amf,
      mac_a);  // to compute MAC, Figure 7, ts33.102
  // oai::utils::output_wrapper::print_buffer("authentication", "Result For
  // F1-Alg: mac_a", mac_a, 8);
  Authentication_5gaka::f2345(
      opc, key, vector.rand, vector.xres, ck, ik,
      ak);  // to compute XRES, CK, IK, AK
  annex_a_4_33501(
      ck, ik, vector.xres, vector.rand, serving_network, vector.xresStar);
  // oai::utils::output_wrapper::print_buffer("authentication", "Result For KDF:
  // xres*(5G HE AV)", vector.xresStar, 16);
  Authentication_5gaka::generate_autn(
      sqn, ak, amf, mac_a,
      vector.autn);  // generate AUTN
  // oai::utils::output_wrapper::print_buffer("authentication", "Generated
  // autn(5G HE AV)", vector.autn, 16);
  Authentication_5gaka::derive_kausf(
      ck, ik, serving_network, sqn, ak,
      vector.kausf);  // derive Kausf
  // oai::utils::output_wrapper::print_buffer("authentication", "Result For KDF:
  // Kausf(5G HE AV)", vector.kausf, AUTH_VECTOR_LENGTH_OCTETS);
  Logger::authentication().debug("Generate_5g_he_av_in_udm finished!");
  return;
}

//------------------------------------------------------------------------------
void authentication::annex_a_4_33501(
    uint8_t ck[16], uint8_t ik[16], uint8_t* input, uint8_t rand[16],
    std::string& serving_network, uint8_t* output) {
  OCTET_STRING_t netName = {};
  if (OCTET_STRING_fromBuf(
          &netName, serving_network.c_str(), serving_network.length()) != 0) {
    Logger::authentication().error(
        "Could not encode Network Name for XRES* derivation");
    return;
  }
  uint8_t S[100];
  S[0] = 0x6B;
  memcpy(&S[1], netName.buf, netName.size);
  S[1 + netName.size] = (netName.size & 0xff00) >> 8;
  S[2 + netName.size] = (netName.size & 0x00ff);
  for (int i = 0; i < 16; i++) S[3 + netName.size + i] = rand[i];
  S[19 + netName.size] = 0x00;
  S[20 + netName.size] = 0x10;
  for (int i = 0; i < 8; i++) S[21 + netName.size + i] = input[i];
  S[29 + netName.size] = 0x00;
  S[30 + netName.size] = 0x08;

  uint8_t plmn[3] = {0x46, 0x0f, 0x11};
  uint8_t oldS[100];
  oldS[0] = 0x6B;
  memcpy(&oldS[1], plmn, 3);
  oldS[4] = 0x00;
  oldS[5] = 0x03;
  for (int i = 0; i < 16; i++) oldS[6 + i] = rand[i];
  oldS[22] = 0x00;
  oldS[23] = 0x10;
  for (int i = 0; i < 8; i++) oldS[24 + i] = input[i];
  oldS[32] = 0x00;
  oldS[33] = 0x08;
  oai::utils::output_wrapper::print_buffer(
      "authentication", "Input string: ", S, 31 + netName.size);
  uint8_t key[AUTH_VECTOR_LENGTH_OCTETS];
  memcpy(&key[0], ck, 16);
  memcpy(&key[16], ik, 16);  // KEY
  // Authentication_5gaka::kdf(key, AUTH_VECTOR_LENGTH_OCTETS, oldS, 33, output,
  // 16);
  uint8_t out[AUTH_VECTOR_LENGTH_OCTETS];
  Authentication_5gaka::kdf(key, 32, S, 31 + netName.size, out, 32);
  for (int i = 0; i < 16; i++) output[i] = out[16 + i];
  oai::utils::output_wrapper::print_buffer(
      "authentication", "XRES*(new)", out, AUTH_VECTOR_LENGTH_OCTETS);
  free(netName.buf);
}

//------------------------------------------------------------------------------
void authentication::apply_sha256(
    unsigned char* message, int msg_len, unsigned char* output) {
  memset(output, 0, Sha256::DIGEST_SIZE);
  Sha256 ctx = {};
  ctx.init();
  ctx.update(message, msg_len);
  ctx.finalResult(output);
}

//------------------------------------------------------------------------------
bool amf_application::parse_sequence_number(
    const char* json_text, uint8_t (&sqn)[SQN_LENGTH]) {
  if (json_text == nullptr) {
    Logger::authentication().error("sequenceNumber column is NULL");
    return false;
  }
  nlohmann::json doc;
  try {
    doc = nlohmann::json::parse(json_text);
  } catch (const std::exception& e) {
    Logger::authentication().error(
        "sequenceNumber is not valid JSON: %s", e.what());
    return false;
  }
  if (!doc.is_object() || !doc.contains("sqn") || !doc["sqn"].is_string()) {
    Logger::authentication().error(
        "sequenceNumber has no string member \"sqn\"");
    return false;
  }
  const std::string sqn_s = doc["sqn"].get<std::string>();
  if (!decode_fixed_hex(sqn_s.c_str(), sqn)) {  // N=6 -> exactly 12 hex chars
    Logger::authentication().error(
        "sequenceNumber.sqn is not 12 hex characters");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
std::string amf_application::build_sequence_number_json(
    const uint8_t (&sqn)[SQN_LENGTH]) {
  nlohmann::json doc;
  doc["sqnScheme"]   = "NON_TIME_BASED";
  doc["sqn"]         = oai::utils::conv::uint8_to_hex_string(sqn, SQN_LENGTH);
  doc["lastIndexes"] = nlohmann::json{{"ausf", 0}};
  return doc.dump();
}

//------------------------------------------------------------------------------
void amf_application::increment_sqn(
    const uint8_t (&in)[SQN_LENGTH], uint8_t (&out)[SQN_LENGTH]) {
  uint64_t v = 0;
  for (size_t i = 0; i < SQN_LENGTH; ++i) v = (v << 8) | in[i];
  v = (v + 32) & 0xFFFFFFFFFFFFULL;  // 48-bit SQN, TS 33.102
  for (size_t i = 0; i < SQN_LENGTH; ++i)
    out[SQN_LENGTH - 1 - i] = static_cast<uint8_t>((v >> (8 * i)) & 0xFF);
}

//------------------------------------------------------------------------------
bool authentication::get_mysql_auth_info(
    const std::string& imsi, mysql_auth_info_t& resp) {
  resp = {};

  if (!db_desc.db_conn) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return false;
  }

  // Escaping dereferences the connection, so it is done inside the lock.
  pthread_mutex_lock(&db_desc.db_cs_mutex);
  std::string ueid;
  if (!sql_escape(imsi, ueid)) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error("Failed to escape ueid");
    return false;
  }

  constexpr size_t I_METHOD = 0;
  constexpr size_t I_KEY    = 1;
  constexpr size_t I_OPC    = 2;
  constexpr size_t I_SQN    = 3;
  constexpr size_t I_AMF    = 4;
  const std::string query =
      "SELECT `authenticationMethod`,`encPermanentKey`,`encOpcKey`,"
      "`sequenceNumber`,`authenticationManagementField` "
      "FROM `AuthenticationSubscription` WHERE `ueid`='" +
      ueid + "'";

  if (mysql_query(db_desc.db_conn, query.c_str())) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error(
        "Query execution failed: %s", mysql_error(db_desc.db_conn));
    return false;
  }
  MYSQL_RES* res = mysql_store_result(db_desc.db_conn);
  pthread_mutex_unlock(&db_desc.db_cs_mutex);
  if (!res) {
    Logger::authentication().error("Data fetched from MySQL is not present");
    return false;
  }

  MYSQL_ROW row = mysql_fetch_row(res);
  if (row == nullptr) {
    Logger::authentication().error(
        "No AuthenticationSubscription row for ueid %s", imsi.c_str());
    mysql_free_result(res);
    return false;
  }

  // Validate and decode into locals; resp is written only on full success.
  uint8_t key[KEY_LENGTH] = {};
  uint8_t opc[KEY_LENGTH] = {};
  uint8_t sqn[SQN_LENGTH] = {};
  uint8_t amf[AMF_LENGTH] = {};
  bool ok                 = true;

  if (row[I_METHOD] == nullptr) {
    Logger::authentication().error("authenticationMethod is NULL");
    ok = false;
  } else {
    const std::string method(row[I_METHOD]);
    if (method != "5G_AKA" && method != "AuthenticationVector") {
      Logger::authentication().error(
          "Unsupported authenticationMethod '%s'", method.c_str());
      ok = false;
    }
  }
  if (ok && !decode_fixed_hex(row[I_KEY], key)) {  // N=16 -> 32 chars
    Logger::authentication().error("encPermanentKey is not 32 hex characters");
    ok = false;
  }
  if (ok && !decode_fixed_hex(row[I_OPC], opc)) {  // N=16 -> 32 chars
    Logger::authentication().error("encOpcKey is not 32 hex characters");
    ok = false;
  }
  if (ok && !parse_sequence_number(row[I_SQN], sqn)) {
    ok = false;
  }
  if (ok && !decode_fixed_hex(row[I_AMF], amf)) {  // N=2 -> 4 chars
    Logger::authentication().error(
        "authenticationManagementField is not 4 hex characters");
    ok = false;
  }

  mysql_free_result(res);
  if (!ok) {
    Logger::authentication().error(
        "Rejecting AuthenticationSubscription row for ueid %s", imsi.c_str());
    return false;
  }
  memcpy(resp.key, key, KEY_LENGTH);
  memcpy(resp.opc, opc, KEY_LENGTH);
  memcpy(resp.sqn, sqn, SQN_LENGTH);
  memcpy(resp.amf, amf, AMF_LENGTH);
  return true;
}

//------------------------------------------------------------------------------
bool authentication::connect_to_mysql() {
  const int mysql_reconnect_val = 1;

  pthread_mutex_init(&db_desc.db_cs_mutex, NULL);
  db_desc.server   = amf_cfg->auth_para.mysql_server;
  db_desc.user     = amf_cfg->auth_para.mysql_user;
  db_desc.password = amf_cfg->auth_para.mysql_pass;
  db_desc.database = amf_cfg->auth_para.mysql_db;
  db_desc.db_conn  = mysql_init(NULL);
  mysql_options(db_desc.db_conn, MYSQL_OPT_RECONNECT, &mysql_reconnect_val);
  if (!mysql_real_connect(
          db_desc.db_conn, db_desc.server.c_str(), db_desc.user.c_str(),
          db_desc.password.c_str(), db_desc.database.c_str(), 0, NULL, 0)) {
    Logger::authentication().error(
        "An error occurred while connecting to db: %s",
        mysql_error(db_desc.db_conn));
    mysql_thread_end();
    return false;
  }
  mysql_set_server_option(db_desc.db_conn, MYSQL_OPTION_MULTI_STATEMENTS_ON);
  return true;
}

//------------------------------------------------------------------------------
bool authentication::sql_escape(const std::string& in, std::string& out) {
  if (!db_desc.db_conn) return false;
  // mysql_real_escape_string contract: the output buffer is 2 * len + 1.
  std::vector<char> buf(2 * in.size() + 1);
  const unsigned long n = mysql_real_escape_string(
      db_desc.db_conn, buf.data(), in.c_str(),
      static_cast<unsigned long>(in.size()));
  if (n == static_cast<unsigned long>(-1)) return false;
  out.assign(buf.data(), n);
  return true;
}

//------------------------------------------------------------------------------
bool authentication::mysql_write_sqn(
    const std::string& imsi, const uint8_t (&sqn)[SQN_LENGTH]) {
  int status     = 0;
  MYSQL_RES* res = nullptr;

  if (!db_desc.db_conn) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return false;
  }

  const std::string doc = build_sequence_number_json(sqn);

  pthread_mutex_lock(&db_desc.db_cs_mutex);

  std::string doc_escaped;
  std::string ueid;
  if (!sql_escape(doc, doc_escaped) || !sql_escape(imsi, ueid)) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error("Failed to escape the UPDATE parameters");
    return false;
  }
  const std::string query =
      "UPDATE `AuthenticationSubscription` SET `sequenceNumber`='" +
      doc_escaped + "' WHERE `ueid`='" + ueid + "'";

  if (mysql_query(db_desc.db_conn, query.c_str())) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error(
        "Query execution failed: %s", mysql_error(db_desc.db_conn));
    return false;
  }

  bool ok = true;
  do {
    res = mysql_store_result(db_desc.db_conn);
    if (res) {
      mysql_free_result(res);
    } else {
      if (mysql_field_count(db_desc.db_conn) == 0) {
        Logger::authentication().debug(
            "[MySQL] %lld rows affected", mysql_affected_rows(db_desc.db_conn));
      } else { /* some error occurred */
        Logger::authentication().error("Could not retrieve result set");
        ok = false;
        break;
      }
    }
    if ((status = mysql_next_result(db_desc.db_conn)) > 0) {
      Logger::authentication().error("Could not execute statement");
      ok = false;
    }
  } while (status == 0);
  pthread_mutex_unlock(&db_desc.db_cs_mutex);
  return ok;
}
