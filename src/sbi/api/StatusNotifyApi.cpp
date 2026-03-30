/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "StatusNotifyApi.h"

#include "Helpers.h"
#include "SmContextStatusNotification.h"
#include "amf_config.hpp"

extern std::unique_ptr<oai::config::amf_config> amf_cfg;

namespace oai {
namespace amf {
namespace api {

using namespace oai::_3gpp::model::helpers;
using namespace oai::_3gpp::model;

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
