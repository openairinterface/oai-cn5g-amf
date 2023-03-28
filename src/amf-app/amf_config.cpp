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

#include "amf_config.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <libconfig.h++>

#include "3gpp_29.502.h"
#include "3gpp_24.501.hpp"
#include "amf_app.hpp"
#include "conversions.hpp"
#include "fqdn.hpp"
#include "if.hpp"
#include "logger.hpp"
#include "string.hpp"
#include "thread_sched.hpp"

extern "C" {
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "common_defs.h"
}

using namespace libconfig;
using namespace amf_application;

namespace config {

//------------------------------------------------------------------------------
amf_config::amf_config() {
  nrf_addr.ipv4_addr.s_addr               = INADDR_ANY;
  nrf_addr.port                           = DEFAULT_HTTP1_PORT;
  nrf_addr.api_version                    = DEFAULT_SBI_API_VERSION;
  ausf_addr.ipv4_addr.s_addr              = INADDR_ANY;
  ausf_addr.port                          = DEFAULT_HTTP1_PORT;
  ausf_addr.api_version                   = DEFAULT_SBI_API_VERSION;
  udm_addr.ipv4_addr.s_addr               = INADDR_ANY;
  udm_addr.port                           = DEFAULT_HTTP1_PORT;
  udm_addr.api_version                    = DEFAULT_SBI_API_VERSION;
  nssf_addr.ipv4_addr.s_addr              = INADDR_ANY;
  nssf_addr.port                          = DEFAULT_HTTP1_PORT;
  nssf_addr.api_version                   = DEFAULT_SBI_API_VERSION;
  instance                                = 0;
  log_level                               = spdlog::level::debug;
  n2                                      = {};
  sbi                                     = {};
  sbi_api_version                         = DEFAULT_SBI_API_VERSION;
  sbi_http2_port                          = DEFAULT_HTTP2_PORT;
  statistics_interval                     = 0;
  guami                                   = {};
  guami_list                              = {};
  relative_amf_capacity                   = 0;
  plmn_list                               = {};
  auth_para                               = {};
  nas_cfg                                 = {};
  smf_pool                                = {};
  support_features.enable_nf_registration = false;
  support_features.enable_nrf_selection   = false;
  support_features.enable_external_nrf    = false;
  support_features.enable_smf_selection   = false;
  support_features.enable_external_ausf   = false;
  support_features.enable_external_udm    = false;
  support_features.enable_external_nssf   = false;
  support_features.use_fqdn_dns           = false;
  support_features.use_http2              = false;
  // TODO:
}

//------------------------------------------------------------------------------
amf_config::~amf_config() {}

//------------------------------------------------------------------------------
int amf_config::load(const std::string& config_file) {
  Logger::amf_app().debug(
      "\nLoad AMF system configuration file(%s)", config_file.c_str());
  Config cfg;
  unsigned char buf_in6_addr[sizeof(struct in6_addr)];

  // Config file
  try {
    cfg.readFile(config_file.c_str());
  } catch (const FileIOException& fioex) {
    Logger::amf_app().error(
        "I/O error while reading file %s - %s", config_file.c_str(),
        fioex.what());
    throw;
  } catch (const ParseException& pex) {
    Logger::amf_app().error(
        "Parse error at %s:%d - %s", pex.getFile(), pex.getLine(),
        pex.getError());
    throw;
  }

  const Setting& root = cfg.getRoot();

  // AMF config
  try {
    const Setting& amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Instance
  const Setting& amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_INSTANCE_ID, instance);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // Statistic Timer interval
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL, statistics_interval);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // PID Dir
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_PID_DIRECTORY, pid_dir);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // AMF Name
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_AMF_NAME, amf_name);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // Log Level
  try {
    std::string string_level;
    amf_cfg.lookupValue(AMF_CONFIG_STRING_LOG_LEVEL, string_level);
    log_level = spdlog::level::from_str(string_level);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // GUAMI
  try {
    const Setting& guami_cfg = amf_cfg[AMF_CONFIG_STRING_GUAMI];
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MCC, guami.mcc);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MNC, guami.mnc);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_RegionID, guami.regionID);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFSetID, guami.AmfSetID);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFPointer, guami.AmfPointer);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // GUAMI List
  try {
    const Setting& guami_list_cfg =
        amf_cfg[AMF_CONFIG_STRING_SERVED_GUAMI_LIST];
    int count = guami_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      guami_t guami;
      const Setting& guami_item = guami_list_cfg[i];
      guami_item.lookupValue(AMF_CONFIG_STRING_MCC, guami.mcc);
      guami_item.lookupValue(AMF_CONFIG_STRING_MNC, guami.mnc);
      guami_item.lookupValue(AMF_CONFIG_STRING_RegionID, guami.regionID);
      guami_item.lookupValue(AMF_CONFIG_STRING_AMFSetID, guami.AmfSetID);
      guami_item.lookupValue(AMF_CONFIG_STRING_AMFPointer, guami.AmfPointer);
      guami_list.push_back(guami);
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // AMF Capacity
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY, relative_amf_capacity);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // PLMN List
  try {
    const Setting& plmn_list_cfg = amf_cfg[AMF_CONFIG_STRING_PLMN_SUPPORT_LIST];
    int count                    = plmn_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      plmn_item_t plmn_item;
      const Setting& item = plmn_list_cfg[i];
      item.lookupValue(AMF_CONFIG_STRING_MCC, plmn_item.mcc);
      item.lookupValue(AMF_CONFIG_STRING_MNC, plmn_item.mnc);
      item.lookupValue(AMF_CONFIG_STRING_TAC, plmn_item.tac);
      const Setting& slice_list_cfg =
          plmn_list_cfg[i][AMF_CONFIG_STRING_SLICE_SUPPORT_LIST];
      int numOfSlice = slice_list_cfg.getLength();
      for (int j = 0; j < numOfSlice; j++) {
        slice_t slice;
        const Setting& slice_item = slice_list_cfg[j];
        unsigned int sst          = 0;
        std::string sd            = {};
        slice_item.lookupValue(AMF_CONFIG_STRING_SST, sst);
        slice.sst = sst;
        slice_item.lookupValue(AMF_CONFIG_STRING_SD, sd);
        slice.sd = SD_NO_VALUE;  // Default value
        try {
          conv::sd_string_to_int(sd, slice.sd);
        } catch (const std::exception& err) {
          Logger::amf_app().error("Invalid SST/SD");
        }

        plmn_item.slice_list.push_back(slice);
      }
      plmn_list.push_back(plmn_item);
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // Supported features
  try {
    const Setting& support_features_cfg =
        amf_cfg[AMF_CONFIG_STRING_SUPPORT_FEATURES];
    string opt;
    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_NF_REGISTRATION, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_nf_registration = true;
    } else {
      support_features.enable_nf_registration = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_NRF_SELECTION, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_nrf_selection = true;
    } else {
      support_features.enable_nrf_selection = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_NRF, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_external_nrf = true;
    } else {
      support_features.enable_external_nrf = false;
    }
    // TODO: should be removed
    support_features.enable_external_nrf = true;

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_smf_selection = true;
    } else {
      support_features.enable_smf_selection = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_external_ausf = true;
    } else {
      support_features.enable_external_ausf = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_external_udm = true;
    } else {
      support_features.enable_external_udm = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_NSSF, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.enable_external_nssf = true;
    } else {
      support_features.enable_external_nssf = false;
    }

    // TODO: should be removed
    support_features.enable_external_nssf =
        support_features.enable_nrf_selection;

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_FQDN_DNS, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.use_fqdn_dns = true;
    } else {
      support_features.use_fqdn_dns = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_HTTP2, opt);
    if (boost::iequals(opt, "yes")) {
      support_features.use_http2 = true;
    } else {
      support_features.use_http2 = false;
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Network interfaces
  // TODO: should move SMF/NRF/AUSF out of this section
  try {
    const Setting& new_if_cfg = amf_cfg[AMF_CONFIG_STRING_INTERFACES];

    // N2
    const Setting& n2_amf_cfg =
        new_if_cfg[AMF_CONFIG_STRING_INTERFACE_NGAP_AMF];
    load_interface(n2_amf_cfg, n2);

    // SBI
    const Setting& sbi_cfg = new_if_cfg[AMF_CONFIG_STRING_INTERFACE_SBI];
    load_interface(sbi_cfg, sbi);

    // SBI API VERSION
    if (!(sbi_cfg.lookupValue(
            AMF_CONFIG_STRING_API_VERSION, sbi_api_version))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
      throw(AMF_CONFIG_STRING_API_VERSION "failed");
    }

    // HTTP2 port
    if (!(sbi_cfg.lookupValue(
            AMF_CONFIG_STRING_SBI_HTTP2_PORT, sbi_http2_port))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
      throw(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
    }

    if (!support_features.enable_smf_selection) {
      // SMF
      const Setting& smf_addr_pool =
          sbi_cfg[AMF_CONFIG_STRING_SMF_INSTANCES_POOL];
      int count = smf_addr_pool.getLength();
      for (int i = 0; i < count; i++) {
        const Setting& smf_addr_item = smf_addr_pool[i];
        smf_inst_t smf_inst          = {};
        struct in_addr smf_ipv4_addr = {};
        unsigned int smf_port        = {};
        uint32_t smf_http2_port      = {};
        std::string smf_api_version  = {};
        std::string selected         = {};

        // Store FQDN
        smf_addr_item.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, smf_inst.fqdn);

        smf_addr_item.lookupValue(
            AMF_CONFIG_STRING_SMF_INSTANCE_ID, smf_inst.id);
        if (!support_features.use_fqdn_dns) {
          smf_addr_item.lookupValue(
              AMF_CONFIG_STRING_IPV4_ADDRESS, smf_inst.ipv4);
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(smf_inst.ipv4).c_str(), smf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR SMF !");
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SMF_INSTANCE_PORT, smf_inst.port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SMF_INSTANCE_PORT
                                    "failed");
            throw(AMF_CONFIG_STRING_SMF_INSTANCE_PORT "failed");
          }
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SBI_HTTP2_PORT, smf_inst.http2_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
            throw(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
          }
          smf_addr_item.lookupValue(
              AMF_CONFIG_STRING_SMF_INSTANCE_VERSION, smf_inst.version);
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SMF_INSTANCE_VERSION, smf_inst.version))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SMF_INSTANCE_VERSION
                                    "failed");
            throw(AMF_CONFIG_STRING_SMF_INSTANCE_VERSION "failed");
          }
        } else {
          smf_addr_item.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, smf_inst.fqdn);
        }

        smf_addr_item.lookupValue(
            AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED, selected);
        if (boost::iequals(selected, "true"))
          smf_inst.selected = true;
        else
          smf_inst.selected = false;
        smf_pool.push_back(smf_inst);
      }
    }

    // NRF
    const Setting& nrf_cfg       = new_if_cfg[AMF_CONFIG_STRING_NRF];
    struct in_addr nrf_ipv4_addr = {};
    unsigned int nrf_port        = 0;
    std::string nrf_api_version  = {};
    string address               = {};

    if (!support_features.use_fqdn_dns) {
      // Store FQDN
      nrf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, nrf_addr.fqdn);

      nrf_cfg.lookupValue(AMF_CONFIG_STRING_NRF_IPV4_ADDRESS, address);
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(address).c_str(), nrf_ipv4_addr,
          "BAD IPv4 ADDRESS FORMAT FOR NRF !");
      nrf_addr.ipv4_addr = nrf_ipv4_addr;
      if (!(nrf_cfg.lookupValue(AMF_CONFIG_STRING_NRF_PORT, nrf_port))) {
        Logger::amf_app().error(AMF_CONFIG_STRING_NRF_PORT "failed");
        throw(AMF_CONFIG_STRING_NRF_PORT "failed");
      }
      nrf_addr.port = nrf_port;
      if (!(nrf_cfg.lookupValue(
              AMF_CONFIG_STRING_API_VERSION, nrf_api_version))) {
        Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
        throw(AMF_CONFIG_STRING_API_VERSION "failed");
      }
      nrf_addr.api_version = nrf_api_version;
    } else {
      nrf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, nrf_addr.fqdn);
      uint8_t addr_type = {};
      fqdn::resolve(nrf_addr.fqdn, address, nrf_port, addr_type);
      if (addr_type != 0) {  // IPv6: TODO
        throw("DO NOT SUPPORT IPV6 ADDR FOR NRF!");
      } else {  // IPv4
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), nrf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR NRF !");
        nrf_addr.ipv4_addr = nrf_ipv4_addr;
        // nrf_addr.port        = nrf_port;

        // We hardcode nrf port from config for the moment
        if (!(nrf_cfg.lookupValue(AMF_CONFIG_STRING_NRF_PORT, nrf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_NRF_PORT "failed");
          throw(AMF_CONFIG_STRING_NRF_PORT "failed");
        }
        nrf_addr.port = nrf_port;
        //
        nrf_addr.api_version =
            DEFAULT_SBI_API_VERSION;  // TODO: get API version
      }
    }

    // AUSF
    if (support_features.enable_external_ausf) {
      const Setting& ausf_cfg       = new_if_cfg[AMF_CONFIG_STRING_AUSF];
      struct in_addr ausf_ipv4_addr = {};
      unsigned int ausf_port        = {};
      std::string ausf_api_version  = {};

      if (!support_features.use_fqdn_dns) {
        // Store FQDN
        ausf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, ausf_addr.fqdn);

        ausf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), ausf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR AUSF !");
        ausf_addr.ipv4_addr = ausf_ipv4_addr;
        if (!(ausf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, ausf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        ausf_addr.port = ausf_port;

        if (!(ausf_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, ausf_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        ausf_addr.api_version = ausf_api_version;
      } else {
        ausf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, ausf_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(ausf_addr.fqdn, address, ausf_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR AUSF!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), ausf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR AUSF !");
          ausf_addr.ipv4_addr = ausf_ipv4_addr;
          // We hardcode nrf port from config for the moment
          if (!(ausf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, ausf_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
            throw(AMF_CONFIG_STRING_PORT "failed");
          }
          ausf_addr.port = ausf_port;
          ausf_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }

    // UDM
    if (support_features.enable_external_udm) {
      const Setting& udm_cfg       = new_if_cfg[AMF_CONFIG_STRING_UDM];
      struct in_addr udm_ipv4_addr = {};
      unsigned int udm_port        = {};
      std::string udm_api_version  = {};

      if (!support_features.use_fqdn_dns) {
        // Store FQDN
        udm_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, udm_addr.fqdn);

        udm_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), udm_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR UDM !");
        udm_addr.ipv4_addr = udm_ipv4_addr;
        if (!(udm_cfg.lookupValue(AMF_CONFIG_STRING_PORT, udm_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        udm_addr.port = udm_port;

        if (!(udm_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, udm_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        udm_addr.api_version = udm_api_version;
      } else {
        udm_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, udm_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(udm_addr.fqdn, address, udm_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR UDM!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), udm_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR UDM !");
          udm_addr.ipv4_addr = udm_ipv4_addr;
          // We hardcode nrf port from config for the moment
          if (!(udm_cfg.lookupValue(AMF_CONFIG_STRING_PORT, udm_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
            throw(AMF_CONFIG_STRING_PORT "failed");
          }
          udm_addr.port = udm_port;
          udm_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }

    // NSSF
    if (support_features.enable_external_nssf) {
      const Setting& nssf_cfg       = new_if_cfg[AMF_CONFIG_STRING_NSSF];
      struct in_addr nssf_ipv4_addr = {};
      unsigned int nssf_port        = {};
      std::string nssf_api_version  = {};

      if (!support_features.use_fqdn_dns) {
        // Store FQDN
        nssf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, nssf_addr.fqdn);
        nssf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), nssf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR NSSF !");
        nssf_addr.ipv4_addr = nssf_ipv4_addr;
        if (!(nssf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, nssf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        nssf_addr.port = nssf_port;

        if (!(nssf_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, nssf_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        nssf_addr.api_version = nssf_api_version;
      } else {
        nssf_cfg.lookupValue(AMF_CONFIG_STRING_FQDN_DNS, nssf_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(nssf_addr.fqdn, address, nssf_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR NSSF!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), nssf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR NSSF !");
          nssf_addr.ipv4_addr = nssf_ipv4_addr;
          nssf_addr.port      = nssf_port;
          nssf_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Emergency support
  try {
    const Setting& core_config = amf_cfg[AMF_CONFIG_STRING_CORE_CONFIGURATION];
    core_config.lookupValue(
        AMF_CONFIG_STRING_EMERGENCY_SUPPORT, is_emergency_support);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Authentication Info
  try {
    const Setting& auth = amf_cfg[AMF_CONFIG_STRING_AUTHENTICATION];
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_SERVER, auth_para.mysql_server);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_USER, auth_para.mysql_user);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_PASS, auth_para.mysql_pass);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_DB, auth_para.mysql_db);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_RANDOM, auth_para.random);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Integrity/Ciphering algorithms (NAS)
  try {
    const Setting& nas = amf_cfg[AMF_CONFIG_STRING_NAS];
    const Setting& intAlg =
        nas[AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST];

    int intCount = intAlg.getLength();
    for (int i = 0; i < intCount; i++) {
      std::string intAlgStr = intAlg[i];
      if (!intAlgStr.compare("NIA0"))
        nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
      if (!intAlgStr.compare("NIA1"))
        nas_cfg.prefered_integrity_algorithm[i] = IA1_128_5G;
      if (!intAlgStr.compare("NIA2"))
        nas_cfg.prefered_integrity_algorithm[i] = IA2_128_5G;
    }
    for (int i = intCount; i < 8; i++) {
      nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
    }
    const Setting& encAlg =
        nas[AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST];
    int encCount = encAlg.getLength();
    for (int i = 0; i < encCount; i++) {
      std::string encAlgStr = encAlg[i];
      if (!encAlgStr.compare("NEA0"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
      if (!encAlgStr.compare("NEA1"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA1_128_5G;
      if (!encAlgStr.compare("NEA2"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA2_128_5G;
    }
    for (int i = encCount; i < 8; i++) {
      nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  return 1;
}

//------------------------------------------------------------------------------
void amf_config::display() {
  Logger::config().info(
      "==== OAI-CN5G %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::config().info(
      "======================    AMF   =====================");
  Logger::config().info("Configuration AMF:");
  Logger::config().info("- Instance ................: %d", instance);
  Logger::config().info("- PID dir .................: %s", pid_dir.c_str());
  Logger::config().info("- AMF NAME.................: %s", amf_name.c_str());
  Logger::config().info(
      "- GUAMI (MCC, MNC, Region ID, AMF Set ID, AMF pointer): ");
  Logger::config().info(
      "    (%s, %s, %s, %s, %s)", guami.mcc.c_str(), guami.mnc.c_str(),
      guami.regionID.c_str(), guami.AmfSetID.c_str(), guami.AmfPointer.c_str());
  Logger::config().info("- Served Guami List:");
  for (int i = 0; i < guami_list.size(); i++) {
    Logger::config().info(
        "    (%s, %s, %s , %s, %s)", guami_list[i].mcc.c_str(),
        guami_list[i].mnc.c_str(), guami_list[i].regionID.c_str(),
        guami_list[i].AmfSetID.c_str(), guami_list[i].AmfPointer.c_str());
  }
  Logger::config().info(
      "- Relative Capacity .......: %d", relative_amf_capacity);
  Logger::config().info("- PLMN Support: ");
  for (int i = 0; i < plmn_list.size(); i++) {
    Logger::config().info(
        "    MCC, MNC ..............: %s, %s", plmn_list[i].mcc.c_str(),
        plmn_list[i].mnc.c_str());
    Logger::config().info("    TAC ...................: %d", plmn_list[i].tac);
    Logger::config().info("    Slice Support .........:");
    for (int j = 0; j < plmn_list[i].slice_list.size(); j++) {
      if (plmn_list[i].slice_list[j].sd != SD_NO_VALUE) {
        Logger::config().info(
            "        SST, SD ...........: %d, %ld (0x%x)",
            plmn_list[i].slice_list[j].sst, plmn_list[i].slice_list[j].sd,
            plmn_list[i].slice_list[j].sd);
      } else {
        Logger::config().info(
            "        SST ...............: %d", plmn_list[i].slice_list[j].sst);
      }
    }
  }
  Logger::config().info(
      "- Emergency Support .......: %s", is_emergency_support.c_str());

  Logger::config().info("- Database: ");
  Logger::config().info(
      "    MySQL Server Addr .....: %s", auth_para.mysql_server.c_str());
  Logger::config().info(
      "    MySQL user ............: %s", auth_para.mysql_user.c_str());
  Logger::config().info(
      "    MySQL pass ............: %s", auth_para.mysql_pass.c_str());
  Logger::config().info(
      "    MySQL DB ..............: %s", auth_para.mysql_db.c_str());

  // TODO:
  /*
    Logger::config().info("- Supported NAS Algorithm: ");
    Logger::config().info(
        "    Integrity Algorithm ...: %s","");
    Logger::config().info(
        "    Ciphering Algorithm ...: %s", "");

    st::string nas_itegrity_algorithm_list = {};
    for(auto s: nas_cfg.prefered_integrity_algorithm){
    }
  */

  Logger::config().info("- N2 Networking:");
  Logger::config().info("    Iface .................: %s", n2.if_name.c_str());
  Logger::config().info("    IP Addr ...............: %s", inet_ntoa(n2.addr4));
  Logger::config().info("    Port ..................: %d", n2.port);

  Logger::config().info("- SBI Networking:");
  Logger::config().info("    Iface .................: %s", sbi.if_name.c_str());
  Logger::config().info(
      "    IP Addr ...............: %s", inet_ntoa(sbi.addr4));
  Logger::config().info("    Port ..................: %d", sbi.port);
  Logger::config().info("    HTTP2 port ............: %d", sbi_http2_port);
  Logger::config().info(
      "    API version............: %s", sbi_api_version.c_str());

  if (support_features.enable_external_nrf) {
    Logger::config().info("- NRF:");
    Logger::config().info(
        "    IP Addr ...............: %s", inet_ntoa(nrf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", nrf_addr.port);
    Logger::config().info(
        "    API version ...........: %s", nrf_addr.api_version.c_str());
  }

  if (support_features.enable_external_nssf) {
    Logger::config().info("- NSSF:");
    Logger::config().info(
        "    IP Addr ...............: %s", inet_ntoa(nssf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", nssf_addr.port);
    Logger::config().info(
        "    API version ...........: %s", nssf_addr.api_version.c_str());
  }

  if (support_features.enable_external_ausf) {
    Logger::config().info("- AUSF:");
    Logger::config().info(
        "    IP Addr ...............: %s", inet_ntoa(ausf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", ausf_addr.port);
    Logger::config().info(
        "    API version ...........: %s", ausf_addr.api_version.c_str());
  }

  if (support_features.enable_external_udm) {
    Logger::config().info("- UDM:");
    Logger::config().info(
        "    IP Addr ...............: %s", inet_ntoa(udm_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", udm_addr.port);
    Logger::config().info(
        "    API version ...........: %s", udm_addr.api_version.c_str());
  }

  if (!support_features.enable_smf_selection) {
    Logger::config().info("- SMF Pool: ");
    for (int i = 0; i < smf_pool.size(); i++) {
      std::string selected = smf_pool[i].selected ? "true" : "false";
      std::string smf_info = support_features.use_fqdn_dns ?
                                 smf_pool[i].fqdn :
                                 smf_pool[i].ipv4.c_str();
      Logger::config().info(
          "    SMF_INSTANCE_ID %d (%s:%d, version %s) is selected: %s",
          smf_pool[i].id, smf_info.c_str(), smf_pool[i].port,
          smf_pool[i].version.c_str(), selected.c_str());
    }
  }

  Logger::config().info("- Supported Features:");
  Logger::config().info(
      "    NF Registration .......: %s",
      support_features.enable_nf_registration ? "Yes" : "No");
  Logger::config().info(
      "    NRF Selection .........: %s",
      support_features.enable_nrf_selection ? "Yes" : "No");
  Logger::config().info(
      "    SMF Selection .........: %s",
      support_features.enable_smf_selection ? "Yes" : "No");
  Logger::config().info(
      "    External NRF ..........: %s",
      support_features.enable_external_nrf ? "Yes" : "No");
  Logger::config().info(
      "    External AUSF .........: %s",
      support_features.enable_external_ausf ? "Yes" : "No");
  Logger::config().info(
      "    External UDM ..........: %s",
      support_features.enable_external_udm ? "Yes" : "No");
  Logger::config().info(
      "    External NSSF .........: %s",
      support_features.enable_external_nssf ? "Yes" : "No");
  Logger::config().info(
      "    Use FQDN ..............: %s",
      support_features.use_fqdn_dns ? "Yes" : "No");
  Logger::config().info(
      "    Use HTTP2..............: %s",
      support_features.use_http2 ? "Yes" : "No");
  Logger::config().info(
      "- Log Level will be .......: %s",
      spdlog::level::to_string_view(log_level));
}

//------------------------------------------------------------------------------
int amf_config::load_interface(
    const libconfig::Setting& if_cfg, interface_cfg_t& cfg) {
  if_cfg.lookupValue(AMF_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(
              cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::amf_app().error(
            "Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(
          words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::amf_app().error(
            "Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS " = %s in config file",
            address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)];  // you never know...
      if (inet_pton(AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) ==
          1) {
        memcpy(&cfg.addr4, buf_in_addr, sizeof(struct in_addr));
      } else {
        Logger::amf_app().error(
            "In conversion: Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS
            " = %s in config file",
            util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(
          ntohs(cfg.addr4.s_addr) &
          0xFFFFFFFF << (32 - std::stoi(util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(AMF_CONFIG_STRING_PORT, cfg.port);
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
bool amf_config::resolve_fqdn(
    const std::string& fqdn, struct in_addr& ipv4_addr) {
  uint8_t addr_type = {};
  unsigned int port = 0;
  string address    = {};

  fqdn::resolve(fqdn, address, port, addr_type);
  if (addr_type != 0) {  // IPv6: TODO
    Logger::amf_app().error("DO NOT SUPPORT IPV6 ADDR FOR THIS NF");
    return false;
  } else {  // IPv4
    if (inet_aton(util::trim(address).c_str(), &ipv4_addr) <= 0) return false;
    //    IPV4_STR_ADDR_TO_INADDR(
    //        util::trim(address).c_str(), ipv4_addr,
    //        "BAD IPv4 ADDRESS FORMAT FOR NF !");
  }
  return true;
}

//------------------------------------------------------------------------------
std::string amf_config::get_amf_n1n2_message_subscribe_uri(
    const std::string& ue_cxt_id) {
  unsigned int sbi_port = DEFAULT_HTTP1_PORT;
  if (support_features.use_http2) {
    sbi_port = sbi_http2_port;
  } else {
    sbi_port = sbi.port;
  }
  return std::string(inet_ntoa(*((struct in_addr*) &sbi.addr4))) + ":" +
         std::to_string(sbi_port) + NAMF_COMMUNICATION_BASE + sbi_api_version +
         "/ue-contexts/" + ue_cxt_id + "/n1-n2-messages/subscriptions";
}

//------------------------------------------------------------------------------
std::string amf_config::get_nrf_nf_discovery_service_uri() {
  return std::string(inet_ntoa(*((struct in_addr*) &nrf_addr.ipv4_addr))) +
         ":" + std::to_string(nrf_addr.port) + "/nnrf-disc/" +
         nrf_addr.api_version + "/nf-instances";
}

//------------------------------------------------------------------------------
std::string amf_config::get_nrf_nf_registration_uri(
    const std::string& nf_instance_id) {
  return std::string(inet_ntoa(*((struct in_addr*) &nrf_addr.ipv4_addr))) +
         ":" + std::to_string(nrf_addr.port) + "/nnrf-nfm/" +
         nrf_addr.api_version + "/nf-instances/" + nf_instance_id;
}

//------------------------------------------------------------------------------
std::string amf_config::get_udm_slice_selection_subscription_data_retrieval_uri(
    const std::string& supi) {
  return std::string(inet_ntoa(*((struct in_addr*) &udm_addr.ipv4_addr))) +
         ":" + std::to_string(udm_addr.port) + "/nudm-sdm/" +
         udm_addr.api_version + "/" + supi + "/nssai";
}

//------------------------------------------------------------------------------
std::string amf_config::get_nssf_network_slice_selection_information_uri() {
  return std::string(inet_ntoa(*((struct in_addr*) &nssf_addr.ipv4_addr))) +
         ":" + std::to_string(nssf_addr.port) + "/nnssf-nsselection/" +
         nssf_addr.api_version + "/network-slice-information";
}

//------------------------------------------------------------------------------
std::string amf_config::get_ausf_ue_authentications_uri() {
  return std::string(inet_ntoa(*((struct in_addr*) &ausf_addr.ipv4_addr))) +
         ":" + std::to_string(ausf_addr.port) + "/nausf-auth/" +
         ausf_addr.api_version + "/ue-authentications";
}

//------------------------------------------------------------------------------
bool amf_config::get_smf_pdu_session_context_uri(
    const std::shared_ptr<pdu_session_context>& psc, std::string& smf_uri) {
  if (!psc) return false;

  if (!psc->smf_info.info_available) {
    Logger::amf_sbi().error("No SMF is available for this PDU session");
    return false;
  }

  std::string smf_addr    = {};
  std::string smf_ip_addr = {};

  smf_addr = psc->smf_info.addr;

  // remove http port from the URI if existed
  std::size_t found_port = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port - 1);
  else
    smf_ip_addr = smf_addr;

  std::size_t found = psc->smf_info.context_location.find(smf_ip_addr);
  if (found != std::string::npos)
    smf_uri = psc->smf_info.context_location;
  else
    smf_uri = smf_addr + ":" + std::to_string(psc->smf_info.port) +
              psc->smf_info.context_location;
  return true;
}

//------------------------------------------------------------------------------
std::string amf_config::get_smf_pdu_session_base_uri(
    const std::string& smf_addr, const uint32_t& smf_port,
    const std::string& smf_api_version) {
  // Remove http port from the URI if existed
  std::string smf_ip_addr = {};
  std::size_t found_port  = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port);
  else
    smf_ip_addr = smf_addr;

  return smf_ip_addr + ":" + std::to_string(smf_port) + "/nsmf-pdusession/" +
         smf_api_version + NSMF_PDU_SESSION_CREATE;
}

//------------------------------------------------------------------------------
void amf_config::to_json(nlohmann::json& json_data) const {
  json_data["instance"]   = instance;
  json_data["pid_dir"]    = pid_dir;
  json_data["log_level"]  = log_level;
  json_data["amf_name"]   = amf_name;
  json_data["guami"]      = guami.to_json();
  json_data["guami_list"] = nlohmann::json::array();
  for (auto s : guami_list) {
    json_data["guami_list"].push_back(s.to_json());
  }

  json_data["relative_amf_capacity"] = relative_amf_capacity;

  json_data["plmn_list"] = nlohmann::json::array();
  for (auto s : plmn_list) {
    json_data["plmn_list"].push_back(s.to_json());
  }
  json_data["is_emergency_support"] = is_emergency_support;

  json_data["auth_para"] = auth_para.to_json();

  json_data["n2"]                    = n2.to_json();
  json_data["sbi"]                   = sbi.to_json();
  json_data["sbi"]["sbi_http2_port"] = sbi_http2_port;

  json_data["support_features"] = support_features.to_json();

  if (support_features.enable_external_nrf) {
    json_data["nrf"] = nrf_addr.to_json();
  }

  if (support_features.enable_external_nssf) {
    json_data["nssf"] = nssf_addr.to_json();
  }

  if (support_features.enable_external_ausf) {
    json_data["ausf"] = ausf_addr.to_json();
  }

  if (support_features.enable_external_udm) {
    json_data["udm"] = udm_addr.to_json();
  }

  json_data["smf_pool"] = nlohmann::json::array();
  for (auto s : smf_pool) {
    json_data["smf_pool"].push_back(s.to_json());
  }
}

//------------------------------------------------------------------------------
bool amf_config::from_json(nlohmann::json& json_data) {
  try {
    if (json_data.find("instance") != json_data.end()) {
      instance = json_data["instance"].get<int>();
    }

    if (json_data.find("pid_dir") != json_data.end()) {
      pid_dir = json_data["pid_dir"].get<std::string>();
    }
    if (json_data.find("amf_name") != json_data.end()) {
      amf_name = json_data["amf_name"].get<std::string>();
    }
    if (json_data.find("log_level") != json_data.end()) {
      log_level = json_data["log_level"].get<spdlog::level::level_enum>();
    }
    if (json_data.find("guami") != json_data.end()) {
      guami.from_json(json_data["guami"]);
    }

    if (json_data.find("guami_list") != json_data.end()) {
      guami_list.clear();
      for (auto s : json_data["guami_list"]) {
        guami_t g = {};
        g.from_json(s);
        guami_list.push_back(g);
      }
    }

    if (json_data.find("relative_amf_capacity") != json_data.end()) {
      relative_amf_capacity = json_data["relative_amf_capacity"].get<int>();
    }

    if (json_data.find("plmn_list") != json_data.end()) {
      plmn_list.clear();
      for (auto s : json_data["plmn_list"]) {
        plmn_item_t p = {};
        p.from_json(s);
        plmn_list.push_back(p);
      }
    }

    if (json_data.find("is_emergency_support") != json_data.end()) {
      is_emergency_support =
          json_data["is_emergency_support"].get<std::string>();
    }

    if (json_data.find("auth_para") != json_data.end()) {
      auth_para.from_json(json_data["auth_para"]);
    }

    if (json_data.find("n2") != json_data.end()) {
      n2.from_json(json_data["n2"]);
    }
    if (json_data.find("sbi") != json_data.end()) {
      sbi.from_json(json_data["sbi"]);
    }

    if (json_data.find("sbi_http2_port") != json_data.end()) {
      sbi_http2_port = json_data["sbi_http2_port"].get<int>();
    }

    if (json_data.find("support_features") != json_data.end()) {
      support_features.from_json(json_data["support_features"]);
    }

    if (support_features.enable_external_nrf) {
      if (json_data.find("nrf") != json_data.end()) {
        nrf_addr.from_json(json_data["nrf"]);
        // if use FQDN -> update IP addr accordingly
        if (support_features.use_fqdn_dns) {
          resolve_fqdn(nrf_addr.fqdn, nrf_addr.ipv4_addr);
        }
      }
    }

    if (support_features.enable_external_nssf) {
      if (json_data.find("nssf") != json_data.end()) {
        nssf_addr.from_json(json_data["nssf"]);
        // if use FQDN -> update IP addr accordingly
        if (support_features.use_fqdn_dns) {
          resolve_fqdn(nssf_addr.fqdn, nssf_addr.ipv4_addr);
        }
      }
    }

    if (support_features.enable_external_ausf) {
      if (json_data.find("ausf") != json_data.end()) {
        ausf_addr.from_json(json_data["ausf"]);
        // if use FQDN -> update IP addr accordingly
        if (support_features.use_fqdn_dns) {
          resolve_fqdn(ausf_addr.fqdn, ausf_addr.ipv4_addr);
        }
      }
    }

    if (support_features.enable_external_udm) {
      if (json_data.find("udm") != json_data.end()) {
        udm_addr.from_json(json_data["udm"]);
        // if use FQDN -> update IP addr accordingly
        if (support_features.use_fqdn_dns) {
          resolve_fqdn(udm_addr.fqdn, udm_addr.ipv4_addr);
        }
      }
    }

    if (json_data.find("smf_pool") != json_data.end()) {
      smf_pool.clear();
      for (auto s : json_data["smf_pool"]) {
        smf_inst_t smf_item      = {};
        struct in_addr ipv4_addr = {};
        smf_item.from_json(s);
        // if use FQDN -> update IP addr accordingly
        if (support_features.use_fqdn_dns) {
          resolve_fqdn(smf_item.fqdn, ipv4_addr);
          smf_item.ipv4 = inet_ntoa(ipv4_addr);
        }
        smf_pool.push_back(smf_item);
      }
    }

  } catch (nlohmann::detail::exception& e) {
    Logger::amf_app().error(
        "Exception when reading configuration from json %s", e.what());
    return false;
  } catch (std::exception& e) {
    Logger::amf_app().error(
        "Exception when reading configuration from json %s", e.what());
    return false;
  }

  return true;
}
}  // namespace config
