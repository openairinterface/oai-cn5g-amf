/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

// R1 — shared SBI promise/wait helper.
//
// The "create promise -> store_promise -> send ITTI -> wait_for_result ->
// decode -> remove_promise" block was copy-pasted across every synchronous SBI
// handler (14 sites in amf_http2_server.cpp for HTTP/2 + 7 sites in
// src/sbi/impl/*ApiImpl.cpp for HTTP/1). This header declares one free function
// that performs that shared MIDDLE exactly once.
//
// SCOPE: Only the transport-agnostic promise/ITTI/wait/decode is extracted. The
// transport-specific response WRITE stays in each handler (send_response()/
// res.write_head()/res.end() for HTTP/2 vs Pistache response.send() for
// HTTP/1).
//
// The definition lives in amf_sbi_promise.cpp, which is explicitly listed in
// the file(GLOB ...) block of src/sbi/CMakeLists.txt.

#pragma once

#include <functional>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "itti_msg.hpp"

namespace amf_application {
class amf_app;
}

namespace oai::amf::sbi {

// Decoded result of a synchronous SBI request/response round-trip.
//
// - http_code:    extracted from kSbiResponseHttpResponseCode; equals
//                 GATEWAY_TIMEOUT (504) when no result was produced before the
//                 wait_for_result() timeout.
// - body:         the full result JSON object returned through the promise (the
//                 same object handlers previously indexed directly, e.g.
//                 kSbiResponseJsonData / "ProblemDetails"). Empty when timed
//                 out.
// - content_type: reserved field (kept per plan sketch); not populated here
//                 because each handler still picks its own Content-Type when it
//                 writes the transport response.
// - location:     extracted from kSbiResponseHeaderLocation (empty when
// absent).
struct sbi_result_t {
  // True when a result was delivered through the promise before the
  // wait_for_result() timeout; false on timeout. Distinguishes a genuine 504
  // response payload from the timeout path (where http_code is also set to
  // GATEWAY_TIMEOUT but body is empty).
  bool has_result          = false;
  uint32_t http_code       = 0;
  nlohmann::json body      = {};
  std::string content_type = {};
  std::string location     = {};
};

// Perform the shared SBI promise/ITTI/wait/decode round-trip.
//
// The caller supplies a factory that builds and fully populates the concrete
// ITTI message for the given promise_id (the id must be set on the message so
// the downstream task can route the response back to this promise). The helper:
//   1. creates a promise + future,
//   2. store_promise(promise_id, promise) (generates promise_id),
//   3. make_msg(promise_id) -> send_msg(),
//   4. wait_for_result() with the existing timeout semantics,
//   5. decodes the result into sbi_result_t (http_code / body / location),
//   6. remove_promise(promise_id),
//   7. returns the sbi_result_t.
//
// On timeout, http_code == GATEWAY_TIMEOUT (504) and body is empty.
sbi_result_t sbi_send_recv(
    amf_application::amf_app* app,
    const std::function<std::shared_ptr<itti_msg>(uint32_t promise_id)>&
        make_msg);

}  // namespace oai::amf::sbi
