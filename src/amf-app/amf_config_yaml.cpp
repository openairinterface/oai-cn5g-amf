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

#include "amf_config_yaml.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"

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
        m_sd.get_value()));

  return out;
}

//------------------------------------------------------------------------------
int s_nssai::get_sst() const {
  return m_sst.get_value();
}

//------------------------------------------------------------------------------
bool s_nssai::get_sd(std::string& sd) const {
  if (m_sd.is_set()) {
    sd = m_sd.get_value();
  }
  return m_sd.is_set();
}

//------------------------------------------------------------------------------
std::string s_nssai::get_sd() const {
  if (m_sd.is_set()) {
    return m_sd.get_value();
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

    if (key == AMF_CONFIG_PID_DIRECTORY) {
      m_pid_directory.from_yaml(elem.second);
    }

    if (key == AMF_CONFIG_AMF_NAME) {
      m_amf_name.from_yaml(elem.second);
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

const local_interface& amf::get_n2() const {
  return m_n2;
}

//------------------------------------------------------------------------------
amf_config_yaml::amf_config_yaml(
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
      oai::config::AMF_CONFIG_NAME,       oai::config::DATABASE_CONFIG};

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
}

//------------------------------------------------------------------------------
amf_config_yaml::~amf_config_yaml() {}

void amf_config_yaml::pre_process() {
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
}

//------------------------------------------------------------------------------
void amf_config_yaml::to_amf_config(amf_config& cfg) {
  std::shared_ptr<amf> amf_local = std::static_pointer_cast<amf>(get_local());
  cfg.instance                   = amf_local->get_instance_id();
  cfg.pid_dir                    = amf_local->get_pid_directory();
  cfg.amf_name                   = amf_local->get_amf_name();
  cfg.log_level                  = spdlog::level::from_str(log_level());

  cfg.relative_amf_capacity = amf_local->get_relative_capacity();
  cfg.statistics_interval   = amf_local->get_statistics_timer_interval();
  cfg.http_request_timeout  = get_http_request_timeout();

  // Parse the "Super" option - "enable_simple_scenario"
  if (amf_local->get_support_features().get_option_enable_simple_scenario()) {
    cfg.support_features.enable_nf_registration   = false;
    cfg.support_features.enable_smf_selection     = false;
    cfg.support_features.enable_external_ausf_udm = false;
    cfg.support_features.enable_nssf = false;  // TODO: to be removed
  } else {                                     // parse the other options
    cfg.support_features.enable_nf_registration = register_nrf();
    cfg.support_features.enable_smf_selection =
        amf_local->get_support_features().get_option_enable_smf_selection();
    cfg.support_features.enable_external_ausf_udm = true;  // To be removed
    cfg.support_features.enable_nssf =
        amf_local->get_support_features().get_option_enable_nssf();
  }

  cfg.support_features.http_version = get_http_version();

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
    cfg.guami = guami_item;
    cfg.guami_list.push_back(guami_item);
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

    cfg.plmn_list.push_back(item);
  }

  // TODO: Emergency support
  cfg.is_emergency_support = false;
  // Database
  if (get_database_config().is_set()) {
    cfg.auth_para.mysql_server = get_database_config().get_host();
    cfg.auth_para.mysql_user   = get_database_config().get_user();
    cfg.auth_para.mysql_pass   = get_database_config().get_pass();
    cfg.auth_para.mysql_db     = get_database_config().get_database_name();
    // cfg.auth_para.connection_timeout =
    // get_database_config().get_connection_timeout(); cfg.database_type =
    // get_database_config().get_database_type();
  }

  cfg.sbi.api_version =
      std::make_optional<std::string>(local().get_sbi().get_api_version());
  cfg.sbi.port    = local().get_sbi().get_port();
  cfg.sbi.addr4   = local().get_sbi().get_addr4();
  cfg.sbi.if_name = local().get_sbi().get_if_name();

  cfg.n2.if_name = amf_local->get_n2().get_if_name();
  cfg.n2.addr4   = amf_local->get_n2().get_addr4();
  cfg.n2.port    = amf_local->get_n2().get_port();

  if (get_nf(oai::config::SMF_CONFIG_NAME)) {
    cfg.smf_addr.api_version =
        get_nf(oai::config::SMF_CONFIG_NAME)->get_sbi().get_api_version();
    cfg.smf_addr.uri_root = get_nf(oai::config::SMF_CONFIG_NAME)->get_url();
  }

  if (get_nf(oai::config::AUSF_CONFIG_NAME)) {
    cfg.ausf_addr.api_version =
        get_nf(oai::config::AUSF_CONFIG_NAME)->get_sbi().get_api_version();
    cfg.ausf_addr.uri_root = get_nf(oai::config::AUSF_CONFIG_NAME)->get_url();
  }

  if (get_nf(oai::config::UDM_CONFIG_NAME)) {
    cfg.udm_addr.api_version =
        get_nf(oai::config::UDM_CONFIG_NAME)->get_sbi().get_api_version();
    cfg.udm_addr.uri_root = get_nf(oai::config::UDM_CONFIG_NAME)->get_url();
  }

  if (get_nf(oai::config::NRF_CONFIG_NAME)) {
    cfg.nrf_addr.api_version =
        get_nf(oai::config::NRF_CONFIG_NAME)->get_sbi().get_api_version();
    cfg.nrf_addr.uri_root = get_nf(oai::config::NRF_CONFIG_NAME)->get_url();
  }

  if (get_nf(oai::config::NSSF_CONFIG_NAME)) {
    cfg.nssf_addr.api_version =
        get_nf(NSSF_CONFIG_NAME)->get_sbi().get_api_version();
    cfg.nssf_addr.uri_root = get_nf(NSSF_CONFIG_NAME)->get_sbi().get_url();
  }

  // NAS conf
  for (const auto& s : amf_local->get_supported_integrity_algorithms()) {
    if (!s.compare("NIA0")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA0);
    }
    if (!s.compare("NIA1")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA1);
    }
    if (!s.compare("NIA2")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA2);
    }
    if (!s.compare("NIA3")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA3);
    }
    if (!s.compare("NIA4")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA4);
    }
    if (!s.compare("NIA5")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA5);
    }
    if (!s.compare("NIA6")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA6);
    }
    if (!s.compare("NIA7")) {
      cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA7);
    }
  }

  // Default values
  if (amf_local->get_supported_integrity_algorithms().size() == 0) {
    cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA0);
    cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA1);
    cfg.nas_cfg.prefered_integrity_algorithm.push_back(_5g_ia_e::_5G_IA2);
  }

  for (const auto& s : amf_local->get_supported_encryption_algorithms()) {
    if (!s.compare("NEA0")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA0);
    }
    if (!s.compare("NEA1")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA1);
    }
    if (!s.compare("NEA2")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA2);
    }
    if (!s.compare("NEA3")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA3);
    }
    if (!s.compare("NEA4")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA4);
    }
    if (!s.compare("NEA5")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA5);
    }
    if (!s.compare("NEA6")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA6);
    }
    if (!s.compare("NEA7")) {
      cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA7);
    }
  }

  // Default values
  if (amf_local->get_supported_encryption_algorithms().size() == 0) {
    cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA0);
    cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA1);
    cfg.nas_cfg.prefered_ciphering_algorithm.push_back(_5g_ea_e::_5G_EA2);
  }
}
}  // namespace oai::config
