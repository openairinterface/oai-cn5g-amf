/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "AMFConfigurationApi.h"

#include "Helpers.h"
#include "amf_config.hpp"

extern std::unique_ptr<oai::config::amf_config> amf_cfg;

namespace oai::amf::api {

using namespace oai::_3gpp::model::helpers;

AMFConfigurationApi::AMFConfigurationApi(
    std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void AMFConfigurationApi::init() {
  setupRoutes();
}

void AMFConfigurationApi::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Get(
      *router, base + amf_sbi_helper::AmfConfPathConfiguration,
      Routes::bind(&AMFConfigurationApi::read_configuration_handler, this));

  Routes::Put(
      *router, base + amf_sbi_helper::AmfConfPathConfiguration,
      Routes::bind(&AMFConfigurationApi::update_configuration_handler, this));

  // Default handler, called when a route is not found
  router->addCustomHandler(Routes::bind(
      &AMFConfigurationApi::configuration_api_default_handler, this));
}

void AMFConfigurationApi::read_configuration_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  try {
    this->read_configuration(response);
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

void AMFConfigurationApi::update_configuration_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  try {
    auto configuration_info = nlohmann::json::parse(request.body());
    this->update_configuration(configuration_info, response);
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

void AMFConfigurationApi::configuration_api_default_handler(
    const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}  // namespace oai::amf::api
