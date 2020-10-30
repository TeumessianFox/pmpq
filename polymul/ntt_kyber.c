#include "ntt_kyber.h"
#include "dprintf.h"
#include "textbook.h"

#define KYBER_N 256
#define KYBER_Q 3329
#define MONT 2285 // 2^16 mod q
#define QINV 62209 // q^-1 mod 2^16

#define KYBER_ETA 2

/* Code to generate zetas and zetas_inv used in the number-theoretic transform:

#define KYBER_ROOT_OF_UNITY 17

static const uint16_t tree[128] = {
  0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
  4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
  2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
  6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
  1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
  5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
  3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
  7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127
};

void init_ntt() {
  unsigned int i, j, k;
  int16_t tmp[128];

  tmp[0] = MONT;
  for(i = 1; i < 128; ++i)
    tmp[i] = fqmul(tmp[i-1], KYBER_ROOT_OF_UNITY*MONT % KYBER_Q);

  for(i = 0; i < 128; ++i)
    zetas[i] = tmp[tree[i]];

  k = 0;
  for(i = 64; i >= 1; i >>= 1)
    for(j = i; j < 2*i; ++j)
      zetas_inv[k++] = -tmp[128 - tree[j]];

  zetas_inv[127] = MONT * (MONT * (KYBER_Q - 1) * ((KYBER_Q - 1)/128) % KYBER_Q) % KYBER_Q;
}

*/

const int16_t zetas[128] = {
  2285, 2571, 2970, 1812, 1493, 1422, 287, 202, 3158, 622, 1577, 182, 962,
  2127, 1855, 1468, 573, 2004, 264, 383, 2500, 1458, 1727, 3199, 2648, 1017,
  732, 608, 1787, 411, 3124, 1758, 1223, 652, 2777, 1015, 2036, 1491, 3047,
  1785, 516, 3321, 3009, 2663, 1711, 2167, 126, 1469, 2476, 3239, 3058, 830,
  107, 1908, 3082, 2378, 2931, 961, 1821, 2604, 448, 2264, 677, 2054, 2226,
  430, 555, 843, 2078, 871, 1550, 105, 422, 587, 177, 3094, 3038, 2869, 1574,
  1653, 3083, 778, 1159, 3182, 2552, 1483, 2727, 1119, 1739, 644, 2457, 349,
  418, 329, 3173, 3254, 817, 1097, 603, 610, 1322, 2044, 1864, 384, 2114, 3193,
  1218, 1994, 2455, 220, 2142, 1670, 2144, 1799, 2051, 794, 1819, 2475, 2459,
  478, 3221, 3021, 996, 991, 958, 1869, 1522, 1628
};

const int16_t zetas_inv[128] = {
  1701, 1807, 1460, 2371, 2338, 2333, 308, 108, 2851, 870, 854, 1510, 2535,
  1278, 1530, 1185, 1659, 1187, 3109, 874, 1335, 2111, 136, 1215, 2945, 1465,
  1285, 2007, 2719, 2726, 2232, 2512, 75, 156, 3000, 2911, 2980, 872, 2685,
  1590, 2210, 602, 1846, 777, 147, 2170, 2551, 246, 1676, 1755, 460, 291, 235,
  3152, 2742, 2907, 3224, 1779, 2458, 1251, 2486, 2774, 2899, 1103, 1275, 2652,
  1065, 2881, 725, 1508, 2368, 398, 951, 247, 1421, 3222, 2499, 271, 90, 853,
  1860, 3203, 1162, 1618, 666, 320, 8, 2813, 1544, 282, 1838, 1293, 2314, 552,
  2677, 2106, 1571, 205, 2918, 1542, 2721, 2597, 2312, 681, 130, 1602, 1871,
  829, 2946, 3065, 1325, 2756, 1861, 1474, 1202, 2367, 3147, 1752, 2707, 171,
  3127, 3042, 1907, 1836, 1517, 359, 758, 1441
};

int16_t montgomery_reduce(int32_t a);
int16_t barrett_reduce(int16_t a);
void poly_reduce(int16_t r[256]);
static int16_t fqmul(int16_t a, int16_t b);
void basemul(
    const int16_t a[2],
    const int16_t b[2],
    int16_t zeta,
    int16_t r[2]);
void ntt(int16_t r[256]);
void invntt(int16_t r[256]);
void basemul_montgomery(
    const int16_t a[256],
    const int16_t b[256],
    int16_t r[256]);

/*************************************************
* Name:        montgomery_reduce
*
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q,
*              where R=2^16
*
* Arguments:   - int32_t a: input integer to be reduced;
*                           has to be in {-q2^15,...,q2^15-1}
*
* Returns:     integer in {-q+1,...,q-1} congruent to a * R^-1 modulo q.
**************************************************/
int16_t montgomery_reduce(int32_t a)
{
  int32_t t;
  int16_t u;

  u = a*QINV;
  t = (int32_t)u*KYBER_Q;
  t = a - t;
  t >>= 16;
  return t;
}

/*************************************************
* Name:        barrett_reduce
*
* Description: Barrett reduction; given a 16-bit integer a, computes
*              16-bit integer congruent to a mod q in {0,...,q}
*
* Arguments:   - int16_t a: input integer to be reduced
*
* Returns:     integer in {0,...,q} congruent to a modulo q.
**************************************************/
int16_t barrett_reduce(int16_t a)
{
  int16_t t;
  const int16_t v = ((1U << 26) + KYBER_Q/2)/KYBER_Q;

  t  = (int32_t)v*a >> 26;
  t *= KYBER_Q;
  return a - t;
}

/*************************************************
* Name:        poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - int16_t r[256]: pointer to input/output polynomial
**************************************************/
void poly_reduce(int16_t r[256])
{
  unsigned int i;
  for(i=0;i<KYBER_N;i++)
    r[i] = barrett_reduce(r[i]);
}

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/
static int16_t fqmul(int16_t a, int16_t b)
{
  return montgomery_reduce((int32_t)a*b);
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/(X^2-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[2]:       pointer to the output polynomial
*              - const int16_t a[2]: pointer to the first factor
*              - const int16_t b[2]: pointer to the second factor
*              - int16_t zeta:       integer defining the reduction polynomial
**************************************************/
void basemul(
    const int16_t a[2],
    const int16_t b[2],
    int16_t zeta,
    int16_t r[2])
{
  r[0]  = fqmul(a[1], b[1]);
  r[0]  = fqmul(r[0], zeta);
  r[0] += fqmul(a[0], b[0]);

  r[1]  = fqmul(a[0], b[1]);
  r[1] += fqmul(a[1], b[0]);
}

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements
*                                of Zq
**************************************************/
void ntt(int16_t r[256])
{
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  for(len = 128; len >= 2; len >>= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k++];
      for(j = start; j < start + len; ++j) {
        t = fqmul(zeta, r[j + len]);
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements
*                                of Zq
**************************************************/
void invntt(int16_t r[256])
{
  unsigned int start, len, j, k;
  int16_t t, zeta;

  k = 0;
  for(len = 2; len <= 128; len <<= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas_inv[k++];
      for(j = start; j < start + len; ++j) {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = t - r[j + len];
        r[j + len] = fqmul(zeta, r[j + len]);
      }
    }
  }

  for(j = 0; j < 256; ++j)
    r[j] = fqmul(r[j], zetas_inv[127]);
}

/*************************************************
* Name:        basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments    - const int16_t a[256]: pointer to first input polynomial
*              - const int16_t b[256]: pointer to second input polynomial
*              - int16_t r[256]:       pointer to output polynomial
**************************************************/
void basemul_montgomery(
    const int16_t a[256],
    const int16_t b[256],
    int16_t r[256])
{
  unsigned int i;
  for(i=0;i<KYBER_N/4;i++) {
    basemul(&a[4*i], &b[4*i], zetas[64+i], &r[4*i]);
    basemul(&a[4*i+2], &b[4*i+2], -zetas[64+i], &r[4*i+2]);
  }
}

int ntt_kyber(
    uint16_t *key,
    int key_length,
    uint16_t *text,
    int text_length,
    uint16_t *result)
{
  if(key_length != text_length)
    return 0x01;
  if(key_length % 2 != 0)
    return 0x02;

  /*
  uint16_t ntt_key[256];
  for (int i = 0; i < KYBER_N; i++){
    ntt_key[i] = key[i];
  }

  dprintf("NTT start\r\n");
  ntt((int16_t *) ntt_key);
  //poly_reduce((int16_t *) ntt_key);
  dprintf("NTT end\r\n");

  dprintf("INTT start\r\n");
  invntt((int16_t *) ntt_key);
  //poly_reduce((int16_t *) ntt_key);
  dprintf("INTT end\r\n");

  int counter = 0;
  for (int i = 0; i < KYBER_N; i++){
    if (ntt_key[i] != key[i]){
      counter++;
    }
  }
  dprintf("After INTT: Differs in %i\r\n", counter);

  if (counter == 0){
    textbook_clean(key, key_length, text, text_length, result);
  }
  */

  dprintf("NTT start\r\n");
  ntt((int16_t *) text);
  poly_reduce((int16_t *) text);
  dprintf("NTT end\r\n");

  basemul_montgomery((int16_t *)key, (int16_t *)text, (int16_t *)result);
  poly_reduce((int16_t *) result);

  dprintf("INTT start\r\n");
  invntt((int16_t *) result);
  poly_reduce((int16_t *) result);
  dprintf("INTT end\r\n");

  return 0x00;
}
