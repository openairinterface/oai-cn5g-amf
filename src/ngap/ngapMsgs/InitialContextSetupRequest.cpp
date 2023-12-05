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
#include "conversions.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::InitialContextSetupRequestMsg()
    : NgapUEMessage() {
  initialContextSetupRequestIEs               = nullptr;
  oldAMF                                      = std::nullopt;
  uEAggregateMaxBitRate                       = std::nullopt;
  coreNetworkAssistanceInformationForInactive = std::nullopt;
  pduSessionResourceSetupRequestList          = std::nullopt;
  ueRadioCapability                           = std::nullopt;
  maskedIMEISV                                = std::nullopt;
  nasPdu                                      = std::nullopt;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupRequestMsg::~InitialContextSetupRequestMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::initialize() {
  initialContextSetupRequestIEs = &(ngapPdu->choice.initiatingMessage->value
                                        .choice.InitialContextSetupRequest);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setOldAmf(const std::string& name) {
  AmfName tmp = {};
  tmp.setValue(name);
  oldAMF = std::optional<AmfName>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AMFName;

  int ret = oldAMF.value().encode(ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getOldAmf(std::string& name) {
  if (!oldAMF.has_value()) return false;
  oldAMF.value().getValue(name);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUEAggregateMaxBitRate(
    const uint64_t& bit_rate_downlink, const uint64_t& bit_rate_uplink) {
  UEAggregateMaxBitRate tmp = {};
  tmp.set(bit_rate_downlink, bit_rate_uplink);
  uEAggregateMaxBitRate = std::optional<UEAggregateMaxBitRate>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaxBitRate IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UEAggregateMaxBitRate IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUEAggregateMaxBitRate(
    uint64_t& bit_rate_downlink, uint64_t& bit_rate_uplink) {
  if (!uEAggregateMaxBitRate.has_value()) return false;
  uEAggregateMaxBitRate.value().get(bit_rate_downlink, bit_rate_uplink);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUEAggregateMaxBitRate(
    const UEAggregateMaxBitRate& bit_rate) {
  uEAggregateMaxBitRate = std::make_optional<UEAggregateMaxBitRate>(bit_rate);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaximumBitRate IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaximumBitRate IE error");

  return;
};

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUEAggregateMaxBitRate(
    UEAggregateMaxBitRate& bit_rate) {
  if (!uEAggregateMaxBitRate.has_value()) return false;
  bit_rate = uEAggregateMaxBitRate.value();
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setCoreNetworkAssistanceInfo(
    const uint16_t& ue_identity_index_value_value,
    const e_Ngap_PagingDRX& ue_specific_drx_value,
    const uint8_t& periodic_reg_update_timer_value,
    const bool& mico_mode_ind_value,
    const std::vector<Tai_t>& tai_list_for_rrc_inactive) {
  CoreNetworkAssistanceInformationForInactive tmp = {};

  UEIdentityIndexValue ue_identity_index_value = {};
  ue_identity_index_value.set(ue_identity_index_value_value);
  DefaultPagingDrx paging_drx = {};
  paging_drx.setValue(ue_specific_drx_value);
  PeriodicRegistrationUpdateTimer periodic_reg_update_timer = {};
  periodic_reg_update_timer.set(periodic_reg_update_timer_value);

  std::vector<TAI> tai_list;
  for (int i = 0; i < tai_list_for_rrc_inactive.size(); i++) {
    TAI tai = {};
    tai.setTAI(
        tai_list_for_rrc_inactive[i].mcc, tai_list_for_rrc_inactive[i].mnc,
        tai_list_for_rrc_inactive[i].tac);
    tai_list.push_back(tai);
  }

  tmp.set(
      ue_identity_index_value, paging_drx, periodic_reg_update_timer,
      mico_mode_ind_value, tai_list);

  coreNetworkAssistanceInformationForInactive =
      std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive;

  int ret = coreNetworkAssistanceInformationForInactive.value().encode(
      ie->value.choice.CoreNetworkAssistanceInformationForInactive);
  if (!ret) {
    Logger::ngap().error(
        "Encode CoreNetworkAssistanceInformationForInactive IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode CoreNetworkAssistanceInformation IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getCoreNetworkAssistanceInfo(
    uint16_t& ue_identity_index_value_value, int& ue_specific_drx_value,
    uint8_t& periodic_reg_update_timer_value, bool& mico_mode_ind_value,
    std::vector<Tai_t>& tai_list_for_rrc_inactive) {
  if (!coreNetworkAssistanceInformationForInactive.has_value()) return false;
  UEIdentityIndexValue ue_identity_index_value              = {};
  std::optional<DefaultPagingDrx> paging_drx                = std::nullopt;
  PeriodicRegistrationUpdateTimer periodic_reg_update_timer = {};

  std::vector<TAI> tai_list;

  coreNetworkAssistanceInformationForInactive.value().get(
      ue_identity_index_value, paging_drx, periodic_reg_update_timer,
      mico_mode_ind_value, tai_list);
  ue_identity_index_value.get(ue_identity_index_value_value);
  if (paging_drx)
    ue_specific_drx_value = paging_drx->getValue();
  else
    ue_specific_drx_value = -1;

  periodic_reg_update_timer.get(periodic_reg_update_timer_value);

  for (std::vector<TAI>::iterator it = std::begin(tai_list);
       it < std::end(tai_list); ++it) {
    Tai_t tai = {};
    it->getTAI(tai);
    tai_list_for_rrc_inactive.push_back(tai);
  }
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setGuami(const Guami_t& value) {
  guami.setGUAMI(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI;

  int ret = guami.encode(ie->value.choice.GUAMI);
  if (!ret) {
    Logger::ngap().error("Encode GUAMI IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode GUAMI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getGuami(Guami_t& value) {
  guami.getGUAMI(
      value.mcc, value.mnc, value.region_id, value.amf_set_id,
      value.amf_pointer);
  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  PduSessionResourceSetupListCxtReq list_cxt_req = {};

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupItemCxtReqList.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemCxtReq pduSessionResourceSetupItemCxtReq = {};
    PduSessionId pDUSessionID                                           = {};
    pDUSessionID.set(list[i].pduSessionId);
    std::optional<NasPdu> nAS_PDU = std::nullopt;

    if (conv::check_bstring(list[i].nas_pdu)) {
      NasPdu tmp = {};
      tmp.set(list[i].nas_pdu);
      nAS_PDU = std::optional<NasPdu>(tmp);
    }
    S_NSSAI s_NSSAI = {};
    s_NSSAI.setSst(list[i].s_nssai.sst);
    if (list[i].s_nssai.sd.size()) s_NSSAI.setSd(list[i].s_nssai.sd);
    pduSessionResourceSetupItemCxtReq.set(
        pDUSessionID, nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    pduSessionResourceSetupItemCxtReqList.push_back(
        pduSessionResourceSetupItemCxtReq);
  }

  list_cxt_req.set(pduSessionResourceSetupItemCxtReqList);
  pduSessionResourceSetupRequestList =
      std::optional<PduSessionResourceSetupListCxtReq>(list_cxt_req);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

  int ret = pduSessionResourceSetupRequestList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListCxtReq);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtReq IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  if (!pduSessionResourceSetupRequestList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
  pduSessionResourceSetupRequestList.value().get(
      pduSessionResourceSetupItemCxtReqList);

  for (std::vector<PduSessionResourceSetupItemCxtReq>::iterator it =
           std::begin(pduSessionResourceSetupItemCxtReqList);
       it < std::end(pduSessionResourceSetupItemCxtReqList); ++it) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PduSessionId pDUSessionID     = {};
    std::optional<NasPdu> nAS_PDU = std::nullopt;
    S_NSSAI s_NSSAI               = {};
    it->get(
        pDUSessionID, nAS_PDU, s_NSSAI,
        request.pduSessionResourceSetupRequestTransfer);
    pDUSessionID.get(request.pduSessionId);
    s_NSSAI.getSst(request.s_nssai.sst);
    s_NSSAI.getSd(request.s_nssai.sd);
    if (nAS_PDU.has_value()) {
      nAS_PDU.value().get(request.nas_pdu);
    }
    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setAllowedNssai(
    const std::vector<S_Nssai>& list) {
  std::vector<S_NSSAI> snssaiList;

  for (int i = 0; i < list.size(); i++) {
    S_NSSAI snssai = {};
    snssai.setSst(list[i].sst);
    uint32_t sd = SD_NO_VALUE;
    if (!list[i].sd.empty()) {
      conv::sd_string_to_int(list[i].sd, sd);
    }
    snssai.setSd(sd);
    snssaiList.push_back(snssai);
  }
  allowedNssai.set(snssaiList);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

  int ret = allowedNssai.encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getAllowedNssai(
    std::vector<S_Nssai>& list) {
  std::vector<S_NSSAI> snssaiList;
  allowedNssai.get(snssaiList);
  for (std::vector<S_NSSAI>::iterator it = std::begin(snssaiList);
       it < std::end(snssaiList); ++it) {
    S_Nssai s_nssai = {};
    it->getSst(s_nssai.sst);
    it->getSd(s_nssai.sd);
    list.push_back(s_nssai);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUESecurityCapability(
    const uint16_t& nr_encryption_algs,
    const uint16_t& nr_integrity_protection_algs,
    const uint16_t& e_utra_encryption_algs,
    const uint16_t& e_utra_integrity_protection_algs) {
  uESecurityCapabilities.set(
      nr_encryption_algs, nr_integrity_protection_algs, e_utra_encryption_algs,
      e_utra_integrity_protection_algs);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

  int ret =
      uESecurityCapabilities.encode(ie->value.choice.UESecurityCapabilities);
  if (!ret) {
    Logger::ngap().error("Encode UESecurityCapabilities IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UESecurityCapabilities IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getUESecurityCapability(
    uint16_t& nr_encryption_algs, uint16_t& nr_integrity_protection_algs,
    uint16_t& e_utra_encryption_algs,
    uint16_t& e_utra_integrity_protection_algs) {
  if (!uESecurityCapabilities.get(
          nr_encryption_algs, nr_integrity_protection_algs,
          e_utra_encryption_algs, e_utra_integrity_protection_algs))
    return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setSecurityKey(uint8_t* key) {
  securityKey.setSecurityKey(key);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityKey;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey;

  int ret = securityKey.encode(ie->value.choice.SecurityKey);
  if (!ret) {
    Logger::ngap().error("Encode SecurityKey IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityKey IE error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getSecurityKey(uint8_t*& key) {
  if (!securityKey.getSecurityKey(key)) return false;

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMobilityRestrictionList(
    const PlmnId& plmn_id) {
  MobilityRestrictionList tmp = {};
  tmp.setPLMN(plmn_id);
  mobilityRestrictionList = std::optional<MobilityRestrictionList>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;

  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_MobilityRestrictionList;

  int ret = mobilityRestrictionList.value().encode(
      ie->value.choice.MobilityRestrictionList);
  if (!ret) {
    Logger::ngap().error("Encode MobilityRestrictionList IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode MobilityRestrictionList IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  nasPdu = std::optional<NasPdu>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

  int ret = nasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU error!");
}

//------------------------------------------------------------------------------
bool InitialContextSetupRequestMsg::getNasPdu(bstring& pdu) {
  if (!nasPdu.has_value()) return false;
  return nasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setUERadioCapability(
    const bstring& ue_radio_capability) {
  UERadioCapability tmp = {};
  tmp.set(ue_radio_capability);
  ueRadioCapability = std::optional<UERadioCapability>(tmp);

  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

  int ret =
      ueRadioCapability.value().encode(ie->value.choice.UERadioCapability);
  if (!ret) {
    Logger::ngap().error("Encode UERadioCapability IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UERadioCapability IE error!");
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::getUERadioCapability(
    bstring& ue_radio_capability) {
  if (!ueRadioCapability.has_value()) return;
  ueRadioCapability.value().get(ue_radio_capability);
}

//------------------------------------------------------------------------------
void InitialContextSetupRequestMsg::setMaskedIMEISV(const std::string& imeisv) {
  Ngap_InitialContextSetupRequestIEs_t* ie =
      (Ngap_InitialContextSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MaskedIMEISV;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

  if (!conv::string_2_masked_imeisv(imeisv, ie->value.choice.MaskedIMEISV)) {
    Logger::ngap().error("Encode MaskedIMEISV IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  int ret =
      ASN_SEQUENCE_ADD(&initialContextSetupRequestIEs->protocolIEs.list, ie);
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
      initialContextSetupRequestIEs = &ngapPdu->choice.initiatingMessage->value
                                           .choice.InitialContextSetupRequest;
    } else {
      Logger::ngap().error("Check InitialContextSetupRequest message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < initialContextSetupRequestIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AMFName) {
          AmfName tmp = {};
          if (!tmp.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.AMFName)) {
            Logger::ngap().error("Decoded NGAP OldAMFName IE error");
            return false;
          }
          oldAMF = std::optional<AmfName>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          UEAggregateMaxBitRate tmp = {};
          if (!tmp.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decoded NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
          uEAggregateMaxBitRate = std::optional<UEAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CoreNetworkAssistanceInformationForInactive: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_CoreNetworkAssistanceInformationForInactive) {
          CoreNetworkAssistanceInformationForInactive tmp = {};

          if (!tmp.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice
                      .CoreNetworkAssistanceInformationForInactive)) {
            Logger::ngap().error(
                "Decoded NGAP CoreNetworkAssistanceInformationForInactive IE "
                "error");
            return false;
          }
          coreNetworkAssistanceInformationForInactive =
              std::optional<CoreNetworkAssistanceInformationForInactive>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP CoreNetworkAssistanceInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_GUAMI: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_GUAMI) {
          if (!guami.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq) {
          PduSessionResourceSetupListCxtReq tmp = {};
          if (!tmp.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceSetupListCxtReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
            return false;
          }
          pduSessionResourceSetupRequestList =
              std::optional<PduSessionResourceSetupListCxtReq>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI) {
          if (!allowedNssai.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities) {
          if (!uESecurityCapabilities.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_SecurityKey) {
          if (!securityKey.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupRequestIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(
                  initialContextSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NasPdu IE error");
            return false;
          }
          nasPdu = std::optional<NasPdu>(tmp);
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
