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

#ifndef AMFConfigurationApi_H_
#define AMFConfigurationApi_H_

#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

namespace oai::amf::api {

class AMFConfigurationApi {
 public:
  AMFConfigurationApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~AMFConfigurationApi() {}
  void init();

  const std::string base = "/namf-oai/";

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
