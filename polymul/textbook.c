#include "textbook.h"

int textbook_simple(
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
  result[(key_length-1)] += key[(key_length -1)] * text[0];

  for(int k = 1; k < key_length-1; k++){
    for(int t = 0; t < text_length; t++){
      result[k+t] += key[k] * text[t];
    }
  }
  return 0x00;
}

int textbook_clean_4(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  if(key_length % 4 != 0)
    return 0x01;

  result[0] = key[0] * text[0];
  for(int t = 1; t < text_length; t++){
    result[t] = key[0] * text[t];
    result[(key_length-1) + t] = key[(key_length -1)] * text[t];
  }
  result[(key_length-1)] += key[(key_length -1)] * text[0];

  for(int k = 1; k < key_length-1; k++){
    for(int t = 0; t < text_length; t=t+4){
      result[k+t+0] += key[k] * text[t+0];
      result[k+t+1] += key[k] * text[t+1];
      result[k+t+2] += key[k] * text[t+2];
      result[k+t+3] += key[k] * text[t+3];
    }
  }
  return 0x00;
}

__attribute__((optimize("unroll-loops")))
int textbook_static(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  (void)(key_length);
  (void)(text_length);
  for(int k = 0; k < DEGREE; k++){
    for(int t = 0; t < DEGREE; t++){
      result[k+t] += key[k] * text[t];
    }
  }
  return 0x00;
}
