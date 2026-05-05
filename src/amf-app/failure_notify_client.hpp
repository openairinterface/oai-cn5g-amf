/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once

#include <memory>

#include "http_client.hpp"
#include "itti_msg_amf_app.hpp"

namespace oai::amf {

// ---------------------------------------------------------------------------
// failure_notify_client
//
// Implements the TS 23.502 §5.2.2.2.7A / TS 29.518 §6.1.5.6
// N1N2TransferFailureNotification producer.
//
// Builds a spec-conformant JSON body (N1N2MsgTxfrFailureNotification schema)
// and POSTs it to the n1n2FailureTxfNotifURI supplied by the Trigger NF.
//
// -- Threading model (Track C refine 1 / Track G10 cleanup pass) --
//
// The HTTP POST is dispatched on a detached std::thread so that send()
// returns immediately without blocking TASK_AMF_SBI.  The http_client is
// constructed with request_type_e::ASYNC (dedicated instance, not the global
// SIMPLE singleton), but because cpr's AsyncResponse.get() still blocks, a
// detached thread is required for true non-blocking behaviour.  All data
// needed by the thread is captured by value.
//
// -- In-flight thread cap (Track G10 cleanup pass) --
//
// A file-scope std::atomic<int> (g_inflight_notify_count) limits concurrent
// detached threads to kFailureNotifyMaxInflight = 64.  The cap prevents OS
// thread-handle exhaustion during T3513 final-expiry fan-out storms (e.g.
// many UEs timing out simultaneously).
//
// Design choice: std::atomic<int> with compare-exchange (not
// std::counting_semaphore<64> which requires C++20; this codebase targets
// C++17 per -std=c++17 in CMakeLists).
//
// Drop policy: if the cap is reached, send() logs a warning with the SUPI
// and URI, increments failure_notify_dropped_total, and returns without
// spawning a thread.  The notification is permanently lost for this event;
// the Trigger NF's own retry / observation is out of AMF scope per
// TS 23.502 §5.2.2.2.7A.
//
// Callers in other tasks must route the notification via
// itti_n1n2_transfer_failure_notification (ITTI dequeue runs in
// TASK_AMF_SBI, which is the only thread that may call send()).
// ---------------------------------------------------------------------------
class failure_notify_client {
 public:
  explicit failure_notify_client(
      std::shared_ptr<oai::http::http_client> http_client);

  // Send a N1N2TransferFailureNotification for the given ITTI message.
  // Must be called from TASK_AMF_SBI.
  void send(const itti_n1n2_transfer_failure_notification& m);

 private:
  std::shared_ptr<oai::http::http_client> http_;
};

}  // namespace oai::amf
