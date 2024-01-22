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

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "amf_http1_server.hpp"
#include "amf_http2_server.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_config_yaml.hpp"
#include "amf_statistics.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "ngap_app.hpp"
#include "normalizer.hh"
#include "options.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

using namespace oai::config;
using namespace amf_application;

amf_config amf_cfg;
itti_mw* itti_inst    = nullptr;
amf_app* amf_app_inst = nullptr;
statistics stacs;

amf_http1_server* http1_server = nullptr;
amf_http2_server* http2_server = nullptr;

std::unique_ptr<amf_config_yaml> amf_cfg_yaml;

//------------------------------------------------------------------------------
void amf_signal_handler(int s) {
  std::cout << "Caught signal " << s << std::endl;
  Logger::system().startup("exiting");

  if (amf_app_inst) {
    amf_app_inst->stop();
  }

  if (itti_inst) {
    itti_inst->send_terminate_msg(TASK_AMF_APP);
    itti_inst->wait_tasks_end();
  }

  std::cout << "Freeing Allocated memory..." << std::endl;

  if (amf_app_inst) {
    delete amf_app_inst;
    amf_app_inst = nullptr;
    std::cout << "AMF APP memory done." << std::endl;
  }

  if (http1_server) {
    http1_server->shutdown();
    delete http1_server;
    http1_server = nullptr;
    std::cout << "HTTP/1 Server memory done." << std::endl;
  }

  if (http2_server) {
    http2_server->stop();
    delete http2_server;
    http2_server = nullptr;
    std::cout << "HTTP/2 Server memory done." << std::endl;
  }

  if (itti_inst) {
    delete itti_inst;
    itti_inst = nullptr;
    std::cout << "ITTI memory done." << std::endl;
  }

  std::cout << "Freeing Allocated memory done" << std::endl;
  exit(s);
}

//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  srand(time(NULL));

  if (!Options::parse(argc, argv)) {
    std::cout << "Options::parse() failed" << std::endl;
    return 1;
  }

  Logger::init("AMF", Options::getlogStdout(), Options::getlogRotFilelog());
  Logger::amf_app().startup("Options parsed!");

  std::signal(SIGTERM, amf_signal_handler);
  std::signal(SIGINT, amf_signal_handler);

  std::string conf_file_name = Options::getYamlConfig();
  Logger::system().debug("Parsing the configuration file, file type YAML.");
  amf_cfg_yaml = std::make_unique<amf_config_yaml>(
      conf_file_name, Options::getlogStdout(), Options::getlogRotFilelog());
  if (!amf_cfg_yaml->init()) {
    Logger::system().error("Reading the configuration failed. Exiting.");
    return 1;
  }
  amf_cfg_yaml->pre_process();
  amf_cfg_yaml->display();
  // Convert from YAML to internal structure
  amf_cfg_yaml->to_amf_config(amf_cfg);

  itti_inst = new itti_mw();
  itti_inst->start(amf_cfg.itti.itti_timer_sched_params);

  amf_app_inst = new amf_app(amf_cfg);

  Logger::amf_app().debug("Initiating AMF server endpoints");
  if (!amf_cfg.support_features.use_http2) {
    // AMF HTTP1 server
    Pistache::Address addr(
        std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.sbi.addr4))),
        Pistache::Port(amf_cfg.sbi.port));
    http1_server = new amf_http1_server(addr, amf_app_inst);
    http1_server->init(2);
    std::thread amf_http1_manager(&amf_http1_server::start, http1_server);
    amf_http1_manager.join();
  } else {
    // AMF HTTP2 server
    http2_server = new amf_http2_server(
        conv::toString(amf_cfg.sbi.addr4), amf_cfg.sbi.port, amf_app_inst);
    std::thread amf_http2_manager(&amf_http2_server::start, http2_server);
    amf_http2_manager.join();
  }

  Logger::amf_app().debug("Initiation Done!");
  pause();
  return 0;
}
