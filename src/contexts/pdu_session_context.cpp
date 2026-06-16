/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "pdu_session_context.hpp"

#include <nlohmann/json.hpp>

#include "conversions.hpp"

//------------------------------------------------------------------------------
pdu_session_context::pdu_session_context() {
  is_n2sm_available       = false;
  is_n1sm_available       = false;
  ran_ue_ngap_id          = 0;
  amf_ue_ngap_id          = INVALID_AMF_UE_NGAP_ID;
  req_type                = 0;
  pdu_session_id          = 0;
  n2sm                    = nullptr;
  n1sm                    = nullptr;
  smf_info.info_available = false;
  smf_info.addr           = {};
  smf_info.api_version    = "v1";
  smf_info.port           = DEFAULT_HTTP2_PORT;
  snssai                  = {};
  plmn                    = {};
  is_ho_accepted          = false;
  up_cnx_state            = up_cnx_state_e::UPCNX_STATE_UNKNOWN;
}

//------------------------------------------------------------------------------
pdu_session_context::~pdu_session_context() {}

//------------------------------------------------------------------------------
nlohmann::json pdu_session_context::to_json() const {
  nlohmann::json j;
  j["ran_ue_ngap_id"]    = ran_ue_ngap_id;
  j["amf_ue_ngap_id"]    = amf_ue_ngap_id;
  j["req_type"]          = req_type;
  j["pdu_session_id"]    = pdu_session_id;
  j["is_n2sm_available"] = is_n2sm_available;
  j["is_n1sm_available"] = is_n1sm_available;
  j["dnn"]               = dnn;
  j["is_ho_accepted"]    = is_ho_accepted;
  j["up_cnx_state"]      = static_cast<int>(up_cnx_state);

  if (is_n2sm_available && n2sm != nullptr) {
    std::string n2sm_hex;
    oai::utils::conv::convert_bstring_2_hex(n2sm, n2sm_hex);
    j["n2sm"] = n2sm_hex;
  } else {
    j["n2sm"] = nullptr;
  }
  if (is_n1sm_available && n1sm != nullptr) {
    std::string n1sm_hex;
    oai::utils::conv::convert_bstring_2_hex(n1sm, n1sm_hex);
    j["n1sm"] = n1sm_hex;
  } else {
    j["n1sm"] = nullptr;
  }

  j["smf_info"]["info_available"]   = smf_info.info_available;
  j["smf_info"]["addr"]             = smf_info.addr;
  j["smf_info"]["port"]             = smf_info.port;
  j["smf_info"]["uri_root"]         = smf_info.uri_root;
  j["smf_info"]["api_version"]      = smf_info.api_version;
  j["smf_info"]["context_location"] = smf_info.context_location;

  j["snssai"]["sst"] = snssai.sst;
  j["snssai"]["sd"]  = snssai.sd;

  j["plmn"]["mcc"] = plmn.mcc;
  j["plmn"]["mnc"] = plmn.mnc;

  return j;
}

//------------------------------------------------------------------------------
void pdu_session_context::from_json(const nlohmann::json& j) {
  if (j.contains("ran_ue_ngap_id"))
    ran_ue_ngap_id = j["ran_ue_ngap_id"].get<uint32_t>();
  if (j.contains("amf_ue_ngap_id"))
    amf_ue_ngap_id = j["amf_ue_ngap_id"].get<uint64_t>();
  if (j.contains("req_type")) req_type = j["req_type"].get<uint8_t>();
  if (j.contains("pdu_session_id"))
    pdu_session_id = j["pdu_session_id"].get<uint8_t>();
  if (j.contains("dnn")) dnn = j["dnn"].get<std::string>();
  if (j.contains("is_ho_accepted"))
    is_ho_accepted = j["is_ho_accepted"].get<bool>();
  if (j.contains("up_cnx_state"))
    up_cnx_state = static_cast<up_cnx_state_e>(j["up_cnx_state"].get<int>());

  if (n1sm) bdestroy(n1sm);
  n1sm              = nullptr;
  is_n1sm_available = false;
  if (n2sm) bdestroy(n2sm);
  n2sm              = nullptr;
  is_n2sm_available = false;

  if (j.contains("smf_info")) {
    const auto& si = j["smf_info"];
    if (si.contains("info_available"))
      smf_info.info_available = si["info_available"].get<bool>();
    if (si.contains("addr")) smf_info.addr = si["addr"].get<std::string>();
    if (si.contains("port")) smf_info.port = si["port"].get<uint32_t>();
    if (si.contains("uri_root"))
      smf_info.uri_root = si["uri_root"].get<std::string>();
    if (si.contains("api_version"))
      smf_info.api_version = si["api_version"].get<std::string>();
    if (si.contains("context_location"))
      smf_info.context_location = si["context_location"].get<std::string>();
  }

  if (j.contains("snssai")) {
    if (j["snssai"].contains("sst"))
      snssai.sst = j["snssai"]["sst"].get<uint8_t>();
    if (j["snssai"].contains("sd"))
      snssai.sd = j["snssai"]["sd"].get<std::string>();
  }

  if (j.contains("plmn")) {
    if (j["plmn"].contains("mcc"))
      plmn.mcc = j["plmn"]["mcc"].get<std::string>();
    if (j["plmn"].contains("mnc"))
      plmn.mnc = j["plmn"]["mnc"].get<std::string>();
  }
}
