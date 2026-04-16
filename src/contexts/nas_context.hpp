/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_NAS_CONTEXT_H_
#define _AMF_NAS_CONTEXT_H_

#include "UeSecurityCapability.hpp"
#include "authentication_algorithms_with_5gaka.hpp"
#include "itti.hpp"
#include "nas_security_context.hpp"
#include "Struct.hpp"
#include <cstdint>
#include <queue>
#include <string>

typedef enum {
  _5GMM_STATE_MIN     = 0,
  _5GMM_STATE_INVALID = _5GMM_STATE_MIN,
  _5GMM_DEREGISTERED,
  _5GMM_REGISTERED,
  _5GMM_DEREGISTERED_INITIATED,
  _5GMM_COMMON_PROCEDURE_INITIATED,
  _5GMM_STATE_MAX
} _5gmm_state_t;

typedef enum { CM_IDLE = 0, CM_CONNECTED } cm_state_t;

typedef enum {
  DEREGISTERED = 0,
  MAX_DETECTION_TIME_EXPIRED,
  PURGED
} loss_of_connectivity_status_t;

// ---- NAS procedure type tracking (replaces boolean flags) ----
// Per §5.1.3.2.3: the network side has no sub-states; track active procedure
// instead. (3GPP TS 24.501 v16.14.0 §5.1.3.2.3 defines only 4 main states on
// the network side)

enum class nas_procedure_type_e : uint8_t {
  NONE = 0,
  // Common procedures (§5.4)
  AUTHENTICATION,         // §5.4.1
  SECURITY_MODE_CONTROL,  // §5.4.2
  IDENTIFICATION,         // §5.4.3
  CONFIGURATION_UPDATE,   // §5.4.4
  NAS_TRANSPORT,          // §5.4.5
  _5GMM_STATUS,           // §5.4.6
  // Specific procedures (§5.5)
  REGISTRATION_INITIAL,    // §5.5.1.2
  REGISTRATION_MOBILITY,   // §5.5.1.3
  REGISTRATION_PERIODIC,   // §5.5.1.3
  DEREGISTRATION_UE,       // §5.5.2.2
  DEREGISTRATION_NETWORK,  // §5.5.2.3
  // Connection management (§5.6)
  SERVICE_REQUEST,  // §5.6.1
  PAGING,           // §5.6.2
  NOTIFICATION      // §5.6.3
};

// Human-readable NAS procedure type for logging
const char* nas_procedure_type_to_string(nas_procedure_type_e type);

// Per-UE procedure context
struct nas_procedure_context_t {
  nas_procedure_type_e specific_procedure;  // The active specific procedure
  nas_procedure_type_e
      common_procedure;  // The active common procedure nested within specific
  _5gmm_state_t prior_state;     // 5GMM state before entering CPI (B-1/B-2 fix)
  bool dereg_switch_off;         // From DEREGISTRATION REQUEST "switch off" IE
  uint8_t dereg_cause;           // 5GMM cause from DEREGISTRATION REQUEST
  uint8_t retransmission_count;  // For the active procedure's timer
};

// Per-UE NAS timer instance (POD)
// Defined here (not in nas_timer_manager.hpp) to avoid circular dependency.
struct nas_timer_instance_t {
  timer_id_t itti_timer_id;      // ITTI timer ID (0 = not running)
  uint8_t retransmission_count;  // Current retransmission count
  bool is_running;               // Timer active flag
};

static constexpr size_t kNasTimerCount =
    7;  // T3550, T3560, T3570, T3522, T3555, T3513, T3565

// Pending N1/N2 message for delivery after paging response — §4.2.3.3 TS 23.502
struct pending_n1n2_msg_t {
  bstring n1sm = nullptr;
  bstring n2sm = nullptr;
  std::string n2sm_info_type;
  uint8_t pdu_session_id = 0;
  uint8_t ppi            = 0;
  std::string callback_uri;

  ~pending_n1n2_msg_t() {
    if (n1sm) bdestroy(n1sm);
    if (n2sm) bdestroy(n2sm);
  }

  pending_n1n2_msg_t() = default;
  pending_n1n2_msg_t(pending_n1n2_msg_t&& o) noexcept
      : n1sm(o.n1sm),
        n2sm(o.n2sm),
        n2sm_info_type(std::move(o.n2sm_info_type)),
        pdu_session_id(o.pdu_session_id),
        ppi(o.ppi),
        callback_uri(std::move(o.callback_uri)) {
    o.n1sm = nullptr;
    o.n2sm = nullptr;
  }
  pending_n1n2_msg_t& operator=(pending_n1n2_msg_t&& o) noexcept {
    if (this != &o) {
      if (n1sm) bdestroy(n1sm);
      if (n2sm) bdestroy(n2sm);
      n1sm           = o.n1sm;
      n2sm           = o.n2sm;
      n2sm_info_type = std::move(o.n2sm_info_type);
      pdu_session_id = o.pdu_session_id;
      ppi            = o.ppi;
      callback_uri   = std::move(o.callback_uri);
      o.n1sm         = nullptr;
      o.n2sm         = nullptr;
    }
    return *this;
  }
  pending_n1n2_msg_t(const pending_n1n2_msg_t&) = delete;
  pending_n1n2_msg_t& operator=(const pending_n1n2_msg_t&) = delete;
};

class nas_context {
 public:
  nas_context();
  ~nas_context();
  bool ctx_avaliability_ind;
  uint64_t amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
  uint64_t old_amf_ue_ngap_id;
  uint32_t old_ran_ue_ngap_id;

  cm_state_t nas_status;
  _5gmm_state_t _5gmm_state;
  bool is_mobile_reachable_timer_timeout;
  timer_id_t mobile_reachable_timer;
  timer_id_t implicit_deregistration_timer;

  // Paging Proceed Flag — §5.6.2.1 TS 24.501
  bool ppf_3gpp = true;  // final T3513 expiry/ implicit deregistration)

  // Paging lifecycle state — §5.6.2 TS 24.501
  bool is_paging_ongoing       = false;
  uint8_t paging_effective_ppi = 0;
  bool paging_completed        = false;
  bool is_mico_mode =
      false;  // T21: true when MICO mode is negotiated (TS 24.501 §5.5.1.3)

  // Pending N1/N2 messages awaiting delivery after paging response
  std::queue<pending_n1n2_msg_t> pending_paging_messages;

  // NAS procedure context — replaces boolean flags per §5.1.3.2.3
  nas_procedure_context_t procedure_ctx;
  // NAS procedure timer instances indexed by nas_timer_type_e
  nas_timer_instance_t nas_timers[kNasTimerCount];

  // Parameters from Registration request
  uint8_t registration_type;  // 3 bits
  bool follow_on_req_pending_ind;
  uint8_t ngksi;  // 4 bits

  std::string imsi;  // TODO: use SUPI instead
  std::string supi;
  std::optional<oai::nas::IMEI_IMEISV_t> imeisv;
  std::optional<std::string> guti;

  std::uint8_t _5gmm_capability[13];
  oai::nas::UeSecurityCapability ue_security_capability;

  std::vector<oai::nas::SNSSAI_t> requested_nssai;
  std::vector<oai::nas::SNSSAI_t> allowed_nssai;  // in Registration Accept
  // Set to true if marked as default
  std::vector<std::pair<bool, oai::nas::SNSSAI_t>> subscribed_snssai;
  std::vector<oai::nas::SNSSAI_t> configured_nssai;
  // TODO: rejected_nssai;
  // std::vector<oai::nas::SNSSAI_t>  default_configured_nssai;
  // std::vector<oai::nas::SNSSAI_t> s_nssai; //for Network Slice selection

  bstring registration_request;  // for AMF re-allocation procedure
  bool registration_request_is_set;
  std::string serving_network;
  bstring auts;

  uint8_t nas_message_for_current_procedure_running;

  // Security-related parameters
#define MAX_5GS_AUTH_VECTORS 1
  _5G_HE_AV_t _5g_he_av[MAX_5GS_AUTH_VECTORS];  // generated by UDM
  _5G_AV_t _5g_av[MAX_5GS_AUTH_VECTORS];        // generated by AUSF
  std::string href;
  uint8_t kamf[MAX_5GS_AUTH_VECTORS][AUTH_VECTOR_LENGTH_OCTETS];
  uint8_t kgNB[AUTH_VECTOR_LENGTH_OCTETS];
  bool is_kgNB_set;
  std::optional<nas_secu_ctx> security_ctx;
  bool is_current_security_available;
  int registration_attempt_counter;  // used to limit the subsequently reject
                                     // registration
                                     // attempts(clause 5.5.1.2.7/5.5.1.3.7,
                                     // 3gpp ts24.501)
  // parameters present
  bool is_imsi_present;
  bool is_5g_guti_present;
  bool is_5g_suci_present;
  bool is_auth_vectors_present;
  bool to_be_register_by_new_suci;

  bool get_kamf(uint8_t index, uint8_t (&k)[AUTH_VECTOR_LENGTH_OCTETS]) const;
  static std::string fivegmm_state_to_string(const _5gmm_state_t& state);
  static std::string cm_state_to_string(const cm_state_t& state);
};

#endif
