PROJECT = polymul
BUILD_DIR = bin

SHARED_DIR = ../common
CFILES = main.c
AFILES += api-asm.S
CFILES += dprintf.c
CFILES += simpleserial.c
CFILES += textbook.c
CFILES += karatsuba.c


DEVICE = stm32f407vgt6
#DEVICE = stm32f405rgt6

ifeq ($(DEVICE),stm32f407vgt6)
		CFILES += hal-stm32f4.c
else
		CFILES += hal-stm32f4_cw.c
endif


# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
