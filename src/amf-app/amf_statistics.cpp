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
#include <cstdio>
#include <string>
#include "logger.hpp"
#include "amf_conversions.hpp"

//------------------------------------------------------------------------------
statistics::statistics() : m_ue_infos(), m_gnbs() {
  gNB_connected = 0;
  UE_connected  = 0;
  UE_registred  = 0;
}

//------------------------------------------------------------------------------
statistics::~statistics() {}

//------------------------------------------------------------------------------
void statistics::display() {
  display_gnbs();
  display_ues();
}

//------------------------------------------------------------------------------
std::string statistics::ie_to_string(
    uint8_t half_ie_len, const std::string& ie_str) const {
  std::string out;
  std::string ie_formatter = "{}{: <{}}{}";
  // Display only maximum half_ie_len*2 characters
  std::string input_str = ie_str;
  uint8_t len           = ie_str.length();
  if (len > (half_ie_len * 2)) input_str = ie_str.substr(0, half_ie_len * 2);
  out.append("|")
      .append(fmt::format(ie_formatter, "", "", half_ie_len - len / 2, ie_str))
      .append(
          fmt::format(ie_formatter, "", "", half_ie_len + len / 2 - len, ""));
  return out;
}

//------------------------------------------------------------------------------
std::string statistics::header_to_string(
    uint8_t header_len, const std::string& header_str) const {
  std::string out;
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
void statistics::display_gnbs() const {
  std::string out;
  std::string inner_indent = fmt::format("{:<{}}", "", kStatisticsIndent);
  uint8_t header_length    = 0;
  // List of gNBs
  uint8_t number_cols = 5;
  header_length =
      kStatisticsHalfIeLengthForGnb * 2 * number_cols + (number_cols - 1);
  out.append("\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  out.append(inner_indent)
      .append(header_to_string(header_length, "gNBs' Information"))
      .append("|\n");

  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Index"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Status"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Global Id"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "gNB Name"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "PLMN"))
      .append("|\n");

  // For testing only
  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "1"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Connected"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "0xe000"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "gNB-OAI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "001,01"))
      .append("|\n");

  // For testing only
  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "11"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Disconnected"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "0xe000"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "gNB-OAI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "001,01"))
      .append("|\n");

  if (gnbs.size() == 0) {
    out.append(inner_indent)
        .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "-"))
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
              ie_to_string(kStatisticsHalfIeLengthForGnb, std::to_string(i)))
          .append(
              ie_to_string(kStatisticsHalfIeLengthForGnb, gnb.second.status))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForGnb,
              amf_conv::uint32_to_hex_string(gnb.second.gnb_id)))
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

  Logger::amf_app().info(out);
}

//------------------------------------------------------------------------------
void statistics::display_ues() const {
  std::string out;
  std::string inner_indent = fmt::format("{:<{}}", "", kStatisticsIndent);
  uint8_t header_length    = 0;

  // List of UEs
  uint8_t number_cols = 8;
  header_length =
      kStatisticsHalfIeLengthForUe * 2 * number_cols + (number_cols - 1);
  out.append("\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  out.append(inner_indent)
      .append(header_to_string(header_length, "UEs' Information"))
      .append("|\n");

  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "Index"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "5GMM State"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "IMSI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "GUTI"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "RAN UE NGAP ID"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "AMF UE NGAP ID"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "PLMN"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "CELL ID"))
      .append("|\n");

  // For testing only
  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "1"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "5GMM-REGISTERED"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "001010000000004"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "1"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "4"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "001,01"))
      .append(ie_to_string(kStatisticsHalfIeLengthForUe, "0xe000"))
      .append("|\n");

  if (ue_infos.size() == 0) {
    out.append(inner_indent)
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
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
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe, ue.second.registerStatus))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, ue.second.imsi))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, ue.second.guti))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string(ue.second.ranid)))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string(ue.second.amfid)))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, plmn))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string(ue.second.cellId)))
          .append("|\n");
      i++;
    }
  }

  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");

  Logger::amf_app().info(out);
}

//------------------------------------------------------------------------------
void statistics::update_ue_info(const ue_info_t& ue_info) {
  if (!(ue_info.imsi.size() > 0)) {
    Logger::amf_app().warn("Update UE Info with invalid IMSI");
    return;
  }

  std::unique_lock lock(m_ue_infos);
  if (ue_infos.count(ue_info.imsi) > 0) {
    ue_infos.erase(ue_info.imsi);
    ue_infos.insert(std::pair<std::string, ue_info_t>(ue_info.imsi, ue_info));
    Logger::amf_app().debug(
        "Update UE Info (IMSI %s) success", ue_info.imsi.c_str());
  } else {
    ue_infos.insert(std::pair<std::string, ue_info_t>(ue_info.imsi, ue_info));
    Logger::amf_app().debug(
        "Add UE Info (IMSI %s) success", ue_info.imsi.c_str());
  }
}

//------------------------------------------------------------------------------
void statistics::update_5gmm_state(
    const std::string& imsi, const std::string& state) {
  std::unique_lock lock(m_ue_infos);
  if (ue_infos.count(imsi) > 0) {
    ue_info_t ue_info      = ue_infos.at(imsi);
    ue_info.registerStatus = state;
    ue_infos.erase(ue_info.imsi);
    ue_infos.insert(std::pair<std::string, ue_info_t>(imsi, ue_info));
    Logger::amf_app().debug(
        "Update UE State (IMSI %s, State %s) success", imsi.c_str(),
        state.c_str());
  } else {
    Logger::amf_app().warn(
        "Update UE State (IMSI %s), UE does not exist!", imsi.c_str());
  }
}

//------------------------------------------------------------------------------
void statistics::remove_gnb(const uint32_t& gnb_id) {
  std::unique_lock lock(m_gnbs);
  if (gnbs.count(gnb_id) > 0) {
    gnbs.erase(gnb_id);
    gNB_connected -= 1;
  }
}

//------------------------------------------------------------------------------
void statistics::add_gnb(const uint32_t& gnb_id, const gnb_infos& gnb) {
  std::unique_lock lock(m_gnbs);
  gnbs.insert(std::pair<uint32_t, gnb_infos>(gnb_id, gnb));
  gNB_connected += 1;
}

//------------------------------------------------------------------------------
void statistics::add_gnb(const std::shared_ptr<gnb_context>& gc) {
  gnb_infos gnb = {};
  gnb.gnb_id    = gc->gnb_id;
  gnb.mcc       = gc->plmn.mcc;
  gnb.mnc       = gc->plmn.mnc;
  gnb.gnb_name  = gc->gnb_name;
  for (auto i : gc->supported_ta_list) {
    gnb.plmn_list.push_back(i);
  }
  std::unique_lock lock(m_gnbs);
  gnbs.insert(std::pair<uint32_t, gnb_infos>(gc->gnb_id, gnb));
  gNB_connected += 1;
}

//------------------------------------------------------------------------------
void statistics::update_gnb(const uint32_t& gnb_id, const gnb_infos& gnb) {
  std::unique_lock lock(m_gnbs);
  if (gnbs.count(gnb_id) > 0) {
    gnbs[gnb_id] = gnb;
  }
}

//------------------------------------------------------------------------------
uint32_t statistics::get_number_connected_gnbs() const {
  std::shared_lock lock(m_gnbs);
  return gnbs.size();
}
