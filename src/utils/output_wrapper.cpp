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

#include "output_wrapper.hpp"

#include "logger.hpp"

//------------------------------------------------------------------------------
void output_wrapper::print_buffer(
    const std::string app, const std::string sink, uint8_t* buf, int len) {
  if (!app.compare("amf_app")) Logger::amf_app().debug(sink.c_str());
  if (!app.compare("amf_n1")) Logger::amf_n1().debug(sink.c_str());
  if (!app.compare("amf_n2")) Logger::amf_n2().debug(sink.c_str());
  if (!app.compare("ngap")) Logger::ngap().debug(sink.c_str());
  if (!app.compare("amf_server")) Logger::amf_server().debug(sink.c_str());
  if (!app.compare("amf_sbi")) Logger::amf_sbi().debug(sink.c_str());
  if (Logger::should_log(spdlog::level::debug)) {
    for (int i = 0; i < len; i++) printf("%02x ", buf[i]);
    printf("\n");
  }
}

//------------------------------------------------------------------------------
void output_wrapper::print_asn_msg(
    const asn_TYPE_descriptor_t* td, const void* struct_ptr) {
  if (Logger::should_log(spdlog::level::debug))
    asn_fprint(stdout, td, struct_ptr);
}
