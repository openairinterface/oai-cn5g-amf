/* From https://gist.github.com/javiermon/6272065#file-gateway_netlink-c */
/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "if.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common_defs.h"
#include "logger.hpp"

#define BUFFER_SIZE 4096

//------------------------------------------------------------------------------
int get_inet_addr_infos_from_iface(
    const std::string& if_name, struct in_addr& inet_addr,
    struct in_addr& inet_network, unsigned int& mtu) {
  struct ifreq ifr;
  char str[INET_ADDRSTRLEN];

  inet_addr.s_addr    = INADDR_ANY;
  inet_network.s_addr = INADDR_ANY;
  mtu                 = 0;

  memset(&ifr, 0, sizeof(ifr));
  int fd                 = socket(AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  // strncpy(ifr.ifr_name, (const char *)if_name.c_str(), IFNAMSIZ-1);
  strcpy(ifr.ifr_name, (const char*) if_name.c_str());
  if (ioctl(fd, SIOCGIFADDR, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s inet addr: error %s\n", if_name.c_str(),
        strerror(errno));
    return RETURNerror;
  }
  struct sockaddr_in* ipaddr = (struct sockaddr_in*) &ifr.ifr_addr;
  // check
  if (inet_ntop(
          AF_INET, (const void*) &ipaddr->sin_addr, str, INET_ADDRSTRLEN) ==
      NULL) {
    close(fd);
    return RETURNerror;
  }
  inet_addr.s_addr = ipaddr->sin_addr.s_addr;

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFNETMASK, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s inet netmask: error %s\n", if_name.c_str(),
        strerror(errno));
    return RETURNerror;
  }
  ipaddr = (struct sockaddr_in*) &ifr.ifr_netmask;
  // check
  if (inet_ntop(
          AF_INET, (const void*) &ipaddr->sin_addr, str, INET_ADDRSTRLEN) ==
      NULL) {
    close(fd);
    return RETURNerror;
  }
  inet_network.s_addr = ipaddr->sin_addr.s_addr;

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFMTU, &ifr)) {
    Logger::system().error(
        "Failed to probe %s MTU: error %s\n", if_name.c_str(), strerror(errno));
  } else {
    mtu = ifr.ifr_mtu;
  }
  close(fd);
  return RETURNok;
}
