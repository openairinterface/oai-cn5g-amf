/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file 3gpp_conversions.cpp
 * \brief
 * \author Shivam Gandhi
 * \company KCL
 * \email: shivam.gandhi@kcl.ac.uk
 */
#include "3gpp_conversions.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include "3gpp_29.500.h"
#include "3gpp_24.501.h"
#include "conversions.hpp"

//------------------------------------------------------------------------------
void xgpp_conv::data_notification_from_openapi(
    const oai::amf::model::NotificationData& nd,
    amf::data_notification_msg& dn_msg) {
  Logger::amf_app().debug(
      "Convert NotificationData (OpenAPI) to "
      "Data Notification Msg");

  //dn_msg.set_notification_event_type(nd.getEvent());
  //dn_msg.set_nf_instance_uri(nd.getNfInstanceUri());

  std::shared_ptr<amf_application::nf_profile> p = {};

  // Only support UPF for now
  if (nd.getNfProfile().getNfType() == "UPF")
    p = std::make_shared<amf_application::upf_profile>();

  nlohmann::json pj = {};
  to_json(pj, nd.getNfProfile());
  p.get()->from_json(pj);
  //dn_msg.set_profile(p);
}

//------------------------------------------------------------------------------
void xgpp_conv::amf_event_exposure_notification_from_openapi(
    const oai::amf::model::NamfEventExposure& nee,
    amf::event_exposure_msg& eem) {
  Logger::amf_app().debug(
      "Convert NamfEventExposure (OpenAPI) to "
      "Event Exposure Msg");

  // Supi
  if (nee.supiIsSet()) {
    supi_t supi             = {.length = 0};
    std::size_t pos         = nee.getSupi().find("-");
    std::string supi_str    = nee.getSupi().substr(pos + 1);
    std::string supi_prefix = nee.getSupi().substr(0, pos);
    amf_string_to_supi(&supi, supi_str.c_str());

    //eem.set_supi(supi);
    //eem.set_supi_prefix(supi_prefix);
    Logger::amf_server().debug(
        "SUPI %s, SUPI Prefix %s, IMSI %s", nee.getSupi().c_str(),
        supi_prefix.c_str(), supi_str.c_str());
  }

  // PDU session ID
  //if (nee.pduSeIdIsSet()) {
  //  Logger::amf_server().debug("PDU Session ID %d", nee.getPduSeId());
  //  eem.set_pdu_session_id(nee.getPduSeId());
  //}

  //eem.set_notif_id(nee.getNotifId());    // NotifId
  //eem.set_notif_uri(nee.getNotifUri());  // NotifUri

  // EventSubscription: TODO
  event_subscription_t event_subscription = {};
  event_subscription.amf_event            = amf_event_t::AMF_EVENT_REACH_ST;
  std::vector<event_subscription_t> event_subscriptions = {};
  event_subscriptions.push_back(event_subscription);
  //eem.set_event_subs(event_subscriptions);

  // std::vector<EventSubscription> eventSubscriptions;
  // for (auto it: nee.getEventSubs()){
  // event_subscription.amf_event = it.getEvent();
  // getDnaiChgType
  // event_subscriptions.push_back(event_subscription);
  //}
}