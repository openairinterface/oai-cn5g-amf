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

#ifndef FILE_3GPP_24_007_SEEN
#define FILE_3GPP_24_007_SEEN

#include <stdint.h>

constexpr uint8_t kType1IeSize = 1;

// Extended Protocol Discriminator (EPD)
// TODO: replaced by emum
#define EPD_5GS_MM_MSG 0b01111110
#define EPD_5GS_SM_MSG 0b00101110

// Extended Protocol Discriminator (EPD)
enum class EPDEnum {
  EPD_RESERVED                     = 0b00001110,
  _5GS_SESSION_MANAGEMENT_MESSAGE  = 0b00101110,
  _5GS_MOBILITY_MANAGEMENT_MESSAGE = 0b01111110
};

#endif
