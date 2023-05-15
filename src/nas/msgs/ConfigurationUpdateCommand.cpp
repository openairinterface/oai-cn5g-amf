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

using namespace nas;

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::ConfigurationUpdateCommand()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, CONFIGURATION_UPDATE_COMMAND) {
  full_name_for_network  = nullopt;
  short_name_for_network = nullopt;
}

//------------------------------------------------------------------------------
ConfigurationUpdateCommand::~ConfigurationUpdateCommand() {}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetFullNameForNetwork(
    const NetworkName& name) {
  full_name_for_network = std::optional<NetworkName>(name);
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
  full_name_for_network = std::optional<NetworkName>(full_name_for_network_tmp);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::GetFullNameForNetwork(
    std::optional<NetworkName>& name) const {
  name = full_name_for_network;
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
  short_name_for_network =
      std::optional<NetworkName>(short_name_for_network_tmp);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateCommand::SetShortNameForNetwork(
    const NetworkName& name) {
  short_name_for_network = std::optional<NetworkName>(name);
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

  if (!full_name_for_network.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", NetworkName::GetIeName().c_str());
  } else {
    encoded_ie_size = full_name_for_network.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", NetworkName::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!short_name_for_network.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", NetworkName::GetIeName().c_str());
  } else {
    encoded_ie_size = short_name_for_network.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", NetworkName::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded ConfigurationUpdateCommand message (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ConfigurationUpdateCommand::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ConfigurationUpdateCommand message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiFullNameForNetwork: {
        Logger::nas_mm().debug("Decoding IEI 0x43: Full Name for Network");
        NetworkName full_name_for_network_tmp = {};
        if ((decoded_result = full_name_for_network_tmp.decodefrombuffer(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Encoding %s error", NetworkName::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        full_name_for_network =
            std::optional<NetworkName>(full_name_for_network_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiShortNameForNetwork: {
        Logger::nas_mm().debug("Decoding IEI 0x45: Short Name for Network");
        NetworkName short_name_for_network_tmp = {};
        if ((decoded_result = short_name_for_network_tmp.decodefrombuffer(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Encoding %s error", NetworkName::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        short_name_for_network =
            std::optional<NetworkName>(short_name_for_network_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      } break;
    }
  }

  Logger::nas_mm().debug(
      "Decoded ConfigurationUpdateCommand message (len %d)", decoded_size);
  return decoded_size;
}
