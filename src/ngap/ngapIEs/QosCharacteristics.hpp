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

#ifndef _QOS_CHARACTERISTICS_H_
#define _QOS_CHARACTERISTICS_H_

#include <optional>

#include "Dynamic5qiDescriptor.hpp"
#include "NonDynamic5qiDescriptor.hpp"

extern "C" {
#include "Ngap_QosCharacteristics.h"
}

namespace ngap {

class QosCharacteristics {
 public:
  QosCharacteristics();
  virtual ~QosCharacteristics();

  int QosCharacteristicsPresent();

  void set(const NonDynamic5qiDescriptor& nonDynamic5qiDescriptor);
  void get(
      std::optional<NonDynamic5qiDescriptor>& nonDynamic5qiDescriptor) const;

  void set(const Dynamic5qiDescriptor& dynamic5qiDescriptor);
  void get(std::optional<Dynamic5qiDescriptor>& dynamic5qiDescriptor) const;

  bool encode(Ngap_QosCharacteristics_t&) const;
  bool decode(const Ngap_QosCharacteristics_t&);

 private:
  std::optional<NonDynamic5qiDescriptor> m_NonDynamic5qiDescriptor;
  std::optional<Dynamic5qiDescriptor> m_Dynamic5qiDescriptor;
};
}  // namespace ngap

#endif
