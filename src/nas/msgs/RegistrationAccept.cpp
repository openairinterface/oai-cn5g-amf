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
  int size =
      ie_5gs_registration_result.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", _5gsRegistrationResult::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  if (!ie_5g_guti.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", _5gsMobileIdentity::GetIeName().c_str());
  } else {
    int size =
        ie_5g_guti.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gsMobileIdentity::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_tai_list.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", _5gsTrackingAreaIdList::GetIeName().c_str());
  } else {
    int size =
        ie_tai_list.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gsTrackingAreaIdList::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_equivalent_plmns.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PlmnList::GetIeName().c_str());
  } else {
    int size = ie_equivalent_plmns.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", PlmnList::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_allowed_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", Nssai::GetIeName().c_str());
  } else {
    int size =
        ie_allowed_nssai.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", Nssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_rejected_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", RejectedNssai::GetIeName().c_str());
  } else {
    int size = ie_rejected_nssai.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", RejectedNssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_configured_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", Nssai::GetIeName().c_str());
  } else {
    int size = ie_configured_nssai.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", Nssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_5gs_network_feature_support.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        _5gsNetworkFeatureSupport::GetIeName().c_str());
  } else {
    int size = ie_5gs_network_feature_support.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gsNetworkFeatureSupport::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pdu_session_status.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PduSessionStatus::GetIeName().c_str());
  } else {
    int size = ie_pdu_session_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", PduSessionStatus::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pdu_session_reactivation_result.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        PduSessionReactivationResult::GetIeName().c_str());
  } else {
    int size = ie_pdu_session_reactivation_result.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error",
          PduSessionReactivationResult::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pdu_session_reactivation_result_error_cause.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        PduSessionReactivationResultErrorCause::GetIeName().c_str());
  } else {
    int size = ie_pdu_session_reactivation_result_error_cause.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error",
          PduSessionReactivationResultErrorCause::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_ladn_information.has_value()) {
    Logger::nas_mm().warn("IE ie_ladn_information is not available");
  } else {
    int size = ie_ladn_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", LadnInformation::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_mico_indication.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", MicoIndication::GetIeName().c_str());
  } else {
    int size = ie_mico_indication.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", MicoIndication::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_network_slicing_indication.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        NetworkSlicingIndication::GetIeName().c_str());
  } else {
    int size = ie_network_slicing_indication.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", NetworkSlicingIndication::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3512_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    int size =
        ie_t3512_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_non_3gpp_deregistration_timer_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer2::GetIeName().c_str());
  } else {
    int size = ie_non_3gpp_deregistration_timer_value.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3502_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer2::GetIeName().c_str());
  } else {
    int size =
        ie_t3502_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_sor_transparent_container.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", SorTransparentContainer::GetIeName().c_str());
  } else {
    int size = ie_sor_transparent_container.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", SorTransparentContainer::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", EapMessage::GetIeName().c_str());
  } else {
    int size =
        ie_eap_message.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", EapMessage::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_nssai_inclusion_mode.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", NssaiInclusionMode::GetIeName().c_str());
  } else {
    int size = ie_nssai_inclusion_mode.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", NssaiInclusionMode::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_negotiated_drx_parameters.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", _5gsDrxParameters::GetIeName().c_str());
  } else {
    int size = ie_negotiated_drx_parameters.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gsDrxParameters::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_non_3gpp_nw_policies.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        Non3gppNwProvidedPolicies::GetIeName().c_str());
  } else {
    int size = ie_non_3gpp_nw_policies.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", Non3gppNwProvidedPolicies::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eps_bearer_context_status.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", EpsBearerContextStatus::GetIeName().c_str());
  } else {
    int size = ie_eps_bearer_context_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", EpsBearerContextStatus::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_extended_drx_parameters.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", ExtendedDrxParameters::GetIeName().c_str());
  } else {
    int size = ie_extended_drx_parameters.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", ExtendedDrxParameters::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3447_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    int size =
        ie_t3447_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3448_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    int size =
        ie_t3448_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3324_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    int size =
        ie_t3324_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_ue_radio_capability_id.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", UeRadioCapabilityId::GetIeName().c_str());
  } else {
    int size = ie_ue_radio_capability_id.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", UeRadioCapabilityId::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pending_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", Nssai::GetIeName().c_str());
  } else {
    int size =
        ie_pending_nssai.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", Nssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded RegistrationAccept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationAccept::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationAccept message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  decoded_result = ie_5gs_registration_result.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", _5gsRegistrationResult::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size(%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiMicoIndication: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiMicoIndication);
        MicoIndication ie_mico_indication_tmp = {};
        if ((decoded_result = ie_mico_indication_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", MicoIndication::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_mico_indication =
            std::optional<MicoIndication>(ie_mico_indication_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNetworkSlicingIndication: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiNetworkSlicingIndication);
        NetworkSlicingIndication ie_network_slicing_indication_tmp = {};
        if ((decoded_result = ie_network_slicing_indication_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              NetworkSlicingIndication::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_network_slicing_indication = std::optional<NetworkSlicingIndication>(
            ie_network_slicing_indication_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNssaiInclusionMode: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNssaiInclusionMode);
        NssaiInclusionMode ie_nssai_inclusion_mode_tmp = {};
        if ((decoded_result = ie_nssai_inclusion_mode_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", NssaiInclusionMode::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_nssai_inclusion_mode =
            std::optional<NssaiInclusionMode>(ie_nssai_inclusion_mode_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNon3gppNwProvidedPolicies: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiNon3gppNwProvidedPolicies);
        Non3gppNwProvidedPolicies ie_non_3gpp_nw_policies_tmp = {};
        if ((decoded_result = ie_non_3gpp_nw_policies_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              Non3gppNwProvidedPolicies::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_non_3gpp_nw_policies = std::optional<Non3gppNwProvidedPolicies>(
            ie_non_3gpp_nw_policies_tmp);
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
        _5gsMobileIdentity ie_5g_guti_tmp = {};
        if ((decoded_result = ie_5g_guti_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", _5gsMobileIdentity::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_5g_guti = std::optional<_5gsMobileIdentity>(ie_5g_guti_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIAllowed: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIAllowed);
        Nssai ie_allowed_nssai_tmp = {};
        if ((decoded_result = ie_allowed_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", Nssai::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_allowed_nssai = std::optional<Nssai>(ie_allowed_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiRa: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRejectedNssaiRa);
        RejectedNssai ie_rejected_nssai_tmp(kIeiRejectedNssaiRa);
        if ((decoded_result = ie_rejected_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", RejectedNssai::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_rejected_nssai = std::optional<RejectedNssai>(ie_rejected_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIConfigured: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIConfigured);
        Nssai ie_configured_nssai_tmp = {};
        if ((decoded_result = ie_configured_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", Nssai::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_configured_nssai = std::optional<Nssai>(ie_configured_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsNetworkFeatureSupport: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIei5gsNetworkFeatureSupport);
        _5gsNetworkFeatureSupport ie_5gs_network_feature_support_tmp = {};
        if ((decoded_result = ie_5gs_network_feature_support_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              _5gsNetworkFeatureSupport::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_5gs_network_feature_support =
            std::optional<_5gsNetworkFeatureSupport>(
                ie_5gs_network_feature_support_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionStatus);
        PduSessionStatus ie_pdu_session_status_tmp = {};
        if ((decoded_result = ie_pdu_session_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", PduSessionStatus::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_pdu_session_status =
            std::optional<PduSessionStatus>(ie_pdu_session_status_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResult: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResult);
        PduSessionReactivationResult ie_pdu_session_reactivation_result_tmp =
            {};
        if ((decoded_result = ie_pdu_session_reactivation_result_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              PduSessionReactivationResult::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_pdu_session_reactivation_result =
            std::optional<PduSessionReactivationResult>(
                ie_pdu_session_reactivation_result_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResultErrorCause: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResultErrorCause);
        PduSessionReactivationResultErrorCause
            ie_pdu_session_reactivation_result_error_cause_tmp = {};
        if ((decoded_result =
                 ie_pdu_session_reactivation_result_error_cause_tmp.Decode(
                     buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              PduSessionReactivationResultErrorCause::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_pdu_session_reactivation_result_error_cause =
            std::optional<PduSessionReactivationResultErrorCause>(
                ie_pdu_session_reactivation_result_error_cause_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case kIeiLadnInformation: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiLadnInformation);
        LadnInformation ie_ladn_information_tmp = {};
        if ((decoded_result = ie_ladn_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", LadnInformation::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_ladn_information =
            std::optional<LadnInformation>(ie_ladn_information_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;

      case kIeiGprsTimer3T3512: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3512);
        GprsTimer3 ie_t3512_value_tmp(kIeiGprsTimer3T3512);
        if ((decoded_result = ie_t3512_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer3::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_t3512_value = std::optional<GprsTimer3>(ie_t3512_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer2Non3gppDeregistration: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiGprsTimer2Non3gppDeregistration);
        GprsTimer2 ie_non_3gpp_deregistration_timer_value_tmp(
            kIeiGprsTimer2Non3gppDeregistration);
        if ((decoded_result = ie_non_3gpp_deregistration_timer_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer2::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_non_3gpp_deregistration_timer_value = std::optional<GprsTimer2>(
            ie_non_3gpp_deregistration_timer_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer2T3502: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer2T3502);
        GprsTimer2 ie_t3502_value_tmp(kIeiGprsTimer2T3502);
        if ((decoded_result = ie_t3502_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer2::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_t3502_value = std::optional<GprsTimer2>(ie_t3502_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiSorTransparentContainer: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiSorTransparentContainer);
        SorTransparentContainer ie_sor_transparent_container_tmp = {};
        if ((decoded_result = ie_sor_transparent_container_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error",
              SorTransparentContainer::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_sor_transparent_container = std::optional<SorTransparentContainer>(
            ie_sor_transparent_container_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        EapMessage ie_eap_message_tmp = {};
        if ((decoded_result = ie_eap_message_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", EapMessage::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI (0x%x)", kIei5gsDrxParameters);
        _5gsDrxParameters ie_negotiated_drx_parameters_tmp = {};
        if ((decoded_result = ie_negotiated_drx_parameters_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", _5gsDrxParameters::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_negotiated_drx_parameters =
            std::optional<_5gsDrxParameters>(ie_negotiated_drx_parameters_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEpsBearerContextStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEpsBearerContextStatus);
        EpsBearerContextStatus ie_eps_bearer_context_status_tmp = {};
        if ((decoded_result = ie_eps_bearer_context_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", EpsBearerContextStatus::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_eps_bearer_context_status = std::optional<EpsBearerContextStatus>(
            ie_eps_bearer_context_status_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiExtendedDrxParameters: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiExtendedDrxParameters);
        ExtendedDrxParameters ie_extended_drx_parameters_tmp = {};
        if ((decoded_result = ie_extended_drx_parameters_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", ExtendedDrxParameters::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_extended_drx_parameters = std::optional<ExtendedDrxParameters>(
            ie_extended_drx_parameters_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3447: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3447);
        GprsTimer3 ie_t3447_value_tmp(kIeiGprsTimer3T3447);
        if ((decoded_result = ie_t3447_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer3::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_t3447_value = std::optional<GprsTimer3>(ie_t3447_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3448: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3448);
        GprsTimer3 ie_t3448_value_tmp(kIeiGprsTimer3T3448);
        if ((decoded_result = ie_t3448_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer3::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_t3448_value = std::optional<GprsTimer3>(ie_t3448_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3T3324: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3324);
        GprsTimer3 ie_t3324_value_tmp(kIeiGprsTimer3T3324);
        if ((decoded_result = ie_t3324_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer3::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_t3324_value = std::optional<GprsTimer3>(ie_t3324_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiUeRadioCapabilityId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiUeRadioCapabilityId);
        UeRadioCapabilityId ie_ue_radio_capability_id_tmp = {};
        if ((decoded_result = ie_ue_radio_capability_id_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", UeRadioCapabilityId::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_ue_radio_capability_id =
            std::optional<UeRadioCapabilityId>(ie_ue_radio_capability_id_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiNSSAIPending: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiNSSAIPending);
        Nssai ie_pending_nssai_tmp = {};
        if ((decoded_result = ie_pending_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", Nssai::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_pending_nssai = std::optional<Nssai>(ie_pending_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kEquivalentPlmns: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kEquivalentPlmns);
        PlmnList ie_equivalent_plmns_tmp = {};
        if ((decoded_result = ie_equivalent_plmns_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", PlmnList::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_equivalent_plmns = std::optional<PlmnList>(ie_equivalent_plmns_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gsTrackingAreaIdentityList: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIei5gsTrackingAreaIdentityList);
        _5gsTrackingAreaIdList ie_tai_list_tmp = {};
        if ((decoded_result = ie_tai_list_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", _5gsTrackingAreaIdList::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_tai_list = std::optional<_5gsTrackingAreaIdList>(ie_tai_list_tmp);
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
