/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_CONFIG_H_
#define _AMF_CONFIG_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <nlohmann/json.hpp>

#include "3gpp_24.501.hpp"
#include "amf.hpp"
#include "common_defs.h"
#include "config.hpp"
#include "if.hpp"
#include "sbi_helper.hpp"
#include "thread_sched.hpp"

constexpr auto AMF_CONFIG_INSTANCE_ID         = "instance_id";
constexpr auto AMF_CONFIG_INSTANCE_ID_LABEL   = "Instance ID";
constexpr auto AMF_CONFIG_PID_DIRECTORY       = "pid_directory";
constexpr auto AMF_CONFIG_PID_DIRECTORY_LABEL = "PID Directory";
constexpr auto AMF_CONFIG_AMF_NAME            = "amf_name";
constexpr auto AMF_CONFIG_AMF_NAME_LABEL      = "AMF Name";

constexpr auto AMF_CONFIG_SUPPORT_FEATURES       = "support_features_options";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_LABEL = "Support Features Options";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SIMPLE_SCENARIO =
    "enable_simple_scenario";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SIMPLE_SCENARIO_LABEL =
    "Enable Simple Scenario";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_NSSF       = "enable_nssf";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_NSSF_LABEL = "Enable NSSF";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_PCF        = "enable_pcf";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_PCF_LABEL  = "Enable PCF";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_ADVANCED_FEATURES =
    "enable_advanced_features";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_ADVANCED_FEATURES_LABEL =
    "Enable Advanced Features";
constexpr auto
    AMF_CONFIG_SUPPORT_FEATURES_ENABLE_ACCESS_AND_MOBILITY_SUBSCRIPTION_DATA_RETRIEVAL =
        "enable_access_and_mobility_subscription_data_retrieval";
constexpr auto
    AMF_CONFIG_SUPPORT_FEATURES_ENABLE_ACCESS_AND_MOBILITY_SUBSCRIPTION_DATA_RETRIEVAL_LABEL =
        "Enable Access and Mobility Subscription Data Retrieval";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_AM_POLICY_ASSOCIATION =
    "enable_am_policy_association";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_AM_POLICY_ASSOCIATION_LABEL =
    "Enable AM Policy Association";
constexpr auto AMF_CONFIG_RELATIVE_CAPACITY       = "relative_capacity";
constexpr auto AMF_CONFIG_RELATIVE_CAPACITY_LABEL = "Relative Capacity";
constexpr auto AMF_CONFIG_STATISTICS_TIMER_INTERVAL =
    "statistics_timer_interval";
constexpr auto AMF_CONFIG_STATISTICS_TIMER_INTERVAL_LABEL =
    "Statistics Timer Interval";

constexpr auto AMF_CONFIG_EMERGENCY_SUPPORT       = "emergency_support";
constexpr auto AMF_CONFIG_EMERGENCY_SUPPORT_LABEL = "Emergency Support";

constexpr auto AMF_CONFIG_SERVED_GUAMI_LIST       = "served_guami_list";
constexpr auto AMF_CONFIG_SERVED_GUAMI_LIST_LABEL = "Served GUAMI List";
constexpr auto AMF_CONFIG_MCC                     = "mcc";
constexpr auto AMF_CONFIG_MCC_LABEL               = "MCC";
constexpr auto AMF_CONFIG_MNC                     = "mnc";
constexpr auto AMF_CONFIG_MNC_LABEL               = "MNC";
constexpr auto AMF_CONFIG_AMF_REGION_ID           = "amf_region_id";
constexpr auto AMF_CONFIG_AMF_REGION_ID_LABEL     = "AMF Region ID";
constexpr auto AMF_CONFIG_AMF_SET_ID              = "amf_set_id";
constexpr auto AMF_CONFIG_AMF_SET_ID_LABEL        = "AMF Set ID";
constexpr auto AMF_CONFIG_AMF_POINTER             = "amf_pointer";
constexpr auto AMF_CONFIG_AMF_POINTER_LABEL       = "AMF Pointer";

constexpr auto AMF_CONFIG_PLMN_SUPPORT_LIST       = "plmn_support_list";
constexpr auto AMF_CONFIG_PLMN_SUPPORT_LIST_LABEL = "PLMN Support List";
constexpr auto AMF_CONFIG_TAC                     = "tac";
constexpr auto AMF_CONFIG_TAC_LABEL               = "TAC";
constexpr auto AMF_CONFIG_NSSAI                   = "nssai";
constexpr auto AMF_CONFIG_NSSAI_LABEL             = "NSSAI";
constexpr auto AMF_CONFIG_SST                     = "sst";
constexpr auto AMF_CONFIG_SST_LABEL               = "SST";
constexpr auto AMF_CONFIG_SD                      = "sd";
constexpr auto AMF_CONFIG_SD_LABEL                = "SD";
constexpr auto AMF_CONFIG_N2                      = "n2";
constexpr auto AMF_CONFIG_N2_LABEL                = "N2";
constexpr auto AMF_CONFIG_SCTP_TTL                = "sctp_ttl";
constexpr auto AMF_CONFIG_SCTP_TTL_LABEL          = "SCTP TTL";
constexpr auto AMF_CONFIG_DEFAULT_DNN             = "default_dnn";
constexpr auto AMF_CONFIG_DEFAULT_DNN_LABEL       = "Default DNN";
constexpr auto AMF_CONFIG_PAGING                  = "paging";
constexpr auto AMF_CONFIG_PAGING_LABEL            = "Paging";
constexpr auto AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE =
    "max_transactions_per_ue";
constexpr auto AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE_LABEL =
    "Max Transactions Per UE";
constexpr auto AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC =
    "registration_defer_timeout_sec";
constexpr auto AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC_LABEL =
    "Registration Defer Timeout";
constexpr auto AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC =
    "temporary_unreachable_defer_timeout_sec";
constexpr auto AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC_LABEL =
    "Temporary Unreachable Defer Timeout";
constexpr auto AMF_CONFIG_PAGING_ENABLE_SUBSCRIPTION_NOTIFICATIONS =
    "enable_subscription_notifications";
constexpr auto AMF_CONFIG_PAGING_ENABLE_SUBSCRIPTION_NOTIFICATIONS_LABEL =
    "Enable Subscription Notifications";
constexpr auto AMF_CONFIG_PAGING_ENABLE_EXTENDED_NGAP_IES =
    "enable_extended_ngap_ies";
constexpr auto AMF_CONFIG_PAGING_ENABLE_EXTENDED_NGAP_IES_LABEL =
    "Enable Extended NGAP IEs";
constexpr auto AMF_CONFIG_PAGING_T3513_INTERVAL_SEC = "t3513_interval_sec";
constexpr auto AMF_CONFIG_PAGING_T3513_INTERVAL_SEC_LABEL =
    "T3513 Interval (sec)";
constexpr auto AMF_CONFIG_PAGING_T3513_MAX_RETRANSMISSIONS =
    "t3513_max_retransmissions";
constexpr auto AMF_CONFIG_PAGING_T3513_MAX_RETRANSMISSIONS_LABEL =
    "T3513 Max Retransmissions";
constexpr auto AMF_CONFIG_PAGING_T3565_INTERVAL_SEC = "t3565_interval_sec";
constexpr auto AMF_CONFIG_PAGING_T3565_INTERVAL_SEC_LABEL =
    "T3565 Interval (sec)";
constexpr auto AMF_CONFIG_PAGING_T3565_MAX_RETRANSMISSIONS =
    "t3565_max_retransmissions";
constexpr auto AMF_CONFIG_PAGING_T3565_MAX_RETRANSMISSIONS_LABEL =
    "T3565 Max Retransmissions";
constexpr auto AMF_CONFIG_PAGING_STAGED_PAGING_ENABLED =
    "staged_paging_enabled";
constexpr auto AMF_CONFIG_PAGING_STAGED_PAGING_ENABLED_LABEL =
    "Staged Paging Enabled";
constexpr auto AMF_CONFIG_PAGING_NARROW_PAGING_FIRST_HOP_TIMEOUT_SEC =
    "narrow_paging_first_hop_timeout_sec";
constexpr auto AMF_CONFIG_PAGING_NARROW_PAGING_FIRST_HOP_TIMEOUT_SEC_LABEL =
    "Narrow Paging First-Hop Timeout (sec)";
constexpr auto AMF_CONFIG_PAGING_ARP_PRIORITY_THRESHOLD_FOR_PRI_PAGING =
    "arp_priority_threshold_for_pri_paging";
constexpr auto AMF_CONFIG_PAGING_ARP_PRIORITY_THRESHOLD_FOR_PRI_PAGING_LABEL =
    "ARP Priority Threshold for Priority Paging";
constexpr auto AMF_CONFIG_PAGING_DEFAULT_PAGING_DRX = "default_paging_drx";
constexpr auto AMF_CONFIG_PAGING_DEFAULT_PAGING_DRX_LABEL =
    "Default Paging DRX";
// old-vs-new 5G-S-TMSI retransmit threshold (TS 24.501 §5.5.1.3.8 NOTE)
constexpr auto AMF_CONFIG_PAGING_NARROW_ATTEMPTS_USING_OLD_5G_STMSI =
    "narrow_attempts_using_old_5g_stmsi";
constexpr auto AMF_CONFIG_PAGING_NARROW_ATTEMPTS_USING_OLD_5G_STMSI_LABEL =
    "Narrow Attempts Using Old 5G-S-TMSI";

constexpr auto AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS =
    "supported_integrity_algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS_LABEL =
    "Supported Integrity Algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS =
    "supported_encryption_algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS_LABEL =
    "Supported Encryption Algorithms";

constexpr auto AMF_CONFIG_OPTION_YES_STR = "Yes";
constexpr auto AMF_CONFIG_OPTION_NO_STR  = "No";

// Regular Expression
constexpr auto MCC_REGEX               = "^[0-9]{3}$";
constexpr auto MNC_REGEX               = "^[0-9]{2,3}$";
constexpr auto AMF_REGION_ID_REGEX     = "^[A-Fa-f0-9]{2}$";
constexpr uint8_t AMF_REGION_ID_LENGTH = 2;
constexpr auto AMF_SET_ID_REGEX        = "^[0-3][A-Fa-f0-9]{2}$";
constexpr uint8_t AMF_SET_ID_LENGTH    = 3;
constexpr auto AMF_POINTER_REGEX       = "^[0-3][A-Fa-f0-9]$";
constexpr uint8_t AMF_ID_LENGTH        = 6;

constexpr auto SUPPORTED_INTEGRITY_ALGORITHMS_REGEX  = "^NIA[0-7]$";
constexpr auto SUPPORTED_ENCRYPTION_ALGORITHMS_REGEX = "^NEA[0-7]$";

constexpr uint8_t SST_MIN_VALUE = 0;
constexpr uint8_t SST_MAX_VALUE = 255;
constexpr auto SD_REGEX         = "(^[A-Fa-f0-9]{6}$)|(^0(x|X)[A-Fa-f0-9]{6}$)";
constexpr uint32_t TAC_MIN_VALUE = 0;
constexpr uint32_t TAC_MAX_VALUE = 16777215;  // 0xffffff
constexpr uint8_t AMF_CONFIG_RELATIVE_CAPACITY_MIN_VALUE = 0;
constexpr uint8_t AMF_CONFIG_RELATIVE_CAPACITY_MAX_VALUE = 255;
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MIN_VALUE =
    5;  // in seconds
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MAX_VALUE =
    600;  // in seconds
constexpr uint32_t AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE_MIN_VALUE = 1;
constexpr uint32_t AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE_MAX_VALUE = 1024;
constexpr uint32_t AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC_MIN_VALUE =
    1;
constexpr uint32_t AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC_MAX_VALUE =
    3600;
constexpr uint32_t
    AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC_MIN_VALUE = 1;
constexpr uint32_t
    AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC_MAX_VALUE  = 3600;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_INTERVAL_SEC_MIN_VALUE        = 1;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_INTERVAL_SEC_MAX_VALUE        = 60;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_MAX_RETRANSMISSIONS_MIN_VALUE = 0;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_MAX_RETRANSMISSIONS_MAX_VALUE = 10;
constexpr uint32_t AMF_CONFIG_PAGING_T3565_INTERVAL_SEC_MIN_VALUE        = 1;
constexpr uint32_t AMF_CONFIG_PAGING_T3565_INTERVAL_SEC_MAX_VALUE        = 60;
constexpr uint32_t AMF_CONFIG_PAGING_T3565_MAX_RETRANSMISSIONS_MIN_VALUE = 0;
constexpr uint32_t AMF_CONFIG_PAGING_T3565_MAX_RETRANSMISSIONS_MAX_VALUE = 10;
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_PAGING_FIRST_HOP_TIMEOUT_SEC_MIN_VALUE = 1;
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_PAGING_FIRST_HOP_TIMEOUT_SEC_MAX_VALUE = 30;
constexpr uint32_t
    AMF_CONFIG_PAGING_ARP_PRIORITY_THRESHOLD_FOR_PRI_PAGING_MIN_VALUE = 1;
constexpr uint32_t
    AMF_CONFIG_PAGING_ARP_PRIORITY_THRESHOLD_FOR_PRI_PAGING_MAX_VALUE = 15;
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_ATTEMPTS_USING_OLD_5G_STMSI_MIN_VALUE = 0;
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_ATTEMPTS_USING_OLD_5G_STMSI_MAX_VALUE = 10;

// Default values
constexpr auto AMF_CONFIG_INSTANCE_ID_DEFAULT_VALUE       = 1;
constexpr auto AMF_CONFIG_AMF_NAME_DEFAULT_VALUE          = "oai-amf";
constexpr auto AMF_CONFIG_PID_DIRECTORY_DEFAULT_VALUE     = "/var/run";
constexpr auto AMF_CONFIG_TEST_PLMN_MCC                   = "001";
constexpr auto AMF_CONFIG_TEST_PLMN_MNC                   = "01";
constexpr auto AMF_CONFIG_DEFAULT_SST                     = 1;
constexpr auto AMF_CONFIG_AMF_REGION_ID_DEFAULT_VALUE     = "ff";   // hex
constexpr auto AMF_CONFIG_AMF_SET_ID_DEFAULT_VALUE        = "001";  // hex
constexpr auto AMF_CONFIG_AMF_POINTER_DEFAULT_VALUE       = "01";   // hex
constexpr auto AMF_CONFIG_TAC_DEFAULT_VALUE               = 1;
constexpr auto AMF_CONFIG_RELATIVE_CAPACITY_DEFAULT_VALUE = 10;
constexpr auto AMF_CONFIG_SCTP_TTL_DEFAULT_VALUE          = 100;
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_DEFAULT_VALUE =
    20;  // in seconds
constexpr auto AMF_CONFIG_DEFAULT_DNN_VALUE = "default";
constexpr uint32_t AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE_DEFAULT_VALUE =
    kPagingMaxPendingMessagesDefault;
constexpr uint32_t
    AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC_DEFAULT_VALUE =
        kPagingRegistrationDeferTimeoutSecDefault;
constexpr uint32_t
    AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC_DEFAULT_VALUE =
        kPagingTemporaryUnreachableDeferTimeoutSecDefault;
constexpr bool
    AMF_CONFIG_PAGING_ENABLE_SUBSCRIPTION_NOTIFICATIONS_DEFAULT_VALUE =
        kPagingEnableSubscriptionNotificationsDefault;
constexpr bool AMF_CONFIG_PAGING_ENABLE_EXTENDED_NGAP_IES_DEFAULT_VALUE =
    kPagingEnableExtendedNgapIesDefault;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_INTERVAL_SEC_DEFAULT_VALUE =
    kPagingT3513IntervalSec;
constexpr uint32_t AMF_CONFIG_PAGING_T3513_MAX_RETRANSMISSIONS_DEFAULT_VALUE =
    0;  // discretionary per TS 24.501 §5.6.2.2.1
constexpr uint32_t AMF_CONFIG_PAGING_T3565_INTERVAL_SEC_DEFAULT_VALUE =
    kPagingT3565IntervalSec;
constexpr uint32_t AMF_CONFIG_PAGING_T3565_MAX_RETRANSMISSIONS_DEFAULT_VALUE =
    kPagingT3565MaxRetransmissions;  // TS 24.501 §5.6.3.4
constexpr bool AMF_CONFIG_PAGING_STAGED_PAGING_ENABLED_DEFAULT_VALUE =
    true;  // narrow-before-wide per TS 23.502 §4.2.3.3
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_PAGING_FIRST_HOP_TIMEOUT_SEC_DEFAULT_VALUE =
        kPagingNarrowFirstHopTimeoutSec;
constexpr uint32_t
    AMF_CONFIG_PAGING_ARP_PRIORITY_THRESHOLD_FOR_PRI_PAGING_DEFAULT_VALUE =
        kPagingArpPriorityThresholdForPriPaging;
constexpr auto AMF_CONFIG_PAGING_DEFAULT_PAGING_DRX_DEFAULT_VALUE =
    kPagingDefaultDrx;
// Default 2: first 2 attempts use old 5G-S-TMSI, then switch.
// TS 24.501 §5.5.1.3.8 NOTE leaves threshold implementation-defined.
constexpr uint32_t
    AMF_CONFIG_PAGING_NARROW_ATTEMPTS_USING_OLD_5G_STMSI_DEFAULT_VALUE = 2;

using namespace oai::common::sbi;

namespace oai::config {

class amf_support_features : public config_type {
 private:
  option_config_value m_enable_simple_scenario{};
  option_config_value m_enable_advanced_features{};
  option_config_value m_enable_nssf{};
  option_config_value m_enable_pcf{};
  option_config_value
      m_enable_access_and_mobility_subscription_data_retrieval{};
  option_config_value m_enable_am_policy_association{};

 public:
  explicit amf_support_features();

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] bool get_option_enable_simple_scenario() const;
  [[nodiscard]] bool get_option_enable_advanced_features() const;
  [[nodiscard]] bool get_option_enable_nssf() const;
  [[nodiscard]] bool get_option_enable_pcf() const;
  [[nodiscard]] bool
  get_option_enable_access_and_mobility_subscription_data_retrieval() const;
  [[nodiscard]] bool get_option_enable_am_policy_association() const;
};

class guami : public config_type {
 private:
  string_config_value m_mcc{};
  string_config_value m_mnc{};
  string_config_value m_amf_region_id{};
  string_config_value m_amf_set_id{};
  string_config_value m_amf_pointer{};

 public:
  explicit guami();
  explicit guami(const std::string& mcc, const std::string& mnc);

  void from_yaml(const YAML::Node& node) override;

  void validate() override;
  void set_validation_regex(const std::string& regex);

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] std::string get_mcc() const;
  [[nodiscard]] std::string get_mnc() const;
  [[nodiscard]] std::string get_amf_region_id() const;
  [[nodiscard]] std::string get_amf_set_id() const;
  [[nodiscard]] std::string get_amf_pointer() const;
};

class s_nssai : public config_type {
 private:
  int_config_value m_sst{};
  string_config_value m_sd{};  // in hex

 public:
  explicit s_nssai(uint8_t sst);
  explicit s_nssai(uint8_t sst, const std::string& sd);

  void from_yaml(const YAML::Node& node) override;

  void validate() override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] bool get_sd(std::string& sd) const;
  [[nodiscard]] std::string get_sd() const;
  [[nodiscard]] int get_sst() const;
};

class plmn_support_item : public config_type {
 private:
  string_config_value m_mcc{};
  string_config_value m_mnc{};
  int_config_value m_tac{};  // TODO: string
  std::vector<s_nssai> m_nssai;

 public:
  explicit plmn_support_item();
  explicit plmn_support_item(const std::string& mcc, const std::string& mnc);

  void from_yaml(const YAML::Node& node) override;

  void validate() override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] std::string get_mcc() const;
  [[nodiscard]] std::string get_mnc() const;
  [[nodiscard]] int get_tac() const;
  [[nodiscard]] std::vector<s_nssai> get_nssai() const;
};

class supported_integrity_algorithms : public config_type {
 private:
  std::vector<string_config_value> m_5g_ia_list;

 public:
  explicit supported_integrity_algorithms();

  void from_yaml(const YAML::Node& node) override;

  void validate() override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] std::vector<std::string> get_supported_integrity_algorithms()
      const;
};

class supported_encryption_algorithms : public config_type {
 private:
  std::vector<string_config_value> m_5g_ea_list;

 public:
  explicit supported_encryption_algorithms();

  void from_yaml(const YAML::Node& node) override;

  void validate() override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] std::vector<std::string> get_supported_encryption_algorithms()
      const;
};

class paging_options : public config_type {
 private:
  int_config_value m_max_transactions_per_ue{};
  int_config_value m_registration_defer_timeout_sec{};
  int_config_value m_temporary_unreachable_defer_timeout_sec{};
  option_config_value m_enable_subscription_notifications{};
  option_config_value m_enable_extended_ngap_ies{};
  // Operator-tunable timer
  int_config_value m_t3513_interval_sec{};
  int_config_value m_t3513_max_retransmissions{};
  int_config_value m_t3565_interval_sec{};
  int_config_value m_t3565_max_retransmissions{};
  option_config_value m_staged_paging_enabled{};
  int_config_value m_narrow_paging_first_hop_timeout_sec{};
  int_config_value m_arp_priority_threshold_for_pri_paging{};
  // "v32" | "v64" | "v128" | "v256" — validated against e_Ngap_PagingDRX keys
  string_config_value m_default_paging_drx{};
  // Old-vs-new 5G-S-TMSI threshold (TS 24.501 §5.5.1.3.8 NOTE)
  int_config_value m_narrow_attempts_using_old_5g_stmsi{};

 public:
  explicit paging_options();

  void from_yaml(const YAML::Node& node) override;

  void validate() override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] uint32_t get_max_transactions_per_ue() const;
  [[nodiscard]] uint32_t get_registration_defer_timeout_sec() const;
  [[nodiscard]] uint32_t get_temporary_unreachable_defer_timeout_sec() const;
  [[nodiscard]] bool get_enable_subscription_notifications() const;
  [[nodiscard]] bool get_enable_extended_ngap_ies() const;
  [[nodiscard]] uint32_t get_t3513_interval_sec() const;
  [[nodiscard]] uint32_t get_t3513_max_retransmissions() const;
  [[nodiscard]] uint32_t get_t3565_interval_sec() const;
  [[nodiscard]] uint32_t get_t3565_max_retransmissions() const;
  [[nodiscard]] bool get_staged_paging_enabled() const;
  [[nodiscard]] uint32_t get_narrow_paging_first_hop_timeout_sec() const;
  [[nodiscard]] uint32_t get_arp_priority_threshold_for_pri_paging() const;
  [[nodiscard]] std::string get_default_paging_drx() const;
  // TS 24.501 §5.5.1.3.8 NOTE
  [[nodiscard]] uint32_t get_narrow_attempts_using_old_5g_stmsi() const;
};

typedef struct support_features_s {
  bool enable_nf_registration;
  bool enable_simple_scenario;
  bool enable_advanced_features;
  bool enable_nssf;
  bool enable_lmf;
  bool enable_pcf;
  bool enable_access_and_mobility_subscription_data_retrieval;
  bool enable_am_policy_association;

  uint8_t http_version;
  nlohmann::json to_json() const {
    nlohmann::json json_data              = {};
    json_data["enable_simple_scenario"]   = this->enable_simple_scenario;
    json_data["enable_advanced_features"] = this->enable_advanced_features;
    json_data["enable_nf_registration"]   = this->enable_nf_registration;
    json_data["enable_nssf"]              = this->enable_nssf;
    json_data["enable_lmf"]               = this->enable_lmf;
    json_data["enable_pcf"]               = this->enable_pcf;
    json_data["enable_access_and_mobility_subscription_data_retrieval"] =
        this->enable_access_and_mobility_subscription_data_retrieval;
    json_data["enable_am_policy_association"] =
        this->enable_am_policy_association;
    json_data["http_version"] = this->http_version;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    try {
      if (json_data.find("enable_simple_scenario") != json_data.end()) {
        this->enable_simple_scenario =
            json_data["enable_simple_scenario"].get<bool>();
      }
      if (json_data.find("enable_advanced_features") != json_data.end()) {
        this->enable_advanced_features =
            json_data["enable_advanced_features"].get<bool>();
      }

      if (json_data.find("enable_nf_registration") != json_data.end()) {
        this->enable_nf_registration =
            json_data["enable_nf_registration"].get<bool>();
      }
      if (json_data.find("enable_nssf") != json_data.end()) {
        this->enable_nssf = json_data["enable_nssf"].get<bool>();
      }
      if (json_data.find("enable_lmf") != json_data.end()) {
        this->enable_lmf = json_data["enable_lmf"].get<bool>();
      }
      if (json_data.find("enable_pcf") != json_data.end()) {
        this->enable_pcf = json_data["enable_pcf"].get<bool>();
      }
      if (json_data.find(
              "enable_access_and_mobility_subscription_data_retrieval") !=
          json_data.end()) {
        this->enable_access_and_mobility_subscription_data_retrieval =
            json_data["enable_access_and_mobility_subscription_data_retrieval"]
                .get<bool>();
      }
      if (json_data.find("enable_am_policy_association") != json_data.end()) {
        this->enable_am_policy_association =
            json_data["enable_am_policy_association"].get<bool>();
      }
      if (json_data.find("http_version") != json_data.end()) {
        this->http_version = json_data["http_version"].get<int>();
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
    }
  }

} support_features_t;

typedef struct paging_cfg_s {
  uint32_t max_transactions_per_ue;
  uint32_t registration_defer_timeout_sec;
  uint32_t temporary_unreachable_defer_timeout_sec;
  bool enable_subscription_notifications;
  bool enable_extended_ngap_ies;
  // -tunable timer and policy knobs
  uint32_t t3513_interval_sec;
  uint32_t t3513_max_retransmissions;
  uint32_t t3565_interval_sec;
  uint32_t t3565_max_retransmissions;
  bool staged_paging_enabled;
  uint32_t narrow_paging_first_hop_timeout_sec;
  uint32_t arp_priority_threshold_for_pri_paging;
  std::string default_paging_drx;  // "v32"|"v64"|"v128"|"v256"
  // Old-vs-new 5G-S-TMSI retransmit threshold (TS 24.501 §5.5.1.3.8 NOTE)
  uint32_t narrow_attempts_using_old_5g_stmsi;

  nlohmann::json to_json() const {
    nlohmann::json json_data             = {};
    json_data["max_transactions_per_ue"] = this->max_transactions_per_ue;
    json_data["registration_defer_timeout_sec"] =
        this->registration_defer_timeout_sec;
    json_data["temporary_unreachable_defer_timeout_sec"] =
        this->temporary_unreachable_defer_timeout_sec;
    json_data["enable_subscription_notifications"] =
        this->enable_subscription_notifications;
    json_data["enable_extended_ngap_ies"]  = this->enable_extended_ngap_ies;
    json_data["t3513_interval_sec"]        = this->t3513_interval_sec;
    json_data["t3513_max_retransmissions"] = this->t3513_max_retransmissions;
    json_data["t3565_interval_sec"]        = this->t3565_interval_sec;
    json_data["t3565_max_retransmissions"] = this->t3565_max_retransmissions;
    json_data["staged_paging_enabled"]     = this->staged_paging_enabled;
    json_data["narrow_paging_first_hop_timeout_sec"] =
        this->narrow_paging_first_hop_timeout_sec;
    json_data["arp_priority_threshold_for_pri_paging"] =
        this->arp_priority_threshold_for_pri_paging;
    json_data["default_paging_drx"] = this->default_paging_drx;
    json_data["narrow_attempts_using_old_5g_stmsi"] =
        this->narrow_attempts_using_old_5g_stmsi;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    try {
      if (json_data.find("max_transactions_per_ue") != json_data.end()) {
        this->max_transactions_per_ue =
            json_data["max_transactions_per_ue"].get<uint32_t>();
      }
      if (json_data.find("registration_defer_timeout_sec") != json_data.end()) {
        this->registration_defer_timeout_sec =
            json_data["registration_defer_timeout_sec"].get<uint32_t>();
      }
      if (json_data.find("temporary_unreachable_defer_timeout_sec") !=
          json_data.end()) {
        this->temporary_unreachable_defer_timeout_sec =
            json_data["temporary_unreachable_defer_timeout_sec"]
                .get<uint32_t>();
      }
      if (json_data.find("enable_subscription_notifications") !=
          json_data.end()) {
        this->enable_subscription_notifications =
            json_data["enable_subscription_notifications"].get<bool>();
      }
      if (json_data.find("enable_extended_ngap_ies") != json_data.end()) {
        this->enable_extended_ngap_ies =
            json_data["enable_extended_ngap_ies"].get<bool>();
      }
      if (json_data.find("t3513_interval_sec") != json_data.end()) {
        this->t3513_interval_sec =
            json_data["t3513_interval_sec"].get<uint32_t>();
      }
      if (json_data.find("t3513_max_retransmissions") != json_data.end()) {
        this->t3513_max_retransmissions =
            json_data["t3513_max_retransmissions"].get<uint32_t>();
      }
      if (json_data.find("t3565_interval_sec") != json_data.end()) {
        this->t3565_interval_sec =
            json_data["t3565_interval_sec"].get<uint32_t>();
      }
      if (json_data.find("t3565_max_retransmissions") != json_data.end()) {
        this->t3565_max_retransmissions =
            json_data["t3565_max_retransmissions"].get<uint32_t>();
      }
      if (json_data.find("staged_paging_enabled") != json_data.end()) {
        this->staged_paging_enabled =
            json_data["staged_paging_enabled"].get<bool>();
      }
      if (json_data.find("narrow_paging_first_hop_timeout_sec") !=
          json_data.end()) {
        this->narrow_paging_first_hop_timeout_sec =
            json_data["narrow_paging_first_hop_timeout_sec"].get<uint32_t>();
      }
      if (json_data.find("arp_priority_threshold_for_pri_paging") !=
          json_data.end()) {
        this->arp_priority_threshold_for_pri_paging =
            json_data["arp_priority_threshold_for_pri_paging"].get<uint32_t>();
      }
      if (json_data.find("default_paging_drx") != json_data.end()) {
        this->default_paging_drx =
            json_data["default_paging_drx"].get<std::string>();
      }
      if (json_data.find("narrow_attempts_using_old_5g_stmsi") !=
          json_data.end()) {
        this->narrow_attempts_using_old_5g_stmsi =
            json_data["narrow_attempts_using_old_5g_stmsi"].get<uint32_t>();
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
    }
  }
} paging_cfg_t;

class amf : public nf {
 private:
  int_config_value m_instance_id;
  string_config_value m_pid_directory;
  string_config_value m_amf_name;
  amf_support_features m_amf_support_features;
  int_config_value m_relative_capacity;
  int_config_value m_statistics_timer_interval;
  option_config_value m_emergency_support;
  std::vector<guami> m_guami_list;
  std::vector<plmn_support_item> m_plmn_support_list;
  supported_integrity_algorithms m_supported_integrity_algorithms;
  supported_encryption_algorithms m_supported_encryption_algorithms;
  local_interface m_n2;
  int_config_value m_sctp_ttl;
  string_config_value m_default_dnn;
  paging_options m_paging_options;

 public:
  explicit amf(
      const std::string& name, const std::string& host,
      const sbi_interface& sbi, const local_interface& local);

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  void validate() override;

  [[nodiscard]] const uint32_t get_instance_id() const;
  [[nodiscard]] const std::string get_pid_directory() const;
  [[nodiscard]] const std::string get_amf_name() const;
  amf_support_features get_support_features() const;
  [[nodiscard]] const uint32_t get_relative_capacity() const;
  [[nodiscard]] const uint32_t get_statistics_timer_interval() const;
  std::vector<guami> get_guami_list() const;
  std::vector<plmn_support_item> get_plmn_list() const;
  [[nodiscard]] std::vector<std::string> get_supported_integrity_algorithms()
      const;
  [[nodiscard]] std::vector<std::string> get_supported_encryption_algorithms()
      const;
  [[nodiscard]] const local_interface& get_n2() const;
  [[nodiscard]] const uint32_t get_sctp_ttl() const;
  [[nodiscard]] const std::string get_default_dnn() const;
  [[nodiscard]] paging_options get_paging() const;
};

class amf_config : public config {
 public:
  explicit amf_config(
      const std::string& config_path, bool log_stdout, bool log_rot_file);
  virtual ~amf_config();

  /*
   * Convert the AMF configuration parameters into internal variables
   * @param void
   * @return void
   */
  void pre_process();

  /*
   * Display the AMF configuration parameters
   * @param void
   * @return void
   */
  void display();

  /*
   * Represent AMF's config as json object
   * @param [nlohmann::json &] json_data: Json data
   * @return void
   */
  void to_json(nlohmann::json& json_data) const;

  /*
   * Update AMF's config from Json
   * @param [nlohmann::json &] json_data: Updated configuration in json format
   * @return true if success otherwise return false
   */
  bool from_json(nlohmann::json& json_data);

  unsigned int instance;
  std::string pid_dir;
  spdlog::level::level_enum amf_log_level;
  interface_cfg_t n2;
  interface_cfg_t sbi;
  itti_cfg_t itti;

  uint32_t sctp_ttl;
  uint32_t http_request_timeout;
  unsigned int statistics_interval;
  std::string amf_name;
  guami_full_format_t guami;
  std::vector<guami_full_format_t> guami_list;
  unsigned int relative_amf_capacity;
  std::vector<plmn_item_t> plmn_list;
  bool is_emergency_support;
  auth_conf_t auth_para;
  nas_conf_t nas_cfg;
  support_features_t support_features;
  paging_cfg_t paging;
  nf_addr_t smf_addr;
  nf_addr_t nrf_addr;
  nf_addr_t ausf_addr;
  nf_addr_t udm_addr;
  nf_addr_t nssf_addr;
  nf_addr_t lmf_addr;
  nf_addr_t pcf_addr;

  std::string default_dnn;
};

}  // namespace oai::config

#endif
