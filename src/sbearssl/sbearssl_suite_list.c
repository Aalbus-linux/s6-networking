/* ISC license. */

/* Copied from Michael Forney's libtls-bearssl */

#include "sbearssl-internal.h"

static sbearssl_suiteinfo const sbearssl_suite_list_[] =
{
  {
    "ECDHE-ECDSA-CHACHA20-POLY1305",
    BR_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256,
    ECDHE|ECDSA|CHACHA20|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-RSA-CHACHA20-POLY1305",
    BR_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256,
    ECDHE|aRSA|CHACHA20|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-AES128-GCM-SHA256",
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
    ECDHE|ECDSA|AES128|AESGCM|AEAD|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-RSA-AES128-GCM-SHA256",
    BR_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
    ECDHE|aRSA|AES128|AESGCM|AEAD|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-ECDSA-AES256-GCM-SHA384",
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    ECDHE|ECDSA|AES256|AESGCM|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-RSA-AES256-GCM-SHA384",
    BR_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    ECDHE|aRSA|AES256|AESGCM|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-AES128-CCM",
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CCM,
    ECDHE|ECDSA|AES128|AESCCM|AEAD|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-ECDSA-AES256-CCM",
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CCM,
    ECDHE|ECDSA|AES256|AESCCM|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-AES128-CCM8",
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8,
    ECDHE|ECDSA|AES128|AESCCM8|AEAD|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-ECDSA-AES256-CCM8",
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8,
    ECDHE|ECDSA|AES256|AESCCM8|AEAD|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-AES128-SHA256",
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256,
    ECDHE|ECDSA|AES128|SHA256|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-RSA-AES128-SHA256",
    BR_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
    ECDHE|aRSA|AES128|SHA256|TLS12|HIGH,
    128,
  },
  {
    "ECDHE-ECDSA-AES256-SHA384",
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    ECDHE|ECDSA|AES256|SHA384|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-RSA-AES256-SHA384",
    BR_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
    ECDHE|aRSA|AES256|SHA384|TLS12|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-AES128-SHA",
    BR_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA,
    ECDHE|ECDSA|AES128|SHA1|TLS10|HIGH,
    128,
  },
  {
    "ECDHE-RSA-AES128-SHA",
    BR_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA,
    ECDHE|aRSA|AES128|SHA1|TLS10|HIGH,
    128,
  },
  {
    "ECDHE-ECDSA-AES256-SHA",
    BR_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA,
    ECDHE|ECDSA|AES256|SHA1|TLS10|HIGH,
    256,
  },
  {
    "ECDHE-RSA-AES256-SHA",
    BR_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA,
    ECDHE|aRSA|AES256|SHA1|TLS10|HIGH,
    256,
  },
 /* ECDH suites, used in BearSSL "full" profile do
  * not have corresponding OpenSSL
  */
  {
    "AES128-GCM-SHA256",
    BR_TLS_RSA_WITH_AES_128_GCM_SHA256,
    kRSA|aRSA|AES128|AESGCM|SHA256|TLS12|HIGH,
    128,
  },
  {
    "AES256-GCM-SHA384",
    BR_TLS_RSA_WITH_AES_256_GCM_SHA384,
    kRSA|aRSA|AES256|AESGCM|SHA384|TLS12|HIGH,
    256,
  },
  {
    "AES128-CCM",
    BR_TLS_RSA_WITH_AES_128_CCM,
    kRSA|aRSA|AES128|AESCCM|TLS12|HIGH,
    128,
  },
  {
    "AES256-CCM",
    BR_TLS_RSA_WITH_AES_256_CCM,
    kRSA|aRSA|AES256|AESCCM|TLS12|HIGH,
    256,
  },
  {
    "AES128-CCM8",
    BR_TLS_RSA_WITH_AES_128_CCM_8,
    kRSA|aRSA|AES128|AESCCM8|TLS12|HIGH,
    128,
  },
  {
    "AES256-CCM8",
    BR_TLS_RSA_WITH_AES_256_CCM_8,
    kRSA|aRSA|AES256|AESCCM8|TLS12|HIGH,
    256,
  },
  {
    "AES128-SHA256",
    BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
    kRSA|aRSA|AES128|SHA256|TLS12|HIGH,
    128,
  },
  {
    "AES256-SHA256",
    BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
    kRSA|aRSA|AES256|SHA256|TLS12|HIGH,
    256,
  },
  {
    "AES128-SHA",
    BR_TLS_RSA_WITH_AES_128_CBC_SHA,
    kRSA|aRSA|AES128|SHA1|TLS10|HIGH,
    128,
  },
  {
    "AES256-SHA",
    BR_TLS_RSA_WITH_AES_256_CBC_SHA,
    kRSA|aRSA|AES256|SHA1|TLS10|HIGH,
    256,
  },
  {
    "ECDHE-ECDSA-DES-CBC3-SHA",
    BR_TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA,
    ECDHE|ECDSA|TRIPLEDES|SHA1|TLS10|MEDIUM,
    112,
  },
  {
    "ECDHE-RSA-DES-CBC3-SHA",
    BR_TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA,
    ECDHE|aRSA|TRIPLEDES|SHA1|TLS10|MEDIUM,
    112,
  },
  {
    "DES-CBC3-SHA",
    BR_TLS_RSA_WITH_3DES_EDE_CBC_SHA,
    kRSA|aRSA|TRIPLEDES|SHA1|TLS10|MEDIUM,
    112,
  },
};

sbearssl_suiteinfo const *const sbearssl_suite_list = sbearssl_suite_list_ ;
size_t const sbearssl_suite_list_len = sizeof(sbearssl_suite_list_) / sizeof(sbearssl_suiteinfo) ;
