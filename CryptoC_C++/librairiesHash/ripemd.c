/***********************************************************************
 *                                                                     *
 *   Code source pour la librairie de fonctions de hachage RIPEMD160   *
 *     en multi-précision (4096 bits si ARCH32 et 8192 si ARCH64)      *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "ripemd.h"


#ifdef ARCH32
/* Prototypes des fonctions kernel RIPEMD160 (ARCH32) */

static ULONG cycle(ULONG, int);
static void chain(ULONG*, ULONG*, ULONG*, ULONG*, ULONG*, ULONG*, int);
static ULONG* uc2ul(UCHAR*);
static void addc(ULONG[], ULONG);
static void appetize(ULONG*);
static void swallow(ULONG*, ULONG*);
static void digest(ULONG*, UCHAR*, ULONG[]);


/* Fonctions kernel RIPEMD160 (ARCH32) */

static ULONG cycle(ULONG x, int n)
{
  ULONG rol;

  rol = (x << n) | (x >> (32 - n));
  
  return rol;
}

static void chain(ULONG* a, ULONG* b, ULONG* c, ULONG* d, ULONG* e, ULONG* x, int n)
{
  *a = *e;
  *e = *d;
  *d = cycle(*c, 10);
  *c = *b;
  *b = cycle(*x, n) + *a;/* Addition modulo 2^32 */

  return;
}

static ULONG* uc2ul(UCHAR* ucptr)
{
  ULONG* ulptr;

#ifdef LITTLE_ENDIAN
  ulptr = (ULONG*)ucptr;
#else
  ulptr = ((ULONG)*(ucptr + 3) << 24) | ((ULONG)*(ucptr + 2) << 16) | ((ULONG)*(ucptr + 1) << 8) | ((ULONG)*ucptr);
#endif

  return ulptr;
}

static void addc(ULONG a[2], ULONG b)
{
  if ((a[0] + b) < a[0]) /* Attention comparaison marche car modulo 2^32 */
    a[1] = a[1] + 1;
  a[0] = a[0] + b;
}

static const int r1[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
			  7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
			  3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
			  1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
			  4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13};

static const int r2[] = { 5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
			  6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
			  15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
			  8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
			  12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11};

static const int s1[] = {11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
			 7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
			 11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
			 11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
			 9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6};

static const int s2[] = { 8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
			  9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
			  9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
			  15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
			  8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11};

static void appetize(ULONG* stomach)
{
  stomach[0] = 0x67452301UL;
  stomach[1] = 0xefcdab89UL;
  stomach[2] = 0x98badcfeUL;
  stomach[3] = 0x10325476UL;
  stomach[4] = 0xc3d2e1f0UL;

  return;
}

static void swallow(ULONG* stomach, ULONG* ULBlock)
{
  int i, rol;
  ULONG x, a1, b1, c1, d1, e1, a2, b2, c2, d2, e2;

  a1 = stomach[0];
  b1 = stomach[1];
  c1 = stomach[2];
  d1 = stomach[3];
  e1 = stomach[4];
  
  a2 = stomach[0];
  b2 = stomach[1];
  c2 = stomach[2];
  d2 = stomach[3];
  e2 = stomach[4];

  for (i = 0; i < 16; i++)
    {
      rol = s1[i];
      x = a1 + (b1 ^ c1 ^ d1) + ULBlock[i];
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = a2 + (b2 ^ (c2 | ~d2)) + ULBlock[r2[i]] + 0x50a28be6UL;
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 16; i < 32; i++)
    {
      rol = s1[i];
      x = a1 + (d1 ^ (b1 & (c1 ^ d1))) + ULBlock[r1[i]] + 0x5a827999UL;
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = a2 + (c2 ^ (d2 & (b2 ^ c2))) + ULBlock[r2[i]] + 0x5c4dd124UL;
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 32; i < 48; i++)
    {
      rol = s1[i];
      x = a1 + ((b1 | ~c1) ^ d1) + ULBlock[r1[i]] + 0x6ed9eba1UL;
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = a2 + ((b2 | ~c2) ^ d2) + ULBlock[r2[i]] + 0x6d703ef3UL;
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 48; i < 64; i++)
    {
      rol = s1[i];
      x = a1 + (c1 ^ (d1 & (b1 ^ c1))) + ULBlock[r1[i]] + 0x8f1bbcdcUL;
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);

      rol = s2[i];
      x = a2 + (d2 ^ (b2 & (c2 ^ d2))) + ULBlock[r2[i]] + 0x7a6d76e9UL;
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 64; i < 80; i++)
    {
      rol = s1[i];
      x = a1 + (b1 ^ (c1 | ~d1)) + ULBlock[r1[i]] + 0xa953fd4eUL;
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);

      rol = s2[i];
      x = a2 + (b2 ^ c2 ^ d2) + ULBlock[r2[i]];
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  d2 = d2 + c1 + stomach[1];
  stomach[1] = stomach[2] + d1 + e2;
  stomach[2] = stomach[3] + e1 + a2;
  stomach[3] = stomach[4] + a1 + b2;
  stomach[4] = stomach[0] + b1 + c2;
  stomach[0] = d2;

  return;
}

static void digest (ULONG* stomach, UCHAR* clear, ULONG total[])
{
  ULONG i, j, rest;
  ULONG ULBlock[16];
  
  memset(ULBlock, 0, sizeof(ULONG) << 4);
  rest = total[0] & 0x3f;
  for (i = 0; i < (rest >> 2); i++)
    {
      ULBlock[i] = *uc2ul(clear);
      clear = clear + 4;
    }
  for (j = (i << 2); j < rest; j++)
    {
      ULBlock[i] = ULBlock[i] | (ULONG)*clear << ((j & 3) << 3);
      clear = clear + 1;
    }
  ULBlock[i] = ULBlock[i] | (ULONG)0x80 << ((j & 3) << 3);
  if (rest > 55)
    {
      swallow(stomach, ULBlock);
      memset(ULBlock, 0, sizeof(ULONG) << 4);
    }
  ULBlock[14] = total[0] << 3;
  ULBlock[15] = (total[0] >> 29) | (total[1] << 3);
  swallow(stomach, ULBlock);

  return;
}

/* Fonctions externes RIPEMD160 */

void ripeinit(RMDSTAT *hws)
{
  appetize(hws->stomach);
  hws->total[0] = 0;
  hws->total[1] = 0;
}

int ripehash(RMDSTAT* hws, UCHAR* clear, ULONG length)
{
  ULONG ULBlock[16];
  ULONG noofblocks;
  ULONG i,j;

  if (length & 63)
    return BIGINT_RMD;
  noofblocks = length >> 6;
  for (i = 0; i < noofblocks; i++)
    {
      for (j = 0; j < 16; j++)
	{
	  ULBlock[j] = *uc2ul(clear);
	  clear = clear + 4;
	}
      swallow(hws->stomach, ULBlock);
    }
  addc(hws->total, length);
  
  return BIGINT_OK;
}

UCHAR* ripefinish(RMDSTAT* hws, UCHAR* clear, ULONG length)
{
  unsigned int i;
  static UCHAR ripemd[RMDVER >> 3];
  ULONG blength = (length >> 6) << 6;

  ripehash(hws, clear, blength);
  addc(hws->total, length - blength);
  digest(hws->stomach, clear + blength, hws->total);
  for (i = 0; i < (RMDVER >> 3); i += 4)
    {
      ripemd[i] = (UCHAR)hws->stomach[i>>2];
      ripemd[i + 1] = (UCHAR)(hws->stomach[i>>2] >> 8);
      ripemd[i + 2] = (UCHAR)(hws->stomach[i>>2] >> 16);
      ripemd[i + 3] = (UCHAR)(hws->stomach[i>>2] >> 24);
    }

  return (UCHAR*)ripemd;
}

UCHAR* ripemd160(UCHAR* clear, ULONG length)
{
  RMDSTAT hws;

  ripeinit(&hws);
  
  return (ripefinish(&hws, clear, length));
}
#endif

#ifdef ARCH64
/* Prototypes des fonctions kernel RIPEMD160 (ARCH64) */

static unsigned int cycle(unsigned int, int);
static void chain(unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*, int);
static unsigned int* uc2ul(UCHAR*);
static void addc(unsigned int[], unsigned int);
static void appetize(unsigned int*);
static void swallow(unsigned int*, unsigned int*);
static void digest(unsigned int*, UCHAR*, unsigned int[]);


/* Fonctions kernel RIPEMD160 (ARCH64) */

static unsigned int cycle(unsigned int x, int n)
{
  unsigned int rol;

  rol = (unsigned int)(x << n) | (unsigned int)(x >> (32 - n));
  
  return rol;
}

static void chain(unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d, unsigned int* e, unsigned int* x, int n)
{
  *a = *e;
  *e = *d;
  *d = cycle(*c, 10);
  *c = *b;
  *b = (unsigned int)(cycle(*x, n) + *a);/* Addition modulo 2^32 */

  return;
}

static unsigned int* uc2ul(UCHAR* ucptr)
{
  unsigned int* ulptr;

#ifdef LITTLE_ENDIAN
  ulptr = (unsigned int*)ucptr;
#else
  ulptr = ((unsigned int)*(ucptr + 3) << 24) | ((unsigned int)*(ucptr + 2) << 16) | ((unsigned int)*(ucptr + 1) << 8) | ((unsigned int)*ucptr);
#endif

  return ulptr;
}

static void addc(unsigned int a[2], unsigned int b)
{
  if ((unsigned int)(a[0] + b) < a[0]) /* Attention comparaison marche car modulo 2^32 */
    a[1] = (unsigned int)(a[1] + 1);
  a[0] = (unsigned int)(a[0] + b);
}


static const int r1[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
			  7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
			  3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
			  1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
			  4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13};

static const int r2[] = { 5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
			  6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
			  15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
			  8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
			  12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11};

static const int s1[] = {11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
			 7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
			 11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
			 11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
			 9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6};

static const int s2[] = { 8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
			  9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
			  9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
			  15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
			  8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11};

static void appetize(unsigned int* stomach)
{
  stomach[0] = 0x67452301U;
  stomach[1] = 0xefcdab89U;
  stomach[2] = 0x98badcfeU;
  stomach[3] = 0x10325476U;
  stomach[4] = 0xc3d2e1f0U;

  return;
}

static void swallow(unsigned int* stomach, unsigned int* ULBlock)
{
  int i, rol;
  unsigned int x, a1, b1, c1, d1, e1, a2, b2, c2, d2, e2;

  a1 = stomach[0];
  b1 = stomach[1];
  c1 = stomach[2];
  d1 = stomach[3];
  e1 = stomach[4];
  
  a2 = stomach[0];
  b2 = stomach[1];
  c2 = stomach[2];
  d2 = stomach[3];
  e2 = stomach[4];

  for (i = 0; i < 16; i++)
    {
      rol = s1[i];
      x = (unsigned int)(a1 + (b1 ^ c1 ^ d1) + ULBlock[i]);
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = (unsigned int)(a2 + (b2 ^ (c2 | ~d2)) + ULBlock[r2[i]] + 0x50a28be6U);
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 16; i < 32; i++)
    {
      rol = s1[i];
      x = (unsigned int)(a1 + (d1 ^ (b1 & (c1 ^ d1))) + ULBlock[r1[i]] + 0x5a827999U);
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = (unsigned int)(a2 + (c2 ^ (d2 & (b2 ^ c2))) + ULBlock[r2[i]] + 0x5c4dd124U);
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 32; i < 48; i++)
    {
      rol = s1[i];
      x = (unsigned int)(a1 + ((b1 | ~c1) ^ d1) + ULBlock[r1[i]] + 0x6ed9eba1U);
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);
      
      rol = s2[i];
      x = (unsigned int)(a2 + ((b2 | ~c2) ^ d2) + ULBlock[r2[i]] + 0x6d703ef3U);
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 48; i < 64; i++)
    {
      rol = s1[i];
      x = (unsigned int)(a1 + (c1 ^ (d1 & (b1 ^ c1))) + ULBlock[r1[i]] + 0x8f1bbcdcU);
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);

      rol = s2[i];
      x = (unsigned int)(a2 + (d2 ^ (b2 & (c2 ^ d2))) + ULBlock[r2[i]] + 0x7a6d76e9U);
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  for (i = 64; i < 80; i++)
    {
      rol = s1[i];
      x = (unsigned int)(a1 + (b1 ^ (c1 | ~d1)) + ULBlock[r1[i]] + 0xa953fd4eU);
      chain(&a1, &b1, &c1, &d1, &e1, &x, rol);

      rol = s2[i];
      x = (unsigned int)(a2 + (b2 ^ c2 ^ d2) + ULBlock[r2[i]]);
      chain(&a2, &b2, &c2, &d2, &e2, &x, rol);
    }
  d2 = (unsigned int)(d2 + c1 + stomach[1]);
  stomach[1] = (unsigned int)(stomach[2] + d1 + e2);
  stomach[2] = (unsigned int)(stomach[3] + e1 + a2);
  stomach[3] = (unsigned int)(stomach[4] + a1 + b2);
  stomach[4] = (unsigned int)(stomach[0] + b1 + c2);
  stomach[0] = d2;

  return;
}

static void digest (unsigned int* stomach, UCHAR* clear, unsigned int total[])
{
  unsigned int i, j, rest;
  unsigned int ULBlock[16];
  
  memset(ULBlock, 0, sizeof(unsigned int) << 4);
  rest = total[0] & 0x3f;
  for (i = 0; i < (rest >> 2); i++)
    {
      ULBlock[i] = *uc2ul(clear);
      clear = clear + 4;
    }
  for (j = (i << 2); j < rest; j++)
    {
      ULBlock[i] = ULBlock[i] | (unsigned int)*clear << ((j & 3) << 3);
      clear = clear + 1;
    }
  ULBlock[i] = ULBlock[i] | (unsigned int)0x80 << ((j & 3) << 3);
  if (rest > 55)
    {
      swallow(stomach, ULBlock);
      memset(ULBlock, 0, sizeof(unsigned int) << 4);
    }
  ULBlock[14] = total[0] << 3;
  ULBlock[15] = (total[0] >> 29) | (total[1] << 3);
  swallow(stomach, ULBlock);

  return;
}

/* Fonctions externes RIPEMD160 (ARCH64) */

void ripeinit(RMDSTAT *hws)
{
  appetize(hws->stomach);
  hws->total[0] = 0;
  hws->total[1] = 0;
}

int ripehash(RMDSTAT* hws, UCHAR* clear, unsigned int length)
{
  unsigned int ULBlock[16];
  unsigned int noofblocks;
  unsigned int i,j;

  if (length & 63)
    return BIGINT_RMD;
  noofblocks = length >> 6;
  for (i = 0; i < noofblocks; i++)
    {
      for (j = 0; j < 16; j++)
	{
	  ULBlock[j] = *uc2ul(clear);
	  clear = clear + 4;
	}
      swallow(hws->stomach, ULBlock);
    }
  addc(hws->total, length);
  
  return BIGINT_OK;
}

UCHAR* ripefinish(RMDSTAT* hws, UCHAR* clear, unsigned int length)
{
  unsigned int i;
  static UCHAR ripemd[RMDVER >> 3];
  unsigned int blength = (length >> 6) << 6;

  ripehash(hws, clear, blength);
  addc(hws->total, length - blength);
  digest(hws->stomach, clear + blength, hws->total);
  for (i = 0; i < (RMDVER >> 3); i += 4)
    {
      ripemd[i] = (UCHAR)hws->stomach[i>>2];
      ripemd[i + 1] = (UCHAR)(hws->stomach[i>>2] >> 8);
      ripemd[i + 2] = (UCHAR)(hws->stomach[i>>2] >> 16);
      ripemd[i + 3] = (UCHAR)(hws->stomach[i>>2] >> 24);
    }

  return (UCHAR*)ripemd;
}

UCHAR* ripemd160(UCHAR* clear, unsigned int length)
{
  RMDSTAT hws;

  ripeinit(&hws);
  
  return (ripefinish(&hws, clear, length));
}
#endif

/* Fin de ripemd.c */
