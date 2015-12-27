/***********************************************************************
 *                                                                     *
 *         Test global de la librairie des constructeurs  C++          *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "arithmcpp.hpp"
#include <stdio.h>

#define MAXTESTLEN BIGINTMAXBIT
#define RND2 (ulrand64_l() % (MAXTESTLEN + 1))
#define RND1 (ulrand64_l() % (MAXTESTLEN/2 + 1))


static void checkconst(BINT&, BINT&, int);
static void checkconst(BIGINT, BINT&, int);
static void checkconst(ULONG, BINT&, int);
static void testconst2(int);
static void testconst3(int);
static void testconst4(int);
static void testconst5(int);
static void testconst6(int);

static void checkconst(BINT& a, BINT& b, int line)
{
  if (!(b >= BINT((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != BINT_OK)
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

static void checkconst(BIGINT a_l, BINT& b, int line)
{
  if (!(b >= BINT((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != BINT_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  UCHAR *N1, *N2;
  int len1, len2;

  N1 = bigint2byte_l(a_l, &len1);
  N2 = bint2byte(b, &len2);
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

static void checkconst(ULONG a, BINT& b, int line)
{
  if (!(b >= BINT((USHORT)0)))
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Objet non-initialisé" << endl;
    }
  if (b.get_status() != BINT_OK)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "Statut de l'objet pas OK" << endl;
    }
  if (BINT(a) != b)
    {
      cerr << "Erreur dans la ligne " << line << endl;
      cerr << "a = " << a << endl;
      cerr << "b = " << b << endl;
      exit(1);
    }
}

static void testconst2(int nbretest)
{
  BIGINT nul_l = { 0 };
  int i;

  cout << "Fonction testconst2()..." << endl;

  BINT nul((USHORT)0);
  BINT val(nul);
  checkconst(nul_l, val, __LINE__);

  BIGINT max_l;
  setmax_l(max_l);
  BINT max(max_l);
  val = BINT(max);
  checkconst(max, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      BINT test = rand64(RND2);
      val = BINT(test);
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
  BINT val(ofl, 16);
  if (val.get_status() == BINT_OFL)
    {
      cerr << "Erreur dans constructeur 3: OFL détecté." << endl;
      cerr << "val.get_status() = " << (int)val.get_status() << endl;
      abort();
    }

  ofl[0] = '0';
  ofl[1] = '\0';
  val = BINT(ofl, 16);
  BIGINT nul_l = { 0 };
  checkconst(nul_l, val, __LINE__);
  delete [] ofl;

  BINT test;
  for (i = 1; i <= nbretest; i++)
    {
      test = rand64(RND2);
      val = BINT(bint2str(test, 16, 1), 16);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = BINT(bint2str(test, 2, 1), 2);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = BINT(bint2str(test, 8, 1), 8);
      checkconst(test, val, __LINE__);
      test = rand64(RND2);
      val = BINT(bint2str(test, 10, 1), 10);
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
  BINT val(ofl, BIGINTMAXBYTE);
  if (val.get_status() == BINT_OFL)
    {
      cerr << "Erreur dans constructeur 4: OFL détecté." << endl;
      cerr << "val.get_status() = " << (int)val.get_status() << endl;
      abort();
    }  
  ofl[0] = 0x00;
  val = BINT(ofl, 1);
  BIGINT nul_l = { 0 };
  checkconst(nul_l, val, __LINE__);
  val = BINT(ofl, 0);
  checkconst(nul_l, val, __LINE__);
  delete [] ofl;

  BINT test;
  for (i = 1; i <= nbretest; i++)
    {
      test = rand64(RND2);
      ofl = bint2byte(test, &len);
      val = BINT(ofl, len);
      checkconst(test, val, __LINE__);
    }
}

static void testconst5(int nbretest)
{
  USHORT u;
  BINT val;
  int i;

  cout << "Fonction testconst5()..." << endl;
  
  u = 0;
  val = BINT(u);
  checkconst(u, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      u = usrand64_l();
      val = BINT(u);
      checkconst(u, val, __LINE__);
    }
}

static void testconst6(int nbretest)
{
  BIGINT t_l, nul_l = { 0 };
  int i;

  cout << "Fonction testconst6()..." << endl;

  BINT val(nul_l);
  checkconst(nul_l, val, __LINE__);

  for (i = 0; i < nbretest; i++)
    {
      rand_l(t_l, RND2);
      val = BINT(t_l);
      checkconst(t_l, val, __LINE__);
    }
}


int main(void)
{
  cout << "Module de test pour les constructeurs C++, " << __FILE__ << endl;
  
  seed64(time(NULL));
  testconst2(10);
  testconst3(10);
  testconst4(10);
  testconst5(10);
  testconst6(10);

  cout << "Tous les tests du fichier " << __FILE__ << " effectués." << endl;
  
  return 0;
}
