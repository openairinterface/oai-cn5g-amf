/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include "authentication.hpp"

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
extern amf_config amf_cfg;

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
  // TODO: remove naked ptr
  Logger::authentication().debug(
      "Generate Authentication Vectors in UDM (locally in AMF)");
  uint8_t* sqn        = nullptr;
  uint8_t* auts       = (uint8_t*) bdata(nc->auts);
  _5G_HE_AV_t* vector = nc->_5g_he_av;
  // Access to MySQL to fetch UE-related information
  if (!connect_to_mysql()) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return false;
  }
  Logger::authentication().debug("Connected to MySQL successfully");
  mysql_auth_info_t mysql_resp = {};
  if (get_mysql_auth_info(nc->imsi, mysql_resp)) {
    if (auts) {
      sqn = Authentication_5gaka::sqn_ms_derive(
          mysql_resp.opc, mysql_resp.key, auts, mysql_resp.rand);
      if (sqn) {
        generate_random(vector[0].rand, RAND_LENGTH);
        mysql_push_rand_sqn(nc->imsi, vector[0].rand, sqn);
        mysql_increment_sqn(nc->imsi);
        oai::utils::utils::free_wrapper((void**) &sqn);
      }
      if (!get_mysql_auth_info(nc->imsi, mysql_resp)) {
        Logger::authentication().error("Cannot get data from MySQL");
        return false;
      }
      sqn = mysql_resp.sqn;
      for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
        generate_random(vector[i].rand, RAND_LENGTH);
        oai::utils::output_wrapper::print_buffer(
            "authentication", "Generated random rand (5G HE AV)",
            vector[i].rand, 16);
        generate_5g_he_av_in_udm(
            mysql_resp.opc, nc->imsi, mysql_resp.key, sqn, nc->serving_network,
            vector[i]);  // serving network name
      }
      mysql_push_rand_sqn(nc->imsi, vector[MAX_5GS_AUTH_VECTORS - 1].rand, sqn);
    } else {
      Logger::authentication().debug("No AUTS ...");
      Logger::authentication().debug(
          "Receive information from MySQL with IMSI %s", nc->imsi.c_str());
      for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
        generate_random(vector[i].rand, RAND_LENGTH);
        sqn = mysql_resp.sqn;
        generate_5g_he_av_in_udm(
            mysql_resp.opc, nc->imsi, mysql_resp.key, sqn, nc->serving_network,
            vector[i]);  // serving network name
      }
      mysql_push_rand_sqn(nc->imsi, vector[MAX_5GS_AUTH_VECTORS - 1].rand, sqn);
    }
    mysql_increment_sqn(nc->imsi);
  } else {
    Logger::authentication().error("Failed to fetch user data from MySQL");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void authentication::generate_random(uint8_t* random_p, ssize_t length) {
  gmp_randinit_default(random_state.state);
  gmp_randseed_ui(random_state.state, time(NULL));
  if (amf_cfg.auth_para.random) {
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
    uint8_t sqn[6], std::string& serving_network, _5G_HE_AV_t& vector) {
  Logger::authentication().debug("Generate 5g_he_av as in UDM");
  uint8_t amf[] = {0x80, 0x00};
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
  OCTET_STRING_t netName;
  OCTET_STRING_fromBuf(
      &netName, serving_network.c_str(), serving_network.length());
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
bool authentication::get_mysql_auth_info(
    const std::string& imsi, mysql_auth_info_t& resp) {
  MYSQL_RES* res;
  MYSQL_ROW row;
  std::string query;

  if (!db_desc.db_conn) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return false;
  }
  query =
      "SELECT `key`,`sqn`,`rand`,`OPc` FROM `users` WHERE `users`.`imsi`='" +
      imsi + "' ";
  pthread_mutex_lock(&db_desc.db_cs_mutex);
  if (mysql_query(db_desc.db_conn, query.c_str())) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error(
        "Query execution failed: %s\n", mysql_error(db_desc.db_conn));
    return false;
  }
  res = mysql_store_result(db_desc.db_conn);
  pthread_mutex_unlock(&db_desc.db_cs_mutex);
  if (!res) {
    Logger::authentication().error("Data fetched from MySQL is not present");
    return false;
  }
  if (row = mysql_fetch_row(res)) {
    if (row[0] == NULL || row[1] == NULL || row[2] == NULL || row[3] == NULL) {
      Logger::authentication().error("row data failed");
      return false;
    }
    memcpy(resp.key, row[0], KEY_LENGTH);
    uint64_t sqn = 0;
    sqn          = atoll(row[1]);
    resp.sqn[0]  = (sqn & (255UL << 40)) >> 40;
    resp.sqn[1]  = (sqn & (255UL << 32)) >> 32;
    resp.sqn[2]  = (sqn & (255UL << 24)) >> 24;
    resp.sqn[3]  = (sqn & (255UL << 16)) >> 16;
    resp.sqn[4]  = (sqn & (255UL << 8)) >> 8;
    resp.sqn[5]  = (sqn & 0xff);
    memcpy(resp.rand, row[2], RAND_LENGTH);
    memcpy(resp.opc, row[3], KEY_LENGTH);
  }
  mysql_free_result(res);
  return true;
}

//------------------------------------------------------------------------------
bool authentication::connect_to_mysql() {
  const int mysql_reconnect_val = 1;

  pthread_mutex_init(&db_desc.db_cs_mutex, NULL);
  db_desc.server   = amf_cfg.auth_para.mysql_server;
  db_desc.user     = amf_cfg.auth_para.mysql_user;
  db_desc.password = amf_cfg.auth_para.mysql_pass;
  db_desc.database = amf_cfg.auth_para.mysql_db;
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
void authentication::mysql_push_rand_sqn(
    const std::string& imsi, uint8_t* rand_p, uint8_t* sqn) {
  int status = 0;
  MYSQL_RES* res;
  char query[1000];
  int query_length     = 0;
  uint64_t sqn_decimal = 0;
  if (!db_desc.db_conn) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return;
  }
  if (!sqn || !rand_p) {
    Logger::authentication().error("Need sqn and rand");
    return;
  }
  sqn_decimal = ((uint64_t) sqn[0] << 40) | ((uint64_t) sqn[1] << 32) |
                ((uint64_t) sqn[2] << 24) | (sqn[3] << 16) | (sqn[4] << 8) |
                sqn[5];
  query_length = sprintf(query, "UPDATE `users` SET `rand`=UNHEX('");
  for (int i = 0; i < RAND_LENGTH; i++) {
    query_length += sprintf(&query[query_length], "%02x", rand_p[i]);
  }

  query_length +=
      sprintf(&query[query_length], "'),`sqn`=%" PRIu64, sqn_decimal);
  query_length +=
      sprintf(&query[query_length], " WHERE `users`.`imsi`='%s'", imsi.c_str());
  pthread_mutex_lock(&db_desc.db_cs_mutex);
  if (mysql_query(db_desc.db_conn, query)) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error(
        "Query execution failed: %s", mysql_error(db_desc.db_conn));
    return;
  }
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
        break;
      }
    }
    if ((status = mysql_next_result(db_desc.db_conn)) > 0)
      Logger::authentication().error("Could not execute statement");
  } while (status == 0);
  pthread_mutex_unlock(&db_desc.db_cs_mutex);
  return;
}

//------------------------------------------------------------------------------
void authentication::mysql_increment_sqn(const std::string& imsi) {
  int status;
  MYSQL_RES* res;
  char query[1000];
  if (db_desc.db_conn == NULL) {
    Logger::authentication().error("Cannot connect to MySQL DB");
    return;
  }
  sprintf(
      query, "UPDATE `users` SET `sqn` = `sqn` + 32 WHERE `users`.`imsi`='%s'",
      imsi.c_str());

  pthread_mutex_lock(&db_desc.db_cs_mutex);

  if (mysql_query(db_desc.db_conn, query)) {
    pthread_mutex_unlock(&db_desc.db_cs_mutex);
    Logger::authentication().error(
        "Query execution failed: %s", mysql_error(db_desc.db_conn));
    return;
  }
  do {
    res = mysql_store_result(db_desc.db_conn);
    if (res) {
      mysql_free_result(res);
    } else {
      if (mysql_field_count(db_desc.db_conn) == 0) {
        Logger::authentication().debug(
            "[MySQL] %lld rows affected", mysql_affected_rows(db_desc.db_conn));
      } else {
        Logger::authentication().error("Could not retrieve result set");
        break;
      }
    }
    if ((status = mysql_next_result(db_desc.db_conn)) > 0)
      Logger::authentication().error("Could not execute statement");
  } while (status == 0);
  pthread_mutex_unlock(&db_desc.db_cs_mutex);
  return;
}
