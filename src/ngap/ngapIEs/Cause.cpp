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

#include "Cause.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
Cause::Cause() {
  m_CausePresent = Ngap_Cause_PR_NOTHING;
  m_CauseValue   = -1;
}

//------------------------------------------------------------------------------
Cause::~Cause() {}

//------------------------------------------------------------------------------
void Cause::setChoiceOfCause(const Ngap_Cause_PR& cause_present) {
  m_CausePresent = cause_present;
}

//------------------------------------------------------------------------------
void Cause::set(const long& causeValue) {
  m_CauseValue = causeValue;
}

//------------------------------------------------------------------------------
bool Cause::encode(Ngap_Cause_t& cause) const {
  cause.present = m_CausePresent;
  switch (m_CausePresent) {
    case Ngap_Cause_PR_radioNetwork: {
      cause.choice.radioNetwork = m_CauseValue;
      break;
    }
    case Ngap_Cause_PR_transport: {
      cause.choice.transport = m_CauseValue;
      break;
    }
    case Ngap_Cause_PR_nas: {
      cause.choice.nas = m_CauseValue;
      break;
    }
    case Ngap_Cause_PR_protocol: {
      cause.choice.protocol = m_CauseValue;
      break;
    }
    case Ngap_Cause_PR_misc: {
      cause.choice.misc = m_CauseValue;
      break;
    }
    default: {
      Logger::ngap().warn("Cause Present error!");
      return false;
      break;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool Cause::decode(const Ngap_Cause_t& cause) {
  m_CausePresent = cause.present;
  switch (m_CausePresent) {
    case Ngap_Cause_PR_radioNetwork: {
      m_CauseValue = cause.choice.radioNetwork;
    } break;
    case Ngap_Cause_PR_transport: {
      m_CauseValue = cause.choice.transport;
    } break;
    case Ngap_Cause_PR_nas: {
      m_CauseValue = cause.choice.nas;
    } break;
    case Ngap_Cause_PR_protocol: {
      m_CauseValue = cause.choice.protocol;
    } break;
    case Ngap_Cause_PR_misc: {
      m_CauseValue = cause.choice.misc;
    } break;
    default: {
      Logger::ngap().warn("Cause Present error!");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
Ngap_Cause_PR Cause::getChoiceOfCause() const {
  return m_CausePresent;
}

//------------------------------------------------------------------------------
long Cause::get() const {
  return m_CauseValue;
}

//------------------------------------------------------------------------------
void Cause::set(const long& value, const Ngap_Cause_PR& cause_present) {
  m_CauseValue   = value;
  m_CausePresent = cause_present;
}
}  // namespace ngap
