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
#include "_5gsTrackingAreaIdList.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "utils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsTrackingAreaIdList::_5gsTrackingAreaIdList()
    : Type4NasIe(kIei5gsTrackingAreaIdentityList), m_tai_list() {
  SetLengthIndicator(
      k5gsTrackingAreaIdListMinimumLength -
      2);  // Minimim length - 2 bytes for header
}

//------------------------------------------------------------------------------
_5gsTrackingAreaIdList::_5gsTrackingAreaIdList(bool iei)
    : Type4NasIe(), m_tai_list() {
  if (iei) {
    SetIei(kIei5gsTrackingAreaIdentityList);
  }
  SetLengthIndicator(
      k5gsTrackingAreaIdListMinimumLength -
      2);  // Minimim length - 2 bytes for header
}

//------------------------------------------------------------------------------
_5gsTrackingAreaIdList::_5gsTrackingAreaIdList(
    const std::vector<p_tai_t>& tai_list)
    : Type4NasIe(kIei5gsTrackingAreaIdentityList) {
  m_tai_list = tai_list;
  // Don't know Length Indicator for now
}

//------------------------------------------------------------------------------
int _5gsTrackingAreaIdList::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int len_pos = 0;
  int encoded_header_size =
      Type4NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  for (int i = 0; i < m_tai_list.size(); i++) {
    int item_len = 0;
    switch (m_tai_list[i].type) {
      case 0x00: {
        int encode_00_type_size = encode_00_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
        if (encode_00_type_size == KEncodeDecodeError) break;
        item_len += encode_00_type_size;
      } break;
      case 0x01: {
        int encode_01_type_size = encode_01_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);

        if (encode_01_type_size == KEncodeDecodeError) break;
        item_len += encode_01_type_size;
      } break;
      case 0x10: {
        int encode_10_type_size = encode_10_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
        if (encode_10_type_size == KEncodeDecodeError) break;
        item_len += encode_10_type_size;
      } break;
    }
    encoded_size += item_len;
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U8(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsTrackingAreaIdList::encode_00_type(
    p_tai_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  // Type of list/Number of elements
  uint8_t octet = 0x00 | ((item.tac_list.size() - 1) &
                          0x1f);  // see Table 9.11.3.9.1@3GPP TS 24.501 V16.1.0
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  // Encode PLMN
  encoded_size += utils::encodeMccMnc2Buffer(
      item.plmn_list[0].mcc, item.plmn_list[0].mnc, buf + encoded_size,
      len - encoded_size);

  // Encode TAC list
  for (int i = 0; i < item.tac_list.size(); i++) {
    ENCODE_U24(buf + encoded_size, item.tac_list[i], encoded_size);
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsTrackingAreaIdList::encode_01_type(
    p_tai_t item, uint8_t* buf, int len) {
  // TODO:
  return KEncodeDecodeError;
}

//------------------------------------------------------------------------------
int _5gsTrackingAreaIdList::encode_10_type(
    p_tai_t item, uint8_t* buf, int len) {
  // TODO:
  return KEncodeDecodeError;
}
