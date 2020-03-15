#include "hal-stm32f4.h"
#include "kprintf.h"

int main(void) {
  hal_setup(CLOCK_BENCHMARK);
  while(1){
    uint16_t received = hal_recv();
    kprintf("\r\nReceived: %c\r\n", received);
  }
	return 0;
}
