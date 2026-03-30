/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef FILE_3GPP_29_503_SEEN
#define FILE_3GPP_29_503_SEEN

#include <vector>

#include "3gpp_23.003.h"

typedef struct nssai_s {
  std::vector<snssai_t> default_single_nssais;
  std::vector<snssai_t> single_nssais;
} nssai_t;

#endif
