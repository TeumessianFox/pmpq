#include "config.h"
#include <stdint.h>


int textbook(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  for(int i = 0; i < key_length; i++){
    for(int j = 0; j < text_length; j++){
      result[i+j] += key[i] * text[j];
      // TODO: check for overflow?!
    }
  }
  return 0x00;
}
