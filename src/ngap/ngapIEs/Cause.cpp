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
  cause_present_ = Ngap_Cause_PR_NOTHING;
  cause_value_   = -1;
}

//------------------------------------------------------------------------------
Cause::~Cause() {}

//------------------------------------------------------------------------------
void Cause::setChoiceOfCause(const Ngap_Cause_PR& cause_present) {
  cause_present_ = cause_present;
}

//------------------------------------------------------------------------------
void Cause::setValue(const long& cause_value) {
  cause_value_ = cause_value;
}

//------------------------------------------------------------------------------
bool Cause::encode(Ngap_Cause_t& cause) const {
  cause.present = cause_present_;
  switch (cause_present_) {
    case Ngap_Cause_PR_radioNetwork: {
      cause.choice.radioNetwork = cause_value_;
      break;
    }
    case Ngap_Cause_PR_transport: {
      cause.choice.transport = cause_value_;
      break;
    }
    case Ngap_Cause_PR_nas: {
      cause.choice.nas = cause_value_;
      break;
    }
    case Ngap_Cause_PR_protocol: {
      cause.choice.protocol = cause_value_;
      break;
    }
    case Ngap_Cause_PR_misc: {
      cause.choice.misc = cause_value_;
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
  cause_present_ = cause.present;
  switch (cause_present_) {
    case Ngap_Cause_PR_radioNetwork: {
      cause_value_ = cause.choice.radioNetwork;
    } break;
    case Ngap_Cause_PR_transport: {
      cause_value_ = cause.choice.transport;
    } break;
    case Ngap_Cause_PR_nas: {
      cause_value_ = cause.choice.nas;
    } break;
    case Ngap_Cause_PR_protocol: {
      cause_value_ = cause.choice.protocol;
    } break;
    case Ngap_Cause_PR_misc: {
      cause_value_ = cause.choice.misc;
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
  return cause_present_;
}

//------------------------------------------------------------------------------
long Cause::getValue() const {
  return cause_value_;
}

//------------------------------------------------------------------------------
void Cause::set(const long& value, const Ngap_Cause_PR& cause_present) {
  cause_value_   = value;
  cause_present_ = cause_present;
}
}  // namespace ngap
