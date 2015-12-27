/***********************************************************************
 *                                                                     *
 *               Header pour les fonctions arithmétiques C++           *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#ifndef libarithmcpp_HPP
#define libarithmcpp_HPP

#include <iostream>
#include <fstream>
#include <new>
using namespace std;

#include "libmparithm.h"


/***********/
/* Erreurs */
/***********/

enum ERRORS {
  largeint_OK = 0x0000, // Pas de problème
  largeint_EOF = 0x0010, // Erreur E/S
  largeint_DBZ = 0x0020, // Division par 0
  largeint_NEW = 0x0040, // Opérateur new renvoie NULL
  largeint_OFL = 0x0080, // Overflow
  largeint_UFL = 0x0100, // Underflow
  largeint_VAL = 0x0200, // Argument non initialisé
  largeint_BOR = 0x0400, // Base incorrecte
  largeint_NPT = 0x0800, // Pointeur NULL
  largeint_ERR = 0x1000 // Erreur inconnue
};

/********************************************/
/* Taille des nombres premiers pour ElGamal */
/********************************************/

#define SIZEOFQ 160

/***************************************/
/* Déclaration des classes d'exception */
/***************************************/

class largeint_exception
{
public:
  const char* function;
  int arg, line;
  virtual void debugdata(void) const = 0;
  virtual ~largeint_exception() {function = 0;};
};

// Classe EOF

class largeint_IOErr : public largeint_exception
{
public:
  largeint_IOErr(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur I/O */

// Classe DBZ

class largeint_DivBy0 : public largeint_exception
{
public:
  largeint_DivBy0(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur division par 0 */

// Classe NEW

class largeint_NullNew : public largeint_exception
{
public:
  largeint_NullNew(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur du pointeur new */

// Classe OFL

class largeint_OFlow : public largeint_exception
{
public:
  largeint_OFlow(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur overflow */

// Classe UFL

class largeint_UFlow : public largeint_exception
{
public:
  largeint_UFlow(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur underflow */

// Classe VAL

class largeint_NoInit : public largeint_exception
{
public:
  largeint_NoInit(const char* const, const int, const int);
  void debugdata(void) const;
};
/* Classe d'erreur init = 0 */

// Classe BOR

class largeint_Base : public largeint_exception
{
public:
  largeint_Base(const char* const, const int);
  void debugdata(void) const;
};
/* Classe d'erreur base invalide */

// Classe NPT

class largeint_NullPtr : public largeint_exception
{
public:
  largeint_NullPtr(const char* const, const int, const int);
  void debugdata(void) const;
};
/* Classe d'erreur pointeur NULL */

// Classe ERR

class largeint_UnknErr : public largeint_exception
{
public:
  largeint_UnknErr(const char* const, const int, const int);
  void debugdata(void) const;
};
/* Classe d'erreur non identifiée */

/******************************************/
/* Déclaration de la classe largeint_init */
/******************************************/

class largeint_init
{
public:
  largeint_init(void);
};
/* Classe d'initialisation pour les I/O */

/*************************************/
/* Déclaration de la classe largeint */
/*************************************/

class largeint
{
 public:

  /* Classes amies */
  /*****************/

  friend largeint_init::largeint_init(void);

  enum {
    largeintdec = 0x10,
    largeintoct = 0x20,
    largeinthex = 0x40,
    largeintshowb = 0x80,
    largeintbin = 0x100,
  };

  /* Constructeurs et destructeur */
  /********************************/

  largeint(void);
  largeint(const largeint&);
  largeint(const char* const, const int);
  largeint(const UCHAR* const, const int);
  largeint(const USHORT);
  largeint(const ULONG);
  largeint(const BIGINT);
#ifndef ARCH64
  largeint(const unsigned int);
#endif
  /* Constructeurs à partir des types classiques du C++ */

  // Destructeur

  ~largeint();
  /* Destructeur sûr */

  /* Opérateur d'assignement */
  /***************************/

  const largeint& operator=(const largeint&);
  /* Opérateur := */

  /* Fonctions membres */
  /*********************/

  // Fonctions membres d'arithmétique

  const largeint& add(const largeint&);
  const largeint& add(const USHORT);
  /* Fonction qui renvoie l'objet additionné de arg1, signale largeint_OFL si overflow et largeint_OK sinon */

  const largeint& sub(const largeint&);
  const largeint& sub(const USHORT);
  /* Fonction qui renvoie l'objet soustrait de arg1, signale largeint_UFL si underflow et largeint_OK sinon */

  const largeint& mul(const largeint&);
  const largeint& mul(const USHORT);
  /* Fonction qui renvoie l'objet multiplié par arg1, signale largeint_OFL si overflow et largeint_OK sinon */

  const largeint& sqr(void);
  /* Fonction qui renvoie l'objet élevé au carré, signale largeint_OFL si overflow et largeint_OK sinon */

  const largeint& div(const largeint&, largeint&);
  const largeint& div(const USHORT, largeint&);
  /* Fonction qui renvoie l'objet divisé par arg1 et renvoie arg2 qui est le reste, signale largeint_DBZ si arg1 = 0 et largeint_OK sinon */

  const largeint& mod(const largeint&);
  const USHORT mod(const USHORT);
  /* Fonction qui renvoie le reste de l'objet divisé par arg1, signale largeint_DBZ si arg1 = 0 et largeint_OK sinon */
  const largeint& mod2(const USHORT);
  /* Fonction qui renvoie le reste de l'objet divisé par 2^arg1, signale largeint_OK */

  // Fonctions membres d'arithmétique modulo

  const largeint& addmod(const largeint&, const largeint&);
  const largeint& addmod(const USHORT, const largeint&);
  /* Fonction qui renvoie la somme de l'objet avec arg1 réduit modulo arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const largeint& submod(const largeint&, const largeint&);
  const largeint& submod(const USHORT, const largeint&);
  /* Fonction qui renvoie la différence de l'objet avec arg1 réduit modulo arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const largeint& mulmod(const largeint&, const largeint&);
  const largeint& mulmod(const USHORT, const largeint&);
  /* Fonction qui renvoie le produit de l'objet avec arg1 réduit modulo arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const largeint& sqrmod(const largeint&);
  /* Fonction qui renvoie lecarré de l'objet réduit modulo arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const int eqmod(const largeint&, const largeint&);
  /* Fonction qui renvoie 1 si l'objet est égal à arg1 modulo arg2 et 0 sinon, signale largeint_DBZ si arg2 = 0 */

  // Fonctions membres exponentielles

  const largeint& expmod(const largeint&, const largeint&);
  /* Fonction qui renvoie l'objet exposant arg1 et réduit modulo arg2, signale largeint_NPT si pointeur NULL, largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const largeint& expmod(const USHORT, const largeint&);
  /* Fonction qui renvoie l'objet exposant arg1 et réduit modulo arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  const largeint& expmod2(const USHORT, const largeint&);
  /* Fonction qui renvoie l'objet exposant 2^arg1 et réduit modulo arg2, largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  // Fonction membres opérant sur les bits

  const largeint& shift(const int);
  /* Fonction qui renvoie l'objet décalé de arg1 bits, signale largeint_OFL si overflow, largeint_UFL si underflow et largeint_OK sinon */

  const largeint& setbit(const unsigned int);
  /* Fonction qui renvoie l'objet dont le bit à la position arg1 est mis à 1, signale largeint_OFL si overflow et largeint_OK sinon */

  const largeint& clearbit(const unsigned int);
  /* Fonction qui renvoie l'objet dont le bit à la position arg1 est mis à 0, signale largeint_OK */

  const int testbit(const unsigned int) const;
  /* Fonction qui renvoie 1 si le bit à la position arg1 était à 1 et sinon */

  const unsigned int numbits(void) const;
  /* Fonction qui renvoie le nombre de bits significatifs de l'objet */

  // Fonctions membres de théorie des nombres

  const int iseven(void) const;
  /* Fonction qui renvoie 1 si l'objet est pair et 0 sinon */

  const int MRprime(const unsigned int, const unsigned int) const;
  const int SSprime(const unsigned int, const unsigned int) const;
  /* Fonction qui renvoie 1 si l'objet est (probablement) premier et 0 sinon, avec les nombres premiers de arg1 et arg2 itérations */

  const largeint issqr(void) const;
  /* Fonction qui renvoie la racine carrée de l'objet si c'est un carré parfait, sinon il n'est pas initialisé */

  const largeint introot(void) const;
  /* Fonction qui renvoie la partie entière de la racine carrée de l'objet */

  const largeint gcd(const largeint&) const;
  /* Fonction qui renvoie le pgcd de l'objet avec arg1 */

  const largeint lcm(const largeint&) const;
  /* Fonction qui renvoie le ppcm de l'objet avec arg1, signale largeint_OFL si overflow et largeint_OK sinon */

  const largeint xgcd(const largeint&, largeint&, int&, largeint&, int&) const;
  /* Fonction qui renvoie le pgcd de l'objet avec arg1 et les coefficients de Bezout */

  const largeint invmod(const largeint&) const;
  /* Fonction qui renvoie l'inverse modulo arg1 de l'objet si celui-ci existe et 0 sinon */
  
  const int jacobi(const largeint&) const;
  /* Fonction qui renvoie la valeur du symbole de Jacobi de l'objet avec arg1 */

  const largeint root(const largeint&) const;
  /* Fonction qui renvoie la racine carrée de l'objet modulo arg1 s'il existe et 0 sinon, signale alors largeint_ERR */

  const largeint root(const largeint&, const largeint&) const;
  /* Fonction qui renvoie la racine carrée de l'objet modulo arg1*arg2 s'il existe et 0 sinon, signale alors largeint_ERR */

  const largeint chinrem(const largeint&, const largeint&, const largeint&) const;
  /* Fonction qui résoud le système de congruences objet mod arg1 et arg2 mod arg3, renvoie largeint_OFL si overflow, largeint_ERR si les modulo ne sont pas premiers et largeint_OK sinon */

  // Fonctions membres de conversion

  const char* largeint2str(const USHORT, const int) const;
  /* Fonction qui renvoie la chaîne de caractères générée par l'objet dans la base arg1 et avec la base montrée selon arg2 */

  const UCHAR* largeint2byte(int*) const;
  /* Fonction qui renvoie une chaîne d'octets générée par l'objet et dont la longueur est donnée par arg1 */

  // Fonction membre de purge

  const void purge(void);
  /* Fonction qui remplit l'objet de 0 */

  /* Fonctions amies */
  /*******************/

  // Fonctions amies d'arithmétique

  friend const largeint add(const largeint&, const largeint&);
  friend const largeint add(const largeint&, const USHORT);
  /* Fonction qui renvoie arg1 additionné de arg2, signale largeint_OFL si overflow et largeint_OK sinon */

  friend const largeint sub(const largeint&, const largeint&);
  friend const largeint sub(const largeint&, const USHORT);
  /* Fonction qui renvoie arg1 soustrait de arg2, signale largeint_UFL si underflow et largeint_OK sinon */

  friend const largeint mul(const largeint&, const largeint&);
  friend const largeint mul(const largeint&, const USHORT);
  /* Fonction qui renvoie arg1 multiplié par arg2, signale largeint_OFL si overflow et largeint_OK sinon */

  friend const largeint sqr(const largeint&);
  /* Fonction qui renvoie arg1 élevé au carré, signale largeint_OFL si overflow et largeint_OK sinon */

  friend const largeint div(const largeint&, const largeint&, largeint&);
  friend const largeint div(const largeint&, const USHORT, largeint&);
  /* Fonction qui renvoie arg1 divisé par arg2 et renvoie arg3 qui est le reste, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  friend const largeint mod(const largeint&, const largeint&);
  friend const USHORT mod(const largeint&, const USHORT);
  /* Fonction qui renvoie le reste de arg1 divisé par arg2, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  friend const largeint mod2(const largeint&, const USHORT);
  /* Fonction qui renvoie le reste de arg1 divisé par 2^arg2, signale largeint_OK */

  // Fonction amies d'arithmétique modulo

  friend const largeint addmod(const largeint&, const largeint&, const largeint&);
  friend const largeint addmod(const largeint&, const USHORT, const largeint&);
  /* Fonction qui renvoie la somme de arg1 avec arg2 réduit modulo arg3, signale largeint_DBZ si arg3 = 0 et largeint_OK sinon */

  friend const largeint submod(const largeint&, const largeint&, const largeint&);
  friend const largeint submod(const largeint&, const USHORT, const largeint&);
  /* Fonction qui renvoie la différence de arg1 avec arg2 réduit modulo arg3, signale largeint_DBZ si arg3 = 0 et largeint_OK sinon */

  friend const largeint mulmod(const largeint&, const largeint&, const largeint&);
  friend const largeint mulmod(const largeint&, const USHORT, const largeint&);
  /* Fonction qui renvoie le produit de arg1 avec arg2 réduit modulo arg3, signale largeint_DBZ si arg3 = 0 et largeint_OK sinon */

  friend const largeint sqrmod(const largeint&, const largeint&);
  /* Fonction qui renvoie le carré de arg1 réduit modulo arg2, signale largeint_DBZ si arg3 = 0 et largeint_OK sinon */

  friend const int eqmod(const largeint &, const largeint&, const largeint&);
  /* Fonction qui renvoie 1 si arg1 est égal à arg2 modulo arg3 et 0 sinon, signale largeint_DBZ si arg3 = 0 */

  // Fonctions amies exponentielles

  friend const largeint expmod(const largeint&, const largeint&, const largeint&);
  /* Fonction qui renvoie arg1 exposant arg2 et réduit modulo arg3, signale largeint_NPT si pointeur NULL, largeint_DBZ si arg3 = 0 et largeint_OK sinon */

  friend const largeint expmod(const largeint&, const USHORT, const largeint&);
  /* Fonction qui renvoie arg1 exposant arg2 et réduit modulo arg3, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  friend const largeint expmod(const USHORT, const largeint&, const largeint&);
  /* Fonction qui renvoie arg1 exposant arg2 et réduit modulo arg3, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  friend const largeint expmod2(const largeint&, const USHORT, const largeint&);
  /* Fonction qui renvoie arg1 exposant 2^arg2 et réduit modulo arg3, signale largeint_DBZ si arg2 = 0 et largeint_OK sinon */

  // Fonctions amies opérant sur les bits

  friend const largeint shift(const largeint&, const int);
  /* Fonction qui renvoie arg1 décalé de arg2 bits, signale largeint_OFL si overflow, largeint_UFL si underflow et largeint_OK sinon */

  friend const unsigned int numbits(const largeint&);
  /* Fonction qui renvoie le nombre de bits significatifs de arg1 */

  // Fonctions amies de théorie des nombres

  friend const int iseven(const largeint&);
  /* Fonction qui renvoie 1 si arg1 est pair et 0 sinon */

  friend const USHORT sieve(const largeint&, const unsigned int);
  friend const int MRprime(const largeint&, const unsigned int, const unsigned int);
  friend const int SSprime(const largeint&, const unsigned int, const unsigned int);
  /* Fonction qui renvoie 1 si arg1 est (probablement) premier et 0 sinon, avec les nombres premiers de arg2 et arg3 itérations */

  friend const largeint issqr(const largeint&);
  /* Fonction qui renvoie la racine carrée de arg1 si c'est un carré parfait, sinon il n'est pas initialisé */

  friend const largeint introot(const largeint&);
  /* Fonction qui renvoie la partie entière de la racine carrée de arg1 */

  friend const largeint gcd(const largeint&, const largeint&);
  /* Fonction qui renvoie le pgcd de arg1 avec arg2 */

  friend const largeint lcm(const largeint&, const largeint&);
  /* Fonction qui renvoie le ppcm de arg1 avec arg2, signale largeint_OFL si overflow et largeint_OK sinon */

  friend const largeint xgcd(const largeint&, const largeint&, largeint&, int&, largeint&, int&);
  /* Fonction qui renvoie le pgcd de arg1 avec arg2 et les coefficients de Bezout */

  friend const largeint invmod(const largeint&, const largeint&);
  /* Fonction qui renvoie l'inverse modulo arg2 de arg1 si celui-ci existe et 0 sinon */

  friend const int jacobi(const largeint&, const largeint&);
  /* Fonction qui renvoie la valeur du symbole de Jacobi de arg1 avec arg2 */

  friend const largeint root(const largeint&, const largeint&);
  /* Fonction qui renvoie la racine carrée de arg1 modulo arg2 s'il existe et 0 sinon, signale alors largeint_ERR */

  friend const largeint root(const largeint&, const largeint&, const largeint&);
  /* Fonction qui renvoie la racine carrée de arg1 modulo arg2*arg3 s'il existe et 0 sinon, signale alors largeint_ERR */

  friend const largeint chinrem(const unsigned int, largeint**);
  /* Fonction qui résoud le système de congruences de arg1 équations avec arg2 les coefficients, signale largeint_OFL si overflow, largeint_ERR si les modulo ne sont pas premiers ou nombre d'équations égal à 0 et largeint_OK sinon */

  friend const largeint primelt(const unsigned int, largeint**);
  /* Fonction qui génère un élément primitif avec arg1 le nombre de facteurs premiers de p-1 et arg2 ceux-ci, signale largeint_ERR si pas premier > 2 et largeint_OK sinon */

  // Fonctions amies de génération aléatoire

  friend void seed64(const largeint&);
  friend void seed64(const ULONG);
  /* Fonction qui initialise la fonction rand64 */

  friend largeint rand64(const int);
  /* Fonction qui génère un nombre aléatoire par congruence linéaire de arg1 bits */

  friend largeint rand64(const largeint&, const largeint&);
  /* Fonction qui génère un nombre aléatoire par congruence linéaire compris entre arg1 et arg2 */

  friend void seedBBS(const largeint&);
  friend void seedBBS(const ULONG);
  /* Fonction qui initialise la fonction randBBS */

  friend largeint randBBS(const int);
  /* Fonction qui génère un nombre aléatoire par Blum-Blum-Shub de arg1 bits */

  friend largeint randBBS(const largeint&, const largeint&);
  /* Fonction qui génère un nombre aléatoire par Blum-Blum-Shub compris entre arg1 et arg2 */

  // Fonctions amies de génération de nombres premiers

  friend const largeint primeBBS(const largeint&, const largeint&, const largeint&);
  /* Fonction qui génère un nombre premier avec BBS compris entre arg1 et arg2 et premier avec arg3 */

  friend const largeint primeBBS(const USHORT, const largeint&);
  /* Fonction qui génère un nombre premier avec BBS de arg1 bits et premier avec arg2 */

  friend const largeint primeBBS(const USHORT);
  /* Fonction qui génère un nombre premier avec BBS de arg1 bits */

  friend const largeint prime64(const largeint&, const largeint&, const largeint&);
  /* Fonction qui génère un nombre premier compris entre arg1 et arg2 et premier avec arg3 */

  friend const largeint prime64(const USHORT, const largeint&);
  /* Fonction qui génère un nombre premier de arg1 bits et premier avec arg2 */

  friend const largeint prime64(const USHORT);
  /* Fonction qui génère un nombre premier de arg1 bits */

  friend const largeint safeprime(const USHORT, const unsigned int);
  /* Fonction qui génère un nombre premier de p possédant un très grand facteur premier de arg1 bits, renvoie 0 si un problème est survenu */

  friend const largeint extendprime(const largeint&, const largeint&, const largeint&);
  /* Fonction qui génère un nombre premier congru à 1 mod arg3 compris entre arg1 et arg2 */

  friend const largeint extendprime(const USHORT, const largeint&);
  /* Fonction qui génère un nombre premier congru à 1 mod arg2 de arg1 bits */

  friend const largeint primeprimelt(const USHORT, largeint&);
  /* Fonction qui génère un nombre premier p possédant arg1 bits et un très grand facteur premier et qui remplit arg2 avec un élément primitif modulo p */

  friend const largeint elgamalprime(const USHORT, const USHORT, largeint&);
  /* Fonction qui génère un nombre premier p possédant arg1 bits et tel que p-1 possède un facteur premier de arg2 bits et d'autres facteurs premiers plus grands que arg2 bits et qui remplit arg3 avec un élément primitif modulo p */

  // Fonctions amies de conversion

  friend char* largeint2str(const largeint&, const USHORT, const int);
  /* Fonction qui renvoie la chaîne de caractères générée par arg1 dans la base arg2 et avec la base montrée selon arg3 */

  friend UCHAR* largeint2byte(const largeint&, int*);
  /* Fonction qui renvoie une chaîne d'octets générée par arg1 et dont la longueur est donnée par arg2 */

  // Fonction amie de purge

  friend const void purge(largeint&);
  /* Fonction qui remplit arg1 de 0 */

  /* Opérateurs surchargés */
  /*************************/

  // Opérateurs de comparaisons surchargés

  friend const int operator==(const largeint&, const largeint&);
  friend const int operator!=(const largeint&, const largeint&);
  friend const int operator<(const largeint&, const largeint&);
  friend const int operator>(const largeint&, const largeint&);
  friend const int operator<=(const largeint&, const largeint&);
  friend const int operator>=(const largeint&, const largeint&);

  // Opérateurs arithmétiques surchargés

  friend const largeint operator+(const largeint&, const largeint&);
  friend const largeint operator+(const largeint&, const USHORT);
  friend const largeint operator+(const USHORT, const largeint&);
  friend const largeint operator-(const largeint&, const largeint&);
  friend const largeint operator-(const largeint&, const USHORT);
  friend const largeint operator*(const largeint&, const largeint&);
  friend const largeint operator*(const largeint&, const USHORT);
  friend const largeint operator*(const USHORT, const largeint&);
  friend const largeint operator/(const largeint&, const largeint&);
  friend const largeint operator/(const largeint&, const USHORT);
  friend const largeint operator%(const largeint&, const largeint&);
  friend const USHORT operator%(const largeint&, const USHORT);

  // Opérateurs booléens surchargés

  friend const largeint operator&(const largeint&, const largeint&);
  friend const largeint operator|(const largeint&, const largeint&);
  friend const largeint operator^(const largeint&, const largeint&);
  friend const largeint operator<<(const largeint&, const int);
  friend const largeint operator>>(const largeint&, const int);

  /* Gestion des erreurs */
  /***********************/

  // Fonction de gestion des erreurs

  static void panic(ERRORS, const char* const, const int, const int);
  /* Fonction qui gère les erreurs en générant des messages quant à la nature et l'emplacement de l'erreur */

  // Extraction du statut d'un objet de la classe largeint

  ERRORS get_status(void);
  /* Fonction qui renvoie le statut de l'objet */

  /* Gestion I/O */
  /***************/

  // Formatage des flux

  static long flag(void);
  static long flag(ostream&);
  static long setf(ostream&, long);
  static long setf(long);
  static long unsetf(ostream&, long);
  static long unsetf(long);
  static long restoref(ostream&, long);
  static long restoref(long);

  // Opérateurs de flux surchargées
  
  friend ostream& operator<<(ostream&, const largeint&);
  friend ofstream& operator<<(ofstream&, const largeint&);
  friend ifstream& operator>>(ifstream&, largeint&);

 private:
  bigint *n_l;
  int init;
  ERRORS status;


  /* Classe largeint_init */
  /************************/

  static largeint_init setup;

  static long flagindex;
};


/**************************/
/* Fonctions auxiliaires  */
/**************************/

// Manipulateurs

ostream& largeint_Bin(ostream&);
ostream& largeint_Oct(ostream&);
ostream& largeint_Dec(ostream&);
ostream& largeint_Hex(ostream&);
ostream& largeint_Showb(ostream&);
ostream& largeint_NoShowb(ostream&);
/* Manipulateurs à insérer dans un flux I/O pour changer le formatage de la sortie */

// Modèles d'écriture et de lecture dans un fichier

int write_ushort(ofstream&, bigint);
int read_ushort(ifstream&, bigint*);



#endif


/* Fin de libarithmcpp.hpp */
