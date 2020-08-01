#ifndef TOOM_COOK_3_LIBPOLYMATH_H
#define TOOM_COOK_3_LIBPOLYMATH_H
#include <stdint.h>

int toom_cook_3_libpolymath(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth
    );

#endif
