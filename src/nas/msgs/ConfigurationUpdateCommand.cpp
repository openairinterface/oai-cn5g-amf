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

#include "ConfigurationUpdateCommand.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::ConfigurationUpdateCommand()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, CONFIGURATION_UPDATE_COMMAND) {
  ie_configuration_update_indication = std::nullopt;
  ie_5g_guti                         = std::nullopt;
  ie_full_name_for_network           = std::nullopt;
  ie_short_name_for_network          = std::nullopt;
}

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::~ConfigurationUpdateCommand() {}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetConfigurationUpdateIndication(
    const ConfigurationUpdateIndication& configuration_update_indication) {
  ie_configuration_update_indication =
      std::make_optional<ConfigurationUpdateIndication>(
          configuration_update_indication);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::GetConfigurationUpdateIndication(
    std::optional<ConfigurationUpdateIndication>&
        configuration_update_indication) {
  configuration_update_indication = ie_configuration_update_indication;
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::Set5gGuti(
    const std::string& mcc, const std::string& mnc, uint8_t amf_region_id,
    uint16_t amf_set_id, uint8_t amf_pointer, uint32_t tmsi) {
  _5gsMobileIdentity ie_5g_guti_tmp = {};
  ie_5g_guti_tmp.SetIei(kIei5gGuti);
  ie_5g_guti_tmp.Set5gGuti(
      mcc, mnc, amf_region_id, amf_set_id, amf_pointer, tmsi);
  ie_5g_guti = std::optional<_5gsMobileIdentity>(ie_5g_guti_tmp);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetFullNameForNetwork(
    const NetworkName& name) {
  ie_full_name_for_network = std::optional<NetworkName>(name);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetFullNameForNetwork(
    const std::string& text_string) {
  NetworkName full_name_for_network_tmp;
  full_name_for_network_tmp.setIEI(kIeiFullNameForNetwork);
  full_name_for_network_tmp.setCodingScheme(0);
  full_name_for_network_tmp.setAddCI(0);
  full_name_for_network_tmp.setNumberOfSpareBits(
      0x07);  // TODO: remove hardcoded value
  full_name_for_network_tmp.setTextString(text_string);
  ie_full_name_for_network =
      std::optional<NetworkName>(full_name_for_network_tmp);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::GetFullNameForNetwork(
    std::optional<NetworkName>& name) const {
  name = ie_full_name_for_network;
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetShortNameForNetwork(
    const std::string& text_string) {
  NetworkName short_name_for_network_tmp;
  short_name_for_network_tmp.setIEI(kIeiShortNameForNetwork);  // TODO
  short_name_for_network_tmp.setCodingScheme(0);
  short_name_for_network_tmp.setAddCI(0);
  short_name_for_network_tmp.setNumberOfSpareBits(
      0x07);  // TODO: remove hardcoded value
  short_name_for_network_tmp.setTextString(text_string);
  ie_short_name_for_network =
      std::optional<NetworkName>(short_name_for_network_tmp);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetShortNameForNetwork(
    const NetworkName& name) {
  ie_short_name_for_network = std::optional<NetworkName>(name);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::GetShortNameForNetwork(
    NetworkName& name) const {}

//------------------------------------------------------------------------------
int ConfigurationUpdateCommand::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding ConfigurationUpdateCommand message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if ((encoded_ie_size = NasHelper::Encode(
           ie_configuration_update_indication, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_5g_guti, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_full_name_for_network, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_short_name_for_network, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded ConfigurationUpdateCommand message (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ConfigurationUpdateCommand::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ConfigurationUpdateCommand message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiConfigurationUpdateIndication: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiConfigurationUpdateIndication);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_configuration_update_indication, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        flag = true;
      }
    }

    switch (octet) {
      case kIeiFullNameForNetwork: {
        Logger::nas_mm().debug("Decoding IEI 0x43: Full Name for Network");
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_full_name_for_network, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiShortNameForNetwork: {
        Logger::nas_mm().debug("Decoding IEI 0x45: Short Name for Network");
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_short_name_for_network, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        // TODO:
        if (flag) {
          Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
          // Stop decoding
          octet = 0x00;
        }
      } break;
    }
  }

  Logger::nas_mm().debug(
      "Decoded ConfigurationUpdateCommand message (len %d)", decoded_size);
  return decoded_size;
}
