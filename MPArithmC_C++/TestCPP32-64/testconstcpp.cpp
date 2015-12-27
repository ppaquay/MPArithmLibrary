/***********************************************************************
 *                                                                     *
 *         Test global de la librairie des constructeurs  C++          *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "libmparithm++.hpp"
#include <stdio.h>

#define MAXTESTLEN BIGINTMAXBIT
#define RND2 (ulrand64_l() % (MAXTESTLEN + 1))
#define RND1 (ulrand64_l() % (MAXTESTLEN/2 + 1))


static void checkconst(largeint&, largeint&, int);
static void checkconst(BIGINT, largeint&, int);
static void checkconst(USHORT, largeint&, int);
static void checkconst(ULONG, largeint&, int);
#ifndef ARCH64
static void checkconst(unsigned int, largeint&, int);
#endif
static void testconst2(int);
static void testconst3(int);
static void testconst4(int);
static void testconst5(int);
static void testconst6(int);
static void testconst7(int);
#ifndef ARCH64
static void testconst8(int);
#endif

static void checkconst(largeint& a, largeint& b, int line)
{
  if (!(b >= largeint((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != largeint_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  if (a != b)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << a << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}

static void checkconst(BIGINT a_l, largeint& b, int line)
{
  if (!(b >= largeint((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != largeint_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  UCHAR *N1, *N2;
  int len1, len2;

  N1 = bigint2byte_l(a_l, &len1);
  N2 = largeint2byte(b, &len2);
  if (len1 != len2)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << bigint2str_l(a_l, 16, 1) << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
  if (strncmp((const char*)N1, (const char*)N2, len1) != 0)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << bigint2str_l(a_l, 16, 1) << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}

static void checkconst(USHORT a, largeint& b, int line)
{
  if (!(b >= largeint((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != largeint_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  if (largeint(a) != b)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << a << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}

static void checkconst(ULONG a, largeint& b, int line)
{
  if (!(b >= largeint((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != largeint_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  if (largeint(a) != b)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << a << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}

#ifndef ARCH64
static void checkconst(unsigned int a, largeint& b, int line)
{
  if (!(b >= largeint((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != largeint_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  if (largeint(a) != b)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << a << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}
#endif

static void testconst2(int nbretest)
{
  BIGINT nul_l = { 0 };
  int i;

  cout << "Fonction testconst2()..." << endl;

  largeint nul((USHORT)0);
  largeint val(nul);
  checkconst(nul_l, val, __LINE__);
  checkconst(nul, val, __LINE__);

  BIGINT max_l;
  setmax_l(max_l);
  largeint max(max_l);
  val = largeint(max);
  checkconst(max, val, __LINE__);
  checkconst(max_l, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      largeint test = rand64(RND2);
      val = largeint(test);
      checkconst(test, val, __LINE__);
    }
}

static void testconst3(int nbretest)
{
  char* ofl;
  int i;

  cout << "Fonction testconst3()..." << endl;

  ofl = new char[(sizeof(char) * (BIGINTMAXDIGIT * 4 + 1))];
  if (ofl == NULL)
    {
      cerr << "Erreur dans l'opérateur new dans testconst3(), ligne " << __LINE__ << endl;
      abort();
    }
  for (i = 0; (unsigned)i < BIGINTMAXDIGIT * 4; i++)
    ofl[i] = 'f';
  ofl[BIGINTMAXDIGIT * 4] = '\0';
  largeint val(ofl, 16);
  if (val.get_status() == largeint_OFL)
    {
      cerr << "Erreur dans constructeur 3: OFL détecté." << endl;
      cerr << "val.get_status() = " << (int)val.get_status() << endl;
      abort();
    }

  ofl[0] = '0';
  ofl[1] = '\0';
  val = largeint(ofl, 16);
  BIGINT nul_l = { 0 };
  checkconst(nul_l, val, __LINE__);
  delete [] ofl;

  largeint test;
  for (i = 1; i <= nbretest; i++)
    {
      test = rand64(RND2);
      val = largeint(largeint2str(test, 16, 1), 16);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = largeint(largeint2str(test, 2, 1), 2);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = largeint(largeint2str(test, 8, 1), 8);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = largeint(largeint2str(test, 10, 1), 10);
      checkconst(test, val, __LINE__);
    }
}

void testconst4(int nbretest)
{
  UCHAR* ofl;
  int i, len;

  cout << "Fonction testconst4()..." << endl;
  
  ofl = new UCHAR[sizeof(UCHAR) * BIGINTMAXBYTE];
  if (ofl == NULL)
    {
      cerr << "Erreur dans l'opérateur new dans testconst4(), ligne " << __LINE__ << endl;
      abort();
    }
  for (i = 0; (unsigned)i < BIGINTMAXBYTE; i++)
    ofl[i] = 0xff;
  largeint val(ofl, BIGINTMAXBYTE);
  if (val.get_status() == largeint_OFL)
    {
      cerr << "Erreur dans constructeur 4: OFL détecté." << endl;
      cerr << "val.get_status() = " << (int)val.get_status() << endl;
      abort();
    }  
  ofl[0] = 0x00;
  val = largeint(ofl, 1);
  BIGINT nul_l = { 0 };
  checkconst(nul_l, val, __LINE__);
  val = largeint(ofl, 0);
  checkconst(nul_l, val, __LINE__);
  delete [] ofl;

  largeint test;
  for (i = 1; i <= nbretest; i++)
    {
      test = rand64(RND2);
      ofl = largeint2byte(test, &len);
      val = largeint(ofl, len);
      checkconst(test, val, __LINE__);
    }
}

static void testconst5(int nbretest)
{
  USHORT u;
  largeint val;
  int i;

  cout << "Fonction testconst5()..." << endl;
  
  u = 0;
  val = largeint(u);
  checkconst(u, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      u = usrand64_l();
      val = largeint(u);
      checkconst(u, val, __LINE__);
    }
}

static void testconst6(int nbretest)
{
  ULONG ul;
  largeint val;
  int i;

  cout << "Fonction testconst6()..." << endl;
  
  ul = 0;
  val = largeint(ul);
  checkconst(ul, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      ul = ulrand64_l();
      val = largeint(ul);
      checkconst(ul, val, __LINE__);
    }
}

static void testconst7(int nbretest)
{
  BIGINT t_l, nul_l = { 0 };
  int i;

  cout << "Fonction testconst7()..." << endl;

  largeint val(nul_l);
  checkconst(nul_l, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      rand_l(t_l, RND2);
      val = largeint(t_l);
      checkconst(t_l, val, __LINE__);
    }
}

#ifndef ARCH64
static void testconst8(int nbretest)
{
  unsigned int i;
  ULONG ul;
  largeint val;
  int j;

  cout << "Fonction testconst8()..." << endl;
  
  i = 0;
  val = largeint(i);
  checkconst(i, val, __LINE__);

  for (j = 0; j < nbretest; j++)
    {
      ul = ulrand64_l();
      i = ul;
      val = largeint(i);
      checkconst(i, val, __LINE__);
    }
}
#endif


int main(void)
{
  cout << "Module de test pour les constructeurs C++, " << __FILE__ << endl;
  
  seed64(time(NULL));

#ifndef SECURE
  testconst2(1000);
  testconst3(1000);
  testconst4(1000);
  testconst5(1000);
  testconst6(1000);
  testconst7(1000);
#ifndef ARCH64
  testconst8(1000);
#endif
#else
  testconst2(100);
  testconst3(100);
  testconst4(100);
  testconst5(100);
  testconst6(100);
  testconst7(100);
#ifndef ARCH64
  testconst8(100);
#endif
#endif

  cout << "Tous les tests du fichier " << __FILE__ << " effectués." << endl;
  
  return 0;
}
