/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nas_timer_manager.hpp"
#include "amf.hpp"

#include <string>

// itti_msg.hpp provides TASK_AMF_N1 (task_id_t enum value).
// The TASK_AMF_T35xx_TIMER_EXPIRE constants are defined in amf_app.hpp, but
// including that header would pull in heavy NGAP generated headers through
//   amf_app.hpp → itti_msg_amf_app.hpp → NgapIesStruct.hpp → …
// The macros expand to simple integers (6–12); their values are reproduced in
// the kTimerConfigs table below with matching comments.  Any change to those
// macros MUST be reflected here.
#include "itti_msg.hpp"
#include "logger.hpp"

// ---------------------------------------------------------------------------
// Static configuration table — one entry per nas_timer_type_e value.
// Indices MUST match the nas_timer_type_e enum in nas_timer_manager.hpp.
// The itti_task_id values match TASK_AMF_T35xx_TIMER_EXPIRE in amf_app.hpp.
// ---------------------------------------------------------------------------
const nas_timer_config_t
    kTimerConfigs[static_cast<size_t>(nas_timer_type_e::NAS_TIMER_COUNT)] = {
        // idx 0 — T3550: Registration Accept (§5.5.1.2.4)
        // itti_task_id = TASK_AMF_T3550_TIMER_EXPIRE = 6
        {nas_timer_type_e::T3550, 6, 4, 6u, "T3550"},
        // idx 1 — T3560: Auth Request / SMC (§5.4.1.3.7 / §5.4.2.7)
        // itti_task_id = TASK_AMF_T3560_TIMER_EXPIRE = 7
        {nas_timer_type_e::T3560, 6, 4, 7u, "T3560"},
        // idx 2 — T3570: Identity Request (§5.4.3.6)
        // itti_task_id = TASK_AMF_T3570_TIMER_EXPIRE = 8
        {nas_timer_type_e::T3570, 6, 4, 8u, "T3570"},
        // idx 3 — T3522: NW-initiated Deregistration Request (§5.5.2.3.5)
        // itti_task_id = TASK_AMF_T3522_TIMER_EXPIRE = 9
        {nas_timer_type_e::T3522, 6, 4, 9u, "T3522"},
        // idx 4 — T3555: Configuration Update Command (§5.4.4.6)
        // itti_task_id = TASK_AMF_T3555_TIMER_EXPIRE = 10
        {nas_timer_type_e::T3555, 6, 4, 10u, "T3555"},
        // idx 5 — T3513: Paging (§5.6.2.2.1); max_retransmissions=2 per plan §2
        // itti_task_id = TASK_AMF_T3513_TIMER_EXPIRE = 11
        {nas_timer_type_e::T3513, kPagingT3513IntervalSec,
         kPagingMaxRetransmissions, 11u, "T3513"},
        // idx 6 — T3565: Notification (§5.6.3)
        // itti_task_id = TASK_AMF_T3565_TIMER_EXPIRE = 12
        {nas_timer_type_e::T3565, 6, 4, 12u, "T3565"},
};

// ---------------------------------------------------------------------------
nas_timer_manager::nas_timer_manager(itti_mw* itti) : itti_(itti) {}

// ---------------------------------------------------------------------------
timer_id_t nas_timer_manager::start_timer(
    nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
    uint64_t amf_ue_ngap_id) {
  size_t idx      = static_cast<size_t>(type);
  const auto& cfg = kTimerConfigs[idx];

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

  if (!nc->nas_timers[idx].is_running) return;

  itti_->timer_remove(nc->nas_timers[idx].itti_timer_id);
  nc->nas_timers[idx].itti_timer_id        = ITTI_INVALID_TIMER_ID;
  nc->nas_timers[idx].is_running           = false;
  nc->nas_timers[idx].retransmission_count = 0;

  Logger::amf_n1().debug("NAS timer %s stopped", kTimerConfigs[idx].name);
}

// ---------------------------------------------------------------------------
bool nas_timer_manager::handle_expiry(
    nas_timer_type_e type, std::shared_ptr<nas_context>& nc,
    uint64_t amf_ue_ngap_id) {
  size_t idx      = static_cast<size_t>(type);
  const auto& cfg = kTimerConfigs[idx];

  nc->nas_timers[idx].is_running = false;
  nc->nas_timers[idx].retransmission_count++;

  if (cfg.max_retransmissions > 0 &&
      nc->nas_timers[idx].retransmission_count <= cfg.max_retransmissions) {
    // Restart ITTI timer — caller must retransmit the NAS message.
    timer_id_t tid = itti_->timer_setup(
        cfg.interval_sec, 0, TASK_AMF_N1, cfg.itti_task_id,
        std::to_string(amf_ue_ngap_id));
    nc->nas_timers[idx].itti_timer_id = tid;
    nc->nas_timers[idx].is_running    = true;

    Logger::amf_n1().debug(
        "NAS timer %s restarted (retx %u/%u, tid %u) for UE %lu", cfg.name,
        nc->nas_timers[idx].retransmission_count, cfg.max_retransmissions, tid,
        amf_ue_ngap_id);
    return true;  // Caller must retransmit
  }

  // Final expiry — reset counter and let caller take terminal action.
  nc->nas_timers[idx].retransmission_count = 0;

  Logger::amf_n1().debug(
      "NAS timer %s final expiry for UE %lu", cfg.name, amf_ue_ngap_id);
  return false;  // Caller must handle final action
}

// ---------------------------------------------------------------------------
void nas_timer_manager::stop_all_procedure_timers(
    std::shared_ptr<nas_context>& nc) {
  for (uint8_t i = 0;
       i < static_cast<uint8_t>(nas_timer_type_e::NAS_TIMER_COUNT); ++i) {
    stop_timer(static_cast<nas_timer_type_e>(i), nc);
  }
}
