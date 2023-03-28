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

#include "mime_parser.hpp"

#include "conversions.hpp"
#include "output_wrapper.hpp"
#include "logger.hpp"
#include "amf.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

bool mime_parser::parse(const std::string& str) {
  Logger::amf_app().debug("Parsing the message with Simple Parser");
  Logger::amf_app().debug("%s", str.c_str());

  std::string CRLF          = "\r\n";
  std::string DOUBLE_CRLF   = "\r\n\r\n";
  std::string boundary      = "----Boundary";
  std::string boundary_full = "--" + boundary + CRLF;

std:
  size_t start_pos, boundary_pos, content_type_pos, content_id_pos, crlf_pos,
      double_crlf_pos;
  mime_part p = {};

  start_pos = str.find(boundary_full);
  if (start_pos == std::string::npos) return false;
  boundary_pos = str.find(boundary_full, start_pos + boundary_full.length());

  while (start_pos != std::string::npos) {
    p = {};
    Logger::amf_server().debug(
        "Debugging :: start_pos = %d, boundary_pos = %d", start_pos,
        boundary_pos);
    content_type_pos =
        str.find("Content-Type", start_pos + boundary_full.length());
    if (content_type_pos == std::string::npos) return false;
    crlf_pos = str.find(CRLF, content_type_pos);
    if (crlf_pos == std::string::npos) return false;
    p.content_type =
        str.find(content_type_pos + 14, crlf_pos - content_type_pos - 14);
    content_id_pos = str.find("Content-Id", start_pos);
    if (content_id_pos == std::string::npos || content_id_pos > boundary_pos) {
      if (mime_parts.count("root") != 0) {
        Logger::amf_app().debug(
            "Root Mime part already exists (Only 1 part allowed without "
            "Content-Id)");
        return false;
      } else {
        p.content_id = "root";
      }
    } else {
      crlf_pos = str.find(CRLF, content_id_pos);
      if (crlf_pos == std::string::npos) return false;
      p.content_id =
          str.substr(content_id_pos + 12, crlf_pos - content_id_pos - 12);
    }
    double_crlf_pos = str.find(DOUBLE_CRLF, start_pos + boundary_full.length());
    if (boundary_pos != std::string::npos)
      p.body = str.substr(
          double_crlf_pos + 4, boundary_pos - double_crlf_pos - 4 - 2);
    else
      p.body = str.substr(
          double_crlf_pos + 4, str.length() - double_crlf_pos - 4 - 2);
    mime_parts[p.content_id] = p;
    start_pos                = boundary_pos;
    boundary_pos = str.find(boundary_full, start_pos + boundary_full.length());
  }
  if (mime_parts.size() == 0) return false;
  return true;
}

//------------------------------------------------------------------------------
uint8_t mime_parser::parse(
    std::string input, std::string& jsonData, std::string& n1sm,
    std::string& n2sm) {
  if (!parse(input)) return 0;
  uint8_t size = mime_parts.size();
  if (mime_parts.count("root") != 0) {
    jsonData = mime_parts["root"].body;
  }
  if (mime_parts.count(N1_SM_CONTENT_ID) != 0) {
    n1sm = mime_parts[N1_SM_CONTENT_ID].body;
  }
  if (mime_parts.count(N2_SM_CONTENT_ID) != 0) {
    n2sm = mime_parts[N2_SM_CONTENT_ID].body;
  }
  return size;
}

//------------------------------------------------------------------------------
void mime_parser::get_mime_parts(
    std::unordered_map<std::string, mime_part>& parts) const {
  for (auto it : mime_parts) {
    parts[it.first] = it.second;
  }
}

//---------------------------------------------------------------------------------------------
unsigned char* mime_parser::format_string_as_hex(const std::string& str) {
  unsigned int str_len = str.length();
  char* data           = (char*) malloc(str_len + 1);
  memset(data, 0, str_len + 1);
  memcpy((void*) data, (void*) str.c_str(), str_len);

  unsigned char* data_hex = (uint8_t*) malloc(str_len / 2 + 1);
  conv::ascii_to_hex(data_hex, (const char*) data);

  Logger::amf_app().debug(
      "[Format string as Hex] Input string (%d bytes): %s ", str_len,
      str.c_str());
  output_wrapper::print_buffer(
      "amf_app", "Data (formatted):", data_hex, str_len / 2);

  // free memory
  free_wrapper((void**) &data);
  return data_hex;
}

//------------------------------------------------------------------------------
void mime_parser::create_multipart_related_content(
    std::string& body, const std::string& json_part, const std::string boundary,
    const std::string& n1_message, const std::string& n2_message) {
  // TODO: provide Content-Ids as function parameters

  // format string as hex
  unsigned char* n1_msg_hex = format_string_as_hex(n1_message);
  unsigned char* n2_msg_hex = format_string_as_hex(n2_message);

  std::string CRLF = "\r\n";
  body.append("--" + boundary + CRLF);
  body.append("Content-Type: application/json" + CRLF);
  body.append(CRLF);
  body.append(json_part + CRLF);

  body.append("--" + boundary + CRLF);
  body.append(
      "Content-Type: application/vnd.3gpp.5gnas" + CRLF +
      "Content-Id: " + N1_SM_CONTENT_ID + CRLF);
  body.append(CRLF);
  body.append(std::string((char*) n1_msg_hex, n1_message.length() / 2) + CRLF);
  body.append("--" + boundary + CRLF);
  body.append(
      "Content-Type: application/vnd.3gpp.ngap" + CRLF +
      "Content-Id: " + N2_SM_CONTENT_ID + CRLF);
  body.append(CRLF);
  body.append(std::string((char*) n2_msg_hex, n2_message.length() / 2) + CRLF);
  body.append("--" + boundary + "--" + CRLF);
}

//------------------------------------------------------------------------------
void mime_parser::create_multipart_related_content(
    std::string& body, const std::string& json_part, const std::string boundary,
    const std::string& message,
    const multipart_related_content_part_e content_type) {
  // TODO: provide Content-Id as function parameters
  // format string as hex
  unsigned char* msg_hex = format_string_as_hex(message);

  std::string CRLF = "\r\n";
  body.append("--" + boundary + CRLF);
  body.append("Content-Type: application/json" + CRLF);
  body.append(CRLF);
  body.append(json_part + CRLF);

  body.append("--" + boundary + CRLF);
  if (content_type == multipart_related_content_part_e::NAS) {  // NAS
    body.append(
        "Content-Type: application/vnd.3gpp.5gnas" + CRLF +
        "Content-Id: " + N1_SM_CONTENT_ID + CRLF);
  } else if (content_type == multipart_related_content_part_e::NGAP) {  // NGAP
    body.append(
        "Content-Type: application/vnd.3gpp.ngap" + CRLF +
        "Content-Id: " + N2_SM_CONTENT_ID + CRLF);
  }
  body.append(CRLF);
  body.append(std::string((char*) msg_hex, message.length() / 2) + CRLF);
  body.append("--" + boundary + "--" + CRLF);
}
