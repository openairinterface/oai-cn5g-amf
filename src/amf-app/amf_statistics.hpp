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

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <vector>

#include "amf.hpp"
#include "config.hpp"
#include "nas_context.hpp"
#include "ngap_app.hpp"

constexpr auto kStatisticGnbStatusConnected    = "Connected";
constexpr auto kStatisticGnbStatusDisconnected = "Disconnected";

typedef struct {
  uint32_t gnb_id;
  // TODO: list of PLMNs
  std::vector<SupportedTaItem_t> plmn_list;
  std::string mcc;
  std::string mnc;
  std::string gnb_name;
  std::string status;
  uint32_t tac;
  // long nrCellId;
  std::string plmn_to_string() const {
    std::string s = {};
    for (auto supported_item : plmn_list) {
      s.append("TAC " + std::to_string(supported_item.tac));
      for (auto plmn_slice : supported_item.plmnSliceSupportList) {
        s.append("( MCC " + plmn_slice.mcc);
        s.append(", MNC " + plmn_slice.mnc);
        for (auto slice : plmn_slice.sliceList) {
          s.append("(SST " + slice.sst + ", SD " + slice.sd + "),");
        }
        s.append(")");
      }
      s.append("),");
    }
    return s;
  }
} gnb_infos;

typedef struct ue_info_s {
  cm_state_t cm_status;
  _5gmm_state_t register_status;
  uint32_t ranid;
  long amfid;
  std::string imsi;
  std::string guti;
  std::string mcc;
  std::string mnc;
  uint32_t cellId;
} ue_info_t;

constexpr uint8_t kStatisticsIndent             = 3;
constexpr uint8_t kStatisticsHalfIndexColLength = 4;
constexpr uint8_t kStatisticsHalfIeLengthForGnb = 18;
constexpr uint8_t kStatisticsHalfIeLengthForUe  = 10;

class statistics {
 public:
  statistics();
  ~statistics();

  /*
   * Display the statistic information for gNB and UE
   * @param void
   * @return void
   */
  void display();

  /*
   * Get the statistic information for all gNBs in string format
   * @param void
   * @return std::string
   */
  std::string get_gnbs_info() const;

  /*
   * Get all the statistic information for all UEs in string format
   * @param void
   * @return std::string
   */
  std::string get_ues_info() const;

  /*
   * Represent column information in string format
   * @param [uint8_t] half_ie_len: half of the column's length
   * @param [const std::string&] ie_str: info in string format
   * @return void
   */
  std::string ie_to_string(
      uint8_t half_ie_len, const std::string& ie_str) const;

  /*
   * Represent table header in string format
   * @param [uint8_t] header_len: the column's length
   * @param [const std::string&] str: info in string format
   * @return void
   */
  std::string header_to_string(
      uint8_t header_len, const std::string& str) const;

  /*
   * Update UE information
   * @param [const ue_info_t&] ue_info: UE information
   * @return void
   */
  void update_ue_info(const ue_info_t& ue_info);

  /*
   * Update UE 5GMM state
   * @param [std::shared_ptr<nas_context>&] nc: UE's NAS context
   * @param [const std::string&] state: UE State
   * @return void
   */
  void update_5gmm_state(
      const std::shared_ptr<nas_context>& nc, const _5gmm_state_t& state);

  /*
   * Remove gNB from the list connected gNB to this AMF
   * @param [const uint32_t] gnb_id: gNB ID
   * @return void
   */
  void remove_gnb(uint32_t gnb_id);

  /*
   * Add gNB to the list connected gNB to this AMF
   * @param [const std::shared_ptr<gnb_context> &] gc: pointer to gNB Context
   * @return void
   */
  void add_gnb(const std::shared_ptr<gnb_context>& gc);

  /*
   * Update gNB info
   * @param [const std::shared_ptr<gnb_context>] gc: gNB's context
   * @param [const std::string&] status: gNB's status
   * @return void
   */
  void update_gnb(
      const std::shared_ptr<gnb_context>& gc, const std::string& status);

  /*
   * Get number of connected gNBs
   * @param void
   * @return number of connected gNBs
   */
  uint32_t get_number_connected_gnbs() const;

 private:
  std::map<uint32_t, gnb_infos> gnbs;
  mutable std::shared_mutex m_gnbs;
  std::map<std::string, ue_info_t> ue_infos;
  mutable std::shared_mutex m_ue_infos;
};

#endif
