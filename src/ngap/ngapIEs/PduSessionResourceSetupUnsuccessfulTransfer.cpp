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

#include "PduSessionResourceSetupUnsuccessfulTransfer.hpp"

#include "logger.hpp"
#include "output_wrapper.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupUnSuccessfulTransferIE::
    PduSessionResourceSetupUnSuccessfulTransferIE() {
  m_PduSessionResourceSetupUnsuccessfulTransferIe =
      (Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t));
}

//------------------------------------------------------------------------------
PduSessionResourceSetupUnSuccessfulTransferIE::
    ~PduSessionResourceSetupUnSuccessfulTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupUnSuccessfulTransferIE::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork causeValue) {
  m_Cause.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  m_Cause.set(causeValue);

  int ret =
      m_Cause.encode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseRadioNetwork IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupUnSuccessfulTransferIE::setCauseTransport(
    e_Ngap_CauseTransport causeValue) {
  m_Cause.setChoiceOfCause(Ngap_Cause_PR_transport);
  m_Cause.set(causeValue);

  int ret =
      m_Cause.encode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseTransport IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupUnSuccessfulTransferIE::setCauseNas(
    e_Ngap_CauseNas causeValue) {
  m_Cause.setChoiceOfCause(Ngap_Cause_PR_nas);
  m_Cause.set(causeValue);

  int ret =
      m_Cause.encode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseNas IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupUnSuccessfulTransferIE::setCauseProtocol(
    e_Ngap_CauseProtocol causeValue) {
  m_Cause.setChoiceOfCause(Ngap_Cause_PR_protocol);
  m_Cause.set(causeValue);

  int ret =
      m_Cause.encode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseProtocol IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupUnSuccessfulTransferIE::setCauseMisc(
    e_Ngap_CauseMisc causeValue) {
  m_Cause.setChoiceOfCause(Ngap_Cause_PR_misc);
  m_Cause.set(causeValue);

  int ret =
      m_Cause.encode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause);
  if (!ret) {
    Logger::ngap().error("Encode CauseMisc IE error");
    return;
  }
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupUnSuccessfulTransferIE::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer,
      m_PduSessionResourceSetupUnsuccessfulTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer, NULL,
      m_PduSessionResourceSetupUnsuccessfulTransferIe, buf, buf_size);
  Logger::ngap().debug("er.encoded( %d)", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupUnSuccessfulTransferIE::decode(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer,
      (void**) &m_PduSessionResourceSetupUnsuccessfulTransferIe, buf, buf_size);

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
  // m_PduSessionResourceSetupUnsuccessfulTransferIe);

  if (!m_Cause.decode(m_PduSessionResourceSetupUnsuccessfulTransferIe->cause)) {
    Logger::ngap().error("Decode Cause IE error");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
long PduSessionResourceSetupUnSuccessfulTransferIE::getChoiceOfCause() const {
  return m_Cause.getChoiceOfCause();
}

//------------------------------------------------------------------------------
long PduSessionResourceSetupUnSuccessfulTransferIE::getCause() const {
  return m_Cause.get();
}
}  // namespace ngap
