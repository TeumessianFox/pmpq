POLYMUL_SRC  = $(POLYMATH_SRC)
POLYMUL_SRC += textbook.c
POLYMUL_SRC += karatsuba.c
POLYMUL_SRC += karatsuba_only.c
POLYMUL_SRC += polymul_chain.c
POLYMUL_SRC += toom_cook_3.c

POLYMUL_ASRC  = schoolbook_24x24.S
POLYMUL_ASRC += asm_testing.S
POLYMUL_ASRC += $(POLYMATH_ASRC)

POLYMUL_SHARED_DIR = $(POLYMUL_PATH)/$(POLYMATH_SHARED_DIR)

include $(POLYMUL_PATH)/libpolymath/libpolymath.mk
