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

#include "NGSetupRequest.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
NGSetupRequestMsg::NGSetupRequestMsg() : NgapMessage() {
  ngSetupRequestIEs = nullptr;
  ranNodeName       = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
NGSetupRequestMsg::~NGSetupRequestMsg() {}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::initialize() {
  ngSetupRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.NGSetupRequest);
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setGlobalRanNodeID(
    const std::string& mcc, const std::string& mnc,
    const Ngap_GlobalRANNodeID_PR& ranNodeType, const uint32_t& ranNodeId,
    const uint8_t& ran_node_id_size) {
  GlobalRanNodeId globalRanNodeIdIE = {};
  globalRanNodeIdIE.setChoiceOfRanNodeId(ranNodeType);

  // TODO: other options for GlobalNgENBId/Global N3IWF ID
  GlobalgNBId globalgNBId = {};
  PlmnId plmn             = {};
  plmn.set(mcc, mnc);
  GNB_ID gnbid = {};
  gnbid.setValue(ranNodeId, ran_node_id_size);
  globalgNBId.set(plmn, gnbid);
  globalRanNodeIdIE.set(globalgNBId);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

  if (!globalRanNodeIdIE.encode(ie->value.choice.GlobalRANNodeID)) {
    Logger::ngap().error("Encode NGAP GlobalRANNodeID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP GlobalRANNodeID IE error");
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setRanNodeName(const std::string& value) {
  RanNodeName tmp = {};
  if (!tmp.setValue(value)) {
    return;
  }

  ranNodeName = std::optional<RanNodeName>(tmp);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANNodeName;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;

  if (!ranNodeName.value().encode(ie->value.choice.RANNodeName)) {
    Logger::ngap().error("Encode NGAP RANNodeName IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANNodeName IE error");
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setSupportedTAList(
    const std::vector<struct SupportedTaItem_s> list) {
  if (list.size() == 0) {
    Logger::ngap().warn("List of Supported Items is empty");
    return;
  }

  SupportedTAList supportedTAListIE = {};
  std::vector<SupportedTaItem> supportedTaItems;

  for (int i = 0; i < list.size(); i++) {
    SupportedTaItem item = {};
    TAC tac              = {};
    tac.set(list[i].tac);
    item.setTac(tac);

    std::vector<BroadcastPlmnItem> broadcastPlmnItems;
    for (int j = 0; j < list[i].b_plmn_list.size(); j++) {
      BroadcastPlmnItem broadcastPlmnItem = {};
      PlmnId broadPlmn                    = {};
      broadPlmn.set(list[i].b_plmn_list[j].mcc, list[i].b_plmn_list[j].mnc);
      std::vector<S_NSSAI> snssais;

      for (int k = 0; k < list[i].b_plmn_list[j].slice_list.size(); k++) {
        S_NSSAI snssai = {};
        snssai.setSst(list[i].b_plmn_list[j].slice_list[k].sst);
        if (list[i].b_plmn_list[j].slice_list[k].sd.size())
          snssai.setSd(list[i].b_plmn_list[j].slice_list[k].sd);
        snssais.push_back(snssai);
      }

      broadcastPlmnItem.setPlmnSliceSupportList(broadPlmn, snssais);
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
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SupportedTAList IE error");
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setDefaultPagingDRX(const e_Ngap_PagingDRX& value) {
  DefaultPagingDrx defaultPagingDRXIE;
  defaultPagingDRXIE.setValue(value);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;

  if (!defaultPagingDRXIE.encode(ie->value.choice.PagingDRX)) {
    Logger::ngap().error("Encode DefaultPagingDRX IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode DefaultPagingDRX IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGSetupRequest) {
      ngSetupRequestIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.NGSetupRequest;
      for (int i = 0; i < ngSetupRequestIEs->protocolIEs.list.count; i++) {
        switch (ngSetupRequestIEs->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_GlobalRANNodeID: {
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID) {
              if (!globalRanNodeId.decode(
                      ngSetupRequestIEs->protocolIEs.list.array[i]
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
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_RANNodeName) {
              ranNodeName = std::make_optional<RanNodeName>();
              if (!ranNodeName.value().decode(
                      ngSetupRequestIEs->protocolIEs.list.array[i]
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
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_SupportedTAList) {
              if (!supportedTAList.decode(
                      ngSetupRequestIEs->protocolIEs.list.array[i]
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
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_PagingDRX) {
              if (!defaultPagingDrx.decode(
                      ngSetupRequestIEs->protocolIEs.list.array[i]
                          ->value.choice.PagingDRX)) {
                Logger::ngap().error("Decoded NGAP DefaultPagingDRX IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP DefaultPagingDRX IE error");
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
bool NGSetupRequestMsg::getGlobalGnbID(
    uint32_t& gnbId, std::string& mcc, std::string& mnc) {
  // TODO: Only support Global gNB ID for now
  if (globalRanNodeId.getChoiceOfRanNodeId() !=
      Ngap_GlobalRANNodeID_PR_globalGNB_ID) {
    Logger::ngap().warn("RAN node type is not supported!");
    return false;
  }

  GlobalgNBId globalgNBId = {};
  if (!globalRanNodeId.get(globalgNBId)) {
    Logger::ngap().warn("There's no value for Global RAN Node ID!");
    return false;
  }

  PlmnId plmn  = {};
  GNB_ID gnbid = {};
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
bool NGSetupRequestMsg::getRanNodeName(std::string& name) {
  if (!ranNodeName.has_value()) return false;
  ranNodeName.value().getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::getSupportedTAList(
    std::vector<struct SupportedTaItem_s>& list) {
  std::vector<SupportedTaItem> supportedTaItems;
  supportedTAList.getSupportedTaItems(supportedTaItems);

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
      std::vector<S_NSSAI> snssai_decode;

      it->getPlmnSliceSupportList(plmnId_decode, snssai_decode);

      PlmnSliceSupport_t broadcastplmn_data = {};
      plmnId_decode.getMcc(broadcastplmn_data.mcc);
      plmnId_decode.getMnc(broadcastplmn_data.mnc);

      for (std::vector<S_NSSAI>::iterator it = std::begin(snssai_decode);
           it < std::end(snssai_decode); ++it) {
        S_Nssai slicesupport_data = {};
        it->getSst(slicesupport_data.sst);
        it->getSd(slicesupport_data.sd);
        broadcastplmn_data.slice_list.push_back(slicesupport_data);
      }
      supporteditem_data.b_plmn_list.push_back(broadcastplmn_data);
    }
    list.push_back(supporteditem_data);
  }

  return true;
}

//------------------------------------------------------------------------------
e_Ngap_PagingDRX NGSetupRequestMsg::getDefaultPagingDRX() {
  return defaultPagingDrx.getValue();
}
}  // namespace ngap
