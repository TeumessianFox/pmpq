#ifndef POLYMATH_H
#define POLYMATH_H
#include <stdint.h>

void poly_add(uint16_t *a, uint16_t *b, int len, uint16_t *c);
void poly_sub(uint16_t *a, uint16_t *b, int len, uint16_t *c);
void poly_mul2(uint16_t *a, int len, uint16_t *b);
void poly_div3(uint16_t *a, int len, uint16_t *b);
void poly_div2(uint16_t *a, int len, uint16_t *b);
void poly_lshd(uint16_t *a, int len, int shift);

#endif
