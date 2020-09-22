PROJECT = polymul
BUILD_DIR = bin

# Example main file to use
CFILES = main.c
CFILES += hal-stm32f4.c
# If simpleserial framework should be used
CFILES += simpleserial.c

DEVICE = stm32f407vgt6

# Folders in which to look for .c, .S and .h files to look for
# /common folder is required
SHARED_DIR = ../common

# Only needed when using the polymul framework
POLYMUL_PATH = ../polymul
include ../polymul/polymul.mk
SHARED_DIR += $(POLYMUL_PATH)
SHARED_DIR += $(POLYMUL_SHARED_DIR)
POLYMUL    ?= TEXTBOOK_CLEAN
CHAIN_SIZE ?= 2
CHAIN      ?= karatsuba, remapped_textbook
DEGREE     ?= 0
TGT_CFLAGS += -D POLYMUL=$(POLYMUL)
TGT_CFLAGS += -D CHAIN_SIZE=$(CHAIN_SIZE)
TGT_CFLAGS += -D CHAIN="$(CHAIN)"
ifneq ($(DEGREE),)
TGT_CFLAGS += -D DEGREE="$(DEGREE)"
endif

# C files to be compiled
CFILES += dprintf.c
CFILES += $(POLYMUL_SRC)

# Assembly files to compile
AFILES = $(POLYMUL_ASRC)


# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
