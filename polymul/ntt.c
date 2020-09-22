#include "ntt.h"
#include "dprintf.h"
#include "textbook.h"

/*  Kyber 512  */
#define PRIME_Q         7681
#define DEGREE_KYBER    256
#define EXP             8     // DEGREE_KYBER = 2^EXP

#define int_t       int16_t
#define uint_t      uint16_t
#define int_dt      int32_t
#define uint_dt     uint32_t
#define WORDLENGTH  16

uint_t reverse(uint_t v);
int spmd(int x,int n,int m);
int invers(int x,int y);
int sqrmp(int x,int m);
void precompute_roots_ntt(uint_t *roots, uint_t *iroots);
inline uint_t modmul(int_t a, int_t b);
void forward_ntt(int_t *x, uint_t *roots);
void inverse_ntt(int_t *x,int_t inv,int_t invpr,uint_t *iroots);
void component_multiplication(uint_t *a, uint_t *b, uint_t *c);

// Uncomment for montgomery method
//#define MONTGOMERY

#ifdef MONTGOMERY
#define R (1 << WORDLENGTH)
#define ND 1        //TODO 1/(R-q) mod R
#define ONE (R % PRIME_Q)
#define R2MODP 5569                    // ((R * R) % PRIME_Q)
#endif

/* Mongomery START */
#ifdef MONTGOMERY
inline int_t redc(uint_dt T);
inline int_t nres(uint_t x);

inline int_t redc(uint_dt T)
{
  uint_t m = (uint_t)T * (uint_t) ND;
  return ((uint_dt)m * PRIME_Q + T) >> WORDLENGTH;
}

inline int_t nres(uint_t x)
{
  return redc((uint_dt)x * R2MODP);
}
#endif
/* Mongomery END */

/* reverse bits */
uint_t reverse(uint_t v)
{
  uint_t r = v;
  int s = sizeof(v) * 8 - 1;

  for(v >>= 1; v; v >>= 1){
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;
  r>>=(WORDLENGTH-EXP);
  return r;
}

/*
 * Some number theory functions borrowed from MIRACL
 * https://github.com/miracl/MIRACL
 */
int spmd(int x,int n,int m)
{
  /*  returns x^n mod m  */
  int r,sx;
  x%=m;
  r=0;
  if (x==0) return r;
  r=1;
  if (n==0) return r;
  sx=x;
  for (;;)
  {
    if (n%2!=0) r=((int_dt)r*sx)%m;
    n/=2;
    if (n==0) return r;
    sx=((int_dt)sx*sx)%m;
  }
}

int invers(int x,int y)
{
  /* returns inverse of x mod y */
  int r,s,q,t,p,pos;
  if (y!=0) x%=y;
  r=1;
  s=0;
  p=y;
  pos=1;

  while (p!=0)
  { /* main euclidean loop */
    q=x/p;
    t=r+s*q; r=s; s=t;
    t=x-p*q; x=p; p=t;
    pos=!pos;
  }
  if (!pos) r=y-r;
  return r;
}

int sqrmp(int x,int m)
{
  /*
   * square root mod a small prime =1 mod 8 by Shanks method
   * returns 0 if root does not exist or m not prime
   */
  int z,y,v,w,t,q,i,e,n,r,pp;
  x%=m;
  if (x==0) return 0;
  if (x==1) return 1;
  if (spmd(x,(m-1)/2,m)!=1) return 0;    /* Legendre symbol not 1   */

  q=m-1;
  e=0;
  while (q%2==0) 
  {
    q/=2;
    e++;
  }
  if (e==0) return 0;      /* even m */
  for (r=2;;r++)
  { /* find suitable z */
    z=spmd(r,q,m);
    if (z==1) continue;
    t=z;
    pp=0;
    for (i=1;i<e;i++)
    { /* check for composite m */
        if (t==(m-1)) pp=1;
  t=((int_dt)t*t)%m;
        if (t==1 && !pp) return 0;
    }
    if (t==(m-1)) break;
    if (!pp) return 0;   /* m is not prime */
  }
  y=z;
  r=e;
  v=spmd(x,(q+1)/2,m);
  w=spmd(x,q,m);
  while (w!=1)
  {
    t=w;
    for (n=0;t!=1;n++) t=((int_dt)t*t)%m;
    if (n>=r) return 0;
    y=spmd(y,(1<<(r-n-1)),m);
    v=((int_dt)v*y)%m;
    y=((int_dt)y*y)%m;
    w=((int_dt)w*y)%m;
    r=n;
  }
  return v;
}

/*
 * Precompute roots of unity and its powers
 */
void precompute_roots_ntt(uint_t *roots, uint_t *iroots)
{
  int q = PRIME_Q;
  int proot = q-1;
  for(int j = 0; j < EXP; j++){
    proot = sqrmp(proot,q);
  }
  roots[0] = 1;
  for(int j = 1; j < DEGREE_KYBER; j++){
    roots[reverse(j)] = ((int64_t)proot*roots[reverse(j-1)])%q;
  }
  for(int j = 0; j < DEGREE_KYBER; j++){
    iroots[j] = invers(roots[j],q);
#ifdef MONTGOMERY
    roots[j]  = nres(roots[j]);
    iroots[j] = nres(iroots[j]);
#endif
  }
}

inline uint_t modmul(int_t a, int_t b)
{
#ifdef MONTGOMERY
  return redc((uint_dt)a*b);
#else
  return (int_t)(((int_dt)a*b)%PRIME_Q);
#endif
}

/*
 *  Cooley-Tukey NTT
 *  based on
 *  - https://eprint.iacr.org/2017/727.pdf
 *
 *  int_t *x   = each element muss be in [0, PRIME_Q-1]
 *  int_t *roots  = precomputed table of inverse of 2n-th roots of unity
 */
void forward_ntt(
    int_t *x,
    uint_t *roots)
{
 int t = DEGREE_KYBER/2;
 int_t q = PRIME_Q;
#ifdef MONTGOMERY
  for(int j = 0; j < DEGREE_KYBER; j++){
    x[j] = nres(x[j]);
  }
#endif
 int m = 1;
 int_t S = 0;
 int_t U = 0;
 int_t V = 0;
 while(m < DEGREE_KYBER){
  int k = 0;
  for(int i = 0; i < m; i++){
    S = roots[m + i];
    for(int j = k; j < k + t; j++){
      U = x[j];
      V = modmul(x[j + t], S);
#ifdef MONTGOMERY
      //Lazy Reduction method from page 8
      x[j] = U + V;
      x[j + t] = U + 2*q - V;
#else
      x[j] = (U + V) % q;
      x[j + t] = (U + q - V) % q;
#endif
    }
    k += 2*t;
  }
  t /= 2;
  m *= 2;
 }
}

/*
 *  Gentleman-Sande INTT
 *  based on
 *  - https://eprint.iacr.org/2017/727.pdf
 *
 *  int_t *x   = each element muss be in [0, PRIME_Q-1]
 *  int_t *roots  = precomputed table of inverse of 2n-th roots of unity
 */

void inverse_ntt(
    int_t *x,
    int_t inv,
    int_t invpr,
    uint_t *iroots)
{
  int_t S = 0;
  int_t U = 0;
  int_t V = 0;
  int_t W = 0;
  int_t q = PRIME_Q;

  int t = 1;
  int m = DEGREE_KYBER/2;
  int n = EXP;
#ifdef MONTGOMERY
  int limit = 1;
#else
  (void)(invpr);
  int limit = 0;
#endif
  while (m > limit){
    n--;
    int k = 0;
    for (int i = 0; i < m; i++){
      S=iroots[m + i];
      for (int j = k; j < k+t; j++){
        U=x[j];
        V=x[j+t];
#ifdef MONTGOMERY
        x[j]=U+V;
        W=U+DEGREE_KYBER*q-V;
#else
        x[j] = (U+V)%q;
        W = U+q-V;
#endif
        x[j+t] = modmul(W,S);
      }
      k+=2*t;
    }
    t*=2;
    m/=2;
  }


#ifdef MONTGOMERY
  t = DEGREE_KYBER/2;
  for (int j = 0; j < t; j++){
    U=x[j];
    V=x[j+t];
    W=U+DEGREE_KYBER*q-V;
    x[j+t]=modmul(W,invpr);
    x[j]=modmul(U+V,inv);
  }
#endif

/* Last iteration merged with n^-1 */
  for (int j = 0; j < DEGREE_KYBER; j++){
#ifdef MONTGOMERY
    /* convert back from Montgomery to "normal" form */
    x[j]=redc(x[j]);
    x[j]-=q;
    x[j]+=(x[j]>>(WORDLENGTH-1))&q;
#else
    x[j] = modmul(x[j], inv);
#endif
  }
}

/* component-wise multiplication */
void component_multiplication(uint_t *a, uint_t *b, uint_t *c)
{
  for (int i = 0; i < DEGREE_KYBER; i++){
    c[i] = a[i] * b[i];
  }
}


int ntt(
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

  uint_t roots[DEGREE_KYBER];
  uint_t inv_roots[DEGREE_KYBER];
  precompute_roots_ntt(roots, inv_roots);
  int q = PRIME_Q;

  dprintf("Roots are precomputed\r\n");

#ifdef MONTGOMERY
  int_t inv = nres(invers(DEGREE_KYBER,q));
  int_t invpr = modmul(inv_roots[1],inv);
  inv -= q;
  inv += (inv>>(WORDLENGTH-1))&q;
  invpr -= q;
  invpr += (invpr>>(WORDLENGTH-1))&q;
#else
  int_t inv = invers(DEGREE_KYBER,q);
  int_t invpr = modmul(inv_roots[1],inv);
#endif
  dprintf("Inverse are precomputed\r\n");

  uint16_t ntt_key[1024];
  for (int i = 0; i < 1024; i++){
    ntt_key[i] = key[i];
  }

  dprintf("NTT start\r\n");
  forward_ntt((int_t *) ntt_key, roots);
  dprintf("NTT end\r\n");
  int differ = 0;
  for (int i = 0; i < DEGREE_KYBER + 2; i++){
    if(ntt_key[i] != key[i])
      differ++;
  }
  if (differ > 0){
    dprintf("key != NTT(key)\r\n");
  }else{
    dprintf("Error key == NTT(key)\r\n");
  }
  dprintf("INTT start\r\n");
  inverse_ntt((int_t *) ntt_key, inv, invpr, inv_roots);
  dprintf("INTT end\r\n");

  /*
  for (int i = 0; i < DEGREE_KYBER; i++){
    result[i] = ntt_key[i];
  }
  */

  differ = 0;
  for (int i = 0; i < DEGREE_KYBER + 2; i++){
    if(ntt_key[i] != key[i])
      differ++;
  }
  if (differ > 0){
    dprintf("Error: key != INTT(NTT(key))\r\n");
    dprintf("Failure\r\n");
    return 0x03;
  }else{
    dprintf("key == INTT(NTT(key))\r\n");
    dprintf("Success\r\n");
  }

  textbook_clean(key, key_length, text, text_length, result);

  return 0x00;
}
