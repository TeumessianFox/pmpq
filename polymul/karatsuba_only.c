#include "karatsuba_only.h"
#include "dprintf.h"

int recursive_karatsuba(
    uint16_t *key,
    uint16_t *text,
    int degree,
    uint16_t *result);

int recursive_karatsuba(
    uint16_t *key,
    uint16_t *text,
    int degree,
    uint16_t *result)
{
  if(degree == 1){
    result[0] = key[0] * text[0];
    return 0x00;
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

  recursive_karatsuba(key,            text,            limb_len, result_low);
  recursive_karatsuba(key + limb_len, text + limb_len, limb_len, result_upper);
  recursive_karatsuba(key_add,        text_add,        limb_len, result_mid);

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
  while(size < len){
    size = 2 * size;
  }
  recursive_karatsuba(key, text, size, result);
  return 0x00;
}
