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

#include "AMFPointer.hpp"
#include "AMFRegionID.hpp"
#include "AMFSetID.hpp"
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
      const PlmnId& m_plmnId, const AMFRegionID& m_aMFRegionID,
      const AMFSetID& m_aMFSetID, const AMFPointer& m_aMFPointer);
  bool setGUAMI(
      const std::string& mcc, const std::string& mnc, const uint8_t& regionId,
      const uint16_t& setId, const uint8_t& pointer);

  bool setGUAMI(
      const std::string& mcc, const std::string& mnc,
      const std::string& regionId, const std::string& setId,
      const std::string& pointer);

  void getGUAMI(
      PlmnId& m_plmnId, AMFRegionID& m_aMFRegionID, AMFSetID& m_aMFSetID,
      AMFPointer& m_aMFPointer);
  void getGUAMI(
      std::string& mcc, std::string& mnc, std::string& regionId,
      std::string& setId, std::string& pointer);

  bool encode2GUAMI(Ngap_GUAMI_t* guami);
  bool decodefromGUAMI(Ngap_GUAMI_t* pdu);

 private:
  PlmnId plmnId;            // Mandatory
  AMFRegionID aMFRegionID;  // Mandatory
  AMFSetID aMFSetID;        // Mandatory
  AMFPointer aMFPointer;    // Mandatory
};

}  // namespace ngap

#endif
