/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once

#include <cstdint>
#include <memory>

// nas_context.hpp transitively pulls in itti.hpp, which provides
// timer_id_t (uint32_t), ITTI_INVALID_TIMER_ID, and the itti_mw class.
#include "nas_context.hpp"

// Timer type indices — MUST match kNasTimerCount = 7 in nas_context.hpp.
// Also must match the TASK_AMF_T35xx_TIMER_EXPIRE constants in amf_app.hpp
// (T3550 → TASK_AMF_T3550_TIMER_EXPIRE, etc.) so that ITTI arg1_user
// values decode correctly in the TIME_OUT switch.
enum class nas_timer_type_e : uint8_t {
  T3550 = 0,  // §5.5.1.2.4: Registration Accept retransmit (6 s, 4 retx)
  T3560 = 1,  // §5.4.1.3.7 / §5.4.2.7: Auth Request / SMC (6 s, 4 retx)
  T3570 = 2,  // §5.4.3.6: Identity Request (6 s, 4 retx)
  T3522 = 3,  // §5.5.2.3.5: NW-initiated Deregistration Request (6 s, 4 retx)
  T3555 = 4,  // §5.4.4.6: Configuration Update Command (6 s, 4 retx)
  T3513 = 5,  // §5.6.2.2.1: Paging (6 s, max 2 retx → final expiry → PPF=FALSE)
  T3565           = 6,  // §5.6.3: Notification (6 s, 4 retx)
  NAS_TIMER_COUNT = 7
};

// Per-timer static configuration.
// Non-paging entries (T3550..T3555) are compile-time constants.
// Paging entries (T3513, T3565) are runtime-initialised from amf_cfg in the
// nas_timer_manager constructor (TS 24.501 §5.6.2.2.1).
struct nas_timer_config_t {
  nas_timer_type_e type;
  uint32_t interval_sec;        // Default interval per 3GPP Table 10.2.2
  uint8_t max_retransmissions;  // 0 = discretionary (T3513)
  uint64_t itti_task_id;        // arg1_user sent to ITTI timer_setup()
  const char* name;             // For logging
};

class nas_timer_manager {
 public:
  // Default constructor: itti_ is null — must be replaced via assignment
  // before any timer operation (amf_n1 constructor does this).
  nas_timer_manager() = default;

  explicit nas_timer_manager(itti_mw* itti);

  // Start a NAS procedure timer for a UE context.
  // Stops any already-running instance of this timer first.
  // For CPI-state timers (T3550/T3560/T3570), enforces mutual exclusivity
  // only one CPI procedure timer may run at a time).
  timer_id_t start_timer(
      nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
      uint64_t amf_ue_ngap_id);

  // Stop a running timer. Safe to call when the timer is not running.
  void stop_timer(nas_timer_type_e type, std::shared_ptr<nas_context>& nc);

  // Handle a timer expiry event dispatched from the ITTI TIME_OUT handler.
  // Increments the retransmission counter.
  //
  // When auto_restart is true (default, used by T3550/T3560/T3570/T3522/T3555):
  //   restarts the ITTI timer automatically before returning.
  //
  // When auto_restart is false (used by T3513/T3565 — TS 24.501 §5.6.2.2.1):
  //   the timer is NOT restarted here.  The caller must send the paging/notify
  //   ITTI message first, and only on send success call start_timer() for the
  //   next attempt.  On send failure the caller invokes the terminal path
  //   directly (PPF=false, drain queues, failure notify).
  //
  // Returns true  — should retransmit; caller MUST retransmit the NAS message.
  // Returns false — final expiry; caller MUST take the terminal action.
  bool handle_expiry(
      nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
      uint64_t amf_ue_ngap_id, bool auto_restart = true);

  // Stop all NAS procedure timers for a UE (call on deregistration / context
  // release to avoid stale ITTI timer callbacks).
  void stop_all_procedure_timers(std::shared_ptr<nas_context>& nc);

  // Retrieve the live interval (seconds) for a timer type.
  // Used by callers that need to log runtime-configured values.
  uint32_t get_interval_sec(nas_timer_type_e type) const;

  // Retrieve the live max_retransmissions for a timer type.
  uint8_t get_max_retransmissions(nas_timer_type_e type) const;

 private:
  itti_mw* itti_ = nullptr;

  // Runtime-initialised table — one entry per nas_timer_type_e value.
  // Indices MUST match nas_timer_type_e; see nas_timer_manager.cpp.
  nas_timer_config_t
      timer_configs_[static_cast<size_t>(nas_timer_type_e::NAS_TIMER_COUNT)]{};
};
