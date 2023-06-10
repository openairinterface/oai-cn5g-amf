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

#include "amf-http2-server.hpp"
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <nlohmann/json.hpp>
#include "conversions.hpp"
#include "amf.hpp"
#include "amf_config.hpp"
#include "3gpp_29.500.h"
#include "3gpp_conversions.hpp"

#include "logger.hpp"
#include "mime_parser.hpp"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;
using namespace oai::amf::model;

extern oai::config::amf_config amf_cfg;
extern itti_mw* itti_inst;

//------------------------------------------------------------------------------
void amf_http2_server::start() {
  boost::system::error_code ec;

  Logger::amf_server().info("HTTP2 server started");
  // n1_n2_message_transfer request (URI: /ue-contexts/{}/n1-n2-messages)
  server.handle(
      NAMF_COMMUNICATION_BASE + amf_cfg.sbi_api_version + "/ue-contexts/",
      [&](const request& request, const response& res) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            Logger::amf_server().debug("");
            Logger::amf_server().info("Received N1N2MessageTransfer Request");
            Logger::amf_server().debug("Message content \n %s", msg.c_str());

            // Get the ueContextId and method
            std::vector<std::string> split_result;
            boost::split(
                split_result, request.uri().path, boost::is_any_of("/"));
            if (split_result.size() != 6) {
              Logger::amf_server().warn("Requested URL is not implemented");
              res.write_head(static_cast<uint32_t>(
                  http_response_codes_e::
                      HTTP_RESPONSE_CODE_NOT_IMPLEMENTED));  // TODO
              res.end();
              return;
            }
            std::string ue_context_id = split_result[split_result.size() - 2];
            Logger::amf_server().info(
                "ue_context_id %s", ue_context_id.c_str());

            // simple parser
            mime_parser sp = {};
            if (!sp.parse(msg)) {
              // send reply!!!
              res.write_head(static_cast<uint32_t>(
                  http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
              res.end();
              return;
            }

            std::unordered_map<std::string, mime_part> parts = {};
            sp.get_mime_parts(parts);
            uint8_t size = parts.size();
            Logger::amf_server().debug("Number of MIME parts %d", size);

            // at least 2 parts for Json data and N1 (+ N2)
            if (size < 2) {
              res.write_head(static_cast<uint32_t>(
                  http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
              res.end();
              Logger::amf_server().debug(
                  "Bad request: should have at least 2 MIME parts");
              return;
            }

            for (auto it : parts) {
              Logger::amf_server().debug(
                  "MIME part: %s (%d)", it.first.c_str(),
                  it.second.body.size());
            }

            bool is_ngap = false;
            if (size > 2) {
              is_ngap = true;
            }

            N1N2MessageTransferReqData n1N2MessageTransferReqData = {};

            try {
              nlohmann::json::parse(parts[JSON_CONTENT_ID_MIME].body.c_str())
                  .get_to(n1N2MessageTransferReqData);
              if (!is_ngap)
                this->n1_n2_message_transfer_handler(
                    ue_context_id, n1N2MessageTransferReqData,
                    parts[N1_SM_CONTENT_ID].body, res);
              else
                this->n1_n2_message_transfer_handler(
                    ue_context_id, n1N2MessageTransferReqData,
                    parts[N1_SM_CONTENT_ID].body, res,
                    parts[N2_SM_CONTENT_ID].body);
            } catch (nlohmann::detail::exception& e) {
              Logger::amf_server().warn(
                  "Cannot parse the JSON data (error: %s)!", e.what());
              res.write_head(static_cast<uint32_t>(
                  http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
              res.end();
              return;
            } catch (std::exception& e) {
              Logger::amf_server().warn("Error: %s!", e.what());
              res.write_head(static_cast<uint32_t>(
                  http_response_codes_e::
                      HTTP_RESPONSE_CODE_INTERNAL_SERVER_ERROR));
              res.end();
              return;
            }
          }
        });
      });

  server.handle(
      NAMF_EVENT_EXPOSURE_BASE + amf_cfg.sbi_api_version +
          NAMF_EVENT_EXPOSURE_SUBSCRIPTION,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          std::string msg((char*) data, len);
          try {
            std::vector<std::string> split_result;
            boost::split(
                split_result, request.uri().path, boost::is_any_of("/"));
            if (request.method().compare("POST") == 0 && len > 0) {
              if (split_result.size() != 4) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    http_response_codes_e::
                        HTTP_RESPONSE_CODE_NOT_IMPLEMENTED));  // TODO
                response.end();
                return;
              }
              AmfCreateEventSubscription amfCreateEventSubscription;
              nlohmann::json::parse(msg.c_str())
                  .get_to(amfCreateEventSubscription);
              this->createEventSubscriptionHandler(
                  amfCreateEventSubscription, response);
            } else if (request.method().compare("DELETE") == 0) {
              if (split_result.size() != 5) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    http_response_codes_e::
                        HTTP_RESPONSE_CODE_NOT_IMPLEMENTED));  // TODO
                response.end();
                return;
              }
              std::string subscriptionId =
                  split_result[split_result.size() - 1];
              Logger::amf_server().debug(
                  "Delete a subscription with ID %s", subscriptionId.c_str());
              if (m_amf_app->handle_event_exposure_delete(subscriptionId)) {
                response.write_head(static_cast<uint32_t>(
                    http_response_codes_e::HTTP_RESPONSE_CODE_204_NO_CONTENT));
                response.end();
              } else {
                // Send response
                nlohmann::json json_data                        = {};
                oai::amf::model::ProblemDetails problem_details = {};
                problem_details.setCause("SUBSCRIPTION_NOT_FOUND");
                to_json(json_data, problem_details);
                response.write_head(static_cast<uint32_t>(
                    http_response_codes_e::HTTP_RESPONSE_CODE_NOT_FOUND));
                response.end(json_data.dump().c_str());
              }
            } else if (request.method().compare("PATCH") == 0) {
              if (split_result.size() != 5) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    http_response_codes_e::
                        HTTP_RESPONSE_CODE_NOT_IMPLEMENTED));  // TODO
                response.end();
                return;
              }
              Logger::amf_server().warn(
                  "Modify EvenExposureSubscription Not Implemented");
              response.write_head(static_cast<uint32_t>(
                  http_response_codes_e::
                      HTTP_RESPONSE_CODE_NOT_IMPLEMENTED));  // TODO
              response.end();
              return;
            } else {
              Logger::amf_server().warn(
                  "Invalid request (error: Invalid Request Method)!");
              response.write_head(static_cast<uint32_t>(
                  http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
              response.end();
              return;
            }
          } catch (std::exception& e) {
            Logger::amf_server().warn("Invalid request (error: %s)!", e.what());
            response.write_head(static_cast<uint32_t>(
                http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
            response.end();
            return;
          }
        });
      });

  if (server.listen_and_serve(ec, m_address, std::to_string(m_port))) {
    std::cerr << "HTTP Server error: " << ec.message() << std::endl;
  }
}

//------------------------------------------------------------------------------

void amf_http2_server::createEventSubscriptionHandler(
    const AmfCreateEventSubscription& amfCreateEventSubscription,
    const response& response) {
  Logger::amf_server().info("Received AmfCreateEventSubscription Request");

  header_map h;

  amf_application::event_exposure_msg event_exposure = {};
  xgpp_conv::amf_event_subscription_from_openapi(
      amfCreateEventSubscription, event_exposure);

  std::shared_ptr<itti_sbi_event_exposure_request> itti_msg =
      std::make_shared<itti_sbi_event_exposure_request>(
          AMF_SERVER, TASK_AMF_APP);
  itti_msg->event_exposure = event_exposure;
  itti_msg->http_version   = 2;

  evsub_id_t sub_id = m_amf_app->handle_event_exposure_subscription(itti_msg);

  nlohmann::json json_data = {};
  to_json(
      json_data["subscription"], amfCreateEventSubscription.getSubscription());

  if (sub_id != -1) {
    std::string location =
        std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.sbi.addr4))) + ":" +
        std::to_string(amf_cfg.sbi.port) + NAMF_EVENT_EXPOSURE_BASE +
        amf_cfg.sbi_api_version + "/namf-evts/" + std::to_string(sub_id);

    json_data["subscriptionId"] = location;
    h.insert(std::make_pair<std::string, header_value>(
        "Location", {location, false}));
  }

  h.insert(std::make_pair<std::string, header_value>(
      "Content-Type", {"application/json", false}));
  response.write_head(
      static_cast<uint32_t>(
          http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED),
      h);
  response.end(json_data.dump().c_str());
}

void amf_http2_server::n1_n2_message_transfer_handler(
    const std::string& ueContextId,
    const N1N2MessageTransferReqData& n1N2MessageTransferReqData,
    const std::string& n1sm_str, const response& res,
    const std::string& n2sm_str) {
  Logger::amf_server().debug(
      "Receive N1N2MessageTransfer Request, handling...");

  nlohmann::json response_json = {};
  response_json["cause"] =
      n1_n2_message_transfer_cause_e2str[N1_N2_TRANSFER_INITIATED];
  uint32_t code =
      static_cast<uint32_t>(http_response_codes_e::HTTP_RESPONSE_CODE_200_OK);

  std::string supi = ueContextId;
  Logger::amf_server().debug(
      "Key for PDU Session context: SUPI (%s)", supi.c_str());
  std::shared_ptr<pdu_session_context> psc = {};

  if (!m_amf_app->find_pdu_session_context(
          supi, (uint8_t) n1N2MessageTransferReqData.getPduSessionId(), psc)) {
    Logger::amf_server().error(
        "Cannot get PDU Session Context with SUPI (%s)", supi.c_str());
    // Send response to the NF Service Consumer (e.g., SMF)
    res.write_head(static_cast<uint32_t>(
        http_response_codes_e::HTTP_RESPONSE_CODE_BAD_REQUEST));
    res.end();
    return;
  }

  bstring n1sm = nullptr;
  conv::msg_str_2_msg_hex(
      n1sm_str.substr(0, n1sm_str.length()), n1sm);  // TODO: verify n1sm_length

  bstring n2sm = nullptr;
  if (!n2sm_str.empty()) {
    conv::msg_str_2_msg_hex(n2sm_str, n2sm);
    psc->n2sm              = bstrcpy(n2sm);
    psc->is_n2sm_avaliable = true;
  } else {
    psc->is_n2sm_avaliable = false;
  }

  psc->n1sm              = bstrcpy(n1sm);
  psc->is_n1sm_avaliable = true;

  auto itti_msg = std::make_shared<itti_n1n2_message_transfer_request>(
      AMF_SERVER, TASK_AMF_APP);
  itti_msg->supi        = ueContextId;
  itti_msg->n1sm        = bstrcpy(n1sm);
  itti_msg->is_n1sm_set = true;
  if (!n2sm_str.empty()) {
    itti_msg->n2sm        = bstrcpy(n2sm);
    itti_msg->is_n2sm_set = true;
  } else {
    itti_msg->is_n2sm_set = false;
  }

  itti_msg->pdu_session_id =
      (uint8_t) n1N2MessageTransferReqData.getPduSessionId();
  nlohmann::json ngap_ie_type = {};
  to_json(
      ngap_ie_type, n1N2MessageTransferReqData.getN2InfoContainer()
                        .getSmInfo()
                        .getN2InfoContent()
                        .getNgapIeType()
                        .getValue());
  itti_msg->n2sm_info_type = ngap_ie_type.dump();

  // For Paging
  if (n1N2MessageTransferReqData.ppiIsSet()) {
    itti_msg->is_ppi_set = true;
    itti_msg->ppi        = n1N2MessageTransferReqData.getPpi();
    response_json["cause"] =
        n1_n2_message_transfer_cause_e2str[ATTEMPTING_TO_REACH_UE];
    code = static_cast<uint32_t>(
        http_response_codes_e::HTTP_RESPONSE_CODE_202_ACCEPTED);
  } else {
    itti_msg->is_ppi_set = false;
  }

  // Send response to the NF Service Consumer (e.g., SMF)
  res.write_head(code);
  res.end(response_json.dump().c_str());

  // Process N1N2 Message Transfer Request
  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }

  bdestroy_wrapper(&n1sm);
  bdestroy_wrapper(&n2sm);
}

//------------------------------------------------------------------------------
void amf_http2_server::stop() {
  server.stop();
}
