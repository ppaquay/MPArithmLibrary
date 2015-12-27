/***********************************************************************
 *                                                                     *
 *     Test  pour la librairie de fonctions de nombres aléatoires      *
 *                  pour les grands entiers (4096 bits)                *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arithm.h"

float chisquare(ULONG sample, ULONG seed);
static int testchi(void);
static void randtest_l(unsigned int nbretest);
static int testchiBBS(void);
static void randtestBBS_l(unsigned int nbretest);


static int f[65536];

float chisquare(ULONG sample, ULONG seed)
{
  ULONG i, t;
  USHORT u;

  ulseed64_l(seed);
  for (i = 0; i < 65536; i++)
    f[i] = 0;
  for (i = 0; i < sample; i++)
    {
      u = usrand64_l();
      f[u] = f[u] + 1;
    }
  for (i = 0, t = 0; i < 65536; i++)
    t = t + f[i] * f[i];
  return (float)(((float)65536 * (float)t / (float)sample) - sample);
}

static int testchi(void)
{
  ULONG i, seed, err = 0;
  float X;

  printf("Fonction testchi()...\n");

  for (i = 0; i < 10; i++)
    {
      seed = ulrand64_l();
      X = chisquare(0xa00000, seed);
      if (X >= 66048)
	{
	  printf("Avertissement: Chi carré > w + 2sqrt(w)\n");
	  err = err + 1;
	}
      else
	{
	  if (X <= 65024)
	    {
	      printf("Avertissement: Chi carré < w - 2sqrt(w)\n");
	      err = err + 1;
	    }
	  else
	    printf("Avertissement: Chi carré OK\n");
	}
    }
  if (err > 3)
    printf("Trop d'erreurs dans le test chi carré\n");
  return err;
}

static int testchiBBS(void)
{
  ULONG i, seed, err = 0;
  float X;

  printf("Fonction testchiBBS()...\n");

  for (i = 0; i < 10; i++)
    {
      seed = ulrandBBS_l();
      X = chisquare(0xa00000, seed);
      if (X >= 66048)
	{
	  printf("Avertissement: Chi carré > w + 2sqrt(w)\n");
	  err = err + 1;
	}
      else
	{
	  if (X <= 65024)
	    {
	      printf("Avertissement: Chi carré < w - 2sqrt(w)\n");
	      err = err + 1;
	    }
	  else
	    printf("Avertissement: Chi carré OK\n");
	}
    }
  if (err > 3)
    printf("Trop d'erreurs dans le test chi carré\n");
  return err;
}

static void randtest_l(unsigned int nbretest)
{
  BIGINT a_l;
  BIGINT un_l = { 1, 1 };
  unsigned i, r;

  printf("Fonction rand_test_l()...\n");

  rand_l(a_l, 0);
  if (!eqz_l(a_l))
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }
  rand_l(a_l, 1);
  if (!equ_l(a_l, un_l))
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }
  for (i = 1; i <= nbretest; i++)
    {
      r = ulrand64_l() % BIGINTMAXBIT;
      rand_l(a_l, r);
      if (numbits_l(a_l) != r)
	{
	  printf("Erreur ligne %d\n", __LINE__);
	  exit (-1);
	}
    }
}

static void randtestBBS_l(unsigned int nbretest)
{
  BIGINT a_l;
  BIGINT un_l = { 1, 1 };
  unsigned i, r;

  printf("Fonction randtestBBS_l()...\n");

  randBBS_l(a_l, 0);
  if (!eqz_l(a_l))
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }
  randBBS_l(a_l, 1);
  if (!equ_l(a_l, un_l))
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }
  for (i = 1; i <= nbretest; i++)
    {
      r = ulrandBBS_l() % BIGINTMAXBIT;
      randBBS_l(a_l, r);
      if (numbits_l(a_l) != r)
	{
	  printf("Erreur ligne %d\n", __LINE__);
	  exit (-1);
	}
    }
}



int main()
{
  printf("Module de test %s pour la librairie de nombres aléatoires en C\n\n", __FILE__);

  ulseed64_l(time(0));
  ulseedBBS_l(time(0));
  testchi();
  testchiBBS();
  randtest_l(100);
  randtestBBS_l(100);

  printf("\nTous les tests du fichier %s effectués.\n", __FILE__);

  return 0;
}

