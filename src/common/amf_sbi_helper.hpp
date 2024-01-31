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

#ifndef _AMF_SBI_HELPER_HPP
#define _AMF_SBI_HELPER_HPP

#include <nlohmann/json.hpp>

#include "amf_config.hpp"
#include "sbi_helper.hpp"

using namespace oai::config;
using namespace oai::common::sbi;

extern amf_config amf_cfg;

namespace oai::amf::api {

class amf_sbi_helper : public sbi_helper {
 public:
  static inline const std::string AmfCommunicationServiceBase =
      sbi_helper::AmfCommBase +
      amf_cfg.sbi.api_version.value_or(kDefaultSbiApiVersion);

  static inline const std::string AmfEventExposureServiceBase =
      sbi_helper::AmfEvtsBase +
      amf_cfg.sbi.api_version.value_or(kDefaultSbiApiVersion);

  static inline const std::string AmfStatusNotifyServiceBase =
      sbi_helper::AmfStatusNotifBase +
      amf_cfg.sbi.api_version.value_or(kDefaultSbiApiVersion);

  static inline const std::string AmfConfigurationServiceBase =
      sbi_helper::AmfConfBase +
      amf_cfg.sbi.api_version.value_or(kDefaultSbiApiVersion);

  static inline const std::string AmfLocationServiceBase =
      sbi_helper::AmflocBase +
      amf_cfg.sbi.api_version.value_or(kDefaultSbiApiVersion);

  static inline const std::string AmfCommPathN1MessageNotify =
      "n1-message-notify";
  static inline const std::string AmfCommPathN1N2Messages = "n1-n2-messages";

  static void set_problem_details(
      nlohmann::json& json_data, const std::string& detail);
};

}  // namespace oai::amf::api

#endif
