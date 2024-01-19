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

#ifndef CONFIGURATION_UPDATE_COMMAND_H_
#define CONFIGURATION_UPDATE_COMMAND_H_

#include "NasIeHeader.hpp"

namespace nas {

class ConfigurationUpdateCommand : public NasMmPlainHeader {
 public:
  ConfigurationUpdateCommand();
  ~ConfigurationUpdateCommand();

  void SetHeader(uint8_t security_header_type);
  void GetSecurityHeaderType(uint8_t security_header_type);
  bool VerifyHeader();

  void SetConfigurationUpdateIndication(
      const ConfigurationUpdateIndication& configuration_update_indication);
  void GetConfigurationUpdateIndication(
      std::optional<ConfigurationUpdateIndication>&
          configuration_update_indication);

  void Set5gGuti(
      const std::string& mcc, const std::string& mnc, uint8_t amf_region_id,
      uint16_t amf_set_id, uint8_t amf_pointer, uint32_t tmsi);
  // TODO: Get

  void SetFullNameForNetwork(const NetworkName& name);
  void SetFullNameForNetwork(const std::string& text_string);
  void GetFullNameForNetwork(std::optional<NetworkName>& name) const;

  void SetShortNameForNetwork(const NetworkName& name);
  void SetShortNameForNetwork(const std::string& text_string);
  void GetShortNameForNetwork(NetworkName& name) const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

 public:
  // Optional
  // Configuration update indication
  std::optional<ConfigurationUpdateIndication>
      ie_configuration_update_indication;        // Optional
  std::optional<_5gsMobileIdentity> ie_5g_guti;  // Optional
  // TODO: TAI list
  // TODO: Allowed NSSAI
  // TODO: Service area list
  // Full name for network
  std::optional<NetworkName> ie_full_name_for_network;
  // Short name for network
  std::optional<NetworkName> ie_short_name_for_network;
  // TODO: Local time zone
  // TODO: Universal time and local time zone
  // TODO: Network daylight saving time
  // TODO: LADN information
  // TODO: MICO indication
  // TODO: Network slicing indication
  // TODO: Configured NSSAI
  // TODO: Rejected NSSAI
  // TODO: Operator-defined access category definitions
  // TODO: SMS indication
  // TODO: T3447 value
  // TODO: CAG information list (Rel 16.4.1)
  // TODO: UE radio capability ID (Rel 16.4.1)
  // TODO: UE radio capability ID deletion indication (Rel 16.4.1)
  // TODO: 5GS registration result (Rel 16.4.1)
  // TODO: Truncated 5G-S-TMSI configuration (Rel 16.4.1)
  // TODO: Additional configuration indication (Rel 16.14.0)
};

}  // namespace nas

#endif
