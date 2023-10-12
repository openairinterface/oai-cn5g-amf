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

#include "PduSessionResourceReleaseCommandTransfer.hpp"

#include "logger.hpp"
#include "output_wrapper.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandTransfer::
    PduSessionResourceReleaseCommandTransfer() {
  pduSessionResourceReleaseCommandTransferIEs =
      (Ngap_PDUSessionResourceReleaseCommandTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandTransfer_t));
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandTransfer::
    ~PduSessionResourceReleaseCommandTransfer() {
  // if(pduSessionResourceReleaseCommandTransferIEs)
  // ASN_STRUCT_FREE(asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,pduSessionResourceReleaseCommandTransferIEs);
  // if (pduSessionResourceReleaseCommandTransferIEs)
  // free(pduSessionResourceReleaseCommandTransferIEs);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandTransfer::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value) {
  causeValue_.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue_.setValue(cause_value);

  int ret =
      causeValue_.encode(pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseRadioNetwork IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandTransfer::setCauseTransport(
    e_Ngap_CauseTransport cause_value) {
  causeValue_.setChoiceOfCause(Ngap_Cause_PR_transport);
  causeValue_.setValue(cause_value);

  int ret =
      causeValue_.encode(pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseTransport IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandTransfer::setCauseNas(
    e_Ngap_CauseNas cause_value) {
  causeValue_.setChoiceOfCause(Ngap_Cause_PR_nas);
  causeValue_.setValue(cause_value);

  int ret =
      causeValue_.encode(pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseNas IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandTransfer::setCauseProtocol(
    e_Ngap_CauseProtocol cause_value) {
  causeValue_.setChoiceOfCause(Ngap_Cause_PR_protocol);
  causeValue_.setValue(cause_value);

  int ret =
      causeValue_.encode(pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseProtocol IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandTransfer::setCauseMisc(
    e_Ngap_CauseMisc cause_value) {
  causeValue_.setChoiceOfCause(Ngap_Cause_PR_misc);
  causeValue_.setValue(cause_value);

  int ret =
      causeValue_.encode(pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseMisc IE error");
    return;
  }
}

//------------------------------------------------------------------------------
int PduSessionResourceReleaseCommandTransfer::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,
      pduSessionResourceReleaseCommandTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer, NULL,
      pduSessionResourceReleaseCommandTransferIEs, buf, buf_size);
  Logger::ngap().debug("er.encoded( %d)", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandTransfer::decode(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,
      (void**) &pduSessionResourceReleaseCommandTransferIEs, buf, buf_size);

  if (rc.code == RC_OK) {
    Logger::ngap().debug("Decoded successfully");
  } else if (rc.code == RC_WMORE) {
    Logger::ngap().debug("More data expected, call again");
    return false;
  } else {
    Logger::ngap().debug("Failure to decode data");
    return false;
  }

  Logger::ngap().debug("rc.consumed to decode %d", rc.consumed);
  // asn_fprint(stderr,
  // &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer,
  // pduSessionResourceSetupUnsuccessfulTransferIEs);

  if (!causeValue_.decode(pduSessionResourceReleaseCommandTransferIEs->cause)) {
    Logger::ngap().error("Decode Cause IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
long PduSessionResourceReleaseCommandTransfer::getChoiceOfCause() {
  return causeValue_.getChoiceOfCause();
}

//------------------------------------------------------------------------------
long PduSessionResourceReleaseCommandTransfer::getCause() {
  return causeValue_.getValue();
}
}  // namespace ngap
