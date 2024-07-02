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

#include "StatusNotifyApi.h"

#include "Helpers.h"
#include "SmContextStatusNotification.h"
#include "amf_config.hpp"

extern oai::config::amf_config amf_cfg;

namespace oai {
namespace amf {
namespace api {

using namespace oai::model::common::helpers;
using namespace oai::model::amf;

StatusNotifyApi::StatusNotifyApi(std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void StatusNotifyApi::init() {
  setupRoutes();
}

void StatusNotifyApi::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Post(
      *router,
      base + amf_sbi_helper::AmfStatusNotifPathPduSessionReleasePduSessionId,
      Routes::bind(&StatusNotifyApi::notify_pdu_session_status_handler, this));

  // Default handler, called when a route is not found
  router->addCustomHandler(
      Routes::bind(&StatusNotifyApi::notify_status_default_handler, this));
}

void StatusNotifyApi::notify_pdu_session_status_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  // Get SUPI
  auto ueContextId = request.param(":ueContextId").as<std::string>();
  // Get PDU Session ID
  auto pduSessionId = request.param(":pduSessionId").as<std::string>();
  // Getting the body param
  SmContextStatusNotification statusNotification;

  try {
    nlohmann::json::parse(request.body()).get_to(statusNotification);
    this->receive_pdu_session_status_notification(
        ueContextId, pduSessionId, statusNotification, response);
  } catch (nlohmann::detail::exception& e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError& e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception& e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}

void StatusNotifyApi::notify_status_default_handler(
    const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}  // namespace api
}  // namespace amf
}  // namespace oai
