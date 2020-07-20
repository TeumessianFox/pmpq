#ifndef TEXTBOOK_H
#define TEXTBOOK_H
#include <stdint.h>

int textbook_simple(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

/*
 * Also work on not zero initialised result array
 */
int textbook_clean(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

/*
 * Only work for key/text_length % 4 == 0
 */
int textbook_clean_4(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

/*
 * Fixed key/text length
 */
#define FIXED_LEN 256
int textbook_static(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result
    );

#endif
