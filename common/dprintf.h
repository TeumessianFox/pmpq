#ifndef KPRINTF_H
#define KPRINTF_H
#include "hal-stm32f4.h"

int kprintf(char *format,...);

/* Some Macro magic to allow debug prints using dprintf()*/
#define dprintf(args...) do{\
  hal_send_char('d');\
  kprintf(args);\
}while(0)

#endif
