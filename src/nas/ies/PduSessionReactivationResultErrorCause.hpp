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

#ifndef _PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_H_
#define _PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_H_

#include "Type6NasIe.hpp"

constexpr uint8_t kPduSessionReactivationResultErrorCauseMinimumLength  = 5;
constexpr uint32_t kPduSessionReactivationResultErrorCauseMaximumLength = 515;
constexpr auto kPduSessionReactivationResultErrorCauseIeName =
    "PDU Session Reactivation Result Error Cause";

namespace nas {

class PduSessionReactivationResultErrorCause : public Type6NasIe {
 public:
  PduSessionReactivationResultErrorCause();
  PduSessionReactivationResultErrorCause(uint8_t session_id, uint8_t value);
  ~PduSessionReactivationResultErrorCause();

  static std::string GetIeName() {
    return kPduSessionReactivationResultErrorCauseIeName;
  }

  void setValue(uint8_t session_id, uint8_t cause);
  std::pair<uint8_t, uint8_t> getValue() const;

  void setValue(const std::vector<std::pair<uint8_t, uint8_t>>& value);
  void getValue(std::vector<std::pair<uint8_t, uint8_t>>& value) const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);

 private:
  std::vector<std::pair<uint8_t, uint8_t>> pdu_session_id_cause_value_pair;
};

}  // namespace nas

#endif
