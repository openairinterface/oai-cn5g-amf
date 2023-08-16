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

#include "amf_cfg_libconfig.hpp"

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

namespace oai::config {

//------------------------------------------------------------------------------
amf_cfg_libconfig::amf_cfg_libconfig() {
  // TODO:
}

//------------------------------------------------------------------------------
amf_cfg_libconfig::~amf_cfg_libconfig() {}

//------------------------------------------------------------------------------
int amf_cfg_libconfig::load(
    const std::string& config_file, amf_config& amf_cfg_value) {
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
    amf_cfg.lookupValue(AMF_CONFIG_STRING_INSTANCE_ID, amf_cfg_value.instance);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // Statistic Timer interval
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL,
        amf_cfg_value.statistics_interval);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // PID Dir
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_PID_DIRECTORY, amf_cfg_value.pid_dir);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // AMF Name
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_AMF_NAME, amf_cfg_value.amf_name);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // Log Level
  try {
    std::string string_level;
    amf_cfg.lookupValue(AMF_CONFIG_STRING_LOG_LEVEL, string_level);
    amf_cfg_value.log_level = spdlog::level::from_str(string_level);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // GUAMI
  try {
    const Setting& guami_cfg = amf_cfg[AMF_CONFIG_STRING_GUAMI];
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MCC, amf_cfg_value.guami.mcc);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MNC, amf_cfg_value.guami.mnc);
    int region_id = {};
    guami_cfg.lookupValue(AMF_CONFIG_STRING_REGION_ID, region_id);
    amf_cfg_value.guami.region_id = region_id & 0xff;
    int amf_set_id                = {};
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMF_SET_ID, amf_set_id);
    amf_cfg_value.guami.amf_set_id = amf_set_id & 0x03ff;
    int amf_pointer                = {};
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMF_POINTER, amf_pointer);
    amf_cfg_value.guami.amf_pointer = amf_pointer & 0x3f;
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
      int region_id = {};
      guami_item.lookupValue(AMF_CONFIG_STRING_REGION_ID, region_id);
      guami.region_id = region_id & 0xff;
      int amf_set_id  = {};
      guami_item.lookupValue(AMF_CONFIG_STRING_AMF_SET_ID, amf_set_id);
      guami.amf_set_id = amf_set_id & 0x03ff;
      int amf_pointer  = {};
      guami_item.lookupValue(AMF_CONFIG_STRING_AMF_POINTER, amf_pointer);
      guami.amf_pointer = amf_pointer & 0x3f;
      amf_cfg_value.guami_list.push_back(guami);
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  // AMF Capacity
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY,
        amf_cfg_value.relative_amf_capacity);
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
      amf_cfg_value.plmn_list.push_back(plmn_item);
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
      amf_cfg_value.support_features.enable_nf_registration = true;
    } else {
      amf_cfg_value.support_features.enable_nf_registration = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.enable_smf_selection = true;
    } else {
      amf_cfg_value.support_features.enable_smf_selection = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.enable_external_ausf = true;
    } else {
      amf_cfg_value.support_features.enable_external_ausf = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.enable_external_udm = true;
    } else {
      amf_cfg_value.support_features.enable_external_udm = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_ENABLE_NSSF, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.enable_nssf = true;
    } else {
      amf_cfg_value.support_features.enable_nssf = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_FQDN_DNS, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.use_fqdn_dns = true;
    } else {
      amf_cfg_value.support_features.use_fqdn_dns = false;
    }

    support_features_cfg.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_HTTP2, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.support_features.use_http2 = true;
    } else {
      amf_cfg_value.support_features.use_http2 = false;
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Network interfaces/NFs
  try {
    const Setting& new_if_cfg = amf_cfg[AMF_CONFIG_STRING_INTERFACES];

    // N2
    const Setting& n2_amf_cfg =
        new_if_cfg[AMF_CONFIG_STRING_INTERFACE_NGAP_AMF];
    load_interface(n2_amf_cfg, amf_cfg_value.n2);

    // SBI
    const Setting& sbi_cfg = new_if_cfg[AMF_CONFIG_STRING_INTERFACE_SBI];
    load_interface(sbi_cfg, amf_cfg_value.sbi);

    // SBI API VERSION
    if (!(sbi_cfg.lookupValue(
            AMF_CONFIG_STRING_API_VERSION, amf_cfg_value.sbi_api_version))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
      throw(AMF_CONFIG_STRING_API_VERSION "failed");
    }

    // HTTP2 port
    if (!(sbi_cfg.lookupValue(
            AMF_CONFIG_STRING_SBI_HTTP2_PORT, amf_cfg_value.sbi_http2_port))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
      throw(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
    }

    if (!amf_cfg_value.support_features.enable_smf_selection) {
      // SMF
      const Setting& smf_addr_pool =
          sbi_cfg[AMF_CONFIG_STRING_SMF_INSTANCES_POOL];
      int count = smf_addr_pool.getLength();
      for (int i = 0; i < count; i++) {
        const Setting& smf_addr_item = smf_addr_pool[i];
        struct in_addr smf_ipv4_addr = {};
        unsigned int smf_port        = {};
        std::string smf_port_str     = {};
        uint32_t smf_http2_port      = {};
        std::string smf_api_version  = {};
        std::string selected         = {};
        string address               = {};

        smf_addr_item.lookupValue(
            AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED, selected);
        if (!boost::iequals(selected, "true")) continue;

        // Store FQDN
        smf_addr_item.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.smf_addr.fqdn);

        if (!amf_cfg_value.support_features.use_fqdn_dns) {
          smf_addr_item.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), amf_cfg_value.smf_addr.ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR SMF !");
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SMF_INSTANCE_PORT, smf_port_str))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SMF_INSTANCE_PORT
                                    "failed");
            throw(AMF_CONFIG_STRING_SMF_INSTANCE_PORT "failed");
          }
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SBI_HTTP2_PORT, smf_http2_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
            throw(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
          }

          if (!amf_cfg_value.support_features.use_http2) {
            amf_cfg_value.smf_addr.uri_root =
                util::trim(address) + ":" + smf_port_str;
          } else {
            amf_cfg_value.smf_addr.uri_root =
                util::trim(address) + ":" + std::to_string(smf_http2_port);
          }

          smf_addr_item.lookupValue(
              AMF_CONFIG_STRING_SMF_INSTANCE_VERSION,
              amf_cfg_value.smf_addr.api_version);
          if (!(smf_addr_item.lookupValue(
                  AMF_CONFIG_STRING_SMF_INSTANCE_VERSION,
                  amf_cfg_value.smf_addr.api_version))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_SMF_INSTANCE_VERSION
                                    "failed");
            throw(AMF_CONFIG_STRING_SMF_INSTANCE_VERSION "failed");
          }
        } else {
          smf_addr_item.lookupValue(
              AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.smf_addr.fqdn);
          uint8_t addr_type = {};
          fqdn::resolve(
              amf_cfg_value.smf_addr.fqdn, address, smf_port, addr_type);
          if (addr_type != 0) {  // IPv6: TODO
            throw("DO NOT SUPPORT IPV6 ADDR FOR SMF!");
          } else {  // IPv4
            IPV4_STR_ADDR_TO_INADDR(
                util::trim(address).c_str(), smf_ipv4_addr,
                "BAD IPv4 ADDRESS FORMAT FOR SMF !");
            amf_cfg_value.smf_addr.ipv4_addr = smf_ipv4_addr;
            // smf_addr.port        = smf_port;

            // We hardcode smf port from config for the moment
            if (!(smf_addr_item.lookupValue(
                    AMF_CONFIG_STRING_PORT, smf_port_str))) {
              Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
              throw(AMF_CONFIG_STRING_PORT "failed");
            }
            if (!(smf_addr_item.lookupValue(
                    AMF_CONFIG_STRING_SBI_HTTP2_PORT, smf_http2_port))) {
              Logger::amf_app().error(AMF_CONFIG_STRING_SBI_HTTP2_PORT
                                      "failed");
              throw(AMF_CONFIG_STRING_SBI_HTTP2_PORT "failed");
            }

            // amf_cfg_value.smf_addr.port = smf_port;
            if (!amf_cfg_value.support_features.use_http2) {
              amf_cfg_value.smf_addr.uri_root =
                  util::trim(address) + ":" + smf_port_str;
            } else {
              amf_cfg_value.smf_addr.uri_root =
                  util::trim(address) + ":" + std::to_string(smf_http2_port);
            }

            amf_cfg_value.smf_addr.api_version =
                DEFAULT_SBI_API_VERSION;  // TODO: get API version
          }
        }
      }
    }

    /*

        if (!amf_cfg_value.support_features.enable_smf_selection) {
          // SMF
          const Setting& smf_cfg       = new_if_cfg[AMF_CONFIG_STRING_SMF];
          struct in_addr smf_ipv4_addr = {};
          unsigned int smf_port        = 0;
          std::string smf_api_version  = {};
          string address               = {};

          if (!amf_cfg_value.support_features.use_fqdn_dns) {
            // Store FQDN
            smf_cfg.lookupValue(
                AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.smf_addr.fqdn);

            smf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
            IPV4_STR_ADDR_TO_INADDR(
                util::trim(address).c_str(), smf_ipv4_addr,
                "BAD IPv4 ADDRESS FORMAT FOR SMF !");
            amf_cfg_value.smf_addr.ipv4_addr = smf_ipv4_addr;
            if (!(smf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, smf_port))) {
              Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
              throw(AMF_CONFIG_STRING_PORT "failed");
            }
            amf_cfg_value.smf_addr.port = smf_port;
            amf_cfg_value.smf_addr.uri_root =
                util::trim(address) + ":" + std::to_string(smf_port);
            if (!(smf_cfg.lookupValue(
                    AMF_CONFIG_STRING_API_VERSION, smf_api_version))) {
              Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
              throw(AMF_CONFIG_STRING_API_VERSION "failed");
            }
            amf_cfg_value.smf_addr.api_version = smf_api_version;
          } else {
            smf_cfg.lookupValue(
                AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.smf_addr.fqdn);
            uint8_t addr_type = {};
            fqdn::resolve(
                amf_cfg_value.smf_addr.fqdn, address, smf_port, addr_type);
            if (addr_type != 0) {  // IPv6: TODO
              throw("DO NOT SUPPORT IPV6 ADDR FOR SMF!");
            } else {  // IPv4
              IPV4_STR_ADDR_TO_INADDR(
                  util::trim(address).c_str(), smf_ipv4_addr,
                  "BAD IPv4 ADDRESS FORMAT FOR SMF !");
              amf_cfg_value.smf_addr.ipv4_addr = smf_ipv4_addr;
              // smf_addr.port        = smf_port;

              // We hardcode smf port from config for the moment
              if (!(smf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, smf_port))) {
                Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
                throw(AMF_CONFIG_STRING_PORT "failed");
              }
              amf_cfg_value.smf_addr.port = smf_port;
              amf_cfg_value.smf_addr.uri_root =
                  util::trim(address) + ":" + std::to_string(smf_port);
              amf_cfg_value.smf_addr.api_version =
                  DEFAULT_SBI_API_VERSION;  // TODO: get API version
            }
          }
        }
    */
    // NRF
    const Setting& nrf_cfg       = new_if_cfg[AMF_CONFIG_STRING_NRF];
    struct in_addr nrf_ipv4_addr = {};
    unsigned int nrf_port        = 0;
    std::string nrf_api_version  = {};
    string address               = {};

    if (!amf_cfg_value.support_features.use_fqdn_dns) {
      // Store FQDN
      nrf_cfg.lookupValue(
          AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.nrf_addr.fqdn);

      nrf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(address).c_str(), nrf_ipv4_addr,
          "BAD IPv4 ADDRESS FORMAT FOR NRF !");
      amf_cfg_value.nrf_addr.ipv4_addr = nrf_ipv4_addr;
      if (!(nrf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, nrf_port))) {
        Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
        throw(AMF_CONFIG_STRING_PORT "failed");
      }
      amf_cfg_value.nrf_addr.port = nrf_port;
      amf_cfg_value.nrf_addr.uri_root =
          util::trim(address) + ":" + std::to_string(nrf_port);
      if (!(nrf_cfg.lookupValue(
              AMF_CONFIG_STRING_API_VERSION, nrf_api_version))) {
        Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
        throw(AMF_CONFIG_STRING_API_VERSION "failed");
      }
      amf_cfg_value.nrf_addr.api_version = nrf_api_version;

    } else {
      nrf_cfg.lookupValue(
          AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.nrf_addr.fqdn);
      uint8_t addr_type = {};
      fqdn::resolve(amf_cfg_value.nrf_addr.fqdn, address, nrf_port, addr_type);
      if (addr_type != 0) {  // IPv6: TODO
        throw("DO NOT SUPPORT IPV6 ADDR FOR NRF!");
      } else {  // IPv4
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), nrf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR NRF !");
        amf_cfg_value.nrf_addr.ipv4_addr = nrf_ipv4_addr;
        // nrf_addr.port        = nrf_port;

        // We hardcode nrf port from config for the moment
        if (!(nrf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, nrf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        amf_cfg_value.nrf_addr.port = nrf_port;
        amf_cfg_value.nrf_addr.uri_root =
            util::trim(address) + ":" + std::to_string(nrf_port);
        amf_cfg_value.nrf_addr.api_version =
            DEFAULT_SBI_API_VERSION;  // TODO: get API version
      }
    }

    // AUSF
    if (amf_cfg_value.support_features.enable_external_ausf) {
      const Setting& ausf_cfg       = new_if_cfg[AMF_CONFIG_STRING_AUSF];
      struct in_addr ausf_ipv4_addr = {};
      unsigned int ausf_port        = {};
      std::string ausf_api_version  = {};

      if (!amf_cfg_value.support_features.use_fqdn_dns) {
        // Store FQDN
        ausf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.ausf_addr.fqdn);

        ausf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), ausf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR AUSF !");
        amf_cfg_value.ausf_addr.ipv4_addr = ausf_ipv4_addr;
        if (!(ausf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, ausf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        amf_cfg_value.ausf_addr.port = ausf_port;
        amf_cfg_value.ausf_addr.uri_root =
            util::trim(address) + ":" + std::to_string(ausf_port);
        if (!(ausf_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, ausf_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        amf_cfg_value.ausf_addr.api_version = ausf_api_version;
      } else {
        ausf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.ausf_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(
            amf_cfg_value.ausf_addr.fqdn, address, ausf_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR AUSF!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), ausf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR AUSF !");
          amf_cfg_value.ausf_addr.ipv4_addr = ausf_ipv4_addr;
          // We hardcode nrf port from config for the moment
          if (!(ausf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, ausf_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
            throw(AMF_CONFIG_STRING_PORT "failed");
          }
          amf_cfg_value.ausf_addr.port = ausf_port;
          amf_cfg_value.ausf_addr.uri_root =
              util::trim(address) + ":" + std::to_string(ausf_port);
          amf_cfg_value.ausf_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }

    // UDM
    if (amf_cfg_value.support_features.enable_external_udm) {
      const Setting& udm_cfg       = new_if_cfg[AMF_CONFIG_STRING_UDM];
      struct in_addr udm_ipv4_addr = {};
      unsigned int udm_port        = {};
      std::string udm_api_version  = {};

      if (!amf_cfg_value.support_features.use_fqdn_dns) {
        // Store FQDN
        udm_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.udm_addr.fqdn);

        udm_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), udm_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR UDM !");
        amf_cfg_value.udm_addr.ipv4_addr = udm_ipv4_addr;
        if (!(udm_cfg.lookupValue(AMF_CONFIG_STRING_PORT, udm_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        amf_cfg_value.udm_addr.port = udm_port;
        amf_cfg_value.udm_addr.uri_root =
            util::trim(address) + ":" + std::to_string(udm_port);
        if (!(udm_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, udm_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        amf_cfg_value.udm_addr.api_version = udm_api_version;
      } else {
        udm_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.udm_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(
            amf_cfg_value.udm_addr.fqdn, address, udm_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR UDM!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), udm_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR UDM !");
          amf_cfg_value.udm_addr.ipv4_addr = udm_ipv4_addr;
          // We hardcode nrf port from config for the moment
          if (!(udm_cfg.lookupValue(AMF_CONFIG_STRING_PORT, udm_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
            throw(AMF_CONFIG_STRING_PORT "failed");
          }
          amf_cfg_value.udm_addr.port = udm_port;
          amf_cfg_value.udm_addr.uri_root =
              util::trim(address) + ":" + std::to_string(udm_port);
          amf_cfg_value.udm_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }

    // NSSF
    if (amf_cfg_value.support_features.enable_nssf) {
      const Setting& nssf_cfg       = new_if_cfg[AMF_CONFIG_STRING_NSSF];
      struct in_addr nssf_ipv4_addr = {};
      unsigned int nssf_port        = {};
      std::string nssf_api_version  = {};

      if (!amf_cfg_value.support_features.use_fqdn_dns) {
        // Store FQDN
        nssf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.nssf_addr.fqdn);
        nssf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), nssf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR NSSF !");
        amf_cfg_value.nssf_addr.ipv4_addr = nssf_ipv4_addr;
        if (!(nssf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, nssf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        amf_cfg_value.nssf_addr.port = nssf_port;
        amf_cfg_value.nssf_addr.uri_root =
            util::trim(address) + ":" + std::to_string(nssf_port);
        if (!(nssf_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, nssf_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        amf_cfg_value.nssf_addr.api_version = nssf_api_version;
      } else {
        nssf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.nssf_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(
            amf_cfg_value.nssf_addr.fqdn, address, nssf_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR NSSF!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), nssf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR NSSF !");
          amf_cfg_value.nssf_addr.ipv4_addr = nssf_ipv4_addr;
          if (!(nssf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, nssf_port))) {
            Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
            throw(AMF_CONFIG_STRING_PORT "failed");
          }
          amf_cfg_value.nssf_addr.port = nssf_port;
          amf_cfg_value.nssf_addr.uri_root =
              util::trim(address) + ":" + std::to_string(nssf_port);
          amf_cfg_value.nssf_addr.api_version =
              DEFAULT_SBI_API_VERSION;  // TODO: get API version
        }
      }
    }
    // LMF
    if (amf_cfg_value.support_features.enable_lmf) {
      const Setting& lmf_cfg       = new_if_cfg[AMF_CONFIG_STRING_LMF];
      struct in_addr lmf_ipv4_addr = {};
      unsigned int lmf_port        = {};
      std::string lmf_api_version  = {};

      if (amf_cfg_value.support_features.use_fqdn_dns) {
        // Store FQDN
        lmf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.lmf_addr.fqdn);

        lmf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
        IPV4_STR_ADDR_TO_INADDR(
            util::trim(address).c_str(), lmf_ipv4_addr,
            "BAD IPv4 ADDRESS FORMAT FOR LMF !");
        amf_cfg_value.lmf_addr.ipv4_addr = lmf_ipv4_addr;
        if (!(lmf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, lmf_port))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
          throw(AMF_CONFIG_STRING_PORT "failed");
        }
        amf_cfg_value.lmf_addr.port = lmf_port;
        amf_cfg_value.lmf_addr.uri_root =
            util::trim(address) + ":" + std::to_string(lmf_port);

        if (!(lmf_cfg.lookupValue(
                AMF_CONFIG_STRING_API_VERSION, lmf_api_version))) {
          Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
          throw(AMF_CONFIG_STRING_API_VERSION "failed");
        }
        amf_cfg_value.lmf_addr.api_version = lmf_api_version;
      } else {
        lmf_cfg.lookupValue(
            AMF_CONFIG_STRING_FQDN_DNS, amf_cfg_value.lmf_addr.fqdn);
        uint8_t addr_type = {};
        fqdn::resolve(
            amf_cfg_value.lmf_addr.fqdn, address, lmf_port, addr_type);
        if (addr_type != 0) {  // IPv6: TODO
          throw("DO NOT SUPPORT IPV6 ADDR FOR LMF!");
        } else {  // IPv4
          IPV4_STR_ADDR_TO_INADDR(
              util::trim(address).c_str(), lmf_ipv4_addr,
              "BAD IPv4 ADDRESS FORMAT FOR LMF !");
          amf_cfg_value.lmf_addr.ipv4_addr   = lmf_ipv4_addr;
          amf_cfg_value.lmf_addr.port        = lmf_port;
          amf_cfg_value.lmf_addr.api_version = "v1";  // TODO: get API version
          amf_cfg_value.lmf_addr.uri_root =
              util::trim(address) + ":" + std::to_string(lmf_port);
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
    std::string opt            = {};
    const Setting& core_config = amf_cfg[AMF_CONFIG_STRING_CORE_CONFIGURATION];
    core_config.lookupValue(AMF_CONFIG_STRING_EMERGENCY_SUPPORT, opt);

    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.is_emergency_support = true;
    } else {
      amf_cfg_value.is_emergency_support = false;
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  // Authentication Info
  try {
    const Setting& auth = amf_cfg[AMF_CONFIG_STRING_AUTHENTICATION];
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_SERVER,
        amf_cfg_value.auth_para.mysql_server);
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_USER, amf_cfg_value.auth_para.mysql_user);
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_PASS, amf_cfg_value.auth_para.mysql_pass);
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_DB, amf_cfg_value.auth_para.mysql_db);
    string opt;
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_RANDOM, opt);
    if (boost::iequals(opt, "yes")) {
      amf_cfg_value.auth_para.random = true;
    } else {
      amf_cfg_value.auth_para.random = false;
    }
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
      if (!intAlgStr.compare("NIA0")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA0);
      }
      if (!intAlgStr.compare("NIA1")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA1);
      }
      if (!intAlgStr.compare("NIA2")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA2);
      }
      if (!intAlgStr.compare("NIA3")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA3);
      }
      if (!intAlgStr.compare("NIA4")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA4);
      }
      if (!intAlgStr.compare("NIA5")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA5);
      }
      if (!intAlgStr.compare("NIA6")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA6);
      }
      if (!intAlgStr.compare("NIA7")) {
        amf_cfg_value.nas_cfg.prefered_integrity_algorithm.push_back(
            _5g_ia_e::_5G_IA7);
      }
    }

    const Setting& encAlg =
        nas[AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST];
    int encCount = encAlg.getLength();
    for (int i = 0; i < encCount; i++) {
      std::string encAlgStr = encAlg[i];
      if (!encAlgStr.compare("NEA0")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA0);
      }
      if (!encAlgStr.compare("NEA1")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA1);
      }
      if (!encAlgStr.compare("NEA2")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA2);
      }
      if (!encAlgStr.compare("NEA3")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA3);
      }
      if (!encAlgStr.compare("NEA4")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA4);
      }
      if (!encAlgStr.compare("NEA5")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA5);
      }
      if (!encAlgStr.compare("NEA6")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA6);
      }
      if (!encAlgStr.compare("NEA7")) {
        amf_cfg_value.nas_cfg.prefered_ciphering_algorithm.push_back(
            _5g_ea_e::_5G_EA7);
      }
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  return 1;
}

//------------------------------------------------------------------------------
int amf_cfg_libconfig::load_interface(
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
bool amf_cfg_libconfig::resolve_fqdn(
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

}  // namespace oai::config
