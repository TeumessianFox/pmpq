TARGET = cw_polymul

PLATFORM = CW308_STM32F4

# Example main file to use
SRC = main_cw.c
# If simpleserial framework should be used
SRC += simpleserial_cw.c

# Only needed if using the polymul framework
POLYMUL_PATH = ../polymul
include ../polymul/polymul.mk
EXTRAINCDIRS += $(POLYMUL_PATH)
VPATH        += $(POLYMUL_PATH)
EXTRAINCDIRS += $(POLYMUL_SHARED_DIR)
VPATH        += $(POLYMUL_SHARED_DIR)
POLYMUL    ?= TEXTBOOK
CHAIN_SIZE ?= 2
CHAIN      ?= karatsuba, remapped_textbook
CFLAGS     += -D POLYMUL=$(POLYMUL)
CFLAGS     += -D CHAIN_SIZE=$(CHAIN_SIZE)
CFLAGS     += -D CHAIN="$(CHAIN)"

# C files to be compiled
SRC += $(POLYMUL_SRC)

# Assembly files to compile
ASRC = $(POLYMUL_ASRC)

# Folders in which to look for .c, .S and .h files to look for
VPATH += ../common
EXTRAINCDIRS += ../common

# Don't change anything bellow
FIRMWAREPATH = ../chipwhisperer/hardware/victims/firmware
include $(FIRMWAREPATH)/Makefile.inc
