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

#include "amf_http2_server.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <nlohmann/json.hpp>

#include "3gpp_29.500.h"
#include "3gpp_conversions.hpp"
#include "amf.hpp"
#include "amf_config.hpp"
#include "amf_conversions.hpp"
#include "amf_sbi_helper.hpp"
#include "logger.hpp"
#include "output_wrapper.hpp"
#include "utils.hpp"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;
using namespace oai::model::amf;
using namespace oai::model::common;
using namespace oai::common::sbi;
using namespace oai::amf::api;

extern oai::config::amf_config amf_cfg;
extern itti_mw* itti_inst;
extern amf_app* amf_app_inst;

//------------------------------------------------------------------------------
void amf_http2_server::start() {
  boost::system::error_code ec;

  Logger::amf_server().info("HTTP2 server being started");
  // N1N2MessageTransfer (URI:/ue-contexts/{ueContextId}/n1-n2-messages)
  // N1 Message Notify (URI:/ue-contexts/{ueContextId}/n1-message-notify)
  // N1N2MessageSubscribe (URI:
  // /ue-contexts/{ueContextId}/n1-n2-messages/subscription)
  // N1N2MessageUnSubscribe (URI:
  // /ue-contexts/{ueContextId}/n1-n2-messages/subscriptions/{subscriptionId})
  server.handle(
      amf_sbi_helper::AmfCommunicationServiceBase +
          amf_sbi_helper::AmfCommPathUeContext,
      [&](const request& request, const response& res) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            Logger::amf_server().debug("");
            Logger::amf_server().debug("Message content \n %s", msg.c_str());

            // Get the ueContextId and method
            std::vector<std::string> split_result;
            boost::split(
                split_result, request.uri().path, boost::is_any_of("/"));
            if (split_result.size() < 6) {
              Logger::amf_server().warn("Requested URL is not implemented");
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
              res.end();
              return;
            }

            std::string ue_context_id = split_result[4];
            Logger::amf_server().info(
                "ue_context_id %s", ue_context_id.c_str());

            if (split_result.size() ==
                6) {  // N1N2MessageTransfer or N1 Message Notify

              // simple parser
              mime_parser sp = {};
              if (!sp.parse(msg)) {
                res.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::BAD_REQUEST));
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
                    oai::common::sbi::http_status_code::BAD_REQUEST));
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

              std::string procedure = split_result[split_result.size() - 1];
              Logger::amf_server().info("Procedure %s", procedure.c_str());
              if (procedure.compare(amf_sbi_helper::AmfCommPathN1N2Messages) ==
                  0) {
                this->n1_n2_message_transfer_handler(ue_context_id, parts, res);
              }
              if (procedure.compare(
                      amf_sbi_helper::AmfCommPathN1MessageNotify) == 0) {
                this->n1_message_notify_handler(ue_context_id, parts, res);
              }
            } else if (split_result.size() == 7) {
              std::string procedure = split_result[split_result.size() - 1];
              Logger::amf_server().info("Procedure %s", procedure.c_str());
              if (procedure.compare("subscriptions") == 0) {
                // TODO:

                UeN1N2InfoSubscriptionCreateData
                    ueN1N2InfoSubscriptionCreateData = {};
                nlohmann::json::parse(msg.c_str())
                    .get_to(ueN1N2InfoSubscriptionCreateData);

                this->n1_n2_message_subscribe_handler(
                    ue_context_id, ueN1N2InfoSubscriptionCreateData, res);
              }
            } else if (split_result.size() == 8) {
              std::string procedure = split_result[split_result.size() - 2];
              Logger::amf_server().info("Procedure %s", procedure.c_str());
              if (procedure.compare("subscriptions") == 0) {
                std::string subscription_id = split_result[7];
                Logger::amf_server().info(
                    "subscription_id %s", subscription_id.c_str());
                this->n1_n2_message_unsubscribe_handler(
                    ue_context_id, subscription_id, res);
              }
            }
          }
        });
      });

  // Event Exposure
  server.handle(
      amf_sbi_helper::AmfEventExposureServiceBase +
          amf_sbi_helper::AmfEvtsPathSubscriptions,
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
                    oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
                response.end();
                return;
              }
              AmfCreateEventSubscription amfCreateEventSubscription;
              nlohmann::json::parse(msg.c_str())
                  .get_to(amfCreateEventSubscription);
              this->create_event_subscription_handler(
                  amfCreateEventSubscription, response);
            } else if (request.method().compare("DELETE") == 0) {
              if (split_result.size() != 5) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
                response.end();
                return;
              }
              std::string subscriptionId =
                  split_result[split_result.size() - 1];
              Logger::amf_server().debug(
                  "Delete a subscription with ID %s", subscriptionId.c_str());
              if (m_amf_app->handle_event_exposure_delete(subscriptionId)) {
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NO_CONTENT));
                response.end();
              } else {
                // Send response
                nlohmann::json json_data       = {};
                ProblemDetails problem_details = {};
                problem_details.setCause("SUBSCRIPTION_NOT_FOUND");
                to_json(json_data, problem_details);
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NOT_FOUND));
                response.end(json_data.dump().c_str());
              }
            } else if (request.method().compare("PATCH") == 0) {
              if (split_result.size() != 5) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
                response.end();
                return;
              }
              Logger::amf_server().warn(
                  "Modify EvenExposureSubscription Not Implemented");
              response.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
              response.end();
              return;
            } else {
              Logger::amf_server().warn(
                  "Invalid request (error: Invalid Request Method)!");
              response.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              response.end();
              return;
            }
          } catch (std::exception& e) {
            Logger::amf_server().warn("Invalid request (error: %s)!", e.what());
            response.write_head(static_cast<uint32_t>(
                oai::common::sbi::http_status_code::BAD_REQUEST));
            response.end();
            return;
          }
        });
      });

  // AMF configuration-related APIs
  server.handle(
      amf_sbi_helper::AmfConfigurationServiceBase +
          amf_sbi_helper::AmfConfPathConfiguration,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          try {
            if (request.method().compare("GET") == 0) {
              this->get_configuration_handler(response);
            }
            if (request.method().compare("PUT") == 0 && len > 0) {
              std::string msg((char*) data, len);
              auto configuration_info = nlohmann::json::parse(msg.c_str());
              this->update_configuration_handler(configuration_info, response);
            }
          } catch (nlohmann::detail::exception& e) {
            Logger::amf_sbi().warn(
                "Can not parse the JSON data (error: %s)!", e.what());
            response.write_head(static_cast<uint32_t>(
                oai::common::sbi::http_status_code::BAD_REQUEST));
            response.end();
            return;
          }
        });
      });

  // NonUEN2MessageTransfer: /non-ue-n2-messages/transfer
  server.handle(
      amf_sbi_helper::AmfCommunicationServiceBase +
          amf_sbi_helper::AmfCommPathNonUeN1N2MessageTransfer,
      [&](const request& request, const response& res) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            Logger::amf_server().debug("");
            Logger::amf_server().info(
                "Received NonUEN2MessageTransfer Request");
            Logger::amf_server().debug("Message content \n %s", msg.c_str());

            // simple parser
            mime_parser sp = {};
            if (!sp.parse(msg)) {
              // send reply!!!
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              res.end();
              return;
            }

            std::unordered_map<std::string, mime_part> parts = {};
            sp.get_mime_parts(parts);
            uint8_t size = parts.size();
            Logger::amf_server().debug("Number of MIME parts %d", size);

            // at least 2 parts for Json data and N2
            if (size < 2) {
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
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

            nlohmann::json response_json = {};
            response_json["cause"] = non_ue_n2_message_transfer_cause_e2str
                [NON_UE_N2_TRANSFER_INITIATED];
            auto code =
                static_cast<uint32_t>(oai::common::sbi::http_status_code::OK);

            oai::model::amf::N2InformationTransferReqData
                n2InformationTransferReqData = {};

            try {
              nlohmann::json::parse(parts[JSON_CONTENT_ID_MIME].body.c_str())
                  .get_to(n2InformationTransferReqData);

            } catch (nlohmann::detail::exception& e) {
              Logger::amf_server().warn(
                  "Cannot parse the JSON data (error: %s)!", e.what());
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              res.end();
              return;
            } catch (std::exception& e) {
              Logger::amf_server().warn("Error: %s!", e.what());
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::INTERNAL_SERVER_ERROR));
              res.end();
              return;
            }

            std::string n2_content_id = {};

            if (n2InformationTransferReqData.getN2Information()
                    .getN2InformationClass()
                    .getEnumValue() !=
                N2InformationClass_anyOf::eN2InformationClass_anyOf::NRPPA) {
              // TODO: Only support NRPPA for now
              response_json["cause"] =
                  n1_n2_message_transfer_cause_e2str[N1_MSG_NOT_TRANSFERRED];
              // Send response to the NF Service Consumer (e.g., SMF)
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              res.end(response_json.dump().c_str());
              return;
            } else {
              n2_content_id = n2InformationTransferReqData.getN2Information()
                                  .getNrppaInfo()
                                  .getNrppaPdu()
                                  .getNgapData()
                                  .getContentId();
              Logger::amf_server().debug(
                  "n2_content_id: %s", n2_content_id.c_str());
            }

            bstring nrppa_pdu  = nullptr;
            bstring routing_id = nullptr;
            amf_conv::msg_str_2_msg_hex(parts[n2_content_id].body, nrppa_pdu);
            amf_conv::string_2_bstring(
                n2InformationTransferReqData.getN2Information()
                    .getNrppaInfo()
                    .getNfId(),
                routing_id);
            auto itti_msg =
                std::make_shared<itti_non_ue_n2_message_transfer_request>(
                    AMF_SERVER, TASK_AMF_APP);

            itti_msg->nrppa_pdu        = bstrcpy(nrppa_pdu);
            itti_msg->routing_id       = bstrcpy(routing_id);
            itti_msg->is_nrppa_pdu_set = true;

            res.write_head(code);
            res.end(response_json.dump().c_str());

            int ret = itti_inst->send_msg(itti_msg);
            if (0 != ret) {
              Logger::amf_server().error(
                  "Could not send ITTI message %s to task TASK_AMF_N2",
                  itti_msg->get_msg_name());
            }
            oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
          }
        });
      });

  // NonUeN2InfoSubscribe: /non-ue-n2-messages/subscriptions
  // NonUeN2InfoUnSubscribe:
  // /non-ue-n2-messages/subscriptions/{n2NotifySubscriptionId}:
  server.handle(
      amf_sbi_helper::AmfCommunicationServiceBase +
          amf_sbi_helper::AmfCommPathNonUeN1N2MessageSubscriptions,
      [&](const request& request, const response& res) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            try {
              std::vector<std::string> split_result;
              boost::split(
                  split_result, request.uri().path, boost::is_any_of("/"));

              if (split_result.size() < 5) {
                Logger::amf_server().warn("Requested URL is not implemented");
                res.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
                res.end();
                return;
              }

              // NonUeN2InfoSubscribe
              if (request.method().compare("POST") == 0 && len > 0 &&
                  (split_result.size() == 5)) {
                NonUeN2InfoSubscriptionCreateData createData = {};
                nlohmann::json::parse(msg.c_str()).get_to(createData);
                this->non_ue_n2_info_subscribe_handler(createData, res);
              } else if (
                  request.method().compare("DELETE") == 0 &&
                  (split_result.size() == 6)) {  // NonUeN2InfoUnSubscribe
                std::string subscription_id =
                    split_result[split_result.size() - 1];
                Logger::amf_server().info(
                    "n2NotifySubscriptionId %s", subscription_id.c_str());
                this->non_ue_n2_info_unsubscribe_handler(subscription_id, res);
              } else {
                Logger::amf_server().warn(
                    "Invalid request (error: Invalid Request Method)!");
                res.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::BAD_REQUEST));
                res.end();
                return;
              }
            } catch (std::exception& e) {
              Logger::amf_server().warn(
                  "Invalid request (error: %s)!", e.what());
              res.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              res.end();
              return;
            }
          }
        });
      });

  // NF Status Notify (URL:
  // /namf-status-notify/pdu-session-release/callback/:ueContextId/:pduSessionId)
  server.handle(
      amf_sbi_helper::AmfStatusNotifyServiceBase +
          amf_sbi_helper::AmfStatusNotifPathPduSessionRelease,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          std::string msg((char*) data, len);
          try {
            std::vector<std::string> split_result;
            boost::split(
                split_result, request.uri().path, boost::is_any_of("/"));
            if (request.method().compare("POST") == 0 && len > 0) {
              if (split_result.size() != 7) {
                Logger::amf_server().warn("Requested URL is not implemented");
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::NOT_IMPLEMENTED));
                response.end();
                return;
              }

              std::string ue_context_id = split_result[split_result.size() - 2];
              Logger::amf_server().info(
                  "ue_context_id %s", ue_context_id.c_str());

              std::string pdu_session_id_str =
                  split_result[split_result.size() - 1];
              Logger::amf_server().info(
                  "pdu_session_id %s", pdu_session_id_str.c_str());

              uint8_t pdu_session_id = 0;
              if (oai::utils::conv::string_to_int8(
                      pdu_session_id_str, pdu_session_id)) {
                Logger::amf_server().debug("Invalid PDU Session ID value");
                response.write_head(static_cast<uint32_t>(
                    oai::common::sbi::http_status_code::BAD_REQUEST));
                response.end();
                return;
              }

              SmContextStatusNotification statusNotification = {};
              nlohmann::json::parse(msg.c_str()).get_to(statusNotification);
              this->status_notify_handler(
                  ue_context_id, pdu_session_id, statusNotification, response);
            } else {
              Logger::amf_server().warn(
                  "Invalid request (error: Invalid Request Method)!");
              response.write_head(static_cast<uint32_t>(
                  oai::common::sbi::http_status_code::BAD_REQUEST));
              response.end();
              return;
            }
          } catch (std::exception& e) {
            Logger::amf_server().warn("Invalid request (error: %s)!", e.what());
            response.write_head(static_cast<uint32_t>(
                oai::common::sbi::http_status_code::BAD_REQUEST));
            response.end();
            return;
          }
        });
      });

  running_server = true;
  if (server.listen_and_serve(ec, m_address, std::to_string(m_port))) {
    Logger::amf_server().debug("HTTP Server status: %s", ec.message());
  }
  running_server = false;
  Logger::amf_server().info("HTTP2 server fully stopped");
}

//------------------------------------------------------------------------------
void amf_http2_server::create_event_subscription_handler(
    const AmfCreateEventSubscription& amfCreateEventSubscription,
    const response& response) {
  Logger::amf_server().info("Received AmfCreateEventSubscription Request");

  header_map h;
  event_exposure_msg event_exposure = {};
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

  // TODO: To be fixed with correct location
  if (sub_id != -1) {
    std::string location =
        std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.sbi.addr4))) + ":" +
        std::to_string(amf_cfg.sbi.port) + NAMF_EVENT_EXPOSURE_BASE +
        amf_cfg.sbi.api_version.value_or(DEFAULT_SBI_API_VERSION) +
        "/namf-evts/" + std::to_string(sub_id);

    json_data["subscriptionId"] = location;
    h.insert(std::make_pair<std::string, header_value>(
        "Location", {location, false}));
  }

  h.insert(std::make_pair<std::string, header_value>(
      "Content-Type", {"application/json", false}));
  response.write_head(
      static_cast<uint32_t>(oai::common::sbi::http_status_code::CREATED), h);
  response.end(json_data.dump().c_str());
}

//------------------------------------------------------------------------------
void amf_http2_server::n1_n2_message_transfer_handler(
    const std::string& ueContextId,
    std::unordered_map<std::string, mime_part>& parts,
    const response& response) {
  Logger::amf_server().debug(
      "Receive N1N2MessageTransfer Request, handling...");

  nlohmann::json response_json = {};
  response_json["cause"] =
      n1_n2_message_transfer_cause_e2str[N1_N2_TRANSFER_INITIATED];
  uint32_t code = static_cast<uint32_t>(oai::common::sbi::http_status_code::OK);

  std::string supi = ueContextId;

  N1N2MessageTransferReqData n1N2MessageTransferReqData = {};
  nlohmann::json::parse(parts[JSON_CONTENT_ID_MIME].body.c_str())
      .get_to(n1N2MessageTransferReqData);

  bool request_valid = true;
  bstring n1sm       = nullptr;
  bstring n2sm       = nullptr;
  bstring nrppa_pdu  = nullptr;
  bstring routing_id = nullptr;

  auto itti_msg = std::make_shared<itti_n1n2_message_transfer_request>(
      AMF_SERVER, TASK_AMF_APP);  // TODO: May not be used
  itti_msg->supi = ueContextId;
  Logger::amf_server().debug("SUPI %s", ueContextId.c_str());

  if (n1N2MessageTransferReqData.n2InfoContainerIsSet()) {
    // N2 Container Present
    Logger::amf_server().debug("N2InfoContainer is present, handling...");

    std::string n2_content_id = {};
    std::string ngap_type     = {};

    // Check N2 Information Class
    switch (n1N2MessageTransferReqData.getN2InfoContainer()
                .getN2InformationClass()
                .getEnumValue()) {
      case N2InformationClass_anyOf::eN2InformationClass_anyOf::SM: {
        Logger::amf_server().debug("N2 Information Class: SM");

        // Validate Content ID
        n2_content_id = n1N2MessageTransferReqData.getN2InfoContainer()
                            .getSmInfo()
                            .getN2InfoContent()
                            .getNgapData()
                            .getContentId();
        Logger::amf_server().debug("n2_content_id: %s", n2_content_id.c_str());
        // Check whether N2 Content Id is valid with MIME part
        if (parts.count(n2_content_id) == 0 ||
            parts[n2_content_id].body.size() == 0) {
          Logger::amf_server().error("Missing n2sm MIME part");

          response.write_head(code);
          response.end(response_json.dump().c_str());

          return;
        }

        // NGAP IE Type
        nlohmann::json ngap_ie_type_json = {};
        to_json(
            ngap_ie_type_json, n1N2MessageTransferReqData.getN2InfoContainer()
                                   .getSmInfo()
                                   .getN2InfoContent()
                                   .getNgapIeType()
                                   .getValue());
        ngap_type = ngap_ie_type_json.get<std::string>();
        Logger::amf_server().debug("NGAP IE Type: %s", ngap_type.c_str());
        // Set NGAP type
        itti_msg->n2sm_info_type = ngap_type;

        Logger::amf_server().debug(
            "Key for PDU Session Context: SUPI (%s)", supi.c_str());
        std::shared_ptr<pdu_session_context> psc = {};

        if (!amf_app_inst->find_pdu_session_context(
                supi, (uint8_t) n1N2MessageTransferReqData.getPduSessionId(),
                psc)) {
          response.write_head(code);
          response.end(response_json.dump().c_str());
          return;
        }

        amf_conv::msg_str_2_msg_hex(parts[n2_content_id].body, n2sm);
        // Store N2 SM in PDU Session Context
        psc->n2sm              = bstrcpy(n2sm);
        psc->is_n2sm_available = true;

        itti_msg->n2sm           = bstrcpy(n2sm);
        itti_msg->is_n2sm_set    = true;
        itti_msg->n2sm_info_type = ngap_type;

        itti_msg->pdu_session_id =
            (uint8_t) n1N2MessageTransferReqData.getPduSessionId();

      } break;

      case N2InformationClass_anyOf::eN2InformationClass_anyOf::NRPPA: {
        Logger::amf_server().debug("N2 Information Class: NRPPA");
        n2_content_id = n1N2MessageTransferReqData.getN2InfoContainer()
                            .getNrppaInfo()
                            .getNrppaPdu()
                            .getNgapData()
                            .getContentId();
        Logger::amf_server().debug("N2 Content Id: %s", n2_content_id.c_str());

        // Check whether N2 Content Id is valid with MIME part
        if (parts.count(n2_content_id) == 0 ||
            parts[n2_content_id].body.size() == 0) {
          Logger::amf_server().error("Missing n2sm MIME part");

          response.write_head(code);
          response.end(response_json.dump().c_str());
          return;
        }

        // NGAP IE Type
        nlohmann::json ngap_ie_type_json = {};
        to_json(
            ngap_ie_type_json, n1N2MessageTransferReqData.getN2InfoContainer()
                                   .getNrppaInfo()
                                   .getNrppaPdu()
                                   .getNgapIeType()
                                   .getValue());
        ngap_type = ngap_ie_type_json.get<std::string>();
        Logger::amf_server().debug("NGAP IE Type: %s", ngap_type.c_str());
        // Set NGAP type
        itti_msg->n2sm_info_type = ngap_type;

        // NRPPA PDU
        amf_conv::msg_str_2_msg_hex(parts[n2_content_id].body, nrppa_pdu);
        amf_conv::string_2_bstring(
            n1N2MessageTransferReqData.getN2InfoContainer()
                .getNrppaInfo()
                .getNfId(),
            routing_id);
        itti_msg->nrppa_pdu        = bstrcpy(nrppa_pdu);
        itti_msg->routing_id       = bstrcpy(routing_id);
        itti_msg->is_nrppa_pdu_set = true;

      } break;

      default: {
        response.write_head(static_cast<uint32_t>(
            oai::common::sbi::http_status_code::BAD_REQUEST));
        response.end(
            "N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer API "
            "(Unsupported N2 Message Class)");
        return;
      }
    }
  }

  if (n1N2MessageTransferReqData.n1MessageContainerIsSet()) {
    Logger::amf_server().debug("N1MessageContainer is present, handling...");

    switch (n1N2MessageTransferReqData.getN1MessageContainer()
                .getN1MessageClass()
                .getEnumValue()) {
      case N1MessageClass_anyOf::eN1MessageClass_anyOf::SM: {
        // N1 SM Container Present
        Logger::amf_server().debug(
            "Key for PDU Session Context: SUPI (%s)", supi.c_str());
        std::shared_ptr<pdu_session_context> psc = {};
        if (!amf_app_inst->find_pdu_session_context(
                supi, (uint8_t) n1N2MessageTransferReqData.getPduSessionId(),
                psc)) {
          response.write_head(code);
          response.end(response_json.dump().c_str());
          return;
        }

        std::string n1_content_id =
            n1N2MessageTransferReqData.getN1MessageContainer()
                .getN1MessageContent()
                .getContentId();
        Logger::amf_server().debug("N1 Content Id: %s", n1_content_id.c_str());

        if (parts.count(n1_content_id) == 0 ||
            parts[n1_content_id].body.size() == 0) {
          response.write_head(code);
          response.end(response_json.dump().c_str());
          return;
        }

        amf_conv::msg_str_2_msg_hex(
            parts[n1_content_id].body.substr(
                0, parts[n1_content_id].body.length()),
            n1sm);
        oai::utils::output_wrapper::print_buffer(
            "amf_server", "Received N1 SM", (uint8_t*) bdata(n1sm),
            blength(n1sm));
        // Store N1 SM in PDU Session Context
        psc->n1sm              = bstrcpy(n1sm);
        psc->is_n1sm_available = true;

        itti_msg->n1sm        = bstrcpy(n1sm);
        itti_msg->is_n1sm_set = true;
        itti_msg->pdu_session_id =
            (uint8_t) n1N2MessageTransferReqData.getPduSessionId();

      } break;

      case N1MessageClass_anyOf::eN1MessageClass_anyOf::LPP: {
        // N1 LPP Container Present
        // TODO:
        response.write_head(static_cast<uint32_t>(
            oai::common::sbi::http_status_code::BAD_REQUEST));
        response.end(
            "N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer API "
            "(Unsupported N1 Message Class: LPP)");
        return;
      } break;

      default: {
        // TODO:
        response.write_head(static_cast<uint32_t>(
            oai::common::sbi::http_status_code::BAD_REQUEST));
        response.end(
            "N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer API "
            "(Unsupported N1 Message Class)");
        return;
      }
    }
  }

  if (!request_valid) {
    response_json["cause"] =
        n1_n2_message_transfer_cause_e2str[N1_MSG_NOT_TRANSFERRED];
    // Send response to the NF Service Consumer (e.g., SMF)
    response.write_head(
        static_cast<uint32_t>(oai::common::sbi::http_status_code::BAD_REQUEST));
    response.end(response_json.dump().c_str());
    return;
  }

  response_json["cause"] =
      n1_n2_message_transfer_cause_e2str[N1_N2_TRANSFER_INITIATED];
  code = static_cast<uint32_t>(oai::common::sbi::http_status_code::OK);

  // For Paging
  if (n1N2MessageTransferReqData.ppiIsSet()) {
    itti_msg->is_ppi_set = true;
    itti_msg->ppi        = n1N2MessageTransferReqData.getPpi();
    response_json["cause"] =
        n1_n2_message_transfer_cause_e2str[ATTEMPTING_TO_REACH_UE];
    code = static_cast<uint32_t>(oai::common::sbi::http_status_code::ACCEPTED);
  } else {
    itti_msg->is_ppi_set = false;
  }

  // Send response to the NF Service Consumer (e.g., SMF)
  response.write_head(code);
  response.end(response_json.dump().c_str());

  // Process N1N2 Message Transfer Request in AMF APP
  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }

  oai::utils::utils::bdestroy_wrapper(&n1sm);
  oai::utils::utils::bdestroy_wrapper(&n2sm);
  oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
  oai::utils::utils::bdestroy_wrapper(&routing_id);
}

//------------------------------------------------------------------------------
void amf_http2_server::n1_message_notify_handler(
    const std::string& ueContextId,
    std::unordered_map<std::string, mime_part>& parts,
    const response& response) {
  Logger::amf_server().debug("Receive N1MessageNotify, handling...");

  nlohmann::json response_json = {};
  response_json["cause"] =
      n1_n2_message_transfer_cause_e2str[N1_N2_TRANSFER_INITIATED];
  uint32_t code =
      static_cast<uint32_t>(oai::common::sbi::http_status_code::NO_CONTENT);

  std::string supi                            = ueContextId;
  N1MessageNotification n1MessageNotification = {};
  nlohmann::json::parse(parts[JSON_CONTENT_ID_MIME].body.c_str())
      .get_to(n1MessageNotification);

  Logger::amf_server().debug("N1MessageContainer is present, handling...");

  std::shared_ptr<itti_sbi_n1_message_notification> itti_msg =
      std::make_shared<itti_sbi_n1_message_notification>(
          TASK_AMF_SBI, TASK_AMF_APP);

  switch (n1MessageNotification.getN1MessageContainer()
              .getN1MessageClass()
              .getEnumValue()) {
    case N1MessageClass_anyOf::eN1MessageClass_anyOf::SM: {
      // N1 SM Container Present
      std::string n1_content_id = n1MessageNotification.getN1MessageContainer()
                                      .getN1MessageContent()
                                      .getContentId();
      Logger::amf_server().debug("N1 Content Id: %s", n1_content_id.c_str());

      if (parts.count(n1_content_id) == 0 ||
          parts[n1_content_id].body.size() == 0) {
        code = static_cast<uint32_t>(
            oai::common::sbi::http_status_code::BAD_REQUEST);
        response_json["cause"] =
            n1_n2_message_transfer_cause_e2str[N1_MSG_NOT_TRANSFERRED];
      } else {
        itti_msg->notification_msg = n1MessageNotification;
        itti_msg->ue_id            = supi;
        itti_msg->n1sm             = parts[n1_content_id].body;
        itti_msg->http_version     = 2;
      }
    } break;

    case N1MessageClass_anyOf::eN1MessageClass_anyOf::LPP: {
      // TODO:
    } break;

    default: {
      code = static_cast<uint32_t>(
          oai::common::sbi::http_status_code::BAD_REQUEST);
      response_json["cause"] =
          n1_n2_message_transfer_cause_e2str[N1_MSG_NOT_TRANSFERRED];
    }
  }

  // Send response to the NF Service Consumer (e.g., SMF)
  response.write_head(code);
  if (code ==
      static_cast<uint32_t>(oai::common::sbi::http_status_code::NO_CONTENT)) {
    response.end();
    // Process N1N2 Message Transfer Request in AMF APP
    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_server().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
    }
  } else {
    response.end(response_json.dump().c_str());
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::n1_n2_message_subscribe_handler(
    const std::string& ueContextId,
    const UeN1N2InfoSubscriptionCreateData& ueN1N2InfoSubscriptionCreateData,
    const response& response) {
  Logger::amf_server().debug("Receive N1N2MessageSubscriber, handling...");
  Logger::amf_server().debug("UE Context ID (%s)", ueContextId.c_str());

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the N1N2SubscribeMessage in amf_app
  std::shared_ptr<itti_sbi_n1n2_message_subscribe> itti_msg =
      std::make_shared<itti_sbi_n1n2_message_subscribe>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->ue_cxt_id         = ueContextId;
  itti_msg->subscription_data = ueN1N2InfoSubscriptionCreateData;
  itti_msg->http_version      = 2;
  itti_msg->promise_id        = promise_id;

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
    // process data
    std::string location        = {};
    uint32_t http_response_code = 0;
    if (result.find("location") != result.end()) {
      location = result["location"].get<std::string>();
    }

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    // UeN1N2InfoSubscriptionCreatedData
    nlohmann::json json_data = {};
    if (result.find("createdData") != result.end()) {
      json_data = result["createdData"];
    }

    if (http_response_code == oai::common::sbi::http_status_code::CREATED) {
      h.insert(std::make_pair<std::string, header_value>(
          "Location", {location, false}));
      h.insert(std::make_pair<std::string, header_value>(
          "Content-Type", {"application/json", false}));

      response.write_head(
          static_cast<uint32_t>(oai::common::sbi::http_status_code::CREATED),
          h);
      response.end(json_data.dump().c_str());

    } else {
      response.write_head(http_response_code);
      response.end();
    }
  } else {
    response.write_head(static_cast<uint32_t>(
        oai::common::sbi::http_status_code::GATEWAY_TIMEOUT));
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::n1_n2_message_unsubscribe_handler(
    const std::string& ueContextId, const std::string& subscriptionId,
    const response& response) {
  Logger::amf_server().debug("Receive N1N2MessageUnsubscriber, handling...");
  Logger::amf_server().debug(
      "UE Context ID %s, Subscription ID %s", ueContextId.c_str(),
      subscriptionId.c_str());

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the N1N2UnsubscribeMessage in amf_app
  std::shared_ptr<itti_sbi_n1n2_message_unsubscribe> itti_msg =
      std::make_shared<itti_sbi_n1n2_message_unsubscribe>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->ue_cxt_id       = ueContextId;
  itti_msg->subscription_id = subscriptionId;
  itti_msg->http_version    = 2;
  itti_msg->promise_id      = promise_id;

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
    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == oai::common::sbi::http_status_code::NO_CONTENT) {
      response.write_head(http_response_code);
      response.end();

    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      h.emplace("content-type", header_value{"application/problem+json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    }
  } else {
    response.write_head(
        static_cast<uint32_t>(
            oai::common::sbi::http_status_code::GATEWAY_TIMEOUT),
        h);
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::non_ue_n2_info_subscribe_handler(
    const NonUeN2InfoSubscriptionCreateData& subscriptionCreateData,
    const response& response) {
  Logger::amf_server().debug("Receive NonUeN2InfoSubscribe, handling...");

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the NonUeN2InfoSubscribe in amf_app
  std::shared_ptr<itti_sbi_non_ue_n2_info_subscribe> itti_msg =
      std::make_shared<itti_sbi_non_ue_n2_info_subscribe>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->subscription_data = subscriptionCreateData;
  itti_msg->http_version      = 2;
  itti_msg->promise_id        = promise_id;

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
    // process data
    std::string location        = {};
    uint32_t http_response_code = 0;
    if (result.find("location") != result.end()) {
      location = result["location"].get<std::string>();
    }

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    // NonUeN2InfoSubscriptionCreatedData
    nlohmann::json json_data = {};
    if (result.find("createdData") != result.end()) {
      json_data = result["createdData"];
    }

    if (http_response_code == oai::common::sbi::http_status_code::CREATED) {
      h.insert(std::make_pair<std::string, header_value>(
          "Location", {location, false}));
      h.insert(std::make_pair<std::string, header_value>(
          "Content-Type", {"application/json", false}));

      response.write_head(
          static_cast<uint32_t>(oai::common::sbi::http_status_code::CREATED),
          h);
      response.end(json_data.dump().c_str());

    } else {
      response.write_head(http_response_code);
      response.end();
    }
  } else {
    response.write_head(static_cast<uint32_t>(
        oai::common::sbi::http_status_code::GATEWAY_TIMEOUT));
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::non_ue_n2_info_unsubscribe_handler(
    const std::string& subscriptionId, const response& response) {
  Logger::amf_server().debug("Receive NonUeN2InfoUnSubscribe, handling...");
  Logger::amf_server().debug("Subscription ID %s", subscriptionId.c_str());

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the NonUEN2InfoUnsubscribe in amf_app
  std::shared_ptr<itti_sbi_non_ue_n2_info_unsubscribe> itti_msg =
      std::make_shared<itti_sbi_non_ue_n2_info_unsubscribe>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->subscription_id = subscriptionId;
  itti_msg->http_version    = 2;
  itti_msg->promise_id      = promise_id;

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
    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == oai::common::sbi::http_status_code::NO_CONTENT) {
      response.write_head(http_response_code);
      response.end();

    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      h.emplace("content-type", header_value{"application/problem+json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    }
  } else {
    response.write_head(
        static_cast<uint32_t>(
            oai::common::sbi::http_status_code::GATEWAY_TIMEOUT),
        h);
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::status_notify_handler(
    const std::string& ueContextId, uint8_t pduSessionId,
    const SmContextStatusNotification& statusNotification,
    const response& response) {
  Logger::amf_server().debug("Receive an NF Status Notify, handling...");
  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the PDU Session Release in amf_app
  std::shared_ptr<itti_sbi_pdu_session_release_notif> itti_msg =
      std::make_shared<itti_sbi_pdu_session_release_notif>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->http_version                = 2;
  itti_msg->promise_id                  = promise_id;
  itti_msg->ue_id                       = ueContextId;
  itti_msg->pdu_session_id              = pduSessionId;
  itti_msg->smContextStatusNotification = statusNotification;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }

  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);

  if (result_opt.has_value()) {
    Logger::amf_server().debug("Got result for promise ID %d", promise_id);
    nlohmann::json result = result_opt.value();
    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == oai::common::sbi::http_status_code::OK) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }

      h.emplace("content-type", header_value{"application/json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());

    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      h.emplace("content-type", header_value{"application/problem+json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    }

  } else {
    response.write_head(static_cast<uint32_t>(
        oai::common::sbi::http_status_code::GATEWAY_TIMEOUT));
    response.end();
  }
}
//------------------------------------------------------------------------------
void amf_http2_server::get_configuration_handler(const response& response) {
  Logger::amf_server().debug("Get AMFConfiguration, handling...");
  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the AMFConfiguration in amf_app
  std::shared_ptr<itti_sbi_amf_configuration> itti_msg =
      std::make_shared<itti_sbi_amf_configuration>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->http_version = 2;
  itti_msg->promise_id   = promise_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }

  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);

  if (result_opt.has_value()) {
    Logger::amf_server().debug("Got result for promise ID %d", promise_id);
    nlohmann::json result = result_opt.value();
    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == oai::common::sbi::http_status_code::OK) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }

      h.emplace("content-type", header_value{"application/json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());

    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      h.emplace("content-type", header_value{"application/problem+json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    }
  } else {
    response.write_head(static_cast<uint32_t>(
        oai::common::sbi::http_status_code::GATEWAY_TIMEOUT));
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::update_configuration_handler(
    nlohmann::json& configuration_info, const response& response) {
  header_map h;

  Logger::amf_server().debug("Update AMFConfiguration, handling...");

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_amf_app->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_amf_app->add_promise(promise_id, p);

  // Handle the AMFConfiguration in amf_app
  std::shared_ptr<itti_sbi_update_amf_configuration> itti_msg =
      std::make_shared<itti_sbi_update_amf_configuration>(
          TASK_AMF_SBI, TASK_AMF_APP, promise_id);

  itti_msg->http_version  = 2;
  itti_msg->promise_id    = promise_id;
  itti_msg->configuration = configuration_info;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }

  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);

  if (result_opt.has_value()) {
    Logger::amf_server().debug("Got result for promise ID %d", promise_id);
    nlohmann::json result = result_opt.value();

    // process data
    uint32_t http_response_code = {0};
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == oai::common::sbi::http_status_code::OK) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }
      h.emplace("content-type", header_value{"application/json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }

      h.emplace("content-type", header_value{"application/problem+json"});
      response.write_head(http_response_code);
      response.end(json_data.dump().c_str());
    }

  } else {
    response.write_head(static_cast<uint32_t>(
        oai::common::sbi::http_status_code::GATEWAY_TIMEOUT));
    response.end();
  }
}

//------------------------------------------------------------------------------
void amf_http2_server::stop() {
  server.stop();
  while (running_server) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  Logger::amf_server().info("HTTP2 server should be fully stopped");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
