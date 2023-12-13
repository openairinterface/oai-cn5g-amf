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

#include "GlobalRanNodeId.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalRanNodeId::GlobalRanNodeId() {
  global_gnb_id_    = std::nullopt;
  global_ng_enb_id_ = std::nullopt;
  id_present_       = Ngap_GlobalRANNodeID_PR_NOTHING;
}

//------------------------------------------------------------------------------
GlobalRanNodeId::~GlobalRanNodeId() {}

//------------------------------------------------------------------------------
void GlobalRanNodeId::setChoiceOfRanNodeId(
    const Ngap_GlobalRANNodeID_PR& id_present) {
  id_present_ = id_present;
}

//------------------------------------------------------------------------------
Ngap_GlobalRANNodeID_PR GlobalRanNodeId::getChoiceOfRanNodeId() const {
  return id_present_;
}

//------------------------------------------------------------------------------
void GlobalRanNodeId::set(const GlobalgNBId& global_gnb_id) {
  global_gnb_id_ = std::optional<GlobalgNBId>{global_gnb_id};
  id_present_    = Ngap_GlobalRANNodeID_PR_globalGNB_ID;
}

//------------------------------------------------------------------------------
void GlobalRanNodeId::set(const GlobalNgENBId& global_ng_enb_id) {
  global_ng_enb_id_ = std::optional<GlobalNgENBId>{global_ng_enb_id};
  id_present_       = Ngap_GlobalRANNodeID_PR_globalNgENB_ID;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::get(GlobalgNBId& global_gnb_id) const {
  if (global_gnb_id_.has_value()) {
    global_gnb_id = global_gnb_id_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::get(GlobalNgENBId& global_ng_enb_id) const {
  if (global_ng_enb_id_.has_value()) {
    global_ng_enb_id = global_ng_enb_id_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::encode(Ngap_GlobalRANNodeID_t& globalRANNodeID) {
  globalRANNodeID.present = id_present_;

  switch (id_present_) {
    case Ngap_GlobalRANNodeID_PR_globalGNB_ID: {
      globalRANNodeID.choice.globalGNB_ID =
          (Ngap_GlobalGNB_ID_t*) calloc(1, sizeof(struct Ngap_GlobalGNB_ID));
      if (!globalRANNodeID.choice.globalGNB_ID) return false;
      if (!global_gnb_id_.value().encode(*globalRANNodeID.choice.globalGNB_ID))
        return false;
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalNgENB_ID: {
      // TODO:
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID: {
      // TODO:
      break;
    }
    default:
      Logger::ngap().warn("GlobalRanNodeId Present encode error!");
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool GlobalRanNodeId::decode(const Ngap_GlobalRANNodeID_t& globalRANNodeID) {
  id_present_ = globalRANNodeID.present;

  switch (id_present_) {
    case Ngap_GlobalRANNodeID_PR_globalGNB_ID: {
      GlobalgNBId tmp = {};
      if (!tmp.decode(*globalRANNodeID.choice.globalGNB_ID)) return false;
      global_gnb_id_ = std::optional<GlobalgNBId>(tmp);
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalNgENB_ID: {
      // TODO:
      Logger::ngap().warn("GlobalRANNodeID Present is globalNgENB!");
      break;
    }
    case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID: {
      // TODO:
      Logger::ngap().warn("GlobalRANNodeID Present is globalN3IWF!");
      break;
    }
    default:
      Logger::ngap().warn("GlobalRanNodeId Present decode error!");
      return false;
  }
  return true;
}
}  // namespace ngap
