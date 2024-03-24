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

#include "InitialContextSetupRequest.hpp"

#include "amf.hpp"
#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::InitialContextSetupRequestMsg()
    : NgapUeMessage() {
  m_InitialContextSetupRequestIes               = nullptr;
  m_OldAmf                                      = std::nullopt;
  m_UeAggregateMaxBitRate                       = std::nullopt;
  m_CoreNetworkAssistanceInformationForInactive = std::nullopt;
  m_PduSessionResourceSetupRequestList          = std::nullopt;
  m_UeRadioCapability                           = std::nullopt;
  m_MaskedImeiSv                                = std::nullopt;
  m_NasPdu                                      = std::nullopt;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::~InitialContextSetupRequestMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::initialize() {
  m_InitialContextSetupRequestIes = &(ngapPdu->choice.initiatingMessage->value
                                          .choice.InitialContextSetupRequest);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setOldAmf(const std::string& name) {
  AmfName tmp = {};
  tmp.set(name);
  m_OldAmf = std::optional<AmfName>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AMFName;

  int ret = m_OldAmf.value().encode(ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getOldAmf(std::string& name) const {
  if (!m_OldAmf.has_value()) return false;
  m_OldAmf.value().get(name);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeAggregateMaxBitRate(
    const uint64_t& bitRateDl, const uint64_t& bitRateUl) {
  UeAggregateMaxBitRate tmp = {};
  tmp.set(bitRateDl, bitRateUl);
  m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UeAggregateMaxBitRate IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UeAggregateMaxBitRate IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeAggregateMaxBitRate(
    uint64_t& bitRateDl, uint64_t& bitRateUl) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  m_UeAggregateMaxBitRate.value().get(bitRateDl, bitRateUl);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeAggregateMaxBitRate(
    const UeAggregateMaxBitRate& bitRate) {
  m_UeAggregateMaxBitRate = std::make_optional<UeAggregateMaxBitRate>(bitRate);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaximumBitRate IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaximumBitRate IE error");

  return;
};

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeAggregateMaxBitRate(
    UeAggregateMaxBitRate& bitRate) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  bitRate = m_UeAggregateMaxBitRate.value();
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setCoreNetworkAssistanceInfo(
    uint16_t ueIdentityIndexValue_Value,
    e_Ngap_PagingDRX defaultPagingDrx_Value,
    uint8_t periodicRegistrationUpdateTimer_Value, bool micoModeIndValue,
    const std::vector<Tai_t>& taiListForRrcInactive) {
  CoreNetworkAssistanceInformationForInactive tmp = {};

  UeIdentityIndexValue ueIdentityIndexValue = {};
  ueIdentityIndexValue.set(ueIdentityIndexValue_Value);
  DefaultPagingDrx defaultPagingDrx = {};
  defaultPagingDrx.set(defaultPagingDrx_Value);
  PeriodicRegistrationUpdateTimer periodicRegistrationUpdateTimer = {};
  periodicRegistrationUpdateTimer.set(periodicRegistrationUpdateTimer_Value);

  std::vector<Tai> taiList;
  for (int i = 0; i < taiListForRrcInactive.size(); i++) {
    Tai tai = {};
    tai.set(
        taiListForRrcInactive[i].mcc, taiListForRrcInactive[i].mnc,
        taiListForRrcInactive[i].tac);
    taiList.push_back(tai);
  }

  tmp.set(
      ueIdentityIndexValue, defaultPagingDrx, periodicRegistrationUpdateTimer,
      micoModeIndValue, taiList);

  m_CoreNetworkAssistanceInformationForInactive =
      std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive;

  int ret = m_CoreNetworkAssistanceInformationForInactive.value().encode(
      ie->value.choice.CoreNetworkAssistanceInformationForInactive);
  if (!ret) {
    Logger::ngap().error(
        "Encode CoreNetworkAssistanceInformationForInactive IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode CoreNetworkAssistanceInformation IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getCoreNetworkAssistanceInfo(
    uint16_t& ueIdentityIndexValue_Value, int& defaultPagingDrx_Value,
    uint8_t& periodicRegistrationUpdateTimer_Value, bool& micoModeIndValue,
    std::vector<Tai_t>& taiListForRrcInactive) const {
  if (!m_CoreNetworkAssistanceInformationForInactive.has_value()) return false;
  UeIdentityIndexValue ueIdentityIndexValue        = {};
  std::optional<DefaultPagingDrx> defaultPagingDrx = std::nullopt;
  PeriodicRegistrationUpdateTimer periodicRegistrationUpdateTimer = {};

  std::vector<Tai> taiList;

  m_CoreNetworkAssistanceInformationForInactive.value().get(
      ueIdentityIndexValue, defaultPagingDrx, periodicRegistrationUpdateTimer,
      micoModeIndValue, taiList);
  ueIdentityIndexValue.get(ueIdentityIndexValue_Value);
  if (defaultPagingDrx)
    defaultPagingDrx_Value = defaultPagingDrx->get();
  else
    defaultPagingDrx_Value = -1;

  periodicRegistrationUpdateTimer.get(periodicRegistrationUpdateTimer_Value);

  for (std::vector<Tai>::iterator it = std::begin(taiList);
       it < std::end(taiList); ++it) {
    Tai_t tai = {};
    it->get(tai);
    taiListForRrcInactive.push_back(tai);
  }
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setGuami(const guami_full_format_t& value) {
  m_Guami.set(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI;

  int ret = m_Guami.encode(ie->value.choice.GUAMI);
  if (!ret) {
    Logger::ngap().error("Encode GUAMI IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode GUAMI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getGuami(guami_full_format_t& value) const {
  m_Guami.get(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  PduSessionResourceSetupListCxtReq cxtReqList = {};

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupItemCxtReqList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemCxtReq pduSessionResourceSetupItemCxtReq = {};
    PduSessionId pDUSessionID                                           = {};
    pDUSessionID.set(list[i].pduSessionId);
    std::optional<NasPdu> nAS_PDU = std::nullopt;

    if (amf_conv::check_bstring(list[i].nasPdu)) {
      NasPdu tmp = {};
      tmp.set(list[i].nasPdu);
      nAS_PDU = std::optional<NasPdu>(tmp);
    }
    SNssai s_NSSAI = {};
    s_NSSAI.setSst(list[i].sNssai.sst);
    if (list[i].sNssai.sd.size()) s_NSSAI.setSd(list[i].sNssai.sd);
    pduSessionResourceSetupItemCxtReq.set(
        pDUSessionID, nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    pduSessionResourceSetupItemCxtReqList.push_back(
        pduSessionResourceSetupItemCxtReq);
  }

  cxtReqList.set(pduSessionResourceSetupItemCxtReqList);
  m_PduSessionResourceSetupRequestList =
      std::optional<PduSessionResourceSetupListCxtReq>(cxtReqList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

  int ret = m_PduSessionResourceSetupRequestList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListCxtReq);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) const {
  if (!m_PduSessionResourceSetupRequestList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  m_PduSessionResourceSetupRequestList.value().get(
      pduSessionResourceSetupItemCxtReqList);

  for (std::vector<PduSessionResourceSetupItemCxtReq>::iterator it =
           std::begin(pduSessionResourceSetupItemCxtReqList);
       it < std::end(pduSessionResourceSetupItemCxtReqList); ++it) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PduSessionId pDUSessionID     = {};
    std::optional<NasPdu> nAS_PDU = std::nullopt;
    SNssai s_NSSAI                = {};
    it->get(
        pDUSessionID, nAS_PDU, s_NSSAI,
        request.pduSessionResourceSetupRequestTransfer);
    pDUSessionID.get(request.pduSessionId);
    s_NSSAI.getSst(request.sNssai.sst);
    s_NSSAI.getSd(request.sNssai.sd);
    if (nAS_PDU.has_value()) {
      nAS_PDU.value().get(request.nasPdu);
    }
    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAllowedNssai(
    const std::vector<S_Nssai>& list) {
  std::vector<SNssai> snssaiList;

  for (int i = 0; i < list.size(); i++) {
    SNssai snssai = {};
    snssai.setSst(list[i].sst);
    uint32_t sd = SD_NO_VALUE;
    if (!list[i].sd.empty()) {
      amf_conv::sd_string_to_int(list[i].sd, sd);
    }
    snssai.setSd(sd);
    snssaiList.push_back(snssai);
  }
  m_AllowedNssai.set(snssaiList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

  int ret = m_AllowedNssai.encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getAllowedNssai(
    std::vector<S_Nssai>& list) const {
  std::vector<SNssai> snssaiList;
  m_AllowedNssai.get(snssaiList);
  for (std::vector<SNssai>::iterator it = std::begin(snssaiList);
       it < std::end(snssaiList); ++it) {
    S_Nssai s_nssai = {};
    it->getSst(s_nssai.sst);
    it->getSd(s_nssai.sd);
    list.push_back(s_nssai);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeSecurityCapability(
    uint16_t nrEncryptionAlgs, uint16_t nrIntegrityProtectionAlgs,
    uint16_t eutraEncryptionAlgs, uint16_t eutraIntegrityProtectionAlgs) {
  m_UeSecurityCapabilities.set(
      nrEncryptionAlgs, nrIntegrityProtectionAlgs, eutraEncryptionAlgs,
      eutraIntegrityProtectionAlgs);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

  int ret =
      m_UeSecurityCapabilities.encode(ie->value.choice.UESecurityCapabilities);
  if (!ret) {
    Logger::ngap().error("Encode UESecurityCapabilities IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UESecurityCapabilities IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUeSecurityCapability(
    uint16_t& nrEncryptionAlgs, uint16_t& nrIntegrityProtectionAlgs,
    uint16_t& eutraEncryptionAlgs,
    uint16_t& eutraIntegrityProtectionAlgs) const {
  if (!m_UeSecurityCapabilities.get(
          nrEncryptionAlgs, nrIntegrityProtectionAlgs, eutraEncryptionAlgs,
          eutraIntegrityProtectionAlgs))
    return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setSecurityKey(uint8_t* key) {
  m_SecurityKey.set(key);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityKey;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey;

  int ret = m_SecurityKey.encode(ie->value.choice.SecurityKey);
  if (!ret) {
    Logger::ngap().error("Encode SecurityKey IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityKey IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getSecurityKey(uint8_t*& key) const {
  if (!m_SecurityKey.get(key)) return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMobilityRestrictionList(
    const PlmnId& plmn_id) {
  MobilityRestrictionList tmp = {};
  tmp.setPlmn(plmn_id);
  m_MobilityRestrictionList = std::optional<MobilityRestrictionList>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;

  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_MobilityRestrictionList;

  int ret = m_MobilityRestrictionList.value().encode(
      ie->value.choice.MobilityRestrictionList);
  if (!ret) {
    Logger::ngap().error("Encode MobilityRestrictionList IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode MobilityRestrictionList IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  m_NasPdu = std::optional<NasPdu>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getNasPdu(bstring& pdu) const {
  if (!m_NasPdu.has_value()) return false;
  return m_NasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUeRadioCapability(
    const bstring& ue_radio_capability) {
  UeRadioCapability tmp = {};
  tmp.set(ue_radio_capability);
  m_UeRadioCapability = std::optional<UeRadioCapability>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

  int ret =
      m_UeRadioCapability.value().encode(ie->value.choice.UERadioCapability);
  if (!ret) {
    Logger::ngap().error("Encode UERadioCapability IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UERadioCapability IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::getUeRadioCapability(
    bstring& ue_radio_capability) const {
  if (!m_UeRadioCapability.has_value()) return;
  m_UeRadioCapability.value().get(ue_radio_capability);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMaskedImeisv(const std::string& imeisv) {
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MaskedIMEISV;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

  if (!amf_conv::string_2_masked_imeisv(
          imeisv, ie->value.choice.MaskedIMEISV)) {
    Logger::ngap().error("Encode MaskedIMEISV IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&m_InitialContextSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode MaskedIMEISV IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialContextSetupRequest) {
      m_InitialContextSetupRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice
               .InitialContextSetupRequest;
    } else {
      Logger::ngap().error("Check InitialContextSetupRequest message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < m_InitialContextSetupRequestIes->protocolIEs.list.count;
       i++) {
    switch (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_OldAMF: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMFName) {
          AmfName tmp = {};
          if (!tmp.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.AMFName)) {
            Logger::ngap().error("Decoded NGAP OldAMFName IE error");
            return false;
          }
          m_OldAmf = std::optional<AmfName>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          UeAggregateMaxBitRate tmp = {};
          if (!tmp.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decoded NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
          m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive) {
          CoreNetworkAssistanceInformationForInactive tmp = {};

          if (!tmp.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice
                      .CoreNetworkAssistanceInformationForInactive)) {
            Logger::ngap().error(
                "Decoded NGAP CoreNetworkAssistanceInformationForInactive IE "
                "error");
            return false;
          }
          m_CoreNetworkAssistanceInformationForInactive =
              std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP CoreNetworkAssistanceInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_GUAMI: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI) {
          if (!m_Guami.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.GUAMI)) {
            Logger::ngap().error("Decoded NGAP GUAMI IE error");

            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP GUAMI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq) {
          PduSessionResourceSetupListCxtReq tmp = {};
          if (!tmp.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceSetupListCxtReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
            return false;
          }
          m_PduSessionResourceSetupRequestList =
              std::optional<PduSessionResourceSetupListCxtReq>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI) {
          if (!m_AllowedNssai.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UESecurityCapabilities: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities) {
          if (!m_UeSecurityCapabilities.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.UESecurityCapabilities)) {
            Logger::ngap().error(
                "Decoded NGAP UESecurityCapabilities IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UESecurityCapabilities IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityKey: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey) {
          if (!m_SecurityKey.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.SecurityKey)) {
            Logger::ngap().error("Decoded NGAP SecurityKey IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP SecurityKey IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(
                  m_InitialContextSetupRequestIes->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NasPdu IE error");
            return false;
          }
          m_NasPdu = std::optional<NasPdu>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP Message PDU error");
        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
