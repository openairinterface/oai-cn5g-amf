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

/*! \file amf_msg.cpp
 \brief
 \author  Shivam Gandhi
 \company KCL
 \date 2021
 \email: shivam.gandhi@kcl.ac.uk
 */
#include "amf_msg.hpp"

namespace amf {

/*
 * class: Event Exposure
 */

//-----------------------------------------------------------------------------
supi_t event_exposure_msg::get_supi() const {
  return m_supi;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_supi(const supi_t& value) {
  m_supi        = value;
  m_supi_is_set = true;
}

//-----------------------------------------------------------------------------
bool event_exposure_msg::is_supi_is_set() const {
  return m_supi_is_set;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_supi_prefix() const {
  return m_supi_prefix;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_supi_prefix(const std::string& prefix) {
  m_supi_prefix = prefix;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_sub_id(std::string const& value) {
  m_sub_id        = value;
  m_sub_id_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_sub_id() const {
  return m_sub_id;
}

//-----------------------------------------------------------------------------
bool event_exposure_msg::is_sub_id_is_set() const {
  return m_sub_id_is_set;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_notif_uri(std::string const& value) {
  m_notif_uri = value;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_notif_uri() const {
  return m_notif_uri;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_notif_id(std::string const& value) {
  m_notif_id = value;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_notif_id() const {
  return m_notif_id;
}

//-----------------------------------------------------------------------------
std::vector<event_subscription_t> event_exposure_msg::get_event_subs() const {
  return m_event_subs;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_event_subs(
    std::vector<event_subscription_t> const& value) {
  m_event_subs.clear();
  for (auto it : value) {
    m_event_subs.push_back(it);
  }
}

/*
 * class: Event Notification
 */
//-----------------------------------------------------------------------------

void event_notification::set_amf_event(const amf_event_t& ev) {
  m_event = ev;
}
//-----------------------------------------------------------------------------
amf_event_t event_notification::get_amf_event() const {
  return m_event;
}

//-----------------------------------------------------------------------------
supi64_t event_notification::get_supi() const {
  return m_supi;
}

//-----------------------------------------------------------------------------
void event_notification::set_supi(const supi64_t& value) {
  m_supi        = value;
  m_supi_is_set = true;
}

//-----------------------------------------------------------------------------
bool event_notification::is_supi_is_set() const {
  return m_supi_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_ad_ipv4_addr(std::string const& value) {
  m_ad_ipv4_addr        = value;
  m_ad_ipv4_addr_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_ad_ipv4_addr() const {
  return m_ad_ipv4_addr;
}

//-----------------------------------------------------------------------------
bool event_notification::is_ad_ipv4_addr_is_set() const {
  return m_ad_ipv4_addr_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_re_ipv4_addr(std::string const& value) {
  m_re_ipv4_addr        = value;
  m_re_ipv4_addr_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_re_ipv4_addr() const {
  return m_re_ipv4_addr;
}

//-----------------------------------------------------------------------------
bool event_notification::is_re_ipv4_addr_is_set() const {
  return m_re_ipv4_addr_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_notif_uri(std::string const& value) {
  m_notif_uri = value;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_notif_uri() const {
  return m_notif_uri;
}

//-----------------------------------------------------------------------------
void event_notification::set_notif_id(std::string const& value) {
  m_notif_id = value;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_notif_id() const {
  return m_notif_id;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_notification_event_type(
    const std::string& type) {
  notification_event_type = type;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_notification_event_type(
    std::string& type) const {
  type = notification_event_type;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_nf_instance_uri(const std::string& uri) {
  nf_instance_uri = uri;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_nf_instance_uri(std::string& uri) const {
  uri = nf_instance_uri;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_profile(const std::shared_ptr<nf_profile>& p) {
  profile = p;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_profile(std::shared_ptr<nf_profile>& p) const {
  p = profile;
}
}  // namespace amf
