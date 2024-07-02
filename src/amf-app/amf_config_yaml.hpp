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

#ifndef _AMF_CONFIG_YAML_H_
#define _AMF_CONFIG_YAML_H_

#include "amf_config.hpp"
#include "config.hpp"

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
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SMF_SELECTION =
    "enable_smf_selection";
constexpr auto AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SMF_SELECTION_LABEL =
    "Enable SMF Selection";
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

constexpr auto AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS =
    "supported_integrity_algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS_LABEL =
    "Supported Integrity Algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS =
    "supported_encryption_algorithms";
constexpr auto AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS_LABEL =
    "Supported Encryption Algorithms";

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

constexpr uint8_t SST_MIN_VALUE  = 0;
constexpr uint8_t SST_MAX_VALUE  = 255;
constexpr auto SD_REGEX          = "^[A-Fa-f0-9]{6}$";
constexpr uint32_t TAC_MIN_VALUE = 0;
constexpr uint32_t TAC_MAX_VALUE = 16777215;  // 0xffffff
constexpr uint8_t AMF_CONFIG_RELATIVE_CAPACITY_MIN_VALUE = 0;
constexpr uint8_t AMF_CONFIG_RELATIVE_CAPACITY_MAX_VALUE = 255;
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MIN_VALUE =
    5;  // in seconds
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MAX_VALUE =
    600;  // in seconds

// Default values
constexpr auto AMF_CONFIG_INSTANCE_ID_DEFAULT_VALUE       = 1;
constexpr auto AMF_CONFIG_PID_DIRECTORY_DEFAULT_VALUE     = "/var/run";
constexpr auto AMF_CONFIG_TEST_PLMN_MCC                   = "001";
constexpr auto AMF_CONFIG_TEST_PLMN_MNC                   = "01";
constexpr auto AMF_CONFIG_DEFAULT_SST                     = 1;
constexpr auto AMF_CONFIG_AMF_REGION_ID_DEFAULT_VALUE     = "ff";   // hex
constexpr auto AMF_CONFIG_AMF_SET_ID_DEFAULT_VALUE        = "001";  // hex
constexpr auto AMF_CONFIG_AMF_POINTER_DEFAULT_VALUE       = "01";   // hex
constexpr auto AMF_CONFIG_TAC_DEFAULT_VALUE               = 1;
constexpr auto AMF_CONFIG_RELATIVE_CAPACITY_DEFAULT_VALUE = 10;
constexpr uint32_t AMF_CONFIG_STATISTICS_TIMER_INTERVAL_DEFAULT_VALUE =
    20;  // in seconds

namespace oai::config {

class amf_support_features : public config_type {
 private:
  option_config_value m_enable_simple_scenario{};
  option_config_value m_enable_nssf{};
  option_config_value m_enable_smf_selection{};

 public:
  explicit amf_support_features();

  void from_yaml(const YAML::Node& node) override;

  [[nodiscard]] std::string to_string(const std::string& indent) const override;
  [[nodiscard]] bool get_option_enable_simple_scenario() const;
  [[nodiscard]] bool get_option_enable_nssf() const;
  [[nodiscard]] bool get_option_enable_smf_selection() const;
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
};

class amf_config_yaml : public config {
 public:
  explicit amf_config_yaml(
      const std::string& config_path, bool log_stdout, bool log_rot_file);
  virtual ~amf_config_yaml();

  void to_amf_config(amf_config& cfg);
  void pre_process();
};
}  // namespace oai::config

#endif
