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

BIGINT a_l = { 6, 47904, 44822, 57466, 26954, 23549, 36110 },
  b_l = { 6, 11754, 27770, 59046, 37388, 47029, 45318 };
BIGINT sous_l;
BIGINTP som_l, uprod_l;
BIGINTD prod_l, carr_l;

main()
{
  printf("TEST DES FONCTIONS KERNEL (BIGINTMAXDIGIT = 6U)\n\n");
  printf("Valeurs initiales :\n");
  printf("a_l = ");
  Ecrit(a_l);
  printf("\n");
  printf("b_l = ");
  Ecrit(b_l);
  printf("\n\nResultats obtenus par ""arithm.c"" :\n");
  add(a_l, b_l, som_l);
  printf("a_l + b_l = ");
  Ecrit(som_l);
  printf("\n");
  sub(b_l, a_l, sous_l);
  printf("b_l - a_l = ");
  Ecrit(sous_l);
  printf("\n");
  mul(a_l, b_l, prod_l);
  printf("a_l * b_l = ");
  Ecrit(prod_l);
  printf("\n");
  umul(a_l, 60000, uprod_l);
  printf("a_l * 60000 = ");
  Ecrit(uprod_l);
  printf("\n");
  sqr(a_l, carr_l);
  printf("a_l * a_l = ");
  Ecrit(carr_l);
  printf("\n");
  sqr(b_l, carr_l);
  printf("b_l * b_l = ");
  Ecrit(carr_l);
  printf("\n\n");
  printf("Solutions conformes a celles de Mathematica 4.1 (Voir MathematicaEssaiKernel.nb)\n");
  
  printf("\n");
}
