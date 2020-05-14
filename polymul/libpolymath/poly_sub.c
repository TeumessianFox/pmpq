#include "polymath.h"

void poly_sub(uint16_t *a, uint16_t *b, int len, uint16_t *c)
{
  for (int i = 0; i < len; i++){
    /*TODO:
     *  Something weird is happening over here
     *  2 - 65535 = 3
     *  65535 in int16 is -1
     *  2 - (-1) = 3
     *  But we actually want uint16 substraction
     *  2 - 65535 = 1
     */
    c[i] = (uint16_t) a[i] - b[i];
  }
}
