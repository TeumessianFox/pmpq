#ifndef CONFIG_H
#define CONFIG_H

/* POLYMUL must be defined as the polynomial multiplication which should be used */
/* Can be set using "make POLYMUL={algo}" */
#define TEXTBOOK_SIMPLE         1
#define TEXTBOOK_CLEAN          2
#define TEXTBOOK_CLEAN_4        3
#define TEXTBOOK_STATIC         4
#define ASM_SCHOOLBOOK_12       5
#define ASM_SCHOOLBOOK_16       6
#define ASM_SCHOOLBOOK_24       7
#define ASM_TESTING             8
#define KARATSUBA_ONLY          9
#define POLYMUL_CHAIN          10
#define NTT                    11

/* Only needs to be changed if a new polymul should be added to the list of
 * options. To add a new one follow the TEXTBOOK example
 * Parameter for polymul:
 * uint16_t *key
 * int key_length -> always > 0
 * uint16_t *text
 * int text_length -> always > 0
 * uint16_t *result -> array is nulled
 */
#include "textbook.h"
#include "karatsuba.h"
#include "karatsuba_only.h"
#include "asm_testing.h"
#include "schoolbook_12x12.h"
#include "schoolbook_16x16.h"
#include "schoolbook_24x24.h"
#include "toom_cook_3.h"
#include "toom_cook_3_libpolymath.h"
#include "ntt.h"
#ifdef CHAIN_SIZE
  #include "polymul_chain.h"
#endif
#if POLYMUL == TEXTBOOK_SIMPLE
  #define polynomial_multiplication(args...) textbook_simple(args)
#elif POLYMUL == TEXTBOOK_CLEAN
  #define polynomial_multiplication(args...) textbook_clean(args)
#elif POLYMUL == TEXTBOOK_CLEAN_4
  #define polynomial_multiplication(args...) textbook_clean_4(args)
#elif POLYMUL == TEXTBOOK_STATIC
  #define polynomial_multiplication(args...) textbook_static(args)
#elif POLYMUL == ASM_SCHOOLBOOK_12
  #define polynomial_multiplication(key, key_length, text, text_length, result) schoolbook_12x12(result, key, text)
#elif POLYMUL == ASM_SCHOOLBOOK_16
  #define polynomial_multiplication(key, key_length, text, text_length, result) schoolbook_16x16(result, key, text)
#elif POLYMUL == ASM_SCHOOLBOOK_24
  #define polynomial_multiplication(key, key_length, text, text_length, result) schoolbook_24x24(result, key, text)
#elif POLYMUL == ASM_TESTING
  #define polynomial_multiplication(key, key_length, text, text_length, result) asm_testing(result, key, text)
#elif POLYMUL == KARATSUBA_ONLY
  #define polynomial_multiplication(args...) karatsuba_only(args)
#elif POLYMUL == POLYMUL_CHAIN
  #define polynomial_multiplication(args...) polymul_chain(args)
#elif POLYMUL == NTT
  #define polynomial_multiplication(args...) ntt(args)
#endif

#endif
