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

#include "_5gsMobileIdentity.hpp"

#include "3gpp_24.501.hpp"
#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsMobileIdentity::_5gsMobileIdentity() : Type6NasIe() {
  type_of_identity_ = 0;
  ClearIe();
}

//------------------------------------------------------------------------------
_5gsMobileIdentity::_5gsMobileIdentity(uint8_t iei) : Type6NasIe(iei) {
  type_of_identity_ = 0;
  ClearIe();
}

//------------------------------------------------------------------------------
_5gsMobileIdentity::~_5gsMobileIdentity() {}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::ClearIe() {
  _5g_guti_         = std::nullopt;
  supi_format_imsi_ = std::nullopt;
  _5g_s_tmsi_       = std::nullopt;
  imeisv_           = std::nullopt;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  switch (type_of_identity_) {
    case SUCI: {
      return EncodeSuci(buf, len);
    } break;
    case _5G_GUTI: {
      return Encode5gGuti(buf, len);
    } break;
    // TODO: IMEI
    case IMEISV: {
      return EncodeImeisv(buf, len);
    } break;
    case _5G_S_TMSI: {
      return Encode5gSTmsi(buf, len);
    } break;
    default: {
      Logger::nas_mm().debug(
          "Unknown Type of Identity  0x%x", type_of_identity_);
    }
  }
  return KEncodeDecodeError;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity");
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  Logger::amf_n1().debug("Decoded 5GSMobilityIdentity IE length %d", ie_len);
  int decoded_size_tmp = 0;
  uint8_t octet        = 0;

  // Type of Identity
  DECODE_U8(
      buf + decoded_size, octet,
      decoded_size_tmp);  // Decode but don't increase decoded_size

  switch (octet & 0x07) {
    case SUCI: {
      type_of_identity_ = SUCI;
      decoded_size +=
          DecodeSuci(buf + decoded_size, len - decoded_size, ie_len);
      Logger::nas_mm().debug("Decoded SUCI (%d octets)", decoded_size);
    } break;
    case _5G_GUTI: {
      type_of_identity_ = _5G_GUTI;
      decoded_size += Decode5gGuti(buf + decoded_size, len - decoded_size);
    } break;
    case _5G_S_TMSI: {
      type_of_identity_ = _5G_S_TMSI;
      decoded_size += Decode5gSTmsi(buf + decoded_size, len - decoded_size);
    } break;
    // TODO: IMEI
    case IMEISV: {
      type_of_identity_ = IMEISV;
      decoded_size += DecodeImeisv(buf + decoded_size, ie_len);
    } break;
    // TODO: MAC Address
    default: {
      // TODO:
      return 0;
    }
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Encode5gGuti(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding 5G-GUTI IEI 0x%x", iei_.value());
  int encoded_size = 0;

  // IEI and Length
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Type of Identity
  ENCODE_U8(
      buf + encoded_size, 0xf0 | _5G_GUTI,
      encoded_size);  // Type of Identity
  // MCC/MNC
  encoded_size += utils::encodeMccMnc2Buffer(
      _5g_guti_.value().mcc, _5g_guti_.value().mnc, buf + encoded_size,
      len - encoded_size);
  // AMF Region ID
  ENCODE_U8(buf + encoded_size, _5g_guti_.value().amf_region_id, encoded_size);
  // AMF Set Id/AMF pointer
  ENCODE_U8(
      buf + encoded_size, ((_5g_guti_.value().amf_set_id & 0x03ff) >> 2),
      encoded_size);
  ENCODE_U8(
      buf + encoded_size,
      ((_5g_guti_.value().amf_pointer & 0x3f) |
       ((_5g_guti_.value().amf_set_id & 0x0003) << 6)),
      encoded_size);

  // TMSI: 4 octets
  uint32_t tmsi = _5g_guti_.value()._5g_tmsi;
  ENCODE_U32(buf + encoded_size, tmsi, encoded_size);

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug("Encoded 5G-GUTI IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Decode5gGuti(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity 5G-GUTI");

  int decoded_size = 0;
  uint8_t octet    = 0;

  // Decode SUPI format/Type of Identity
  DECODE_U8(buf + decoded_size, octet, decoded_size);  // Type of Identity
  // TODO:validate Type of Identity

  _5G_GUTI_t tmp = {};
  decoded_size += utils::decodeMccMncFromBuffer(
      tmp.mcc, tmp.mnc, buf + decoded_size, len - decoded_size);

  Logger::nas_mm().debug("MCC %s, MNC %s", tmp.mcc.c_str(), tmp.mnc.c_str());

  DECODE_U8(buf + decoded_size, tmp.amf_region_id, decoded_size);
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  tmp.amf_set_id = octet << 2;  // 8 most significant bits
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  tmp.amf_set_id |=
      ((octet & 0xc0) >> 6);       // 2 most significant bits of this octet
                                   // as 2 lest significant bits of AMF Set ID
  tmp.amf_pointer = octet & 0x3f;  // 6 lest significant bits

  // TMSI, 4 octets
  DECODE_U32(buf + decoded_size, tmp._5g_tmsi, decoded_size);

  Logger::nas_mm().debug("TMSI 0x%x", tmp._5g_tmsi);
  _5g_guti_ = std::optional<_5G_GUTI_t>(tmp);
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity 5G-GUTI");
  return decoded_size;
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::Set5gGuti(
    const std::string& mcc, const std::string& mnc,
    const uint8_t& amf_region_id, const uint16_t& amf_set_id,
    const uint8_t& amf_pointer, const uint32_t& _5g_tmsi) {
  // Clear all identity types first
  ClearIe();

  // Set value for 5G GUTI
  type_of_identity_ = _5G_GUTI;
  _5G_GUTI_t tmp    = {};
  tmp.mcc           = mcc;
  tmp.mnc           = mnc;
  tmp.amf_region_id = amf_region_id;
  tmp.amf_set_id    = amf_set_id;
  tmp.amf_pointer   = amf_pointer;
  tmp._5g_tmsi      = _5g_tmsi;
  _5g_guti_         = std::optional<_5G_GUTI_t>(tmp);
  SetLengthIndicator(k5gsMobileIdentityIe5gGutiLength);
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::Get5gGuti(std::optional<_5G_GUTI_t>& guti) const {
  guti = _5g_guti_;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::EncodeSuci(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SUCI");

  if (!supi_format_imsi_.has_value()) return KEncodeDecodeError;

  int encoded_size = 0;
  // IEI and Length
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // SUPI format + Type of Identity
  ENCODE_U8(
      buf + encoded_size, (0x70 & (SUPI_FORMAT_IMSI << 4)) | (0x07 & SUCI),
      encoded_size);

  // MCC/MNC
  encoded_size += utils::encodeMccMnc2Buffer(
      supi_format_imsi_.value().mcc, supi_format_imsi_.value().mnc,
      buf + encoded_size, len - encoded_size);

  // Routing Indicator
  encoded_size += EncodeRoutingIndicator(
      supi_format_imsi_.value().routing_indicator, buf + encoded_size,
      len - encoded_size);

  // Protection Scheme
  ENCODE_U8(
      buf + encoded_size, 0x0f & supi_format_imsi_.value().protection_scheme_id,
      encoded_size);

  // Home Network Public Key Identifier
  ENCODE_U8(
      buf + encoded_size, supi_format_imsi_.value().home_network_pki,
      encoded_size);

  // MSIN
  std::string msin   = {};
  uint8_t digit_low  = 0;
  uint8_t digit_high = 0;
  for (int i = 0; i < supi_format_imsi_.value().msin.length() / 2; i++) {
    conv::string_to_int8(
        supi_format_imsi_.value().msin.substr(i, 1), digit_low);
    conv::string_to_int8(
        supi_format_imsi_.value().msin.substr(i + 1, 1), digit_high);
    uint8_t octet = (0xf0 & (digit_high << 4)) | (digit_low & 0x0f);
    ENCODE_U8(buf + encoded_size, octet, encoded_size);
  }
  if (supi_format_imsi_.value().msin.length() % 2 != 0) {
    conv::string_to_int8(
        supi_format_imsi_.value().msin.substr(
            supi_format_imsi_.value().msin.length() - 1, 1),
        digit_low);
    uint8_t octet = 0xf0 | (digit_low & 0x0f);
    ENCODE_U8(buf + encoded_size, octet, encoded_size);
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug("Encoded SUCI IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::DecodeSuci(uint8_t* buf, int len, int ie_len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity SUCI");
  int decoded_size = 0;
  uint8_t octet    = 0;

  // Decode SUPI format/Type of Identity
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  switch ((octet & 0x70) >> 4) {
    case SUPI_FORMAT_IMSI: {
      SUCI_imsi_t supi_format_imsi_tmp = {};
      supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;

      decoded_size += utils::decodeMccMncFromBuffer(
          supi_format_imsi_tmp.mcc, supi_format_imsi_tmp.mnc,
          buf + decoded_size, len - decoded_size);
      Logger::nas_mm().debug(
          "MCC %s, MNC %s", supi_format_imsi_tmp.mcc.c_str(),
          supi_format_imsi_tmp.mnc.c_str());

      // Routing Indicator
      uint8_t digit[4];
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      digit[0] = octet & 0x0f;
      digit[1] = (octet & 0xf0) >> 4;

      DECODE_U8(buf + decoded_size, octet, decoded_size);
      digit[2] = octet & 0x0f;
      digit[3] = (octet & 0xf0) >> 4;

      if (!digit[0] && digit[1] == 0x0f && digit[2] == 0x0f &&
          digit[3] == 0x0f) {
        supi_format_imsi_tmp.routing_indicator =
            std::nullopt;  // No Routing Indicator is configured
        Logger::nas_mm().debug("No Routing Indicator is configured");
      } else {
        std::string result = {};
        for (int i = 0; i < 4; i++) {
          if (digit[i] >= 0x00 && digit[i] <= 0x09)
            result += (const std::string) (std::to_string(digit[i]));
          else if (digit[i] == 0x0f)
            break;
          else
            Logger::nas_mm().error(
                "Decoded Routing Indicator is not BCD coding");
        }

        supi_format_imsi_tmp.routing_indicator =
            std::optional<std::string>(result);
        Logger::nas_mm().debug(
            "Decoded Routing Indicator %s",
            supi_format_imsi_tmp.routing_indicator.value().c_str());
      }

      // Protection scheme Id
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      supi_format_imsi_tmp.protection_scheme_id = 0x0f & octet;
      // Home network public key identifier
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      supi_format_imsi_tmp.home_network_pki = octet;

      // MSIN
      std::string msin = {};
      // TODO: get MSIN according to Protection Scheme ID to support
      // ECIES scheme profile A/B
      uint8_t digit_low  = 0;
      uint8_t digit_high = 0;
      int numMsin        = ie_len - decoded_size;
      for (int i = 0; i < (numMsin - 1); i++) {
        DECODE_U8(buf + decoded_size, octet, decoded_size);
        digit_high = (octet & 0xf0) >> 4;
        digit_low  = octet & 0x0f;
        msin +=
            ((const std::string) (std::to_string(digit_low)) +
             (const std::string) (std::to_string(digit_high)));
      }

      // Verify if the MSIN includes an odd number of digits,
      // bits 5 to 8 of the last octet shall be coded as "1111"
      DECODE_U8(buf + decoded_size, octet, decoded_size);
      digit_high = (octet & 0xf0) >> 4;
      digit_low  = octet & 0x0f;
      if (digit_high != 0x0f) {
        msin +=
            ((const std::string) (std::to_string(digit_low)) +
             (const std::string) (std::to_string(digit_high)));
      } else {
        msin += (const std::string) (std::to_string(digit_low));
      }

      supi_format_imsi_tmp.msin = msin;
      Logger::nas_mm().debug(
          "Decoded MSIN %s", supi_format_imsi_tmp.msin.c_str());

      supi_format_imsi_ = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);
      Logger::nas_mm().debug(
          "Decoded 5GSMobilityIdentity SUCI SUPI format IMSI (len %d)",
          decoded_size);
      return decoded_size;
    } break;
    case SUPI_FORMAT_NETWORK_SPECIFIC_IDENTIFIER: {
      Logger::nas_mm().warn(
          "SUCI/SUPI format with Network Specific Identifier is not supported");
      // TODO:
    } break;
    default: {
      // TODO:
    } break;
  }
  return decoded_size;
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::SetSuciWithSupiImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, const uint8_t protection_sch_id,
    const std::string& msin) {
  // Clear all identity types first
  ClearIe();

  // Set value for SUCI/SUPI format IMSI
  SUCI_imsi_t supi_format_imsi_tmp = {};

  iei_                             = 0;
  type_of_identity_                = SUCI;
  supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;
  supi_format_imsi_tmp.mcc         = mcc;
  supi_format_imsi_tmp.mnc         = mnc;
  supi_format_imsi_tmp.routing_indicator =
      std::optional<std::string>(routing_ind);
  supi_format_imsi_tmp.protection_scheme_id = protection_sch_id;
  supi_format_imsi_tmp.home_network_pki     = HOME_NETWORK_PKI_0_WHEN_PSI_0;
  supi_format_imsi_tmp.msin                 = msin;
  SetLengthIndicator(10 + ceil(msin.length() / 2));

  supi_format_imsi_ = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::SetSuciWithSupiImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, const uint8_t protection_sch_id,
    const uint8_t home_pki, const std::string& msin_digits) {
  // Clear all identity types first
  ClearIe();

  // Set value for SUCI/SUPI format IMSI
  SUCI_imsi_t supi_format_imsi_tmp = {};

  supi_format_imsi_tmp.supi_format = SUPI_FORMAT_IMSI;
  supi_format_imsi_tmp.mcc         = mcc;
  supi_format_imsi_tmp.mnc         = mnc;
  supi_format_imsi_ = std::optional<SUCI_imsi_t>(supi_format_imsi_tmp);
  // TODO: SetLengthIndicator();
}

//------------------------------------------------------------------------------
bool _5gsMobileIdentity::GetSuciWithSupiImsi(SUCI_imsi_t& suci) const {
  if (!supi_format_imsi_.has_value()) return false;
  suci = supi_format_imsi_.value();
  return true;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::EncodeRoutingIndicator(
    std::optional<std::string> routing_indicator, uint8_t* buf, int len) {
  int encoded_size = 0;
  if (!routing_indicator.has_value()) {
    Logger::nas_mm().debug("No Routing Indicator is configured, encoding");
    ENCODE_U8(buf + encoded_size, 0xf0, encoded_size);
    ENCODE_U8(buf + encoded_size, 0xff, encoded_size);
    return encoded_size;
  }

  Logger::nas_mm().debug(
      "Routing Indicator (%s)", routing_indicator.value().c_str());
  int rooutid = utils::fromString<int>(routing_indicator.value());
  switch (routing_indicator.value().length()) {
    case 1: {
      *buf = 0xf0 | (0x0f & rooutid);
      encoded_size++;
      *(buf + encoded_size) = 0xff;
      encoded_size++;
    } break;
    case 2: {
      *buf = (0x0f & (rooutid / 10)) | ((0x0f & (rooutid % 10)) << 4);
      encoded_size++;
      *(buf + encoded_size) = 0xff;
      encoded_size++;
    } break;
    case 3: {
      *buf = (0x0f & (rooutid / 100)) | ((0x0f & ((rooutid % 100) / 10)) << 4);
      encoded_size++;
      *(buf + encoded_size) = 0xf0 | (0x0f & (rooutid % 10));
      encoded_size++;
    } break;
    case 4: {
      *buf =
          (0x0f & (rooutid / 1000)) | ((0x0f & ((rooutid % 1000) / 100)) << 4);
      encoded_size++;
      *(buf + encoded_size) =
          (0x0f & ((rooutid % 100) / 10)) | ((0x0f & (rooutid % 10)) << 4);
      encoded_size++;
    } break;
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::EncodeMsin(
    const std::string& msin_str, uint8_t* buf, int len) {
  Logger::nas_mm().warn("Encode MSIN to Buffer is not implemented yet!");
  return KEncodeDecodeError;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Encode5gSTmsi(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding 5GSMobilityIdentity 5G-S-TMSI");
  if (!_5g_s_tmsi_.has_value()) return KEncodeDecodeError;

  int encoded_size = 0;

  // IEI and Length
  int encoded_header_size = Type6NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Type of identity
  ENCODE_U8(buf + encoded_size, 0xf0 | _5G_S_TMSI, encoded_size);

  // AMF Set ID and AMF Pointer
  ENCODE_U8(
      buf + encoded_size, ((_5g_s_tmsi_.value().amf_set_id) & 0x03fc) >> 2,
      encoded_size);
  ENCODE_U8(
      buf + encoded_size,
      (((_5g_s_tmsi_.value().amf_set_id) & 0x0003) << 6) |
          ((_5g_s_tmsi_.value().amf_pointer) & 0x3f),
      encoded_size);

  // 5G-TMSI
  int tmsi = utils::fromString<int>(_5g_s_tmsi_.value()._5g_tmsi);
  ENCODE_U32(buf + encoded_size, tmsi, encoded_size);

  Logger::nas_mm().debug(
      "Encoded 5GSMobilityIdentity 5G-S-TMSI (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::Decode5gSTmsi(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity 5G-S-TMSI");
  int decoded_size            = 0;
  _5G_S_TMSI_t _5g_s_tmsi_tmp = {};
  uint8_t octet               = 0;

  // Decode SUPI format/Type of Identity
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  // TODO: validate Type of Identity

  // AMF Set ID and AMF Pointer
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _5g_s_tmsi_tmp.amf_set_id = 0x0000 | ((uint16_t) octet) << 2;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _5g_s_tmsi_tmp.amf_set_id |= (octet & 0xc0) >> 6;
  _5g_s_tmsi_tmp.amf_pointer = octet & 0x3f;

  // 5G TMSI
  uint32_t tmsi = 0;
  DECODE_U32(buf + decoded_size, tmsi, decoded_size);
  _5g_s_tmsi_tmp._5g_tmsi = amf_conv::tmsi_to_string(tmsi);
  _5g_s_tmsi_             = std::optional<_5G_S_TMSI_t>(_5g_s_tmsi_tmp);

  Logger::nas_mm().debug(
      "Decoded 5GSMobilityIdentity 5G-S-TMSI (len %d)", decoded_size);
  return decoded_size;
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::Set5gSTmsi(
    const uint16_t amf_set_id, const uint8_t amf_pointer,
    const std::string& tmsi) {
  // Clear all identity types first
  ClearIe();

  // Set value for 5GS TMSI
  _5G_S_TMSI_t _5g_s_tmsi_tmp = {};
  type_of_identity_           = _5G_S_TMSI;
  _5g_s_tmsi_tmp.amf_set_id   = amf_set_id;
  _5g_s_tmsi_tmp.amf_pointer  = amf_pointer;
  _5g_s_tmsi_tmp._5g_tmsi     = tmsi;

  _5g_s_tmsi_ = std::optional<_5G_S_TMSI_t>(_5g_s_tmsi_tmp);
  SetLengthIndicator(k5gsMobileIdentityIe5gSTmsiLength);
}

//------------------------------------------------------------------------------
bool _5gsMobileIdentity::Get5gSTmsi(
    uint16_t& amf_set_id, uint8_t& amf_pointer, std::string& tmsi) const {
  if (!_5g_s_tmsi_.has_value()) return false;

  amf_set_id  = _5g_s_tmsi_.value().amf_set_id;
  amf_pointer = _5g_s_tmsi_.value().amf_pointer;
  tmsi        = _5g_s_tmsi_.value()._5g_tmsi;
  return true;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::EncodeImeisv(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding IMEISV IE");
  if (!imeisv_.has_value()) return KEncodeDecodeError;

  int encoded_size = 0;
  int len_pos      = 0;
  // IEI and Length
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  uint8_t octet = {0};

  uint8_t digit_low  = 0;
  uint8_t digit_high = 0;

  int i = 0;
  while (i < imeisv_.value().identity.length()) {
    if (i == 0) {
      digit_low = 0x07 & IMEISV;  // TODO: odd/even indic
      conv::string_to_int8(imeisv_.value().identity.substr(i, 1), digit_high);
    } else if (i < imeisv_.value().identity.length() - 1) {
      conv::string_to_int8(imeisv_.value().identity.substr(i, 1), digit_low);
      conv::string_to_int8(
          imeisv_.value().identity.substr(i + 1, 1), digit_high);
      i++;
    } else if (i == imeisv_.value().identity.length() - 1) {
      conv::string_to_int8(imeisv_.value().identity.substr(i, 1), digit_low);
      digit_high = 0x0f;
    }
    uint8_t octet = (0xf0 & (digit_high << 4)) | (digit_low & 0x0f);
    ENCODE_U8(buf + encoded_size, octet, encoded_size);
    i++;
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug("Encoded IMEISV IE (len %d octets)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsMobileIdentity::DecodeImeisv(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSMobilityIdentity IMEISV");
  int decoded_size             = 0;
  IMEI_IMEISV_t imeisv_tmp     = {};
  imeisv_tmp.type_of_identity_ = IMEISV;
  imeisv_tmp.identity          = {};

  uint8_t digit_low  = 0;
  uint8_t digit_high = 0;
  uint8_t octet      = 0;
  for (int i = 0; i < len; i++) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    digit_high = (octet & 0xf0) >> 4;
    digit_low  = octet & 0x0f;
    if (i == 0) {
      imeisv_tmp.identity += (const std::string) (std::to_string(
          digit_high));  // octet 4 (Identity digit 1 4bits, odd/even indic 1
                         // bit, type of identity 3 bits)
    } else if (i < (len - 1)) {
      imeisv_tmp.identity +=
          ((const std::string) (std::to_string(digit_low)) +
           (const std::string) (std::to_string(digit_high)));
    } else {  // Bits 5 to 8 of the last octet: end mark coded as "1111"
      imeisv_tmp.identity += (const std::string) (std::to_string(digit_low));
      if (digit_high != 0x0f) {
        Logger::nas_mm().warn(
            "IMEISV: Bits 5 to 8 of the last octet should filled with an end "
            "mark coded as 1111");
      }
    }
  }

  Logger::nas_mm().debug(
      "Decoded 5GSMobilityIdentity IMEISV: %s", imeisv_tmp.identity.c_str());

  imeisv_ = std::optional<IMEI_IMEISV_t>(imeisv_tmp);
  Logger::nas_mm().debug(
      "Decoded 5GSMobilityIdentity IMEISV len (%d)", decoded_size);
  return decoded_size;
}

//------------------------------------------------------------------------------
void _5gsMobileIdentity::SetImeisv(const IMEI_IMEISV_t& imeisv) {
  // Clear all identity types first
  ClearIe();

  // Set value for IMEISV
  imeisv_ = std::optional<IMEI_IMEISV_t>(imeisv);
}

//------------------------------------------------------------------------------
bool _5gsMobileIdentity::GetImeisv(IMEI_IMEISV_t& imeisv) const {
  if (!imeisv_.has_value()) return false;
  imeisv = imeisv_.value();
  return true;
}
