#include "polymath.h"

#define INVERSE_3 43691

void poly_div3(uint16_t *a, int len, uint16_t *b)
{
  for (int i = 0; i < len; i++){
    b[i] = (uint16_t)(a[i] * INVERSE_3);
  }
}
