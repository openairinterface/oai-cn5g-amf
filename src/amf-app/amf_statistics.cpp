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

#include "amf_statistics.hpp"

#include <string>

#include "amf_conversions.hpp"
#include "logger.hpp"

//------------------------------------------------------------------------------
statistics::statistics() : m_ue_infos(), m_gnbs() {}

//------------------------------------------------------------------------------
statistics::~statistics() {}

//------------------------------------------------------------------------------
void statistics::display() {
  std::string out = {};
  out.append(get_gnbs_info());
  out.append(get_ues_info());
  Logger::amf_app().info(out);
}

//------------------------------------------------------------------------------
std::string statistics::ie_to_string(
    uint8_t half_ie_len, const std::string& ie_str) const {
  std::string out          = {};
  std::string ie_formatter = "{}{: <{}}{}";
  // Display only maximum half_ie_len*2 characters
  std::string input_str = ie_str;
  uint8_t len           = ie_str.length();
  if (len > (half_ie_len * 2)) input_str = ie_str.substr(0, half_ie_len * 2);
  len = input_str.length();
  out.append("|")
      .append(
          fmt::format(ie_formatter, "", "", half_ie_len - len / 2, input_str))
      .append(
          fmt::format(ie_formatter, "", "", half_ie_len + len / 2 - len, ""));
  return out;
}

//------------------------------------------------------------------------------
std::string statistics::header_to_string(
    uint8_t header_len, const std::string& header_str) const {
  std::string out              = {};
  std::string header_formatter = "{}{:-<{}}{}";
  uint8_t half_header_len      = header_len / 2;
  // Display only maximum half_table_len*2 characters
  std::string input_str = header_str;
  uint8_t len           = header_str.length();
  if (len > (half_header_len * 2))
    input_str = header_str.substr(0, half_header_len * 2);
  out.append("|")
      .append(fmt::format(
          header_formatter, "", "", half_header_len - len / 2, header_str))
      .append(fmt::format(
          header_formatter, "", "", half_header_len + len / 2 - len, ""));
  if (header_len % 2 == 1) {
    std::string aligned_str = fmt::format("{:-<{}}", "", 1);
    out.append(aligned_str);
  }
  return out;
}

//------------------------------------------------------------------------------
std::string statistics::get_gnbs_info() const {
  std::string out          = {};
  std::string inner_indent = fmt::format("{:<{}}", "", kStatisticsIndent);
  uint8_t header_length    = 0;
  // List of gNBs
  uint8_t number_cols = 4;  // without column index
  header_length       = kStatisticsHalfIndexColLength * 2 +
                  kStatisticsHalfIeLengthForGnb * 2 * number_cols + number_cols;
  out.append("\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  out.append(inner_indent)
      .append(header_to_string(header_length, "gNBs' Information"))
      .append("|\n");

  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIndexColLength, "Index"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Status"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Global Id"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "gNB Name"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "PLMN"))
      .append("|\n");

  if (gnbs.size() == 0) {
    out.append(inner_indent)
        .append(ie_to_string(kStatisticsHalfIndexColLength, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "-"))
        .append("|\n");
  } else {
    int i = 1;
    for (auto const& gnb : gnbs) {
      std::string plmn = gnb.second.mcc + "," + gnb.second.mnc;
      out.append(inner_indent)
          .append(
              ie_to_string(kStatisticsHalfIndexColLength, std::to_string(i)))
          .append(
              ie_to_string(kStatisticsHalfIeLengthForGnb, gnb.second.status))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForGnb,
              amf_conv::uint32_to_hex_string_full_format(gnb.second.gnb_id)))
          .append(
              ie_to_string(kStatisticsHalfIeLengthForGnb, gnb.second.gnb_name))
          .append(ie_to_string(kStatisticsHalfIeLengthForGnb, plmn))
          .append("|\n");
      i++;
    }
  }

  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  return out;
}

//------------------------------------------------------------------------------
std::string statistics::get_ues_info() const {
  std::string out          = {};
  std::string inner_indent = fmt::format("{:<{}}", "", kStatisticsIndent);
  uint8_t header_length    = 0;

  // List of UEs
  uint8_t number_cols = 7;
  header_length       = kStatisticsHalfIndexColLength * 2 +
                  kStatisticsHalfIeLengthForUe * 2 * number_cols + number_cols;
  out.append("\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  out.append(inner_indent)
      .append(header_to_string(header_length, "UEs' Information"))
      .append("|\n");

  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIndexColLength, "Index"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "5GMM State"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "IMSI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "GUTI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "RAN UE NGAP ID"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "AMF UE NGAP ID"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "PLMN"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "Cell Id"))
      .append("|\n");

  if (ue_infos.size() == 0) {
    out.append(inner_indent)
        .append(ie_to_string(kStatisticsHalfIndexColLength, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append("|\n");
  } else {
    int i = 1;
    for (auto const& ue : ue_infos) {
      std::string plmn = ue.second.mcc + "," + ue.second.mnc;
      out.append(inner_indent)
          .append(
              ie_to_string(kStatisticsHalfIndexColLength, std::to_string(i)))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              nas_context::fivegmm_state_to_string(ue.second.register_status)))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, ue.second.imsi))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, ue.second.guti))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string_full_format(ue.second.ranid)))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string_full_format(ue.second.amfid)))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, plmn))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string_full_format(ue.second.cellId)))
          .append("|\n");
      i++;
    }
  }

  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  return out;
}

//------------------------------------------------------------------------------
void statistics::update_ue_info(const ue_info_t& ue_info) {
  if (!(ue_info.imsi.size() > 0)) {
    Logger::amf_app().warn("Update UE Info with invalid IMSI");
    return;
  }

  std::unique_lock lock(m_ue_infos);
  if (ue_infos.count(ue_info.imsi) > 0) {
    ue_infos.at(ue_info.imsi) = ue_info;
    Logger::amf_app().debug(
        "The UE's Info (IMSI %s) has been successfully updated!",
        ue_info.imsi.c_str());
  } else {
    ue_infos.emplace(std::make_pair(ue_info.imsi, ue_info));
    Logger::amf_app().debug(
        "A new UE (IMSI %s) has been successfully added!",
        ue_info.imsi.c_str());
  }
}

//------------------------------------------------------------------------------
void statistics::update_5gmm_state(
    const std::shared_ptr<nas_context>& nc, const _5gmm_state_t& state) {
  if (!nc) return;
  std::unique_lock lock(m_ue_infos);
  if (ue_infos.count(nc->imsi) > 0) {
    ue_info_t ue_info       = ue_infos.at(nc->imsi);
    ue_info.register_status = state;
    ;
    if (nc->guti.has_value()) ue_info.guti = nc->guti.value();
    ue_infos.at(nc->imsi) = ue_info;
    Logger::amf_app().debug(
        "The UE's state (IMSI %s, State %s) has been successfully updated!",
        nc->imsi.c_str(), nas_context::fivegmm_state_to_string(state).c_str());
  } else {
    Logger::amf_app().warn(
        "Update UE State (IMSI %s), UE does not exist!", nc->imsi.c_str());
  }
}

//------------------------------------------------------------------------------
void statistics::remove_gnb(uint32_t gnb_id) {
  std::unique_lock lock(m_gnbs);
  if (gnbs.count(gnb_id) > 0) {
    gnbs.erase(gnb_id);
  }
}

//------------------------------------------------------------------------------
void statistics::add_gnb(const std::shared_ptr<gnb_context>& gc) {
  gnb_infos gnb = {};
  gnb.gnb_id    = gc->gnb_id;
  gnb.mcc       = gc->plmn.mcc;
  gnb.mnc       = gc->plmn.mnc;
  gnb.gnb_name  = gc->gnb_name;
  gnb.status    = kStatisticGnbStatusConnected;
  for (auto i : gc->supported_ta_list) {
    gnb.plmn_list.push_back(i);
  }
  std::unique_lock lock(m_gnbs);
  if (gnbs.count(gc->gnb_id) > 0) {
    gnbs.at(gc->gnb_id) = gnb;
    Logger::amf_app().debug("The gNB's info has been successfully updated!");
  } else {
    gnbs.emplace(std::make_pair(gc->gnb_id, gnb));
    Logger::amf_app().debug("A new gNB has been successfully added!");
  }
}

//------------------------------------------------------------------------------
void statistics::update_gnb(
    const std::shared_ptr<gnb_context>& gc, const std::string& status) {
  gnb_infos gnb = {};
  gnb.gnb_id    = gc->gnb_id;
  gnb.mcc       = gc->plmn.mcc;
  gnb.mnc       = gc->plmn.mnc;
  gnb.gnb_name  = gc->gnb_name;
  gnb.status    = status;
  for (auto i : gc->supported_ta_list) {
    gnb.plmn_list.push_back(i);
  }

  std::unique_lock lock(m_gnbs);
  if (gnbs.count(gc->gnb_id) > 0) {
    gnbs.at(gc->gnb_id) = gnb;
    Logger::amf_app().debug("The gNB's info has been successfully updated!");
  } else {
    gnbs.emplace(std::make_pair(gc->gnb_id, gnb));
    Logger::amf_app().debug("A new gNB has been successfully added!");
  }
}

//------------------------------------------------------------------------------
uint32_t statistics::get_number_connected_gnbs() const {
  std::shared_lock lock(m_gnbs);
  return gnbs.size();
}
