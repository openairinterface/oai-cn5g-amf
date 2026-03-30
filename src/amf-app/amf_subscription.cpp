/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "amf_subscription.hpp"

#include "3gpp_conversions.hpp"
#include "logger.hpp"

using namespace amf_application;

void amf_subscription::display() {
  Logger::amf_app().debug("Subscription info");
  Logger::amf_app().debug("\tSubscription ID: %d", (uint32_t) sub_id);
  Logger::amf_app().debug(
      "\tEvent type: %s", xgpp_conv::amf_event_type_to_string(ev_type).c_str());
  if (supi_is_set) Logger::amf_app().debug("\tSUPI: %s", supi.c_str());
  Logger::amf_app().debug(
      "\tNotify Correlation ID: %s", notify_correlation_id.c_str());
  Logger::amf_app().debug("\tNotify URI: %s", notify_uri.c_str());
  Logger::amf_app().debug("\tNF ID: %s", nf_id.c_str());
};
