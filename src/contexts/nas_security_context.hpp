/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NAS_SECURITY_CONTEXT_H_
#define _NAS_SECURITY_CONTEXT_H_

#include <stdint.h>

#define AUTH_KNAS_INT_SIZE 16 /* NAS integrity key     */
#define AUTH_KNAS_ENC_SIZE 16 /* NAS cyphering key     */
#define NGKSI_MAX_VALUE 6

/* Type of security context */
typedef enum {
  SECURITY_CTX_TYPE_NOT_AVAILABLE = 0,
  SECURITY_CTX_TYPE_PARTIAL_NATIVE,
  SECURITY_CTX_TYPE_FULL_NATIVE,
  SECURITY_CTX_TYPE_MAPPED
} nas_sc_type_t;

/*
 Internal data used for security mode control procedure
 */
typedef uint8_t ngksi_t;

typedef struct {
  uint32_t spare : 8;
  uint32_t overflow : 16;
  uint32_t seq_num : 8;
} count_t;

typedef struct {
  uint8_t _5gs_encryption;
  uint8_t _5gs_integrity;
} capability_t;

typedef struct {
  uint8_t encryption : 4;
  uint8_t integrity : 4;
} selected_algs;

class nas_secu_ctx {
 public:
  int vector_pointer;
  nas_sc_type_t sc_type;
  ngksi_t ngksi;
  uint8_t knas_enc[AUTH_KNAS_ENC_SIZE];
  uint8_t knas_int[AUTH_KNAS_INT_SIZE];
  count_t dl_count;
  count_t ul_count;
  bool ul_count_valid = false;
  capability_t ue_algorithms;
  selected_algs nas_algs;
};

#endif
