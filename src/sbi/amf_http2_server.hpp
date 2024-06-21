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

#ifndef FILE_AMF_HTTP2_SERVER_SEEN
#define FILE_AMF_HTTP2_SERVER_SEEN

#include <nghttp2/asio_http2_server.h>

#include "AmfCreateEventSubscription.h"
#include "N1N2MessageTransferError.h"
#include "N1N2MessageTransferReqData.h"
#include "N1N2MessageTransferRspData.h"
#include "N2InformationTransferReqData.h"
#include "NonUeN2InfoSubscriptionCreateData.h"
#include "amf.hpp"
#include "amf_app.hpp"
#include "mime_parser.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#include "uint_generator.hpp"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;
using namespace oai::model::amf;

class amf_http2_server {
 public:
  amf_http2_server(
      std::string addr, uint32_t port, amf_application::amf_app* amf_app_inst)
      : m_address(addr), m_port(port), server(), m_amf_app(amf_app_inst) {}
  virtual ~amf_http2_server(){};
  void start();
  void init(size_t thr) {}

  void create_event_subscription_handler(
      const AmfCreateEventSubscription& amfCreateEventSubscription,
      const response& response);

  void n1_n2_message_transfer_handler(
      const std::string& ueContextId,
      std::unordered_map<std::string, mime_part>& parts, const response& res);

  void n1_message_notify_handler(
      const std::string& ueContextId,
      std::unordered_map<std::string, mime_part>& parts, const response& res);

  void n1_n2_message_subscribe_handler(
      const std::string& ueContextId,
      const UeN1N2InfoSubscriptionCreateData& ueN1N2InfoSubscriptionCreateData,
      const response& response);

  void n1_n2_message_unsubscribe_handler(
      const std::string& ueContextId, const std::string& subscriptionId,
      const response& response);

  void non_ue_n2_info_subscribe_handler(
      const NonUeN2InfoSubscriptionCreateData& subscriptionCreateData,
      const response& response);

  void non_ue_n2_info_unsubscribe_handler(
      const std::string& subscriptionId, const response& response);

  void status_notify_handler(
      const std::string& ueContextId, uint8_t pduSessionId,
      const SmContextStatusNotification& statusNotification,
      const response& response);

  void get_configuration_handler(const response& response);

  void update_configuration_handler(
      nlohmann::json& configuration_info, const response& response);

  void stop();

 private:
  oai::utils::uint_generator<uint32_t> m_promise_id_generator;
  std::string m_address;
  uint32_t m_port;
  http2 server;
  amf_application::amf_app* m_amf_app;
  bool running_server;

 protected:
  static uint64_t generate_promise_id() {
    return oai::utils::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }
};

#endif
