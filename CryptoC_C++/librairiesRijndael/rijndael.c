/***********************************************************************
 *                                                                     *
 *  Code source pour la librairie des fonctions de cryptage Rijndael   *
 *      (largement inspiré de l'implémentation de Rijmen-Barreto)      *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#include "rijndael.h"

#include "boxes.dat"

/* Fonctions de nettoyage des variables */
/****************************************/

#ifdef SECURE
#define SECMEMSET(X, Y, Z) secmemset((X), (Y), (Z))
static void *secmemset(void*, int, size_t);
#else
#define SECMEMSET(X, Y, Z) (void)0
#endif

/* Prototypes des fonctions kernel */
/***********************************/

static word8 gfmul(word8, word8);
/* Multiplication de 2 éléments de GF(2^8) */
static void AddRoundKey(word8 [4][MAXNB], word32 [MAXNB], word8);
/* Ou exclusif entre le texte clair et la roundkey */
static void ShiftRows(word8 [4][MAXNB], word8);
/* Applique des shifts gauches cycliques à la matrice a */
static void SubBytes(word8 [4][MAXNB], word8);
/* Substitue les bytes par l'intermédiaire des S-box */
static void MixColumns(word8 [4][MAXNB], word8);
/* Multiplie chaque colonne par le polynome a(x) modulo x^4 + 1 */
static void InvShiftRows(word8 [4][MAXNB], word8);
/* Fonction inverse de ShiftRows() */
static void InvSubBytes(word8 [4][MAXNB], word8);
/* Fonction inverse de SubBytes */
static void InvMixColumns(word8 [4][MAXNB], word8);
/* Fonction inverse de MixColumns */

/* Code source des fonctions de nettoyage des variables */
/********************************************************/

#ifdef SECURE
static void *secmemset(void *v, int c, size_t n)
{
  volatile char *p = (volatile char *)v;
  
  while (n--)
    *p++ = c;

  return v;
}
#endif

/* Code source des fonctions kernel */
/************************************/

static word8 gfmul(word8 a, word8 b)
{
  if (a && b)
    return (InvLogtable[(Logtable[a] + Logtable[b]) % 255]);
  else
    return 0;
}

static void AddRoundKey(word8 a[4][MAXNB], word32 rk[MAXNB], word8 NB)
{
  int j;
  
  for (j = 0; j < NB; j++)
    {
      a[0][j] = a[0][j] ^ (word8)(rk[j] & 0xff);
      a[1][j] = a[1][j] ^ (word8)((rk[j] >> 8) & 0xff);
      a[2][j] = a[2][j] ^ (word8)((rk[j] >> 16) & 0xff);
      a[3][j] = a[3][j] ^ (word8)((rk[j] >> 24) & 0xff);
    }
}

static void ShiftRows(word8 a[4][MAXNB], word8 NB)
{
  word8 temp[MAXNB];
  int i, j;

  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < NB; j++)
	temp[j] = a[i][(j + shifts[((NB - 4) >> 1)][i]) % NB];
      for (j = 0; j < NB; j++)
	a[i][j] = temp[j];
    }

  SECMEMSET(temp, 0, MAXNB);

}

static void SubBytes(word8 a[4][MAXNB], word8 NB)
{
  int i, j;
  
  for (i = 0; i < 4; i++)
    for (j = 0; j < NB; j++)
      a[i][j] = S[a[i][j]];
}

static void MixColumns(word8 a[4][MAXNB], word8 NB)
{
  word8 temp[4][MAXNB];
  int i, j;

  for (j = 0; j < NB; j++)
    for (i = 0; i < 4; i++)
      temp[i][j] = gfmul(2, a[i][j]) ^ gfmul(3, a[(i + 1) % 4][j]) ^ a[(i + 2) % 4][j] ^ a[(i + 3) % 4][j];
  for (i = 0; i < 4; i++)
    for (j = 0; j < NB; j++)
      a[i][j] = temp[i][j];

  SECMEMSET(temp, 0, 4 * MAXNB);

}

static void InvShiftRows(word8 a[4][MAXNB], word8 NB)
{
    word8 temp[MAXNB];
  int i, j;

  for (i = 0; i < 4; i++)
    {
      for (j = 0; j < NB; j++)
	temp[j] = a[i][(j + invshifts[((NB - 4) >> 1)][i]) % NB];
      for (j = 0; j < NB; j++)
	a[i][j] = temp[j];
    }

  SECMEMSET(temp, 0, MAXNB);

}

static void InvSubBytes(word8 a[4][MAXNB], word8 NB)
{
  int i, j;
  
  for (i = 0; i < 4; i++)
    for (j = 0; j < NB; j++)
      a[i][j] = Si[a[i][j]];
}

static void InvMixColumns(word8 a[4][MAXNB], word8 NB)
{
  word8 temp[4][MAXNB];
  int i, j;

  for (j = 0; j < NB; j++)
    for (i = 0; i < 4; i++)
      temp[i][j] = gfmul(0xe, a[i][j]) ^ gfmul(0xb, a[(i + 1) % 4][j]) ^ gfmul(0xd, a[(i + 2) % 4][j]) ^ gfmul(0x9, a[(i + 3) % 4][j]);
  for (i = 0; i < 4; i++)
    for (j = 0; j < NB; j++)
      a[i][j] = temp[i][j];

  SECMEMSET(temp, 0, 4 * MAXNB);

}

/* Code source des fonctions externes */
/**************************************/

/* Fonction d'expansion de clé */

int RijndaelKeyExpansion(word8 k[4][MAXNK], int kbits, int bbits, word32 w[(MAXNR + 1) * MAXNB])
{
  int NK, NB, NR;
  int j;
  word32 temp;

  switch (kbits)
    {
    case 128:
      NK = 4;
      break;
    case 192:
      NK = 6;
      break;
    case 256:
      NK = 8;
      break;
    default:
      return -1;
    }

  switch (bbits)
    {
    case 128:
      NB = 4;
      break;
    case 192:
      NB = 6;
      break;
    case 256:
      NB = 8;
      break;
    default:
      return -2;
    }

  switch (kbits >= bbits ? kbits : bbits)
    {
    case 128:
      NR = 10;
      break;
    case 192:
      NR = 12;
      break;
    case 256:
      NR = 14;
      break;
    default:
      return -3;
    }

  for (j = 0; j < NK; j++)
    w[j] = (word32)(k[0][j]) | ((word32)(k[1][j] << 8)) | ((word32)(k[2][j] << 16)) | ((word32)(k[3][j] << 24));
  for (j = NK; j < NB * (NR + 1); j++)
    {
      temp = w[j - 1];
      if (j % NK == 0)
	temp = ((S[(word8)((temp >> 8) & 0xff)]) |
		(S[(word8)((temp >> 16) & 0xff)] << 8) |
		(S[(word8)((temp >> 24) & 0xff)] << 16) |
		(S[(word8)(temp & 0xff)] << 24)) ^ rcon[j / NK - 1];
      else if ((NK > 6) && (j % NK == 4))
	temp = (S[(temp >> 24) & 0xff] << 24) | (S[(temp >> 16) & 0xff] << 16) | (S[(temp >> 8) & 0xff] << 8) | (S[temp & 0xff]);
      w[j] = w[j - NK] ^ temp;
    }

  SECMEMSET(&temp, 0, 4);

  return 0;
}

/* Fonction de cryptage */

int RijndaelEncrypt(word8 a[4][MAXNB], int kbits, int bbits, word32 rk[(MAXNR + 1) * MAXNB])
{
  int r, NB, NR;

  switch (bbits)
    {
    case 128:
      NB = 4;
      break;
    case 192:
      NB = 6;
      break;
    case 256:
      NB = 8;
      break;
    default:
      return -2;
    }

  switch (kbits >= bbits ? kbits : bbits)
    {
    case 128:
      NR = 10;
      break;
    case 192:
      NR = 12;
      break;
    case 256:
      NR = 14;
      break;
    default:
      return -3;
    }

  AddRoundKey(a, rk, NB);
  for (r = 1; r < NR; r++)
    {
      SubBytes(a, NB);
      ShiftRows(a, NB);
      MixColumns(a, NB);
      AddRoundKey(a, (rk + r * NB), NB);
    }
  SubBytes(a, NB);
  ShiftRows(a, NB);
  AddRoundKey(a, (rk + NR * NB), NB);

  return 0;
}

/* Fonction de décryptage */

int RijndaelDecrypt(word8 a[4][MAXNB], int kbits, int bbits, word32 rk[(MAXNR + 1) * MAXNB])
{
  int r, NB, NR;

  switch (bbits)
    {
    case 128:
      NB = 4;
      break;
    case 192:
      NB = 6;
      break;
    case 256:
      NB = 8;
      break;
    default:
      return -2;
    }

  switch (kbits >= bbits ? kbits : bbits)
    {
    case 128:
      NR = 10;
      break;
    case 192:
      NR = 12;
      break;
    case 256:
      NR = 14;
      break;
    default:
      return -3;
    }

  AddRoundKey(a, (rk + NR * NB), NB);
  for (r = NR - 1; r > 0; r--)
    {
      InvShiftRows(a, NB);
      InvSubBytes(a, NB);
      AddRoundKey(a, (rk + r * NB), NB);
      InvMixColumns(a, NB);
    }
  InvShiftRows(a, NB);
  InvSubBytes(a, NB);
  AddRoundKey(a, rk, NB);

  return 0;
}











/* Fin de rijndael.c */

