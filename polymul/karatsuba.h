#ifndef KARATSUBA_H
#define KARATSUBA_H
#include <stdint.h>

int karatsuba(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

#endif
