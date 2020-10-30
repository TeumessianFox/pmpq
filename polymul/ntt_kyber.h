#ifndef NTT_KYBER_H
#define NTT_KYBER_H
#include <stdint.h>

int ntt_kyber(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

#endif
