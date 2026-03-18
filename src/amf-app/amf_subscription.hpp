/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "3gpp_29.518.h"
#include "amf.hpp"

namespace amf_application {

/*
 * Manage the Subscription Info
 */

class amf_subscription {
 public:
  amf_subscription()
      : sub_id(),
        ev_type(),
        supi(),
        notify_correlation_id(),
        notify_uri(),
        nf_id() {
    supi_is_set = false;
  }

  /*
   * Display the AMF Subscription information
   * @param void
   * @return void
   */
  void display();

  evsub_id_t sub_id;
  amf_event_type_t ev_type;
  bool supi_is_set;
  std::string supi;
  std::string notify_correlation_id;
  std::string notify_uri;  // subsChangeNotifyUri ?
  std::string nf_id;
};

}  // namespace amf_application
