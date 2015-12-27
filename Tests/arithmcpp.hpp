/***********************************************************************
 *                                                                     *
 *               Header pour les fonctions arithmétiques C++           *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#ifndef arithmcpp_HPP
#define arithmcpp_HPP

#include <iostream.h>
#include <fstream.h>
#include <new.h>
#include "arithm.h"

/* Erreurs */

enum BINT_ERRORZ {
  BINT_OK = 0x0000,
  BINT_EOF = 0x0010,
  BINT_DBZ = 0x0020,
  BINT_NHP = 0x0040,
  BINT_OFL = 0x0080,
  BINT_UFL = 0x0100,
  BINT_VAL = 0x0200,
  BINT_BOR = 0x0400,
  BINT_NPT = 0x0800,
  BINT_ERR = 0x1000
};


/* Déclaration de la classe BINT */

class BINT
{
 public:

  // Constructeurs

  BINT(void);
  BINT(const BINT&);
  BINT(const char* const, const int);
  BINT(const UCHAR* const, const int);
  BINT(const USHORT);
  BINT(const BIGINT);

  // Destructeur

  ~BINT();

  // Opérateurs d'assignement

  const BINT& operator=(const BINT&);

  // Opérateurs logiques surchargés amis

  friend const int operator==(const BINT&, const BINT&);
  friend const int operator!=(const BINT&, const BINT&);
  friend const int operator<(const BINT&, const BINT&);
  friend const int operator>(const BINT&, const BINT&);
  friend const int operator<=(const BINT&, const BINT&);
  friend const int operator>=(const BINT&, const BINT&);

  // Opérateurs surchargés booléens amis

  friend const BINT operator&(const BINT&, const BINT&);
  friend const BINT operator|(const BINT&, const BINT&);
  friend const BINT operator^(const BINT&, const BINT&);
  friend const BINT operator<<(const BINT&, const int);
  friend const BINT operator>>(const BINT&, const int);

  // Opérateurs surchargés arithmétiques amis

  friend const BINT operator+(const BINT&, const BINT&);
  friend const BINT operator+(const BINT&, const USHORT);
  friend const BINT operator-(const BINT&, const BINT&);
  friend const BINT operator-(const BINT&, const USHORT);
  friend const BINT operator*(const BINT&, const BINT&);
  friend const BINT operator/(const BINT&, const BINT&);
  friend const BINT operator%(const BINT&, const BINT&);

  // Fonctions membres d'arithmétique

  const BINT& add(const BINT&);
  const BINT& sub(const BINT&);
  const BINT& mul(const BINT&);
  const BINT& sqr(void);
  const BINT& div(const BINT&, BINT&);
  const BINT& mod(const BINT&);
  const BINT& mod2(const USHORT);

  // Fonctions membres d'arithmétique modulaire

  const BINT& addmod(const BINT&, const BINT&);
  const BINT& submod(const BINT&, const BINT&);
  const BINT& mulmod(const BINT&, const BINT&);
  const BINT& sqrmod(const BINT&);
  const BINT& expmod(const BINT&, const BINT&);
  const BINT& expmod(const USHORT, const BINT&);
  const BINT& expmod2(const USHORT, const BINT&);
  const int eqmod(const BINT&, const BINT&);
  const BINT& shift(const int);
  const BINT& setbit(const unsigned int);
  const BINT& clearbit(const unsigned int);
  const int testbit(const unsigned int);

  // Fonctions membres de théorie des nombres

  const unsigned int numbits(void);
  const int iseven(void);
  const int prime(void);
  const BINT issqr(void);
  const BINT introot(void);
  const BINT gcd(const BINT&);
  const BINT lcm(const BINT&);
  const BINT xgcd(const BINT&, BINT&, int&, BINT&, int&);
  const BINT invmod(const BINT&);
  const int jacobi(const BINT&);
  const BINT root(const BINT&);
  const BINT root(const BINT&, const BINT&);
  const BINT chinrem(const BINT&, const BINT&, const BINT&);

  // Fonctions membres de conversion

  char* bint2str(const USHORT, const int);
  UCHAR* bint2byte(int*);

  // Fonctions d'arithmétique amies

  friend const BINT add(const BINT&, const BINT&);
  friend const BINT sub(const BINT&, const BINT&);
  friend const BINT mul(const BINT&, const BINT&);
  friend const BINT sqr(const BINT&);
  friend const BINT div(const BINT&, const BINT&, BINT&);
  friend const BINT mod(const BINT&, const BINT&);
  friend const BINT mod2(const BINT&, const USHORT);

  // Fonctions d'arithmétique modulaire amies

  friend const BINT addmod(const BINT&, const BINT&, const BINT&);
  friend const BINT submod(const BINT&, const BINT&, const BINT&);
  friend const BINT mulmod(const BINT&, const BINT&, const BINT&);
  friend const BINT sqrmod(const BINT&, const BINT&);
  friend const BINT expmod(const BINT&, const BINT&, const BINT&);
  friend const BINT expmod(const BINT&, const USHORT, const BINT&);
  friend const BINT expmod(const USHORT, const BINT&, const BINT&);
  friend const BINT expmod2(const BINT&, const USHORT, const BINT&);
  friend const int eqmod(const BINT&, const BINT&, const BINT&);
  friend const BINT shift(const BINT&, const int);

  // Fonctions de théorie des nombres amies

  friend const unsigned int numbits(const BINT&);
  friend const int iseven(const BINT&);
  friend const int prime(const BINT&);
  friend const BINT issqr(const BINT&);
  friend const BINT introot(const BINT&);
  friend const BINT gcd(const BINT&, const BINT&);
  friend const BINT lcm(const BINT&, const BINT&);
  friend const BINT xgcd(const BINT&, const BINT&, BINT&, int&, BINT&, int&);
  friend const BINT invmod(const BINT&, const BINT&);
  friend const int jacobi(const BINT&, const BINT&);
  friend const BINT root(const BINT&, const BINT&);
  friend const BINT root(const BINT&, const BINT&, const BINT&);

  // Fonctions de conversion amies

  friend char* bint2str(const BINT&, const USHORT, const int);
  friend UCHAR* bint2byte(const BINT&, int*);

  // Fonctions de génération de nombres pseudo-aléatoires

  friend void seed64(const ULONG);
  friend void seed64(const BINT&);
  friend BINT rand64(const int);
  friend BINT rand64(const BINT&, const BINT&);
  friend int seedBBS(const ULONG);
  friend int seedBBS(const BINT&);
  friend BINT randBBS(const int);
  friend BINT randBBS(const BINT&, const BINT&);
  friend const BINT findprime(const BINT&, const BINT&, const BINT&);
  friend const BINT findprime(const USHORT, const BINT&);

  // Opérateurs de flux surchargés

  friend ostream& operator<<(ostream&, const BINT&);
  friend ofstream& operator<<(ofstream&, const BINT&);
  friend ifstream& operator>>(ifstream&, BINT&);

  // Formatage des flux

  BINT& write_format(const int, const int);

  // Fonction de gestion des erreurs

  static void panic(BINT_ERRORZ, const char* const, const int, const int);

  // Fonction membre de purge

  void purge(void);

  // Extraction du statut d'un objet de la classe BINT

  BINT_ERRORZ get_status(void);

 private:
  bigint *n_l;
  int maxlen;
  int init;
  BINT_ERRORZ status;

  int base;
  int showbase;
};

// Modèles d'écriture et de lecture dans un fichier (USHORT)

template <class T>
int write_ushort(T& s, bigint src)
{
  UCHAR buffer[sizeof(bigint)];
  unsigned int i, j;

  for (i = 0, j = 0; i < sizeof(bigint); i++, j = (i << 3))
    buffer[i] = (UCHAR)((src & (0xff << j)) >> j);
  s.write((const char*)buffer, sizeof(bigint));
  if (s == 0)
    return -1;
  else
    return 0;
}

template <class T>
int read_ushort(T& s, bigint* dest)
{
  UCHAR buffer[sizeof(bigint)];
  unsigned int i, j;

  s.read((char*)buffer, sizeof(bigint));
  if (s == 0)
    return -1;
  else
    {
      *dest = 0;
      for (i = 0, j = 0; i < sizeof(bigint); i++, j = (i << 3))
	*dest = *dest | ((bigint)buffer[i] << j);
      return 0;
    }
}

#endif

/* Fin de arithmcpp.hpp */
