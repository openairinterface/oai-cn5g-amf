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
#include "spdlog/fmt/fmt.h"

#include "3gpp_23.003.h"

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}
using namespace std;
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
  string mcc;
  string mnc;
} nas_plmn_t;

typedef struct {
  uint8_t type;
  std::vector<nas_plmn_t> plmn_list;
  std::vector<uint32_t> tac_list;
} p_tai_t;

}  // namespace nas

#endif
