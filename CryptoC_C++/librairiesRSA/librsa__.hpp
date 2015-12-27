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
/* Déclaration des classes auxiliaires */
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
/* Déclaration des classes RSAPriv et RSAPub */
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
  /* Destructeur sûr */

  /* Fonctions membres */
  /*********************/

  // Fonction de décryptage

  UCHAR* decrypt(const largeint&, int*);
  /* Fonction de décryptage du RSA, arg1 est le texte crypté (largeint) et arg2 est rempli par la taille en byte du texte décrypté, renvoie le texte clair (chaine de bytes) */

  // Fonction de signature

  largeint sign(const UCHAR* const, const int);
  /* Fonction de signature du RSA, utilise RIPEMD160, arg1 est le message à signer, arg2 la taille en byte du message, renvoie la signature (largeint) */

  // Fonction d'export de la clé publique

  RSAKeyPub exportkeypub(void);
  /* Fonction qui génére la clé publique (e,n) à partir de la clé privée (e, d, p, q, n) */

  /* Fonctions amies de la classe RSAPriv */
  /******************************************/

  // Opérateurs de comparaison

  friend int operator==(const RSAPriv&, const RSAPriv&);
  /* Compare deux clés privées, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const RSAPriv&, const RSAPriv&);
  /* Compare deux clés privées, renvoie 1 si elles sont différentes et 0 sinon */

  // Opérateurs de flux

  friend ofstream& operator<<(ofstream&, const RSAPriv&);
  friend ifstream& operator>>(ifstream&, RSAPriv&);
  /* Opérateurs permettant de lire et d'écrire des clés privées dans un fichier */

  /* Opérateur d'affectation */
  /***************************/

  RSAPriv& operator=(const RSAPriv&);
  /* Opérateur permettant d'affecter une clé privée à une clé privée générique */

private:
  RSAKey keypriv;

  /* Fonctions auxiliaires */
  /*************************/

  // Fonction de décryptage rapide

  largeint fdecrypt(const largeint&);
  /* Fonction permettant de décrypter rapidement à l'aide des restes chinois, arg1 est le texte crypté (largeint), renvoie le texte clair (largeint) */

  // Gestion des clés

  int testkey(void);
  /* Fonction permettant de vérifier si la clé est valide en générant un message aléatoire et en le cryptant et décryptant avec fdecrypt */
  int makekey(const int);
  /* Fonction permettant de générer une clé privée (e,d,p,q,n) utilisée dans les constructeurs */
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
  /* Constructeur permettant de générer une clé publique à partir d'une clé privée */
  
  // Destructeur

  ~RSAPub();
  /* Destructeur sûr */

  /* Fonctions membres */
  /*********************/
  
  // Fonction de cryptage

  largeint crypt(const UCHAR* const, const int);
  /* Fonction de cryptage du RSA, arg1 est le texte clair et arg2 est la taille en bytes du texte clair (Taille texte clair < Taille du modulo n), renvoie le texte crypté (largeint) */

  // Fonction d'authentification de la signature

  int authen(const UCHAR* const, const largeint&, const int);
  /* Fonction d'authentification de la signature du RSA, arg1 est le message, arg2 est la taille en bytes du message, renvoie 1 si c'est authentifié et 0 sinon */

  /* Fonctions amies de la classe RSAPub */
  /***************************************/

  // Opérateurs de comparaison

  friend int operator==(const RSAPub&, const RSAPub&);
  /* Compare deux clés publiques, renvoie 1 si elles sont identiques et 0 sinon */
  friend int operator!=(const RSAPub&, const RSAPub&);
  /* Compare deux clés publiques, renvoie 1 si elles sont différentes et 0 sinon */

  // Opérateurs de flux

  friend ofstream& operator<<(ofstream&, const RSAPub&);
  friend ifstream& operator>>(ifstream&, RSAPub&);
  /* Opérateurs permettant de lire et d'écrire des clés publiques dans un fichier */

private:
  RSAKeyPub keypub;
};

/****************************************************/
/* Fonctions externes aux classes RSAPriv et RSAPub */
/****************************************************/



#endif

/* Fin de librsa++.hpp */
