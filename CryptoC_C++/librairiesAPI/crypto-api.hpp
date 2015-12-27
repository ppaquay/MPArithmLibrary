/***********************************************************************
 *                                                                     *
 *                Header pour les fonctions de l'API en C              *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#ifndef CRYPTO_API_HPP
#define CRYPTO_API_HPP

#include "librsa++.hpp"

#define DOUBTFUL 1
#define SEMITRUSTED 2
#define TRUSTED 3

#define HASHCOUNT 10000

#define AES128 1
#define AES192 2
#define AES256 3

#define RIPEMD160 1
#define HASH1 2

#define PK_ENCRYPTED_SK 1
#define SIGNATURE 2
#define PUBLIC_KEY 3
#define SECRET_KEY 4
#define S_ENCRYPTED_DATA 5
#define USER_ID 6

class KeyStruct
{
public:
  string UserName;
  string UserEMail;
  USHORT Len;
};

class KeyPriv
{
public:
  inline KeyPriv(void) {};
  KeyPriv(const string, const string, const int);
  KeyPriv(const string, const string, const int, const largeint&);

  ~KeyPriv();

  KeyStruct Export_KeyStruct(void);
  RSAPub Export_RSAPub(void);

  friend int operator==(const KeyPriv&, const KeyPriv&);

  friend void writeprivkey(const char*, const KeyPriv&);
  friend void readprivkey(const char*, KeyPriv&);

private:
  KeyStruct PrivKeyStruct;
  RSAPriv KeyPrivRSA;

  void makekeystruct(const string, const string, const int);
  friend ofstream& operator<<(ofstream&, const KeyPriv&);
  friend ifstream& operator>>(ifstream&, KeyPriv&);
};

class KeyPub
{
public:
  inline KeyPub(void) {};
  KeyPub(const KeyPriv&);

  ~KeyPub();

  friend int operator==(const KeyPub&, const KeyPub&);

  friend void writepubkey(const char*, const KeyPub&);
  friend void readpubkey(const char*, KeyPub&);

private:
  KeyStruct PubKeyStruct;
  RSAPub KeyPubRSA;

  friend ofstream& operator<<(ofstream&, const KeyPub&);
  friend ifstream& operator>>(ifstream&, KeyPub&);
};

// Fonction pour la génération de la clé à partir d'un mot de passe

UCHAR* string2key(const UCHAR*, const ULONG, const int, const UCHAR*);

// Fonction pour la génération d'une clé de session

UCHAR* sessionkey(const int);
/* Génère une clé de session de la taille correspondante à l'algo donné en arg1, renvoie les bytes de cette clé */

// Fonctions pour l'utilisation de l'ASCII Armor

UCHAR* base64_encode(UCHAR*, int, int);
/* Formate un bloc de texte en encodage base64, arg1 est le texte à formater, arg2 est la longueur du texte en bytes et arg3 est le wrap qui formate le texte sur 80 colonnes, revoie le texte formaté et 0 si un problème est survenu */
UCHAR* base64_decode(UCHAR*, int*, int, int*);
/* Elimine le formatage base64, arg1 est le texte formaté, arg2 est rempli avec la longueur du texte non-formaté, arg3 est 1 ou 0 selon que l'on veut que la vérification de la syntaxe base64 soit stricte ou non, arg4 est rempli avec l'erreur détectée, renvoie le texte non-formaté */







#endif

/* Fin de crypto-api.hpp */
