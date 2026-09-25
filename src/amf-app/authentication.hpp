/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AUTHENTICATION_H_
#define _AUTHENTICATION_H_

#include <mysql/mysql.h>
#include <pthread.h>

#include <cstring>
#include <memory>
#include <string>

#include "conversions.hpp"
#include "nas_context.hpp"

namespace amf_application {

#define KEY_LENGTH (16)
#define SQN_LENGTH (6)
#define RAND_LENGTH (16)
#define AMF_LENGTH (2)

typedef struct {
  uint8_t key[KEY_LENGTH];
  uint8_t sqn[SQN_LENGTH];
  uint8_t opc[KEY_LENGTH];
  uint8_t amf[AMF_LENGTH];  // authenticationManagementField
} mysql_auth_info_t;

// Decodes exactly 2*N hex characters from a const char `src` into an array
// `des`. Returns false, if any of:
//   * src == nullptr          -- row[n] is a real null pointer for a NULL
//   column
//   * strlen(src) != 2 * N    -- wrong length, subsumes the odd-length case
//   * any character outside [0-9A-Fa-f]
// On success it has written exactly N bytes and returns true.
//
template<size_t N>
static bool decode_fixed_hex(const char* src, uint8_t (&des)[N]) {
  if (src == nullptr) return false;
  if (std::strlen(src) != 2 * N) return false;
  for (size_t i = 0; i < 2 * N; ++i) {
    const char c      = src[i];
    const bool is_hex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
                        (c >= 'A' && c <= 'F');
    if (!is_hex) return false;
  }
  oai::utils::conv::hex_str_to_uint8(src, des);  // input now fully validated
  return true;
}

// Parses a MySQL `json` column value and extracts the 6-byte SQN.
// Returns false and writes nothing if: json_text == nullptr; parse throws;
// "sqn" is absent or not a string; its length != 12; or it is not all hex.
bool parse_sequence_number(const char* json_text, uint8_t (&sqn)[SQN_LENGTH]);

// Serialises the UDM-shaped document:
//   {"sqnScheme":"NON_TIME_BASED","sqn":"<12 lower hex>",
//    "lastIndexes":{"ausf":0}}
std::string build_sequence_number_json(const uint8_t (&sqn)[SQN_LENGTH]);

// Increment SQN: 48-bit SQN + 32
void increment_sqn(const uint8_t (&in)[SQN_LENGTH], uint8_t (&out)[SQN_LENGTH]);

typedef struct {
  MYSQL* db_conn;
  std::string server;
  std::string user;
  std::string password;
  std::string database;
  pthread_mutex_t db_cs_mutex;
} database_t;

class authentication {
 public:
  authentication();
  virtual ~authentication() {};

  static authentication& get_instance() {
    static authentication instance;
    return instance;
  }

  /*
   * Generate the Authentication Vectors (locally at AMF)
   * @param [std::shared_ptr<nas_context>&] nc: Pointer to the UE NAS Context
   * @return true if generated successfully, otherwise return false
   */
  bool authentication_vectors_generator_in_ausf(
      std::shared_ptr<nas_context>& nc);

  /*
   * Generate the Authentication Vectors (RANDOM, locally at AMF)
   * @param [std::shared_ptr<nas_context>&] nc: Pointer to the UE NAS Context
   * @return true if generated successfully, otherwise return false
   */
  bool authentication_vectors_generator_in_udm(
      std::shared_ptr<nas_context>& nc);

  /*
   * Get the Authentication info from the DB (MySQL)
   * @param [const std::string&] imsi: UE IMSI
   * @param [mysql_auth_info_t&] resp: Response from MySQL
   * @return true if retrieve successfully, otherwise return false
   */
  bool get_mysql_auth_info(const std::string& imsi, mysql_auth_info_t& resp);

  /*
   * Write the `sequenceNumber` JSON document for this subscriber (MySQL).
   * @param [const std::string&] imsi: UE IMSI, used as `ueid`
   * @param [const uint8_t(&)[SQN_LENGTH]] sqn: SQN to store
   * @return true if the UPDATE executed successfully, otherwise false
   */
  bool mysql_write_sqn(
      const std::string& imsi, const uint8_t (&sqn)[SQN_LENGTH]);

  /*
   * Establish the connection to the DB (MySQL)
   * @return true if successfully, otherwise return false
   */
  bool connect_to_mysql();

  /*
   * Generate a RAND with corresponding length
   * @param [uint8_t*] random_p: RAND
   * @param [ssize_t] length: length of RAND
   * @return void
   */
  void generate_random(uint8_t* random_p, ssize_t length);

  /*
   * Generate 5G HE AV Vector (locally at AMF)
   * @param [uint8_t[16]] opc: OPC
   * @param [const std::string&] imsi: UE IMSI
   * @param [uint8_t[16]] key: Operator Key
   * @param [uint8_t[16]] sqn: SQN
   * @param [std::string&] serving_network: Serving Network
   * @param [_5G_HE_AV_t&] vector: Generated vector
   * @param [const uint8_t[AMF_LENGTH]] amf: AMF field from the DB
   * @return void
   */
  void generate_5g_he_av_in_udm(
      const uint8_t opc[16], const std::string& imsi, uint8_t key[16],
      uint8_t sqn[6], std::string& serving_network, _5G_HE_AV_t& vector,
      const uint8_t amf[AMF_LENGTH]);

  /*
   * Perform annex_a_4_33501 algorithm
   * @param [uint8_t[16]] ck: ck
   * @param [uint8_t[16]] ik: ik
   * @param [uint8_t*] input: input
   * @param [uint8_t[16]] rand: rand
   * @param [std::string&] serving_network: Serving Network
   * @param [uint8_t*] output: output
   * @return void
   */
  void annex_a_4_33501(
      uint8_t ck[16], uint8_t ik[16], uint8_t* input, uint8_t rand[16],
      std::string& serving_network, uint8_t* output);

  /*
   * Sha256 Algorithm
   * @param [unsigned char*] message: Input message
   * @param [int] msg_len: Length of the input message
   * @param [unsigned char*] output: Output message
   * @return void
   */
  void apply_sha256(unsigned char* message, int msg_len, unsigned char* output);

 private:
  /*
   * mysql_real_escape_string wrapper. Returns false if the connection is
   * unusable or the escape fails; `out` is only written on success.
   */
  bool sql_escape(const std::string& in, std::string& out);

  static uint8_t no_random_delta;
  random_state_t random_state;
  database_t db_desc;
};
}  // namespace amf_application

#endif
