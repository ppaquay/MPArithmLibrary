/***********************************************************************
 *                                                                     *
 *            Code source pour la librairie ElGamal en C++             *
 *                                                                     *
 *                                                                     *
 *                         Pierre Paquay                               *
 *                                                                     *
 ***********************************************************************/

#include "libeg++.hpp"


static USHORT wiener_map(USHORT);
static UCHAR* pkcs1(const UCHAR*, const int, const UCHAR, const UCHAR*, const int);
static UCHAR* pkcs1_inv(const UCHAR*, int*);

/* Constructeurs et destructeur de la classe EGPriv */
/*****************************************************/

// Constructeurs

EGPriv::EGPriv(const int len)
{
  int status = 0;

  seed64(time(NULL));
  seedBBS(time(NULL));
  while (status != 1)
    status = makekey(len);
}

EGPriv::EGPriv(const int len, const largeint& seed)
{
  int status = 0;

  seed64(seed);
  seedBBS(seed);
  while (status != 1)
    status = makekey(len);
}

// Destructeur

EGPriv::~EGPriv()
{
  keypriv.p.purge();
  keypriv.g.purge();
  keypriv.A.purge();
  keypriv.a.purge();
  keypriv.bitlen = 0;
}

/* Fonctions membres de la classe EGPriv */
/******************************************/

// Fonction de décryptage

UCHAR* EGPriv::decrypt(const EGMess& C, int* lenmess)
{
  UCHAR* M;

  M = largeint2byte(fdecrypt(C), lenmess);

  return (pkcs1_inv(M, lenmess));
}

// Fonction de signature

EGSign EGPriv::sign(const UCHAR* const M, const int lenmess)
{
  largeint k, temp1, temp2;
  EGSign S;
  largeint one = largeint((USHORT)1);
  int lenblock;
  USHORT bytlen;

  bytlen = (keypriv.bitlen >> 3);
  if ((keypriv.bitlen % 8) > 0)
    bytlen = bytlen + 1;
  lenblock = bytlen - 1;
  UCHAR* block = new UCHAR[lenblock];
  if (pkcs1(block, lenblock, 01, ripemd160((UCHAR*)M, (ULONG)lenmess), 20) == NULL)
    {
      delete [] block;
      S.r = largeint((USHORT)0);
      S.s = largeint((USHORT)0);
      return S;
    }
  largeint m(block, lenblock);
  delete [] block;
  do
    k = randBBS(one, keypriv.p - 2);
  while (gcd(k, keypriv.p - 1) != one);
  S.r = expmod(keypriv.g, k, keypriv.p);
  temp1 = mulmod(keypriv.a, S.r, keypriv.p - 1);
  temp1 = submod(m, temp1, keypriv.p - 1);
  temp2 = invmod(k, keypriv.p - 1);
  S.s = mulmod(temp2, temp1, keypriv.p - 1);
  
  return S;
}

// Fonction d'export de la clé publique

EGKeyPub EGPriv::exportkeypub(void)
{
  EGKeyPub keypubtemp;

  keypubtemp.p = keypriv.p;
  keypubtemp.g = keypriv.g;
  keypubtemp.A = keypriv.A;
  keypubtemp.bitlen = keypriv.bitlen;

  return keypubtemp;
}

/* Fonctions amies de la classe EGPriv */
/******************************************/

// Opérateurs de comparaison

int operator==(const EGPriv& key1, const EGPriv& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.keypriv.p == key2.keypriv.p) && (key1.keypriv.g == key2.keypriv.g)
      && (key1.keypriv.a == key2.keypriv.a) && (key1.keypriv.A == key2.keypriv.A)
      && (key1.keypriv.bitlen == key2.keypriv.bitlen))
    return 1;
  else
    return 0;
}

int operator!=(const EGPriv& key1, const EGPriv& key2)
{
  if (&key1 == &key2)
    return 0;
  if ((key1.keypriv.p != key2.keypriv.p) || (key1.keypriv.g != key2.keypriv.g)
      || (key1.keypriv.a != key2.keypriv.a) || (key1.keypriv.A != key2.keypriv.A)
      || (key1.keypriv.bitlen != key2.keypriv.bitlen))
    return 1;
  else
    return 0;
}

// Opérateurs de flux

ofstream& operator<<(ofstream& s, const EGPriv& wkey)
{
  s << wkey.keypriv.p << wkey.keypriv.g
    << wkey.keypriv.a << wkey.keypriv.A;
  write_ushort(s, wkey.keypriv.bitlen);

  return s;
}

ifstream& operator>>(ifstream& s, EGPriv& rkey)
{
  s >> rkey.keypriv.p >> rkey.keypriv.g
    >> rkey.keypriv.a >> rkey.keypriv.A;
  read_ushort(s, &rkey.keypriv.bitlen);

  return s;
}

/* Opérateur d'affectation */
/***************************/

EGPriv& EGPriv::operator=(const EGPriv& akey)
{
  if (&akey != this)
    {
      keypriv.p = akey.keypriv.p;
      keypriv.g = akey.keypriv.g;
      keypriv.a = akey.keypriv.a;
      keypriv.A = akey.keypriv.A;
      keypriv.bitlen = akey.keypriv.bitlen;
    }
  return *this;
}
      

/* Fonctions auxiliaires de la classe EGPriv */
/**********************************************/

// Fonction de décryptage rapide

largeint EGPriv::fdecrypt(const EGMess& C)
{
  largeint temp, exp;
  largeint m;

  exp = keypriv.p - 1 - keypriv.a;
  temp = expmod(C.B0, exp, keypriv.p);
  m = mulmod(C.c, temp, keypriv.p);

  return m;
}

// Gestion des clés

int EGPriv::testkey(void)
{
  largeint m = randBBS(keypriv.bitlen >> 1);
  largeint one = largeint((USHORT)1);
  largeint b;
  largeint temp;
  EGMess C;
  
  b = randBBS(one, keypriv.p - 2);
  C.B0 = expmod(keypriv.g, b, keypriv.p);
  temp = expmod(keypriv.A, b, keypriv.p);
  C.c = mulmod(m, temp, keypriv.p);
  if (fdecrypt(C) == m)
    return 1;
  else
    return 0;
}

static USHORT wiener_map(USHORT n)
{
  static struct { USHORT p_n, q_n; } t[] =
    {	/*   p	  q	 attack cost */
      {  512, 119 },	/* 9 x 10^17 */
      {  768, 145 },	/* 6 x 10^21 */
      { 1024, 165 },	/* 7 x 10^24 */
      { 1280, 183 },	/* 3 x 10^27 */
      { 1536, 198 },	/* 7 x 10^29 */
      { 1792, 212 },	/* 9 x 10^31 */
      { 2048, 225 },	/* 8 x 10^33 */
      { 2304, 237 },	/* 5 x 10^35 */
      { 2560, 249 },	/* 3 x 10^37 */
      { 2816, 259 },	/* 1 x 10^39 */
      { 3072, 269 },	/* 3 x 10^40 */
      { 3328, 279 },	/* 8 x 10^41 */
      { 3584, 288 },	/* 2 x 10^43 */
      { 3840, 296 },	/* 4 x 10^44 */
      { 4096, 305 },	/* 7 x 10^45 */
      { 4352, 313 },	/* 1 x 10^47 */
      { 4608, 320 },	/* 2 x 10^48 */
      { 4864, 328 },	/* 2 x 10^49 */
      { 5120, 335 },	/* 3 x 10^50 */
      { 0, 0 }
    };
  int i;

  for(i = 0; t[i].p_n; i++)
    {
      if (n <= t[i].p_n)
	return t[i].q_n;
    }
  /* not in table - use some arbitrary high number ;-) */
  return  (n / 8 + 200);
}

int EGPriv::makekey(const int len)
{
  largeint min = largeint((USHORT)1);
  largeint max;
  USHORT qbits;

  qbits = wiener_map((USHORT)len);
  keypriv.p = elgamalprime((USHORT)len, qbits, keypriv.g);
  max = keypriv.p - 2;
  keypriv.a = randBBS(min, max);
  keypriv.A = expmod(keypriv.g, keypriv.a, keypriv.p);
  keypriv.bitlen = numbits(keypriv.p);

  return testkey();
}

/* Constructeurs et destructeur de la classe EGPub */
/*****************************************************/

// Constructeurs

EGPub::EGPub(const EGPriv& key)
{
  keypub = ((EGPriv)key).exportkeypub();
}

// Destructeur

EGPub::~EGPub()
{
  keypub.p.purge();
  keypub.g.purge();
  keypub.A.purge();
  keypub.bitlen = 0;
}

/* Fonctions membres */
/*********************/

// Fonction de cryptage

EGMess EGPub::crypt(const UCHAR* const M, int lenmess)
{
  largeint b, temp;
  largeint one = largeint((USHORT)1);
  EGMess C;
  int lenblock;
  USHORT bytlen;

  bytlen = (keypub.bitlen >> 3);
  if ((keypub.bitlen % 8) > 0)
    bytlen = bytlen + 1;
  lenblock = bytlen - 1;
  UCHAR* block = new UCHAR[lenblock];
  if (pkcs1(block, lenblock, 02, M, lenmess) == NULL)
    {
      delete [] block;
      C.B0 = largeint((USHORT)0);
      C.c = largeint((USHORT)0);
      return C;
    }
  largeint m(block, lenblock);
  b = randBBS(one, keypub.p - 2);
  C.B0 = expmod(keypub.g, b, keypub.p);
  temp = expmod(keypub.A, b, keypub.p);
  C.c = mulmod(m, temp, keypub.p);

  return C;
}

// Fonction d'authentification de la signature

int EGPub::authen(const UCHAR* const M, const EGSign& S, const int lenmess)
{
  largeint member1, member2, temp1, temp2;
  largeint one = largeint((USHORT)1);
  USHORT bytlen;
  int lenblock, verif = 0;
 
  if ((S.r >= one) && (S.r <= keypub.p - 1))
    {
      temp1 = expmod(keypub.A, S.r, keypub.p);
      temp2 = expmod(S.r, S.s, keypub.p);
      member1 = mulmod(temp1, temp2, keypub.p);
      bytlen = (keypub.bitlen >> 3);
      if ((keypub.bitlen % 8) > 0)
	bytlen = bytlen + 1;
      lenblock = bytlen - 1;
      UCHAR* block = new UCHAR[lenblock];
      if (pkcs1(block, lenblock, 01, ripemd160((UCHAR*)M, (ULONG)lenmess), 20) == NULL)
	{
	  delete [] block;
	  return verif;
	}
      largeint m(block, lenblock);
      delete [] block;
      member2 = expmod(keypub.g, m, keypub.p);
      if (member1 == member2)
	verif = 1;
    }

  return verif;
}

/* Fonctions amies de la classe RSAPub */
/***************************************/

// Opérateurs de comparaison

int operator==(const EGPub& key1, const EGPub& key2)
{
  if (&key1 == &key2)
    return 1;
  if ((key1.keypub.p == key2.keypub.p) && (key1.keypub.g == key2.keypub.g)
      && (key1.keypub.A == key2.keypub.A) && (key1.keypub.bitlen == key2.keypub.bitlen))
    return 1;
  else
    return 0;
}

int operator!=(const EGPub& key1, const EGPub& key2)
{
  if (&key1 == &key2)
    return 0;
  if ((key1.keypub.p != key2.keypub.p) || (key1.keypub.g != key2.keypub.g)
      || (key1.keypub.A != key2.keypub.A) || (key1.keypub.bitlen != key2.keypub.bitlen))
    return 1;
  else
    return 0;
}

// Opérateur de flux

ofstream& operator<<(ofstream& s, const EGPub& wkey)
{
  s << wkey.keypub.p << wkey.keypub.g << wkey.keypub.A;
  write_ushort(s, wkey.keypub.bitlen);

  return s;
}

ifstream& operator>>(ifstream& s, EGPub& rkey)
{
  s >> rkey.keypub.p >> rkey.keypub.g >> rkey.keypub.A;
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



/* Fin de libeg++.cpp */
