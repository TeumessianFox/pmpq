#include "hal-stm32f4.h"

int main(void) {
  hal_setup(CLOCK_BENCHMARK);
  while(1){
    hal_send_str("?\r\n");
  }
	return 0;
}
