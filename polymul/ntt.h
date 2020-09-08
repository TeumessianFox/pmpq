#ifndef NTT_H
#define NTT_H
#include <stdint.h>

int ntt(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

#endif
