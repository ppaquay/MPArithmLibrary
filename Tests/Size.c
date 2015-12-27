/* Test de la taille des mots sur un processeur 32bits */

#include <stdio.h>
#include "libarithm-0.9.3.h"

int main()
{
  printf("Taille de int: %d octets, donc %d bits\n", (int)sizeof(int), (int)sizeof(int) * 8);
  printf("Taille de long: %d octets, donc %d bits\n", (int)sizeof(long), (int)sizeof(long) * 8);
  printf("Taille de unsigned short (bigint): %d octets, donc %d bits\n", (int)sizeof(unsigned short), (int)sizeof(unsigned short) * 8);
  printf("Taille de unsigned long (bigintd): %d octets, donc %d bits\n", (int)sizeof(unsigned long), (int)sizeof(unsigned long) * 8);
 printf("Taille de unsigned long long: %d octets, donc %d bits\n", (int)sizeof(unsigned long long), (int)sizeof(unsigned long long) * 8);
 printf("Taille de BIGINT: %d octets, donc %d bits\n", (int)sizeof(BIGINT), (int)sizeof(BIGINT) * 8);

 return 0;

}
