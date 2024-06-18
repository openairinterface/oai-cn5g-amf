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

/*
 * StatusNotifyApi.h
 *
 *
 */

#ifndef StatusNotifyApi_H_
#define StatusNotifyApi_H_

#include <SmContextStatusNotification.h>
#include <pistache/http.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>

#include "amf_sbi_helper.hpp"

namespace oai::amf::api {

using namespace oai::model::amf;

class StatusNotifyApi {
 public:
  StatusNotifyApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~StatusNotifyApi() {}
  void init();

  const std::string base = amf_sbi_helper::AmfStatusNotifyServiceBase;

 private:
  void setupRoutes();

  void notify_pdu_session_status_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void notify_status_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  /// <summary>
  ///
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="NotificationData"></param>
  virtual void receive_pdu_session_status_notification(
      const std::string& ueContextId, const std::string& pduSessionId,
      const SmContextStatusNotification& statusNotification,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace oai::amf::api

#endif /* StatusNotifyApi_H_ */
