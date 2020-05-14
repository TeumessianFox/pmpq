#include "textbook.h"

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

int textbook_clean(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  result[0] = key[0] * text[0];
  for(int t = 1; t < text_length; t++){
    result[t] = key[0] * text[t];
    result[(key_length-1) + t] = key[(key_length -1)] * text[t];
  }
  result[(key_length-1) + 0] += key[(key_length -1)] * text[0];
  for(int k = 1; k < key_length-1; k++){
    for(int t = 0; t < text_length; t++){
      result[k+t] += key[k] * text[t];
    }
  }
  return 0x00;
}
