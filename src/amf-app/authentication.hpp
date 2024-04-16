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

#ifndef _AUTHENTICATION_H_
#define _AUTHENTICATION_H_

#include <mysql/mysql.h>
#include <pthread.h>

#include <memory>
#include <string>

#include "nas_context.hpp"

namespace amf_application {

#define KEY_LENGTH (16)
#define SQN_LENGTH (6)
#define RAND_LENGTH (16)

typedef struct {
  uint8_t key[KEY_LENGTH];
  uint8_t sqn[SQN_LENGTH];
  uint8_t opc[KEY_LENGTH];
  uint8_t rand[RAND_LENGTH];
} mysql_auth_info_t;

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
  virtual ~authentication(){};

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
   * Update the RAND and SQN to the DB (MySQL)
   * @param [const std::string&] imsi: UE IMSI
   * @param [uint8_t*] rand_p: RAND
   * @param [uint8_t*] sqn: SQN
   * @return void
   */
  void mysql_push_rand_sqn(
      const std::string& imsi, uint8_t* rand_p, uint8_t* sqn);

  /*
   * Increment SQN in the DB for next round (MySQL)
   * @param [const std::string&] imsi: UE IMSI
   * @return void
   */
  void mysql_increment_sqn(const std::string& imsi);

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
   * @return void
   */
  void generate_5g_he_av_in_udm(
      const uint8_t opc[16], const std::string& imsi, uint8_t key[16],
      uint8_t sqn[6], std::string& serving_network, _5G_HE_AV_t& vector);

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
  static uint8_t no_random_delta;
  random_state_t random_state;
  database_t db_desc;
};
}  // namespace amf_application

#endif
