#include "ripemd.h"


int main()
{
  UCHAR* mess = (UCHAR*)"abcdefghijklmnopqrstuvwxyz";
  UCHAR* hash;
  BIGINT H;
  
  hash = ripemd160((UCHAR*)mess, (ULONG)strlen(mess));

  printf("Hash Value = %s\n", hash);

  byte2bigint_l(H, hash, strlen(hash));

  printf("Hash value = %s\n", bigint2str_l(H, 16, 0));

  return 0;
}

