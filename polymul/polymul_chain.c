#include "config.h"

const algo chain[CHAIN_SIZE] = {CHAIN};

int remapped_schoolbook_24x24(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  (void)(key_length);
  (void)(text_length);
  (void)(depth);
  return schoolbook_24x24(result, key, text);
}

int remapped_textbook(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  (void)(depth);
  return textbook_simple(key, key_length, text, text_length, result);
}

int remapped_textbook_clean(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  (void)(depth);
  return textbook_clean(key, key_length, text, text_length, result);
}

int polymul_chain(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  return (*chain[0])(key, key_length, text, text_length, result, 0);
}
