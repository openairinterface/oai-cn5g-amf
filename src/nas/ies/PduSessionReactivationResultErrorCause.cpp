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

#include "PduSessionReactivationResultErrorCause.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
PduSessionReactivationResultErrorCause::PduSessionReactivationResultErrorCause()
    : Type6NasIe(kIeiPduSessionReactivationResultErrorCause),
      pdu_session_id_cause_value_pair() {
  std::pair<uint8_t, uint8_t> value = std::make_pair<uint8_t, uint8_t>(0, 0);
  pdu_session_id_cause_value_pair.push_back(value);
  SetLengthIndicator(
      2);  // Minimum length with 1 pair (PDU Session ID, Cause Value)
}

//------------------------------------------------------------------------------
PduSessionReactivationResultErrorCause::PduSessionReactivationResultErrorCause(
    uint8_t session_id, uint8_t cause)
    : Type6NasIe(kIeiPduSessionReactivationResultErrorCause) {
  std::pair<uint8_t, uint8_t> value = std::make_pair(session_id, cause);
  pdu_session_id_cause_value_pair.push_back(value);
  SetLengthIndicator(
      2);  // Minimum length with 1 pair (PDU Session ID, Cause Value)
}

//------------------------------------------------------------------------------
PduSessionReactivationResultErrorCause::
    ~PduSessionReactivationResultErrorCause() {}

//------------------------------------------------------------------------------
void PduSessionReactivationResultErrorCause::setValue(
    uint8_t session_id, uint8_t cause) {
  std::pair<uint8_t, uint8_t> value = std::make_pair(session_id, cause);
  pdu_session_id_cause_value_pair.clear();
  pdu_session_id_cause_value_pair.push_back(value);
}

//------------------------------------------------------------------------------
void PduSessionReactivationResultErrorCause::setValue(
    const std::vector<std::pair<uint8_t, uint8_t>>& value) {
  if (value.size() > 0) pdu_session_id_cause_value_pair = value;
  SetLengthIndicator(value.size() * 2);
}

//------------------------------------------------------------------------------
std::pair<uint8_t, uint8_t> PduSessionReactivationResultErrorCause::getValue()
    const {
  if (pdu_session_id_cause_value_pair.size() > 0) {
    return pdu_session_id_cause_value_pair[0];
  }
  return std::pair<uint8_t, uint8_t>(0, 0);
}

//------------------------------------------------------------------------------
int PduSessionReactivationResultErrorCause::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length (later)
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // PDU Session ID/Cause Value
  for (const auto& i : pdu_session_id_cause_value_pair) {
    ENCODE_U8(buf + encoded_size, i.first, encoded_size);
    ENCODE_U8(buf + encoded_size, i.second, encoded_size);
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PduSessionReactivationResultErrorCause::Decode(
    uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  // Decode PDU Session ID/Cause Value
  pdu_session_id_cause_value_pair.clear();
  while (decoded_size < ie_len - 2) {
    uint8_t pdu_session_id = {};
    uint8_t cause_value    = {};
    DECODE_U8(buf + decoded_size, pdu_session_id, decoded_size);
    DECODE_U8(buf + decoded_size, cause_value, decoded_size);
    std::pair<uint8_t, uint8_t> value =
        std::make_pair(pdu_session_id, cause_value);
    pdu_session_id_cause_value_pair.push_back(value);
  }

  for (const auto& i : pdu_session_id_cause_value_pair) {
    Logger::nas_mm().debug(
        "PDU Session ID 0x%x, Cause Value", i.first, i.second);
  }

  Logger::nas_mm().debug(
      "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
