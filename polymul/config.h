#ifndef CONFIG_H
#define CONFIG_H

/* POLYMUL must be defined as the polynomial multiplication which should be used */
/* Can be set using "make POLYMUL={algo}" */
#define TEXTBOOK             1
#define KARATSUBA            2
#define ASM_SCHOOLBOOK_24    3
#define ASM_TESTING          4
#define KARATSUBA_ONLY       5
#define POLYMUL_CHAIN        6

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
#include "asm_testing.h"
#include "karatsuba_only.h"
#include "schoolbook_24x24.h"
#include "polymul_chain.h"
#if POLYMUL == TEXTBOOK
  #define polynomial_multiplication(args...) textbook(args)
#elif POLYMUL == KARATSUBA
  #define polynomial_multiplication(args...) karatsuba(args)
#elif POLYMUL == ASM_SCHOOLBOOK_24
  #define polynomial_multiplication(key, key_length, text, text_length, result) schoolbook_24x24(result, key, text)
#elif POLYMUL == ASM_TESTING
  #define polynomial_multiplication(key, key_length, text, text_length, result) asm_testing(result, key, text)
#elif POLYMUL == KARATSUBA_ONLY
  #define polynomial_multiplication(args...) karatsuba_only(args)
#elif POLYMUL == POLYMUL_CHAIN
  #define polynomial_multiplication(args...) polymul_chain(args)
#endif

/* Max amount of uint16_t to be received by a command */
#define MAX_SS_LEN 1024

#endif
