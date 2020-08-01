#include "config.h"
#include "common_config.h"

#define ONE_THIRD   (MAX_SS_LEN + 2)/3
#define TWO_THIRD   ((MAX_SS_LEN + 2)*2)/3
#define INVERSE_3 43691

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

  /* Evaluation phase */
  for (int i = 0; i < lower_len; i++){
    /* w0 = k0+k2 */
    w0[i] = k0[i] + k2[i];
    /* w2 = w0-k1 = k2-k1+k0 */
    w2[i] = w0[i] - k1[i];
    /* w0 = w0+k1 = k2+k1+k0 */
    w0[i] = w0[i] + k1[i];

    /* w4 = t0+t2 */
    result[i] = t0[i] + t2[i];
    /* w1 = w4-t1 = t2-t1+t0 */
    w1[i] = result[i] - t1[i];
    /* w4 = w4+t1 = t2+t1+t0 */
    result[i] = result[i] + t1[i];
  }

  /* w3 = w2*w1 */
  (*chain[depth+1])(w2, lower_len, w1, lower_len, w3, depth+1);

  /* w1 = w0*w4 */
  (*chain[depth+1])(w0, lower_len, result, lower_len, w1, depth+1);

  for (int i = 0; i < lower_len; i++){
    /* w0 = (w0+k2)*2 -k0 = 4k2+2k1+k0 */
    w0[i] = w0[i] + k2[i];
    w0[i] = w0[i] << 1;
    w0[i] = w0[i] - k0[i];

    /* w4 = (w4+t2)*2 -t0 = 4t2+2t1+t0 */
    result[i] = result[i] + t2[i];
    result[i] = result[i] << 1;
    result[i] = result[i] - t0[i];
  }

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

  for (int i = 0; i < len; i++){
    /* w2 = (w2-w3)/3  -> [5 3 1 1 0] */
    w2[i] = w2[i] - w3[i];
    w2[i] = (uint16_t)(w2[i] * INVERSE_3);

    /* w3 = (w1-w3)/2  -> [0 1 0 1 0] */
    w3[i] = w1[i] - w3[i];
    w3[i] = w3[i] >> 1;

    /* w1 = w1-w0  -> [1 1 1 1 0] */
    w1[i] = w1[i] - w0[i];

    /* w2 = (w2-w1)/2 - w4*2 */
    w2[i] = w2[i] - w1[i];
    w2[i] = w2[i] >> 1;
    w2[i] = w2[i] - result[i];
    w2[i] = w2[i] - result[i];

    /* w1 = w1-w3-w4 */
    w1[i] = w1[i] - w3[i];
    w1[i] = w1[i] - result[i];

    /* w3 = w3-w2 */
    w3[i] = w3[i] - w2[i];
  }

  /* at this point shift W[n] by B*n */
  int total_len = key_length + text_length - 1 - len;
  for (int i = len - 1; i >= 0; i--){
    result[total_len + i] = result[i];
    result[i] = w0[i];
  }
  total_len = total_len - 2 * lower_len;
  for (int i = len - 1; i >= 0; i--){
    result[total_len + i] = w1[i];
  }
  int total_len_2 = total_len - lower_len;
  total_len += lower_len;
  for (int i = len - 1; i >= 0; i--){
    result[total_len + i] += w2[i];
  }
  for (int i = len - 1; i >= 0; i--){
    result[total_len_2 + i] += w3[i];
  }

  return 0x00;
}
