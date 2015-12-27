 /***********************************************************************
 *                                                                     *
 *        Test  pour la librairie de fonctions des restes chinois      *
 *                  pour les grands entiers (4096 bits)                *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libmparithm.h"

#define MAXTESTLEN (BIGINTMAXBIT / 2)
#define RND (1 + ulrand64_l() % (MAXTESTLEN/2))

static void simpletest_l(void);
static void gentest_l(int nbretest);

static void simpletest_l(void)
{
  BIGINT a_l, b_l, m_l, n_l, s_l;
  bigint **coef_l;

  printf("Fonction simpletest_l()...\n");

  coef_l = (bigint **) malloc (sizeof(bigint *) * 8);

  coef_l[0] = a_l;
  coef_l[1] = m_l;
  coef_l[2] = b_l;
  coef_l[3] = n_l;

  rand_l(a_l, RND);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  rand_l(b_l, RND);
  setzero_l(n_l);
  if (chinrem_l(2, coef_l, s_l) == 0)
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }

  rand_l(a_l, RND);
  setone_l(m_l);
  setzero_l(b_l);
  rand_l(n_l, RND);
  if (chinrem_l(2, coef_l, s_l) != 0)
    {
      printf("Erreur ligne %d\n", __LINE__);
      exit (-1);
    }
}

static void gentest_l(int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, n_l, o_l, s_l,g1_l, g2_l;
  bigint **coef_l;
  int i;

  printf("Fonction gentest_l()...\n");

  coef_l = (bigint **) malloc (sizeof(bigint *) * 8);

  coef_l[0] = a_l;
  coef_l[1] = m_l;
  coef_l[2] = b_l;
  coef_l[3] = n_l;
  coef_l[4] = c_l;
  coef_l[5] = o_l;

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(m_l, RND);
      do
	{
	  rand_l(n_l, usrand64_l() % RND / 3);
	  gcd_l(m_l, n_l, g1_l);
	}
      while (!eq1_l(g1_l));
      do
	{
	  rand_l(o_l, usrand64_l() % RND / 3);
	  gcd_l(m_l, o_l, g1_l);
	  gcd_l(n_l, o_l, g2_l);
	}
      while ((!eq1_l(g1_l)) || (!eq1_l(g2_l)));
      rand_l(a_l, usrand64_l() % RND / 4);
      rand_l(b_l, usrand64_l() % RND / 4);
      rand_l(c_l, usrand64_l() % RND / 4);
      chinrem_l(3, coef_l, s_l);
      if ((!eqmod_l(a_l, s_l, m_l)) || (!eqmod_l(b_l, s_l, n_l)) || (!eqmod_l(c_l, s_l, o_l)))
	{
	  printf("Erreur ligne %d\n", __LINE__);
	  exit (-1);
	}
    }
}


int main()
{
  printf("Module de test %s pour la librairie des restes chinois en C\n\n", __FILE__);
  clock_t t, debut, fin;
  t = 0;
  debut = clock();
  ulseed64_l(time(0));

#ifndef SECURE
  simpletest_l();
  gentest_l(10000);
#else
  simpletest_l();
  gentest_l(100);
#endif

  fin = clock();
  t = t + (fin - debut);

  printf("Temps pour %s    = %ld\n", __FILE__, t);
  printf("\nTous les tests du fichier %s effectu�s.\n", __FILE__);

  return 0;
}

