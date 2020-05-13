#include "polymath.h"

void poly_div3(uint16_t *a, int len, uint16_t *b)
{
  for (int i = 0; i < len; i++){
    //TODO you can do better
    b[i] = a[i] / 3;
  }
}
