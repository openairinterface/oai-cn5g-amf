/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_PAGING_CONTEXT_HPP_SEEN
#define FILE_PAGING_CONTEXT_HPP_SEEN

#include <stdint.h>

#include <chrono>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

#include "itti.hpp"   // timer_id_t
#include "utils.hpp"  // oai::utils::utils::bdestroy_wrapper

extern "C" {
#include "bstrlib.h"
}

// Types backing the CN-initiated paging procedure. They live on `ue_context`
// (see `ue_context.hpp`), which is the only UE object that survives the eight
// CM-IDLE teardown sites in `amf_n2.cpp`; `ue_ngap_context` is destroyed there
// and must therefore never hold paging state.

// Lifecycle of one paging transaction.
enum class paging_state_e : uint8_t {
  kIdle,        // no paging transaction in flight
  kInProgress,  // PAGING sent, waiting for the UE to answer
  kResponded,   // the UE answered; the buffer is being drained
  kAbandoned    // the response window expired; the buffer is being dropped
};

// Outcome of an attempt to open a paging transaction.
enum class paging_start_result_e : uint8_t {
  kStarted,
  kAlreadyInProgress,
  kNotPageable
};

// Maximum number of downlink N1/N2 payloads buffered per UE while it is being
// paged. Pushing beyond this evicts the oldest record, which is moved out to
// the caller so that the caller's scope frees it.
constexpr size_t kMaxPendingPayloads = 4;

// Length of the one-shot paging-response window, in seconds. This is NOT T3513
// and it is NOT a retransmission timer: it only bounds the lifetime of a
// transaction and of its buffered payloads.
constexpr uint32_t kPagingResponseWindowSeconds = 10;

// One buffered downlink N1/N2 payload.
//
// Owns its two bstrings. bstring is a raw pointer type: a silent copy would
// give two owners and the second drain would double-free. Copy is DELETED.
//
// The producer must always `bstrcpy()` into `n1sm`/`n2sm`, never steal the
// bstring off `itti_n1n2_message_transfer_request`: both SBI servers already
// hand out two owners per payload and free their own local at the end of the
// handler.
struct buffered_n1n2_t {
  std::string n1n2_message_id;
  std::string failure_notif_uri;  // n1n2FailureTxfNotifURI, for a later phase
  bstring n1sm     = nullptr;
  bool is_n1sm_set = false;
  bstring n2sm     = nullptr;
  bool is_n2sm_set = false;
  std::string n2sm_info_type;
  uint8_t pdu_session_id = 0;
  // Written by the caller before the push; `ue_context` only sweeps on it.
  std::chrono::steady_clock::time_point expires_at{};

  buffered_n1n2_t()                                  = default;
  buffered_n1n2_t(const buffered_n1n2_t&)            = delete;
  buffered_n1n2_t& operator=(const buffered_n1n2_t&) = delete;
  buffered_n1n2_t(buffered_n1n2_t&& o) noexcept { steal(std::move(o)); }
  buffered_n1n2_t& operator=(buffered_n1n2_t&& o) noexcept {
    if (this != &o) {
      free_payloads();
      steal(std::move(o));
    }
    return *this;
  }
  ~buffered_n1n2_t() { free_payloads(); }

 private:
  // Idempotent; nulls both bstrings.
  void free_payloads() {
    oai::utils::utils::bdestroy_wrapper(&n1sm);
    oai::utils::utils::bdestroy_wrapper(&n2sm);
    is_n1sm_set = is_n2sm_set = false;
  }
  // Leaves the moved-from record empty.
  void steal(buffered_n1n2_t&& o) noexcept {
    n1n2_message_id   = std::move(o.n1n2_message_id);
    failure_notif_uri = std::move(o.failure_notif_uri);
    n2sm_info_type    = std::move(o.n2sm_info_type);
    n1sm              = o.n1sm;
    o.n1sm            = nullptr;
    is_n1sm_set       = o.is_n1sm_set;
    o.is_n1sm_set     = false;
    n2sm              = o.n2sm;
    o.n2sm            = nullptr;
    is_n2sm_set       = o.is_n2sm_set;
    o.is_n2sm_set     = false;
    pdu_session_id    = o.pdu_session_id;
    expires_at        = o.expires_at;
  }
};

static_assert(
    !std::is_copy_constructible_v<buffered_n1n2_t>,
    "buffered_n1n2_t owns raw bstrings and must never be copied");

// State of the one in-flight paging transaction of a UE.
//
// Deliberately copyable and free of owning pointers: TASK_AMF_N2 takes a
// snapshot of it under a shared lock, releases the lock, and then encodes and
// sends with no lock held. The buffered payloads therefore live in a separate
// `ue_context` member, not here.
struct paging_ctx_t {
  paging_state_e state = paging_state_e::kIdle;
  std::string guti_key;
  uint32_t epoch             = 0;
  timer_id_t window_timer_id = 0;
};

#endif /* FILE_PAGING_CONTEXT_HPP_SEEN */
