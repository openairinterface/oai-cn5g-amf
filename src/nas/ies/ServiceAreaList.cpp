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
#include "ServiceAreaList.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "utils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ServiceAreaList::ServiceAreaList()
    : Type4NasIe(kIei5gsTrackingAreaIdentityList), ie_list_() {
  SetLengthIndicator(
      kServiceAreaListMinimumLength -
      2);  // Minimim length - 2 bytes for header
}

//------------------------------------------------------------------------------
ServiceAreaList::ServiceAreaList(bool iei) : Type4NasIe(), ie_list_() {
  if (iei) {
    SetIei(kIeiServiceAreaList);
  }
  SetLengthIndicator(
      kServiceAreaListMinimumLength -
      2);  // Minimim length - 2 bytes for header
}

//------------------------------------------------------------------------------
ServiceAreaList::ServiceAreaList(
    const std::vector<service_area_list_ie_t>& list)
    : Type4NasIe(kIeiServiceAreaList) {
  // "Allowed type" should be the same in all the partial service area lists
  for (int i = 0; i < ie_list_.size(); i++) {
    if (ie_list_[i].type != ie_list_[0].type) return;
  }
  // only store the first 16 TAIs
  uint8_t size = (list.size() > kServiceAreaListMaximumSupportedTAIs) ?
                     kServiceAreaListMaximumSupportedTAIs :
                     list.size();
  for (int i = 0; i < size; i++) {
    ie_list_.push_back(list[i]);
  }
  // Don't know Length Indicator for now
}

//------------------------------------------------------------------------------
int ServiceAreaList::Encode(uint8_t* buf, int len) {
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

  for (int i = 0; i < ie_list_.size(); i++) {
    int item_len = 0;
    switch (ie_list_[i].type) {
      case 0x00: {
        int encode_00_type_size =
            encode_00_type(ie_list_[i], buf + encoded_size, len - encoded_size);
        if (encode_00_type_size == KEncodeDecodeError) break;
        item_len += encode_00_type_size;
      } break;
      case 0x01: {
        int encode_01_type_size =
            encode_01_type(ie_list_[i], buf + encoded_size, len - encoded_size);

        if (encode_01_type_size == KEncodeDecodeError) break;
        item_len += encode_01_type_size;
      } break;
      case 0x10: {
        int encode_10_type_size =
            encode_10_type(ie_list_[i], buf + encoded_size, len - encoded_size);
        if (encode_10_type_size == KEncodeDecodeError) break;
        item_len += encode_10_type_size;
      } break;
      case 0x11: {
        int encode_11_type_size =
            encode_11_type(ie_list_[i], buf + encoded_size, len - encoded_size);
        if (encode_11_type_size == KEncodeDecodeError) break;
        item_len += encode_11_type_size;
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
int ServiceAreaList::encode_00_type(
    service_area_list_ie_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  // Allowed type/Type of list/Number of elements
  uint8_t octet = (item.allowed_type & 0x80) | (item.type & 0x60) |
                  ((item.tac_list.size() - 1) &
                   0x1f);  // see Table 9.11.3.49.2@3GPP TS 24.501 V16.14.0
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
int ServiceAreaList::encode_01_type(
    service_area_list_ie_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  // Allowed type/Type of list/Number of elements
  uint8_t octet = (item.allowed_type & 0x80) | (item.type & 0x60) |
                  ((item.tac_list.size() - 1) &
                   0x1f);  // see Table 9.11.3.49.3@3GPP TS 24.501 V16.14.0
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  // Encode PLMN
  encoded_size += utils::encodeMccMnc2Buffer(
      item.plmn_list[0].mcc, item.plmn_list[0].mnc, buf + encoded_size,
      len - encoded_size);

  // Encode TAC
  ENCODE_U24(buf + encoded_size, item.tac_list[0], encoded_size);

  return encoded_size;
}

//------------------------------------------------------------------------------
int ServiceAreaList::encode_10_type(
    service_area_list_ie_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  // Allowed type/Type of list/Number of elements
  uint8_t octet = (item.allowed_type & 0x80) | (item.type & 0x60) |
                  ((item.tac_list.size() - 1) &
                   0x1f);  // see Table 9.11.3.49.4@3GPP TS 24.501 V16.14.0
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  int list_size = (item.plmn_list.size() > item.tac_list.size()) ?
                      item.plmn_list.size() :
                      item.tac_list.size();

  for (int i = 0; i < list_size; i++) {
    // Encode PLMN
    encoded_size += utils::encodeMccMnc2Buffer(
        item.plmn_list[i].mcc, item.plmn_list[i].mnc, buf + encoded_size,
        len - encoded_size);
    // Encode TAC
    ENCODE_U24(buf + encoded_size, item.tac_list[i], encoded_size);
  }

  return encoded_size;
}

//------------------------------------------------------------------------------
int ServiceAreaList::encode_11_type(
    service_area_list_ie_t item, uint8_t* buf, int len) {
  int encoded_size = 0;
  // Allowed type/Type of list/Number of elements
  uint8_t octet = 0x00 | (item.type & 0x60) |
                  0x00;  // see Table 9.11.3.49.5@3GPP TS 24.501 V16.14.0
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
