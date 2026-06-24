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
//#include "AccessAndMobilitySubscriptionData.h"
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

  // TODO [AMF-N2-QOS]: Extend UE context to store dynamic QoS parameters
  // Reference: Phase 2: N2 QoS Message Completeness
  //
  // Task 2.0: Data Models and Schema Design
  //   - Add ue_ambr_dl and ue_ambr_ul fields (uint64_t, bps) to UE context
  //     - Default to 0 (unset) at context creation
  //     - Populated from UDM AM data response (Task 2.1)
  //     - Updated when SMF provides UE-AMBR in N1N2MessageTransfer (Task 2.2)
  //   - Add has_ue_ambr boolean to distinguish "not yet fetched" from "0 bps"
  //   - Fallback precedence: SMF-provided > UDM-subscribed > compile-time default
  //
  // Standards:
  //   - TS 29.503 §5.2.2.2 (UDM AM data — ueAmbr)
  //   - TS 29.518 §6.3.5.2 (N1N2MessageTransferReqData — ueAmbr)
  //   - TS 38.413 §9.3.1.47 (UE Aggregate Maximum Bit Rate IE)
  //
  // [QOS-MOCK] Phase 2 — Dynamic UE-AMBR storage ([AMF-N2-QOS]). Mocks the
  // Task 2.0 data-model TODO above:
  //   - Fields ue_ambr_dl / ue_ambr_ul / has_ue_ambr: added for real.
  //   - Source of the values: MOCKED — instead of being populated from UDM
  //     (Task 2.1) or the SMF N1N2 request (Task 2.2), mock values are seeded
  //     at context creation (see ue_context.cpp) so the N2 messages exercise
  //     the dynamic-UE-AMBR code path instead of the hardcoded constant.
  uint64_t ue_ambr_dl = 0;  // bps
  uint64_t ue_ambr_ul = 0;  // bps
  bool has_ue_ambr    = false;

  // PCF related info
  oai::common::sbi::nf_addr_t pcf_addr;
  std::optional<oai::_3gpp::model::PolicyAssociation> policy_association;
  std::string policy_association_location;
};

#endif
