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

class Guami {
 public:
  Guami();
  virtual ~Guami();

  void set(
      const PlmnId& plmnId, const AmfRegionId& amfRegionId,
      const AmfSetId& amfSetId, const AmfPointer& amfPointer);
  void get(
      PlmnId& plmnId, AmfRegionId& amfRegionId, AmfSetId& amfSetId,
      AmfPointer& amfPointer) const;

  bool set(
      const std::string& mcc, const std::string& mnc, uint8_t regionId,
      uint16_t setId, uint8_t pointer);
  void get(
      std::string& mcc, std::string& mnc, uint8_t& regionId, uint16_t& setId,
      uint8_t& pointer) const;

  bool set(
      const std::string& mcc, const std::string& mnc,
      const std::string& regionId, const std::string& setId,
      const std::string& pointer);
  void get(
      std::string& mcc, std::string& mnc, std::string& regionId,
      std::string& setId, std::string& pointer) const;

  bool encode(Ngap_GUAMI_t& guami) const;
  bool decode(const Ngap_GUAMI_t& pdu);

 private:
  PlmnId m_PlmnId;            // Mandatory
  AmfRegionId m_AmfRegionId;  // Mandatory
  AmfSetId m_AmfSetId;        // Mandatory
  AmfPointer m_AmfPointer;    // Mandatory
};

}  // namespace ngap

#endif
