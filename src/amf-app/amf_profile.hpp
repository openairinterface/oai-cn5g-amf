/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_AMF_PROFILE_HPP_SEEN
#define FILE_AMF_PROFILE_HPP_SEEN

#include <arpa/inet.h>
#include <netinet/in.h>

#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "3gpp_29.510.h"
#include "amf.hpp"
#include "logger.hpp"
#include "nf_profile.hpp"

namespace amf_application {

class amf_profile : public oai::sba::nf_profile {
 public:
  amf_profile() : oai::sba::nf_profile() {
    nf_type     = "NF_TYPE_UNKNOWN";
    custom_info = {};
  }

  amf_profile(const std::string& id) : oai::sba::nf_profile(id) {
    nf_type     = "NF_TYPE_UNKNOWN";
    custom_info = {};
  }

  amf_profile(const amf_profile& s) : oai::sba::nf_profile() { *this = s; }

  amf_profile& operator=(const amf_profile& s) {
    nf_instance_id   = s.nf_instance_id;
    heartBeat_timer  = s.heartBeat_timer;
    snssais          = s.snssais;
    ipv4_addresses   = s.ipv4_addresses;
    priority         = s.priority;
    capacity         = s.capacity;
    nf_type          = s.nf_type;
    nf_instance_name = s.nf_instance_name;
    nf_status        = s.nf_status;
    fqdn             = s.fqdn;
    plmn_list        = s.plmn_list;
    ipv6_addresses   = s.ipv6_addresses;
    json_data        = s.json_data;
    is_updated       = s.is_updated;
    custom_info      = s.custom_info;
    amf_info         = s.amf_info;
    nf_services      = s.nf_services;
    nf_service_list  = s.nf_service_list;
    return *this;
  }
  // amf_profile(amf_profile &b) = delete;

  ~amf_profile() override {
    Logger::amf_app().debug("Delete AMF Profile instance...");
  }

  /*
   * Set NF instance services
   * @param [std::vector<nf_service_t> &] n: nf_service
   * @return void
   */
  void set_nf_services(const std::vector<oai::common::sbi::nf_service_t>& n);

  /*
   * Add nf service
   * @param [snssai_t &] n: nf service
   * @return void
   */
  void add_nf_service(const oai::common::sbi::nf_service_t& n);

  /*
   * Get NF services
   * @param [std::vector<snssai_t> &] n: store instance's nf services
   * @return void:
   */
  void get_nf_services(std::vector<oai::common::sbi::nf_service_t>& n) const;

  /* Remove all NF instance IPv4 addresses. */
  void delete_nf_ipv4_addresses();

  /*
   * Delete all NF services
   * @param void
   * @return void:
   */
  void delete_nf_services();

  /*
   * Add nf service
   * @param [snssai_t &] n: nf service
   * @return void
   */
  void add_nf_service_list(const oai::common::sbi::nf_service_t& n);

  /*
   * Set custom info
   * @param [const nlohmann::json &] c: custom info to be set
   * @return void
   */
  void set_custom_info(const nlohmann::json& c);

  /*
   * Get custom info
   * @param [nlohmann::json &] c: Store custom info
   * @return void
   */
  void get_custom_info(nlohmann::json& c) const;

  /*
   * Set amf info
   * @param [amf_info_t &] s: amf info
   * @return void
   */
  void set_amf_info(const oai::common::sbi::amf_info_t& s);

  /*
   * Get NF instance amf info
   * @param [amf_info_t &] s: store instance's amf info
   * @return void:
   */
  void get_amf_info(oai::common::sbi::amf_info_t& s) const;

  /*
   * Print related-information for NF profile
   * @param void
   * @return void:
   */
  void display() override;

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void to_json(nlohmann::json& data) const override;

  /*
   * Covert from a json represetation to AMF profile
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void from_json(const nlohmann::json& data);

  /*
   * Handle heartbeart timeout event
   * @param [uint64_t] ms: current time
   * @return void
   */
  void handle_heartbeart_timeout(uint64_t ms);

 protected:
  std::map<std::string, oai::common::sbi::nf_service_t> nf_service_list;
  oai::common::sbi::amf_info_t amf_info;
};

}  // namespace amf_application

#endif
