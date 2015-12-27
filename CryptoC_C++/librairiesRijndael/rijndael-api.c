/***********************************************************************
 *                                                                     *
 *    Code source pour la librairie des fonctions de l'API Rijndael    *
 *      (largement inspiré de l'implémentation de Rijmen-Barreto)      *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#include "rijndael-api.h"

/* Fonctions de nettoyage des variables */
/****************************************/

#ifdef SECURE
#define SECMEMSET(X, Y, Z) secmemset((X), (Y), (Z))
static void *secmemset(void*, int, size_t);
#else
#define SECMEMSET(X, Y, Z) (void)0
#endif

/* Code source des fonctions de nettoyage des variables */
/********************************************************/

#ifdef SECURE
static void *secmemset(void *v, int c, size_t n)
{
  volatile char *p = (volatile char *)v;
  
  while (n--)
    *p++ = c;

  return v;
}
#endif

/* Code source des différentes fonctions externes */
/**************************************************/

int MakeKey(keyinstance *key, word8 direction, int keylen, int blocklen, char *keymaterial)
{
  word8 k[4][MAXNK];
  int i, j, t;

  if (key == NULL)
    return BAD_KEY_INST;
  if ((direction == ENCRYPT_DIR) || (direction == DECRYPT_DIR))
    key->direction = direction;
  else
    return BAD_KEY_DIR;
  if ((keylen == 128) || (keylen == 192) || (keylen == 256))
    key->keylen = keylen;
  else
    return BAD_KEY_MAT;
  if ((blocklen == 128) || (blocklen == 192) || (blocklen == 256))
    key->blocklen = blocklen;
  else
    return BAD_KEY_MAT;
  if (keymaterial)
    strncpy(key->keymaterial, keymaterial, keylen / 4);
  for (i = 0; i < (key->keylen >> 3); i++)
    {
      t = key->keymaterial[i << 1];
      if ((t >= '0') && (t <= '9'))
	j = (t - '0') << 4;
      else if ((t >= 'a') && (t <= 'f'))
	j = (t - 'a' + 10) << 4;
      else if ((t >= 'A') && (t <= 'F'))
	j = (t - 'A' + 10) << 4;
      else
	return BAD_KEY_MAT;
      t = key->keymaterial[(i << 1) + 1];
      if ((t >= '0') && (t <= '9'))
	j = j ^ (t - '0');
      else if ((t >= 'a') && (t <= 'f'))
	j = j ^ (t - 'a' + 10);
      else if ((t >= 'A') && (t <= 'F'))
	j = j ^ (t - 'A' + 10);
      else
	return BAD_KEY_MAT;      
      k[i % 4][i / 4] = (word8)j;
      RijndaelKeyExpansion(k, key->keylen, key->blocklen, key->keysched);
    }

  SECMEMSET(k, 0, 4 * MAXNK);
  
  return 1;
}

int CipherInit(cipherinstance *cipher, word8 mode, int blocklen, char *IV)
{
  int i, j, t;

  if (cipher == NULL)
    return BAD_CIPHER_INST;
  if ((mode == ECB_MODE) || (mode == CBC_MODE))
    cipher->mode = mode;
  else
    return BAD_CIPHER_MODE;
  if ((blocklen == 128) || (blocklen == 192) || (blocklen == 256))
    cipher->blocklen = blocklen;
  else
    return BAD_CIPHER_STATE;
  if (IV != NULL)
    {
      for (i = 0; i < (cipher->blocklen >> 3); i++)
	{
	  t = IV[i << 1];
	  if ((t >= '0') && (t <= '9'))
	  j = (t - '0') << 4;
	  else if ((t >= 'a') && (t <= 'f'))
	    j = (t - 'a' + 10) << 4;
	  else if ((t >= 'A') && (t <= 'F'))
	    j = (t - 'A' + 10) << 4;
	  else
	    return BAD_CIPHER_INST;
	  t = IV[(i << 1) + 1];
	  if ((t >= '0') && (t <= '9'))
	    j = j ^ (t - '0');
	  else if ((t >= 'a') && (t <= 'f'))
	    j = j ^ (t - 'a' + 10);
	  else if ((t >= 'A') && (t <= 'F'))
	    j = j ^ (t - 'A' + 10);
	  else
	    return BAD_CIPHER_INST;
	  cipher->IV[i] = (word8)j;
	}
    }
  
  return 1;
}

int BlockEncrypt(cipherinstance *cipher, keyinstance *key, word8 *input, int inputlen, word8 *outbuffer)
{
  int i, j, t, numblocks;
  word8 block[4][MAXNB];

  if ((key == NULL) || (key->direction != ENCRYPT_DIR) || ((key->keylen != 128) && (key->keylen != 192) && (key->keylen != 256)))
    return BAD_KEY_MAT;
  if ((cipher == NULL) || ((cipher->mode != ECB_MODE) && (cipher->mode != CBC_MODE)) || ((cipher->blocklen != 128) && (cipher->blocklen != 192) && (cipher->blocklen != 256)))
    return BAD_CIPHER_STATE;
  if ((input == NULL) || (inputlen <= 0))
    return 0;
  numblocks = inputlen / cipher->blocklen;
  switch (cipher->mode)
    {
    case ECB_MODE:
      for (i = 0; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      break;
    case CBC_MODE:
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  block[t][j] = cipher->IV[4 * j + t] & 0xff;
      for (i = 0; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = block[t][j] ^ (input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff);
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      break;
    default:
      return BAD_CIPHER_STATE;
    }

  SECMEMSET(block, 0, 4 * MAXNB);

  return numblocks * (cipher->blocklen);
}

int BlockDecrypt(cipherinstance *cipher, keyinstance *key, word8 *input, int inputlen, word8 *outbuffer)
{
  int i, j, t, numblocks;
  word8 block[4][MAXNB];

  if ((cipher == NULL) || (cipher->blocklen != key->blocklen) || ((cipher->mode != ECB_MODE) && (cipher->mode != CBC_MODE)) || ((cipher->blocklen != 128) && (cipher->blocklen != 192) && (cipher->blocklen != 256)))
    return BAD_CIPHER_STATE;
  if ((key == NULL) || (key->direction != DECRYPT_DIR) || ((key->keylen != 128) && (key->keylen !=192) && (key->keylen != 256)))
    return BAD_KEY_MAT;
  if ((input == NULL) || (inputlen <= 0))
    return 0;
  numblocks = inputlen / cipher->blocklen;
  switch (cipher->mode)
    {
    case ECB_MODE:
      for (i = 0; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      break;
    case CBC_MODE:
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  block[t][j] = input[(j << 2) + t] & 0xff;
      RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  outbuffer[4 * j + t] = block[t][j] ^ cipher->IV[(j << 2) + t];
      for (i = 1; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j] ^ input[(cipher->blocklen >> 3) * (i - 1) + (j << 2) + t];
	}
      break;
    default:
      return BAD_CIPHER_STATE;
    }

  SECMEMSET(block, 0, 4 * MAXNB);

  return numblocks * (cipher->blocklen);
}

int PadEncrypt(cipherinstance *cipher, keyinstance *key, word8 *input, int inputoctets, word8* outbuffer)
{
  int i, j, t, numblocks, blockoctets, padlen;
  word8 block[4][MAXNB];

  if ((key == NULL) || (key->direction != ENCRYPT_DIR) || ((key->keylen != 128) && (key->keylen != 192) && (key->keylen != 256)))
    return BAD_KEY_MAT;
  if ((cipher == NULL) || ((cipher->mode != ECB_MODE) && (cipher->mode != CBC_MODE)) || ((cipher->blocklen != 128) && (cipher->blocklen != 192) && (cipher->blocklen != 256)))
    return BAD_CIPHER_STATE;
  if ((input == NULL) || (inputoctets <= 0))
    return 0;
  blockoctets = cipher->blocklen >> 3;
  numblocks = inputoctets / blockoctets;
  switch (cipher->mode)
    {
    case ECB_MODE:
      for (i = 0; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      padlen = blockoctets - (inputoctets - blockoctets * numblocks);
      if ((padlen > 0) && (padlen <= blockoctets))
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      if (4 * j + t < blockoctets - padlen)
		block[t][j] = input[(cipher->blocklen >> 3) * numblocks + (j << 2) + t] & 0xff;
	      else
		block[t][j] = padlen;
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * numblocks + (j << 2) + t] = block[t][j];
	}
      else
	return BAD_DATA;
      break;
    case CBC_MODE:
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  block[t][j] = cipher->IV[(j << 2) + t] & 0xff;
      for (i = 0; i < numblocks; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = block[t][j] ^ (input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff);
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      padlen = blockoctets - (inputoctets - blockoctets * numblocks);
      if ((padlen > 0) && (padlen <= blockoctets))
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      if (4 * j + t < blockoctets - padlen) 
		block[t][j] = block[t][j] ^ (input[(cipher->blocklen >> 3) * numblocks + (j << 2) + t] & 0xff);
	      else
		block[t][j] = block[t][j] ^ (word8)padlen;
	  RijndaelEncrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * numblocks + (j << 2) + t] = block[t][j];
	}
      else
	return BAD_DATA;
      break;
    default:
      return BAD_CIPHER_STATE;
    }

  SECMEMSET(block, 0, 4 * MAXNB);

  return (numblocks + 1) * (cipher->blocklen >> 3);
}

int PadDecrypt(cipherinstance *cipher, keyinstance *key, word8 *input, int inputoctets, word8 *outbuffer)
{
  int i, j, t = 0, numblocks, blockoctets, padlen;
  word8 block[4][MAXNB];

  if ((cipher == NULL) || (cipher->blocklen != key->blocklen) || ((cipher->mode != ECB_MODE) && (cipher->mode != CBC_MODE)) || ((cipher->blocklen != 128) && (cipher->blocklen != 192) && (cipher->blocklen != 256)))
    return BAD_CIPHER_STATE;
  if ((key == NULL) || (key->direction != DECRYPT_DIR) || ((key->keylen != 128) && (key->keylen !=192) && (key->keylen != 256)))
    return BAD_KEY_MAT;
  if ((input == NULL) || (inputoctets <= 0))
    return 0;
  blockoctets = cipher->blocklen >> 3;
  numblocks = inputoctets / blockoctets;
  switch (cipher->mode)
    {
    case ECB_MODE:
      for (i = 0; i < numblocks - 1; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j];
	}
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  block[t][j] = input[(cipher->blocklen >> 3) * (numblocks - 1) + (j << 2) + t] & 0xff;
      RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
      padlen = block[3][(cipher->blocklen >> 5) - 1];
      if ((padlen <= 0) || (padlen >= blockoctets))
	return BAD_DATA;
      do
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * (numblocks - 1) + (j << 2) + t] = block[t][j];
	}
      while ((j << 2) + t < blockoctets - padlen);
      break;
    case CBC_MODE:
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  block[t][j] = input[(j << 2) + t] & 0xff;
      RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
      for (j = 0; j < (cipher->blocklen >> 5); j++)
	for (t = 0; t < 4; t++)
	  outbuffer[(j << 2) + t] = block[t][j] ^ cipher->IV[(j << 2) + t];
      for (i = 1; i < numblocks - 1; i++)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * i + (j << 2) + t] & 0xff;
	  RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      outbuffer[(cipher->blocklen >> 3) * i + (j << 2) + t] = block[t][j] ^ input[(cipher->blocklen >> 3) * (i - 1) + (j << 2) + t];
	}
      if (numblocks > 1)
	{
	  for (j = 0; j < (cipher->blocklen >> 5); j++)
	    for (t = 0; t < 4; t++)
	      block[t][j] = input[(cipher->blocklen >> 3) * (numblocks - 1) + (j << 2) + t] & 0xff;
	  RijndaelDecrypt(block, key->keylen, cipher->blocklen, key->keysched);
	  padlen = block[3][(cipher->blocklen >> 5) - 1] ^ input[(cipher->blocklen >> 3) * (numblocks - 1) - 1];
	  if ((padlen <= 0) || (padlen >= blockoctets))
	    return BAD_DATA;
	  do
	    {
	      for (j = 0; j < (cipher->blocklen >> 5); j++)
		for (t = 0; t < 4; t++)
		  outbuffer[(cipher->blocklen >> 3) * (numblocks - 1) + (j << 2) + t] = block[t][j] ^ input[(cipher->blocklen >> 3) * (numblocks - 2) + (j << 2) + t];
	    }
	  while ((j << 2) + t < blockoctets - padlen);
	}
      else
	{
	  padlen = block[3][(cipher->blocklen >> 5) - 1] ^ cipher->IV[(cipher->blocklen >> 3) - 1];
	  if ((padlen <= 0) || (padlen >= blockoctets))
	    return BAD_DATA;
	  do
	    {
	      for (j = 0; j < (cipher->blocklen >> 5); j++)
		for (t = 0; t < 4; t++)
		  outbuffer[4 * j + t] = block[t][j] ^ cipher->IV[(j << 2) + t];
	    }
	  while ((j << 2) + t < blockoctets - padlen);
	}
      break;
    default:
      return BAD_CIPHER_STATE;
    }

  SECMEMSET(block, 0, 4 * MAXNB);

  return numblocks * (cipher->blocklen >> 3) - padlen;
}


/* Fin de rinjdael-api.c */
