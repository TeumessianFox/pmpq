#include "polymath.h"

void poly_add(uint16_t *a, uint16_t *b, int len, uint16_t *c)
{
  for (int i = 0; i < len; i++){
    c[i] = a[i] + b[i];
  }
}
