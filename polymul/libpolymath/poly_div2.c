#include "polymath.h"

void poly_div2(uint16_t *a, int len, uint16_t *b)
{
  for(int i = 0; i<len; i++){
    b[i] = a[i] >> 1;
  }
}
