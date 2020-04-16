/*
 * Don't make any changes to this file
 * All settings can be changed in polymul/config.h
 */

#include <stdint.h>
#include "hal.h"
#include "config.h"
#include "simpleserial_cw.h"

uint16_t key[MAX_SS_LEN];
int key_length = 0;
uint16_t text[MAX_SS_LEN];
int text_length = 0;
int result_length = 0;
volatile int reset_m4 = 0;

static uint8_t recv_key(int size, uint16_t* data)
{
  key_length = size;
  for(int i = 0; i < size; i++){
    key[i] = data[i];
  }
  for(int i = size; i < MAX_SS_LEN; i++){
    key[i] = 0;
  }
  return 0x00;
}

static uint8_t recv_plain(int size, uint16_t* data)
{
  text_length = size;
  for(int i = 0; i < size; i++){
    text[i] = data[i];
  }
  for(int i = size; i < MAX_SS_LEN; i++){
    text[i] = 0;
  }
  simpleserial_put('r', text_length * 2, (uint8_t *)text);
  return 0x00;
}

static uint8_t reset_board(int size, uint16_t *data)
{
  (void)(size);
  (void)(data);
  key_length = 0;
  text_length = 0;
  result_length = 0;
  reset_m4 = 1;
  return 0x00;
}

static int init(void)
{
  simpleserial_init();
  simpleserial_addcmd('k', MAX_SS_LEN, recv_key);
  simpleserial_addcmd('p', MAX_SS_LEN, recv_plain);
  simpleserial_addcmd('x', MAX_SS_LEN, reset_board);
  return 0;
}

int main(void) {
  platform_init();
  init_uart();
  init();
  while(1){
    simpleserial_get();
    simpleserial_get();
    simpleserial_get();
    while(reset_m4 != 1){}
    reset_m4 = 0;
  }

  return 0;
}
