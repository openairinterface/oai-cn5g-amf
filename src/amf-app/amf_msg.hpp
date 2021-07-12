/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file amf_msg.hpp
 \brief
 \author  Shivam Gandhi
 \company KCL
 \date 2021
 \email: shivam.gandhi@kcl.ac.uk
 */
#ifndef FILE_AMF_MSG_HPP_SEEN
#define FILE_AMF_MSG_HPP_SEEN

#include "amf.hpp"
#include "3gpp_29.508.h"
#include "pistache/http.h"
#include "amf_profile.hpp"

namespace amf {


class event_exposure_msg {
 public:
  supi_t get_supi() const;
  void set_supi(const supi_t& value);
  bool is_supi_is_set() const;
  std::string get_supi_prefix() const;
  void set_supi_prefix(const std::string& value);
  void set_sub_id(std::string const& value);
  std::string get_sub_id() const;
  bool is_sub_id_is_set() const;
  void set_notif_uri(std::string const& value);
  std::string get_notif_uri() const;
  void set_notif_id(std::string const& value);
  std::string get_notif_id() const;
  std::vector<event_subscription_t> get_event_subs() const;
  void set_event_subs(std::vector<event_subscription_t> const& value);

 private:
  supi_t m_supi;
  bool m_supi_is_set;
  std::string m_supi_prefix;

  std::string m_sub_id;  // m_SubId;
  bool m_sub_id_is_set;
  std::string m_notif_uri;                         // m_NotifUri;
  std::string m_notif_id;                          // m_NotifId;
  std::vector<event_subscription_t> m_event_subs;  // m_EventSubs;

  // NotificationMethod m_NotifMethod;
  // bool m_NotifMethodIsSet;
  // int32_t m_MaxReportNbr;
  // bool m_MaxReportNbrIsSet;
  // std::string m_Expiry;
  // bool m_ExpiryIsSet;
  // int32_t m_RepPeriod;
  // bool m_RepPeriodIsSet;
  // Guami m_Guami;
  // bool m_GuamiIsSet;
  // std::string m_ServiveName;
  // bool m_ServiveNameIsSet;
  // std::vector<std::string> m_AltNotifIpv4Addrs;
  // bool m_AltNotifIpv4AddrsIsSet;
  // std::vector<Ipv6Addr> m_AltNotifIpv6Addrs;
  // bool m_AltNotifIpv6AddrsIsSet;
  // bool m_AnyUeInd;
  // bool m_AnyUeIndIsSet;
  // std::string m_Gpsi;
  // bool m_GpsiIsSet;
  // std::string m_GroupId;
  // bool m_GroupIdIsSet;
  // bool m_ImmeRep;
  // bool m_ImmeRepIsSet;
  // std::string m_SupportedFeatures;
  // bool m_SupportedFeaturesIsSet;
};

class event_notification {
 public:
  void set_amf_event(const amf_event_t& ev);
  amf_event_t get_amf_event() const;

  void set_supi(const supi64_t& supi);
  supi64_t get_supi() const;
  bool is_supi_is_set() const;
  // m_AdIpv4Addr
  void set_ad_ipv4_addr(std::string const& value);
  std::string get_ad_ipv4_addr() const;
  bool is_ad_ipv4_addr_is_set() const;
  // m_ReIpv4Addr
  void set_re_ipv4_addr(std::string const& value);
  std::string get_re_ipv4_addr() const;
  bool is_re_ipv4_addr_is_set() const;

  void set_notif_uri(std::string const& value);
  std::string get_notif_uri() const;
  void set_notif_id(std::string const& value);
  std::string get_notif_id() const;

 private:
  std::string m_notif_uri;  // m_NotifUri;
  std::string m_notif_id;   // m_NotifId;

  amf_event_t m_event;  // AmfEvent
  // std::string m_TimeStamp;

  supi64_t m_supi;
  bool m_supi_is_set;

  // for a UE IP address change
  std::string m_ad_ipv4_addr;  // m_AdIpv4Addr
  bool m_ad_ipv4_addr_is_set;  // m_AdIpv4AddrIsSet;
  std::string m_re_ipv4_addr;  // m_ReIpv4Addr;
  bool m_re_ipv4_addr_is_set;  // m_ReIpv4AddrIsSet;

  // for a PLMN Change
  // PlmnId m_PlmnId;
  // bool m_PlmnIdIsSet;

  // for an access type change
  // AccessType m_AccType;
  // bool m_AccTypeIsSet;

  // std::string m_Gpsi;
  // bool m_GpsiIsSet;
  // std::string m_SourceDnai;
  // bool m_SourceDnaiIsSet;
  // std::string m_TargetDnai;
  // bool m_TargetDnaiIsSet;
  // DnaiChangeType m_DnaiChgType;
  // bool m_DnaiChgTypeIsSet;
  // std::string m_TargetUeIpv4Addr;
  // bool m_TargetUeIpv4AddrIsSet;
  // std::string m_SourceUeIpv4Addr;
  // bool m_SourceUeIpv4AddrIsSet;
  // Ipv6Prefix m_SourceUeIpv6Prefix;
  // bool m_SourceUeIpv6PrefixIsSet;
  // Ipv6Prefix m_TargetUeIpv6Prefix;
  // bool m_TargetUeIpv6PrefixIsSet;
  // RouteToLocation m_SourceTraRouting;
  // bool m_SourceTraRoutingIsSet;
  // RouteToLocation m_TargetTraRouting;
  // bool m_TargetTraRoutingIsSet;
  // std::string m_UeMac;
  // bool m_UeMacIsSet;
  // Ipv6Prefix m_AdIpv6Prefix;
  // bool m_AdIpv6PrefixIsSet;
  // Ipv6Prefix m_ReIpv6Prefix;
  // bool m_ReIpv6PrefixIsSet;
  // DddStatus m_DddStatus;
  // bool m_DddStatusIsSet;
  // std::string m_MaxWaitTime;
  // bool m_MaxWaitTimeIsSet;
};

class data_notification_msg {
 public:
  void set_notification_event_type(const std::string& type);
  void get_notification_event_type(std::string& type) const;
  void set_nf_instance_uri(const std::string& uri);
  void get_nf_instance_uri(std::string& uri) const;
  void set_profile(const std::shared_ptr<nf_profile>& p);
  void get_profile(std::shared_ptr<nf_profile>& p) const;

 private:
  std::string notification_event_type;
  std::string nf_instance_uri;
  std::shared_ptr<nf_profile> profile;
  // bool m_NfProfileIsSet;
  // std::vector<ChangeItem> m_ProfileChanges;
  // bool m_ProfileChangesIsSet;
};
}  // namespace amf

#endif