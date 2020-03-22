/*
 * Don't make any changes to this file
 * All settings can be changed in polymul/config.h
 */

#include <stdint.h>
#include "hal-stm32f4.h"
#include "dprintf.h"
#include "simpleserial.h"
#include "config.h"

uint16_t key[MAX_SS_LEN];
int key_length = 0;
uint16_t text[MAX_SS_LEN];
int text_length = 0;
int result_length = 0;

static uint16_t recv_key(int size, uint16_t* data)
{
  key_length = size;
  for(int i = 0; i < size; i++){
    key[i] = data[i];
  }
  return 0x00;
}

static uint16_t recv_plain(int size, uint16_t* data)
{
  text_length = size;
  for(int i = 0; i < size; i++){
    text[i] = data[i];
  }
  return 0x00;
}

static int init(void)
{
  hal_setup(CLOCK_BENCHMARK);
  simpleserial_init();
  int status1 = simpleserial_addcmd('k', MAX_SS_LEN, recv_key);
  int status2 = simpleserial_addcmd('p', MAX_SS_LEN, recv_plain);
  if(status1){
    return status1;
  } else if(status2){
    return status2;
  }
  return 0;
}

int main(void) {
  int init_status = init();
  simpleserial_put('z', 1, (uint16_t *) &init_status);
  if(init_status){
    return init_status;
  }

  /* Receive key and plain text */
  simpleserial_get();
  simpleserial_get();

  if(key_length <= 0){
    dprintf("No key received\r\n");
    return 1;
  }
  if(text_length <= 0){
    dprintf("No text received\r\n");
    return 1;
  }

  result_length = key_length + text_length;

  uint16_t result[result_length];
  for(int i = 0; i < result_length; i++){
    result[i] = 0;
  }
  int status = 0;
  status = polynomial_multiplication(
              (uint16_t *)key, key_length,
              (uint16_t *)text, text_length,
              result);

  simpleserial_put('r', result_length, (uint16_t *)result);
  simpleserial_put('z', 1, (uint16_t *) &status);

	return 0;
}
