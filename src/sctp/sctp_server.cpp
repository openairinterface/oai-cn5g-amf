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

#include "sctp_server.hpp"

#include "logger.hpp"
#include "utils.hpp"

extern "C" {
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "bstrlib.h"
}

namespace sctp {

pthread_t tmp_thread;

//------------------------------------------------------------------------------
sctp_application::~sctp_application() {}

//------------------------------------------------------------------------------
sctp_server::sctp_server(const char* address, const uint16_t port_num) {
  Logger::sctp().debug("Creating socket!");
  create_socket(address, port_num);
  app_         = nullptr;
  sctp_desc_   = {};
  server_addr_ = {};
  events_      = {};
  sctp_ctx_    = {};
}

//------------------------------------------------------------------------------
sctp_server::~sctp_server() {
  int res;

  res = pthread_cancel(tmp_thread);
  if (res != 0) {
    Logger::sctp().error(
        "pthread_cancel on sctp_receiver_thread failed %s", strerror(errno));
  }

  res = shutdown(socket_, SHUT_RDWR);
  if (res != 0) {
    Logger::sctp().error("shutdown on socket_ failed %s", strerror(errno));
  }
  res = close(socket_);
  if (res != 0) {
    Logger::sctp().error("close on socket_ failed %s", strerror(errno));
  }
  Logger::sctp().debug("Thread on sctp_receiver_thread should have ended!");
}

//------------------------------------------------------------------------------
int sctp_server::create_socket(const char* address, const uint16_t port_num) {
  struct addrinfo* res;
  if (getaddrinfo(address, 0, NULL, &res) < 0) {
    Logger::sctp().error(
        "Getaddrinfo on %s: %s:%d", address, strerror(errno), errno);
    return RETURNerror;
  } else {
    Logger::sctp().debug("Getaddrinfo on %s was OK", address);
  }
  if ((socket_ = socket(res->ai_family, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
    Logger::sctp().error("Socket: %s:%d", strerror(errno), errno);
    return RETURNerror;
  }
  Logger::sctp().info("Created socket (%d)", socket_);
  bzero(&server_addr_, sizeof(server_addr_));
  server_addr_.sin_family      = res->ai_family;
  server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr_.sin_port        = htons(port_num);
  inet_pton(AF_INET, address, &server_addr_.sin_addr);
  if (bind(socket_, (struct sockaddr*) &server_addr_, sizeof(server_addr_)) !=
      0) {
    Logger::sctp().error("Socket bind: %s:%d", strerror(errno), errno);
    return RETURNerror;
  }
  bzero(&events_, sizeof(events_));
  events_.sctp_data_io_event     = 1;
  events_.sctp_shutdown_event    = 1;
  events_.sctp_association_event = 1;
  // TODO:
  // events_.sctp_send_failure_event = 1;
  // events_.sctp_partial_delivery_event = 1;
  // events_.sctp_address_event = 1;
  // events_.sctp_peer_error_event = 1;

  // HACK on the sizeof(events_)!
  // it SHALL be equal to the minimal value across OS version
  // socklen_t len = (socklen_t) sizeof(events_);
  // - Ubuntu-20    host: 13 bytes
  // - Ubuntu-22    host: 14 bytes
  // - RHEL8/Rocky8 host: 14 bytes
  // - RHEL9/Rocky9 host: 14 bytes
  // 12 is chosen as minimal value.
  setsockopt(socket_, IPPROTO_SCTP, SCTP_EVENTS, &events_, 12);
  listen(socket_, 5);  // the queue length for completely established sockets
                       // waiting to be accepted
  // deallocation
  freeaddrinfo((struct addrinfo*) res);
  return RETURNok;
}

//------------------------------------------------------------------------------
void sctp_server::start_receive(sctp_application* app) {
  app_ = app;
  pthread_create(&thread_, NULL, sctp_receiver_thread, (void*) this);
}

//------------------------------------------------------------------------------
void* sctp_server::sctp_receiver_thread(void* arg) {
  sctp_server* ptr = (sctp_server*) arg;
  Logger::sctp().info("Create pthread to receive SCTP message");
  int fdmax;
  int clientsock;
  fd_set master;
  fd_set read_fds;

  if (arg == NULL) pthread_exit(NULL);
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  FD_SET(ptr->get_socket(), &master);
  fdmax      = ptr->get_socket();
  tmp_thread = pthread_self();

  while (true) {
    memcpy(&read_fds, &master, sizeof(master));
    if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
      Logger::sctp().error(
          "[socket(%d)] Select() error: %s:%d", ptr->get_socket(),
          strerror(errno), errno);
      pthread_exit(NULL);
    }
    for (int i = 0; i <= fdmax; i++) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == ptr->get_socket()) {
          if ((clientsock = accept(ptr->get_socket(), NULL, NULL)) < 0) {
            Logger::sctp().error(
                "[socket(%d)] Accept() error: %s:%d", ptr->get_socket(),
                strerror(errno), errno);
            pthread_exit(NULL);
          } else {
            FD_SET(clientsock, &master);
            if (clientsock > fdmax) fdmax = clientsock;
          }
        } else {
          int ret = ptr->sctp_read_from_socket(i, ptr->app_->get_ppid());
          if (ret == SCTP_RC_DISCONNECT) {
            FD_CLR(i, &master);
            if (i == fdmax) {
              while (FD_ISSET(fdmax, &master) == false) fdmax -= 1;
            }
          }
        }
      }
    }
  }
  return NULL;
}

//------------------------------------------------------------------------------
int sctp_server::get_socket() {
  return socket_;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_read_from_socket(int sd, uint32_t ppid) {
  int flags                    = 0;
  socklen_t from_len           = 0;
  struct sctp_sndrcvinfo sinfo = {0};
  struct sockaddr_in6 addr     = {0};
  uint8_t buffer[SCTP_RECV_BUFFER_SIZE];

  if (sd < 0) return RETURNerror;
  memset((void*) &addr, 0, sizeof(struct sockaddr_in6));
  from_len = (socklen_t) sizeof(struct sockaddr_in6);
  memset((void*) &sinfo, 0, sizeof(struct sctp_sndrcvinfo));

  int n = sctp_recvmsg(
      sd, (void*) buffer, SCTP_RECV_BUFFER_SIZE, (struct sockaddr*) &addr,
      &from_len, &sinfo, &flags);

  if (n < 0) {
    Logger::sctp().error("sctp_recvmsg error:: %s:%d", strerror(errno), errno);
    return SCTP_RC_ERROR;
  }

  if (flags & MSG_NOTIFICATION) {
    union sctp_notification* snp = (union sctp_notification*) buffer;

    switch (snp->sn_header.sn_type) {
      case SCTP_SHUTDOWN_EVENT: {
        Logger::sctp().debug("SCTP Shutdown Event received");
        return sctp_handle_com_down(snp->sn_shutdown_event.sse_assoc_id);
        break;
      }
      case SCTP_ASSOC_CHANGE: {
        Logger::sctp().debug("SCTP Association Change event received");
        return handle_assoc_change(sd, ppid, &snp->sn_assoc_change);
        break;
      }
      default: {
        Logger::sctp().error(
            "Unhandled notification type (%d)", snp->sn_header.sn_type);
        break;
      }
    }
  } else {
    sctp_association_t* association;
    if ((association = sctp_is_assoc_in_list(
             (sctp_assoc_id_t) sinfo.sinfo_assoc_id)) == NULL) {
      return SCTP_RC_ERROR;
    }
    association->messages_recv++;

    if (ntohl(sinfo.sinfo_ppid) != association->ppid) {
      Logger::sctp().error(
          "Received data from peer with unsolicited PPID (%d), expecting (%d)",
          ntohl(sinfo.sinfo_ppid), association->ppid);
      return SCTP_RC_ERROR;
    }

    Logger::sctp().info(
        "[Assoc_id %d, Socket %d] Received a message (length %d) from port %d, "
        "on stream %d, PPID %d",
        sinfo.sinfo_assoc_id, sd, n, ntohs(addr.sin6_port), sinfo.sinfo_stream,
        ntohl(sinfo.sinfo_ppid));

    bstring payload = blk2bstr(buffer, n);
    // Handle payload
    app_->handle_receive(
        payload, (sctp_assoc_id_t) sinfo.sinfo_assoc_id, sinfo.sinfo_stream,
        association->instreams, association->outstreams);
    oai::utils::utils::bdestroy_wrapper(&payload);
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_handle_com_down(sctp_assoc_id_t assoc_id) {
  app_->handle_sctp_shutdown(assoc_id);
  return SCTP_RC_DISCONNECT;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_handle_reset(const sctp_assoc_id_t assoc_id) {
  return RETURNok;
}

//------------------------------------------------------------------------------
int sctp_server::handle_assoc_change(
    int sd, uint32_t ppid, struct sctp_assoc_change* sctp_assoc_changed) {
  int rc = SCTP_RC_NORMAL_READ;

  switch (sctp_assoc_changed->sac_state) {
    case SCTP_COMM_UP: {
      if (add_new_association(sd, ppid, sctp_assoc_changed) == NULL) {
        Logger::sctp().error(
            "Add new association with PPID (%d) socket (%d) error", ppid, sd);
        rc = SCTP_RC_ERROR;
      }
      break;
    }
    case SCTP_RESTART: {
      if (sctp_is_assoc_in_list(
              (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id) != NULL) {
        rc = sctp_handle_reset(
            (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);
      }
      break;
    }
    case SCTP_COMM_LOST:
    case SCTP_SHUTDOWN_COMP:
    case SCTP_CANT_STR_ASSOC: {
      if (sctp_is_assoc_in_list(
              (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id) != NULL) {
        rc = sctp_handle_com_down(
            (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);
      }
      break;
    }
    default:
      Logger::sctp().error(
          "Unhandled SCTP message (%d)", sctp_assoc_changed->sac_state);
      break;
  }

  return rc;
}

//------------------------------------------------------------------------------
sctp_association_t* sctp_server::add_new_association(
    int sd, uint32_t ppid, struct sctp_assoc_change* sctp_assoc_changed) {
  sctp_association_t* new_association = NULL;
  new_association = (sctp_association_t*) calloc(1, sizeof(sctp_association_t));
  new_association->sd         = sd;
  new_association->ppid       = ppid;
  new_association->instreams  = sctp_assoc_changed->sac_inbound_streams;
  new_association->outstreams = sctp_assoc_changed->sac_outbound_streams;
  new_association->assoc_id =
      (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id;
  Logger::sctp().debug(
      "Add new association with Id (%d)",
      (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);

  sctp_ctx_.push_back(new_association);

  sctp_get_local_addresses(sd, NULL, NULL);
  sctp_get_peer_addresses(
      sd, &new_association->peer_addresses,
      &new_association->nb_peer_addresses);
  app_->handle_sctp_new_association(
      new_association->assoc_id, new_association->instreams,
      new_association->outstreams);

  return new_association;
}

//------------------------------------------------------------------------------
sctp_association_t* sctp_server::sctp_is_assoc_in_list(
    sctp_assoc_id_t assoc_id) {
  sctp_association_t* assoc_desc = NULL;

  if (assoc_id < 0) {
    return NULL;
  }

  for (int i = 0; i < sctp_ctx_.size(); i++) {
    if (sctp_ctx_[i]->assoc_id == assoc_id) {
      return sctp_ctx_[i];
    }
  }

  return assoc_desc;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_get_peer_addresses(
    int sock, struct sockaddr** remote_addr, int* nb_remote_addresses) {
  int nb;
  struct sockaddr* temp_addr_p = NULL;

  if ((nb = sctp_getpaddrs(sock, -1, &temp_addr_p)) <= 0) {
    Logger::sctp().error("Failed to retrieve peer addresses");
    return RETURNerror;
  }

  Logger::sctp().info("----------------------");
  Logger::sctp().info("Peer addresses: ");

  for (int j = 0; j < nb; j++) {
    if (temp_addr_p[j].sa_family == AF_INET) {
      char address[16]         = {0};
      struct sockaddr_in* addr = NULL;
      addr                     = (struct sockaddr_in*) &temp_addr_p[j];
      if (inet_ntop(AF_INET, &addr->sin_addr, address, sizeof(address)) !=
          NULL) {
        Logger::sctp().info("    - IPv4 Addr: %s", address);
      }
    } else {
      struct sockaddr_in6* addr = NULL;
      char address[40]          = {0};
      addr                      = (struct sockaddr_in6*) &temp_addr_p[j];
      if (inet_ntop(
              AF_INET6, &addr->sin6_addr.s6_addr, address, sizeof(address)) !=
          NULL) {
        Logger::sctp().info("    - IPv6 Addr: %s", address);
      }
    }
  }

  Logger::sctp().info("----------------------");

  if (remote_addr != NULL && nb_remote_addresses != NULL) {
    *nb_remote_addresses = nb;
    *remote_addr         = temp_addr_p;
  } else {
    sctp_freepaddrs((struct sockaddr*) temp_addr_p);
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_get_local_addresses(
    int sock, struct sockaddr** local_addr, int* nb_local_addresses) {
  int nb                       = 0;
  struct sockaddr* temp_addr_p = NULL;
  if ((nb = sctp_getladdrs(sock, -1, &temp_addr_p)) <= 0) {
    Logger::sctp().error("Failed to retrieve local addresses");
    return RETURNerror;
  }

  if (temp_addr_p) {
    Logger::sctp().info("----------------------");
    Logger::sctp().info("Local addresses: ");
    for (int j = 0; j < nb; j++) {
      if (temp_addr_p[j].sa_family == AF_INET) {
        char address[16]         = {0};
        struct sockaddr_in* addr = NULL;
        addr                     = (struct sockaddr_in*) &temp_addr_p[j];
        if (inet_ntop(AF_INET, &addr->sin_addr, address, sizeof(address)) !=
            NULL) {
          Logger::sctp().info("    - IPv4 Addr: %s", address);
        }
      } else if (temp_addr_p[j].sa_family == AF_INET6) {
        struct sockaddr_in6* addr = NULL;
        char address[40]          = {0};
        addr                      = (struct sockaddr_in6*) &temp_addr_p[j];
        if (inet_ntop(
                AF_INET6, &addr->sin6_addr.s6_addr, address, sizeof(address)) !=
            NULL) {
          Logger::sctp().info("    - Ipv6 Addr: %s", address);
        }
      } else {
        Logger::sctp().error(
            "    - Unknown address family %d", temp_addr_p[j].sa_family);
      }
    }

    if (local_addr != NULL && nb_local_addresses != NULL) {
      *nb_local_addresses = nb;
      *local_addr         = temp_addr_p;
    } else {
      sctp_freeladdrs((struct sockaddr*) temp_addr_p);
    }
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_send_msg(
    sctp_assoc_id_t sctp_assoc_id, sctp_stream_id_t stream, bstring* payload) {
  sctp_association_t* assoc_desc = NULL;
  if ((assoc_desc = sctp_is_assoc_in_list(sctp_assoc_id)) == NULL) {
    Logger::sctp().error(
        "This association Id (%d) has not been found in the association list",
        sctp_assoc_id);
    return RETURNerror;
  }
  if (assoc_desc->sd == -1) {
    Logger::sctp().error(
        "The socket is invalid (may be closed, assoc id %d)", sctp_assoc_id);
    return RETURNerror;
  }
  Logger::sctp().debug(
      "[Socket %d, Assoc ID %d] Sending buffer %p of %d bytes on stream %d "
      "with PPID %d",
      assoc_desc->sd, sctp_assoc_id, bdata(*payload), blength(*payload), stream,
      assoc_desc->ppid);

  // Set timetolive to 100ms
  if (sctp_sendmsg(
          assoc_desc->sd, (const void*) bdata(*payload),
          (size_t) blength(*payload), NULL, 0, htonl(assoc_desc->ppid), 0,
          stream, 100, 0) < 0) {
    Logger::sctp().error(
        "[Socket %d] Send stream %u, PPID %u, len %u failed (%s, %d)",
        assoc_desc->sd, stream, htonl(assoc_desc->ppid), blength(*payload),
        strerror(errno), errno);
    //*payload = NULL;
    return RETURNerror;
  }
  Logger::sctp().debug(
      "Successfully sent %d bytes on stream %d", blength(*payload), stream);
  //*payload = NULL;
  assoc_desc->messages_sent++;
  return RETURNok;
}

}  // namespace sctp
