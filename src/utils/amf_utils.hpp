/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_AMF_UTILS_SEEN
#define FILE_AMF_UTILS_SEEN

namespace oai::amf::utils {

static bool compare_buffer(const uint8_t* a, const uint8_t* b, size_t len) {
  uint8_t diff = 0;
  for (size_t i = 0; i < len; i++) {
    diff |= (uint8_t) (a[i] ^ b[i]);
  }
  return diff == 0;
}
}  // namespace oai::amf::utils

#endif /* FILE_AMF_UTILS_SEEN */
