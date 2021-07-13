/**
 * Namf_EventExposure
 * Session Management Event Exposure Service. © 2019, 3GPP Organizational
 * Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 */

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
 * SubscriptionsCollectionApiImpl.h
 *
 *
 */

#ifndef SUBSCRIPTIONS_COLLECTION_API_IMPL_H_
#define SUBSCRIPTIONS_COLLECTION_API_IMPL_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>

#include <SubscriptionsCollectionApi.h>

#include <pistache/optional.h>

#include "NamfEventExposure.h"
#include "ProblemDetails.h"
#include "amf_app.hpp"

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;
using namespace amf_application;

class SubscriptionsCollectionApiImpl
    : public oai::amf::api::SubscriptionsCollectionApi {
 public:
  SubscriptionsCollectionApiImpl(
      std::shared_ptr<Pistache::Rest::Router>, amf_app* amf_app_inst,
      std::string address);
  ~SubscriptionsCollectionApiImpl() {}

  void create_individual_subcription(
      const NamfEventExposure& namfEventExposure,
      Pistache::Http::ResponseWriter& response);

 private:
  amf_app* m_amf_app;
  std::string m_address;

 protected:
  static uint64_t generate_promise_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }
};

}  // namespace api
}  // namespace amf
}  // namespace oai

#endif
