/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "amf_sbi_promise.hpp"

#include <optional>

#include <boost/thread/future.hpp>

#include "3gpp_29.500.h"
#include "amf_app.hpp"
#include "http_definitions.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "utils.hpp"

extern itti_mw* itti_inst;

namespace oai::amf::sbi {

sbi_result_t sbi_send_recv(
    amf_application::amf_app* app,
    const std::function<std::shared_ptr<itti_msg>(uint32_t promise_id)>&
        make_msg) {
  sbi_result_t result_data = {};

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = {};
  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  app->store_promise(promise_id, p);
  Logger::amf_server().debug("Promise ID generated %d", promise_id);

  // Build the concrete ITTI message for this promise and send it
  std::shared_ptr<itti_msg> itti_msg = make_msg(promise_id);

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }

  // Wait for the result available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);

  if (result_opt.has_value()) {
    Logger::amf_server().debug("Got result for promise ID %d", promise_id);
    nlohmann::json result = result_opt.value();

    result_data.has_result = true;
    if (result.find(kSbiResponseHeaderLocation) != result.end()) {
      result_data.location =
          result[kSbiResponseHeaderLocation].get<std::string>();
    }
    if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
      result_data.http_code = result[kSbiResponseHttpResponseCode].get<int>();
    }
    result_data.body = result;
  } else {
    result_data.http_code = oai::common::sbi::http_status_code::GATEWAY_TIMEOUT;
  }

  // Remove the promise from the list since the result is processed or not
  // available
  app->remove_promise(promise_id);

  return result_data;
}

}  // namespace oai::amf::sbi
