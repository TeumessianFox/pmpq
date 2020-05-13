#include "polymath.h"

void poly_lshd(uint16_t *a, int len, int shift)
{
  int i;
  for(i = len-1; i > shift; i--){
    a[i] = a[i - shift];
  }
  for(i=0; i<shift; i++){
    a[i] = 0;
  }
}
