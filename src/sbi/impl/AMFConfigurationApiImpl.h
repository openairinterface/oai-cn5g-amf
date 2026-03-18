/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef AMF_CONFIGURATION_API_IMPL_H_
#define AMF_CONFIGURATION_API_IMPL_H_

#include "amf_app.hpp"
#include <AMFConfigurationApi.h>
#include <pistache/http.h>
#include <pistache/optional.h>

namespace oai::amf::api {

using namespace oai::_3gpp::model;

class AMFConfigurationApiImpl : public oai::amf::api::AMFConfigurationApi {
 private:
  amf_application::amf_app* m_amf_app;

 public:
  AMFConfigurationApiImpl(
      std::shared_ptr<Pistache::Rest::Router>, amf_app* amf_app_inst);
  ~AMFConfigurationApiImpl() {}

  void read_configuration(Pistache::Http::ResponseWriter& response);
  void update_configuration(
      nlohmann::json& configuration_info,
      Pistache::Http::ResponseWriter& response);
};

}  // namespace oai::amf::api

#endif
