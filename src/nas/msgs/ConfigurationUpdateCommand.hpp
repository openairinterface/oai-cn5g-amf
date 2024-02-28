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

 private:
  // Configuration update indication
  std::optional<ConfigurationUpdateIndication>
      ie_configuration_update_indication_;        // Optional
  std::optional<_5gsMobileIdentity> ie_5g_guti_;  // Optional
  // TODO: TAI list (Optional)
  // TODO: Allowed NSSAI (Optional)
  // TODO: Service area list (Optional)
  // Full name for network (Optional)
  std::optional<NetworkName> ie_full_name_for_network_;  // Optional
  // Short name for network
  std::optional<NetworkName> ie_short_name_for_network_;  // Optional
  // TODO: Local time zone (Optional)
  // TODO: Universal time and local time zone (Optional)
  // TODO: Network daylight saving time (Optional)
  // TODO: LADN information (Optional)
  // TODO: MICO indication (Optional)
  // TODO: Network slicing indication (Optional)
  // TODO: Configured NSSAI (Optional)
  // TODO: Rejected NSSAI (Optional)
  // TODO: Operator-defined access category definitions (Optional)
  // TODO: SMS indication (Optional)
  // TODO: T3447 value (Optional)
  // TODO: CAG information list (Rel 16.4.1) (Optional)
  // TODO: UE radio capability ID (Rel 16.4.1) (Optional)
  // TODO: UE radio capability ID deletion indication (Rel 16.4.1) (Optional)
  // TODO: 5GS registration result (Rel 16.4.1) (Optional)
  // TODO: Truncated 5G-S-TMSI configuration (Rel 16.4.1) (Optional)
  // TODO: Additional configuration indication (Rel 16.14.0) (Optional)
};

}  // namespace nas

#endif
