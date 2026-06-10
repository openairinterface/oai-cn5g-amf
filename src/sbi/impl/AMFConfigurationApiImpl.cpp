/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include <AMFConfigurationApiImpl.h>

#include "3gpp_29.500.h"
#include "logger.hpp"
#include "amf_sbi_promise.hpp"
#include "utils.hpp"

namespace oai::amf::api {

using namespace oai::_3gpp::model;

AMFConfigurationApiImpl::AMFConfigurationApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr, amf_app* amf_app_inst)
    : AMFConfigurationApi(rtr), m_amf_app(amf_app_inst) {}

void AMFConfigurationApiImpl::read_configuration(
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug("Receive AMFConfiguration, handling...");

  oai::amf::sbi::sbi_result_t result_data = oai::amf::sbi::sbi_send_recv(
      m_amf_app, [&](uint32_t promise_id) -> std::shared_ptr<itti_msg> {
        // Handle the AMFConfiguration in amf_app
        std::shared_ptr<itti_sbi_amf_configuration> itti_msg =
            std::make_shared<itti_sbi_amf_configuration>(
                TASK_AMF_SBI, TASK_AMF_APP, promise_id);
        itti_msg->promise_id = promise_id;
        return itti_msg;
      });

  if (result_data.has_result) {
    nlohmann::json& result = result_data.body;
    // process data
    uint32_t http_response_code = result_data.http_code;
    nlohmann::json json_data    = {};

    if (http_response_code == oai::common::sbi::http_status_code::OK) {
      if (result.find(kSbiResponseJsonData) != result.end()) {
        json_data = result[kSbiResponseJsonData];
      }
      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/json"));
      response.send(Pistache::Http::Code::Ok, json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/problem+json"));
      response.send(
          Pistache::Http::Code(http_response_code), json_data.dump().c_str());
    }
  } else {
    // TODO:
    response.send(Pistache::Http::Code::Gateway_Timeout);
  }
}

void AMFConfigurationApiImpl::update_configuration(
    nlohmann::json& configuration_info,
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug("Update AMFConfiguration, handling...");

  oai::amf::sbi::sbi_result_t result_data = oai::amf::sbi::sbi_send_recv(
      m_amf_app, [&](uint32_t promise_id) -> std::shared_ptr<itti_msg> {
        // Handle the AMFConfiguration in amf_app
        std::shared_ptr<itti_sbi_update_amf_configuration> itti_msg =
            std::make_shared<itti_sbi_update_amf_configuration>(
                TASK_AMF_SBI, TASK_AMF_APP, promise_id);
        itti_msg->promise_id    = promise_id;
        itti_msg->configuration = configuration_info;
        return itti_msg;
      });

  if (result_data.has_result) {
    nlohmann::json& result = result_data.body;
    // process data
    uint32_t http_response_code = result_data.http_code;
    nlohmann::json json_data    = {};

    if (http_response_code == oai::common::sbi::http_status_code::OK) {
      if (result.find(kSbiResponseJsonData) != result.end()) {
        json_data = result[kSbiResponseJsonData];
      }
      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/json"));
      response.send(Pistache::Http::Code::Ok, json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      response.headers().add<Pistache::Http::Header::ContentType>(
          Pistache::Http::Mime::MediaType("application/problem+json"));
      response.send(
          Pistache::Http::Code(http_response_code), json_data.dump().c_str());
    }
  } else {
    // TODO:
    response.send(Pistache::Http::Code::Gateway_Timeout);
  }
}

}  // namespace oai::amf::api
