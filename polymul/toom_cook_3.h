#ifndef TOOM_COOK_3_H
#define TOOM_COOK_3_H
#include <stdint.h>

int toom_cook_3(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth
    );

#endif
