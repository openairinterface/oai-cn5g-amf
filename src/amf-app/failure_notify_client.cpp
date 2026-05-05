/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

// TS 23.502 §5.2.2.2.7A — N1N2TransferFailureNotification producer
// TS 29.518 §6.1.5.6   — N1N2MsgTxfrFailureNotification body schema

#include "failure_notify_client.hpp"

#include <atomic>
#include <nlohmann/json.hpp>
#include <thread>

#include "N1N2MessageTransferCause_anyOf.h"
#include "amf_statistics.hpp"
#include "http_definitions.hpp"
#include "logger.hpp"

extern statistics stacs;

namespace {
// Track G10 (cleanup pass): hard cap on concurrently running detached
// failure-notification threads.
//
// Design: std::atomic<int> with compare-exchange (C++17 compatible; the
// project targets -std=c++17 so std::counting_semaphore<N> / C++20 is not
// available).
//
// Cap value: 64 — chosen to be large enough for all but pathological
// multi-hundred-UE simultaneous T3513 timeout storms while remaining well
// below typical OS default per-process thread limits (~1000-32768).  The cap
// is intentionally generous; the expected steady-state in-flight count is
// O(1)-O(10) per deployment.
//
// Drop policy: if the cap is reached, send() logs a warning (SUPI + URI),
// increments failure_notify_dropped_total, and returns.  The notification is
// permanently dropped; TS 23.502 §5.2.2.2.7A places no retransmission
// obligation on the AMF for failure notifications.
constexpr int kFailureNotifyMaxInflight = 64;
std::atomic<int> g_inflight_notify_count{0};

// Map the enum value to the JSON string.  Mirrors the to_json() in
// N1N2MessageTransferCause_anyOf.cpp so we do not create a model object
// just to serialise the enum.
const char* cause_to_string(oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
                                eN1N2MessageTransferCause_anyOf cause) {
  using E = oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf;
  switch (cause) {
    case E::ATTEMPTING_TO_REACH_UE:
      return "ATTEMPTING_TO_REACH_UE";
    case E::N1_N2_TRANSFER_INITIATED:
      return "N1_N2_TRANSFER_INITIATED";
    case E::WAITING_FOR_ASYNCHRONOUS_TRANSFER:
      return "WAITING_FOR_ASYNCHRONOUS_TRANSFER";
    case E::UE_NOT_RESPONDING:
      return "UE_NOT_RESPONDING";
    case E::N1_MSG_NOT_TRANSFERRED:
      return "N1_MSG_NOT_TRANSFERRED";
    case E::N2_MSG_NOT_TRANSFERRED:
      return "N2_MSG_NOT_TRANSFERRED";
    case E::UE_NOT_REACHABLE_FOR_SESSION:
      return "UE_NOT_REACHABLE_FOR_SESSION";
    case E::TEMPORARY_REJECT_REGISTRATION_ONGOING:
      return "TEMPORARY_REJECT_REGISTRATION_ONGOING";
    case E::TEMPORARY_REJECT_HANDOVER_ONGOING:
      return "TEMPORARY_REJECT_HANDOVER_ONGOING";
    case E::REJECTION_DUE_TO_PAGING_RESTRICTION:
      return "REJECTION_DUE_TO_PAGING_RESTRICTION";
    case E::AN_NOT_RESPONDING:
      return "AN_NOT_RESPONDING";
    case E::FAILURE_CAUSE_UNSPECIFIED:
      return "FAILURE_CAUSE_UNSPECIFIED";
    default:
      return "FAILURE_CAUSE_UNSPECIFIED";
  }
}
}  // namespace

namespace oai::amf {

failure_notify_client::failure_notify_client(
    std::shared_ptr<oai::http::http_client> http_client)
    : http_(std::move(http_client)) {}

// ---------------------------------------------------------------------------
// send — build the TS 29.518 §6.1.5.6 body and POST it to the Trigger NF.
//
// Body fields sent (others from §6.1.5.6 omitted — out of current scope):
//   cause          — mandatory
//   maxWaitingTime — optional (ExtBufSupport / deferred-queue scenarios)
//   ngApCause      — optional diagnostic
// ---------------------------------------------------------------------------
void failure_notify_client::send(
    const itti_n1n2_transfer_failure_notification& m) {
  if (m.failure_txf_notif_uri.empty()) {
    Logger::amf_sbi().debug(
        "failure_notify_client::send: no URI for SUPI %s — skipping",
        m.supi.c_str());
    return;
  }

  // Minimal syntactic URI validity check (C5 — no blocking getaddrinfo).
  // We require at least "http://" or "https://" followed by a non-empty host.
  // DNS resolution is deferred to http_client at send time (async timeout).
  {
    const auto& uri = m.failure_txf_notif_uri;
    const bool has_http =
        (uri.substr(0, 7) == "http://") || (uri.substr(0, 8) == "https://");
    const std::string::size_type auth_start =
        has_http ? uri.find("//") + 2 : std::string::npos;
    const bool has_host = has_http && (auth_start < uri.size()) &&
                          (uri[auth_start] != '/') && (uri[auth_start] != '\0');
    if (!has_http || !has_host) {
      Logger::amf_sbi().warn(
          "failure_notify_client::send: malformed URI '%s' for SUPI %s — "
          "skipping (TS 29.518 §6.1.5.6)",
          uri.c_str(), m.supi.c_str());
      stacs.increment_paging_failure_notify_failed();
      return;
    }
  }

  // Build the TS 29.518 §6.1.5.6 N1N2MsgTxfrFailureNotification body.
  // The OpenAPI-generated model class is minimal (cause + n1n2MsgDataUri only)
  // so we hand-roll the JSON to include maxWaitingTime and ngApCause.
  nlohmann::json body;
  body["cause"] = cause_to_string(m.cause);

  if (m.max_waiting_time.has_value()) {
    body["maxWaitingTime"] = m.max_waiting_time.value();
  }

  if (m.ng_ap_cause.has_value() && !m.ng_ap_cause.value().empty()) {
    // ng_ap_cause is stored as "group/value" — split on '/'
    const std::string& raw = m.ng_ap_cause.value();
    const auto slash_pos   = raw.find('/');
    nlohmann::json ng_ap_cause_obj;
    if (slash_pos != std::string::npos) {
      ng_ap_cause_obj["group"] = raw.substr(0, slash_pos);
      try {
        ng_ap_cause_obj["value"] = std::stoi(raw.substr(slash_pos + 1));
      } catch (...) {
        ng_ap_cause_obj["value"] = raw.substr(slash_pos + 1);
      }
    } else {
      ng_ap_cause_obj["group"] = raw;
    }
    body["ngApCause"] = ng_ap_cause_obj;
  }

  const std::string body_str = body.dump();

  Logger::amf_sbi().info(
      "[Track C] N1N2TransferFailureNotification: POST %s  cause=%s  supi=%s",
      m.failure_txf_notif_uri.c_str(), cause_to_string(m.cause),
      m.supi.c_str());
  Logger::amf_sbi().debug(
      "[Track C] N1N2TransferFailureNotification body: %s", body_str.c_str());

  // Track C refine 1 (FAIL-1 fix): dispatch the blocking HTTP POST on a
  // detached thread so TASK_AMF_SBI is not stalled for up to
  // http_request_timeout ms per fan-out notification.
  //
  // Note: send_async_http_request() (invoked when the http_client was
  // constructed with request_type_e::ASYNC) still calls cpr::AsyncResponse
  // .get() internally and therefore blocks the caller.  The only truly
  // non-blocking path is a detached thread.  All captured values are copied
  // by value so there is no lifetime hazard: http_ is a shared_ptr (ref-
  // counted), uri/body/supi are std::string copies.
  // (TS 23.502 §5.2.2.2.7A)
  //
  // Track G10 (cleanup pass): attempt to increment the in-flight counter
  // before spawning.  If the cap (kFailureNotifyMaxInflight=64) is already
  // reached, drop the notification, log a warning, and update the dropped
  // counter instead of spawning a thread that could exhaust OS handles.
  // Use compare-exchange (C++17, no counting_semaphore) so that the counter
  // is only incremented when below the cap — no separate decrement needed on
  // the cap-reached path.
  {
    int current = g_inflight_notify_count.load(std::memory_order_relaxed);
    while (true) {
      if (current >= kFailureNotifyMaxInflight) {
        Logger::amf_sbi().warn(
            "[Track G10] N1N2TransferFailureNotification: in-flight cap (%d) "
            "reached — dropping notification for supi=%s uri=%s",
            kFailureNotifyMaxInflight, m.supi.c_str(),
            m.failure_txf_notif_uri.c_str());
        stacs.increment_paging_failure_notify_dropped();
        return;
      }
      if (g_inflight_notify_count.compare_exchange_weak(
              current, current + 1, std::memory_order_acquire,
              std::memory_order_relaxed)) {
        break;
      }
      // compare_exchange_weak reloaded current on failure — retry.
    }
  }

  std::string captured_uri                         = m.failure_txf_notif_uri;
  std::string captured_supi                        = m.supi;
  std::shared_ptr<oai::http::http_client> http_ref = http_;

  std::thread([http_ref, captured_uri, captured_supi, body_str]() mutable {
    // Track G10: always decrement on exit regardless of outcome so the slot
    // is released for the next notification.
    struct InFlightGuard {
      ~InFlightGuard() {
        g_inflight_notify_count.fetch_sub(1, std::memory_order_release);
      }
    } guard;

    try {
      oai::http::request http_request =
          http_ref->prepare_json_request(captured_uri, body_str);
      auto http_response = http_ref->send_http_request(
          oai::common::sbi::method_e::POST, http_request);

      const auto status_code = static_cast<uint32_t>(http_response.status_code);

      if (status_code == static_cast<uint32_t>(
                             oai::common::sbi::http_status_code::NO_RESPONSE)) {
        Logger::amf_sbi().error(
            "[Track C] N1N2TransferFailureNotification: no response from %s "
            "(supi=%s)",
            captured_uri.c_str(), captured_supi.c_str());
        stacs.increment_paging_failure_notify_failed();
        return;
      }

      const bool success = (status_code >= 200) && (status_code < 300);
      if (!success) {
        Logger::amf_sbi().warn(
            "[Track C] N1N2TransferFailureNotification: HTTP %u from %s "
            "(supi=%s)",
            status_code, captured_uri.c_str(), captured_supi.c_str());
        stacs.increment_paging_failure_notify_failed();
        return;
      }

      Logger::amf_sbi().info(
          "[Track C] N1N2TransferFailureNotification: HTTP %u from %s "
          "(supi=%s) — sent",
          status_code, captured_uri.c_str(), captured_supi.c_str());
      stacs.increment_paging_failure_notify_sent();

    } catch (const std::exception& ex) {
      Logger::amf_sbi().error(
          "[Track C] N1N2TransferFailureNotification: exception posting to %s "
          "(supi=%s): %s",
          captured_uri.c_str(), captured_supi.c_str(), ex.what());
      stacs.increment_paging_failure_notify_failed();
    } catch (...) {
      Logger::amf_sbi().error(
          "[Track C] N1N2TransferFailureNotification: unknown exception "
          "posting to %s (supi=%s)",
          captured_uri.c_str(), captured_supi.c_str());
      stacs.increment_paging_failure_notify_failed();
    }
  }).detach();
}

}  // namespace oai::amf
