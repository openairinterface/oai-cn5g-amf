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

#include "DownlinkRANStatusTransfer.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
DownlinkRANStatusTransfer::DownlinkRANStatusTransfer() : NgapUEMessage() {
  downlinkranstatustransferIEs = nullptr;

  setMessageType(NgapMessageType::DOWNLINK_RAN_STATUS_TRANSFER);
  initialize();
}

//------------------------------------------------------------------------------
DownlinkRANStatusTransfer::~DownlinkRANStatusTransfer() {}

//------------------------------------------------------------------------------
void DownlinkRANStatusTransfer::initialize() {
  downlinkranstatustransferIEs = &(ngapPdu->choice.initiatingMessage->value
                                       .choice.DownlinkRANStatusTransfer);
}

//------------------------------------------------------------------------------
void DownlinkRANStatusTransfer::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downlinkranstatustransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownlinkRANStatusTransfer::setRanUeNgapId(const uint32_t& id) {
  ranUeNgapId.set(id);

  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downlinkranstatustransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownlinkRANStatusTransfer::setRANStatusTransfer_TransparentContainer(
    const long& drbIDValue, const long& ulPdcpValue, const long& ulHfnPdcpValue,
    const long& dlPdcpValue, const long& dlHfnPdcpValue) {
  Ngap_DRB_ID_t dRB_id               = {};
  dRB_id                             = drbIDValue;
  CountValueForPdcpSn18 countValueUL = {};
  countValueUL.setvalue(ulPdcpValue, ulHfnPdcpValue);
  CountValueForPdcpSn18 countValueDL{};
  countValueDL.setvalue(dlPdcpValue, dlHfnPdcpValue);
  DrbStatusUl18 statusUL18 = {};
  statusUL18.set(countValueUL);
  DrbStatusDl18 statusDL18 = {};
  statusDL18.set(countValueDL);

  DrbStatusDl statusDL = {};
  statusDL.setDRBStatusDL18(statusDL18);
  DrbStatusUl statusUL = {};
  statusUL.setdRBStatusUL(statusUL18);
  std::vector<DrbSubjectToStatusTransferItem> dRBSubjectItemList;
  DrbSubjectToStatusTransferItem m_item = {};
  m_item.setdRBSubjectItem(dRB_id, statusUL, statusDL);
  dRBSubjectItemList.push_back(m_item);
  DrbSubjectToStatusTransferList m_list = {};
  m_list.setdRBSubjectItem(dRBSubjectItemList);
  ranStatusTransfer_TransparentContainer.setDRBSubjectList(m_list);

  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;
  bool ret = ranStatusTransfer_TransparentContainer.encode(
      ie->value.choice.RANStatusTransfer_TransparentContainer);
  if (!ret) {
    Logger::ngap().error(
        "Encode RANStatusTransfer_TransparentContainer IE error");
    free_wrapper((void**) &ie);
  }
  if (ASN_SEQUENCE_ADD(&downlinkranstatustransferIEs->protocolIEs.list, ie) !=
      0) {
    Logger::ngap().error(
        "Encode ranstatustransfer_transparentcontainer IE error");
  }
}

//------------------------------------------------------------------------------
void DownlinkRANStatusTransfer::getRANStatusTransfer_TransparentContainer(
    long& drbIDValue, long& ulPdcpValue, long& ulHfnPdcpValue,
    long& dlPdcpValue, long& dlHfnPdcpValue) {
  // TODO:
}

//------------------------------------------------------------------------------
bool DownlinkRANStatusTransfer::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_DownlinkRANStatusTransfer &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_DownlinkRANStatusTransfer) {
      downlinkranstatustransferIEs = &ngapPdu->choice.initiatingMessage->value
                                          .choice.DownlinkRANStatusTransfer;
    } else {
      Logger::ngap().error("Check DownlinkRANStatusTransfer message error!");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < downlinkranstatustransferIEs->protocolIEs.list.count;
       i++) {
    switch (downlinkranstatustransferIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  downlinkranstatustransferIEs->protocolIEs.list.array[i]
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
        if (downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  downlinkranstatustransferIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer: {
        if (downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkranstatustransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer) {
          if (!ranStatusTransfer_TransparentContainer.decode(
                  downlinkranstatustransferIEs->protocolIEs.list.array[i]
                      ->value.choice.RANStatusTransfer_TransparentContainer)) {
            Logger::ngap().error(
                "Decoded NGAP RANStatusTransfer_TransparentContainer IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP RANStatusTransfer_TransparentContainer IE error");
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
