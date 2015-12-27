#include <stdio.h>

int main(void)
{
  unsigned long a = 4294940870UL;
  unsigned long b = 3921392554UL;
  unsigned long c;

  printf("a + b = %lu\n", a + b); /* Attention réduction mod 2^32 si m32 et non si m64 */

  return 0;
}

