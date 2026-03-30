/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_3GPP_CONVERSIONS_HPP_SEEN
#define FILE_3GPP_CONVERSIONS_HPP_SEEN

#include "3gpp_29.518.h"
#include "AmfCreateEventSubscription.h"
#include "amf_msg.hpp"

using namespace amf_application;
namespace xgpp_conv {

/*
 * Convert AmfCreatedEventSubscription from OpenAPI into Event Exposure Msg
 * @param [const oai::_3gpp::model::AmfCreatedEventSubscription&]
 * event_subscription: AmfCreatedEventSubscription in OpenAPI
 * @param [amf_application::event_exposure_msg&] event_exposure: Event Exposure
 * Msg
 * @return void
 */
void amf_event_subscription_from_openapi(
    const oai::_3gpp::model::AmfCreateEventSubscription& event_subscription,
    event_exposure_msg& event_exposure);

std::string amf_event_type_to_string(amf_event_type_t type);
}  // namespace xgpp_conv

#endif /* FILE_3GPP_CONVERSIONS_HPP_SEEN */
