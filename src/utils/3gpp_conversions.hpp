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

/*! \file 3gpp_conversions.hpp
 \brief
 \author Shivam Gandhi
 \company KCL
 \email: shivam.gandhi@kcl.ac.uk
 */

#ifndef FILE_3GPP_CONVERSIONS_HPP_SEEN
#define FILE_3GPP_CONVERSIONS_HPP_SEEN

#include "NamfEventExposure.h"
#include "NotificationData.h"
#include "amf_msg.hpp"

namespace xgpp_conv {

/*
 * Convert Data Notification from OpenAPI into Data Notification Msg
 * @param [const NotificationData&] nd: Data
 * Notification in OpenAPI
 * @param [amf::data_notification_msg&] dn_msg: Data Notification msg
 * @return void
 */
void data_notification_from_openapi(
    const oai::amf::model::NotificationData& nd,
    amf::data_notification_msg& dn_msg);

/*
 * Convert NamfEventExposure from OpenAPI into Event Exposure Msg
 * @param [const NamfEventExposure&] nee:
 * NamfEventExposure in OpenAPI
 * @param [amf::event_exposure_msg&] eem: Event Exposure Msg
 * @return void
 */
void amf_event_exposure_notification_from_openapi(
    const oai::amf::model::NamfEventExposure& nee,
    amf::event_exposure_msg& eem);

}  // namespace xgpp_conv

#endif /* FILE_3GPP_CONVERSIONS_HPP_SEEN */
