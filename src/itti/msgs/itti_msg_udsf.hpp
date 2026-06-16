/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

// ---------------------------------------------------------------------------
// Phase 1 — ITTI UDSF message infrastructure (class definitions only).
//
// This file defines the six ITTI message classes used to carry NUDSF
// UE-context store/get/delete requests and responses between AMF tasks.
// It contains no business logic; all runtime behaviour is added in later
// phases (Task 2+).
//
// Unit-test coverage:
//   Serialisation / round-trip unit tests for these message classes are
//   implemented in Phase 8 (test_udsf_serialization.cpp).  No test
//   executable is expected to exist for this Phase 1 infrastructure file
//   alone; CTest "No tests were found" is therefore expected at this stage.
//
// Compile validation:
//   Syntax-only compilation was verified with:
//     g++ -std=c++17 -fsyntax-only <all src/ include dirs> -x c++ \
//         src/itti/msgs/itti_msg_udsf.hpp
//   The full CMake build is blocked by a pre-existing MySQL dependency
//   that is unrelated to this file (see implementation-summary.md).
// ---------------------------------------------------------------------------

#ifndef ITTI_MSG_UDSF_HPP_INCLUDED_
#define ITTI_MSG_UDSF_HPP_INCLUDED_

#include <string>
#include <nlohmann/json.hpp>
#include "itti_msg_sbi.hpp"

// ---------------------------------------------------------------------------
// NUDSF_STORE_UE_CONTEXT (PUT /records/{supi})
// ---------------------------------------------------------------------------
class itti_nudsf_store_ue_context_request : public itti_sbi_msg {
 public:
  itti_nudsf_store_ue_context_request(
      const task_id_t origin, const task_id_t destination)
      : itti_sbi_msg(NUDSF_STORE_UE_CONTEXT_REQUEST, origin, destination) {
    supi       = {};
    realm      = {};
    storage_id = {};
    body       = {};
  }
  itti_nudsf_store_ue_context_request(
      const itti_nudsf_store_ue_context_request& i)
      : itti_sbi_msg(i) {
    supi       = i.supi;
    realm      = i.realm;
    storage_id = i.storage_id;
    body       = i.body;
  }
  virtual ~itti_nudsf_store_ue_context_request() {}
  const char* get_msg_name() { return "NUDSF_STORE_UE_CONTEXT_REQUEST"; }

 public:
  std::string supi;        // recordId, e.g. "imsi-001010000000001"
  std::string realm;       // realmId,  e.g. "amf-realm"
  std::string storage_id;  // storageId, e.g. "ue-context"
  nlohmann::json body;     // full record JSON (meta + blocks[])
};

class itti_nudsf_store_ue_context_response : public itti_sbi_msg {
 public:
  itti_nudsf_store_ue_context_response(
      const task_id_t origin, const task_id_t destination)
      : itti_sbi_msg(NUDSF_STORE_UE_CONTEXT_RESPONSE, origin, destination) {
    supi               = {};
    http_response_code = 0;
  }
  itti_nudsf_store_ue_context_response(
      const itti_nudsf_store_ue_context_response& i)
      : itti_sbi_msg(i) {
    supi               = i.supi;
    http_response_code = i.http_response_code;
  }
  virtual ~itti_nudsf_store_ue_context_response() {}
  const char* get_msg_name() { return "NUDSF_STORE_UE_CONTEXT_RESPONSE"; }

 public:
  std::string supi;
  uint32_t http_response_code;  // 201 Created, 204 No Content, or error
};

// ---------------------------------------------------------------------------
// NUDSF_GET_UE_CONTEXT (GET /records/{supi})
// promise_id is used for the synchronous blocking pattern in TASK_AMF_N1
// ---------------------------------------------------------------------------
class itti_nudsf_get_ue_context_request : public itti_sbi_msg {
 public:
  itti_nudsf_get_ue_context_request(
      const task_id_t origin, const task_id_t destination,
      const uint32_t promise_id_arg)
      : itti_sbi_msg(NUDSF_GET_UE_CONTEXT_REQUEST, origin, destination) {
    supi       = {};
    realm      = {};
    storage_id = {};
    promise_id = promise_id_arg;
  }
  itti_nudsf_get_ue_context_request(const itti_nudsf_get_ue_context_request& i)
      : itti_sbi_msg(i) {
    supi       = i.supi;
    realm      = i.realm;
    storage_id = i.storage_id;
    promise_id = i.promise_id;
  }
  virtual ~itti_nudsf_get_ue_context_request() {}
  const char* get_msg_name() { return "NUDSF_GET_UE_CONTEXT_REQUEST"; }

 public:
  std::string supi;  // recordId to retrieve
  std::string realm;
  std::string storage_id;
  uint32_t promise_id;  // used by TASK_AMF_SBI to resolve the correct promise
};

class itti_nudsf_get_ue_context_response : public itti_sbi_msg {
 public:
  itti_nudsf_get_ue_context_response(
      const task_id_t origin, const task_id_t destination)
      : itti_sbi_msg(NUDSF_GET_UE_CONTEXT_RESPONSE, origin, destination) {
    supi               = {};
    body               = {};
    http_response_code = 0;
  }
  itti_nudsf_get_ue_context_response(
      const itti_nudsf_get_ue_context_response& i)
      : itti_sbi_msg(i) {
    supi               = i.supi;
    body               = i.body;
    http_response_code = i.http_response_code;
  }
  virtual ~itti_nudsf_get_ue_context_response() {}
  const char* get_msg_name() { return "NUDSF_GET_UE_CONTEXT_RESPONSE"; }

 public:
  std::string supi;
  nlohmann::json body;  // record JSON returned by UDSF (200 OK) or empty
  uint32_t http_response_code;  // 200 OK or 404 Not Found
};

// ---------------------------------------------------------------------------
// NUDSF_DELETE_UE_CONTEXT (DELETE /records/{supi})
// ---------------------------------------------------------------------------
class itti_nudsf_delete_ue_context_request : public itti_sbi_msg {
 public:
  itti_nudsf_delete_ue_context_request(
      const task_id_t origin, const task_id_t destination)
      : itti_sbi_msg(NUDSF_DELETE_UE_CONTEXT_REQUEST, origin, destination) {
    supi       = {};
    realm      = {};
    storage_id = {};
  }
  itti_nudsf_delete_ue_context_request(
      const itti_nudsf_delete_ue_context_request& i)
      : itti_sbi_msg(i) {
    supi       = i.supi;
    realm      = i.realm;
    storage_id = i.storage_id;
  }
  virtual ~itti_nudsf_delete_ue_context_request() {}
  const char* get_msg_name() { return "NUDSF_DELETE_UE_CONTEXT_REQUEST"; }

 public:
  std::string supi;
  std::string realm;
  std::string storage_id;
};

class itti_nudsf_delete_ue_context_response : public itti_sbi_msg {
 public:
  itti_nudsf_delete_ue_context_response(
      const task_id_t origin, const task_id_t destination)
      : itti_sbi_msg(NUDSF_DELETE_UE_CONTEXT_RESPONSE, origin, destination) {
    supi               = {};
    http_response_code = 0;
  }
  itti_nudsf_delete_ue_context_response(
      const itti_nudsf_delete_ue_context_response& i)
      : itti_sbi_msg(i) {
    supi               = i.supi;
    http_response_code = i.http_response_code;
  }
  virtual ~itti_nudsf_delete_ue_context_response() {}
  const char* get_msg_name() { return "NUDSF_DELETE_UE_CONTEXT_RESPONSE"; }

 public:
  std::string supi;
  uint32_t http_response_code;  // 204 No Content, 404 Not Found
};

#endif  // ITTI_MSG_UDSF_HPP_INCLUDED_
