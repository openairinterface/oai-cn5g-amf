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

#ifndef _NGAP_APPLICATION_H_
#define _NGAP_APPLICATION_H_

#include <map>
#include <memory>
#include <shared_mutex>

#include "gNB_context.hpp"
#include "sctp_server.hpp"

using namespace sctp;

namespace oai::ngap {

class ngap_app : public sctp_application {
 public:
  ngap_app(const std::string& address, const uint16_t port_num);
  virtual ~ngap_app();

  /*
   * Get Payload protocol ID
   * @param void
   * @return PP ID in Uint32_t:
   */
  uint32_t get_ppid();

  /*
   * Handle SCTP payload (decode it and send ITTI msg to N2)
   * @param [bstring] payload: payload
   * @param [sctp_assoc_id_t] assoc_id: gNB association ID
   * @param [sctp_stream_id_t] stream: stream
   * @param [sctp_stream_id_t] instreams: instreams
   * @param [sctp_stream_id_t] outstreams: outstreams
   * @return void:
   */
  void handle_receive(
      bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream,
      sctp_stream_id_t instreams, sctp_stream_id_t outstreams);

  /*
   * Handle new SCTP TNL Association (clause 8.7.1.1, 3gpp ts38.413)
   * @param [sctp_assoc_id_t] assoc_id: gNB association ID
   * @param [sctp_stream_id_t] instreams: instreams
   * @param [sctp_stream_id_t] outstreams: outstreams
   * @return void:
   */
  void handle_sctp_new_association(
      sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams,
      sctp_stream_id_t outstreams);

  /*
   * Handle SCTP Shutdown message
   * @param [sctp_assoc_id_t] assoc_id: gNB association ID
   * @return void:
   */
  void handle_sctp_shutdown(sctp_assoc_id_t assoc_id);

  /*
   * Verify whether an association id associated with a GNB context exist
   * @param [const sctp_assoc_id_t&] assoc_id: gNB association ID
   * @return true if exist, otherwise return false
   */
  bool is_assoc_id_2_gnb_context(const sctp_assoc_id_t& assoc_id) const;

  /*
   *  Get the gNB Context associated with an association Id if exist
   * @param [const sctp_assoc_id_t&] assoc_id: gNB association ID
   * @param [std::shared_ptr<gnb_context>&] gc: store the gNB context if exist
   * @return true if the context exists and not null, otherwise return false
   */
  bool assoc_id_2_gnb_context(
      const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context>& gc);

  /*
   *  Get all gNB Assoc Ids
   * @return true if the context exists and not null, otherwise return false
   */
  std::vector<sctp_assoc_id_t> get_all_assoc_ids();

  /*
   * Store gNB Context associated with an association id
   * @param [const sctp_assoc_id_t&] assoc_id: gNB association ID
   * @param [std::shared_ptr<gnb_context>&] gc: pointer to the gNB context
   * @return void
   */
  void set_assoc_id_2_gnb_context(
      const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context> gc);

  /*
   * Verify whether a GNB context associated with a gNB ID exist
   * @param [const long&] gnb_id: gNB ID
   * @return true if exist, otherwise return false
   */
  bool is_gnb_id_2_gnb_context(const long& gnb_id) const;

  /*
   * Store gNB Context associated with an association id
   * @param [const long&] gnb_id: gNB ID
   * @param [const std::shared_ptr<gnb_context>&] gc: pointer to the gNB context
   * @return void
   */
  void set_gnb_id_2_gnb_context(
      const long& gnb_id, const std::shared_ptr<gnb_context>& gc);

  /*
   * Get the gNB Context associated with a gNB id if exits and not null
   * @param [const long& ] gnb_id: gNB ID
   * @param [const std::shared_ptr<gnb_context>&] gc: pointer to the gNB context
   * @return true if the context exists and not null, otherwise return false
   */
  bool gnb_id_2_gnb_context(
      const long& gnb_id, std::shared_ptr<gnb_context>& gc) const;

  /*
   * Remove the gNB Context associated with a gNB id
   * @param [const long& ] gnb_id: gNB ID
   * @return void
   */
  void remove_gnb_context(const long& gnb_id);

 protected:
  sctp_server sctp_s_38412;
  uint32_t ppid_;
  std::map<sctp_assoc_id_t, std::shared_ptr<gnb_context>> assoc2gnb_context;
  mutable std::shared_mutex m_assoc2gnb_context;
  std::map<long, std::shared_ptr<gnb_context>> gnbid2gnb_context;
  mutable std::shared_mutex m_gnbid2gnb_context;
};

}  // namespace oai::ngap

#endif
