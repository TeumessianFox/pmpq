#ifndef KARATSUBA_ONLY_H
#define KARATSUBA_ONLY_H
#include <stdint.h>

int karatsuba_only(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

#endif
