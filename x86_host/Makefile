PROJECT = polymul
BUILD_DIR = bin

# Example main file to use
CFILES = main_x86.c

# Folders in which to look for .c, .S and .h files to look for
SHARED_DIR += ../common

# Only needed when using the polymul framework
POLYMUL_PATH = ../polymul
include ../polymul/polymul.mk
SHARED_DIR += $(POLYMUL_PATH)
SHARED_DIR += $(POLYMUL_SHARED_DIR)
POLYMUL    ?= TEXTBOOK
CHAIN_SIZE ?= 2
CHAIN      ?= karatsuba, remapped_textbook
TGT_CFLAGS += -D POLYMUL=$(POLYMUL)
TGT_CFLAGS += -D CHAIN_SIZE=$(CHAIN_SIZE)
TGT_CFLAGS += -D CHAIN="$(CHAIN)"

# C files to be compiled
CFILES += $(POLYMUL_SRC)

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))

include rules.mk
