/***********************************************************************
 *                                                                     *
 *                Code source pour la librairie RSA en C++             *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "librsa++.hpp"


static UCHAR* pkcs1(const UCHAR*, const int, const UCHAR, const UCHAR*, const int);
static UCHAR* pkcs1_inv(const UCHAR*, int*);

/* Constructeurs et destructeur de la classe RSAPriv */
/*****************************************************/

// Constructeurs

RSAPriv::RSAPriv(const int len)
{
  int status = 0;

  seedBBS(time(NULL));
  while (status != 1)
    status = makekey(len);
}

RSAPriv::RSAPriv(const int len, const largeint& seed)
{
  int status = 0;

  seedBBS(seed);
  while (status != 1)
    status = makekey(len);
}

// Destructeur

RSAPriv::~RSAPriv()
{
  keypriv.p.purge();
  keypriv.q.purge();
  keypriv.n.purge();
  keypriv.e.purge();
  keypriv.d.purge();
  keypriv.bitlen = 0;
}

/* Fonctions membres de la classe RSAPriv */
/******************************************/

// Fonction de décryptage

UCHAR* RSAPriv::decrypt(const largeint& c, int* lenmess)
{
  UCHAR* M;

  M = largeint2byte(fdecrypt(c), lenmess);

  return (pkcs1_inv(M, lenmess));
}

// Fonction de signature

largeint RSAPriv::sign(const UCHAR* const R, const int lenmess)
{
  largeint s;
  int lenblock;
  USHORT bytlen;

  bytlen = (keypriv.bitlen >> 3);
  if ((keypriv.bitlen % 8) > 0)
    bytlen = bytlen + 1;
  lenblock = bytlen - 1;
  UCHAR* block = new UCHAR[lenblock];
  if (pkcs1(block, lenblock, 01, ripemd160((UCHAR*)R, (ULONG)lenmess), 20) == NULL)
    {
      delete [] block;
      return largeint((USHORT)0);
    }
  largeint r(block, lenblock);
  delete [] block;
  s = fdecrypt(r);

  return s;
}

// Fonction d'export de la clé publique

RSAKeyPub RSAPriv::exportkeypub(void)
{
  RSAKeyPub keypubtemp;

  keypubtemp.e = keypriv.e;
  keypubtemp.n = keypriv.n;
  keypubtemp.bitlen = keypriv.bitlen;

  return keypubtemp;
}

/* Fonctions amies de la classe RSAPriv */
/******************************************/

// Opérateurs de comparaison

int operator==(const RSAPriv& key1, const RSAPriv& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.keypriv.e == key2.keypriv.e) && (key1.keypriv.d == key2.keypriv.d)
      && (key1.keypriv.n == key2.keypriv.n) && (key1.keypriv.p == key2.keypriv.p)
      && (key1.keypriv.q == key2.keypriv.q) && (key1.keypriv.bitlen == key2.keypriv.bitlen))
    return 1;
  else
    return 0;
}

int operator!=(const RSAPriv& key1, const RSAPriv& key2)
{
  if (&key1 == &key2)
    return 0;
  if ((key1.keypriv.e != key2.keypriv.e) || (key1.keypriv.d != key2.keypriv.d)
      || (key1.keypriv.n != key2.keypriv.n) || (key1.keypriv.p != key2.keypriv.p)
      || (key1.keypriv.q != key2.keypriv.q) || (key1.keypriv.bitlen != key2.keypriv.bitlen))
    return 1;
  else
    return 0;
}

// Opérateurs de flux

ofstream& operator<<(ofstream& s, const RSAPriv& wkey)
{
  s << wkey.keypriv.d << wkey.keypriv.e << wkey.keypriv.n
    << wkey.keypriv.p << wkey.keypriv.q;
  write_ushort(s, wkey.keypriv.bitlen);

  return s;
}

ifstream& operator>>(ifstream& s, RSAPriv& rkey)
{
  s >> rkey.keypriv.d >> rkey.keypriv.e >> rkey.keypriv.n
    >> rkey.keypriv.p >> rkey.keypriv.q;
  read_ushort(s, &rkey.keypriv.bitlen);

  return s;
}

/* Opérateur d'affectation */
/***************************/

RSAPriv& RSAPriv::operator=(const RSAPriv& akey)
{
  if (&akey != this)
    {
      keypriv.e = akey.keypriv.e;
      keypriv.d = akey.keypriv.d;
      keypriv.n = akey.keypriv.n;
      keypriv.p = akey.keypriv.p;
      keypriv.q = akey.keypriv.q;
      keypriv.bitlen = akey.keypriv.bitlen;
    }
  return *this;
}
      

/* Fonctions auxiliaires de la classe RSAPriv */
/**********************************************/

// Fonction de décryptage rapide

largeint RSAPriv::fdecrypt(const largeint& c)
{
  largeint a1, a2, m, dp, dq, r;

  dp = keypriv.d % (keypriv.p - 1);
  dq = keypriv.d % (keypriv.q - 1);
  r = invmod(keypriv.p, keypriv.q);
  a1 = expmod(c, dp, keypriv.p);
  a2 = expmod(c, dq, keypriv.q);
  m = submod(a2, a1, keypriv.q);
  m = a1 + mulmod(m, r, keypriv.q) * keypriv.p;

  return m;
}

// Gestion des clés

int RSAPriv::testkey(void)
{
  largeint m = randBBS(keypriv.bitlen >> 1);
  largeint c;

  c = expmod(m, keypriv.e, keypriv.n);
  if (fdecrypt(c) == m)
    return 1;
  else
    return 0;
}

int RSAPriv::makekey(const int len)
{
  largeint qmin, qmax, phi;
  USHORT m;
  largeint zero = largeint((USHORT)0);
  largeint one = largeint((USHORT)1);

  m = ((len + 1) >> 1) - ((usrandBBS_l() % 11) + 2);/* 2^(m-r-1) <= p < 2^(m-r) avec m = Floor((len+1)/2) et r aléatoire dans [2,13[ */
  keypriv.p = primeBBS(m);
  qmin = zero.setbit(len - 1) / keypriv.p + 1;/* qmin = Floor((2^(len - 1))/p + 1) */
  qmax = zero.setbit(len) / keypriv.p;/* qmax = Floor((2^len)/p) */
  keypriv.q = primeBBS(qmin, qmax, one);
  keypriv.n = keypriv.p * keypriv.q;/* 2^(len - 1) <= n < 2^len */
  phi = keypriv.n - keypriv.p - keypriv.q + 1;
  keypriv.e = randBBS(64) | one;
  while (gcd(keypriv.e, phi) != one)
    keypriv.e = keypriv.e + 2;
  keypriv.d = (keypriv.e).invmod(phi);
  keypriv.bitlen = numbits(keypriv.n);

  return testkey();
}

/* Constructeurs et destructeur de la classe RSAPub */
/*****************************************************/

// Constructeurs

RSAPub::RSAPub(const RSAPriv& key)
{
  keypub = ((RSAPriv)key).exportkeypub();
}

// Destructeur

RSAPub::~RSAPub()
{
  keypub.n.purge();
  keypub.e.purge();
  keypub.bitlen = 0;
}

/* Fonctions membres */
/*********************/

// Fonction de cryptage

largeint RSAPub::crypt(const UCHAR* const M, int lenmess)
{
  largeint c;
  int lenblock;
  USHORT bytlen;

  bytlen = (keypub.bitlen >> 3);
  if ((keypub.bitlen % 8) > 0)
    bytlen = bytlen + 1;
  lenblock = bytlen - 1; /* Pour compenser le fait que dans PKCS1, on place 0x00 en tête, ce qui n'est pas utile ici puisqu'on élimine les leading zeros (Taille des blocs est bytlen - 1 < bytlen donc bloc < n */
  UCHAR* block = new UCHAR[lenblock];
  if (pkcs1(block, lenblock, 02, M, lenmess) == NULL)
    {
      delete [] block;
      return largeint((USHORT)0);
    }
  largeint m(block, lenblock);
  c = expmod(m, keypub.e, keypub.n);

  return c;
}

// Fonction d'authentification de la signature

int RSAPub::authen(const UCHAR* const M, const largeint& s, const int lenmess)
{
  UCHAR* S;
  UCHAR* H;
  int l, verif = 0;
  
  S = largeint2byte(expmod(s, keypub.e, keypub.n), &l);
  S = pkcs1_inv(S, &l);
  H = ripemd160((UCHAR*)M, (ULONG)lenmess);
  if (l == 20)
    verif = !memcmp((char*)H, (char*)S, 20);

  return verif;
} 

/* Fonctions amies de la classe RSAPub */
/***************************************/

// Opérateurs de comparaison

int operator==(const RSAPub& key1, const RSAPub& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.keypub.e == key2.keypub.e) && (key1.keypub.n == key2.keypub.n)
      && (key1.keypub.bitlen == key2.keypub.bitlen))
    return 1;
  else
    return 0;
}

int operator!=(const RSAPub& key1, const RSAPub& key2)
{
  if (&key1 == &key2)
    return 0;
  if ((key1.keypub.e != key2.keypub.e) || (key1.keypub.n != key2.keypub.n)
      || (key1.keypub.bitlen != key2.keypub.bitlen))
    return 1;
  else
    return 0;
}

// Opérateur de flux

ofstream& operator<<(ofstream& s, const RSAPub& wkey)
{
  s << wkey.keypub.e << wkey.keypub.n;
  write_ushort(s, wkey.keypub.bitlen);

  return s;
}

ifstream& operator>>(ifstream& s, RSAPub& rkey)
{
  s >> rkey.keypub.e >> rkey.keypub.n;
  read_ushort(s, &rkey.keypub.bitlen);

  return s;
}

/****************************************************/
/* Fonctions externes aux classes RSAPriv et RSAPub */
/****************************************************/

UCHAR* pkcs1(const UCHAR* EB, const int lenmod, const UCHAR BT, const UCHAR* data, const int lendata)
{
  int i, l = lenmod - 2 - lendata;
  UCHAR* temp = (UCHAR*)EB;

  if (l < 8)
    return NULL;
  switch (BT)
    {
    case 01:
      *temp = 0x01;
      temp = temp + 1;
      while (l > 0)
	{
	  *temp = 0xff;
	  temp = temp + 1;
	  l = l - 1;
	}
      break;
    case 02:
      *temp = 0x02;
      temp = temp + 1;
      while (l > 0)
	{
	  do
	    *temp = ucrandBBS_l();
	  while (*temp == 0); 
	  temp = temp + 1;
	  l = l - 1;
	}
      break;
    default:
      return NULL;
    }
  *temp = 0x00;
  temp = temp + 1;
  for (i = 1; i <= lendata; i++)
    {
      *temp = *data;
      temp = temp + 1;
      data = data + 1;
    }

  return (UCHAR*)EB;
}

UCHAR* pkcs1_inv(const UCHAR* EB, int* lendata)
{
  UCHAR* temp = (UCHAR*)EB;
  UCHAR BT;
  int err = 0, nbrerempl = 0;

  BT = *temp;
  if (BT > 2)
    return NULL;
  temp = temp + 1;
  *lendata = *lendata - 1;
  switch (BT)
    {
    case 0:
    case 1:
      while ((*temp != 0) && (*lendata > 0))
	{
	  if (*temp != 0xff)
	    {
	      err = 1;
	      break;
	    }
	  temp = temp + 1;
	  *lendata = *lendata - 1;
	  nbrerempl = nbrerempl + 1;
	}
      break;
    case 2:
      while ((*temp != 0) && (*lendata > 0))
	{
	  temp = temp + 1;
	  *lendata = *lendata - 1;
	  nbrerempl = nbrerempl + 1;
	}
      break;
    default:
      return NULL;
    }
  if ((nbrerempl < 8) || (err == 1) || (*lendata == 0))
    return NULL;
  else
    {
      temp = temp + 1;
      *lendata = *lendata - 1;
    }
  
  return temp;
}



/* Fin de librsa++.cpp */
