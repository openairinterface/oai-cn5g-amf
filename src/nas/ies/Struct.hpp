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

#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <stdint.h>

#include <iostream>
#include <vector>

#include "3gpp_23.003.h"
#include <fmt/format.h>

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}

namespace nas {

typedef struct SNSSAI_s {
  uint8_t sst;
  int32_t sd;
  int8_t mHplmnSst;
  int32_t mHplmnSd;
  uint8_t length;

  SNSSAI_s& operator=(const struct SNSSAI_s& s) {
    sst       = s.sst;
    sd        = s.sd;
    mHplmnSst = s.mHplmnSst;
    mHplmnSd  = s.mHplmnSd;
    length    = s.length;
    return *this;
  }

  std::string ToString() {
    std::string s;
    s.append(fmt::format("SST {:#x}", sst));

    if (length >= (SST_LENGTH + SD_LENGTH)) {
      s.append(fmt::format(" SD {:#x}", sd));
    }

    if ((length == (SST_LENGTH + SST_LENGTH)) ||
        (length >= (SST_LENGTH + SD_LENGTH + SST_LENGTH))) {
      s.append(fmt::format(" M-HPLMN SST {:#x}", mHplmnSst));
    }

    if (length == (SST_LENGTH + SD_LENGTH + SST_LENGTH + SD_LENGTH)) {
      s.append(fmt::format(" M-HPLMN SD {:#x}", mHplmnSd));
    }
    return s;
  }

} SNSSAI_t;

typedef struct {
  uint8_t ie_type;
  uint8_t ie_len;
  bstring ie_value;
} IE_t;

typedef struct {
  uint8_t ie_type;
  uint16_t ie_len;
  bstring ie_value;
} IE_t_E;

typedef struct {
  uint8_t length;
  uint8_t payloadContainerType : 4;
  std::vector<IE_t> optionalIE;
} PayloadContainerEntry;

typedef struct IMEISV_s {
  bstring identity;
} IMEISV_t;

typedef struct {
  std::string mcc;
  std::string mnc;
} nas_plmn_t;

typedef struct {
  uint8_t type;
  std::vector<nas_plmn_t> plmn_list;
  std::vector<uint32_t> tac_list;
} p_tai_t;

typedef struct {
  uint8_t allowed_type;
  uint8_t type;
  std::vector<nas_plmn_t> plmn_list;
  std::vector<uint32_t> tac_list;
} service_area_list_ie_t;

// 5G-GUTI
typedef struct _5G_GUTI_s {
  std::string mcc;
  std::string mnc;
  uint8_t amf_region_id;
  uint8_t amf_set_id;
  uint16_t amf_pointer;
  uint32_t _5g_tmsi;
} _5G_GUTI_t;

// IMEI or IMEISV
typedef struct IMEI_or_IMEISV_s {
  uint8_t type_of_identity_ : 3;
  bool odd_even_indic;   // for imei, even means bits 5 to 8 of last octet is
                         // "1111", for imeisv, bits 5 to 8 of last octet is
                         // "1111"
  std::string identity;  // "46011000001": without 1111
} IMEI_IMEISV_t;

// TODO:
// 5GS mobile identity information element for type of identity "SUCI" and SUPI
// format "IMSI"

// SUCI and SUPI format IMSI and
// Protection scheme Id "Null scheme"
typedef struct SUCI_imsi_s {
  uint8_t supi_format : 3;
  std::string mcc;
  std::string mnc;
  std::optional<std::string> routing_indicator;  //"1234"
  uint8_t protection_scheme_id : 4;              // 0000
  uint8_t home_network_pki;                      // 00000000
  std::string msin;  // two types of coding; BCD & hexadecimal
} SUCI_imsi_t;       // SUPI format "IMSI"

// TODO: SUCI and SUPI format "Network specific identifier"

// 5G-S-TMSI
typedef struct _5G_S_TMSI_s {
  uint16_t amf_set_id;
  uint8_t amf_pointer;
  std::string _5g_tmsi;
} _5G_S_TMSI_t;

// TODO: 5GS mobile identity information element for type of identity "MAC
// address"
}  // namespace nas

#endif
