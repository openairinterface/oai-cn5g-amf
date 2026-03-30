/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NAS_ALGORITHMS_H_
#define _NAS_ALGORITHMS_H_

#include <math.h>
#include <nettle/aes.h>
#include <nettle/ctr.h>
#include <nettle/nettle-meta.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/cmac.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "authentication_algorithms_with_5gaka.hpp"
extern "C" {
#include "conversions.h"
#include "snow3g.h"
}

#define SECU_DIRECTION_UPLINK 0
#define SECU_DIRECTION_DOWNLINK 1

#define derive_key_nas_enc(aLGiD, kseaf, kNAS)                                 \
  Authentication_5gaka::derive_knas(NAS_ENC_ALG, aLGiD, kseaf, kNAS)

#define derive_key_nas_int(aLGiD, kseaf, kNAS)                                 \
  Authentication_5gaka::derive_knas(NAS_INT_ALG, aLGiD, kseaf, kNAS)

#define derive_key_rrc_enc(aLGiD, kseaf, kNAS)                                 \
  Authentication_5gaka::derive_knas(RRC_ENC_ALG, aLGiD, kseaf, kNAS)

#define derive_key_rrc_int(aLGiD, kseaf, kNAS)                                 \
  Authentication_5gaka::derive_knas(RRC_INT_ALG, aLGiD, kseaf, kNAS)

#define derive_key_up_enc(aLGiD, kseaf, kNAS)                                  \
  Authentication_5gaka::derive_knas(UP_ENC_ALG, aLGiD, kseaf, kNAS)

#define derive_key_up_int(aLGiD, kseaf, kNAS)                                  \
  Authentication_5gaka::derive_knas(UP_INT_ALG, aLGiD, kseaf, kNAS)

typedef struct {
  uint8_t* key;
  uint32_t key_length;
  uint32_t count;
  uint8_t bearer;
  uint8_t direction;
  uint8_t* message;
  /* length in bits */
  uint32_t blength;
} nas_stream_cipher_t;

class nas_algorithms {
 public:
  static int nas_stream_encrypt_nea1(
      nas_stream_cipher_t* const stream_cipher, uint8_t* const out);
  static int nas_stream_encrypt_nia1(
      nas_stream_cipher_t* const stream_cipher, uint8_t const out[4]);
  static int nas_stream_encrypt_nea2(
      nas_stream_cipher_t* const stream_cipher, uint8_t* const out);
  static int nas_stream_encrypt_nia2(
      nas_stream_cipher_t* const stream_cipher, uint8_t const out[4]);
};

#endif
