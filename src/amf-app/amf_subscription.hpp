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

/*! \file amf_subscription.hpp
 \brief
 \author  Shivam Gandhi
 \company KCL
 \date 2021
 \email: shivam.gandhi@kcl.ac.uk
 */

#include <map>
#include <memory>
#include <shared_mutex>
#include <utility>
#include <vector>

#include "3gpp_29.508.h"
#include "common_root_types.h"
#include "itti.hpp"

namespace amf {

/*
 * Manage the Subscription Info
 */
class amf_subscription {
 public:
  amf_subscription() {}

 public:
  evsub_id_t sub_id;
  amf_event_t ev_type;
  supi64_t supi;
  std::string notif_id;
  std::string notif_uri;
};

}  // namespace amf
