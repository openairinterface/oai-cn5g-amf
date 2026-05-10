/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
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
      .append(ie_to_string(2 * kStatisticsHalfIeLengthForUe, "IMSI/SUPI"))
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
        .append(ie_to_string(2 * kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append(ie_to_string(kStatisticsHalfIeLengthForUe, "-"))
        .append("|\n");
  } else {
    int i = 1;
    for (auto const& ue : ue_infos) {
      std::string cell_id_str = {};
      oai::utils::conv::int_to_string_hex(
          ue.second.cellId, cell_id_str, 9);  // 36 bits
      std::string ue_id =
          ue.second.imsi.empty() ? ue.second.supi : ue.second.imsi;

      std::string plmn = ue.second.mcc + "," + ue.second.mnc;
      out.append(inner_indent)
          .append(
              ie_to_string(kStatisticsHalfIndexColLength, std::to_string(i)))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              nas_context::fivegmm_state_to_string(ue.second.register_status)))
          .append(ie_to_string(2 * kStatisticsHalfIeLengthForUe, ue_id))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, ue.second.guti))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string_full_format(ue.second.ranid)))
          .append(ie_to_string(
              kStatisticsHalfIeLengthForUe,
              amf_conv::uint32_to_hex_string_full_format(ue.second.amfid)))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, plmn))
          .append(ie_to_string(kStatisticsHalfIeLengthForUe, cell_id_str))
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
std::string statistics::get_paging_info() const {
  std::string out          = {};
  std::string inner_indent = fmt::format("{:<{}}", "", kStatisticsIndent);
  uint8_t number_cols      = 2;
  uint8_t header_length    = kStatisticsHalfIndexColLength * 2 +
                          kStatisticsHalfIeLengthForGnb * 2 * number_cols +
                          number_cols;

  size_t paging_depth                = 0;
  size_t awaiting_registration_depth = 0;
  size_t temporary_unreachable_depth = 0;
  {
    std::shared_lock lock(m_paging_queue_depths);
    for (const auto& [supi, depth] : paging_queue_depths) {
      paging_depth += depth.paging_depth;
      awaiting_registration_depth += depth.awaiting_registration_depth;
      temporary_unreachable_depth += depth.temporary_unreachable_depth;
    }
  }

  auto append_counter_row = [&](const std::string& name, uint64_t value) {
    out.append(inner_indent)
        .append(ie_to_string(kStatisticsHalfIndexColLength, name))
        .append(ie_to_string(
            kStatisticsHalfIeLengthForGnb * 2, std::to_string(value)))
        .append(ie_to_string(
            kStatisticsHalfIeLengthForGnb * 2 - kStatisticsHalfIndexColLength,
            ""))
        .append("|\n");
  };

  out.append("\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");
  out.append(inner_indent)
      .append(header_to_string(header_length, "Paging Metrics"))
      .append("|\n");
  out.append(inner_indent)
      .append(ie_to_string(kStatisticsHalfIndexColLength, "Metric"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, "Value"))
      .append(ie_to_string(kStatisticsHalfIeLengthForGnb, ""))
      .append("|\n");

  append_counter_row(
      "requests",
      paging_metrics.requests_total.load(std::memory_order_relaxed));
  append_counter_row(
      "direct",
      paging_metrics.direct_deliveries_total.load(std::memory_order_relaxed));
  append_counter_row(
      "cycles", paging_metrics.cycles_total.load(std::memory_order_relaxed));
  append_counter_row(
      "retries", paging_metrics.retries_total.load(std::memory_order_relaxed));
  append_counter_row(
      "successes",
      paging_metrics.successes_total.load(std::memory_order_relaxed));
  append_counter_row(
      "timeouts",
      paging_metrics.timeouts_total.load(std::memory_order_relaxed));
  append_counter_row(
      "rejections",
      paging_metrics.rejections_total.load(std::memory_order_relaxed));
  append_counter_row(
      "defer-reg", paging_metrics.deferred_registration_total.load(
                       std::memory_order_relaxed));
  append_counter_row(
      "defer-temp", paging_metrics.deferred_temporary_unreachable_total.load(
                        std::memory_order_relaxed));
  append_counter_row(
      "no-target",
      paging_metrics.no_target_total.load(std::memory_order_relaxed));
  append_counter_row(
      "page-qfull",
      paging_metrics.paging_queue_full_total.load(std::memory_order_relaxed));
  append_counter_row(
      "await-qfull", paging_metrics.awaiting_registration_queue_full_total.load(
                         std::memory_order_relaxed));
  append_counter_row(
      "temp-qfull", paging_metrics.temporary_unreachable_queue_full_total.load(
                        std::memory_order_relaxed));
  append_counter_row(
      "cb-ok",
      paging_metrics.callback_success_total.load(std::memory_order_relaxed));
  append_counter_row(
      "cb-fail",
      paging_metrics.callback_failures_total.load(std::memory_order_relaxed));
  // ITTI send-failure counter added here alongside the other paging_* counters
  append_counter_row(
      "send-fail",
      paging_metrics.send_failed_total.load(std::memory_order_relaxed));
  // TS 29.518 §6.1.5.6, N1N2TransferFailureNotification outcome counters.
  append_counter_row(
      "fn-sent",
      paging_metrics.failure_notify_sent_total.load(std::memory_order_relaxed));
  append_counter_row(
      "fn-fail", paging_metrics.failure_notify_failed_total.load(
                     std::memory_order_relaxed));
  // Dropped notifications (in-flight cap reached)
  append_counter_row(
      "fn-drop", paging_metrics.failure_notify_dropped_total.load(
                     std::memory_order_relaxed));
  append_counter_row("queue", paging_depth);
  append_counter_row("await-reg", awaiting_registration_depth);
  append_counter_row("temp-unr", temporary_unreachable_depth);

  out.append(inner_indent)
      .append(header_to_string(header_length, ""))
      .append("|\n");
  return out;
}

//------------------------------------------------------------------------------
void statistics::update_ue_info(const ue_info_t& ue_info) {
  if (ue_info.imsi.empty() && ue_info.supi.empty()) {
    Logger::amf_app().debug("Update UE Info with invalid IMSI/SUPI");
    return;
  }

  std::string ue_id = {};
  if (ue_info.imsi.empty()) {
    Logger::amf_app().debug(
        "Update UE Info with empty IMSI, using SUPI (%s) instead",
        ue_info.supi.c_str());
    ue_id = ue_info.supi;
  } else {
    ue_id = ue_info.imsi;
  }

  std::unique_lock lock(m_ue_infos);
  if (!ue_info.imsi.empty() && !ue_info.supi.empty()) {
    // Remove old SUPI entry if exists
    if (ue_infos.count(ue_info.supi) > 0) ue_infos.erase(ue_info.supi);
    Logger::amf_app().debug(
        "UE SUPI %s has been successfully erased!", ue_info.supi.c_str());
  }

  if (ue_infos.count(ue_id) > 0) {
    ue_infos.at(ue_id) = ue_info;
    Logger::amf_app().info(
        "The UE's Info (UE_ID %s) has been successfully updated!",
        ue_id.c_str());
  } else {
    ue_infos.emplace(std::make_pair(ue_id, ue_info));
    Logger::amf_app().info(
        "A new UE (UE_ID %s) has been successfully added!", ue_id.c_str());
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

//------------------------------------------------------------------------------
void statistics::increment_paging_requests() {
  paging_metrics.requests_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_direct_deliveries() {
  paging_metrics.direct_deliveries_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_cycles() {
  paging_metrics.cycles_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_retries() {
  paging_metrics.retries_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_successes() {
  paging_metrics.successes_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_timeouts() {
  paging_metrics.timeouts_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_rejections() {
  paging_metrics.rejections_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_deferred_registration() {
  paging_metrics.deferred_registration_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_deferred_temporary_unreachable() {
  paging_metrics.deferred_temporary_unreachable_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_no_target() {
  paging_metrics.no_target_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_queue_full() {
  paging_metrics.paging_queue_full_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_awaiting_registration_queue_full() {
  paging_metrics.awaiting_registration_queue_full_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_temporary_unreachable_queue_full() {
  paging_metrics.temporary_unreachable_queue_full_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_callback_successes() {
  paging_metrics.callback_success_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_callback_failures() {
  paging_metrics.callback_failures_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_send_failed() {
  paging_metrics.send_failed_total.fetch_add(1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
// TS 29.518 §6.1.5.6 N1N2TransferFailureNotification outcomes
void statistics::increment_paging_failure_notify_sent() {
  paging_metrics.failure_notify_sent_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::increment_paging_failure_notify_failed() {
  paging_metrics.failure_notify_failed_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
// Dropped notifications (in-flight cap reached)
void statistics::increment_paging_failure_notify_dropped() {
  paging_metrics.failure_notify_dropped_total.fetch_add(
      1, std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
void statistics::update_paging_queue_depths(
    const std::string& supi, size_t paging_depth,
    size_t awaiting_registration_depth, size_t temporary_unreachable_depth) {
  if (supi.empty()) {
    return;
  }

  std::unique_lock lock(m_paging_queue_depths);
  if ((paging_depth == 0) && (awaiting_registration_depth == 0) &&
      (temporary_unreachable_depth == 0)) {
    paging_queue_depths.erase(supi);
    return;
  }

  paging_queue_depths[supi] = paging_queue_depth_t{
      paging_depth, awaiting_registration_depth, temporary_unreachable_depth};
}
