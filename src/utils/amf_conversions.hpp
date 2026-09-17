/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_AMF_CONVERSIONS_HPP_SEEN
#define FILE_AMF_CONVERSIONS_HPP_SEEN

#include <netinet/in.h>
#include <stdint.h>

#include <iostream>
#include <string>

#include "bstrlib.h"
#include "conversions.hpp"
#include "Struct.hpp"
#include "utils.hpp"

extern "C" {
#include "BIT_STRING.h"
#include "OCTET_STRING.h"
}

class amf_conv : public oai::utils::conv {
 public:
  static void msg_str_2_msg_hex(std::string msg, bstring& b);
  static void octet_stream_2_hex_stream_bis(
      uint8_t* buf, int len, std::string& out);
  static char* bstring2charString(bstring b);
  static unsigned char* format_string_as_hex(std::string str);
  // TODO: bitstring_2_int32
  static void to_lower(std::string& str);
  static void to_lower(bstring& b_str);
  static std::string get_serving_network_name(
      const std::string& mnc, const std::string& mcc);
  static std::string uint32_to_hex_string_full_format(uint32_t value);
  static std::string suci_to_supi(const oai::nas::SUCI_imsi_t& suci);
  static std::string imsi_to_supi(const std::string& imsi);
  static std::string supi_to_imsi(const std::string& supi);
  static std::string get_imsi(
      const std::string& mcc, const std::string& mnc, const std::string& msin);
  static void octet_stream_2_hex_stream(
      uint8_t* buf, int len, std::string& out);
};
#endif /* FILE_CONVERSIONS_HPP_SEEN */
