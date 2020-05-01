#include "config.h"

int karatsuba(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  int half_len = key_length;
  if (half_len < text_length)
    half_len = text_length;
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

  (*chain[depth+1])(key, half_len, text, half_len, result_low, depth+1);
  (*chain[depth+1])(key + half_len, half_len, text + half_len, half_len, result_upper, depth+1);

  uint16_t key_add[half_len];
  uint16_t text_add[half_len];
  for(int i = 0; i < half_len; i++){
    key_add[i] = key[half_len + i] + key[i];
    text_add[i] = text[half_len + i] + text[i];
  }
  (*chain[depth+1])(key_add, half_len, text_add, half_len, result_mid,depth+1);

  for(int i = 0; i < result_half; i++){
    result[i] += result_low[i];
    result[half_len + i] += result_mid[i] - result_upper[i] - result_low[i];
    result[result_half + i] += result_upper[i];
  }

  return 0x00;
}
