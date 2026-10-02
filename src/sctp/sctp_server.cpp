/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "sctp_server.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <algorithm>
#include <chrono>

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
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "bstrlib.h"
}

namespace sctp {

pthread_t tmp_thread;

namespace {
constexpr auto kAcceptRetryDelay             = std::chrono::seconds(1);
constexpr rlim_t kMaxReservedFileDescriptors = 128;

bool is_transient_accept_error(int error) {
  return error == EINTR || error == EAGAIN || error == EWOULDBLOCK ||
         error == ECONNABORTED || error == EPROTO || error == ENETDOWN ||
         error == ENOPROTOOPT || error == EHOSTDOWN || error == ENONET ||
         error == EHOSTUNREACH || error == EOPNOTSUPP || error == ENETUNREACH;
}

bool is_resource_accept_error(int error) {
  return error == EMFILE || error == ENFILE || error == ENOBUFS ||
         error == ENOMEM;
}

bool is_unrecoverable_accept_error(int error) {
  return error == EBADF || error == EFAULT || error == EINVAL ||
         error == ENOTSOCK;
}
}  // namespace

//------------------------------------------------------------------------------
sctp_application::~sctp_application() {}

//------------------------------------------------------------------------------
sctp_server::sctp_server(const char* address, const uint16_t port_num) {
  Logger::sctp().debug("Creating socket!");
  create_socket(address, port_num);
  app_         = nullptr;
  server_addr_ = {};
  events_      = {};
  sctp_ctx_    = {};
  sctp_ttl     = 100;
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

  // Free all remaining associations
  {
    std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
    for (auto* association : sctp_ctx_) {
      if (association != nullptr) {
        if (association->peer_addresses != nullptr) {
          sctp_freepaddrs(association->peer_addresses);
        }
        free(association);
      }
    }
    sctp_ctx_.clear();
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

  // Set the default parameters for the SCTP association
  // Please refer to https://man7.org/linux/man-pages/man7/sctp.7.html, for more
  // info
  struct sctp_initmsg init_msg;
  init_msg.sinit_num_ostreams   = SCTP_OUT_STREAMS;
  init_msg.sinit_max_instreams  = SCTP_IN_STREAMS;
  init_msg.sinit_max_attempts   = SCTP_MAX_ATTEMPTS;
  init_msg.sinit_max_init_timeo = SCTP_TIMEOUT;

  if (setsockopt(
          socket_, IPPROTO_SCTP, SCTP_INITMSG, &init_msg, sizeof(init_msg)) <
      0) {
    Logger::sctp().error("setsockopt SCTP_INITMSG");
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
  int flag = 1;
  setsockopt(socket_, IPPROTO_SCTP, SCTP_NODELAY, &flag, sizeof(flag));
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
  bool accept_paused = false;
  std::chrono::steady_clock::time_point accept_retry_at;

  tmp_thread = pthread_self();
  if (arg == NULL) pthread_exit(NULL);
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  const int server_socket = ptr->get_socket();
  if (server_socket < 0 || server_socket >= FD_SETSIZE) {
    Logger::sctp().error(
        "SCTP server socket descriptor (%d) is outside the range supported by "
        "select() (FD_SETSIZE: %d)",
        server_socket, FD_SETSIZE);
    pthread_exit(NULL);
  }

  struct rlimit nofile_limit;
  rlim_t descriptor_ceiling = FD_SETSIZE;
  if (getrlimit(RLIMIT_NOFILE, &nofile_limit) == 0) {
    descriptor_ceiling =
        std::min<rlim_t>(descriptor_ceiling, nofile_limit.rlim_cur);
  } else {
    Logger::sctp().warn(
        "Could not read RLIMIT_NOFILE, using FD_SETSIZE as the descriptor "
        "ceiling: %s:%d",
        strerror(errno), errno);
  }
  const rlim_t reserved_descriptors =
      std::min<rlim_t>(kMaxReservedFileDescriptors, descriptor_ceiling / 4);
  const int client_fd_limit =
      static_cast<int>(descriptor_ceiling - reserved_descriptors);
  if (server_socket >= client_fd_limit) {
    Logger::sctp().error(
        "SCTP server socket descriptor (%d) leaves no safe descriptor range "
        "for clients (limit: %d)",
        server_socket, client_fd_limit);
    pthread_exit(NULL);
  }
  Logger::sctp().info(
      "SCTP client descriptors are limited to values below %d, reserving %lu "
      "descriptors for other AMF services",
      client_fd_limit, static_cast<unsigned long>(reserved_descriptors));

  FD_SET(server_socket, &master);
  fdmax = server_socket;

  while (true) {
    struct timeval accept_timeout;
    struct timeval* select_timeout = NULL;
    if (accept_paused) {
      const auto now = std::chrono::steady_clock::now();
      if (now >= accept_retry_at) {
        FD_SET(server_socket, &master);
        accept_paused = false;
      } else {
        const auto remaining =
            std::chrono::duration_cast<std::chrono::microseconds>(
                accept_retry_at - now);
        accept_timeout.tv_sec  = remaining.count() / 1000000;
        accept_timeout.tv_usec = remaining.count() % 1000000;
        select_timeout         = &accept_timeout;
      }
    }

    memcpy(&read_fds, &master, sizeof(master));
    const int ready = select(fdmax + 1, &read_fds, NULL, NULL, select_timeout);
    if (ready == -1) {
      const int select_errno = errno;
      if (select_errno == EINTR) continue;
      Logger::sctp().error(
          "[socket(%d)] Select() error: %s:%d", server_socket,
          strerror(select_errno), select_errno);
      pthread_exit(NULL);
    }
    if (ready == 0) continue;

    for (int i = 0; i <= fdmax; i++) {
      if (FD_ISSET(i, &read_fds)) {
        if (i == server_socket) {
          if ((clientsock = accept(server_socket, NULL, NULL)) < 0) {
            const int accept_errno = errno;
            if (is_transient_accept_error(accept_errno)) continue;
            if (is_resource_accept_error(accept_errno)) {
              Logger::sctp().warn(
                  "[socket(%d)] Pausing accepts after resource exhaustion: "
                  "%s:%d",
                  server_socket, strerror(accept_errno), accept_errno);
              FD_CLR(server_socket, &master);
              accept_paused = true;
              accept_retry_at =
                  std::chrono::steady_clock::now() + kAcceptRetryDelay;
              continue;
            }
            if (is_unrecoverable_accept_error(accept_errno)) {
              Logger::sctp().error(
                  "[socket(%d)] Unrecoverable accept() error: %s:%d",
                  server_socket, strerror(accept_errno), accept_errno);
              pthread_exit(NULL);
            }
            Logger::sctp().warn(
                "[socket(%d)] Pausing accepts after unexpected error: %s:%d",
                server_socket, strerror(accept_errno), accept_errno);
            FD_CLR(server_socket, &master);
            accept_paused = true;
            accept_retry_at =
                std::chrono::steady_clock::now() + kAcceptRetryDelay;
            continue;
          } else {
            if (clientsock >= client_fd_limit) {
              Logger::sctp().warn(
                  "Rejecting SCTP socket descriptor (%d): client descriptor "
                  "limit is %d",
                  clientsock, client_fd_limit);
              if (close(clientsock) != 0) {
                Logger::sctp().error(
                    "[socket(%d)] Close error: %s:%d", clientsock,
                    strerror(errno), errno);
              }
              FD_CLR(server_socket, &master);
              accept_paused = true;
              accept_retry_at =
                  std::chrono::steady_clock::now() + kAcceptRetryDelay;
              continue;
            }
            FD_SET(clientsock, &master);
            if (clientsock > fdmax) fdmax = clientsock;
          }
        } else {
          int ret = ptr->sctp_read_from_socket(i, ptr->app_->get_ppid());
          if (ret == SCTP_RC_DISCONNECT) {
            FD_CLR(i, &master);
            if (close(i) != 0) {
              Logger::sctp().error(
                  "[socket(%d)] Close error: %s:%d", i, strerror(errno), errno);
            }
            if (accept_paused) {
              FD_SET(server_socket, &master);
              accept_paused = false;
            }
            if (i == fdmax) {
              while (fdmax > server_socket && !FD_ISSET(fdmax, &master)) {
                fdmax -= 1;
              }
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
    const int receive_errno = errno;
    if (receive_errno == EINTR || receive_errno == EAGAIN ||
        receive_errno == EWOULDBLOCK || receive_errno == ENOMEM ||
        receive_errno == ENOBUFS) {
      return SCTP_RC_NORMAL_READ;
    }
    Logger::sctp().error(
        "[socket(%d)] sctp_recvmsg error: %s:%d", sd, strerror(receive_errno),
        receive_errno);
    return sctp_handle_socket_down(sd);
  }

  if (n == 0) {
    Logger::sctp().debug("[socket(%d)] SCTP peer closed the connection", sd);
    return sctp_handle_socket_down(sd);
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
  Logger::sctp().debug(
      "Handling disconnection of the association with Id (%d)", assoc_id);
  // Remove the association from the list and free its resources first,
  // so that no message can be sent on this association anymore
  remove_association(assoc_id);
  app_->handle_sctp_shutdown(assoc_id);
  return SCTP_RC_DISCONNECT;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_handle_socket_down(int sd) {
  sctp_assoc_id_t assoc_id = 0;
  bool association_found   = false;
  {
    std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
    for (const auto* association : sctp_ctx_) {
      if (association != nullptr && association->sd == sd) {
        assoc_id          = association->assoc_id;
        association_found = true;
        break;
      }
    }
  }

  if (association_found) return sctp_handle_com_down(assoc_id);
  return SCTP_RC_DISCONNECT;
}

//------------------------------------------------------------------------------
int sctp_server::sctp_handle_reset(
    int sd, uint32_t ppid, struct sctp_assoc_change* sctp_assoc_changed) {
  sctp_assoc_id_t assoc_id = (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id;
  Logger::sctp().debug(
      "Handling SCTP Restart for the association with Id (%d) as down + up",
      assoc_id);

  // Down: clean up the old association (if known) and notify the application
  if (sctp_is_assoc_in_list(assoc_id) != NULL) {
    remove_association(assoc_id);
    app_->handle_sctp_shutdown(assoc_id);
  }

  // Up: re-add the association
  if (add_new_association(sd, ppid, sctp_assoc_changed) == NULL) {
    Logger::sctp().error(
        "Re-add association with Id (%d) after SCTP Restart error", assoc_id);
    return SCTP_RC_ERROR;
  }
  return SCTP_RC_NORMAL_READ;
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
      rc = sctp_handle_reset(sd, ppid, sctp_assoc_changed);
      break;
    }
    case SCTP_COMM_LOST:
    case SCTP_SHUTDOWN_COMP:
    case SCTP_CANT_STR_ASSOC: {
      if (sctp_is_assoc_in_list(
              (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id) != NULL) {
        rc = sctp_handle_com_down(
            (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);
      } else {
        // The accepted socket still needs to be removed from select() and
        // closed when association setup did not complete successfully.
        rc = SCTP_RC_DISCONNECT;
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
  if (new_association == NULL) {
    Logger::sctp().error(
        "Failed to allocate memory for the new association with Id (%d)",
        (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);
    return NULL;
  }
  new_association->sd         = sd;
  new_association->ppid       = ppid;
  new_association->instreams  = sctp_assoc_changed->sac_inbound_streams;
  new_association->outstreams = sctp_assoc_changed->sac_outbound_streams;
  new_association->assoc_id =
      (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id;
  Logger::sctp().debug(
      "Add new association with Id (%d)",
      (sctp_assoc_id_t) sctp_assoc_changed->sac_assoc_id);

  sctp_get_local_addresses(sd, NULL, NULL);
  if (sctp_get_peer_addresses(
          sd, &new_association->peer_addresses,
          &new_association->nb_peer_addresses) != RETURNok) {
    Logger::sctp().error(
        "Failed to get peer addresses for the new association with Id (%d)",
        new_association->assoc_id);
    free(new_association);
    return NULL;
  }

  {
    std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
    sctp_ctx_.push_back(new_association);
  }

  app_->handle_sctp_new_association(
      new_association->assoc_id, new_association->instreams,
      new_association->outstreams);

  return new_association;
}

//------------------------------------------------------------------------------
sctp_association_t* sctp_server::sctp_find_assoc_locked(
    sctp_assoc_id_t assoc_id) {
  for (size_t i = 0; i < sctp_ctx_.size(); i++) {
    if ((sctp_ctx_[i] != nullptr) && (sctp_ctx_[i]->assoc_id == assoc_id)) {
      return sctp_ctx_[i];
    }
  }
  return NULL;
}

//------------------------------------------------------------------------------
sctp_association_t* sctp_server::sctp_is_assoc_in_list(
    sctp_assoc_id_t assoc_id) {
  std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
  return sctp_find_assoc_locked(assoc_id);
}

//------------------------------------------------------------------------------
void sctp_server::remove_association(sctp_assoc_id_t assoc_id) {
  std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
  for (auto it = sctp_ctx_.begin(); it != sctp_ctx_.end(); ++it) {
    if ((*it != nullptr) && ((*it)->assoc_id == assoc_id)) {
      if ((*it)->peer_addresses != nullptr) {
        sctp_freepaddrs((*it)->peer_addresses);
      }
      free(*it);
      sctp_ctx_.erase(it);
      Logger::sctp().debug("Removed the association with Id (%d)", assoc_id);
      return;
    }
  }
  Logger::sctp().debug(
      "The association with Id (%d) is not in the association list", assoc_id);
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
  // Duplicate the socket under the association lock. The receiver thread may
  // remove the association and close its descriptor while sctp_sendmsg() is in
  // progress; the duplicate keeps the underlying socket alive and cannot be
  // confused with a subsequently reused descriptor number.
  int sd        = -1;
  int send_sd   = -1;
  uint32_t ppid = 0;
  {
    std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
    sctp_association_t* assoc_desc = sctp_find_assoc_locked(sctp_assoc_id);
    if (assoc_desc == NULL) {
      Logger::sctp().error(
          "This association Id (%d) has not been found in the association "
          "list",
          sctp_assoc_id);
      return RETURNerror;
    }
    if (assoc_desc->sd == -1) {
      Logger::sctp().error(
          "The socket is invalid (may be closed, assoc id %d)", sctp_assoc_id);
      return RETURNerror;
    }
    sd      = assoc_desc->sd;
    ppid    = assoc_desc->ppid;
    send_sd = dup(sd);
    if (send_sd == -1) {
      Logger::sctp().error(
          "Could not duplicate socket for association Id (%d): %s:%d",
          sctp_assoc_id, strerror(errno), errno);
      return RETURNerror;
    }
  }

  Logger::sctp().debug(
      "[Socket %d, Assoc ID %d] Sending buffer %p of %d bytes on stream %d "
      "with PPID %d",
      sd, sctp_assoc_id, bdata(*payload), blength(*payload), stream, ppid);

  // Set timetolive to 500ms
  if (sctp_sendmsg(
          send_sd, (const void*) bdata(*payload), (size_t) blength(*payload),
          NULL, 0, htonl(ppid), 0, stream, this->sctp_ttl, 0) < 0) {
    const int send_errno = errno;
    close(send_sd);
    Logger::sctp().error(
        "[Socket %d] Send stream %u, PPID %u, len %u failed (%s, %d)", sd,
        stream, htonl(ppid), blength(*payload), strerror(send_errno),
        send_errno);
    //*payload = NULL;
    return RETURNerror;
  }
  if (close(send_sd) != 0) {
    Logger::sctp().error(
        "[socket(%d)] Close duplicate error: %s:%d", send_sd, strerror(errno),
        errno);
  }
  Logger::sctp().debug(
      "Successfully sent %d bytes on stream %d", blength(*payload), stream);
  //*payload = NULL;
  {
    std::lock_guard<std::mutex> lock(sctp_ctx_mutex_);
    sctp_association_t* assoc_desc = sctp_find_assoc_locked(sctp_assoc_id);
    if (assoc_desc != NULL) assoc_desc->messages_sent++;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void sctp_server::sctp_set_ttl(uint32_t sctp_ttl) {
  this->sctp_ttl = sctp_ttl;
}

}  // namespace sctp
