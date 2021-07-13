/**
 * Namf_EventExposure
 * Session Management Event Exposure Service. © 2019, 3GPP Organizational
 * Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 */

/*
 * SubscriptionsCollectionApi.h
 *
 *
 */

#ifndef SubscriptionsCollectionApi_H_
#define SubscriptionsCollectionApi_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>

#include "NamfEventExposure.h"
#include "ProblemDetails.h"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

class SubscriptionsCollectionApi {
 public:
  SubscriptionsCollectionApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~SubscriptionsCollectionApi() {}
  void init();

  const std::string base = "/namf_event-exposure/";

 private:
  void setupRoutes();

  void create_individual_subcription_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void subscriptions_collection_api_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  /// <summary>
  /// Create an individual subscription for event notifications from the AMF
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="namfEventExposure"></param>
  virtual void create_individual_subcription(
      const NamfEventExposure& namfEventExposure,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif /* SubscriptionsCollectionApi_H_ */
