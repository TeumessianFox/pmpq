PROJECT = polymul
BUILD_DIR = bin

TARGET_HW =

ifeq ($(TARGET_HW),)
		CFILES = main.c
		CFILES += hal-stm32f4.c
		DEVICE = stm32f407vgt6
else
		CFILES = main_cw.c
		CFILES += hal-stm32f4_cw.c
		DEVICE = stm32f405rgt6
endif

SHARED_DIR = ../common
AFILES += api-asm.S
CFILES += dprintf.c
CFILES += simpleserial.c
CFILES += textbook.c
CFILES += karatsuba.c
AFILES += schoolbook_24x24.S


# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
