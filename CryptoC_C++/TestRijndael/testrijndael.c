#include <time.h>
#include "libmparithm.h"
#include "rijndael.h"

#define NBRETESTS 300000

/*static void writekey(word8 [][], int);
static void writeplaintext(word8 [][], int);
static void writeciphertext(word8 [][], int);

static void writekey(word8 key[4][MAXNK], int Nk)
{
  int i, j;

  printf("Key = 0x");
  for (j = 0; j < Nk; j++)
    for (i = 0; i < 4; i++)
      {
	if (key[i][j] < 16)
	  printf("0");
	printf("%x", key[i][j]);
      }
  printf("\n");
}

static void writeplaintext(word8 state[4][MAXNB], int Nb)
{
  int i, j;

  printf("Plaintext = 0x");
  for (j = 0; j < Nb; j++)
    for (i = 0; i < 4; i++)
      {
	if (state[i][j] < 16)
	  printf("0");
	printf("%x", state[i][j]);
      }
  printf("\n");
}

static void writeciphertext(word8 state[4][MAXNB], int Nb)
{
  int i, j;

  printf("Ciphertext = 0x");
  for (j = 0; j < Nb; j++)
    for (i = 0; i < 4; i++)
      {
	if (state[i][j] < 16)
	  printf("0");
	printf("%x", state[i][j]);
      }
  printf("\n");
}*/

int main()
{
  word32 roundkey[(MAXNR + 1) * MAXNB];
  word8 key[4][MAXNK];
  word8 in[4][MAXNB];
  int Nb, Nk, i, j, false = 0;
  unsigned int nbre;
  word8 state[4][MAXNB];
 
  ulseed64_l(time(0));
  printf("\nPROGRAMME DE TEST DE L'ALGORITHME RIJNDAEL...\n\n");

  for (nbre = 0; nbre < NBRETESTS; nbre++)
    {
      Nk = ((usrand64_l() % 3) + 2) << 1;
      Nb = ((usrand64_l() % 3) + 2) << 1;
      
      for (j = 0; j < Nb; j++)
	for (i = 0; i < 4; i++)
	  in[i][j] = (word8)(usrand64_l() & 0xff);
      
      for (j = 0; j < Nb; j++)
	for (i = 0; i < 4; i++)
	  state[i][j] = in[i][j];
      
      /*writeplaintext(state, Nb);*/
      
      for (j = 0; j < Nk; j++)
	for (i = 0; i < 4; i++)
	  key[i][j] = (word8)(usrand64_l() & 0xff);
      
      /*writekey(key, Nk);*/
      
      RijndaelKeyExpansion(key, 32 * Nk, 32 * Nb, roundkey);
      RijndaelEncrypt(state, 32 * Nk, 32 * Nb, roundkey);
      
      /*writeciphertext(state, Nb);*/
      
      RijndaelDecrypt(state, 32 * Nk, 32 * Nb, roundkey);
      
      /*writeplaintext(state, Nb);*/
      
      for (j = 0; j < Nb; j++)
	for (i = 0; i < 4; i++)
	  if (state[i][j] != in[i][j])
	    false = false + 1;
      /*if (false)
	printf("Erreur dans le cryptage/décryptage !!!\n");
      else
	printf("Texte décrypté = texte clair ---> OK\n");
      printf("\n");*/

      if (nbre % (NBRETESTS / 10) == 0)
	printf("%d%% des tests effectués...\n", (nbre / (NBRETESTS / 10)) * 10);
    }

  if (!false)
    printf("Tous les tests ont été réalisés sans erreur.\n");
  else
    printf("Il y a eu %d erreur(s) !!!!\n", false);

  return 0;
}
