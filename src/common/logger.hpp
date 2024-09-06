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

#include "logger_base.hpp"

static const std::string ASYNC_CMD          = "async_cmd";
static const std::string AMF_APP            = "amf_app";
static const std::string CONFIG             = "config";
static const std::string SYSTEM             = "system";
static const std::string SCTP               = "sctp";
static const std::string NAS_MM             = "nas_mm";
static const std::string NGAP               = "ngap";
static const std::string ITTI               = "itti";
static const std::string AMF_N2             = "amf_n2";
static const std::string AMF_N1             = "amf_n1";
static const std::string AMF_SBI            = "amf_sbi";
static const std::string AMF_SERVER_LOG     = "amf_server";
static const std::string AMF_AUTHENTICATION = "amf_authentication";

class Logger : public oai::logger::logger_common {
 public:
  static void init(
      const std::string& name, const bool log_stdout, const bool log_rot_file) {
    oai::logger::logger_common(name, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, ASYNC_CMD, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_APP, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, CONFIG, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, SYSTEM, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, SCTP, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, NAS_MM, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, NGAP, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, ITTI, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_N2, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_N1, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_SBI, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_SERVER_LOG, log_stdout, log_rot_file);
    oai::logger::logger_registry::register_logger(
        name, AMF_AUTHENTICATION, log_stdout, log_rot_file);
  }
  static void set_level(spdlog::level::level_enum level) {
    oai::logger::logger_registry::set_level(level);
  }

  static void set_lttng(bool isLttngActive) {
    oai::logger::logger_registry::set_lttng_is_active(isLttngActive);
  }

  static bool should_log(spdlog::level::level_enum level) {
    return oai::logger::logger_registry::should_log(level);
  }

  static const oai::logger::printf_logger& async_cmd() {
    return oai::logger::logger_registry::get_logger(ASYNC_CMD);
  }
  static const oai::logger::printf_logger& amf_app() {
    return oai::logger::logger_registry::get_logger(AMF_APP);
  }
  static const oai::logger::printf_logger& config() {
    return oai::logger::logger_registry::get_logger(CONFIG);
  }
  static const oai::logger::printf_logger& system() {
    return oai::logger::logger_registry::get_logger(SYSTEM);
  }
  static const oai::logger::printf_logger& sctp() {
    return oai::logger::logger_registry::get_logger(SCTP);
  }
  static const oai::logger::printf_logger& nas_mm() {
    return oai::logger::logger_registry::get_logger(NAS_MM);
  }
  static const oai::logger::printf_logger& ngap() {
    return oai::logger::logger_registry::get_logger(NGAP);
  }
  static const oai::logger::printf_logger& itti() {
    return oai::logger::logger_registry::get_logger(ITTI);
  }
  static const oai::logger::printf_logger& amf_n2() {
    return oai::logger::logger_registry::get_logger(AMF_N2);
  }
  static const oai::logger::printf_logger& amf_n1() {
    return oai::logger::logger_registry::get_logger(AMF_N1);
  }
  static const oai::logger::printf_logger& amf_sbi() {
    return oai::logger::logger_registry::get_logger(AMF_SBI);
  }
  static const oai::logger::printf_logger& amf_server() {
    return oai::logger::logger_registry::get_logger(AMF_SERVER_LOG);
  }
  static const oai::logger::printf_logger& authentication() {
    return oai::logger::logger_registry::get_logger(AMF_AUTHENTICATION);
  }
};
