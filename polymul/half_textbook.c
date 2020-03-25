#include "half_textbook.h"


int half_textbook(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  for(int k = 0; k < key_length; k++){
    for(int i = 0; i < text_length/2; i++){
      result[k + i] += key[k] * text[i];
    }
    for(int i = text_length/2; i < text_length; i++){
      result[k + i] += key[k] * text[i];
    }
  }
  return 0x00;
}
