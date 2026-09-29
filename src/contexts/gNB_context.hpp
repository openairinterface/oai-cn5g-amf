/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _GNB_CONTEXT_H_
#define _GNB_CONTEXT_H_

#include <vector>

#include "3gpp_23.003.h"
#include "UeRetentionInformation.hpp"
#include "NgapIesStruct.hpp"
#include "sctp_server.hpp"
#include "SupportedTaItem.hpp"

extern "C" {
#include "Ngap_PagingDRX.h"
#include "bstrlib.h"
}

typedef enum {
  NGAP_INIT,
  NGAP_RESETING,
  NGAP_READY,
  NGAP_SHUTDOWN
} ng_gnb_state_t;

static const std::vector<std::string> ng_gnb_state_str = {
    "NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};

class gnb_context {
 public:
  std::string gnb_name;
  uint32_t gnb_id;  // Global RAN Node ID

  ng_gnb_state_t ng_state;
  plmn_t plmn;
  e_Ngap_PagingDRX default_paging_drx;  // v32, v64, v128, v256
  std::vector<oai::ngap::SupportedTaItem> supported_ta_list;
  std::optional<oai::ngap::UeRetentionInformation> ue_retention_info;

  sctp::sctp_assoc_id_t sctp_assoc_id;
  sctp::sctp_stream_id_t next_sctp_stream;
  sctp::sctp_stream_id_t instreams;
  sctp::sctp_stream_id_t outstreams;
};

#endif
