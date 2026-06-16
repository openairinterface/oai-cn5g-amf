/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef SRC_ITTI_ITTI_DISPATCH_HPP_INCLUDED_
#define SRC_ITTI_ITTI_DISPATCH_HPP_INCLUDED_

#include "itti_msg.hpp"
#include "logger.hpp"

// Cast an itti_msg base pointer to the expected concrete message type, then
// invoke the handler with a reference to it.
template<typename MsgT, typename Handler>
void itti_dispatch(itti_msg* msg, Handler&& h) {
  auto* m = dynamic_cast<MsgT*>(msg);
  if (!m) {
    Logger::itti().error(
        "itti_dispatch: dynamic_cast failed for msg type %d",
        msg ? msg->msg_type : -1);
    return;
  }
  h(*m);
}

#endif  // SRC_ITTI_ITTI_DISPATCH_HPP_INCLUDED_
