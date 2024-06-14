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

#include <boost/algorithm/string.hpp>

#include "amf.hpp"
#include "conversions.hpp"
#include "logger.hpp"
#include "output_wrapper.hpp"
#include "utils.hpp"

bool mime_parser::parse(const std::string& str) {
  std::string CRLF = "\r\n";
  Logger::amf_app().debug("Parsing the message with the Simple Parser");

  // find boundary
  std::size_t content_type_pos = str.find("Content-Type");  // first part
  if ((content_type_pos <= 4) or (content_type_pos == std::string::npos))
    return false;

  std::string boundary_str =
      str.substr(2, content_type_pos - 4);  // 2 for -- and 2 for CRLF
  Logger::amf_app().debug("Boundary: %s", boundary_str.c_str());
  std::string boundary_full = "--" + boundary_str + CRLF;
  std::string last_boundary = "--" + boundary_str + "--" + CRLF;

  std::size_t crlf_pos           = str.find(CRLF, content_type_pos);
  std::size_t boundary_pos       = str.find(boundary_full);
  std::size_t boundary_last_post = str.find(last_boundary);

  while (boundary_pos < boundary_last_post) {
    mime_part p      = {};
    content_type_pos = str.find("Content-Type", boundary_pos);
    crlf_pos         = str.find(CRLF, content_type_pos);
    if ((content_type_pos == std::string::npos) or
        (crlf_pos == std::string::npos))
      break;
    p.content_type =
        str.substr(content_type_pos + 14, crlf_pos - (content_type_pos + 14));
    Logger::amf_app().debug("Content Type: %s", p.content_type.c_str());

    // if (p.content_type.compare("application/json") == 0) {
    if (boost::iequals(p.content_type, "application/json") or
        boost::iequals(p.content_type, "application/problem+json")) {
      p.content_id = JSON_CONTENT_ID_MIME;
      crlf_pos =
          str.find(CRLF + CRLF, content_type_pos);  // beginning of content
    } else {
      std::size_t content_id_pos = str.find("Content-Id", content_type_pos);

      if ((content_id_pos == std::string::npos)) {
        return false;
      } else {
        crlf_pos = str.find(CRLF, content_id_pos);
        if (crlf_pos == std::string::npos) return false;
        p.content_id =
            str.substr(content_id_pos + 12, crlf_pos - content_id_pos - 12);
        crlf_pos =
            str.find(CRLF + CRLF, content_id_pos);  // beginning of content
      }
    }

    Logger::amf_app().debug("Content Id: %s", p.content_id.c_str());

    boundary_pos = str.find(boundary_full, crlf_pos);
    if (boundary_pos == std::string::npos) {
      boundary_pos = str.find(last_boundary, crlf_pos);
    }
    if (boundary_pos > 0) {
      p.body = str.substr(crlf_pos + 4, boundary_pos - 2 - (crlf_pos + 4));
      Logger::amf_app().debug("Body: %s", p.body.c_str());
      mime_parts[p.content_id] = p;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
uint8_t mime_parser::parse(
    std::string input, std::string& jsonData, std::string& n1sm,
    std::string& n2sm) {
  if (!parse(input)) return 0;
  uint8_t size = mime_parts.size();
  if (mime_parts.count(JSON_CONTENT_ID_MIME) != 0) {
    jsonData = mime_parts[JSON_CONTENT_ID_MIME].body;
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
bool mime_parser::get(const std::string& content_id, std::string& content) {
  if (mime_parts.count(content_id) != 0) {
    content = mime_parts[content_id].body;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void mime_parser::get(
    const std::string& content_id, std::optional<std::string>& content) {
  if (mime_parts.count(content_id) != 0) {
    content = std::make_optional<std::string>(mime_parts[content_id].body);
  }
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
  oai::utils::conv::ascii_to_hex(data_hex, (const char*) data);

  Logger::amf_app().debug(
      "[Format string as Hex] Input string (%d bytes): %s ", str_len,
      str.c_str());
  oai::utils::output_wrapper::print_buffer(
      "amf_app", "Data (formatted):", data_hex, str_len / 2);

  // free memory
  oai::utils::utils::free_wrapper((void**) &data);
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
