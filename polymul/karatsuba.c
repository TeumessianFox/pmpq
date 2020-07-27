#include "config.h"

int karatsuba(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  if(key_length != text_length)
    return 0x01;
  int degree = key_length;
  if(degree % 2 == 1){
    // TODO: What todo for degrees which are not a power of 2?
    degree += 1;
  }
  int limb_result_len = degree - 1;
  int limb_len = degree / 2;

  uint16_t result_mid[limb_result_len];
  uint16_t result_upper[limb_result_len];
  uint16_t result_low[limb_result_len];
  uint16_t key_add[limb_len];
  uint16_t text_add[limb_len];
  for(int i = 0; i < limb_len; i++){
    key_add[i] = key[limb_len + i] + key[i];
    text_add[i] = text[limb_len + i] + text[i];
  }

  (*chain[depth+1])(key,            limb_len, text,            limb_len, result_low,   depth+1);
  (*chain[depth+1])(key + limb_len, limb_len, text + limb_len, limb_len, result_upper, depth+1);
  (*chain[depth+1])(key_add,        limb_len, text_add,        limb_len, result_mid,   depth+1);

  for(int i = 0; i < limb_result_len; i++){
    result[i] = result_low[i];
    result[limb_result_len + i + 1] = result_upper[i];
  }

  int limb_result_middle = (limb_result_len - 1) / 2;
  result[limb_result_len] = result_mid[limb_result_middle] - result_upper[limb_result_middle] - result_low[limb_result_middle];
  for(int i = 0; i < limb_result_middle; i++){
    result[limb_len + i] += result_mid[i] - result_upper[i] - result_low[i];
    result[limb_result_len + i + 1] += result_mid[limb_result_middle + i + 1] - result_upper[limb_result_middle + i + 1] - result_low[limb_result_middle + i + 1];
  }

  return 0x00;
}
