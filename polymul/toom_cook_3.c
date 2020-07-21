#include "polymath.h"
#include "config.h"
#include "common_config.h"
//#include "dprintf.h"

#define ONE_THIRD   (MAX_SS_LEN + 2)/3
#define TWO_THIRD   ((MAX_SS_LEN + 2)*2)/3

/* Based on:
 * Marco BODRATO, "Towards Optimal Toom-Cook Multiplication for
 * Univariate and Multivariate Polynomials in Characteristic 2 and 0";
 * "WAIFI'07 proceedings" (C.Carlet and B.Sunar, eds.)  LNCS#4547,
 * Springer, Madrid, Spain, June 2007, pp. 116-133.
 * http://bodrato.it/papers/#Toom-Cook
 */
int toom_cook_3(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result,
    int depth)
{
  int lower_len = key_length;
  if (key_length < text_length)
    lower_len = text_length;
  lower_len = (lower_len + 2) / 3;

  uint16_t *k0 = key;
  uint16_t *t0 = text;
  uint16_t *k1 = k0 + lower_len;
  uint16_t *t1 = t0 + lower_len;
  uint16_t *k2 = k1 + lower_len;
  uint16_t *t2 = t1 + lower_len;
  uint16_t w0[TWO_THIRD];
  uint16_t w1[TWO_THIRD];
  uint16_t w2[TWO_THIRD];
  uint16_t w3[TWO_THIRD];

  for(int i = 0; i < TWO_THIRD; i++){
    w0[i] = 0;
    w1[i] = 0;
    w2[i] = 0;
    w3[i] = 0;
  }

  /* Evaluation phase */
  /* w0 = k0+k2 */
  poly_add(k0, k2, lower_len, w0);

  /* w4 = t0+t2 */
  poly_add(t0, t2, lower_len, result);

  /* w2 = w0-k1 = k2-k1+k0 */
  //for(int i = 0; i < lower_len; i++)
    //dprintf("w0[%i] = %i and k1[%i] = %i\r\n", i, w0[i], i, k1[i]);
  poly_sub(w0, k1, lower_len, w2);

  /* w0 = w0+k1 = k2+k1+k0 */
  poly_add(w0, k1, lower_len, w0);

  /* w1 = w4-t1 = t2-t1+t0 */
  //for(int i = 0; i < lower_len; i++)
    //dprintf("w4[%i] = %i and t1[%i] = %i\r\n", i, result[i], i, t1[i]);
  poly_sub(result, t1, lower_len, w1);

  /* w4 = w4+t1 = t2+t1+t0 */
  poly_add(result, t1, lower_len, result);

  /* w3 = w2*w1 */
  (*chain[depth+1])(w2, lower_len, w1, lower_len, w3, depth+1);

  /* w1 = w0*w4 */
  (*chain[depth+1])(w0, lower_len, result, lower_len, w1, depth+1);

  /* w0 = (w0+k2)*2 -k0 = 4k2+2k1+k0 */
  poly_add(w0, k2, lower_len, w0);
  poly_mul2(w0, lower_len, w0);
  poly_sub(w0, k0, lower_len, w0);

  /* w4 = (w4+t2)*2 -t0 = 4t2+2t1+t0 */
  poly_add(result, t2, lower_len, result);
  poly_mul2(result, lower_len, result);
  poly_sub(result, t0, lower_len, result);

  /* w2 = w0*w4 */
  (*chain[depth+1])(w0, lower_len, result, lower_len, w2, depth+1);

  /* w0 = k0*t0 */
  (*chain[depth+1])(k0, lower_len, t0, lower_len, w0, depth+1);

  /* w4 = k2*t2 */
  (*chain[depth+1])(k2, lower_len, t2, lower_len, result, depth+1);

  /* Interpolation */
  /* From http://marco.bodrato.it/papers/WhatAboutToomCookMatricesOptimality.pdf
     now solve the matrix

       0  0  0  0  1
       1 -1  1 -1  1
       1  1  1  1  1
       16 8  4  2  1
       1  0  0  0  0

       using 8 subtractions, 3 shifts, 
             1 division by 3. (one shift is replaced by a repeated subtraction)
  */

  int len = (lower_len<<1)-1;

  /* w2 = (w2-w3)/3  -> [5 3 1 1 0] */
  //for(int i = 0; i < len; i++)
    //dprintf("w2[%i] = %i and w3[%i] = %i\r\n", i, w2[i], i, w3[i]);
  poly_sub(w2, w3, len, w2);
  //for(int i = 0; i < len; i++)
    //dprintf("Before w2[%i] = %i\r\n", i, w2[i]);
  poly_div3(w2, len, w2);
  //for(int i = 0; i < len; i++)
    //dprintf("After w2[%i] = %i\r\n", i, w2[i]);

  /* w3 = (w1-w3)/2  -> [0 1 0 1 0] */
  //for(int i = 0; i < len; i++)
    //dprintf("w1[%i] = %i and w3[%i] = %i\r\n", i, w1[i], i, w3[i]);
  poly_sub(w1, w3, len, w3);
  //for(int i = 0; i < len; i++)
    //dprintf("Before w3[%i] = %i\r\n", i, w3[i]);
  poly_div2(w3, len, w3);
  //for(int i = 0; i < len; i++)
    //dprintf("After w3[%i] = %i\r\n", i, w3[i]);

  /* w1 = w1-w0  -> [1 1 1 1 0] */
  poly_sub(w1, w0, len, w1);

  /* w2 = (w2-w1)/2 - w4*2 */
  //for(int i = 0; i < len; i++)
    //dprintf("w2[%i] = %i and w1[%i] = %i\r\n", i, w2[i], i, w1[i]);
  poly_sub(w2, w1, len, w2);
  //for(int i = 0; i < len; i++)
    //dprintf("Before w2[%i] = %i\r\n", i, w2[i]);
  poly_div2(w2, len, w2);
  //for(int i = 0; i < len; i++)
    //dprintf("After w2[%i] = %i\r\n", i, w2[i]);
  //for(int i = 0; i < len; i++)
    //dprintf("After w4[%i] = %i\r\n", i, result[i]);
  //w2[0] += 32768;
  //w2[1] += 32768;
  //w2[2] += 32768;
  poly_sub(w2, result, len, w2);
  poly_sub(w2, result, len, w2);

  /* w1 = w1-w3-w4 */
  poly_sub(w1, w3, len, w1);
  poly_sub(w1, result, len, w1);

  /* w3 = w3-w2 */
  poly_sub(w3, w2, len, w3);

  /* at this point shift W[n] by B*n */
  poly_lshd(result, key_length+text_length, lower_len);
  poly_add(w2, result, len, result);
  poly_lshd(result, key_length+text_length, lower_len);
  poly_add(w1, result, len, result);
  poly_lshd(result, key_length+text_length, lower_len);
  poly_add(w3, result, len, result);
  poly_lshd(result, key_length+text_length, lower_len);
  poly_add(w0, result, len, result);

  return 0x00;
}
