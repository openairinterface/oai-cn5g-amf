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

#include "RegistrationAccept.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationAccept::RegistrationAccept()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, REGISTRATION_ACCEPT) {
  ie_5g_guti                                     = std::nullopt;
  ie_equivalent_plmns                            = std::nullopt;
  ie_allowed_nssai                               = std::nullopt;
  ie_rejected_nssai                              = std::nullopt;
  ie_configured_nssai                            = std::nullopt;
  ie_5gs_network_feature_support                 = std::nullopt;
  ie_pdu_session_status                          = std::nullopt;
  ie_pdu_session_reactivation_result             = std::nullopt;
  ie_pdu_session_reactivation_result_error_cause = std::nullopt;
  ie_ladn_information                            = std::nullopt;
  ie_mico_indication                             = std::nullopt;
  ie_network_slicing_indication                  = std::nullopt;
  ie_service_area_list                           = std::nullopt;
  ie_t3512_value                                 = std::nullopt;
  ie_non_3gpp_deregistration_timer_value         = std::nullopt;
  ie_t3502_value                                 = std::nullopt;
  ie_sor_transparent_container                   = std::nullopt;
  ie_eap_message                                 = std::nullopt;
  ie_nssai_inclusion_mode                        = std::nullopt;
  ie_negotiated_drx_parameters                   = std::nullopt;
  ie_non_3gpp_nw_policies                        = std::nullopt;
  ie_eps_bearer_context_status                   = std::nullopt;
  ie_extended_drx_parameters                     = std::nullopt;
  ie_t3447_value                                 = std::nullopt;
  ie_t3448_value                                 = std::nullopt;
  ie_t3324_value                                 = std::nullopt;
  ie_ue_radio_capability_id                      = std::nullopt;
  ie_pending_nssai                               = std::nullopt;
  ie_tai_list                                    = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationAccept::~RegistrationAccept() {}

//------------------------------------------------------------------------------
void RegistrationAccept::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gsRegistrationResult(
    bool emergency, bool nssaa, bool sms, uint8_t value) {
  ie_5gs_registration_result.set(emergency, nssaa, sms, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, uint8_t protection_sch_id,
    const std::string& msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "Encoding SUCI and SUPI format for IMSI error, please choose right "
        "scheme");
    return;
  } else {
    _5gsMobileIdentity ie_5g_guti_tmp = {};
    ie_5g_guti_tmp.SetIei(kIei5gGuti);
    ie_5g_guti_tmp.SetSuciWithSupiImsi(
        mcc, mnc, routing_ind, protection_sch_id, msin);
    ie_5g_guti = std::optional<_5gsMobileIdentity>(ie_5g_guti_tmp);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, uint8_t protection_sch_id,
    const uint8_t& hnpki, const std::string& msin) {
  // TODO:
}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gGuti(
    const std::string& mcc, const std::string& mnc, uint8_t amf_region_id,
    uint16_t amf_set_id, uint8_t amf_pointer, uint32_t tmsi) {
  _5gsMobileIdentity ie_5g_guti_tmp = {};
  ie_5g_guti_tmp.SetIei(kIei5gGuti);
  ie_5g_guti_tmp.Set5gGuti(
      mcc, mnc, amf_region_id, amf_set_id, amf_pointer, tmsi);
  ie_5g_guti = std::optional<_5gsMobileIdentity>(ie_5g_guti_tmp);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetImeiImeisv() {}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gSTmsi() {}

//------------------------------------------------------------------------------
void RegistrationAccept::SetEquivalentPlmns(
    const std::vector<nas_plmn_t>& list) {
  PlmnList ie_equivalent_plmns_tmp = {};
  ie_equivalent_plmns_tmp.Set(kEquivalentPlmns, list);
  ie_equivalent_plmns = std::optional<PlmnList>(ie_equivalent_plmns_tmp);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetAllowedNssai(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_allowed_nssai = std::make_optional<Nssai>(kIeiNSSAIAllowed, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetRejectedNssai(
    const std::vector<RejectedSNssai>& nssai) {
  if (nssai.size() > 0) {
    ie_rejected_nssai = std::make_optional<RejectedNssai>(0x11);
    ie_rejected_nssai.value().SetRejectedSNssais(nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetConfiguredNssai(
    const std::vector<struct SNSSAI_s>& nssai) {
  if (nssai.size() > 0) {
    ie_configured_nssai = std::make_optional<Nssai>(kIeiNSSAIConfigured, nssai);
  }
}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gsNetworkFeatureSupport(
    uint8_t value, uint8_t value2) {
  ie_5gs_network_feature_support =
      std::make_optional<_5gsNetworkFeatureSupport>(value, value2);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetPduSessionStatus(uint16_t value) {
  ie_pdu_session_status = std::make_optional<PduSessionStatus>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetPduSessionReactivationResult(uint16_t value) {
  ie_pdu_session_reactivation_result =
      std::make_optional<PduSessionReactivationResult>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetPduSessionReactivationResultErrorCause(
    uint8_t session_id, uint8_t value) {
  ie_pdu_session_reactivation_result_error_cause =
      std::make_optional<PduSessionReactivationResultErrorCause>(
          session_id, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetMicoIndication(bool sprti, bool raai) {
  ie_mico_indication = std::make_optional<MicoIndication>(sprti, raai);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetLadnInformation(
    const LadnInformation& ladn_information) {
  ie_ladn_information = std::make_optional<LadnInformation>(ladn_information);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetNetworkSlicingIndication(bool dcni, bool nssci) {
  ie_network_slicing_indication = std::make_optional<NetworkSlicingIndication>(
      kIeiNetworkSlicingIndication, dcni, nssci);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetServiceAreaList(
    const std::vector<service_area_list_ie_t>& list) {
  ie_service_area_list = std::make_optional<ServiceAreaList>(list);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetT3512Value(uint8_t unit, uint8_t value) {
  ie_t3512_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3512, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetNon3gppDeregistrationTimerValue(uint8_t value) {
  ie_non_3gpp_deregistration_timer_value = std::make_optional<GprsTimer2>(
      kIeiGprsTimer2Non3gppDeregistration, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetT3502Value(uint8_t value) {
  ie_t3502_value = std::make_optional<GprsTimer2>(kIeiGprsTimer2T3502, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetSorTransparentContainer(
    uint8_t header, const uint8_t (&value)[16]) {
  ie_sor_transparent_container =
      std::make_optional<SorTransparentContainer>(header, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetEapMessage(const bstring& eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetNssaiInclusionMode(uint8_t value) {
  ie_nssai_inclusion_mode = std::make_optional<NssaiInclusionMode>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::Set5gsDrxParameters(uint8_t value) {
  ie_negotiated_drx_parameters = std::make_optional<_5gsDrxParameters>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetNon3gppNwProvidedPolicies(uint8_t value) {
  ie_non_3gpp_nw_policies =
      std::make_optional<Non3gppNwProvidedPolicies>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetEpsBearerContextsStatus(uint16_t value) {
  ie_eps_bearer_context_status =
      std::make_optional<EpsBearerContextStatus>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetExtendedDrxParameters(
    uint8_t paging_time, uint8_t value) {
  ie_extended_drx_parameters =
      std::make_optional<ExtendedDrxParameters>(paging_time, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetT3447Value(uint8_t unit, uint8_t value) {
  ie_t3447_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3447, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetT3448Value(uint8_t unit, uint8_t value) {
  ie_t3448_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3448, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetT3324Value(uint8_t unit, uint8_t value) {
  ie_t3324_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3324, unit, value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetUeRadioCapabilityId(const bstring& value) {
  ie_ue_radio_capability_id = std::make_optional<UeRadioCapabilityId>(value);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetPendingNssai(
    const std::vector<struct SNSSAI_s>& nssai) {
  ie_pending_nssai = std::make_optional<Nssai>(kIeiNSSAIPending, nssai);
}

//------------------------------------------------------------------------------
void RegistrationAccept::SetTaiList(const std::vector<p_tai_t>& tai_list) {
  ie_tai_list = std::make_optional<_5gsTrackingAreaIdList>(tai_list);
}

//------------------------------------------------------------------------------
int RegistrationAccept::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationAccept message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // 5GS Registration Result
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gs_registration_result, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_5g_guti, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_tai_list, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size =
           NasHelper::Encode(ie_equivalent_plmns, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_allowed_nssai, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_rejected_nssai, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size =
           NasHelper::Encode(ie_configured_nssai, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gs_network_feature_support, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size =
           NasHelper::Encode(ie_pdu_session_status, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_pdu_session_reactivation_result, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_pdu_session_reactivation_result_error_cause, buf, len,
           encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size =
           NasHelper::Encode(ie_ladn_information, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_mico_indication, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_network_slicing_indication, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size =
           NasHelper::Encode(ie_service_area_list, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3512_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_non_3gpp_deregistration_timer_value, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3502_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_sor_transparent_container, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_eap_message, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_nssai_inclusion_mode, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_negotiated_drx_parameters, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_non_3gpp_nw_policies, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_eps_bearer_context_status, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_extended_drx_parameters, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3447_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3448_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3324_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_ue_radio_capability_id, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_pending_nssai, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  Logger::nas_mm().debug(
      "Encoded RegistrationAccept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationAccept::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationAccept message");
  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  if ((decoded_ie_size = NasHelper::Decode(
           ie_5gs_registration_result, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiMicoIndication: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiMicoIndication);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_mico_indication, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNetworkSlicingIndication: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiNetworkSlicingIndication);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_network_slicing_indication, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNssaiInclusionMode: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNssaiInclusionMode);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_nssai_inclusion_mode, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNon3gppNwProvidedPolicies: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiNon3gppNwProvidedPolicies);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_non_3gpp_nw_policies, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
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
      case kIei5gGuti: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIei5gGuti);
        if ((decoded_ie_size =
                 NasHelper::Decode(ie_5g_guti, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIAllowed: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIAllowed);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_allowed_nssai, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiRa: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRejectedNssaiRa);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_rejected_nssai, kIeiRejectedNssaiRa, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIConfigured: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIConfigured);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_configured_nssai, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsNetworkFeatureSupport: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIei5gsNetworkFeatureSupport);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_5gs_network_feature_support, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionStatus);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_pdu_session_status, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResult: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResult);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_pdu_session_reactivation_result, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResultErrorCause: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResultErrorCause);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_pdu_session_reactivation_result_error_cause, buf, len,
                 decoded_size, true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiLadnInformation: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiLadnInformation);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_ladn_information, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiServiceAreaList: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiServiceAreaList);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_service_area_list, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case kIeiGprsTimer3T3512: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3512);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3512_value, kIeiGprsTimer3T3512, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer2Non3gppDeregistration: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiGprsTimer2Non3gppDeregistration);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_non_3gpp_deregistration_timer_value,
                 kIeiGprsTimer2Non3gppDeregistration, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer2T3502: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer2T3502);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3502_value, kIeiGprsTimer2T3502, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiSorTransparentContainer: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiSorTransparentContainer);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_sor_transparent_container, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eap_message, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI (0x%x)", kIei5gsDrxParameters);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_negotiated_drx_parameters, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEpsBearerContextStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEpsBearerContextStatus);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eps_bearer_context_status, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiExtendedDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiExtendedDrxParameters);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_extended_drx_parameters, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3447: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3447);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3447_value, kIeiGprsTimer3T3447, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3448: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3448);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3448_value, kIeiGprsTimer3T3448, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3324: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3324);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3324_value, kIeiGprsTimer3T3324, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiUeRadioCapabilityId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiUeRadioCapabilityId);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_ue_radio_capability_id, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIPending: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIPending);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_pending_nssai, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kEquivalentPlmns: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kEquivalentPlmns);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_equivalent_plmns, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsTrackingAreaIdentityList: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIei5gsTrackingAreaIdentityList);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_tai_list, buf, len, decoded_size, true)) ==
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
      "Decoded RegistrationAccept message len (%d)", decoded_size);
  return decoded_size;
}
