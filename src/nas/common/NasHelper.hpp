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

#pragma once

#include "logger.hpp"

namespace nas {

class NasHelper {
 public:
  template<typename T>
  static int Encode(
      std::optional<T>& ie, uint8_t*& buf, int& len,
      int& encoded_size) noexcept {
    if (!ie.has_value()) {
      Logger::nas_mm().debug("IE %s is not available", T::GetIeName().c_str());
      return KEncodeDecodeOK;
    } else {
      int encoded_ie_size =
          ie.value().Encode(buf + encoded_size, len - encoded_size);
      if (encoded_ie_size != KEncodeDecodeError) {
        encoded_size += encoded_ie_size;
        return KEncodeDecodeOK;
      } else {
        Logger::nas_mm().error("Encoding %s error", T::GetIeName().c_str());
        return KEncodeDecodeError;
      }
    }
  }

  template<typename T>
  static int Encode(
      T& ie, uint8_t*& buf, int& len, int& encoded_size) noexcept {
    int encoded_ie_size = ie.Encode(buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
      return KEncodeDecodeOK;
    } else {
      Logger::nas_mm().error("Encoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  template<typename T>
  static int Decode(
      std::optional<T>& ie, uint8_t*& buf, int& len, int& decoded_size,
      bool iei) noexcept {
    T ie_tmp = {};
    int decoded_result =
        ie_tmp.Decode(buf + decoded_size, len - decoded_size, iei);
    if (decoded_result == KEncodeDecodeError) {
      Logger::nas_mm().error("Decoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
    decoded_size += decoded_result;
    ie = std::optional<T>(ie_tmp);
    return KEncodeDecodeOK;
  }

  template<typename T>
  static int Decode(
      std::optional<T>& ie, uint8_t iei_value, uint8_t*& buf, int& len,
      int& decoded_size, bool iei) noexcept {
    T ie_tmp(iei_value);
    int decoded_result =
        ie_tmp.Decode(buf + decoded_size, len - decoded_size, iei);
    if (decoded_result == KEncodeDecodeError) {
      Logger::nas_mm().error("Decoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
    decoded_size += decoded_result;
    ie = std::optional<T>(ie_tmp);
    return KEncodeDecodeOK;
  }

  template<typename T>
  static int Decode(
      std::optional<T>& ie, uint8_t*& buf, int& len, int& decoded_size,
      const bool& high_pos, bool iei) noexcept {
    T ie_tmp = {};
    int decoded_result =
        ie_tmp.Decode(buf + decoded_size, len - decoded_size, high_pos, iei);
    if (decoded_result == KEncodeDecodeError) {
      Logger::nas_mm().error("Decoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
    decoded_size += decoded_result;
    ie = std::optional<T>(ie_tmp);
    return KEncodeDecodeOK;
  }

  template<typename T>
  static int Decode(
      T& ie, uint8_t*& buf, int& len, int& decoded_size, bool iei) noexcept {
    int decoded_result = ie.Decode(buf + decoded_size, len - decoded_size, iei);
    if (decoded_result == KEncodeDecodeError) {
      Logger::nas_mm().error("Decoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
    decoded_size += decoded_result;
    return KEncodeDecodeOK;
  }

  template<typename T>
  static int Decode(
      T& ie, uint8_t*& buf, int& len, int& decoded_size, const bool& high_pos,
      bool iei) noexcept {
    int decoded_result =
        ie.Decode(buf + decoded_size, len - decoded_size, high_pos, iei);
    if (decoded_result == KEncodeDecodeError) {
      Logger::nas_mm().error("Decoding %s error", T::GetIeName().c_str());
      return KEncodeDecodeError;
    }
    decoded_size += decoded_result;
    return KEncodeDecodeOK;
  }
};

}  // namespace nas
