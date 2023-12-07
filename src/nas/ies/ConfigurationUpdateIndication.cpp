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

#include "ConfigurationUpdateIndication.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ConfigurationUpdateIndication::ConfigurationUpdateIndication()
    : Type1NasIe(false), red_(false), ack_(false) {}

//------------------------------------------------------------------------------
ConfigurationUpdateIndication::ConfigurationUpdateIndication(bool red, bool ack)
    : Type1NasIe(false), red_(red), ack_(ack) {
  SetValue();
}

//------------------------------------------------------------------------------
ConfigurationUpdateIndication::~ConfigurationUpdateIndication() {}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::SetRed(bool red) {
  red_ = red;
  SetValue();
}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::GetRed(bool& red) const {
  red = red_;
}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::SetAck(bool ack) {
  ack_ = ack;
  SetValue();
}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::GetAck(bool& ack) const {
  ack = ack_;
}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::SetValue() {
  value_ = red_ ? 0x02 : 0x00;
  value_ = ack_ ? (value_ | 0x01) : (value_ | 0x00);
}

//------------------------------------------------------------------------------
void ConfigurationUpdateIndication::GetValue() {
  red_ = value_ & 0x02;
  ack_ = value_ & 0x01;
}

//------------------------------------------------------------------------------
int ConfigurationUpdateIndication::Encode(uint8_t* buf, int len) {
  SetValue();
  return Type1NasIe::Encode(buf, len);
}

//------------------------------------------------------------------------------
int ConfigurationUpdateIndication::Decode(uint8_t* buf, int len, bool is_iei) {
  int decoded_size = Type1NasIe::Decode(buf, len, is_iei);
  if (decoded_size == KEncodeDecodeError) return KEncodeDecodeError;
  GetValue();
  return decoded_size;
}
