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

#include "NgSetupResponse.hpp"

#include "amf.hpp"
#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NgSetupResponseMsg::NgSetupResponseMsg() : NgapMessage() {
  m_NgSetupResponsIes = nullptr;
  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_RESPONSE);
  initialize();
  // criticalityDiagnostics = nullptr;
}

//------------------------------------------------------------------------------
NgSetupResponseMsg::~NgSetupResponseMsg() {}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::initialize() {
  m_NgSetupResponsIes =
      &(ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse);
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::setAmfName(const std::string& name) {
  if (!m_AmfName.set(name)) {
    Logger::ngap().warn("Not a valid AMF Name value!");
    return false;
  }

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFName;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_AMFName;

  if (!m_AmfName.encode(ie->value.choice.AMFName)) {
    Logger::ngap().error("Encode NGAP AMFName IE error");
    utils::free_wrapper((void**) &ie);
    return false;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs.list, ie);
  if (ret != 0) {
    Logger::ngap().error("Encode NGAP AMFName IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setGuamiList(std::vector<struct GuamiItem_s>& list) {
  ServedGuamiItem servedGUAMIItem = {};
  for (int i = 0; i < list.size(); i++) {
    Guami guami = {};
    guami.set(
        list[i].mcc, list[i].mnc, list[i].regionId, list[i].amfSetId,
        list[i].amfPointer);
    servedGUAMIItem.setGuami(guami);

    if (list[i].backupAmfName.size() > 0) {
      AmfName amf_name = {};
      if (amf_name.set(list[i].backupAmfName)) {
        servedGUAMIItem.setBackupAmfName(amf_name);
      }
    }
    m_ServedGuamiList.addItem(servedGUAMIItem);
  }

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ServedGUAMIList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList;

  if (!m_ServedGuamiList.encode(ie->value.choice.ServedGUAMIList)) {
    Logger::ngap().error("Encode NGAP ServedGUAMIList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP ServedGUAMIList IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setRelativeAmfCapacity(const long& capacity) {
  m_RelativeAmfCapacity.set(capacity);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RelativeAMFCapacity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

  if (!m_RelativeAmfCapacity.encode(ie->value.choice.RelativeAMFCapacity)) {
    Logger::ngap().error("Encode NGAP RelativeAMFCapacity IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP RelativeAMFCapacity IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setPlmnSupportList(
    const std::vector<PlmnSliceSupport_t>& list) {
  std::vector<PlmnSupportItem> plmnSupportItems;

  for (int i = 0; i < list.size(); i++) {
    PlmnSupportItem plmnSupportItem = {};
    PlmnId plmn                     = {};
    plmn.set(list[i].mcc, list[i].mnc);
    Logger::ngap().debug(
        "MCC %s, MNC %s", list[i].mcc.c_str(), list[i].mnc.c_str());

    std::vector<SNssai> snssais;
    for (int j = 0; j < list[i].sliceList.size(); j++) {
      SNssai snssai = {};
      snssai.setSst(list[i].sliceList[j].sst);

      uint32_t sd = SD_NO_VALUE;
      if (!list[i].sliceList[j].sd.empty()) {
        amf_conv::sd_string_to_int(list[i].sliceList[j].sd, sd);
      }
      snssai.setSd(sd);
      snssais.push_back(snssai);
    }
    plmnSupportItem.set(plmn, snssais);
    plmnSupportItems.push_back(plmnSupportItem);
  }

  m_PlmnSupportList.set(plmnSupportItems);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_PLMNSupportList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList;

  if (!m_PlmnSupportList.encode(ie->value.choice.PLMNSupportList)) {
    Logger::ngap().error("Encode NGAP PLMNSupportList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP PLMNSupportList IE error");
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGSetupResponse) {
      m_NgSetupResponsIes =
          &ngapPdu->choice.successfulOutcome->value.choice.NGSetupResponse;
    } else {
      Logger::ngap().error("Check NGSetupResponse message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "MessageType error, ngapPdu->present %d", ngapPdu->present);
    return false;
  }
  for (int i = 0; i < m_NgSetupResponsIes->protocolIEs.list.count; i++) {
    switch (m_NgSetupResponsIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMFName: {
        if (m_NgSetupResponsIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_NgSetupResponsIes->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_AMFName) {
          if (!m_AmfName.decode(m_NgSetupResponsIes->protocolIEs.list.array[i]
                                    ->value.choice.AMFName)) {
            Logger::ngap().error("Decoded NGAP AMFName error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMFName error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_ServedGUAMIList: {
        if (m_NgSetupResponsIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_NgSetupResponsIes->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList) {
          if (!m_ServedGuamiList.decode(
                  m_NgSetupResponsIes->protocolIEs.list.array[i]
                      ->value.choice.ServedGUAMIList)) {
            Logger::ngap().error("Decoded NGAP ServedGUAMIList error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP ServedGUAMIList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity: {
        if (m_NgSetupResponsIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_NgSetupResponsIes->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity) {
          if (!m_RelativeAmfCapacity.decode(
                  m_NgSetupResponsIes->protocolIEs.list.array[i]
                      ->value.choice.RelativeAMFCapacity)) {
            Logger::ngap().error("Decoded NGAP RelativeAMFCapacity error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RelativeAMFCapacity error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PLMNSupportList: {
        if (m_NgSetupResponsIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_NgSetupResponsIes->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList) {
          if (!m_PlmnSupportList.decode(
                  m_NgSetupResponsIes->protocolIEs.list.array[i]
                      ->value.choice.PLMNSupportList)) {
            Logger::ngap().error("Decoded NGAP PLMNSupportList error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP PLMNSupportList error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERetentionInformation: {
        if (m_NgSetupResponsIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_NgSetupResponsIes->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_UERetentionInformation) {
          UeRetentionInformation tmp = {};
          if (!tmp.decode(m_NgSetupResponsIes->protocolIEs.list.array[i]
                              ->value.choice.UERetentionInformation)) {
            Logger::ngap().error(
                "Decoded NGAP UeRetentionInformation IE error");
            return false;
          }
          m_UeRetentionInformation =
              std::make_optional<UeRetentionInformation>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP UeRetentionInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        Logger::ngap().debug("Decoded NGAP CriticalityDiagnostics");
      } break;
      default: {
        Logger::ngap().error("Decoded NGAP Message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::getAmfName(std::string& name) const {
  m_AmfName.get(name);
  return true;
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::getGuamiList(
    std::vector<struct GuamiItem_s>& list) const {
  std::vector<ServedGuamiItem> servedGUAMIItems;
  m_ServedGuamiList.get(servedGUAMIItems);

  for (std::vector<ServedGuamiItem>::iterator it = std::begin(servedGUAMIItems);
       it != std::end(servedGUAMIItems); ++it) {
    GuamiItem_t guamiItem = {};
    Guami guami           = {};
    it->getGuami(guami);
    guami.get(
        guamiItem.mcc, guamiItem.mnc, guamiItem.regionId, guamiItem.amfSetId,
        guamiItem.amfPointer);

    AmfName amf_name = {};
    if (it->getBackupAmfName(amf_name)) {
      amf_name.get(guamiItem.backupAmfName);
    }

    list.push_back(guamiItem);
  }

  return true;
}

//------------------------------------------------------------------------------
long NgSetupResponseMsg::getRelativeAmfCapacity() const {
  return m_RelativeAmfCapacity.get();
}

//------------------------------------------------------------------------------
bool NgSetupResponseMsg::getPlmnSupportList(
    std::vector<PlmnSliceSupport_t>& list) const {
  std::vector<PlmnSupportItem> plmnsupportItemItems;
  m_PlmnSupportList.get(plmnsupportItemItems);

  for (std::vector<PlmnSupportItem>::iterator it =
           std::begin(plmnsupportItemItems);
       it < std::end(plmnsupportItemItems); ++it) {
    PlmnSliceSupport_t plmnSliceSupport = {};
    PlmnId plmn                         = {};
    std::vector<SNssai> snssais;

    it->get(plmn, snssais);
    plmn.getMcc(plmnSliceSupport.mcc);
    plmn.getMnc(plmnSliceSupport.mnc);
    for (std::vector<SNssai>::iterator it = std::begin(snssais);
         it < std::end(snssais); ++it) {
      S_Nssai snssai = {};
      it->getSst(snssai.sst);
      it->getSd(snssai.sd);
      plmnSliceSupport.sliceList.push_back(snssai);
    }

    list.push_back(plmnSliceSupport);
  }

  return true;
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::setUeRetentionInformation(
    const UeRetentionInformation& value) {
  m_UeRetentionInformation = std::make_optional<UeRetentionInformation>(value);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UERetentionInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_UERetentionInformation;

  if (!m_UeRetentionInformation.value().encode(
          ie->value.choice.UERetentionInformation)) {
    Logger::ngap().error("Encode NGAP UeRetentionInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupResponsIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UeRetentionInformation IE error");
}

//------------------------------------------------------------------------------
void NgSetupResponseMsg::getUeRetentionInformation(
    std::optional<UeRetentionInformation>& value) const {
  value = m_UeRetentionInformation;
}

}  // namespace ngap
