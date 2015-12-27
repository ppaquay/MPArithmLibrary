/***********************************************************************
 *                                                                     *
 *                Header pour les fonctions du RSA en C++              *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#ifndef RSA_HPP
#define RSA_HPP

#include "libmparithm++.hpp"
#include "ripemd.h"

/***************************************/
/* D�claration des classes auxiliaires */
/***************************************/

// Classe RSAKey

class RSAKey
{
public:
  largeint p, q, n, e, d;
  USHORT bitlen;
};

// Classe RSAKeyPub

class RSAKeyPub
{
public:
  largeint n, e;
  USHORT bitlen;
};

/*********************************************/
/* D�claration des classes RSAPriv et RSAPub */
/*********************************************/

// Classe RSAPriv

class RSAPriv
{
public:

  /* Constructeurs et destructeur */
  /********************************/

  // Constructeurs

  inline RSAPriv(void) {};
  RSAPriv(const int);
  RSAPriv(const int, const largeint&);
  /* Constructeurs prenant en argument le nombre de bit du modulo n et pour le second un seed largeint */
  
  // Destructeur

  ~RSAPriv();
  /* Destructeur s�r */

  /* Fonctions membres */
  /*********************/

  // Fonction de d�cryptage

  UCHAR* decrypt(const largeint&, int*);
  /* Fonction de d�cryptage du RSA, arg1 est le texte crypt� (largeint) et arg2 est rempli par la taille en byte du texte d�crypt�, renvoie le texte clair (chaine de bytes) */

  // Fonction de signature

  largeint sign(const UCHAR* const, const int);
  /* Fonction de signature du RSA, utilise RIPEMD160, arg1 est le message � signer, arg2 la taille en byte du message, renvoie la signature (largeint) */

  // Fonction d'export de la cl� publique

  RSAKeyPub exportkeypub(void);
  /* Fonction qui g�n�re la cl� publique (e,n) � partir de la cl� priv�e (e, d, p, q, n) */

  /* Fonctions amies de la classe RSAPriv */
  /******************************************/

  // Op�rateurs de comparaison

  friend int operator==(const RSAPriv&, const RSAPriv&);
  /* Compare deux cl�s priv�es, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const RSAPriv&, const RSAPriv&);
  /* Compare deux cl�s priv�es, renvoie 1 si elles sont diff�rentes et 0 sinon */

  // Op�rateurs de flux

  friend ofstream& operator<<(ofstream&, const RSAPriv&);
  friend ifstream& operator>>(ifstream&, RSAPriv&);
  /* Op�rateurs permettant de lire et d'�crire des cl�s priv�es dans un fichier */

  /* Op�rateur d'affectation */
  /***************************/

  RSAPriv& operator=(const RSAPriv&);
  /* Op�rateur permettant d'affecter une cl� priv�e � une cl� priv�e g�n�rique */

private:
  RSAKey keypriv;

  /* Fonctions auxiliaires */
  /*************************/

  // Fonction de d�cryptage rapide

  largeint fdecrypt(const largeint&);
  /* Fonction permettant de d�crypter rapidement � l'aide des restes chinois, arg1 est le texte crypt� (largeint), renvoie le texte clair (largeint) */

  // Gestion des cl�s

  int testkey(void);
  /* Fonction permettant de v�rifier si la cl� est valide en g�n�rant un message al�atoire et en le cryptant et d�cryptant avec fdecrypt */
  int makekey(const int);
  /* Fonction permettant de g�n�rer une cl� priv�e (e,d,p,q,n) utilis�e dans les constructeurs */
};

// Classe RSAPub

class RSAPub
{
public:
  /* Constructeurs et destructeur */
  /********************************/

  // Constructeurs

  inline RSAPub(void) {};
  RSAPub(const RSAPriv&);
  /* Constructeur permettant de g�n�rer une cl� publique � partir d'une cl� priv�e */
  
  // Destructeur

  ~RSAPub();
  /* Destructeur s�r */

  /* Fonctions membres */
  /*********************/
  
  // Fonction de cryptage

  largeint crypt(const UCHAR* const, const int);
  /* Fonction de cryptage du RSA, arg1 est le texte clair et arg2 est la taille en bytes du texte clair (Taille texte clair < Taille du modulo n), renvoie le texte crypt� (largeint) */

  // Fonction d'authentification de la signature

  int authen(const UCHAR* const, const largeint&, const int);
  /* Fonction d'authentification de la signature du RSA, arg1 est le message, arg2 est la taille en bytes du message, renvoie 1 si c'est authentifi� et 0 sinon */

  /* Fonctions amies de la classe RSAPub */
  /***************************************/

  // Op�rateurs de comparaison

  friend int operator==(const RSAPub&, const RSAPub&);
  /* Compare deux cl�s publiques, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const RSAPub&, const RSAPub&);
  /* Compare deux cl�s publiques, renvoie 1 si elles sont diff�rentes et 0 sinon */

  // Op�rateurs de flux

  friend ofstream& operator<<(ofstream&, const RSAPub&);
  friend ifstream& operator>>(ifstream&, RSAPub&);
  /* Op�rateurs permettant de lire et d'�crire des cl�s publiques dans un fichier */

private:
  RSAKeyPub keypub;
};

/****************************************************/
/* Fonctions externes aux classes RSAPriv et RSAPub */
/****************************************************/



#endif

/* Fin de librsa++.hpp */
