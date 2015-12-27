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

BIGINT a_l = { 6, 40736, 802, 57469, 26954, 23549, 36110 },
  b_l = { 6, 21754, 27770, 59046, 37388, 47029, 45318 }, c_l = { 3, 57169, 4220, 8 };
BIGINT sous_l;
BIGINT som_l;
BIGINT prod_l, carr_l, rest_l;
BIGINTM quo_l;

main()
{
  printf("TEST DES FONCTIONS ARITHMETIQUES (BIGINTMAXDIGIT = 6U)\n\n");
  printf("Valeurs initiales :\n");
  printf("a_l = ");
  Ecrit(a_l);
  printf("\n");
  printf("b_l = ");
  Ecrit(b_l);
  printf("\n");
  printf("c_l = ");
  Ecrit(c_l);
  printf("\n");
  printf("\n\nResultats obtenus par ""arithm.c"" :\n");
  printf("a_l + b_l-OFL = %d\n", add_l(a_l, b_l, som_l));
  printf("a_l + b_l = ");
  Ecrit(som_l);
  printf("\n");
  printf("a_l + 60000-OFL = %d\n", uadd_l(a_l, 60000, som_l));
  printf("a_l + 60000 = ");
  Ecrit(som_l);
  printf("\n");
  printf("a_l - b_l-UFL = %d\n", sub_l(a_l, b_l, sous_l));
  printf("a_l - b_l = ");
  Ecrit(sous_l);
  printf("\n");
  printf("a_l - 60000-UFL = %d\n", usub_l(a_l, 60000, sous_l));
  printf("a_l - 60000 = ");
  Ecrit(sous_l);
  printf("\n");
  printf("a_l * b_l-OFL = %d\n", mul_l(a_l, b_l, prod_l));
  printf("a_l * b_l = ");
  Ecrit(prod_l);
  printf("\n");
  printf("a_l * 60000-OFL = %d\n", umul_l(a_l, 60000, prod_l));
  printf("a_l * 60000 = ");
  Ecrit(prod_l);
  printf("\n");
  printf("a_l * a_l-OFL = %d\n", sqr_l(a_l, carr_l));
  printf("a_l * a_l = ");
  Ecrit(carr_l);
  printf("\n");
  printf("b_l * b_l-OFL = %d\n", sqr_l(b_l, carr_l));
  printf("b_l * b_l = ");
  Ecrit(carr_l);
  printf("\n");
  printf("b_l / a_l-DBZ = %d\n", div_l(b_l, a_l, quo_l,rest_l));
  printf("b_l / a_l = ");
  Ecrit(quo_l);
  printf("\n");
  printf("b_l mod a_l = ");
  Ecrit(rest_l);
  printf("\n");
  sqr(b_l, carr_l);
  printf("b_l * b_l / c_l-DBZ = %d\n", div_l(carr_l, c_l, quo_l,rest_l));
  printf("b_l * b_l / c_l = ");
  Ecrit(quo_l);
  printf("\n");
  printf("b_l * b_l mod c_l = ");
  Ecrit(rest_l);
  printf("\n");
  printf("a_l / 60000-DBZ = %d\n", udiv_l(a_l, 60000, quo_l,rest_l));
  printf("a_l / 60000 = ");
  Ecrit(quo_l);
  printf("\n");
  printf("a_l mod 60000 = ");
  Ecrit(rest_l);
  printf("\n");
  printf("b_l / a_l-DBZ = %d\n", mod_l(b_l, a_l, rest_l));
  printf("b_l mod a_l = ");
  Ecrit(rest_l);
  printf("\n");
  printf("a_l mod 60000 = %d", umod_l(a_l, 60000));
  printf("\n");

  printf("\n\n");
  printf("Solutions conformes a celles de Mathematica 4.1 (Voir MathematicaEssaiArithElem.nb)\n");
  
  printf("\n");
}
