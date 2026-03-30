/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef AMFConfigurationApi_H_
#define AMFConfigurationApi_H_

#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

#include "amf_sbi_helper.hpp"

namespace oai::amf::api {

class AMFConfigurationApi {
 public:
  AMFConfigurationApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~AMFConfigurationApi() {}
  void init();

  const std::string base = amf_sbi_helper::AmfConfigurationServiceBase();

 private:
  void setupRoutes();

  void read_configuration_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void update_configuration_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void configuration_api_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  virtual void read_configuration(Pistache::Http::ResponseWriter& response) = 0;
  virtual void update_configuration(
      nlohmann::json& configuration_info,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace oai::amf::api

#endif
