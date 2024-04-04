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

#include "NgSetupRequest.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NgSetupRequestMsg::NgSetupRequestMsg() : NgapMessage() {
  m_NgSetupRequestIes = nullptr;
  m_RanNodeName       = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
NgSetupRequestMsg::~NgSetupRequestMsg() {}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::initialize() {
  m_NgSetupRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.NGSetupRequest);
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::setGlobalRanNodeId(
    const std::string& mcc, const std::string& mnc,
    const Ngap_GlobalRANNodeID_PR& ranNodeType, const uint32_t& ranNodeId,
    const uint8_t& ranNodeIdSize) {
  GlobalRanNodeId globalRanNodeIdIE = {};
  globalRanNodeIdIE.setChoiceOfRanNodeId(ranNodeType);

  // TODO: other options for GlobalNgEnbId/Global N3IWF ID
  GlobalGnbId globalgNBId = {};
  PlmnId plmn             = {};
  plmn.set(mcc, mnc);
  GnbId gnbid = {};
  gnbid.set(ranNodeId, ranNodeIdSize);
  globalgNBId.set(plmn, gnbid);
  globalRanNodeIdIE.set(globalgNBId);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

  if (!globalRanNodeIdIE.encode(ie->value.choice.GlobalRANNodeID)) {
    Logger::ngap().error("Encode NGAP GlobalRANNodeID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP GlobalRANNodeID IE error");
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::setRanNodeName(const std::string& value) {
  RanNodeName tmp = {};
  if (!tmp.set(value)) {
    return;
  }

  m_RanNodeName = std::optional<RanNodeName>(tmp);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANNodeName;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;

  if (!m_RanNodeName.value().encode(ie->value.choice.RANNodeName)) {
    Logger::ngap().error("Encode NGAP RANNodeName IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANNodeName IE error");
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::setSupportedTaList(
    const std::vector<struct SupportedTaItem_s>& list) {
  if (list.size() == 0) {
    Logger::ngap().warn("List of Supported Items is empty");
    return;
  }

  SupportedTaList supportedTAListIE = {};
  std::vector<SupportedTaItem> supportedTaItems;

  for (int i = 0; i < list.size(); i++) {
    SupportedTaItem item = {};
    TAC tac              = {};
    tac.set(list[i].tac);
    item.setTac(tac);

    std::vector<BroadcastPlmnItem> broadcastPlmnItems;
    for (int j = 0; j < list[i].plmnSliceSupportList.size(); j++) {
      BroadcastPlmnItem broadcastPlmnItem = {};
      PlmnId broadPlmn                    = {};
      broadPlmn.set(
          list[i].plmnSliceSupportList[j].mcc,
          list[i].plmnSliceSupportList[j].mnc);
      std::vector<SNssai> snssais;

      for (int k = 0; k < list[i].plmnSliceSupportList[j].sliceList.size();
           k++) {
        SNssai snssai = {};
        snssai.setSst(list[i].plmnSliceSupportList[j].sliceList[k].sst);
        if (list[i].plmnSliceSupportList[j].sliceList[k].sd.size())
          snssai.setSd(list[i].plmnSliceSupportList[j].sliceList[k].sd);
        snssais.push_back(snssai);
      }

      broadcastPlmnItem.set(broadPlmn, snssais);
      broadcastPlmnItems.push_back(broadcastPlmnItem);
    }
    item.setBroadcastPlmnList(broadcastPlmnItems);

    supportedTaItems.push_back(item);
  }
  supportedTAListIE.setSupportedTaItems(supportedTaItems);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SupportedTAList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_SupportedTAList;

  if (!supportedTAListIE.encode(ie->value.choice.SupportedTAList)) {
    Logger::ngap().error("Encode SupportedTAList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SupportedTAList IE error");
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::setDefaultPagingDrx(const e_Ngap_PagingDRX& value) {
  DefaultPagingDrx defaultPagingDRXIE;
  defaultPagingDRXIE.set(value);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;

  if (!defaultPagingDRXIE.encode(ie->value.choice.PagingDRX)) {
    Logger::ngap().error("Encode DefaultPagingDRX IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode DefaultPagingDRX IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
bool NgSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGSetupRequest) {
      m_NgSetupRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice.NGSetupRequest;
      for (int i = 0; i < m_NgSetupRequestIes->protocolIEs.list.count; i++) {
        switch (m_NgSetupRequestIes->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_GlobalRANNodeID: {
            if (m_NgSetupRequestIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                m_NgSetupRequestIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID) {
              if (!m_GlobalRanNodeId.decode(
                      m_NgSetupRequestIes->protocolIEs.list.array[i]
                          ->value.choice.GlobalRANNodeID)) {
                Logger::ngap().error("Decoded NGAP GlobalRanNodeId IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP GlobalRanNodeId IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_RANNodeName: {
            if (m_NgSetupRequestIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                m_NgSetupRequestIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_RANNodeName) {
              m_RanNodeName = std::make_optional<RanNodeName>();
              if (!m_RanNodeName.value().decode(
                      m_NgSetupRequestIes->protocolIEs.list.array[i]
                          ->value.choice.RANNodeName)) {
                Logger::ngap().error("Decoded NGAP RanNodeName IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP RanNodeName IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_SupportedTAList: {
            if (m_NgSetupRequestIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                m_NgSetupRequestIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_SupportedTAList) {
              if (!m_SupportedTaList.decode(
                      m_NgSetupRequestIes->protocolIEs.list.array[i]
                          ->value.choice.SupportedTAList)) {
                Logger::ngap().error("Decoded NGAP SupportedTAList IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP SupportedTAList IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_DefaultPagingDRX: {
            if (m_NgSetupRequestIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                m_NgSetupRequestIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_PagingDRX) {
              if (!m_DefaultPagingDrx.decode(
                      m_NgSetupRequestIes->protocolIEs.list.array[i]
                          ->value.choice.PagingDRX)) {
                Logger::ngap().error("Decoded NGAP DefaultPagingDRX IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP DefaultPagingDRX IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_UERetentionInformation: {
            if (m_NgSetupRequestIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                m_NgSetupRequestIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_UERetentionInformation) {
              UeRetentionInformation tmp = {};
              if (!tmp.decode(m_NgSetupRequestIes->protocolIEs.list.array[i]
                                  ->value.choice.UERetentionInformation)) {
                Logger::ngap().error(
                    "Decoded NGAP UERetentionInformation IE error");
                return false;
              }
              m_UeRetentionInformation =
                  std::make_optional<UeRetentionInformation>(tmp);
            } else {
              Logger::ngap().error(
                  "Decoded NGAP UERetentionInformation IE error");
              return false;
            }
          } break;
          default: {
            Logger::ngap().error("Decoded NGAP message PDU error");
            return false;
          }
        }
      }
    } else {
      Logger::ngap().error("Check NGSetupRequest message error");
      return false;
    }
  } else {
    Logger::ngap().error("Check NGSetupRequest message error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool NgSetupRequestMsg::getGlobalGnbId(
    uint32_t& gnbId, std::string& mcc, std::string& mnc) const {
  // TODO: Only support Global gNB ID for now
  if (m_GlobalRanNodeId.getChoiceOfRanNodeId() !=
      Ngap_GlobalRANNodeID_PR_globalGNB_ID) {
    Logger::ngap().warn("RAN node type is not supported!");
    return false;
  }

  GlobalGnbId globalgNBId = {};
  if (!m_GlobalRanNodeId.get(globalgNBId)) {
    Logger::ngap().warn("There's no value for Global RAN Node ID!");
    return false;
  }

  PlmnId plmn = {};
  GnbId gnbid = {};
  globalgNBId.get(plmn, gnbid);
  plmn.getMcc(mcc);
  plmn.getMnc(mnc);
  if (!gnbid.get(gnbId)) {
    Logger::ngap().warn("There's no value for gNB ID!");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool NgSetupRequestMsg::getRanNodeName(std::string& name) const {
  if (!m_RanNodeName.has_value()) return false;
  m_RanNodeName.value().get(name);
  return true;
}

//------------------------------------------------------------------------------
bool NgSetupRequestMsg::getSupportedTaList(
    std::vector<struct SupportedTaItem_s>& list) const {
  std::vector<SupportedTaItem> supportedTaItems;
  m_SupportedTaList.getSupportedTaItems(supportedTaItems);

  for (std::vector<SupportedTaItem>::iterator it = std::begin(supportedTaItems);
       it < std::end(supportedTaItems); ++it) {
    TAC tac = {};
    it->getTac(tac);

    SupportedTaItem_t supporteditem_data = {};
    supporteditem_data.tac               = tac.get();

    std::vector<BroadcastPlmnItem> broadcastPLMNItems;
    it->getBroadcastPlmnList(broadcastPLMNItems);

    for (std::vector<BroadcastPlmnItem>::iterator it =
             std::begin(broadcastPLMNItems);
         it < std::end(broadcastPLMNItems); ++it) {
      PlmnId plmnId_decode = {};
      std::vector<SNssai> snssai_decode;

      it->get(plmnId_decode, snssai_decode);

      PlmnSliceSupport_t broadcastplmn_data = {};
      plmnId_decode.getMcc(broadcastplmn_data.mcc);
      plmnId_decode.getMnc(broadcastplmn_data.mnc);

      for (std::vector<SNssai>::iterator it = std::begin(snssai_decode);
           it < std::end(snssai_decode); ++it) {
        S_Nssai slicesupport_data = {};
        it->getSst(slicesupport_data.sst);
        it->getSd(slicesupport_data.sd);
        broadcastplmn_data.sliceList.push_back(slicesupport_data);
      }
      supporteditem_data.plmnSliceSupportList.push_back(broadcastplmn_data);
    }
    list.push_back(supporteditem_data);
  }

  return true;
}

//------------------------------------------------------------------------------
e_Ngap_PagingDRX NgSetupRequestMsg::getDefaultPagingDrx() const {
  return m_DefaultPagingDrx.get();
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::setUeRetentionInformation(
    const UeRetentionInformation& value) {
  m_UeRetentionInformation = std::make_optional<UeRetentionInformation>(value);
}

//------------------------------------------------------------------------------
void NgSetupRequestMsg::getUeRetentionInformation(
    std::optional<UeRetentionInformation>& value) const {
  value = m_UeRetentionInformation;
}

}  // namespace ngap
