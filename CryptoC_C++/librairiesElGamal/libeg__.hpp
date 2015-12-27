/***********************************************************************
 *                                                                     *
 *            Header pour les fonctions de ElGamal en C++              *
 *                                                                     *
 *                                                                     *
 *                          Pierre Paquay                              *
 *                                                                     *
 ***********************************************************************/


#ifndef EG_HPP
#define EG_HPP

#include "libmparithm++.hpp"
#include "ripemd.h"

/***************************************/
/* D�claration des classes auxiliaires */
/***************************************/

// Classe EGKey

class EGKey
{
public:
  largeint p, g, a, A;
  USHORT bitlen;
};

// Classe EGKeyPub

class EGKeyPub
{
public:
  largeint p, g, A;
  USHORT bitlen;
};

// Classe EGMess

class EGMess
{
public:
  largeint B0, c;
};

// Classe EGSign

class EGSign
{
public:
  largeint r, s;
};

/*********************************************/
/* D�claration des classes EGPriv et EGPub */
/*********************************************/

// Classe EGPriv

class EGPriv
{
public:

  /* Constructeurs et destructeur */
  /********************************/

  // Constructeurs

  inline EGPriv(void) {};
  EGPriv(const int);
  EGPriv(const int, const largeint&);
  /* Constructeurs prenant en argument le nombre de bit du modulo n et pour le second un seed largeint */
  
  // Destructeur

  ~EGPriv();
  /* Destructeur s�r */

  /* Fonctions membres */
  /*********************/

  // Fonction de d�cryptage

  UCHAR* decrypt(const EGMess&, int*);
  /* Fonction de d�cryptage de ElGamal, arg1 est le texte crypt� (EGMess) et arg2 est rempli par la taille en byte du texte d�crypt�, renvoie le texte clair (chaine de bytes) */

  // Fonction de signature

  EGSign sign(const UCHAR* const, const int);
  /* Fonction de signature de ElGamal, utilise RIPEMD160, arg1 est le message � signer, arg2 la taille en byte du message, renvoie la signature (largeint) */

  // Fonction d'export de la cl� publique

  EGKeyPub exportkeypub(void);
  /* Fonction qui g�n�re la cl� publique (p, g, a) � partir de la cl� priv�e (p, g, x) */

  /* Fonctions amies de la classe EGPriv */
  /******************************************/

  // Op�rateurs de comparaison

  friend int operator==(const EGPriv&, const EGPriv&);
  /* Compare deux cl�s priv�es, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const EGPriv&, const EGPriv&);
  /* Compare deux cl�s priv�es, renvoie 1 si elles sont diff�rentes et 0 sinon */

  // Op�rateurs de flux

  friend ofstream& operator<<(ofstream&, const EGPriv&);
  friend ifstream& operator>>(ifstream&, EGPriv&);
  /* Op�rateurs permettant de lire et d'�crire des cl�s priv�es dans un fichier */

  /* Op�rateur d'affectation */
  /***************************/

  EGPriv& operator=(const EGPriv&);
  /* Op�rateur permettant d'affecter une cl� priv�e � une cl� priv�e g�n�rique */

private:
  EGKey keypriv;

  /* Fonctions auxiliaires */
  /*************************/

  // Fonction de d�cryptage rapide

  largeint fdecrypt(const EGMess&);
  /* Fonction permettant de d�crypter rapidement � l'aide des restes chinois, arg1 est le texte crypt� (largeint), renvoie le texte clair (largeint) */

  // Gestion des cl�s

  int testkey(void);
  /* Fonction permettant de v�rifier si la cl� est valide en g�n�rant un message al�atoire et en le cryptant et d�cryptant avec fdecrypt */
  int makekey(const int);
  /* Fonction permettant de g�n�rer une cl� priv�e (p,g,a) utilis�e dans les constructeurs */
};

// Classe EGPub

class EGPub
{
public:
  /* Constructeurs et destructeur */
  /********************************/

  // Constructeurs

  inline EGPub(void) {};
  EGPub(const EGPriv&);
  /* Constructeur permettant de g�n�rer une cl� publique � partir d'une cl� priv�e */
  
  // Destructeur

  ~EGPub();
  /* Destructeur s�r */

  /* Fonctions membres */
  /*********************/
  
  // Fonction de cryptage

  EGMess crypt(const UCHAR* const, const int);
  /* Fonction de cryptage de ElGamal, arg1 est le texte clair et arg2 est la taille en bytes du texte clair (Taille texte clair < Taille du modulo n), renvoie le texte crypt� (EGMess) */

  // Fonction d'authentification de la signature

  int authen(const UCHAR* const, const EGSign&, const int);
  /* Fonction d'authentification de la signature de ElGamal, arg1 est le message, arg2 est la taille en bytes du message, renvoie 1 si c'est authentifi� et 0 sinon */

  /* Fonctions amies de la classe EGPub */
  /***************************************/

  // Op�rateurs de comparaison

  friend int operator==(const EGPub&, const EGPub&);
  /* Compare deux cl�s publiques, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const EGPub&, const EGPub&);
  /* Compare deux cl�s publiques, renvoie 1 si elles sont diff�rentes et 0 sinon */

  // Op�rateurs de flux

  friend ofstream& operator<<(ofstream&, const EGPub&);
  friend ifstream& operator>>(ifstream&, EGPub&);
  /* Op�rateurs permettant de lire et d'�crire des cl�s publiques dans un fichier */

private:
  EGKeyPub keypub;
};


#endif

/* Fin de libeg++.hpp */
