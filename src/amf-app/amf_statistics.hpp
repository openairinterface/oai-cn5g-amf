/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <atomic>
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
  std::vector<SupportedTaItem> plmn_list;
  std::string mcc;
  std::string mnc;
  std::string gnb_name;
  std::string status;
  uint32_t tac;
  // long nrCellId;
  std::string plmn_to_string() const {
    std::string s = {};
    for (auto supported_item : plmn_list) {
      s.append("TAC " + std::to_string(supported_item.getTac().get()));
      for (auto plmn_slice : supported_item.getBroadcastPlmnList()) {
        s.append("( MCC " + plmn_slice.getPlmn().getMcc());
        s.append(", MNC " + plmn_slice.getPlmn().getMnc());
        for (auto slice : plmn_slice.getSNssai()) {
          s.append(
              "(SST " + slice.getSstStr() + ", SD " + slice.getSd() + "),");
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
  uint64_t amfid;
  std::string imsi;
  std::string guti;
  std::string supi;
  std::string mcc;
  std::string mnc;
  uint64_t cellId;
} ue_info_t;

constexpr uint8_t kStatisticsIndent             = 3;
constexpr uint8_t kStatisticsHalfIndexColLength = 4;
constexpr uint8_t kStatisticsHalfIeLengthForGnb = 18;
constexpr uint8_t kStatisticsHalfIeLengthForUe  = 10;

class statistics {
 public:
  struct paging_metrics_t {
    std::atomic<uint64_t> requests_total{0};
    std::atomic<uint64_t> direct_deliveries_total{0};
    std::atomic<uint64_t> cycles_total{0};
    std::atomic<uint64_t> retries_total{0};
    std::atomic<uint64_t> successes_total{0};
    std::atomic<uint64_t> timeouts_total{0};
    std::atomic<uint64_t> rejections_total{0};
    std::atomic<uint64_t> deferred_registration_total{0};
    std::atomic<uint64_t> deferred_temporary_unreachable_total{0};
    std::atomic<uint64_t> no_target_total{0};
    std::atomic<uint64_t> paging_queue_full_total{0};
    std::atomic<uint64_t> awaiting_registration_queue_full_total{0};
    std::atomic<uint64_t> temporary_unreachable_queue_full_total{0};
    std::atomic<uint64_t> callback_success_total{0};
    std::atomic<uint64_t> callback_failures_total{0};
    // TTI send failure on T3513/T3565 retransmit path
    std::atomic<uint64_t> send_failed_total{0};
    // TS 29.518 §6.1.5.6 N1N2TransferFailureNotification outcomes
    std::atomic<uint64_t> failure_notify_sent_total{0};
    std::atomic<uint64_t> failure_notify_failed_total{0};
    // Notifications dropped because the in-flight
    // detached-thread cap (kFailureNotifyMaxInflight=64) was reached.
    std::atomic<uint64_t> failure_notify_dropped_total{0};
  };

  statistics();
  virtual ~statistics();

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
  std::string get_paging_info() const;

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
  void increment_paging_requests();
  void increment_paging_direct_deliveries();
  void increment_paging_cycles();
  void increment_paging_retries();
  void increment_paging_successes();
  void increment_paging_timeouts();
  void increment_paging_rejections();
  void increment_paging_deferred_registration();
  void increment_paging_deferred_temporary_unreachable();
  void increment_paging_no_target();
  void increment_paging_queue_full();
  void increment_awaiting_registration_queue_full();
  void increment_temporary_unreachable_queue_full();
  void increment_paging_callback_successes();
  void increment_paging_callback_failures();
  // Count ITTI send failures on T3513/T3565 retransmit path
  void increment_paging_send_failed();
  // TS 29.518 §6.1.5.6 N1N2TransferFailureNotification outcomes
  void increment_paging_failure_notify_sent();
  void increment_paging_failure_notify_failed();
  // Dropped notifications (in-flight cap reached)
  void increment_paging_failure_notify_dropped();
  void update_paging_queue_depths(
      const std::string& supi, size_t paging_depth,
      size_t awaiting_registration_depth, size_t temporary_unreachable_depth);

 private:
  struct paging_queue_depth_t {
    size_t paging_depth                = 0;
    size_t awaiting_registration_depth = 0;
    size_t temporary_unreachable_depth = 0;
  };

  std::map<uint32_t, gnb_infos> gnbs;
  mutable std::shared_mutex m_gnbs;
  std::map<std::string, ue_info_t> ue_infos;
  mutable std::shared_mutex m_ue_infos;
  paging_metrics_t paging_metrics;
  std::map<std::string, paging_queue_depth_t> paging_queue_depths;
  mutable std::shared_mutex m_paging_queue_depths;
};

#endif
