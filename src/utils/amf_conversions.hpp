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

#ifndef FILE_AMF_CONVERSIONS_HPP_SEEN
#define FILE_AMF_CONVERSIONS_HPP_SEEN

#include <netinet/in.h>
#include <stdint.h>

#include <iostream>
#include <string>

#include "bstrlib.h"
#include "conversions.hpp"
#include "utils.hpp"

extern "C" {
#include "BIT_STRING.h"
#include "OCTET_STRING.h"
}

class amf_conv : public oai::utils::conv {
 public:
  static void msg_str_2_msg_hex(std::string msg, bstring& b);
  static char* bstring2charString(bstring b);
  static unsigned char* format_string_as_hex(std::string str);
  static void bstring_2_string(const bstring& b_str, std::string& str);
  static void string_2_bstring(const std::string& str, bstring& b_str);
  // TODO: bitstring_2_int32
  static void to_lower(std::string& str);
  static void to_lower(bstring& b_str);
  static std::string get_ue_context_key(
      const uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id);
  static std::string get_serving_network_name(
      const std::string& mnc, const std::string& mcc);
  static std::string uint32_to_hex_string_full_format(uint32_t value);
  static std::string tmsi_to_guti(
      const std::string& mcc, const std::string& mnc, uint8_t region_id,
      const std::string& _5g_s_tmsi);
  static std::string tmsi_to_guti(
      const std::string& mcc, const std::string& mnc, uint8_t region_id,
      uint16_t amf_set_id, uint8_t amf_pointer, const std::string& tmsi);
  static std::string imsi_to_supi(const std::string& imsi);
  static std::string get_imsi(
      const std::string& mcc, const std::string& mnc, const std::string& msin);
};
#endif /* FILE_CONVERSIONS_HPP_SEEN */
