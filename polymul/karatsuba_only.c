#include "karatsuba_only.h"

int recursive_karatsuba(
    uint16_t *key,
    uint16_t *text,
    int key_length,
    uint16_t *result);

int recursive_karatsuba(
    uint16_t *key,
    uint16_t *text,
    int key_length,
    uint16_t *result)
{
  int half_len = key_length;
  if(half_len == 1){
    result[0] = key[0] * text[0];
    return 0x00;
  }
  if(half_len % 2 != 0)
    half_len += 1;
  int result_half = half_len;
  half_len = half_len / 2;

  uint16_t result_mid[result_half];
  uint16_t result_upper[result_half];
  uint16_t result_low[result_half];
  for(int i = 0; i < result_half; i++){
    result_mid[i] = 0;
    result_upper[i] = 0;
    result_low[i] = 0;
  }

  recursive_karatsuba(key, text, half_len, result_low);
  recursive_karatsuba(key + half_len, text + half_len, half_len, result_upper);

  uint16_t key_add[half_len];
  uint16_t text_add[half_len];
  for(int i = 0; i < half_len; i++){
    key_add[i] = key[half_len + i] + key[i];
    text_add[i] = text[half_len + i] + text[i];
  }
  recursive_karatsuba(key_add, text_add, half_len, result_mid);

  for(int i = 0; i < result_half; i++){
    result[i] += result_low[i];
    result[half_len + i] += result_mid[i] - result_upper[i] - result_low[i];
    result[result_half + i] += result_upper[i];
  }

  return 0x00;
}

int karatsuba_only(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  int len = key_length;
  if(len < text_length)
    len = text_length;
  int size = 1;
  while(size <= len){
    size = 2 * size;
  }
  recursive_karatsuba(key, text, size, result);
  return 0x00;
}
