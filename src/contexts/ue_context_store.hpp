/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_STORE_H_
#define _UE_CONTEXT_STORE_H_

#include <cassert>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <utility>
#include <vector>

#include "amf.hpp"
#include "ue_context.hpp"

class ue_context_store {
 public:
  using ran_gnb_key_t =
      std::pair<uint32_t, uint32_t>;  // <ran_ue_ngap_id, gnb_id>

  // ---- Primary index: by amf_ue_ngap_id -------------------------------------

  // Find the context by amf_ue_ngap_id. Returns nullptr if absent.
  std::shared_ptr<ue_context> find(uint64_t amf_ue_ngap_id) const {
    std::shared_lock lock(m_);
    auto it = by_amf_id_.find(amf_ue_ngap_id);
    if (it != by_amf_id_.end()) {
      return it->second;
    }
    return nullptr;
  }

  // Insert/update the index only.
  void upsert(uint64_t amf_ue_ngap_id, const std::shared_ptr<ue_context>& uc) {
    assert(amf_ue_ngap_id != INVALID_AMF_UE_NGAP_ID);
    if (amf_ue_ngap_id == INVALID_AMF_UE_NGAP_ID) {
      return;
    }
    std::unique_lock lock(m_);
    by_amf_id_[amf_ue_ngap_id] = uc;
  }

  // Find the context by amf_ue_ngap_id, creating a fresh ue_context if absent.
  std::shared_ptr<ue_context> get_or_create(uint64_t amf_ue_ngap_id) {
    assert(amf_ue_ngap_id != INVALID_AMF_UE_NGAP_ID);
    if (amf_ue_ngap_id == INVALID_AMF_UE_NGAP_ID) {
      return nullptr;
    }
    std::unique_lock lock(m_);
    auto it = by_amf_id_.find(amf_ue_ngap_id);
    if (it != by_amf_id_.end()) {
      return it->second;
    }
    auto uc                    = std::make_shared<ue_context>();
    by_amf_id_[amf_ue_ngap_id] = uc;
    return uc;
  }

  // ---- Secondary index: by SUPI ---------------------------------------------

  // Find by SUPI. Returns nullptr if absent or expired
  std::shared_ptr<ue_context> find_by_supi(const std::string& supi) const {
    std::unique_lock lock(m_);
    auto it = by_supi_.find(supi);
    if (it != by_supi_.end()) {
      if (auto uc = it->second.lock()) {
        return uc;
      }
      by_supi_.erase(it);
    }
    return nullptr;
  }

  // Insert/update the by-SUPI index only (stores a weak reference to the
  // owner).
  void bind_supi(
      const std::string& supi, const std::shared_ptr<ue_context>& uc) {
    std::unique_lock lock(m_);
    by_supi_[supi] = uc;
  }

  void unbind_supi(const std::string& supi) {
    std::unique_lock lock(m_);
    by_supi_.erase(supi);
  }

  // ---- Secondary index: by GUTI ---------------------------------------------

  // Find by GUTI. Returns nullptr if absent or expired
  std::shared_ptr<ue_context> find_by_guti(const std::string& guti) const {
    std::unique_lock lock(m_);
    auto it = by_guti_.find(guti);
    if (it != by_guti_.end()) {
      if (auto uc = it->second.lock()) {
        return uc;
      }
      by_guti_.erase(it);
    }
    return nullptr;
  }

  // Insert/update the by-GUTI index (stores a weak reference to the owner).
  // Erases the context's previously-bound GUTI (uc->guti) before setting the
  // new key, so a GUTI realloc cannot leave a stale entry pointing at this
  // context.
  void bind_guti(
      const std::string& guti, const std::shared_ptr<ue_context>& uc) {
    std::unique_lock lock(m_);
    if (uc && !uc->guti.empty() && uc->guti != guti) {
      by_guti_.erase(uc->guti);
    }
    by_guti_[guti] = uc;
  }

  void unbind_guti(const std::string& guti) {
    std::unique_lock lock(m_);
    by_guti_.erase(guti);
  }

  // ---- Secondary index: by <ran_ue_ngap_id, gnb_id> -------------------------

  // Find by <ran_ue_ngap_id, gnb_id>. Returns nullptr if absent or expired
  std::shared_ptr<ue_context> find_by_ran_gnb(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id) const {
    std::unique_lock lock(m_);
    auto it = by_ran_gnb_.find(ran_gnb_key_t{ran_ue_ngap_id, gnb_id});
    if (it != by_ran_gnb_.end()) {
      if (auto uc = it->second.lock()) {
        return uc;
      }
      by_ran_gnb_.erase(it);
    }
    return nullptr;
  }

  // Insert/update the by-<ran,gnb> index only (stores a weak reference).
  void bind_ran_gnb(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id,
      const std::shared_ptr<ue_context>& uc) {
    std::unique_lock lock(m_);
    by_ran_gnb_[ran_gnb_key_t{ran_ue_ngap_id, gnb_id}] = uc;
  }

  void unbind_ran_gnb(uint32_t ran_ue_ngap_id, uint32_t gnb_id) {
    std::unique_lock lock(m_);
    by_ran_gnb_.erase(ran_gnb_key_t{ran_ue_ngap_id, gnb_id});
  }

  // ---- Others ----------------------------------------------------------

  // Copy all live SUPI keys out under the lock. Expired weak entries (owner
  // destroyed) are skipped.
  std::vector<std::string> all_supis() const {
    std::shared_lock lock(m_);
    std::vector<std::string> supis;
    supis.reserve(by_supi_.size());
    for (const auto& kvp : by_supi_) {
      if (!kvp.second.expired()) {
        supis.push_back(kvp.first);
      }
    }
    return supis;
  }

  // Apply fn to every context. Null entries are skipped; the callback runs
  // while the shared lock is held, so it must not call back into the store.
  void for_each(
      const std::function<void(const std::shared_ptr<ue_context>&)>& fn) const {
    std::shared_lock lock(m_);
    for (const auto& kvp : by_amf_id_) {
      if (kvp.second) {
        fn(kvp.second);
      }
    }
  }

  // amf_ue_ngap_id reassignment: when the ue_context is updated with a new
  // amf_ue_ngap_id (GUTI re-reg / uplink-NAS GUTI), NOT for handover (where
  // amf_ue_ngap_id is stable).
  std::shared_ptr<ue_context> rekey(uint64_t old_id, uint64_t new_id) {
    std::unique_lock lock(m_);
    if (old_id == new_id) {
      auto same = by_amf_id_.find(old_id);
      return same != by_amf_id_.end() ? same->second : nullptr;
    }
    auto it = by_amf_id_.find(old_id);
    if (it == by_amf_id_.end()) {
      return nullptr;
    }
    std::shared_ptr<ue_context> uc = it->second;
    by_amf_id_.erase(it);
    by_amf_id_[new_id] = uc;
    return uc;
  }

  // Unified multi-index removal
  bool remove(uint64_t amf_ue_ngap_id) {
    std::unique_lock lock(m_);
    auto it = by_amf_id_.find(amf_ue_ngap_id);
    if (it == by_amf_id_.end()) {
      return false;
    }
    std::shared_ptr<ue_context> uc = it->second;
    by_amf_id_.erase(it);
    if (uc) {
      if (!uc->supi.empty()) {
        by_supi_.erase(uc->supi);
      }
      if (!uc->guti.empty()) {
        by_guti_.erase(uc->guti);
      }
      by_ran_gnb_.erase(ran_gnb_key_t{uc->ran_ue_ngap_id, uc->gnb_id});
    }
    return true;
  }

 private:
  // primary index, keyed by amf_ue_ngap_id.
  std::map<uint64_t, std::shared_ptr<ue_context>> by_amf_id_;
  // Secondary indexes hold weak references to the object; they expire
  // automatically when the owner entry is removed. Mutable so the const finders
  // can erase expired entries.
  mutable std::map<std::string, std::weak_ptr<ue_context>> by_supi_;
  mutable std::map<std::string, std::weak_ptr<ue_context>> by_guti_;
  mutable std::map<ran_gnb_key_t, std::weak_ptr<ue_context>> by_ran_gnb_;
  mutable std::shared_mutex m_;
};

#endif
