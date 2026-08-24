/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
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
#include <optional>
#include <pistache/router.h>

#include "amf_sbi_helper.hpp"

namespace oai::amf::api {

using namespace oai::_3gpp::model;

class StatusNotifyApi {
 public:
  StatusNotifyApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~StatusNotifyApi() {}
  void init();

  const std::string base = amf_sbi_helper::AmfStatusNotifyServiceBase();

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
