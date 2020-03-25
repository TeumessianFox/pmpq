#ifndef CONFIG_H
#define CONFIG_H

/* ######################################################################### */
/* Make changes as needed */

/* POLYMUL must be defined as the polynomial multiplication which should be used */
// #define POLYMUL TEXTBOOK
/* Used for automated tests using python */
#include "auto_python_tests.h"

#define TEXTBOOK 1
#define HALF_TEXTBOOK 2

/* Max amount of uint16_t to be received by a command */
#define MAX_SS_LEN 1024

/* Only needs to be changed if a new polymul should be added to the list of
 * options. To add a new one follow the TEXTBOOK example
 * Parameter for polymul:
 * uint16_t *key
 * int key_length -> always > 0
 * uint16_t *text
 * int text_length -> always > 0
 * uint16_t *result -> array is nulled
 */
#if POLYMUL == TEXTBOOK
  #include "textbook.h"
  #define polynomial_multiplication(args...) textbook(args)
#elif POLYMUL == HALF_TEXTBOOK
  #include "half_textbook.h"
  #define polynomial_multiplication(args...) half_textbook(args)
#endif

/* End of user changes */
/* You should not need to change anything bellow */
/* ######################################################################### */

#endif
