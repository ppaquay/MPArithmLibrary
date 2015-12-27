/***********************************************************************
 *                                                                     *
 *     Test  pour la librairie de fonctions de théorie des nombres     *
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

#define MAXTESTLEN BIGINTMAXBIT
#define RND (ulrand64_l() % (MAXTESTLEN + 1))
#define RNDDIGIT (ulrand64_l() % (BIGINTMAXDIGIT + 1))

static void fermat_l(int nbretest);


static void fermat_l(int nbretest)
{
  BIGINT a_l, p_l, q_l, s1_l, s2_l;
  int i;

  printf("Fonction fermat_l()...\n");

  for (i = 0; i < nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(p_l, RND/2);
      while (eqz_l(p_l));
      if (iseven_l(p_l))
	uadd_l(p_l, 1, p_l);
      while (!MRprime_l(p_l, 302, 5))
	uadd_l(p_l, 2, p_l);
      expmodk_l(a_l, p_l, p_l, s1_l); 
      if (!eqmod_l(s1_l, a_l, p_l))
	{
	  printf("Erreur dans MR, lign %d\n", __LINE__);
	  exit (-1);
	}
      do
	rand_l(q_l, RND/2);
      while (eqz_l(q_l));
      if (iseven_l(q_l))
	uadd_l(q_l, 1, q_l);
      while (!SSprime_l(q_l, 302, 5))
	uadd_l(q_l, 2, q_l);
      expmodk_l(a_l, q_l, q_l, s2_l); 
      if (!eqmod_l(s2_l, a_l, q_l))
	{
	  printf("Erreur dans SS, lign %d\n", __LINE__);
	  exit (-1);
	}
    }
}




int main()
{
  printf("Module de test %s pour la librairie de théorie des nombres en C\n\n", __FILE__);
  clock_t t, debut, fin;
  t = 0;

  debut = clock();
  ulseed64_l(time(0));

#ifndef SECURE
  fermat_l(10);
#else
  fermat_l(2);
#endif

  fin = clock();
  t = t + (fin - debut);

  printf("Temps pour %s    = %ld\n", __FILE__, t);


  printf("\nTous les tests du fichier %s effectués.\n", __FILE__);

  return 0;
}
