/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#pragma once

#include <nghttp2/asio_http2_server.h>

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <nlohmann/json.hpp>
#include <string>

#include "amf_config.hpp"
#include "logger.hpp"
#include "ProblemDetails.h"

extern std::unique_ptr<oai::config::amf_config> amf_cfg;

namespace oai {
namespace amf {
namespace sbi {

// Maximum accepted request-body size
constexpr std::size_t kMaxRequestBodySize = 32768;

//------------------------------------------------------------------------------
// Parse a request body into a JSON object.
// Returns false (never throws) on malformed input.
inline bool parse_json_body(const std::string& body, nlohmann::json& out) {
  try {
    out = nlohmann::json::parse(body);
    return true;
  } catch (std::exception& e) {
    Logger::amf_server().warn(
        "Cannot parse the JSON data (error: %s)!", e.what());
    return false;
  } catch (...) {
    Logger::amf_server().warn("Cannot parse the JSON data (unknown error)!");
    return false;
  }
}

//------------------------------------------------------------------------------
// Parse a request body directly into a typed (OpenAPI model) object.
// Covers both json::parse and from_json/get_to failures (missing fields,
// wrong types). Returns false (never throws) on any failure.
template<typename T>
inline bool parse_json_body(const std::string& body, T& out) {
  try {
    nlohmann::json::parse(body).get_to(out);
    return true;
  } catch (std::exception& e) {
    Logger::amf_server().warn(
        "Cannot parse the JSON data (error: %s)!", e.what());
    return false;
  } catch (...) {
    Logger::amf_server().warn("Cannot parse the JSON data (unknown error)!");
    return false;
  }
}

//------------------------------------------------------------------------------
inline std::string problem_title_from_status(uint32_t status) {
  switch (status) {
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 411:
      return "Length Required";
    case 413:
      return "Payload Too Large";
    case 415:
      return "Unsupported Media Type";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 503:
      return "Service Unavailable";
    case 504:
      return "Gateway Timeout";
    default:
      return (status >= 500) ? "Server Error" : "Client Error";
  }
}

//------------------------------------------------------------------------------
inline std::string problem_cause_from_status(uint32_t status) {
  switch (status) {
    case 400:
      return "INVALID_MSG_FORMAT";
    case 401:
      return "UNAUTHORIZED";
    case 403:
      return "FORBIDDEN";
    case 404:
      return "NF_RESOURCE_NOT_FOUND";
    case 413:
      return "PAYLOAD_TOO_LARGE";
    case 415:
      return "UNSUPPORTED_MEDIA_TYPE";
    case 501:
      return "NOT_IMPLEMENTED";
    case 504:
      return "TIMED_OUT_REQUEST";
    default:
      return (status >= 500) ? "SYSTEM_FAILURE" : "MANDATORY_IE_INCORRECT";
  }
}

//------------------------------------------------------------------------------
// Build a minimal ProblemDetails JSON body for the given error status.
inline nlohmann::json build_problem_details(
    uint32_t status, const std::string& detail = {}) {
  oai::_3gpp::model::ProblemDetails problem_details;
  problem_details.setStatus(static_cast<int32_t>(status));
  problem_details.setTitle(problem_title_from_status(status));
  problem_details.setCause(problem_cause_from_status(status));
  if (!detail.empty()) problem_details.setDetail(detail);
  nlohmann::json json_data = {};
  to_json(json_data, problem_details);
  return json_data;
}

//------------------------------------------------------------------------------
class request_body_buffer {
 public:
  explicit request_body_buffer(std::size_t max_size = kMaxRequestBodySize)
      : m_max_size(max_size) {}

  // Append one DATA-frame chunk. Returns false exactly once — on the call
  // that first exceeds the ceiling (so the caller can reply 413 a single
  // time); any further chunks of a rejected body are discarded silently.
  bool append(const uint8_t* data, std::size_t len) {
    if (m_rejected) return true;  // already rejected/reported, discard
    if ((m_body.size() + len) > m_max_size) {
      m_rejected = true;
      m_body.clear();
      m_body.shrink_to_fit();
      return false;
    }
    m_body.append(reinterpret_cast<const char*>(data), len);
    return true;
  }

  // True when the body exceeded the ceiling (413 already sent by the caller).
  bool rejected() const { return m_rejected; }

  const std::string& body() const { return m_body; }

 private:
  std::string m_body;
  std::size_t m_max_size;
  bool m_rejected = false;
};

//------------------------------------------------------------------------------
// True when OAuth2 enforcement is explicitly enabled. Defaults to false.
inline bool oauth2_enforcement_enabled() {
  static const bool enabled = []() {
    const char* v = std::getenv("AMF_SBI_OAUTH2_ENABLE");
    if (v == nullptr) return false;
    const std::string s(v);
    return s == "1" || s == "true" || s == "TRUE" || s == "yes" || s == "on";
  }();
  return enabled;
}

//------------------------------------------------------------------------------
// Minimal base64url decoder (RFC 7515) for the JWT payload segment.
// Returns false on malformed input.
inline bool base64url_decode(const std::string& in, std::string& out) {
  static const auto val = [](char c) -> int {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '-') return 62;
    if (c == '_') return 63;
    return -1;
  };
  out.clear();
  int buffer = 0;
  int bits   = 0;
  for (char c : in) {
    if (c == '=') break;  // padding (usually absent in base64url)
    const int d = val(c);
    if (d < 0) return false;
    buffer = (buffer << 6) | d;
    bits += 6;
    if (bits >= 8) {
      bits -= 8;
      out.push_back(static_cast<char>((buffer >> bits) & 0xFF));
    }
  }
  return true;
}

//------------------------------------------------------------------------------
// Extract the "Bearer <token>" value from an Authorization header value.
// Returns false if the scheme is missing/not Bearer or the token is empty.
inline bool extract_bearer_token(
    const std::string& header_value, std::string& token) {
  const std::string prefix = "Bearer ";
  if (header_value.size() <= prefix.size()) return false;
  // Case-insensitive scheme compare.
  for (std::size_t i = 0; i < prefix.size(); ++i) {
    if (std::tolower(static_cast<unsigned char>(header_value[i])) !=
        std::tolower(static_cast<unsigned char>(prefix[i])))
      return false;
  }
  token = header_value.substr(prefix.size());
  // Trim surrounding whitespace.
  while (!token.empty() &&
         std::isspace(static_cast<unsigned char>(token.back())))
    token.pop_back();
  while (!token.empty() &&
         std::isspace(static_cast<unsigned char>(token.front())))
    token.erase(token.begin());
  return !token.empty();
}

//------------------------------------------------------------------------------
// Validate JWT structure + claims that are checkable without the signing key.
// NOTE: signature is NOT verified (see file-level TODO).
inline bool validate_access_token(
    const std::string& token, std::string& error_detail) {
  // Structural check: header.payload.signature
  const auto dot1 = token.find('.');
  if (dot1 == std::string::npos) {
    error_detail = "malformed access token (not a JWT)";
    return false;
  }
  const auto dot2 = token.find('.', dot1 + 1);
  if (dot2 == std::string::npos ||
      token.find('.', dot2 + 1) != std::string::npos) {
    error_detail = "malformed access token (expected 3 JWT segments)";
    return false;
  }

  const std::string payload_b64 = token.substr(dot1 + 1, dot2 - dot1 - 1);
  std::string payload_json;
  if (!base64url_decode(payload_b64, payload_json)) {
    error_detail = "malformed access token (payload not base64url)";
    return false;
  }

  nlohmann::json claims;
  try {
    claims = nlohmann::json::parse(payload_json);
  } catch (std::exception&) {
    error_detail = "malformed access token (payload not JSON)";
    return false;
  }

  // Expiry (RFC 7519 `exp`, seconds since epoch).
  if (!claims.contains("exp") || !claims["exp"].is_number()) {
    error_detail = "access token missing exp claim";
    return false;
  }
  const std::int64_t exp = claims["exp"].get<std::int64_t>();
  const std::int64_t now = static_cast<std::int64_t>(std::time(nullptr));
  if (exp < now) {
    error_detail = "access token expired";
    return false;
  }

  // Required OAuth2/CCA claims — presence check only for now.
  // TODO(spec): match issuer==NRF id, audience==this AMF, scope==service name.
  for (const char* required : {"iss", "aud", "scope"}) {
    if (!claims.contains(required)) {
      error_detail = std::string("access token missing ") + required + " claim";
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
// OAuth2 enforcement: returns true when the request may proceed (either
// enforcement is disabled, or the token passed the local checks). Returns false
// when the request has been rejected — in which case a 401 response has ALREADY
// been written to `res`.
inline bool authorize_request(
    const nghttp2::asio_http2::server::request& request,
    const nghttp2::asio_http2::server::response& res) {
  if (!oauth2_enforcement_enabled()) return true;  // pass-through (default)

  std::string error_detail;
  // nghttp2 lower-cases header field names.
  const auto it = request.header().find("authorization");
  std::string token;
  if (it == request.header().end()) {
    error_detail = "missing Authorization header";
  } else if (!extract_bearer_token(it->second.value, token)) {
    error_detail = "Authorization header is not a Bearer token";
  } else if (validate_access_token(token, error_detail)) {
    return true;  // authorized
  }

  Logger::amf_server().warn(
      "SBI OAuth2: rejecting request to %s (%s)", request.uri().path.c_str(),
      error_detail.c_str());

  nghttp2::asio_http2::header_map h;
  h.emplace(
      "www-authenticate", nghttp2::asio_http2::header_value{
                              "Bearer error=\"invalid_token\"", false});
  h.emplace(
      "content-type",
      nghttp2::asio_http2::header_value{"application/problem+json", false});
  const nlohmann::json body = build_problem_details(401, error_detail);
  res.write_head(401, h);
  res.end(body.dump().c_str());
  return false;
}

}  // namespace sbi
}  // namespace amf
}  // namespace oai
