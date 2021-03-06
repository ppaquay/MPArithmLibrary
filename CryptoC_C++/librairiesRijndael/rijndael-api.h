/***********************************************************************
 *                                                                     *
 *       Header pour la librairie des fonctions de l'API Rijndael      *
 *       (largement inspir� de l'impl�mentation de Rijmen-Barreto)     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#ifndef libRijndaelAPI_H
#define libRijndaelAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rijndael.h"

/* D�finition des constantes utiles */

#define ENCRYPT_DIR 0
#define DECRYPT_DIR 1

#define ECB_MODE 2
#define CBC_MODE 3
#define BITSPERBLOCK 256

#define BAD_KEY_DIR -1
#define BAD_KEY_MAT -2
#define BAD_KEY_INST -3
#define BAD_CIPHER_MODE -4
#define BAD_CIPHER_STATE -5
#define BAD_CIPHER_INST -6
#define BAD_DATA -7

#define MAX_KEY_SIZE 64 /* Nbre de caract�res ASCII pour repr�senter la cl� */
#define MAX_IV_SIZE 32 /* Nbre de bytes pour repr�senter un IV */

/* D�finition des diff�rents types utiles */

typedef struct {
  word8 direction;
  int keylen;
  char keymaterial[MAX_KEY_SIZE + 1];
  int blocklen;
  word32 keysched[(MAXNR + 1) * MAXNB];
} keyinstance;

typedef struct {
  word8 mode;
  word8 IV[MAX_IV_SIZE];
  int blocklen;
} cipherinstance;

/* Prototypes des fonctions utiles */

extern int MakeKey(keyinstance*, word8, int, int, char*);
/* Cr�e la struct keyinstance � partir des donn�es fournies en argument, arg2 est la direction (crypte ou d�crypte), arg3 la longueur de la cl� en bits, arg4 est la longueur de bloc en bits, arg5 est la cl� donn�e en ASCII interpr�t�e en hexad�cimal, renvoie 1 si tout s'est bien pass� et un code d'erreur sinon */

extern int CipherInit(cipherinstance*, word8, int, char*);
/* Cr�e la struct cipherinstance � partir des donn�es fournies en argument, arg2 est le mode de blocs, arg3 est la longueur de bloc en bits, arg4 est le vecteur d'initialisation en ASCII interpr�t� en hexad�cimal renvoie 1 si tout s'est bien pass� et un code d'erreur sinon */

extern int BlockEncrypt(cipherinstance*, keyinstance*, word8*, int, word8*);
/* Fonction d'encryption par blocs en mode ECB ou CBC, arg1 est la struct cipherinstance, arg2 est la struct keyinstance, arg3 est l'input en bytes, arg4 est la longueur de l'input en bits, arg5 est rempli avec l'output en bytes, renvoie le nombre de bits crypt�s et un code d'erreur sinon */

extern int BlockDecrypt(cipherinstance*, keyinstance*, word8*, int, word8*);
/* Fonction de d�cryption par blocs en mode ECB ou CBC, arg1 est la struct cipherinstance, arg2 est la struct keyinstance, arg3 est l'input en bytes, arg4 est la longueur de l'input en bits, arg5 est rempli avec l'output en bytes, renvoie le nombre de bits d�crypt�s et un code d'erreur sinon */

extern int PadEncrypt(cipherinstance*, keyinstance*, word8*, int, word8*);
/* Fonction d'encryption par blocs en mode ECB ou CBC avec padding RFC 2040, arg1 est la struct cipherinstance, arg2 est la struct keyinstance, arg3 est l'input en bytes, arg4 est la longueur de l'input en bytes, arg5 est rempli avec l'output en bytes, renvoie le nombre de bytes crypt�s et un code d'erreur sinon */

extern int PadDecrypt(cipherinstance*, keyinstance*, word8*, int, word8*);
/* Fonction de d�cryption par blocs en mode ECB ou CBC avec padding RFC 2040, arg1 est la struct cipherinstance, arg2 est la struct keyinstance, arg3 est l'input en bytes, arg4 est la longueur de l'input en bytes, arg5 est rempli avec l'output en bytes, renvoie le nombre de bytes d�crypt�s et un code d'erreur sinon */




















#endif

/* Fin de rijndael-api.h */
