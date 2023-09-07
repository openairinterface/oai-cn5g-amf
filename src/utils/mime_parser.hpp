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

#ifndef FILE_MIME_PARSER_HPP_SEEN
#define FILE_MIME_PARSER_HPP_SEEN

#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

enum class multipart_related_content_part_e { JSON = 0, NAS = 1, NGAP = 2 };

static const std::vector<std::string> multipart_related_content_part_e2str = {
    "JSON", "NAS", "NGAP"};

typedef struct mime_part {
  std::string content_type;
  std::string content_id;
  std::string body;
} mime_part;

class mime_parser {
 public:
  /*
   * Parse the input string into different Mime parts
   * @param [const std::string &] str: input string
   * @return void
   */
  bool parse(const std::string& str);

  uint8_t parse(
      std::string input, std::string& jsonData, std::string& n1sm,
      std::string& n2sm);

  /*
   * Get content of a Mime part with corresponding Content ID
   * @param [const std::string&] content_id: Content ID
   * @param [std::string&] content: Mime content
   * @return true if content with Content ID exist, otherwise false
   */
  bool get(const std::string& content_id, std::string& content);

  /*
   * Get content of a Mime part with corresponding Content ID
   * @param [const std::string&] content_id: Content ID
   * @param [std::optional<std::string>&] content: Mime content
   * @return true if content with Content ID exist, otherwise false
   */
  void get(const std::string& content_id, std::optional<std::string>& content);

  /*
   * Get vector of Mime parts
   * @param [std::vector<mime_part> &] parts: store vector of Mime parts
   * @return void
   */
  void get_mime_parts(std::unordered_map<std::string, mime_part>& parts) const;

  /*
   * Represent a string as hex
   * @param [const std::string&] str: input string
   * @return String represents string in hex format
   */
  unsigned char* format_string_as_hex(const std::string& str);

  /*
   * Create HTTP body content for multipart/related message
   * @param [std::string] body: Body of the created message
   * @param [std::string] json_part: Json part of multipart/related msg
   * @param [std::string] boundary: Boundary of multipart/related msg
   * @param [std::string] n1_message: N1 (NAS) part
   * @param [std::string] n2_message: N2 (NGAP) part
   * @return void
   */
  void create_multipart_related_content(
      std::string& body, const std::string& json_part,
      const std::string boundary, const std::string& n1_message,
      const std::string& n2_message);

  /*
   * Create HTTP body content for multipart/related message
   * @param [std::string] body: Body of the created message
   * @param [std::string] json_part: Json part of multipart/related msg
   * @param [std::string] boundary: Boundary of multipart/related msg
   * @param [std::string] message: N1 (NAS) or N2 (NGAP) part
   * @param [uint8_t] content_type: 1 for NAS content, else NGAP content
   * @return void
   */
  void create_multipart_related_content(
      std::string& body, const std::string& json_part,
      const std::string boundary, const std::string& message,
      const multipart_related_content_part_e content_type);

 private:
  std::unordered_map<std::string, mime_part> mime_parts;
};

#endif /* FILE_MIME_PARSER_HPP_SEEN */
