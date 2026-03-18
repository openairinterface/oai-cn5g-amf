/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "pdu_session_context.hpp"

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
