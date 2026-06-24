/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _GNB_CONTEXT_STORE_H_
#define _GNB_CONTEXT_STORE_H_

#include <map>
#include <memory>
#include <shared_mutex>
#include <vector>

#include "gNB_context.hpp"
#include "sctp_server.hpp"

using namespace sctp;

class gnb_context_store {
 public:
  // Find by SCTP association id. Returns nullptr if absent or null
  std::shared_ptr<gnb_context> find_by_assoc(
      const sctp_assoc_id_t& assoc_id) const {
    std::shared_lock lock(m_);
    auto it = by_assoc_.find(assoc_id);
    if (it != by_assoc_.end()) {
      return it->second;
    }
    return nullptr;
  }

  // Find by gnb_id. Returns nullptr if absent or null
  std::shared_ptr<gnb_context> find_by_gnbid(const long& gnb_id) const {
    std::shared_lock lock(m_);
    auto it = by_gnbid_.find(gnb_id);
    if (it != by_gnbid_.end()) {
      return it->second;
    }
    return nullptr;
  }

  // True if a non-null gNB context exists for this gnb_id
  bool exists_by_gnbid(const long& gnb_id) const {
    std::shared_lock lock(m_);
    auto it = by_gnbid_.find(gnb_id);
    return it != by_gnbid_.end() && it->second != nullptr;
  }

  // True if a non-null gNB context exists for this association id
  bool exists_by_assoc(const sctp_assoc_id_t& assoc_id) const {
    std::shared_lock lock(m_);
    auto it = by_assoc_.find(assoc_id);
    return it != by_assoc_.end() && it->second != nullptr;
  }

  // Insert/update the by-association-id index only
  void set_by_assoc(
      const sctp_assoc_id_t& assoc_id, const std::shared_ptr<gnb_context>& gc) {
    std::unique_lock lock(m_);
    by_assoc_[assoc_id] = gc;
  }

  // Insert/update the by-gnb_id index only
  void set_by_gnbid(
      const long& gnb_id, const std::shared_ptr<gnb_context>& gc) {
    std::unique_lock lock(m_);
    by_gnbid_[gnb_id] = gc;
  }

  // Copy all association id keys out under the lock
  std::vector<sctp_assoc_id_t> all_assoc_ids() const {
    std::vector<sctp_assoc_id_t> assoc_ids;
    std::shared_lock lock(m_);
    for (const auto& it : by_assoc_) {
      assoc_ids.push_back(it.first);
    }
    return assoc_ids;
  }

  // All-index removal
  bool remove(const std::shared_ptr<gnb_context>& gc) {
    if (!gc) {
      return false;
    }
    std::unique_lock lock(m_);
    by_gnbid_.erase(gc->gnb_id);
    by_assoc_.erase(gc->sctp_assoc_id);
    return true;
  }

 private:
  std::map<long, std::shared_ptr<gnb_context>> by_gnbid_;
  std::map<sctp_assoc_id_t, std::shared_ptr<gnb_context>> by_assoc_;
  mutable std::shared_mutex m_;
};

#endif
