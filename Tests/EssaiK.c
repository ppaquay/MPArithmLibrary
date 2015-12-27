#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arithm.h"
#define MAXTESTLEN BIGINTMAXBIT
#define RND (ulrand64_l() % (MAXTESTLEN + 1))

BIGINT a_l, e_l, m_l, s1_l, s2_l;
USHORT a, e;
int i;

int main()
{
  ulseed64_l(time(0));

  for (i = 1; i <= 1000; i++)
    {
      do
        rand_l(m_l, RND);
      while (iseven_l(m_l));
      e = usrand64_l();
      a = usrand64_l();
      rand_l(a_l, RND);
      rand_l(e_l, RND);
      expmodmonk_l(a_l, e_l, m_l, s1_l);
      expmodk_l(a_l, e_l, m_l, s2_l);
      if (!equ_l(s1_l, s2_l))
        {
          printf("ERREUR !\n");
          printf("s1 = %s\n", bigint2str_l(s1_l, 10, 0));
          printf("s2 = %s\n", bigint2str_l(s2_l, 10, 0));
          exit (-1);
        }
      uexpmodmon_l(a_l, e, m_l, s1_l);
      uexpmod_l(a_l, e, m_l, s2_l);
      if (!equ_l(s1_l, s2_l))
        {
          printf("ERREUR !\n");
          printf("s1 = %s\n", bigint2str_l(s1_l, 10, 0));
          printf("s2 = %s\n", bigint2str_l(s2_l, 10, 0));
          exit (-1);
        }
      wexpmodmon_l(a, e_l, m_l, s1_l);
      wexpmod_l(a, e_l, m_l, s2_l);
      if (!equ_l(s1_l, s2_l))
        {
          printf("ERREUR !\n");
          printf("s1 = %s\n", bigint2str_l(s1_l, 10, 0));
          printf("s2 = %s\n", bigint2str_l(s2_l, 10, 0));
          exit (-1);
        }
    }

  return 0;
}
