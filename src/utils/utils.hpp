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

#ifndef FILE_UTILS_HPP_SEEN
#define FILE_UTILS_HPP_SEEN

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <sstream>
#include <string>

constexpr uint8_t kMccMncLength           = 3;
constexpr uint32_t KFutureStatusTimeoutMs = 1000;

class utils {
 public:
  template<class T>
  static inline T fromString(const std::string& str) {
    std::istringstream is(str);
    T v;
    is >> v;
    return v;
  }

  // For NAS
  static int encodeMccMnc2Buffer(
      const std::string& mcc_str, const std::string& mnc_str, uint8_t* buf,
      int len);
  static int decodeMccMncFromBuffer(
      std::string& mcc_str, std::string& mnc_str, uint8_t* buf, int len);

  // For Boost
  template<typename T>
  static void wait_for_result(
      boost::shared_future<T>& f, std::optional<T>& result) {
    boost::future_status status;
    // wait for timeout or ready
    status = f.wait_for(boost::chrono::milliseconds(KFutureStatusTimeoutMs));
    if (status == boost::future_status::ready) {
      assert(f.is_ready());
      assert(f.has_value());
      assert(!f.has_exception());
      // return the result when available
      result = std::make_optional<T>(f.get());
      return;
    } else {
      result = std::nullopt;
      return;
    }
  }
};
#endif /* FILE_UTILS_HPP_SEEN */
