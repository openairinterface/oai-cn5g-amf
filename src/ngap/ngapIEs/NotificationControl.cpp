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

#include "NotificationControl.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NotificationControl::NotificationControl() {
  notification_control_ = 0;
}

//------------------------------------------------------------------------------
NotificationControl::~NotificationControl() {}

//------------------------------------------------------------------------------
void NotificationControl::setNotificationControl(
    e_Ngap_NotificationControl value) {
  notification_control_ = value;
}

//------------------------------------------------------------------------------
bool NotificationControl::getNotificationControl(
    e_Ngap_NotificationControl& value) const {
  if (notification_control_ == Ngap_NotificationControl_notification_requested)
    value = Ngap_NotificationControl_notification_requested;
  else
    return 0;

  return true;
}

//------------------------------------------------------------------------------
bool NotificationControl::encode(
    Ngap_NotificationControl_t& notificationControl) const {
  notificationControl = notification_control_;

  return true;
}

//------------------------------------------------------------------------------
bool NotificationControl::decode(
    Ngap_NotificationControl_t notificationControl) {
  notification_control_ = notificationControl;

  return true;
}
}  // namespace ngap
