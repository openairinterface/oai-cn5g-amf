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

#include "amf_conversions.hpp"

#include <arpa/inet.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <sstream>

#include "amf.hpp"
#include "logger.hpp"
#include "output_wrapper.hpp"

constexpr uint8_t kUint32Length =
    8;  // 4 bytes  -8 characters representation in hex

//------------------------------------------------------------------------------
unsigned char* amf_conv::format_string_as_hex(std::string str) {
  unsigned int str_len     = str.length();
  unsigned char* datavalue = (unsigned char*) malloc(str_len / 2 + 1);
  if (!datavalue) return nullptr;

  unsigned char* data = (unsigned char*) malloc(str_len + 1);
  if (!data) {
    oai::utils::utils::free_wrapper((void**) &data);
    return nullptr;
  }
  memset(data, 0, str_len + 1);
  memcpy((void*) data, (void*) str.c_str(), str_len);

  Logger::amf_app().debug("Data %s (%d bytes)", str.c_str(), str_len);
  Logger::amf_app().debug("Data (formatted):");
  for (int i = 0; i < str_len; i++) {
    char datatmp[3] = {0};
    memcpy(datatmp, &data[i], 2);
    // Ensure both characters are hexadecimal
    bool bBothDigits = true;

    for (int j = 0; j < 2; ++j) {
      if (!isxdigit(datatmp[j])) bBothDigits = false;
    }
    if (!bBothDigits) break;
    // Convert two hexadecimal characters into one character
    unsigned int nAsciiCharacter;
    sscanf(datatmp, "%x", &nAsciiCharacter);
    if (Logger::should_log(spdlog::level::debug))
      printf("%x ", nAsciiCharacter);
    // Concatenate this character onto the output
    datavalue[i / 2] = (unsigned char) nAsciiCharacter;

    // Skip the next character
    i++;
  }
  if (Logger::should_log(spdlog::level::debug)) printf("\n");

  oai::utils::utils::free_wrapper((void**) &data);
  return datavalue;
}

//------------------------------------------------------------------------------
char* amf_conv::bstring2charString(bstring b) {
  if (!b) return nullptr;
  char* buf = (char*) calloc(1, blength(b) + 1);
  if (!buf) return nullptr;
  uint8_t* value = (uint8_t*) bdata(b);
  for (int i = 0; i < blength(b); i++) buf[i] = (char) value[i];
  buf[blength(b)] = '\0';
  // oai::utils::utils::free_wrapper((void**) &value);
  value = nullptr;
  return buf;
}

//------------------------------------------------------------------------------
void amf_conv::msg_str_2_msg_hex(std::string msg, bstring& b) {
  std::string msg_hex_str = {};
  convert_string_2_hex(msg, msg_hex_str);
  Logger::amf_app().debug("Msg hex %s", msg_hex_str.c_str());
  unsigned int msg_len = msg_hex_str.length();
  char* data           = (char*) malloc(msg_len + 1);
  if (!data) {
    oai::utils::utils::free_wrapper((void**) &data);
    return;
  }

  memset(data, 0, msg_len + 1);
  memcpy((void*) data, (void*) msg_hex_str.c_str(), msg_len);

  uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
  if (!msg_hex) {
    oai::utils::utils::free_wrapper((void**) &msg_hex);
    return;
  }

  oai::utils::conv::ascii_to_hex(msg_hex, (const char*) data);
  b = blk2bstr(msg_hex, (msg_len / 2));
  oai::utils::utils::free_wrapper((void**) &data);
  oai::utils::utils::free_wrapper((void**) &msg_hex);
}

//------------------------------------------------------------------------------
void amf_conv::bstring_2_string(const bstring& b_str, std::string& str) {
  if (!b_str) return;
  auto b = bstrcpy(b_str);
  // std::string str_tmp((char*) bdata(b) , blength(b));
  str.assign((char*) bdata(b), blength(b));
}

//------------------------------------------------------------------------------
void amf_conv::string_2_bstring(const std::string& str, bstring& b_str) {
  b_str = blk2bstr(str.c_str(), str.length());
}

//------------------------------------------------------------------------------
void amf_conv::to_lower(std::string& str) {
  boost::algorithm::to_lower(str);
}

//------------------------------------------------------------------------------
void amf_conv::to_lower(bstring& b_str) {
  btolower(b_str);
}

//------------------------------------------------------------------------------
std::string amf_conv::get_ue_context_key(
    const uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id) {
  return (
      "app_ue_ranid_" + std::to_string(ran_ue_ngap_id) + ":amfid_" +
      std::to_string(amf_ue_ngap_id));
}

//------------------------------------------------------------------------------
std::string amf_conv::get_serving_network_name(
    const std::string& mnc, const std::string& mcc) {
  std::string snn = {};
  if (mnc.length() == 2)  // TODO: remove hardcoded value
    snn = "5G:mnc0" + mnc + ".mcc" + mcc + ".3gppnetwork.org";
  else
    snn = "5G:mnc" + mnc + ".mcc" + mcc + ".3gppnetwork.org";
  return snn;
}

//------------------------------------------------------------------------------
std::string amf_conv::uint32_to_hex_string_full_format(uint32_t value) {
  char hex_str[kUint32Length + 1];
  sprintf(hex_str, "%X", value);
  std::string out = std::string(hex_str);
  if (out.size() % 2 == 1) out = "0" + out;

  return ("0x" + out);
}

//------------------------------------------------------------------------------
std::string amf_conv::tmsi_to_guti(
    const std::string& mcc, const std::string& mnc, uint8_t region_id,
    const std::string& _5g_s_tmsi) {
  std::string region_id_str = {};
  int_to_string_hex(region_id, region_id_str, 2);  // Region ID: 8 bits
  return {mcc + mnc + region_id_str + _5g_s_tmsi};
}

//------------------------------------------------------------------------------
std::string amf_conv::tmsi_to_guti(
    const std::string& mcc, const std::string& mnc, uint8_t region_id,
    uint16_t amf_set_id, uint8_t amf_pointer, const std::string& tmsi) {
  uint32_t amf_id        = {};
  std::string amf_id_str = {};
  get_amf_id(region_id, amf_set_id, amf_pointer, amf_id);
  int_to_string_hex(amf_id, amf_id_str, 6);  // AMF ID: 24 bits
  return {mcc + mnc + amf_id_str + tmsi};
}

//------------------------------------------------------------------------------
std::string amf_conv::imsi_to_supi(const std::string& imsi) {
  std::string supi_type = DEFAULT_SUPI_TYPE;
  if (!supi_type.empty()) return {supi_type + "-" + imsi};
  return imsi;
}

//------------------------------------------------------------------------------
std::string amf_conv::get_imsi(
    const std::string& mcc, const std::string& mnc, const std::string& msin) {
  return {mcc + mnc + msin};
}
