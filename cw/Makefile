TARGET = cw_polymul

PLATFORM = CW308_STM32F4

SRC += main_cw_test.c
SRC += simpleserial_cw.c
SRC += textbook.c
SRC += karatsuba.c

ASRC += schoolbook_24x24.S

POLYMUL ?= TEXTBOOK
CFLAGS += -D POLYMUL=$(POLYMUL)

VPATH += ../common
EXTRAINCDIRS += ../common
VPATH += ../polymul
EXTRAINCDIRS += ../polymul

FIRMWAREPATH = ../chipwhisperer/hardware/victims/firmware
include $(FIRMWAREPATH)/Makefile.inc
