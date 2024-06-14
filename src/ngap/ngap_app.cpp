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

#include "ngap_app.hpp"

#include "amf_config.hpp"
#include "logger.hpp"
#include "ngap_message_callback.hpp"
#include "ngap_utils.hpp"

extern "C" {
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
}

using namespace sctp;
using namespace oai::config;
using namespace oai::ngap;

//------------------------------------------------------------------------------
ngap_app::ngap_app(const std::string& address, const uint16_t port_num)
    : ppid_(60), sctp_s_38412(address.c_str(), port_num) {
  sctp_s_38412.start_receive(this);
  Logger::ngap().info(
      "Set N2 AMF IPv4 Addr %s, port %d", address.c_str(), port_num);
}

//------------------------------------------------------------------------------
ngap_app::~ngap_app() {}

//------------------------------------------------------------------------------
void ngap_app::handle_receive(
    bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream,
    sctp_stream_id_t instreams, sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Handling SCTP payload from SCTP Server on assoc_id (%d), stream_id "
      "(%d), instreams (%d), outstreams (%d)",
      assoc_id, stream, instreams, outstreams);

  Ngap_NGAP_PDU_t* ngap_msg_pdu =
      (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  asn_dec_rval_t dec_ret;

  dec_ret = aper_decode(
      NULL, &asn_DEF_Ngap_NGAP_PDU, (void**) &ngap_msg_pdu, bdata(payload),
      blength(payload), 0, 0);

  Logger::ngap().debug(
      "Decoded NGAP message, procedure code %d, present %d",
      ngap_msg_pdu->choice.initiatingMessage->procedureCode,
      ngap_msg_pdu->present);
  ngap_utils::print_asn_msg(&asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);

  if ((ngap_msg_pdu->choice.initiatingMessage->procedureCode >
       (NGAP_PROCEDURE_CODE_MAX_VALUE - 1)) or
      (ngap_msg_pdu->present > NGAP_PRESENT_MAX_VALUE)) {
    Logger::ngap().error(
        "Invalid procedure code %d or present %d",
        ngap_msg_pdu->choice.initiatingMessage->procedureCode,
        ngap_msg_pdu->present);
    return;
  }

  // Handle the message
  (*messages_callback[ngap_msg_pdu->choice.initiatingMessage->procedureCode]
                     [ngap_msg_pdu->present - 1])(
      assoc_id, stream, ngap_msg_pdu);
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_new_association(
    sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams,
    sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Ready to handle new NGAP SCTP association request (id %d)", assoc_id);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(assoc_id, gc)) {
    Logger::ngap().debug(
        "Create a new gNB context with assoc_id (%d)", assoc_id);
    gc = std::make_shared<gnb_context>();
    set_assoc_id_2_gnb_context(assoc_id, gc);
  } else {
    if (gc->ng_state == NGAP_RESETING || gc->ng_state == NGAP_SHUTDOWN) {
      Logger::ngap().warn(
          "Received a new association request on an association that is being "
          "%s, ignoring",
          ng_gnb_state_str[gc->ng_state]);
      return;
    } else {
      Logger::ngap().debug("Update gNB context with assoc id (%d)", assoc_id);
    }
  }

  // Update gNB Context
  gc->sctp_assoc_id    = assoc_id;
  gc->instreams        = instreams;
  gc->outstreams       = outstreams;
  gc->next_sctp_stream = 1;
  gc->ng_state         = NGAP_INIT;
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_shutdown(sctp_assoc_id_t assoc_id) {
  Logger::ngap().debug(
      "Handle a SCTP Shutdown event (association id: %d)", assoc_id);

  // Handle the message
  (*events_callback[0])(assoc_id);
}

//------------------------------------------------------------------------------
uint32_t ngap_app::get_ppid() {
  return ppid_;
}

//------------------------------------------------------------------------------
bool ngap_app::is_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id) const {
  std::shared_lock lock(m_assoc2gnb_context);
  if (assoc2gnb_context.count(assoc_id) > 0) {
    if (assoc2gnb_context.at(assoc_id) != nullptr) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool ngap_app::assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context>& gc) {
  std::shared_lock lock(m_assoc2gnb_context);
  if (assoc2gnb_context.count(assoc_id) > 0) {
    if (assoc2gnb_context.at(assoc_id) == nullptr) return false;
    gc = assoc2gnb_context.at(assoc_id);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
std::vector<sctp::sctp_assoc_id_t> ngap_app::get_all_assoc_ids() {
  std::shared_lock lock(m_assoc2gnb_context);
  std::vector<sctp::sctp_assoc_id_t> assoc_ids;
  for (auto& it : assoc2gnb_context) {
    assoc_ids.push_back(it.first);
  }
  return assoc_ids;
}

//------------------------------------------------------------------------------
void ngap_app::set_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context> gc) {
  std::shared_lock lock(m_assoc2gnb_context);
  assoc2gnb_context[assoc_id] = gc;
  return;
}

//------------------------------------------------------------------------------
bool ngap_app::is_gnb_id_2_gnb_context(const long& gnb_id) const {
  std::shared_lock lock(m_gnbid2gnb_context);
  if (gnbid2gnb_context.count(gnb_id) > 0) {
    if (gnbid2gnb_context.at(gnb_id) != nullptr) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool ngap_app::gnb_id_2_gnb_context(
    const long& gnb_id, std::shared_ptr<gnb_context>& gc) const {
  std::shared_lock lock(m_gnbid2gnb_context);
  if (gnbid2gnb_context.count(gnb_id) > 0) {
    if (gnbid2gnb_context.at(gnb_id) == nullptr) return false;
    gc = gnbid2gnb_context.at(gnb_id);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void ngap_app::set_gnb_id_2_gnb_context(
    const long& gnb_id, const std::shared_ptr<gnb_context>& gc) {
  std::unique_lock lock(m_gnbid2gnb_context);
  gnbid2gnb_context[gnb_id] = gc;
  return;
}

//------------------------------------------------------------------------------
void ngap_app::remove_gnb_context(const long& gnb_id) {
  if (is_gnb_id_2_gnb_context(gnb_id)) {
    std::unique_lock lock(m_gnbid2gnb_context);
    gnbid2gnb_context.erase(gnb_id);
    return;
  }
}
