#include "hal-stm32f4.h"
#include "kprintf.h"
#include "simpleserial.h"

static uint8_t recv_key(int size, uint8_t* data)
{
  simpleserial_put('r', size, data);
  return 0x00;
}

static uint8_t recv_plain(int size, uint8_t* data)
{
  simpleserial_put('r', size, data);
  return 0x00;
}

static void init(void)
{
  hal_setup(CLOCK_BENCHMARK);
  simpleserial_init();
  simpleserial_addcmd('k', MAX_SS_CMDS, recv_key);
  simpleserial_addcmd('p', MAX_SS_CMDS, recv_plain);
  return;
}

int main(void) {
  init();
  int count = simpleserial_get();
  kprintf("d%i\r\n", count);
  count = simpleserial_get();
  kprintf("d%i\r\n", count);
	return 0;
}
