/***********************************************************************
 *                                                                     *
 *      Programme principal (test) pour la librairie de fonctions      *
 *    d'arithmétique élémentaire pour les grands entiers (4096 bits)   *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include "arithm.h"

BIGINT a_l = { 6, 42408, 49667, 49634, 44337, 41988, 29479 },
  b_l = { 5, 32965, 36330, 30617, 9061, 54 }, m_l = { 5, 61127, 61135, 38898, 48816, 25726 }, c_l = { 6, 28414, 32662, 62120, 10582, 38610, 43747 };
BIGINT sous_l;
BIGINT som_l;
BIGINT prod_l, carr_l;

main()
{
  printf("TEST DES FONCTIONS ARITHMETIQUES MODULO (BIGINTMAXDIGIT = 6U)\n\n");
  printf("Valeurs initiales :\n");
  printf("a_l = ");
  Ecrit(a_l);
  printf("\n");
  printf("b_l = ");
  Ecrit(b_l);
  printf("\n");
  printf("m_l = ");
  Ecrit(m_l);
  printf("\n");
  printf("c_l = ");
  Ecrit(c_l);
  printf("\n");
  printf("\n\nResultats obtenus par ""arithm.c"" :\n");
  printf("a_l + b_l mod m_l-DBZ = %d\n", madd_l(a_l, b_l, m_l, som_l));
  printf("a_l + b_l mod m_l = ");
  Ecrit(som_l);
  printf("\n");
  printf("a_l - b_l mod m_l-DBZ = %d\n", msub_l(a_l, b_l, m_l, sous_l));
  printf("a_l - b_l mod m_l = ");
  Ecrit(sous_l);
  printf("\n");
  printf("-a_l + b_l mod m_l-DBZ = %d\n", msub_l(b_l, a_l, m_l, sous_l));
  printf("-a_l + b_l mod m_l = ");
  Ecrit(sous_l);
  printf("\n");
  printf("a_l * b_l mod m_l-DBZ = %d\n", mmul_l(a_l, b_l, m_l, prod_l));
  printf("a_l * b_l mod m_l = ");
  Ecrit(prod_l);
  printf("\n");
  printf("a_l * a_l mod m_l-DBZ = %d\n", msqr_l(a_l, m_l, carr_l));
  printf("a_l * a_l mod m_l = ");
  Ecrit(carr_l);
  printf("\n");
  printf("b_l * b_l mod m_l-DBZ = %d\n", msqr_l(b_l, m_l, carr_l));
  printf("b_l * b_l mod m_l = ");
  Ecrit(carr_l);
  printf("\n");
  printf("a_l + 60001 mod m_l-DBZ = %d\n", muadd_l(a_l, 60001,m_l, som_l));
  printf("a_l + 60001 mod m_l = ");
  Ecrit(som_l);
  printf("\n");
  printf("a_l =? c_l mod m_l-OUI = %d\n", mequ_l(a_l, c_l, m_l));
  printf("a_l =? b_l mod m_l-OUI = %d\n", mequ_l(a_l, b_l, m_l));
  printf("\n");


  printf("\n\n");
  printf("Solutions conformes a celles de Mathematica 4.1 (Voir MathematicaEssaiArithMod.nb)\n");
  
  printf("\n");
}
