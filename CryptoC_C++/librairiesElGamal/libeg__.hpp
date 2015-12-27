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
/* Déclaration des classes auxiliaires */
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
/* Déclaration des classes EGPriv et EGPub */
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
  /* Destructeur sûr */

  /* Fonctions membres */
  /*********************/

  // Fonction de décryptage

  UCHAR* decrypt(const EGMess&, int*);
  /* Fonction de décryptage de ElGamal, arg1 est le texte crypté (EGMess) et arg2 est rempli par la taille en byte du texte décrypté, renvoie le texte clair (chaine de bytes) */

  // Fonction de signature

  EGSign sign(const UCHAR* const, const int);
  /* Fonction de signature de ElGamal, utilise RIPEMD160, arg1 est le message à signer, arg2 la taille en byte du message, renvoie la signature (largeint) */

  // Fonction d'export de la clé publique

  EGKeyPub exportkeypub(void);
  /* Fonction qui génére la clé publique (p, g, a) à partir de la clé privée (p, g, x) */

  /* Fonctions amies de la classe EGPriv */
  /******************************************/

  // Opérateurs de comparaison

  friend int operator==(const EGPriv&, const EGPriv&);
  /* Compare deux clés privées, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const EGPriv&, const EGPriv&);
  /* Compare deux clés privées, renvoie 1 si elles sont différentes et 0 sinon */

  // Opérateurs de flux

  friend ofstream& operator<<(ofstream&, const EGPriv&);
  friend ifstream& operator>>(ifstream&, EGPriv&);
  /* Opérateurs permettant de lire et d'écrire des clés privées dans un fichier */

  /* Opérateur d'affectation */
  /***************************/

  EGPriv& operator=(const EGPriv&);
  /* Opérateur permettant d'affecter une clé privée à une clé privée générique */

private:
  EGKey keypriv;

  /* Fonctions auxiliaires */
  /*************************/

  // Fonction de décryptage rapide

  largeint fdecrypt(const EGMess&);
  /* Fonction permettant de décrypter rapidement à l'aide des restes chinois, arg1 est le texte crypté (largeint), renvoie le texte clair (largeint) */

  // Gestion des clés

  int testkey(void);
  /* Fonction permettant de vérifier si la clé est valide en générant un message aléatoire et en le cryptant et décryptant avec fdecrypt */
  int makekey(const int);
  /* Fonction permettant de générer une clé privée (p,g,a) utilisée dans les constructeurs */
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
  /* Constructeur permettant de générer une clé publique à partir d'une clé privée */
  
  // Destructeur

  ~EGPub();
  /* Destructeur sûr */

  /* Fonctions membres */
  /*********************/
  
  // Fonction de cryptage

  EGMess crypt(const UCHAR* const, const int);
  /* Fonction de cryptage de ElGamal, arg1 est le texte clair et arg2 est la taille en bytes du texte clair (Taille texte clair < Taille du modulo n), renvoie le texte crypté (EGMess) */

  // Fonction d'authentification de la signature

  int authen(const UCHAR* const, const EGSign&, const int);
  /* Fonction d'authentification de la signature de ElGamal, arg1 est le message, arg2 est la taille en bytes du message, renvoie 1 si c'est authentifié et 0 sinon */

  /* Fonctions amies de la classe EGPub */
  /***************************************/

  // Opérateurs de comparaison

  friend int operator==(const EGPub&, const EGPub&);
  /* Compare deux clés publiques, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const EGPub&, const EGPub&);
  /* Compare deux clés publiques, renvoie 1 si elles sont différentes et 0 sinon */

  // Opérateurs de flux

  friend ofstream& operator<<(ofstream&, const EGPub&);
  friend ifstream& operator>>(ifstream&, EGPub&);
  /* Opérateurs permettant de lire et d'écrire des clés publiques dans un fichier */

private:
  EGKeyPub keypub;
};


#endif

/* Fin de libeg++.hpp */
