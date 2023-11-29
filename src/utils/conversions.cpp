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

#include "conversions.hpp"

#include <arpa/inet.h>
#include <ctype.h>
#include <inttypes.h>
#include <iomanip>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include "amf.hpp"
#include "logger.hpp"
#include "output_wrapper.hpp"

constexpr uint8_t kUint32Length =
    8;  // 4 bytes  -8 characters representation in hex

static const char hex_to_ascii_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

static const signed char ascii_to_hex_table[0x100] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

//------------------------------------------------------------------------------
int conv::ascii_to_hex(uint8_t* dst, const char* h) {
  const unsigned char* hex = (const unsigned char*) h;
  unsigned i               = 0;

  for (;;) {
    int high, low;

    while (*hex && isspace(*hex)) hex++;

    if (!*hex) return 1;

    high = ascii_to_hex_table[*hex++];

    if (high < 0) return 0;

    while (*hex && isspace(*hex)) hex++;

    if (!*hex) return 0;

    low = ascii_to_hex_table[*hex++];

    if (low < 0) return 0;

    dst[i++] = (high << 4) | low;
  }
}

//------------------------------------------------------------------------------
std::string conv::mccToString(
    const uint8_t digit1, const uint8_t digit2, const uint8_t digit3) {
  std::string s  = {};
  uint16_t mcc16 = digit1 * 100 + digit2 * 10 + digit3;
  // s.append(std::to_string(digit1)).append(std::to_string(digit2)).append(std::to_string(digit3));
  s.append(std::to_string(mcc16));
  return s;
}

//------------------------------------------------------------------------------
std::string conv::mncToString(
    const uint8_t digit1, const uint8_t digit2, const uint8_t digit3) {
  std::string s  = {};
  uint16_t mcc16 = 0;

  if (digit3 == 0x0F) {
    mcc16 = digit1 * 10 + digit2;
  } else {
    mcc16 = digit1 * 100 + digit2 * 10 + digit3;
  }
  s.append(std::to_string(mcc16));
  return s;
}

//------------------------------------------------------------------------------
std::string conv::tmsi_to_string(const uint32_t tmsi) {
  std::string s        = {};
  std::string tmsi_str = uint32_to_hex_string(tmsi);
  uint8_t length       = kUint32Length - tmsi_str.size();
  for (uint8_t i = 0; i < length; i++) {
    s.append("0");
  }
  s.append(std::to_string(tmsi));
  return s;
}

//------------------------------------------------------------------------------
struct in_addr conv::fromString(const std::string addr4) {
  unsigned char buf[sizeof(struct in6_addr)] = {};
  int s              = inet_pton(AF_INET, addr4.c_str(), buf);
  struct in_addr* ia = (struct in_addr*) buf;
  return *ia;
}

//------------------------------------------------------------------------------
struct in6_addr conv::fromStringV6(const std::string& addr6) {
  unsigned char buf[sizeof(struct in6_addr)] = {};
  struct in6_addr ipv6_addr {};
  if (inet_pton(AF_INET6, addr6.c_str(), buf) == 1) {
    memcpy(&ipv6_addr, buf, sizeof(struct in6_addr));
  }
  return ipv6_addr;
}

//------------------------------------------------------------------------------
std::string conv::toString(const struct in_addr& inaddr) {
  std::string s              = {};
  char str[INET6_ADDRSTRLEN] = {};
  if (inet_ntop(AF_INET, (const void*) &inaddr, str, INET6_ADDRSTRLEN) ==
      NULL) {
    s.append("Error in_addr");
  } else {
    s.append(str);
  }
  return s;
}

//------------------------------------------------------------------------------
std::string conv::toString(const struct in6_addr& in6addr) {
  std::string s              = {};
  char str[INET6_ADDRSTRLEN] = {};
  if (inet_ntop(AF_INET6, (const void*) &in6addr, str, INET6_ADDRSTRLEN) ==
      nullptr) {
    s.append("Error in6_addr");
  } else {
    s.append(str);
  }
  return s;
}

//------------------------------------------------------------------------------
void conv::convert_string_2_hex(
    std::string& input_str, std::string& output_str) {
  unsigned char* data = (unsigned char*) malloc(input_str.length() + 1);
  if (!data) {
    free_wrapper((void**) &data);
    return;
  }
  memset(data, 0, input_str.length() + 1);
  memcpy((void*) data, (void*) input_str.c_str(), input_str.length());
  output_wrapper::print_buffer(
      "amf_app", "Data input", data, input_str.length());

  char* datahex = (char*) malloc(input_str.length() * 2 + 1);
  if (!datahex) {
    free_wrapper((void**) &datahex);
    free_wrapper((void**) &data);
    return;
  }
  memset(datahex, 0, input_str.length() * 2 + 1);

  for (int i = 0; i < input_str.length(); i++)
    sprintf(datahex + i * 2, "%02x", data[i]);

  output_str = reinterpret_cast<char*>(datahex);
  free_wrapper((void**) &datahex);
  free_wrapper((void**) &data);
}

//------------------------------------------------------------------------------
unsigned char* conv::format_string_as_hex(std::string str) {
  unsigned int str_len     = str.length();
  unsigned char* datavalue = (unsigned char*) malloc(str_len / 2 + 1);
  if (!datavalue) return nullptr;

  unsigned char* data = (unsigned char*) malloc(str_len + 1);
  if (!data) {
    free_wrapper((void**) &data);
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

  free_wrapper((void**) &data);
  return datavalue;
}

//------------------------------------------------------------------------------
char* conv::bstring2charString(bstring b) {
  if (!b) return nullptr;
  char* buf = (char*) calloc(1, blength(b) + 1);
  if (!buf) return nullptr;
  uint8_t* value = (uint8_t*) bdata(b);
  for (int i = 0; i < blength(b); i++) buf[i] = (char) value[i];
  buf[blength(b)] = '\0';
  // free_wrapper((void**) &value);
  value = nullptr;
  return buf;
}

//------------------------------------------------------------------------------
void conv::msg_str_2_msg_hex(std::string msg, bstring& b) {
  std::string msg_hex_str = {};
  convert_string_2_hex(msg, msg_hex_str);
  Logger::amf_app().debug("Msg hex %s", msg_hex_str.c_str());
  unsigned int msg_len = msg_hex_str.length();
  char* data           = (char*) malloc(msg_len + 1);
  if (!data) {
    free_wrapper((void**) &data);
    return;
  }

  memset(data, 0, msg_len + 1);
  memcpy((void*) data, (void*) msg_hex_str.c_str(), msg_len);

  uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
  if (!msg_hex) {
    free_wrapper((void**) &msg_hex);
    return;
  }

  conv::ascii_to_hex(msg_hex, (const char*) data);
  b = blk2bstr(msg_hex, (msg_len / 2));
  free_wrapper((void**) &data);
  free_wrapper((void**) &msg_hex);
}

//------------------------------------------------------------------------------
bool conv::octet_string_2_bstring(
    const OCTET_STRING_t& octet_str, bstring& b_str) {
  if (!octet_str.buf or (octet_str.size == 0)) return false;
  b_str = blk2bstr(octet_str.buf, octet_str.size);
  return true;
}

//------------------------------------------------------------------------------
bool conv::bstring_2_octet_string(
    const bstring& b_str, OCTET_STRING_t& octet_str) {
  if (!b_str) return false;
  OCTET_STRING_fromBuf(&octet_str, (char*) bdata(b_str), blength(b_str));
  return true;
}

//------------------------------------------------------------------------------
bool conv::octet_string_2_bit_string(
    const OCTET_STRING_t& octet_str, BIT_STRING_t& bit_str,
    const uint8_t& bits_unused) {
  if (!check_octet_string(octet_str)) return false;

  bit_str.buf = (uint8_t*) calloc(octet_str.size + 1, sizeof(uint8_t));
  if (!bit_str.buf) return false;

  memcpy(bit_str.buf, octet_str.buf, octet_str.size);
  ((uint8_t*) bit_str.buf)[octet_str.size] = '\0';
  bit_str.bits_unused                      = bits_unused;

  return true;
}

//------------------------------------------------------------------------------
bool conv::bstring_2_bit_string(const bstring& b_str, BIT_STRING_t& bit_str) {
  int size = blength(b_str);
  if (!b_str or size <= 0) return false;
  if (!bdata(b_str)) return false;

  bit_str.buf = (uint8_t*) calloc(size + 1, sizeof(uint8_t));
  if (!bit_str.buf) return false;

  if (check_bstring(b_str))
    memcpy((void*) bit_str.buf, (char*) b_str->data, blength(b_str));
  ((uint8_t*) bit_str.buf)[size] = '\0';
  bit_str.size                   = size;
  bit_str.bits_unused            = 0;

  return true;
}

//------------------------------------------------------------------------------
bool conv::sd_string_to_int(const std::string& sd_str, uint32_t& sd) {
  sd = SD_NO_VALUE;
  if (sd_str.empty()) return false;
  uint8_t base = 10;
  try {
    if (sd_str.size() > 2) {
      if (boost::iequals(sd_str.substr(0, 2), "0x")) {
        base = 16;
      }
    }
    sd = std::stoul(sd_str, nullptr, base);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int for S-NSSAI SD, error: %s",
        e.what());
    sd = SD_NO_VALUE;
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool conv::sd_string_hex_to_int(const std::string& sd_str, uint32_t& sd) {
  sd = SD_NO_VALUE;
  if (sd_str.empty()) return false;
  uint8_t base = 16;
  try {
    sd = std::stoul(sd_str, nullptr, base);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int for S-NSSAI SD, error: %s",
        e.what());
    sd = SD_NO_VALUE;
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void conv::sd_int_to_string_hex(uint32_t sd, std::string& sd_str) {
  std::stringstream stream_str;
  stream_str << std::hex << sd;
  std::string sd_tmp(stream_str.str());
  sd_str = sd_tmp;
}
//------------------------------------------------------------------------------
bool conv::string_to_int(
    const std::string& str, uint32_t& value, const uint8_t& base) {
  if (str.empty()) return false;
  if ((base != 10) or (base != 16)) {
    Logger::amf_app().warn("Only support Dec or Hex string value");
    return false;
  }
  if (base == 16) {
    if (str.size() <= 2) return false;
    if (!boost::iequals(str.substr(0, 2), "0x")) return false;
  }
  try {
    value = std::stoul(str, nullptr, base);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int, error: %s", e.what());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool conv::string_to_int8(const std::string& str, uint8_t& value) {
  if (str.empty()) return false;
  try {
    value = (uint8_t) std::stoi(str);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int, error: %s", e.what());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool conv::string_to_int32(const std::string& str, uint32_t& value) {
  if (str.empty()) return false;
  try {
    value = (uint32_t) std::stoi(str);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int, error: %s", e.what());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void conv::int_to_string_hex(
    uint32_t value, std::string& value_str, uint8_t length) {
  std::stringstream stream_str;
  if (length > 0) {
    stream_str << std::setfill('0') << std::setw(length) << std::hex << value;
  } else {
    stream_str << std::hex << value;
  }

  std::string value_tmp(stream_str.str());
  value_str = value_tmp;
}

//------------------------------------------------------------------------------
bool conv::string_hex_to_int(const std::string& value_str, uint32_t& value) {
  if (value_str.empty()) return false;
  uint8_t base = 16;
  try {
    value = std::stoul(value_str, nullptr, base);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int, error: %s", e.what());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
uint32_t conv::string_hex_to_int(const std::string& value_str) {
  uint32_t value = {};
  if (value_str.empty()) return value;
  uint8_t base = 16;
  try {
    value = std::stoul(value_str, nullptr, base);
  } catch (const std::exception& e) {
    Logger::amf_app().error(
        "Error when converting from string to int, error: %s", e.what());
    value = {};
  }
  return value;
}

//------------------------------------------------------------------------------
void conv::bstring_2_string(const bstring& b_str, std::string& str) {
  if (!b_str) return;
  auto b = bstrcpy(b_str);
  // std::string str_tmp((char*) bdata(b) , blength(b));
  str.assign((char*) bdata(b), blength(b));
}

//------------------------------------------------------------------------------
void conv::string_2_bstring(const std::string& str, bstring& b_str) {
  b_str = blk2bstr(str.c_str(), str.length());
}

//------------------------------------------------------------------------------
void conv::octet_string_2_string(
    const OCTET_STRING_t& octet_str, std::string& str) {
  if (!octet_str.buf or (octet_str.size == 0)) return;
  // std::string str_tmp((char *) octet_str.buf , octet_str.size);
  str.assign((char*) octet_str.buf, octet_str.size);
}

//------------------------------------------------------------------------------
void conv::string_2_octet_string(
    const std::string& str, OCTET_STRING_t& o_str) {
  o_str.buf = (uint8_t*) calloc(1, str.length() + 1);
  if (!o_str.buf) return;
  // o_str.buf = strcpy(new char[str.length() + 1], str.c_str());
  // memcpy(o_str.buf, str.c_str(), str.size());
  std::copy(str.begin(), str.end(), o_str.buf);
  o_str.size              = str.length();
  o_str.buf[str.length()] = '\0';  // just in case
}

//------------------------------------------------------------------------------
bool conv::int8_2_octet_string(const uint8_t& value, OCTET_STRING_t& o_str) {
  o_str.buf = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!o_str.buf) return false;
  o_str.size   = 1;
  o_str.buf[0] = value;
  return true;
}

//------------------------------------------------------------------------------
bool conv::octet_string_2_int8(const OCTET_STRING_t& o_str, uint8_t& value) {
  if (o_str.size != 1) return false;
  value = o_str.buf[0];
  return true;
}

//------------------------------------------------------------------------------
bool conv::octet_string_copy(
    OCTET_STRING_t& destination, const OCTET_STRING_t& source) {
  if (!source.buf) return false;
  OCTET_STRING_fromBuf(&destination, (char*) source.buf, source.size);
  return true;
}

//------------------------------------------------------------------------------
void conv::to_lower(std::string& str) {
  boost::algorithm::to_lower(str);
}

//------------------------------------------------------------------------------
void conv::to_lower(bstring& b_str) {
  btolower(b_str);
}
//------------------------------------------------------------------------------
bool conv::check_bstring(const bstring& b_str) {
  if (b_str == nullptr || b_str->slen < 0 || b_str->data == nullptr)
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool conv::check_octet_string(const OCTET_STRING_t& octet_str) {
  if (!octet_str.buf or (octet_str.size == 0)) return false;
  return true;
}

//------------------------------------------------------------------------------
std::string conv::get_ue_context_key(
    const uint32_t ran_ue_ngap_id, long amf_ue_ngap_id) {
  return (
      "app_ue_ranid_" + std::to_string(ran_ue_ngap_id) + ":amfid_" +
      std::to_string(amf_ue_ngap_id));
}

//------------------------------------------------------------------------------
std::string conv::get_serving_network_name(
    const std::string& mnc, const std::string& mcc) {
  std::string snn = {};
  if (mnc.length() == 2)  // TODO: remove hardcoded value
    snn = "5G:mnc0" + mnc + ".mcc" + mcc + ".3gppnetwork.org";
  else
    snn = "5G:mnc" + mnc + ".mcc" + mcc + ".3gppnetwork.org";
  return snn;
}

//------------------------------------------------------------------------------
std::string conv::uint32_to_hex_string(uint32_t value) {
  char hex_str[kUint32Length + 1];
  sprintf(hex_str, "%X", value);
  return std::string(hex_str);
}

//------------------------------------------------------------------------------
std::string conv::tmsi_to_guti(
    const std::string& mcc, const std::string& mnc, uint8_t region_id,
    const std::string& _5g_s_tmsi) {
  std::string region_id_str = {};
  int_to_string_hex(region_id, region_id_str);
  return {mcc + mnc + region_id_str + _5g_s_tmsi};
}

//------------------------------------------------------------------------------
std::string conv::tmsi_to_guti(
    const std::string& mcc, const std::string& mnc, uint8_t region_id,
    uint16_t amf_set_id, uint8_t amf_pointer, const std::string& tmsi) {
  uint32_t amf_id        = {};
  std::string amf_id_str = {};
  get_amf_id(region_id, amf_set_id, amf_pointer, amf_id);
  int_to_string_hex(amf_id, amf_id_str);
  return {mcc + mnc + amf_id_str + tmsi};
}
//------------------------------------------------------------------------------
std::string conv::imsi_to_supi(const std::string& imsi) {
  std::string supi_type = DEFAULT_SUPI_TYPE;
  if (!supi_type.empty()) return {supi_type + "-" + imsi};
  return imsi;
}

//------------------------------------------------------------------------------
std::string conv::get_imsi(
    const std::string& mcc, const std::string& mnc, const std::string& msin) {
  return {mcc + mnc + msin};
}

//------------------------------------------------------------------------------
bool conv::string_2_masked_imeisv(
    const std::string& imeisv_str, BIT_STRING_t& imeisv) {
  int len = imeisv_str.length();
  if (len != 16) return false;  // Must contain 16 digits

  imeisv.buf = (uint8_t*) calloc(8, sizeof(uint8_t));
  if (!imeisv.buf) {
    return false;
  }

  uint8_t digit_low  = 0;
  uint8_t digit_high = 0;

  int i = 0;
  int j = 0;
  while (i < len) {
    string_to_int8(imeisv_str.substr(i, 1), digit_low);
    string_to_int8(imeisv_str.substr(i + 1, 1), digit_high);
    i             = i + 2;
    uint8_t octet = (0xf0 & (digit_high << 4)) | (digit_low & 0x0f);
    imeisv.buf[j] = octet;
    j++;
  }
  // last 4 digits of the SNR masked by setting the corresponding bits to 1
  imeisv.buf[5] = 0xff;
  imeisv.buf[6] = 0xff;

  imeisv.size        = 8;
  imeisv.bits_unused = 0;
  return true;
}

//------------------------------------------------------------------------------
void conv::get_amf_id(
    uint8_t amf_region_id, uint16_t amf_set_id, uint8_t amf_pointer,
    uint32_t& amf_id) {
  // AMF Region ID: 8bits
  // AMF Set ID: 10 bits
  // AMF Pointer: 6 bits
  amf_id = 0x00ffffff & ((amf_region_id << 16) | ((amf_set_id & 0x03ff) << 6) |
                         (amf_pointer & 0x3f));
}
//------------------------------------------------------------------------------
void conv::get_amf_id(
    const std::string& amf_region_id, const std::string& amf_set_id,
    const std::string& amf_pointer, uint32_t& amf_id) {
  uint8_t amf_region_id_int = {};
  uint16_t amf_set_id_int   = {};
  uint8_t amf_pointer_int   = {};

  get_amf_id(
      string_hex_to_int(amf_region_id), string_hex_to_int(amf_set_id),
      string_hex_to_int(amf_pointer), amf_id);
}
