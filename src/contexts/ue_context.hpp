/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_H_
#define _UE_CONTEXT_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "NgapIesStruct.hpp"
#include "pdu_session_context.hpp"
#include "sbi_helper.hpp"
#include "PolicyAssociation.h"

extern "C" {
#include "Ngap_RRCEstablishmentCause.h"
}

using namespace oai::ngap;

class ue_context {
 public:
  ue_context();
  virtual ~ue_context(){};
  bool get_pdu_session_context(
      std::uint8_t session_id,
      std::shared_ptr<pdu_session_context>& context) const;
  void add_pdu_session_context(
      std::uint8_t session_id,
      const std::shared_ptr<pdu_session_context>& context);
  void copy_pdu_sessions(const std::shared_ptr<ue_context>& ue_ctx);
  bool get_pdu_sessions_context(
      std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx) const;

  bool remove_pdu_sessions_context(uint8_t pdu_session_id);
  bool set_up_cnx_state(uint8_t pdu_session_id, const up_cnx_state_e& state);

 public:
  uint32_t ran_ue_ngap_id;  // 32bits
  uint64_t amf_ue_ngap_id;  // 40bits
  uint32_t gnb_id;
  std::string supi;
  uint32_t tmsi;

  uint8_t rrc_estb_cause;
  bool is_ue_context_request;
  NrCgi_t cgi;
  Tai_t tai;
  // pdu session id <-> pdu_session_contex
  std::map<std::uint8_t, std::shared_ptr<pdu_session_context>> pdu_sessions;
  mutable std::shared_mutex m_pdu_session;

  std::string amf_3gpp_access_location;
  // std::optional<oai::_3gpp::model::AccessAndMobilitySubscriptionData>
  // am_data;

  std::optional<std::string> nrf_uri;

  // PCF related info
  oai::common::sbi::nf_addr_t pcf_addr;
  std::optional<oai::_3gpp::model::PolicyAssociation> policy_association;
  std::string policy_association_location;

  // UDM SDM subscription (Nudm_SDM_Subscribe, TS 29.503 §5.2.3.3.3)
  std::string udm_sdm_subscription_id;
  // Set when an SDM notification arrives while the UE is CM-IDLE so that
  // a Configuration Update Command can be sent on the next NAS connection.
  bool pending_sdm_update = false;
};

#endif
