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

#include "amf_config.hpp"

#include "3gpp_29.502.h"
#include "amf_app.hpp"
#include "amf_conversions.hpp"
#include "amf_sbi_helper.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace amf_application;
using namespace oai::amf::api;

namespace oai::config {

//------------------------------------------------------------------------------
amf_support_features::amf_support_features() {
  m_set = true;
}

//------------------------------------------------------------------------------
void amf_support_features::from_yaml(const YAML::Node& node) {
  if (node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SIMPLE_SCENARIO]) {
    m_enable_simple_scenario.from_yaml(
        node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SIMPLE_SCENARIO]);
  }
  if (node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_NSSF]) {
    m_enable_nssf.from_yaml(node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_NSSF]);
  }
  if (node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SMF_SELECTION]) {
    m_enable_smf_selection.from_yaml(
        node[AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SMF_SELECTION]);
  }
}

//------------------------------------------------------------------------------
std::string amf_support_features::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());

  std::string enable_simple_scenario_string =
      m_enable_simple_scenario.get_value() ? AMF_CONFIG_OPTION_YES_STR :
                                             AMF_CONFIG_OPTION_NO_STR;
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM,
      AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SIMPLE_SCENARIO_LABEL, inner_width,
      enable_simple_scenario_string));

  std::string enable_nssf_string = m_enable_nssf.get_value() ?
                                       AMF_CONFIG_OPTION_YES_STR :
                                       AMF_CONFIG_OPTION_NO_STR;
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM,
      AMF_CONFIG_SUPPORT_FEATURES_ENABLE_NSSF_LABEL, inner_width,
      enable_nssf_string));

  std::string enable_smf_selection_string = m_enable_smf_selection.get_value() ?
                                                AMF_CONFIG_OPTION_YES_STR :
                                                AMF_CONFIG_OPTION_NO_STR;
  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM,
      AMF_CONFIG_SUPPORT_FEATURES_ENABLE_SMF_SELECTION_LABEL, inner_width,
      enable_smf_selection_string));
  return out;
}

//------------------------------------------------------------------------------
bool amf_support_features::get_option_enable_simple_scenario() const {
  return m_enable_simple_scenario.get_value();
}

//------------------------------------------------------------------------------
bool amf_support_features::get_option_enable_nssf() const {
  return m_enable_nssf.get_value();
}

//------------------------------------------------------------------------------
bool amf_support_features::get_option_enable_smf_selection() const {
  return m_enable_smf_selection.get_value();
}

//------------------------------------------------------------------------------
guami::guami() {
  m_mcc = string_config_value(AMF_CONFIG_MCC, AMF_CONFIG_TEST_PLMN_MCC);
  m_mcc.set_validation_regex(MCC_REGEX);
  m_mnc = string_config_value(AMF_CONFIG_MNC, AMF_CONFIG_TEST_PLMN_MNC);
  m_mnc.set_validation_regex(MNC_REGEX);
  m_amf_region_id = string_config_value(
      AMF_CONFIG_AMF_REGION_ID, AMF_CONFIG_AMF_REGION_ID_DEFAULT_VALUE);
  m_amf_region_id.set_validation_regex(AMF_REGION_ID_REGEX);
  m_amf_set_id = string_config_value(
      AMF_CONFIG_AMF_SET_ID, AMF_CONFIG_AMF_SET_ID_DEFAULT_VALUE);
  m_amf_set_id.set_validation_regex(AMF_SET_ID_REGEX);
  m_amf_pointer = string_config_value(
      AMF_CONFIG_AMF_POINTER, AMF_CONFIG_AMF_POINTER_DEFAULT_VALUE);
  m_amf_pointer.set_validation_regex(AMF_POINTER_REGEX);
  m_set = true;
}

//------------------------------------------------------------------------------
guami::guami(const std::string& mcc, const std::string& mnc) : guami() {
  m_mcc = string_config_value(AMF_CONFIG_MCC, mcc);
  m_mnc = string_config_value(AMF_CONFIG_MNC, mnc);
  m_mcc.set_validation_regex(MCC_REGEX);
  m_mnc.set_validation_regex(MNC_REGEX);
  m_amf_region_id = string_config_value(
      AMF_CONFIG_AMF_REGION_ID, AMF_CONFIG_AMF_REGION_ID_DEFAULT_VALUE);
  m_amf_region_id.set_validation_regex(AMF_REGION_ID_REGEX);
  m_amf_set_id = string_config_value(
      AMF_CONFIG_AMF_SET_ID, AMF_CONFIG_AMF_SET_ID_DEFAULT_VALUE);
  m_amf_set_id.set_validation_regex(AMF_SET_ID_REGEX);
  m_amf_pointer = string_config_value(
      AMF_CONFIG_AMF_POINTER, AMF_CONFIG_AMF_POINTER_DEFAULT_VALUE);
  m_amf_pointer.set_validation_regex(AMF_POINTER_REGEX);
  m_set = true;
}

//------------------------------------------------------------------------------
void guami::from_yaml(const YAML::Node& node) {
  if (node[AMF_CONFIG_MCC]) {
    m_mcc.from_yaml(node[AMF_CONFIG_MCC]);
  }
  if (node[AMF_CONFIG_MNC]) {
    m_mnc.from_yaml(node[AMF_CONFIG_MNC]);
  }
  if (node[AMF_CONFIG_AMF_REGION_ID]) {
    m_amf_region_id.from_yaml(node[AMF_CONFIG_AMF_REGION_ID]);
  }
  if (node[AMF_CONFIG_AMF_SET_ID]) {
    m_amf_set_id.from_yaml(node[AMF_CONFIG_AMF_SET_ID]);
  }
  if (node[AMF_CONFIG_AMF_POINTER]) {
    m_amf_pointer.from_yaml(node[AMF_CONFIG_AMF_POINTER]);
  }
}

//------------------------------------------------------------------------------
std::string guami::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, AMF_CONFIG_MCC_LABEL, inner_width,
      m_mcc.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_MNC_LABEL, inner_width,
      m_mnc.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_AMF_REGION_ID_LABEL,
      inner_width, m_amf_region_id.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_AMF_SET_ID_LABEL, inner_width,
      m_amf_set_id.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_AMF_POINTER_LABEL,
      inner_width, m_amf_pointer.get_value()));

  return out;
}

//------------------------------------------------------------------------------
std::string guami::get_mcc() const {
  return m_mcc.get_value();
}

//------------------------------------------------------------------------------
std::string guami::get_mnc() const {
  return m_mnc.get_value();
}

//------------------------------------------------------------------------------
std::string guami::get_amf_region_id() const {
  return m_amf_region_id.get_value();
}

//------------------------------------------------------------------------------
std::string guami::get_amf_set_id() const {
  return m_amf_set_id.get_value();
}

//------------------------------------------------------------------------------
std::string guami::get_amf_pointer() const {
  return m_amf_pointer.get_value();
}

//------------------------------------------------------------------------------
void guami::validate() {
  if (!m_set) return;
  m_mcc.validate();
  m_mnc.validate();
  m_amf_region_id.validate();
  m_amf_set_id.validate();
  m_amf_pointer.validate();
}

//------------------------------------------------------------------------------
s_nssai::s_nssai(uint8_t sst) {
  m_sst = int_config_value(AMF_CONFIG_SST, sst);
  m_sst.set_validation_interval(SST_MIN_VALUE, SST_MAX_VALUE);
  m_sd =
      string_config_value(AMF_CONFIG_SD, oai::model::common::SD_DEFAULT_VALUE);
  m_sd.set_validation_regex(SD_REGEX);
  m_set = true;
}

//------------------------------------------------------------------------------
s_nssai::s_nssai(uint8_t sst, const std::string& sd) {
  m_sst = int_config_value(AMF_CONFIG_SST, sst);
  m_sst.set_validation_interval(SST_MIN_VALUE, SST_MAX_VALUE);
  m_sd = string_config_value(AMF_CONFIG_SD, sd);
  m_sd.set_validation_regex(SD_REGEX);
  m_set = true;
}

//------------------------------------------------------------------------------
void s_nssai::from_yaml(const YAML::Node& node) {
  if (node[AMF_CONFIG_SST]) {
    m_sst.from_yaml(node[AMF_CONFIG_SST]);
  }
  if (node[AMF_CONFIG_SD]) {
    m_sd.from_yaml(node[AMF_CONFIG_SD]);
  }
}

//------------------------------------------------------------------------------
std::string s_nssai::to_string(const std::string& indent) const {
  std::string out;
  unsigned int inner_width = get_inner_width(indent.length());

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, AMF_CONFIG_SST_LABEL, inner_width,
      m_sst.get_value()));

  if (m_sd.is_set())
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_SD_LABEL, inner_width,
        get_sd()));

  return out;
}

//------------------------------------------------------------------------------
int s_nssai::get_sst() const {
  return m_sst.get_value();
}

//------------------------------------------------------------------------------
bool s_nssai::get_sd(std::string& sd) const {
  if (m_sd.is_set()) {
    std::string tmp = m_sd.get_value();
    if (tmp.size() == 8)
      sd = tmp.substr(2);
    else
      sd = tmp;
  }
  return m_sd.is_set();
}

//------------------------------------------------------------------------------
std::string s_nssai::get_sd() const {
  if (m_sd.is_set()) {
    std::string tmp = m_sd.get_value();
    if (tmp.size() == 8)
      return tmp.substr(2);
    else
      return tmp;
  }
  return {};
}

//------------------------------------------------------------------------------
void s_nssai::validate() {
  if (!m_set) return;
  m_sst.validate();
  m_sd.validate();
}

//------------------------------------------------------------------------------
plmn_support_item::plmn_support_item() {
  m_mcc = string_config_value(AMF_CONFIG_MCC, AMF_CONFIG_TEST_PLMN_MCC);
  m_mcc.set_validation_regex(MCC_REGEX);
  m_mnc = string_config_value(AMF_CONFIG_MNC, AMF_CONFIG_TEST_PLMN_MNC);
  m_mnc.set_validation_regex(MNC_REGEX);
  m_tac = int_config_value(AMF_CONFIG_TAC, AMF_CONFIG_TAC_DEFAULT_VALUE);
  m_tac.set_validation_interval(TAC_MIN_VALUE, TAC_MAX_VALUE);
  m_set = true;
}

//------------------------------------------------------------------------------
plmn_support_item::plmn_support_item(
    const std::string& mcc, const std::string& mnc) {
  m_mcc = string_config_value(AMF_CONFIG_MCC, mcc);
  m_mcc.set_validation_regex(MCC_REGEX);
  m_mnc = string_config_value(AMF_CONFIG_MNC, mnc);
  m_mnc.set_validation_regex(MNC_REGEX);
  m_tac = int_config_value(AMF_CONFIG_TAC, AMF_CONFIG_TAC_DEFAULT_VALUE);
  m_tac.set_validation_interval(TAC_MIN_VALUE, TAC_MAX_VALUE);
  m_set = true;
}

//------------------------------------------------------------------------------
void plmn_support_item::from_yaml(const YAML::Node& node) {
  if (node[AMF_CONFIG_MCC]) {
    m_mcc.from_yaml(node[AMF_CONFIG_MCC]);
  }
  if (node[AMF_CONFIG_MNC]) {
    m_mnc.from_yaml(node[AMF_CONFIG_MNC]);
  }
  if (node[AMF_CONFIG_TAC]) {
    m_tac.from_yaml(node[AMF_CONFIG_TAC]);
  }

  if (!node[AMF_CONFIG_NSSAI].IsSequence()) {
    Logger::amf_app().warn("Could not parse %s", AMF_CONFIG_NSSAI_LABEL);
  } else {
    for (int i = 0; i < node[AMF_CONFIG_NSSAI].size(); i++) {
      s_nssai snssai(1);
      snssai.from_yaml(node[AMF_CONFIG_NSSAI][i]);
      m_nssai.push_back(snssai);

      oai::model::common::Snssai s_nssai_model = {};
      nlohmann::json j =
          oai::utils::conv::yaml_to_json(node[AMF_CONFIG_NSSAI][i], false);
      nlohmann::from_json(j, s_nssai_model);
    }
  }
}

//------------------------------------------------------------------------------
std::string plmn_support_item::to_string(const std::string& indent) const {
  std::string out;
  std::string inner_indent = indent + indent;
  unsigned int inner_width = get_inner_width(indent.length());

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, INNER_LIST_ELEM, AMF_CONFIG_MCC_LABEL, inner_width,
      m_mcc.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_MNC_LABEL, inner_width,
      m_mnc.get_value()));

  out.append(indent).append(fmt::format(
      BASE_FORMATTER, EMPTY_LIST_ELEM, AMF_CONFIG_TAC_LABEL, inner_width,
      m_tac.get_value()));

  out.append(inner_indent)
      .append(fmt::format("{} {}\n", OUTER_LIST_ELEM, AMF_CONFIG_NSSAI_LABEL));

  for (const auto& i : m_nssai) out.append(i.to_string(inner_indent + indent));
  return out;
}

//------------------------------------------------------------------------------
std::string plmn_support_item::get_mcc() const {
  return m_mcc.get_value();
}

//------------------------------------------------------------------------------
std::string plmn_support_item::get_mnc() const {
  return m_mnc.get_value();
}

//------------------------------------------------------------------------------
int plmn_support_item::get_tac() const {
  return m_tac.get_value();
}

//------------------------------------------------------------------------------
std::vector<s_nssai> plmn_support_item::get_nssai() const {
  return m_nssai;
}

//------------------------------------------------------------------------------
void plmn_support_item::validate() {
  if (!m_set) return;
  m_mcc.validate();
  m_mnc.validate();
  m_tac.validate();
  for (auto& n : m_nssai) {
    n.validate();
  }
}

//------------------------------------------------------------------------------
supported_integrity_algorithms::supported_integrity_algorithms() {
  m_set = true;
}

//------------------------------------------------------------------------------
void supported_integrity_algorithms::from_yaml(const YAML::Node& node) {
  bool no_item_available = false;
  if (!node.IsSequence()) {
    Logger::amf_app().warn(
        "Could not parse %s", AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS_LABEL);
    no_item_available = true;
  } else {
    for (int i = 0; i < node.size(); i++) {
      string_config_value value{};
      value.set_validation_regex(SUPPORTED_INTEGRITY_ALGORITHMS_REGEX);
      value.from_yaml(node[i]);

      m_5g_ia_list.push_back(value);
    }
    if (node.size() == 0) no_item_available = true;
  }
  // Default values
  if (no_item_available) {
    m_5g_ia_list.push_back(string_config_value("NIA", "NIA0"));
    m_5g_ia_list.push_back(string_config_value("NIA", "NIA1"));
    m_5g_ia_list.push_back(string_config_value("NIA", "NIA2"));
  }
}

//------------------------------------------------------------------------------
std::string supported_integrity_algorithms::to_string(
    const std::string& indent) const {
  std::string out;
  std::string inner_indent = indent + indent;
  unsigned int inner_width = get_inner_width(indent.length());

  for (const auto& i : m_5g_ia_list) {
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, INNER_LIST_ELEM, "", inner_width, i.get_value()));
  }
  return out;
}

//------------------------------------------------------------------------------
std::vector<std::string>
supported_integrity_algorithms::get_supported_integrity_algorithms() const {
  std::vector<std::string> _5g_ia_str_list;
  for (const auto& i : m_5g_ia_list) {
    _5g_ia_str_list.push_back(i.get_value());
  }
  // Default values
  if (_5g_ia_str_list.size() == 0) {
    _5g_ia_str_list.push_back("NIA0");
    _5g_ia_str_list.push_back("NIA1");
    _5g_ia_str_list.push_back("NIA2");
  }
  return _5g_ia_str_list;
}

//------------------------------------------------------------------------------
void supported_integrity_algorithms::validate() {
  if (!m_set) return;
  for (auto& ia : m_5g_ia_list) {
    ia.validate();
  }
}

//------------------------------------------------------------------------------
supported_encryption_algorithms::supported_encryption_algorithms() {
  m_set = true;
}

//------------------------------------------------------------------------------
void supported_encryption_algorithms::from_yaml(const YAML::Node& node) {
  bool no_item_available = false;
  if (!node.IsSequence()) {
    Logger::amf_app().warn(
        "Could not parse %s", AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS_LABEL);
    no_item_available = true;
  } else {
    for (int i = 0; i < node.size(); i++) {
      string_config_value value{};
      value.set_validation_regex(SUPPORTED_ENCRYPTION_ALGORITHMS_REGEX);
      value.from_yaml(node[i]);
      m_5g_ea_list.push_back(value);
    }
    if (node.size() == 0) no_item_available = true;
  }
  // Default values
  if (no_item_available) {
    m_5g_ea_list.push_back(string_config_value("NEA", "NEA0"));
    m_5g_ea_list.push_back(string_config_value("NEA", "NEA1"));
    m_5g_ea_list.push_back(string_config_value("NEA", "NEA2"));
  }
}

//------------------------------------------------------------------------------
std::string supported_encryption_algorithms::to_string(
    const std::string& indent) const {
  std::string out;
  std::string inner_indent = indent + indent;
  unsigned int inner_width = get_inner_width(indent.length());

  for (const auto& i : m_5g_ea_list) {
    out.append(indent).append(fmt::format(
        BASE_FORMATTER, INNER_LIST_ELEM, "", inner_width, i.get_value()));
  }
  return out;
}

//------------------------------------------------------------------------------
std::vector<std::string>
supported_encryption_algorithms::get_supported_encryption_algorithms() const {
  std::vector<std::string> _5g_ea_str_list;
  for (const auto& i : m_5g_ea_list) {
    _5g_ea_str_list.push_back(i.get_value());
  }
  // Default values
  if (m_5g_ea_list.size() == 0) {
    _5g_ea_str_list.push_back("NEA0");
    _5g_ea_str_list.push_back("NEA1");
    _5g_ea_str_list.push_back("NEA2");
  }
  return _5g_ea_str_list;
}

//------------------------------------------------------------------------------
void supported_encryption_algorithms::validate() {
  if (!m_set) return;
  for (auto& ea : m_5g_ea_list) {
    ea.validate();
  }
}

//------------------------------------------------------------------------------
amf::amf(
    const std::string& name, const std::string& host, const sbi_interface& sbi,
    const local_interface& local)
    : nf(name, host, sbi) {
  m_n2            = local;
  m_pid_directory = string_config_value(
      AMF_CONFIG_PID_DIRECTORY, AMF_CONFIG_PID_DIRECTORY_DEFAULT_VALUE);
  m_instance_id = int_config_value(
      AMF_CONFIG_INSTANCE_ID, AMF_CONFIG_INSTANCE_ID_DEFAULT_VALUE);
  m_amf_name = string_config_value(
      AMF_CONFIG_AMF_NAME, AMF_CONFIG_AMF_NAME_DEFAULT_VALUE);
  m_sctp_ttl =
      int_config_value(AMF_CONFIG_SCTP_TTL, AMF_CONFIG_SCTP_TTL_DEFAULT_VALUE);
  m_default_dnn =
      string_config_value(AMF_CONFIG_DEFAULT_DNN, AMF_CONFIG_DEFAULT_DNN_VALUE);
  m_relative_capacity = int_config_value(
      AMF_CONFIG_RELATIVE_CAPACITY, AMF_CONFIG_RELATIVE_CAPACITY_DEFAULT_VALUE);
  m_relative_capacity.set_validation_interval(
      AMF_CONFIG_RELATIVE_CAPACITY_MIN_VALUE,
      AMF_CONFIG_RELATIVE_CAPACITY_MAX_VALUE);
  m_statistics_timer_interval = int_config_value(
      AMF_CONFIG_STATISTICS_TIMER_INTERVAL,
      AMF_CONFIG_STATISTICS_TIMER_INTERVAL_DEFAULT_VALUE);
  m_statistics_timer_interval.set_validation_interval(
      AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MIN_VALUE,
      AMF_CONFIG_STATISTICS_TIMER_INTERVAL_MAX_VALUE);
}
//------------------------------------------------------------------------------
void amf::from_yaml(const YAML::Node& node) {
  nf::from_yaml(node);

  // Load AMF specified parameter
  for (const auto& elem : node) {
    auto key = elem.first.as<std::string>();

    if (key == AMF_CONFIG_N2) {
      m_n2.set_host(get_host());
      m_n2.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_INSTANCE_ID) {
      m_instance_id.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_SCTP_TTL) {
      m_sctp_ttl.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_PID_DIRECTORY) {
      m_pid_directory.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_AMF_NAME) {
      m_amf_name.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_DEFAULT_DNN) {
      m_default_dnn.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_RELATIVE_CAPACITY) {
      m_relative_capacity.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_STATISTICS_TIMER_INTERVAL) {
      m_statistics_timer_interval.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_SUPPORT_FEATURES) {
      m_amf_support_features.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_EMERGENCY_SUPPORT) {
      m_emergency_support.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_SERVED_GUAMI_LIST) {
      if (!elem.second.IsSequence()) {
        Logger::amf_app().warn("Could not parse %s", key);
      } else {
        for (int i = 0; i < elem.second.size(); i++) {
          guami g(AMF_CONFIG_TEST_PLMN_MCC, AMF_CONFIG_TEST_PLMN_MNC);
          g.from_yaml(elem.second[i]);
          m_guami_list.push_back(g);
        }
      }
    }

    if (key == AMF_CONFIG_PLMN_SUPPORT_LIST) {
      if (!elem.second.IsSequence()) {
        Logger::amf_app().warn("Could not parse %s", key);
      } else {
        for (int i = 0; i < elem.second.size(); i++) {
          plmn_support_item plmn_item(
              AMF_CONFIG_TEST_PLMN_MCC, AMF_CONFIG_TEST_PLMN_MNC);
          plmn_item.from_yaml(elem.second[i]);
          m_plmn_support_list.push_back(plmn_item);
        }
      }
    }

    if (key == AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS) {
      m_supported_integrity_algorithms.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS) {
      m_supported_encryption_algorithms.from_yaml(elem.second);
    }
  }
}

//------------------------------------------------------------------------------
std::string amf::to_string(const std::string& indent) const {
  std::string out;
  std::string inner_indent = indent + indent;
  unsigned int inner_width = get_inner_width(inner_indent.length());

  out.append(nf::to_string(indent));

  out.append(inner_indent)
      .append(fmt::format("{} {}\n", OUTER_LIST_ELEM, m_n2.get_config_name()));
  out.append(m_n2.to_string(add_indent(inner_indent)));

  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_INSTANCE_ID_LABEL,
          inner_width, m_instance_id.get_value()));
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_PID_DIRECTORY_LABEL,
          inner_width, m_pid_directory.get_value()));
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_AMF_NAME_LABEL,
          inner_width, m_amf_name.get_value()));
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_SCTP_TTL_LABEL,
          inner_width, m_sctp_ttl.get_value()));
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_DEFAULT_DNN_LABEL,
          inner_width, m_default_dnn.get_value()))
      .append("(ms)");

  out.append(inner_indent)
      .append(fmt::format(
          "{} {}\n", OUTER_LIST_ELEM, AMF_CONFIG_SUPPORT_FEATURES_LABEL));
  out.append(m_amf_support_features.to_string(inner_indent + indent));

  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_RELATIVE_CAPACITY_LABEL,
          inner_width, m_relative_capacity.get_value()));

  std::string emergency_support_string = m_emergency_support.get_value() ?
                                             AMF_CONFIG_OPTION_YES_STR :
                                             AMF_CONFIG_OPTION_NO_STR;
  out.append(inner_indent)
      .append(fmt::format(
          BASE_FORMATTER, OUTER_LIST_ELEM, AMF_CONFIG_EMERGENCY_SUPPORT_LABEL,
          inner_width, emergency_support_string));

  out.append(inner_indent)
      .append(fmt::format(
          "{} {}\n", OUTER_LIST_ELEM, AMF_CONFIG_SERVED_GUAMI_LIST_LABEL));
  for (const auto& i : m_guami_list)
    out.append(i.to_string(inner_indent + indent));

  out.append(inner_indent)
      .append(fmt::format(
          "{} {}\n", OUTER_LIST_ELEM, AMF_CONFIG_PLMN_SUPPORT_LIST_LABEL));
  for (const auto& i : m_plmn_support_list)
    out.append(i.to_string(inner_indent + indent));

  out.append(inner_indent)
      .append(fmt::format(
          "{} {}\n", OUTER_LIST_ELEM,
          AMF_CONFIG_SUPPORTED_INTEGRITY_ALGORITHMS_LABEL));
  out.append(m_supported_integrity_algorithms.to_string(inner_indent + indent));

  out.append(inner_indent)
      .append(fmt::format(
          "{} {}\n", OUTER_LIST_ELEM,
          AMF_CONFIG_SUPPORTED_ENCRYPTION_ALGORITHMS_LABEL));
  out.append(
      m_supported_encryption_algorithms.to_string(inner_indent + indent));

  return out;
}

void amf::validate() {
  nf::validate();
  m_sctp_ttl.validate();
  m_instance_id.validate();
  m_relative_capacity.validate();
  m_statistics_timer_interval.validate();
  for (auto& g : m_guami_list) {
    g.validate();
  }
  for (auto& p : m_plmn_support_list) {
    p.validate();
  }
  m_supported_integrity_algorithms.validate();
  m_supported_encryption_algorithms.validate();
  m_n2.validate();
}

//------------------------------------------------------------------------------
const uint32_t amf::get_instance_id() const {
  return m_instance_id.get_value();
}
//------------------------------------------------------------------------------
const std::string amf::get_pid_directory() const {
  return m_pid_directory.get_value();
}

//------------------------------------------------------------------------------
const std::string amf::get_amf_name() const {
  return m_amf_name.get_value();
}

//------------------------------------------------------------------------------
std::vector<guami> amf::get_guami_list() const {
  return m_guami_list;
}

//------------------------------------------------------------------------------
std::vector<plmn_support_item> amf::get_plmn_list() const {
  return m_plmn_support_list;
}

//------------------------------------------------------------------------------
std::vector<std::string> amf::get_supported_integrity_algorithms() const {
  return m_supported_integrity_algorithms.get_supported_integrity_algorithms();
}

//------------------------------------------------------------------------------
std::vector<std::string> amf::get_supported_encryption_algorithms() const {
  return m_supported_encryption_algorithms
      .get_supported_encryption_algorithms();
}

//------------------------------------------------------------------------------
amf_support_features amf::get_support_features() const {
  return m_amf_support_features;
}

//------------------------------------------------------------------------------
const uint32_t amf::get_relative_capacity() const {
  return m_relative_capacity.get_value();
}

//------------------------------------------------------------------------------
const uint32_t amf::get_statistics_timer_interval() const {
  return m_statistics_timer_interval.get_value();
}

//------------------------------------------------------------------------------
const local_interface& amf::get_n2() const {
  return m_n2;
}

//------------------------------------------------------------------------------
const uint32_t amf::get_sctp_ttl() const {
  return m_sctp_ttl.get_value();
}

//------------------------------------------------------------------------------
const std::string amf::get_default_dnn() const {
  return m_default_dnn.get_value();
}

//------------------------------------------------------------------------------
amf_config::amf_config(
    const std::string& config_path, bool log_stdout, bool log_rot_file)
    : oai::config::config(
          config_path, oai::config::AMF_CONFIG_NAME, log_stdout, log_rot_file) {
  m_used_sbi_values = {
      oai::config::AMF_CONFIG_NAME, oai::config::AUSF_CONFIG_NAME,
      oai::config::SMF_CONFIG_NAME, oai::config::UDM_CONFIG_NAME,
      oai::config::NRF_CONFIG_NAME, oai::config::NSSF_CONFIG_NAME};
  m_used_config_values = {
      oai::config::LOG_LEVEL_CONFIG_NAME, oai::config::REGISTER_NF_CONFIG_NAME,
      oai::config::NF_CONFIG_HTTP_NAME,   oai::config::NF_LIST_CONFIG_NAME,
      oai::config::AMF_CONFIG_NAME,       oai::config::DATABASE_CONFIG,
      oai::config::NF_CONFIG_TLS_NAME};

  // TODO with NF_Type and switch
  // TODO: Still we need to add default NFs even we don't use this in all_in_one
  // use case
  auto m_amf = std::make_shared<amf>(
      "AMF", "oai-amf", sbi_interface("SBI", "oai-amf1", 80, "v1", "eth0"),
      local_interface(AMF_CONFIG_N2_LABEL, "oai-amf", 38412, "eth0"));
  add_nf(oai::config::AMF_CONFIG_NAME, m_amf);

  auto m_smf = std::make_shared<nf>(
      "SMF", "oai-smf", sbi_interface("SBI", "oai-smf", 80, "v1", "eth0"));
  add_nf(oai::config::SMF_CONFIG_NAME, m_smf);

  auto m_ausf = std::make_shared<nf>(
      "AUSF", "oai-ausf", sbi_interface("SBI", "oai-ausf", 80, "v1", ""));
  add_nf(oai::config::AUSF_CONFIG_NAME, m_ausf);

  auto m_udm = std::make_shared<nf>(
      "UDM", "oai-udm", sbi_interface("SBI", "oai-udm", 80, "v1", ""));
  add_nf(oai::config::UDM_CONFIG_NAME, m_udm);

  auto m_nrf = std::make_shared<nf>(
      "NRF", "oai-nrf", sbi_interface("SBI", "oai-nrf", 80, "v1", ""));
  add_nf(oai::config::NRF_CONFIG_NAME, m_nrf);

  auto m_nssf = std::make_shared<nf>(
      "NSSF", "oai-nssf", sbi_interface("SBI", "oai-nssf", 80, "v1", ""));
  add_nf("nssf", m_nssf);

  update_used_nfs();
  smf_addr.ipv4_addr.s_addr  = INADDR_ANY;
  smf_addr.port              = DEFAULT_HTTP2_PORT;
  smf_addr.api_version       = DEFAULT_SBI_API_VERSION;
  nrf_addr.ipv4_addr.s_addr  = INADDR_ANY;
  nrf_addr.port              = DEFAULT_HTTP2_PORT;
  nrf_addr.api_version       = DEFAULT_SBI_API_VERSION;
  ausf_addr.ipv4_addr.s_addr = INADDR_ANY;
  ausf_addr.port             = DEFAULT_HTTP2_PORT;
  ausf_addr.api_version      = DEFAULT_SBI_API_VERSION;
  udm_addr.ipv4_addr.s_addr  = INADDR_ANY;
  udm_addr.port              = DEFAULT_HTTP2_PORT;
  udm_addr.api_version       = DEFAULT_SBI_API_VERSION;
  lmf_addr.ipv4_addr.s_addr  = INADDR_ANY;
  lmf_addr.port              = DEFAULT_HTTP2_PORT;
  lmf_addr.api_version       = DEFAULT_SBI_API_VERSION;
  nssf_addr.ipv4_addr.s_addr = INADDR_ANY;
  nssf_addr.port             = DEFAULT_HTTP2_PORT;
  nssf_addr.api_version      = DEFAULT_SBI_API_VERSION;
  instance                   = 0;
  amf_log_level              = spdlog::level::debug;
  n2                         = {};
  sbi                        = {};
  sbi.api_version       = std::optional<std::string>(DEFAULT_SBI_API_VERSION);
  statistics_interval   = 0;
  guami                 = {};
  guami_list            = {};
  relative_amf_capacity = 0;
  plmn_list             = {};
  auth_para             = {};
  nas_cfg               = {};
  support_features.enable_nf_registration   = false;
  support_features.enable_smf_selection     = false;
  support_features.enable_external_ausf_udm = false;
  support_features.enable_nssf              = false;
  support_features.enable_lmf               = false;
  support_features.http_version             = 2;  // HTTP/2 by default
  is_emergency_support                      = false;
}

//------------------------------------------------------------------------------
amf_config::~amf_config() {}

void amf_config::pre_process() {
  // Process configuration information to display only the appropriate
  // information
  std::shared_ptr<amf> amf_local = std::static_pointer_cast<amf>(get_local());
  if (!amf_local->get_support_features().get_option_enable_simple_scenario()) {
    get_database_config().unset_config();
    std::shared_ptr<nf> ausf = get_nf(AUSF_CONFIG_NAME);
    ausf->set_config();
    std::shared_ptr<nf> udm = get_nf(UDM_CONFIG_NAME);
    udm->set_config();
    std::shared_ptr<nf> nrf = get_nf(NRF_CONFIG_NAME);
    nrf->set_config();
    if (amf_local->get_support_features().get_option_enable_nssf()) {
      get_nf(NSSF_CONFIG_NAME)->set_config();
    }
  } else {
    std::shared_ptr<nf> ausf = get_nf(AUSF_CONFIG_NAME);
    ausf->unset_config();
    std::shared_ptr<nf> udm = get_nf(UDM_CONFIG_NAME);
    udm->unset_config();
    std::shared_ptr<nf> nrf = get_nf(NRF_CONFIG_NAME);
    nrf->unset_config();
    // TODO: unset Register_NF
  }

  // TODO: should be removed
  instance      = amf_local->get_instance_id();
  pid_dir       = amf_local->get_pid_directory();
  amf_name      = amf_local->get_amf_name();
  amf_log_level = spdlog::level::from_str(log_level());

  relative_amf_capacity = amf_local->get_relative_capacity();
  statistics_interval   = amf_local->get_statistics_timer_interval();
  http_request_timeout  = get_http_request_timeout();

  // Parse the "Super" option - "enable_simple_scenario"
  if (amf_local->get_support_features().get_option_enable_simple_scenario()) {
    support_features.enable_nf_registration   = false;
    support_features.enable_smf_selection     = false;
    support_features.enable_external_ausf_udm = false;
    support_features.enable_nssf              = false;  // TODO: to be removed
  } else {  // parse the other options
    support_features.enable_nf_registration = register_nrf();
    support_features.enable_smf_selection =
        amf_local->get_support_features().get_option_enable_smf_selection();
    support_features.enable_external_ausf_udm = true;  // To be removed
    support_features.enable_nssf =
        amf_local->get_support_features().get_option_enable_nssf();
  }

  support_features.http_version = get_http_version();

  for (const auto& i : amf_local->get_guami_list()) {
    guami_full_format_t guami_item = {};
    guami_item.mcc                 = i.get_mcc();
    guami_item.mnc                 = i.get_mnc();
    guami_item.amf_set_id =
        oai::utils::conv::string_hex_to_int(i.get_amf_set_id());
    guami_item.region_id =
        oai::utils::conv::string_hex_to_int(i.get_amf_region_id());
    guami_item.amf_pointer =
        oai::utils::conv::string_hex_to_int(i.get_amf_pointer());
    guami = guami_item;
    guami_list.push_back(guami_item);
  }

  for (const auto& i : amf_local->get_plmn_list()) {
    plmn_item_t item = {};
    item.mcc         = i.get_mcc();
    item.mnc         = i.get_mnc();
    item.tac         = i.get_tac();
    for (const auto& s : i.get_nssai()) {
      snssai_t slice = {};
      slice.sst      = s.get_sst();
      slice.sd       = s.get_sd();
      item.slice_list.push_back(slice);
    }

    plmn_list.push_back(item);
  }

  // TODO: Emergency support
  is_emergency_support = false;
  // Database
  if (get_database_config().is_set()) {
    auth_para.mysql_server = get_database_config().get_host();
    auth_para.mysql_user   = get_database_config().get_user();
    auth_para.mysql_pass   = get_database_config().get_pass();
    auth_para.mysql_db     = get_database_config().get_database_name();
    // auth_para.connection_timeout =
    // get_database_config().get_connection_timeout(); database_type =
    // get_database_config().get_database_type();
  }

  sbi.api_version =
      std::make_optional<std::string>(local().get_sbi().get_api_version());
  sbi.port    = local().get_sbi().get_port();
  sbi.addr4   = local().get_sbi().get_addr4();
  sbi.if_name = local().get_sbi().get_if_name();

  n2.if_name = amf_local->get_n2().get_if_name();
  n2.addr4   = amf_local->get_n2().get_addr4();
  n2.port    = amf_local->get_n2().get_port();

  sctp_ttl    = amf_local->get_sctp_ttl();
  default_dnn = amf_local->get_default_dnn();

  if (get_nf(oai::config::SMF_CONFIG_NAME)) {
    smf_addr.api_version =
        get_nf(oai::config::SMF_CONFIG_NAME)->get_sbi().get_api_version();
    smf_addr.uri_root =
        get_nf(oai::config::SMF_CONFIG_NAME)->get_url(amf_cfg->enable_tls());
  }

  if (get_nf(oai::config::AUSF_CONFIG_NAME)) {
    ausf_addr.api_version =
        get_nf(oai::config::AUSF_CONFIG_NAME)->get_sbi().get_api_version();
    ausf_addr.uri_root =
        get_nf(oai::config::AUSF_CONFIG_NAME)->get_url(amf_cfg->enable_tls());
  }

  if (get_nf(oai::config::UDM_CONFIG_NAME)) {
    udm_addr.api_version =
        get_nf(oai::config::UDM_CONFIG_NAME)->get_sbi().get_api_version();
    udm_addr.uri_root =
        get_nf(oai::config::UDM_CONFIG_NAME)->get_url(amf_cfg->enable_tls());
  }

  if (get_nf(oai::config::NRF_CONFIG_NAME)) {
    nrf_addr.api_version =
        get_nf(oai::config::NRF_CONFIG_NAME)->get_sbi().get_api_version();
    nrf_addr.uri_root =
        get_nf(oai::config::NRF_CONFIG_NAME)->get_url(amf_cfg->enable_tls());
  }

  if (get_nf(oai::config::NSSF_CONFIG_NAME)) {
    nssf_addr.api_version =
        get_nf(NSSF_CONFIG_NAME)->get_sbi().get_api_version();
    nssf_addr.uri_root =
        get_nf(NSSF_CONFIG_NAME)->get_sbi().get_url(amf_cfg->enable_tls());
  }

  // NAS conf
  for (const auto& s : amf_local->get_supported_integrity_algorithms()) {
    if (!s.compare("NIA0")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA0);
    }
    if (!s.compare("NIA1")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA1);
    }
    if (!s.compare("NIA2")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA2);
    }
    if (!s.compare("NIA3")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA3);
    }
    if (!s.compare("NIA4")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA4);
    }
    if (!s.compare("NIA5")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA5);
    }
    if (!s.compare("NIA6")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA6);
    }
    if (!s.compare("NIA7")) {
      nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA7);
    }
  }

  // Default values
  if (amf_local->get_supported_integrity_algorithms().size() == 0) {
    nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA0);
    nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA1);
    nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA2);
  }

  for (const auto& s : amf_local->get_supported_encryption_algorithms()) {
    if (!s.compare("NEA0")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA0);
    }
    if (!s.compare("NEA1")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA1);
    }
    if (!s.compare("NEA2")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA2);
    }
    if (!s.compare("NEA3")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA3);
    }
    if (!s.compare("NEA4")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA4);
    }
    if (!s.compare("NEA5")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA5);
    }
    if (!s.compare("NEA6")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA6);
    }
    if (!s.compare("NEA7")) {
      nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA7);
    }
  }

  // Default values
  if (amf_local->get_supported_encryption_algorithms().size() == 0) {
    nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA0);
    nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA1);
    nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA2);
  }
}

//------------------------------------------------------------------------------
void amf_config::display() {
  Logger::config().info(
      "==== OAI-CN5G %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::config().info(
      "======================    AMF   =====================");
  Logger::config().info("Configuration AMF:");
  Logger::config().info("- Instance ................: %d", instance);
  Logger::config().info("- PID dir .................: %s", pid_dir.c_str());
  Logger::config().info("- AMF NAME.................: %s", amf_name.c_str());
  Logger::config().info(
      "- GUAMI (MCC, MNC, Region ID, AMF Set ID, AMF pointer): ");
  Logger::config().info(
      "    (%s, %s, %d, %d, %d)", guami.mcc.c_str(), guami.mnc.c_str(),
      guami.region_id, guami.amf_set_id, guami.amf_pointer);
  Logger::config().info("- Served Guami List:");
  for (int i = 0; i < guami_list.size(); i++) {
    Logger::config().info(
        "    (%s, %s, %d , %d, %d)", guami_list[i].mcc.c_str(),
        guami_list[i].mnc.c_str(), guami_list[i].region_id,
        guami_list[i].amf_set_id, guami_list[i].amf_pointer);
  }
  Logger::config().info(
      "- Relative Capacity .......: %d", relative_amf_capacity);
  Logger::config().info("- PLMN Support: ");
  for (int i = 0; i < plmn_list.size(); i++) {
    Logger::config().info(
        "    MCC, MNC ..............: %s, %s", plmn_list[i].mcc.c_str(),
        plmn_list[i].mnc.c_str());
    Logger::config().info("    TAC ...................: %d", plmn_list[i].tac);
    Logger::config().info("    Slice Support .........:");
    for (int j = 0; j < plmn_list[i].slice_list.size(); j++) {
      if (plmn_list[i].slice_list[j].get_sd_int() != SD_NO_VALUE) {
        Logger::config().info(
            "        SST, SD ...........: %d, %s",
            plmn_list[i].slice_list[j].sst, plmn_list[i].slice_list[j].sd);
      } else {
        Logger::config().info(
            "        SST ...............: %d", plmn_list[i].slice_list[j].sst);
      }
    }
  }

  Logger::config().info(
      "- Default DNN.................: %s", default_dnn.c_str());
  Logger::config().info(
      "- Emergency Support .......: %s", is_emergency_support ?
                                             AMF_CONFIG_OPTION_YES_STR :
                                             AMF_CONFIG_OPTION_NO_STR);

  if (!support_features.enable_external_ausf_udm) {
    Logger::config().info("- Database: ");
    Logger::config().info(
        "    MySQL Server Addr .....: %s", auth_para.mysql_server.c_str());
    Logger::config().info(
        "    MySQL user ............: %s", auth_para.mysql_user.c_str());
    Logger::config().info(
        "    MySQL pass ............: %s", auth_para.mysql_pass.c_str());
    Logger::config().info(
        "    MySQL DB ..............: %s", auth_para.mysql_db.c_str());
  }

  Logger::config().info("- N2 Networking:");
  Logger::config().info("    Iface .................: %s", n2.if_name.c_str());
  Logger::config().info("    IP Addr ...............: %s", inet_ntoa(n2.addr4));
  Logger::config().info("    Port ..................: %d", n2.port);

  Logger::config().info("- SBI Networking:");
  Logger::config().info("    Iface .................: %s", sbi.if_name.c_str());
  Logger::config().info(
      "    IP Addr ...............: %s", inet_ntoa(sbi.addr4));
  Logger::config().info("    Port ..................: %d", sbi.port);
  if (sbi.api_version.has_value())
    Logger::config().info(
        "    API version............: %s", sbi.api_version.value().c_str());

  if (!support_features.enable_smf_selection) {
    Logger::config().info("- SMF:");
    Logger::config().info(
        "    URI root ...............: %s", smf_addr.uri_root);
    Logger::config().info(
        "    API version ...........: %s", smf_addr.api_version.c_str());
  }

  if (support_features.enable_nf_registration) {
    Logger::config().info("- NRF:");
    Logger::config().info(
        "    URI root ...............: %s", nrf_addr.uri_root);
    Logger::config().info(
        "    API version ...........: %s", nrf_addr.api_version.c_str());
  }

  if (support_features.enable_nssf) {
    Logger::config().info("- NSSF:");
    Logger::config().info(
        "    URI root ...............: %s", nssf_addr.uri_root);
    Logger::config().info(
        "    API version ...........: %s", nssf_addr.api_version.c_str());
  }

  if (support_features.enable_external_ausf_udm) {
    // AUSF
    Logger::config().info("- AUSF:");
    Logger::config().info(
        "    URI root ...............: %s", ausf_addr.uri_root);
    Logger::config().info(
        "    API version ...........: %s", ausf_addr.api_version.c_str());
    // UDM
    Logger::config().info("- UDM:");
    Logger::config().info(
        "    URI root ...............: %s", udm_addr.uri_root);
    Logger::config().info(
        "    API version ...........: %s", udm_addr.api_version.c_str());
  }

  if (support_features.enable_lmf) {
    Logger::config().info("- LMF:");
    Logger::config().info(
        "    IP Addr ...............: %s", inet_ntoa(lmf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", lmf_addr.port);
    Logger::config().info(
        "    API version ...........: %s", lmf_addr.api_version.c_str());
  }

  Logger::config().info("- Supported Features:");
  Logger::config().info(
      "    NF Registration .......: %s",
      support_features.enable_nf_registration ? AMF_CONFIG_OPTION_YES_STR :
                                                AMF_CONFIG_OPTION_NO_STR);
  Logger::config().info(
      "    SMF Selection .........: %s", support_features.enable_smf_selection ?
                                             AMF_CONFIG_OPTION_YES_STR :
                                             AMF_CONFIG_OPTION_NO_STR);
  Logger::config().info(
      "    External AUSF .........: %s",
      support_features.enable_external_ausf_udm ? AMF_CONFIG_OPTION_YES_STR :
                                                  AMF_CONFIG_OPTION_NO_STR);
  Logger::config().info(
      "    External UDM ..........: %s",
      support_features.enable_external_ausf_udm ? AMF_CONFIG_OPTION_YES_STR :
                                                  AMF_CONFIG_OPTION_NO_STR);
  Logger::config().info(
      "    External NSSF .........: %s", support_features.enable_nssf ?
                                             AMF_CONFIG_OPTION_YES_STR :
                                             AMF_CONFIG_OPTION_NO_STR);
  Logger::config().info(
      "    External LMF ..........: %s",
      support_features.enable_lmf ? "Yes" : "No");

  Logger::config().info(
      "    HTTP version...........: %d", support_features.http_version);
  Logger::config().info(
      "- Log Level ...............: %s",
      spdlog::level::to_string_view(amf_log_level));

  Logger::config().info("- Supported NAS Algorithm: ");
  std::string supported_integrity_alg = {};
  for (const auto& i : nas_cfg.prefered_integrity_algorithm) {
    supported_integrity_alg.append(get_5g_ia_str(i)).append(" ");
  }
  Logger::config().info(
      "    Ordered Integrity Algorithm: %s", supported_integrity_alg);
  std::string supported_ciphering_alg = {};
  for (const auto& i : nas_cfg.prefered_ciphering_algorithm) {
    supported_ciphering_alg.append(get_5g_ea_str(i)).append(" ");
  }
  Logger::config().info(
      "    Ordered Ciphering Algorithm: %s", supported_ciphering_alg);
}

//------------------------------------------------------------------------------
void amf_config::to_json(nlohmann::json& json_data) const {
  json_data["instance"]    = instance;
  json_data["log_level"]   = amf_log_level;
  json_data["amf_name"]    = amf_name;
  json_data["default_dnn"] = default_dnn;
  json_data["guami"]       = guami.to_json();
  json_data["guami_list"]  = nlohmann::json::array();
  for (auto s : guami_list) {
    json_data["guami_list"].push_back(s.to_json());
  }

  json_data["relative_amf_capacity"] = relative_amf_capacity;

  json_data["plmn_list"] = nlohmann::json::array();
  for (auto s : plmn_list) {
    json_data["plmn_list"].push_back(s.to_json());
  }
  json_data["is_emergency_support"] = is_emergency_support ?
                                          AMF_CONFIG_OPTION_YES_STR :
                                          AMF_CONFIG_OPTION_NO_STR;

  if (!support_features.enable_external_ausf_udm) {
    json_data["auth_para"] = auth_para.to_json();
  }

  json_data["n2"]  = n2.to_json();
  json_data["sbi"] = sbi.to_json();

  json_data["support_features_options"] = support_features.to_json();

  json_data["smf"] = smf_addr.to_json();

  if (support_features.enable_nf_registration) {
    json_data["nrf"] = nrf_addr.to_json();
  }

  if (support_features.enable_nssf) {
    json_data["nssf"] = nssf_addr.to_json();
  }

  if (support_features.enable_external_ausf_udm) {
    json_data["ausf"] = ausf_addr.to_json();
    json_data["udm"]  = udm_addr.to_json();
  }

  json_data["supported_nas_algorithms"] = nas_cfg.to_json();
  if (support_features.enable_lmf) {
    json_data["lmf"] = lmf_addr.to_json();
  }
}

//------------------------------------------------------------------------------
bool amf_config::from_json(nlohmann::json& json_data) {
  try {
    if (json_data.find("instance") != json_data.end()) {
      instance = json_data["instance"].get<int>();
    }

    if (json_data.find("pid_dir") != json_data.end()) {
      pid_dir = json_data["pid_dir"].get<std::string>();
    }
    if (json_data.find("amf_name") != json_data.end()) {
      amf_name = json_data["amf_name"].get<std::string>();
    }
    if (json_data.find("default_dnn") != json_data.end()) {
      amf_name = json_data["default_dnn"].get<std::string>();
    }
    if (json_data.find("log_level") != json_data.end()) {
      amf_log_level = json_data["log_level"].get<spdlog::level::level_enum>();
    }
    if (json_data.find("guami") != json_data.end()) {
      guami.from_json(json_data["guami"]);
    }

    if (json_data.find("guami_list") != json_data.end()) {
      guami_list.clear();
      for (auto s : json_data["guami_list"]) {
        guami_full_format_t g = {};
        g.from_json(s);
        guami_list.push_back(g);
      }
    }

    if (json_data.find("relative_amf_capacity") != json_data.end()) {
      relative_amf_capacity = json_data["relative_amf_capacity"].get<int>();
    }

    if (json_data.find("plmn_list") != json_data.end()) {
      plmn_list.clear();
      for (auto s : json_data["plmn_list"]) {
        plmn_item_t p = {};
        p.from_json(s);
        plmn_list.push_back(p);
      }
    }

    if (json_data.find("is_emergency_support") != json_data.end()) {
      is_emergency_support = json_data["is_emergency_support"].get<bool>();
    }

    if (json_data.find("auth_para") != json_data.end()) {
      auth_para.from_json(json_data["auth_para"]);
    }

    if (json_data.find("n2") != json_data.end()) {
      n2.from_json(json_data["n2"]);
    }
    if (json_data.find("sbi") != json_data.end()) {
      sbi.from_json(json_data["sbi"]);
    }

    if (json_data.find("support_features_options") != json_data.end()) {
      support_features.from_json(json_data["support_features_options"]);
    }

    if (json_data.find("smf") != json_data.end()) {
      smf_addr.from_json(json_data["smf"]);
    }

    if (support_features.enable_nf_registration) {
      if (json_data.find("nrf") != json_data.end()) {
        nrf_addr.from_json(json_data["nrf"]);
      }
    }

    if (support_features.enable_nssf) {
      if (json_data.find("nssf") != json_data.end()) {
        nssf_addr.from_json(json_data["nssf"]);
      }
    }

    if (support_features.enable_external_ausf_udm) {
      if (json_data.find("ausf") != json_data.end()) {
        ausf_addr.from_json(json_data["ausf"]);
      }
      if (json_data.find("udm") != json_data.end()) {
        udm_addr.from_json(json_data["udm"]);
      }
    }

    if (support_features.enable_lmf) {
      if (json_data.find("lmf") != json_data.end()) {
        lmf_addr.from_json(json_data["lmf"]);
      }
    }

  } catch (nlohmann::detail::exception& e) {
    Logger::amf_app().error(
        "Exception when reading configuration from json %s", e.what());
    return false;
  } catch (std::exception& e) {
    Logger::amf_app().error(
        "Exception when reading configuration from json %s", e.what());
    return false;
  }

  return true;
}

}  // namespace oai::config
