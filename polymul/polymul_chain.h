#ifndef POLYMUL_CHAIN_H
#define POLYMUL_CHAIN_H
#include <stdint.h>

typedef int (*algo)(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth);

extern const algo chain[CHAIN_SIZE];

int remapped_schoolbook_24x24(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth);

int remapped_textbook(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth);

int polymul_chain(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result);

#endif
