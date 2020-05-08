TARGET = cw_polymul

PLATFORM = CW308_STM32F4

# If simpleserial framework should be used
SRC += simpleserial_cw.c
# Example main file to use
SRC += main_cw_test.c

# C files to be compiled
SRC += textbook.c
SRC += karatsuba.c
SRC += karatsuba_only.c
SRC += polymul_chain.c

# Assembly files to compile
ASRC += schoolbook_24x24.S

# Only needed if using the polymul framework
POLYMUL    ?= TEXTBOOK
CHAIN_SIZE ?= 2
CHAIN      ?= karatsuba, remapped_textbook
CFLAGS     += -D POLYMUL=$(POLYMUL)
CFLAGS     += -D CHAIN_SIZE=$(CHAIN_SIZE)
CFLAGS     += -D CHAIN="$(CHAIN)"

# Folders in which to look for .c, .S and .h files to look for
VPATH += ../common
EXTRAINCDIRS += ../common
VPATH += ../polymul
EXTRAINCDIRS += ../polymul

# Don't change anything bellow
FIRMWAREPATH = ../chipwhisperer/hardware/victims/firmware
include $(FIRMWAREPATH)/Makefile.inc
