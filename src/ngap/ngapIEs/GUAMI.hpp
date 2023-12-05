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

#ifndef _GUAMI_H_
#define _GUAMI_H_

#include "AmfPointer.hpp"
#include "AmfRegionId.hpp"
#include "AmfSetId.hpp"
#include "PlmnId.hpp"

extern "C" {
#include "Ngap_GUAMI.h"
}

namespace ngap {

class GUAMI {
 public:
  GUAMI();
  virtual ~GUAMI();

  void setGUAMI(
      const PlmnId& m_plmnId, const AmfRegionId& m_aMFRegionID,
      const AmfSetId& m_aMFSetID, const AmfPointer& m_aMFPointer);
  void getGUAMI(
      PlmnId& m_plmnId, AmfRegionId& m_aMFRegionID, AmfSetId& m_aMFSetID,
      AmfPointer& m_aMFPointer);

  bool setGUAMI(
      const std::string& mcc, const std::string& mnc, const uint8_t& regionId,
      const uint16_t& setId, const uint8_t& pointer);
  void getGUAMI(
      std::string& mcc, std::string& mnc, uint8_t regionId, uint16_t setId,
      uint8_t pointer);

  bool setGUAMI(
      const std::string& mcc, const std::string& mnc,
      const std::string& regionId, const std::string& setId,
      const std::string& pointer);
  void getGUAMI(
      std::string& mcc, std::string& mnc, std::string& regionId,
      std::string& setId, std::string& pointer);

  bool encode(Ngap_GUAMI_t& guami);
  bool decode(const Ngap_GUAMI_t& pdu);

 private:
  PlmnId plmnId;            // Mandatory
  AmfRegionId aMFRegionID;  // Mandatory
  AmfSetId aMFSetID;        // Mandatory
  AmfPointer aMFPointer;    // Mandatory
};

}  // namespace ngap

#endif
