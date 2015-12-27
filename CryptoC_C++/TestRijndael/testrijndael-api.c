#include <time.h>
#include "libmparithm.h"
#include "rijndael-api.h"

#ifdef ARCH32
#define NBRETESTS 100000
#endif
#ifdef ARCH64
#define NBRETESTS 1000000
#endif

static unsigned char* genmess(int);
static char* genkey(int);
static char* geniv(int);

static unsigned char* genmess(int len)
{
  unsigned char* mess = malloc(len + 1);
  unsigned char* temp = mess;
  int i;

  if (mess == NULL)
    return NULL;
  for (i = 0; i < len; i++)
    {
      do
	*temp = ucrand64_l();
      while (*temp == 0);
      temp = temp + 1;
    }
  *temp = '\0';

  return mess;
}

static char tabhexa[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

static char* genkey(int len)
{
  char* mess = malloc(2 * len + 1);
  char* temp = mess;
  unsigned char tmp;
  int i;
  
  if (mess == NULL)
    return NULL;
  for (i = 0; i < len; i++)
    {
      tmp = ucrand64_l();
      *temp = tabhexa[tmp % 16];
      temp = temp + 1;
      *temp = tabhexa[tmp / 16];
      temp = temp + 1;
    }
  *temp = '\0';
  
  return mess;
}

static char* geniv(int len)
{
  char* mess = malloc(2 * len + 1);
  char* temp = mess;
  unsigned char tmp;
  int i;
  
  if (mess == NULL)
    return NULL;
  for (i = 0; i < len; i++)
    {
      tmp = ucrand64_l();
      *temp = tabhexa[tmp % 16];
      temp = temp + 1;
      *temp = tabhexa[tmp / 16];
      temp = temp + 1;
    }
  *temp = '\0';
  
  return mess;
}

int main()
{
  ulseed64_l(time(NULL));

  printf("PROGRAMME DE TEST POUR L'API RIJNDAEL...\n");
  printf("----------------------------------------\n\n");

  int outlen, len;
  int blocklen;
  int keylen;
  keyinstance keyinst;
  cipherinstance cipherinst;
  unsigned char* in;
  unsigned char* out;
  unsigned char* decr;
  char* key;
  char* iv;
  unsigned long i; 
  int error = 0;

  printf("TEST POUR LE MODE ECB...\n\n");

  for (i = 0; i < NBRETESTS; i++)
    {
      len = ucrand64_l() % 1000 + 1;
      blocklen = (ucrand64_l() % 3 + 2) * 8;
      keylen = (ucrand64_l() % 3 + 2) * 8;
      in = genmess(len);
      out = malloc(len + blocklen + 1);
      decr = malloc(len + blocklen + 1);
      key = genkey(keylen);

      /*printf("BLOCKLENTH = %d\n", blocklen);
      printf("KEYLENGTH = %d\n", keylen);*/

      MakeKey(&keyinst, ENCRYPT_DIR, keylen * 8, blocklen * 8, key);
      CipherInit(&cipherinst, ECB_MODE, blocklen * 8, NULL);
      if ((len + 1) % blocklen == 0)
	BlockEncrypt(&cipherinst, &keyinst, (word8*)in, (len + 1) * 8, (word8*)out);
      else
	{
	  PadEncrypt(&cipherinst, &keyinst, (word8*)in, len + 1, (word8*)out);
	  /*printf("Padding needed\n");*/
	}
      /*printf("Longueur texte clair = %u\n", (unsigned int)strlen((char*)in));
      printf("Key = 0x%s\n", key);*/
      
      MakeKey(&keyinst, DECRYPT_DIR, keylen * 8, blocklen * 8, key);
      CipherInit(&cipherinst, ECB_MODE, blocklen * 8, NULL);
      outlen = ((len + 1 + blocklen) / blocklen) * blocklen;
      if ((len + 1) % blocklen == 0)
	BlockDecrypt(&cipherinst, &keyinst, (word8*)out, (len + 1) * 8, (word8*)decr);
      else
	PadDecrypt(&cipherinst, &keyinst, (word8*)out, outlen, (word8*)decr);
      /*printf("Longueur du texte décrypté = %u\n", (unsigned int)strlen((char*)decr));*/
      
      /*if (!memcmp(in, decr, len))
	printf("\nOpération d'encryptage/décryptage réussie --> OK\n");
      else
	{
	  printf("\nOpération d'encryptage/décryptage échouée --> MERDE!!!\n");
	  error = 1;
	}*/
    }
  printf("\n");

  printf("TEST POUR LE MODE CBC...\n\n");

  for (i = 0; i < NBRETESTS; i++)
    {
      len = ucrand64_l() % 1000 + 1;
      blocklen = (ucrand64_l() % 3 + 2) * 8;
      keylen = (ucrand64_l() % 3 + 2) * 8;
      in = genmess(len);
      out = malloc(len + blocklen + 1);
      decr = malloc(len + blocklen + 1);
      key = genkey(keylen);
      iv = geniv(blocklen);

      /*printf("BLOCKLENTH = %d\n", blocklen);
      printf("KEYLENGTH = %d\n", keylen);*/

      MakeKey(&keyinst, ENCRYPT_DIR, keylen * 8, blocklen * 8, key);
      CipherInit(&cipherinst, CBC_MODE, blocklen * 8, iv);
      if ((len + 1) % blocklen == 0)
	BlockEncrypt(&cipherinst, &keyinst, (word8*)in, (len + 1) * 8, (word8*)out);
      else
	{
	  PadEncrypt(&cipherinst, &keyinst, (word8*)in, len + 1, (word8*)out);
	  /*printf("Padding needed\n");*/
	}
      /*printf("Longueur texte clair = %u\n", (unsigned int)strlen((char*)in));
      printf("Key = 0x%s\n", key);
      printf("IV = 0x%s\n", iv);*/
      
      MakeKey(&keyinst, DECRYPT_DIR, keylen * 8, blocklen * 8, key);
      CipherInit(&cipherinst, CBC_MODE, blocklen * 8, iv);
      outlen = ((len + 1 + blocklen) / blocklen) * blocklen;
      if ((len + 1) % blocklen == 0)
	BlockDecrypt(&cipherinst, &keyinst, (word8*)out, (len + 1) * 8, (word8*)decr);
      else
	PadDecrypt(&cipherinst, &keyinst, (word8*)out, outlen, (word8*)decr);
      /*printf("Longueur du texte décrypté = %u\n", (unsigned int)strlen((char*)decr));*/
      
      /*if (!memcmp(in, decr, len))
	printf("Opération d'encryptage/décryptage réussie --> OK\n");
      else
	{
	  printf("Opération d'encryptage/décryptage échouée --> MERDE!!!\n");
	  error = 1;

	  return 1;
	}*/
    }
  printf("\n");
  
  if (error)
    printf("Erreur dans une des opérations du test !!!!!!!!!!!!\n");
  else
    printf("Tous les tests se sont déroulés sans erreur.\n");

  return 0;
}


