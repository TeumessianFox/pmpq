#include "hal-stm32f4.h"

int main(void) {
  hal_setup(CLOCK_BENCHMARK);
  while(1){
    uint16_t received = hal_recv();
    hal_send_str((const char *) (&received));
    hal_send_str("\rReceived\r\n");
  }
	return 0;
}
