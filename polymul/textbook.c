#include "config.h"
#include <stdint.h>


int textbook(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  for(int k = 0; k < key_length; k++){
    for(int t = 0; t < text_length; t++){
      result[k+t] += key[k] * text[t];
    }
  }
  return 0x00;
}
