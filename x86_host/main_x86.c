#include <stdint.h>
#include "config.h"
#include "common_config.h"
#include <stdio.h>

uint16_t key[MAX_SS_LEN];
uint16_t text[MAX_SS_LEN];
int key_length = 0;
int text_length = 0;
int result_length = 0;

#define len 6
//const uint16_t key_copy[len] = {     0,    0,    1,65535,    0,    0};
//const uint16_t text_copy[len] = { 1776,1239, 507, 177,2397,3750};
const uint16_t key_copy[len] = {1,65535,    0,    1,    1,    1};
const uint16_t text_copy[len] = {3519, 528,4557,2979,3678,2634};

int main(void) {
  for(int i = 0; i < MAX_SS_LEN; i++){
    key[i] = 0;
    text[i] = 0;
  }
  key_length = len;
  text_length = len;

  for(int i = 0; i < len; i++){
    key[i] = key_copy[i];
    text[i] = text_copy[i];
  }
  printf("KEY:\r\n");
  for(int i = 0; i < key_length; i++){
    if(i % 6 == 0 && i != 0){
      printf("\r\n");
    }
    printf("%u", key[i]);
    if(i != key_length){
      printf(" ");
    }
  }
  printf("\r\n\n");
  printf("TEXT:\r\n");
  for(int i = 0; i < text_length; i++){
    if(i % 6 == 0 && i != 0){
      printf("\r\n");
    }
    printf("%u", text[i]);
    if(i != text_length){
      printf(" ");
    }
  }
  printf("\r\n\n");

  result_length = key_length + text_length - 1;

  uint16_t result[result_length];
  for(int i = 0; i < result_length; i++){
    result[i] = 0;
  }
  int status = 0;

  status = polynomial_multiplication(
            key, key_length,
            text, text_length,
            result);

  printf("RESULT:\r\n");
  for(int i = 0; i < result_length; i++){
    if(i % 6 == 0 && i != 0){
      printf("\r\n");
    }
    printf("%u", result[i]);
    if(i != result_length){
      printf(" ");
    }
  }
  printf("\r\n");

	return status;
}


int schoolbook_24x24(uint16_t *r, const uint16_t *a, const uint16_t *b){
  (void)(r);
  (void)(a);
  (void)(b);
  return 0;
}
