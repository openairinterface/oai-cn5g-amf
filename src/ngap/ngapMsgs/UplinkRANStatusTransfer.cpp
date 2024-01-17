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

#include "UplinkRANStatusTransfer.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UplinkRANStatusTransfer::UplinkRANStatusTransfer() : NgapUEMessage() {
  uplinkRANStatusTransferIEs = nullptr;

  setMessageType(NgapMessageType::UPLINK_RAN_STATUS_TRANSFER);
  initialize();
}

//------------------------------------------------------------------------------
UplinkRANStatusTransfer::~UplinkRANStatusTransfer() {}

//------------------------------------------------------------------------------
void UplinkRANStatusTransfer::initialize() {
  uplinkRANStatusTransferIEs =
      &ngapPdu->choice.initiatingMessage->value.choice.UplinkRANStatusTransfer;
}

//------------------------------------------------------------------------------
void UplinkRANStatusTransfer::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_UplinkRANStatusTransferIEs_t* ie =
      (Ngap_UplinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_UplinkRANStatusTransferIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkRANStatusTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void UplinkRANStatusTransfer::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_UplinkRANStatusTransferIEs_t* ie =
      (Ngap_UplinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_UplinkRANStatusTransferIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkRANStatusTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void UplinkRANStatusTransfer::setRANStatusTransfer_TransparentContainer(
    const RanStatusTransferTransparentContainer& ranContainer) {
  ranStatusTransfer_TransparentContainer = ranContainer;

  Ngap_UplinkRANStatusTransferIEs_t* ie =
      (Ngap_UplinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_UplinkRANStatusTransferIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;

  int ret = ranStatusTransfer_TransparentContainer.encode(
      ie->value.choice.RANStatusTransfer_TransparentContainer);
  if (!ret) {
    Logger::ngap().error(
        "Encode RANStatusTransfer_TransparentContainer IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkRANStatusTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void UplinkRANStatusTransfer::getRANStatusTransfer_TransparentContainer(
    RanStatusTransferTransparentContainer& ranContainer) {
  ranContainer = ranStatusTransfer_TransparentContainer;
}

//------------------------------------------------------------------------------
bool UplinkRANStatusTransfer::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UplinkRANStatusTransfer &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UplinkRANStatusTransfer) {
      uplinkRANStatusTransferIEs = &ngapPdu->choice.initiatingMessage->value
                                        .choice.UplinkRANStatusTransfer;
    } else {
      Logger::ngap().error("Check UplinkRANStatusTransfer message error");
      return false;
    }
  } else {
    Logger::ngap().error("UplinkRANStatusTransfer message type error");
    return false;
  }
  for (int i = 0; i < uplinkRANStatusTransferIEs->protocolIEs.list.count; i++) {
    switch (uplinkRANStatusTransferIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
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
        if (uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
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
        if (uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer) {
          if (!ranStatusTransfer_TransparentContainer.decode(
                  uplinkRANStatusTransferIEs->protocolIEs.list.array[i]
                      ->value.choice.RANStatusTransfer_TransparentContainer)) {
            Logger::ngap().error(
                "Decoded NGAP RANStatusTransfer_TransparentContainer IE error");
            return false;
          }
          Logger::ngap().error(
              "Decoded NGAP RANStatusTransfer_TransparentContainer IE error");
        } else {
          Logger::ngap().error(
              "Decoded NGAP RANStatusTransfer_TransparentContainer IE error");
        }
      } break;
      default: {
        Logger::ngap().error("Decoded NGAP message PDU error");
        return false;
      }
    }
  }
  return true;
}

}  // namespace ngap
