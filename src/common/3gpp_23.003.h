/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#ifndef FILE_3GPP_23_003_SEEN
#define FILE_3GPP_23_003_SEEN

#include <stdint.h>
#include <string>

#include <nlohmann/json.hpp>

const uint32_t SD_NO_VALUE               = 0xFFFFFF;
const uint8_t SST_MAX_STANDARDIZED_VALUE = 127;

const uint8_t SST_LENGTH = 1;
const uint8_t SD_LENGTH  = 3;

typedef struct s_nssai  // section 28.4, TS23.003
{
  uint8_t sST;
  // uint32_t sD:24;
  std::string sD;
  // s_nssai(const uint8_t& sst,  const uint32_t sd) : sST(sst), sD(sd) {}
  s_nssai(const uint8_t& sst, const std::string sd) : sST(sst), sD(sd) {}
  s_nssai() : sST(), sD() {}
  s_nssai(const s_nssai& p) : sST(p.sST), sD(p.sD) {}
  bool operator==(const struct s_nssai& s) const {
    if ((s.sST == this->sST) && (s.sD.compare(this->sD) == 0)) {
      return true;
    } else {
      return false;
    }
  }
} snssai_t;

typedef struct plmn_s {
  std::string mcc;
  std::string mnc;
} plmn_t;

#define INVALID_TAC_0000 (uint16_t) 0x0000
#define INVALID_TAC_FFFE (uint16_t) 0xFFFE
#define INVALID_TAC (uint32_t) 0x00000000

#define INVALID_TMSI                                                           \
  UINT32_MAX /*!< \brief  The network shall not allocate a TMSI with all 32    \
                bits equal to 1 (this is because the TMSI must be stored in    \
                the SIM, and the SIM uses 4 octets with all bits               \
                            equal to 1 to indicate that no valid TMSI is       \
                available).  */
typedef uint16_t tac_t;
typedef struct tai_s {
  plmn_t plmn; /*!< \brief  <MCC> + <MNC>        */
  tac_t tac;   /*!< \brief  Tracking Area Code   */
} tai_t;

typedef struct guami_s {
  plmn_t plmn;
  std::string amf_id;
} guami_t;

typedef struct guami_full_format_s {
  std::string mcc;
  std::string mnc;
  uint8_t region_id;
  uint16_t amf_set_id;
  uint8_t amf_pointer;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["mcc"]         = this->mcc;
    json_data["mnc"]         = this->mnc;
    json_data["region_id"]   = this->region_id;
    json_data["amf_set_id"]  = this->amf_set_id;
    json_data["amf_pointer"] = this->amf_pointer;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mcc         = json_data["mcc"].get<std::string>();
    this->mnc         = json_data["mnc"].get<std::string>();
    this->region_id   = json_data["region_id"].get<int>();
    this->amf_set_id  = json_data["amf_set_id"].get<int>();
    this->amf_pointer = json_data["amf_pointer"].get<int>();
  }
} guami_full_format_t;

#endif
