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

#include "amf_sbi_helper.hpp"

#include <boost/algorithm/string.hpp>
#include <regex>
#include <vector>

#include "ProblemDetails.h"
#include "logger.hpp"

namespace oai::amf::api {
//------------------------------------------------------------------------------
void amf_sbi_helper::set_problem_details(
    nlohmann::json& json_data, const std::string& detail) {
  Logger::amf_app().error("%s", detail);
  oai::model::common::ProblemDetails problem_details;
  problem_details.setDetail(detail);
  to_json(json_data, problem_details);
}
}  // namespace oai::amf::api
