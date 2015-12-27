#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libmparithm.h"

#define MAXTESTLEN BIGINTMAXBIT
#define RND (ulrand64_l() % (MAXTESTLEN + 1))
#define RNDDIGIT (ulrand64_l() % (BIGINTMAXDIGIT + 1))

int main(void)
{
  BIGINT a_l, m_l, e_l, p_l;
  int i, j;
  time_t start, finish;
  double duration;

  ulseed64_l(time(0));

  rand_l(a_l, 2048);
  rand_l(m_l, 2048);

  /*start = time(0);*/
  /*time(&now);*/
  duration = 0;
  for (i = 0; i < 5; i++)
    {
      rand_l(e_l, 8192);
      start = time(0);
      /*printf("start = %ld\n", start);*/
      for (j = 0; j < 10; j++)
	expmod7_l(a_l, e_l, m_l, p_l);
      finish = time(0);
      /*printf("finish = %ld\n", start);*/
      duration = duration + difftime(finish, start);
    }
  /*finish = time(0);*/
  /*duration = difftime(finish, start);*/

  printf("Durée: %f\n", duration);
  /*printf("Durée: %ld\n", clock());*/
  
  return 0;
}
