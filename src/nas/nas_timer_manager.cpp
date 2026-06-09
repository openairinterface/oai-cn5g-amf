/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nas_timer_manager.hpp"
#include "amf.hpp"
#include "amf_config.hpp"

#include <memory>
#include <string>

// itti_msg.hpp provides TASK_AMF_N1 (task_id_t enum value).
// The TASK_AMF_T35xx_TIMER_EXPIRE constants are defined in amf_app.hpp, but
// including that header would pull in heavy NGAP generated headers through
//   amf_app.hpp → itti_msg_amf_app.hpp → NgapIesStruct.hpp → …
// The macros expand to simple integers (6–12); their values are reproduced in
// the timer_configs_ table below with matching comments.  Any change to those
// macros MUST be reflected here.
#include "itti_msg.hpp"
#include "logger.hpp"

// Global amf_config — defined in main.cpp, available by the time the
// nas_timer_manager constructor runs (amf_cfg is initialised before amf_app).
extern std::unique_ptr<oai::config::amf_config> amf_cfg;

// ---------------------------------------------------------------------------
nas_timer_manager::nas_timer_manager(itti_mw* itti) : itti_(itti) {
  // Non-paging timers: compile-time defaults per 3GPP Table 10.2.2.
  // Indices MUST match the nas_timer_type_e enum in nas_timer_manager.hpp.
  // The itti_task_id values match TASK_AMF_T35xx_TIMER_EXPIRE in amf_app.hpp.

  // idx 0 — T3550: Registration Accept (§5.5.1.2.4)
  // itti_task_id = TASK_AMF_T3550_TIMER_EXPIRE = 6
  timer_configs_[0] = {nas_timer_type_e::T3550, 6, 4, 6u, "T3550"};

  // idx 1 — T3560: Auth Request / SMC (§5.4.1.3.7 / §5.4.2.7)
  // itti_task_id = TASK_AMF_T3560_TIMER_EXPIRE = 7
  timer_configs_[1] = {nas_timer_type_e::T3560, 6, 4, 7u, "T3560"};

  // idx 2 — T3570: Identity Request (§5.4.3.6)
  // itti_task_id = TASK_AMF_T3570_TIMER_EXPIRE = 8
  timer_configs_[2] = {nas_timer_type_e::T3570, 6, 4, 8u, "T3570"};

  // idx 3 — T3522: NW-initiated Deregistration Request (§5.5.2.3.5)
  // itti_task_id = TASK_AMF_T3522_TIMER_EXPIRE = 9
  timer_configs_[3] = {nas_timer_type_e::T3522, 6, 4, 9u, "T3522"};

  // idx 4 — T3555: Configuration Update Command (§5.4.4.6)
  // itti_task_id = TASK_AMF_T3555_TIMER_EXPIRE = 10
  timer_configs_[4] = {nas_timer_type_e::T3555, 6, 4, 10u, "T3555"};

  // idx 5 — T3513: Paging (§5.6.2.2.1) — runtime-configurable
  // itti_task_id = TASK_AMF_T3513_TIMER_EXPIRE = 11
  // Defensive defaults: fall back to compile-time constants when amf_cfg is
  // not yet available (unit tests, early startup).
  {
    uint32_t interval_sec =
        (amf_cfg && amf_cfg->paging.t3513_interval_sec != 0) ?
            amf_cfg->paging.t3513_interval_sec :
            kPagingT3513IntervalSec;
    uint8_t max_retx =
        (amf_cfg) ?
            static_cast<uint8_t>(amf_cfg->paging.t3513_max_retransmissions) :
            kPagingMaxRetransmissions;
    timer_configs_[5] = {
        nas_timer_type_e::T3513, interval_sec, max_retx, 11u, "T3513"};
  }

  // idx 6 — T3565: Notification (§5.6.3) — runtime-configurable
  // itti_task_id = TASK_AMF_T3565_TIMER_EXPIRE = 12
  {
    uint32_t interval_sec =
        (amf_cfg && amf_cfg->paging.t3565_interval_sec != 0) ?
            amf_cfg->paging.t3565_interval_sec :
            kPagingT3565IntervalSec;
    uint8_t max_retx =
        (amf_cfg) ?
            static_cast<uint8_t>(amf_cfg->paging.t3565_max_retransmissions) :
            kPagingT3565MaxRetransmissions;
    timer_configs_[6] = {
        nas_timer_type_e::T3565, interval_sec, max_retx, 12u, "T3565"};
  }

  Logger::amf_n1().debug(
      "NAS timer manager initialised: T3513=%us max_retx=%u, T3565=%us "
      "max_retx=%u",
      timer_configs_[5].interval_sec, timer_configs_[5].max_retransmissions,
      timer_configs_[6].interval_sec, timer_configs_[6].max_retransmissions);
}

// ---------------------------------------------------------------------------
timer_id_t nas_timer_manager::start_timer(
    nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
    uint64_t amf_ue_ngap_id) {
  size_t idx      = static_cast<size_t>(type);
  const auto& cfg = timer_configs_[idx];

  // If the same timer is already running, cancel it first.
  if (nc->nas_timers[idx].is_running) {
    stop_timer(type, nc);
  }

  // CPI-state mutual exclusivity: T3550 / T3560 / T3570 cannot
  // run simultaneously — stop the other two before starting this one.
  if (type == nas_timer_type_e::T3550 || type == nas_timer_type_e::T3560 ||
      type == nas_timer_type_e::T3570) {
    if (type != nas_timer_type_e::T3550)
      stop_timer(nas_timer_type_e::T3550, nc);
    if (type != nas_timer_type_e::T3560)
      stop_timer(nas_timer_type_e::T3560, nc);
    if (type != nas_timer_type_e::T3570)
      stop_timer(nas_timer_type_e::T3570, nc);
  }

  // Reset retransmission counter for a fresh start.
  nc->nas_timers[idx].retransmission_count = 0;

  timer_id_t tid = itti_->timer_setup(
      cfg.interval_sec, 0, TASK_AMF_N1, cfg.itti_task_id,
      std::to_string(amf_ue_ngap_id));

  nc->nas_timers[idx].itti_timer_id = tid;
  nc->nas_timers[idx].is_running    = true;

  Logger::amf_n1().debug(
      "NAS timer %s started (tid %u) for UE %lu", cfg.name, tid,
      amf_ue_ngap_id);
  return tid;
}

// ---------------------------------------------------------------------------
void nas_timer_manager::stop_timer(
    nas_timer_type_e type, std::shared_ptr<nas_context>& nc) {
  size_t idx = static_cast<size_t>(type);

  if (!nc->nas_timers[idx].is_running &&
      nc->nas_timers[idx].itti_timer_id == ITTI_INVALID_TIMER_ID) {
    return;
  }

  if (nc->nas_timers[idx].is_running) {
    itti_->timer_remove(nc->nas_timers[idx].itti_timer_id);
  }
  nc->nas_timers[idx].itti_timer_id        = ITTI_INVALID_TIMER_ID;
  nc->nas_timers[idx].is_running           = false;
  nc->nas_timers[idx].retransmission_count = 0;

  Logger::amf_n1().debug("NAS timer %s stopped", timer_configs_[idx].name);
}

// ---------------------------------------------------------------------------
// handle_expiry — increment retransmission counter and optionally restart the
// ITTI timer.
//
// auto_restart = true  (T3550/T3560/T3570/T3522/T3555): restarts the timer
//   before returning true so the caller only needs to re-send the message.
//
// auto_restart = false (T3513/T3565 — TS 24.501 §5.6.2.2.1): does NOT restart
//   the timer.  The caller must send the ITTI paging/notify message FIRST and
//   only on send success call start_timer() to arm the next attempt.  On send
//   failure the caller invokes the terminal path (PPF=false, drain queues).
//   This ordering prevents stale timer ticks when the ITTI send fails.
// ---------------------------------------------------------------------------
bool nas_timer_manager::handle_expiry(
    nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
    uint64_t amf_ue_ngap_id, bool auto_restart) {
  size_t idx      = static_cast<size_t>(type);
  const auto& cfg = timer_configs_[idx];

  nc->nas_timers[idx].is_running = false;
  nc->nas_timers[idx].retransmission_count++;

  if (cfg.max_retransmissions > 0 &&
      nc->nas_timers[idx].retransmission_count <= cfg.max_retransmissions) {
    if (auto_restart) {
      // Restart ITTI timer — caller must retransmit the NAS message.
      timer_id_t tid = itti_->timer_setup(
          cfg.interval_sec, 0, TASK_AMF_N1, cfg.itti_task_id,
          std::to_string(amf_ue_ngap_id));
      nc->nas_timers[idx].itti_timer_id = tid;
      nc->nas_timers[idx].is_running    = true;

      Logger::amf_n1().debug(
          "NAS timer %s restarted (retx %u/%u, tid %u) for UE %lu", cfg.name,
          nc->nas_timers[idx].retransmission_count, cfg.max_retransmissions,
          tid, amf_ue_ngap_id);
    } else {
      // Caller owns timer restart — timer remains stopped until send succeeds.
      Logger::amf_n1().debug(
          "NAS timer %s retx %u/%u for UE %lu — awaiting send before restart",
          cfg.name, nc->nas_timers[idx].retransmission_count,
          cfg.max_retransmissions, amf_ue_ngap_id);
    }
    return true;  // Caller must retransmit
  }

  // Final expiry — reset counter and let caller take terminal action.
  nc->nas_timers[idx].retransmission_count = 0;

  Logger::amf_n1().debug(
      "NAS timer %s final expiry for UE %lu", cfg.name, amf_ue_ngap_id);
  return false;  // Caller must handle final action
}

// ---------------------------------------------------------------------------
uint32_t nas_timer_manager::get_interval_sec(nas_timer_type_e type) const {
  return timer_configs_[static_cast<size_t>(type)].interval_sec;
}

// ---------------------------------------------------------------------------
uint8_t nas_timer_manager::get_max_retransmissions(
    nas_timer_type_e type) const {
  return timer_configs_[static_cast<size_t>(type)].max_retransmissions;
}

// ---------------------------------------------------------------------------
void nas_timer_manager::stop_all_procedure_timers(
    std::shared_ptr<nas_context>& nc) {
  for (uint8_t i = 0;
       i < static_cast<uint8_t>(nas_timer_type_e::NAS_TIMER_COUNT); ++i) {
    stop_timer(static_cast<nas_timer_type_e>(i), nc);
  }
}
