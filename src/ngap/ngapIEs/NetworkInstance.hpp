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

#ifndef _NETWORK_INSTANCE_H_
#define _NETWORK_INSTANCE_H_

extern "C" {
#include "Ngap_NetworkInstance.h"
}

namespace ngap {

class NetworkInstance {
 public:
  NetworkInstance();
  NetworkInstance(const long& networkInstance);
  virtual ~NetworkInstance();

  void set(const long& networkInstance);
  bool get(long& networkInstance) const;

  bool encode(Ngap_NetworkInstance_t&) const;
  bool decode(const Ngap_NetworkInstance_t&);

 private:
  long m_NetworkInstance;
};

}  // namespace ngap

#endif
