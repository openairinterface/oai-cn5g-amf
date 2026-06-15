/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_STORE_H_
#define _UE_CONTEXT_STORE_H_

#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <vector>

#include "ue_context.hpp"

class ue_context_store {
 public:
  // Find by ue_context_key. Returns nullptr if absent
  std::shared_ptr<ue_context> find(const std::string& key) const {
    std::shared_lock lock(m_);
    auto it = by_key_.find(key);
    if (it != by_key_.end()) {
      return it->second;
    }
    return nullptr;
  }

  // Find by SUPI. Returns nullptr if absent
  std::shared_ptr<ue_context> find_by_supi(const std::string& supi) const {
    std::shared_lock lock(m_);
    auto it = by_supi_.find(supi);
    if (it != by_supi_.end()) {
      return it->second;
    }
    return nullptr;
  }

  // Insert/update the by-key index only
  void upsert(const std::string& key, const std::shared_ptr<ue_context>& uc) {
    std::unique_lock lock(m_);
    by_key_[key] = uc;
  }

  // Insert/update the by-SUPI index only
  void bind_supi(
      const std::string& supi, const std::shared_ptr<ue_context>& uc) {
    std::unique_lock lock(m_);
    by_supi_[supi] = uc;
  }

  std::shared_ptr<ue_context> get_or_create(const std::string& key) {
    std::unique_lock lock(m_);
    auto it = by_key_.find(key);
    if (it != by_key_.end()) {
      return it->second;
    }
    auto uc      = std::make_shared<ue_context>();
    by_key_[key] = uc;
    return uc;
  }

  std::vector<std::string> all_supis() const {
    std::shared_lock lock(m_);
    std::vector<std::string> supis;
    supis.reserve(by_supi_.size());
    for (const auto& kvp : by_supi_) {
      supis.push_back(kvp.first);
    }
    return supis;
  }

  // All-index removal
  bool remove(const std::string& key) {
    std::unique_lock lock(m_);
    auto it = by_key_.find(key);
    if (it == by_key_.end()) {
      return false;
    }
    std::shared_ptr<ue_context> uc = it->second;
    by_key_.erase(it);
    if (uc) {
      by_supi_.erase(uc->supi);
    }
    return true;
  }

 private:
  std::map<std::string, std::shared_ptr<ue_context>> by_key_;
  std::map<std::string, std::shared_ptr<ue_context>> by_supi_;
  mutable std::shared_mutex m_;
};

#endif
