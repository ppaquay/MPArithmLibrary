/***********************************************************************
 *                                                                     *
 *      Test global de la librairie de fonctions arithmétiques C++     *
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

static void report_error(BINT&, BINT&, BINT&, BINT&, int);
static void report_error(BINT&, BINT&, BINT&, int);
static void report_error(BINT&, BINT&, int);
static void testadd(int);
static void testsub(int);
static void testmul(int);
static void testsqr(int);
static void testdiv(int);
static void testeqmod(int);
static void testexpmod(int);
static void testset(int);
static void testshift(int);
static void testprop(int);
static void testcancel(int);
static void testdist(int);
static void testexp(int);


static void report_error(BINT& a, BINT& b, BINT& c, BINT& d, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  cerr << "d = " << d << endl;
  abort();
}

static void report_error(BINT& a, BINT& b, BINT& c, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  abort();
}

static void report_error(BINT& a, BINT& b, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  abort();
}

static void testadd(int nbretest)
{
  BINT a, b, c, d, e, m;
  int i;

  cout << "Fonction testadd()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      e = a + b;
      if ((e != (b + a)) || (e != c.add(b)) || (e != d.add(a))
	  || (e != add(a, b)) || (e != add(b, a)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      e = a + b;
      if ((e != (b + a)) || (e != c.add(b)) || (e != d.add(a))
	  || (e != add(a, b)) || (e != add(b, a)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == BINT((USHORT)0));
      e = (a + b) % m;
      if ((e != (b + a) % m) || (e != c.addmod(b, m)) || (e != d.addmod(a, m))
	  || (e != addmod(a, b, m)) || (e != addmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      do
	{
	  m = rand64(RND2);
	}
      while (m == BINT((USHORT)0));
      e = addmod(a, b, m);
      if ((e != c.addmod(b, m)) || (e != d.addmod(a, m))
	  || (e != addmod(a, b, m)) || (e != addmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);
    }
}

static void testsub(int nbretest)
{
  BINT a, b, c, d, e, m;
  int i;

  cout << "Fonction testsub()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      e = a - b;
      if ((e != BINT((USHORT)0) - (b - a)) || (e != c.sub(b)) 
	  || (e != sub(a, b)))
	{
	  cerr << "a - b = " << a - b;
	  cerr << "0 - (b - a) = " << BINT((USHORT)0) - (b - a);
	  cerr << "sub(a, b) = " << sub(a, b);
	  cerr << "a.sub(b) = " << a.sub(b);
	  report_error(a, b, c, d, __LINE__);
	}
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      e = a - b;
      if ((e != BINT((USHORT)0) - (b - a)) || (e != c.sub(b)) 
	  || (e != sub(a, b)))
	{
	  cerr << "a - b = " << a - b;
	  cerr << "0 - (b - a) = " << BINT((USHORT)0) - (b - a);
	  cerr << "sub(a, b) = " << sub(a, b);
	  cerr << "a.sub(b) = " << a.sub(b);
	  report_error(a, b, c, d, __LINE__);
	}
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == BINT((USHORT)0));
      if (c.submod(b, m) != submod(a, b, m))
	report_error(a, b, c, __LINE__);
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      do
	{
	  m = rand64(RND2);
	}
      while (m == BINT((USHORT)0));
      if (c.submod(b, m) != submod(a, b, m))
	report_error(a, b, c, __LINE__);
    }
}

static void testmul(int nbretest)
{
  BINT a, b, c, d, e, m;
  int i;

  cout << "Fonction testmul()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      e = a * b;
      if ((e != (b * a)) || (e != c.mul(b)) || (e != d.mul(a))
	  || (e != mul(a, b)) || (e != mul(b, a)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      e = a * b;
      if ((e != (b * a)) || (e != c.mul(b)) || (e != d.mul(a))
	  || (e != mul(a, b)) || (e != mul(b, a)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == BINT((USHORT)0));
      e = (a * b) % m;
      if ((e != (b * a) % m) || (e != c.mulmod(b, m)) || (e != d.mulmod(a, m))
	  || (e != mulmod(a, b, m)) || (e != mulmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);  
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      do
	{
	  m = rand64(RND2);
	}
      while (m == BINT((USHORT)0));
      e = mulmod(a, b, m);
      if ((e != c.mulmod(b, m)) || (e != d.mulmod(a, m))
	  || (e != mulmod(a, b, m)) || (e != mulmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);  
    }
}

static void testsqr(int nbretest)
{
  BINT a, b, m;
  int i;

  cout << "Fonction testsqr()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND1);
      b = a;
      if (((b * b) != a.sqr()))
	report_error(a, b, __LINE__);
      a = rand64(RND2);
      b = a;
      if (((b * b) != a.sqr()))
	report_error(a, b, __LINE__);
      a = rand64(RND1);
      b = a;
      do
	{
	  m = rand64(RND1);
	}
      while (m == BINT((USHORT)0));
      b = sqrmod(b, m);
      if ((b != a.sqrmod(m)))
	report_error(a, b, __LINE__);
      a = rand64(RND2);
      b = a;
      do
	{
	  m = rand64(RND2);
	}
      while (m == BINT((USHORT)0));
      b = sqrmod(b, m);
      if ((b != a.sqrmod(m)))
	report_error(a, b, __LINE__);
    }
}

static void testdiv(int nbretest)
{
  BINT a, b, c, d, e, r, r1, r2;
  int i;

  cout << "Fonction testdiv()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND2);
      do
	d = b = rand64(RND2);
      while (b == BINT((USHORT)0));
      e = a / b;
      if ((e != div(a, b, r)) || (e != c.div(b, r)))
	report_error(a, b, c, __LINE__);
      c = a = rand64(RND2);
      do
	d = b = rand64(RND2);
      while (b == BINT((USHORT)0));
      div(a, b, r1);
      a.div(b, r2);
      e = c % d;
      if ((e != r1) || (e != r2))
	report_error(a, b, c, d, __LINE__);
    }
}

static void testeqmod(int nbretest)
{
  BINT a, b, k, m;
  int i;

  cout << "Fonction testeqmod()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND2);
      b = rand64(RND2);
      do
	m = rand64(RND2);
      while (m == BINT((USHORT)0));
      if ((eqmod(a, b, m) && !(a % m == b % m)) || (!eqmod(a, b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
      b = rand64(RND1);
      do
	{
	  k = rand64(RND1);
	  m = rand64(RND1);
	}
      while ((m == BINT((USHORT)0)) || (k == BINT((USHORT)0)));
      a = k * m + b;
      if ((eqmod(a, b, m) && !(a % m == b % m)) || (!eqmod(a, b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
      if ((a.eqmod(b, m) && !(a % m == b % m)) || (!a.eqmod(b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
    }
}

static void testexpmod(int nbretest)
{
  BINT a, b, c, d, e, f, g, m;
  USHORT u;
  int i;

  cout << "Fonction testexpmod()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND2);
      f = a;
      g = a;
      e = b = rand64(RND2);
      do
	m = rand64(RND2);
      while (m == BINT((USHORT)0));
      d = expmod(a, b, m);
      if ((d != a.expmod(b, m)) || (d != c.expmod(b, m))
	  || (d != expmod(f, b, m)) || (d != g.expmod(e, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND1);
      e = u = usrand64_l();
      if (expmod(u, a, m) != expmod(e, a, m))
	report_error(a, e, m, d, __LINE__);
    }
}

static void testset(int nbretest)
{
  BINT s, t;
  unsigned long pos;
  int i;

  cout << "Fonction testset()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      s = rand64(RND2);
      pos = ulrand64_l() % BIGINTMAXBIT;
      t = s;
      s.setbit(pos);
      if (!s.testbit(pos))
	{
	  cerr << "Erreur dans setbit, testbit: " << s << "Pos = " << pos << endl;
	  exit(1);
	}
      if (t.testbit(pos))
	t.clearbit(pos);
      if (s == t)
	{
	  cerr << "Erreur dans setbit, clearbit: " << s << "Pos = " << pos << endl;
	  exit(1);
	}
    }
}

static void testshift(int nbretest)
{
  BINT s((USHORT)0), t;
  unsigned long pos, shift;
  int i;

  cout << "Fonction testshift()..." << endl;

  s.setbit(BIGINTMAXBIT - 1);
  t = s;
  t = (t >> (BIGINTMAXBIT - 1));
  if ((t != BINT((USHORT)1)) || (t != (s >> (BIGINTMAXBIT - 1))))
    {
      cerr << "Erreur dans shift: t != 1..., t = " << t << endl;
      exit(1);
    }
  if (((t << (BIGINTMAXBIT - 1)) != s) || ((t = (t << (BIGINTMAXBIT - 1))) != s))
    {
      cerr << "Erreur dans shift: t != s..., t = " << t << endl;
      exit(1);
    }

  for (i = 0; i < nbretest; i++)
    {
      s = BINT((USHORT)0);
      pos = ulrand64_l() % BIGINTMAXBIT;
      shift = ulrand64_l() % (BIGINTMAXBIT - pos);
      s.setbit(pos);
      t = (s << shift);
      s = (s << shift);
      if ((!s.testbit(pos + shift)) || (s != t))
	{
	  cerr << "Erreur dans shift, s = " << s << "Pos = " << pos << endl;
	  cerr << "pos + shift = " << pos + shift << endl;
	  exit(1);
	}
      t = (s >> shift);
      s = (s >> shift);
      if ((!s.testbit(pos)) || (s != t))
	{
	  cerr << "Erreur dans shift, s = " << s << "Pos = " << pos << endl;
	  cerr << "pos + shift = " << pos + shift << endl;
	  exit(1);
	}
    }
}

static void testprop(int nbretest)
{
  BINT a, b, c, m;
  BINT Nul = BINT((USHORT)0), Un = BINT((USHORT)1);
  int i;

  cout << "Fonction testprop()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND2);
      b = rand64(RND2);
      if ((a + Nul != a) || (a * Un != a)
	  || (add(a, Nul) != a) || (mul(a, Un) != a)
	  || (a.add(Nul) != a) || (a.mul(Un) != a))
	report_error(a, b, __LINE__);
      a = rand64(RND2);
      b = rand64(RND2);
      if ((a + b != b + a) || (a * b != b * a)
	  || (add(a, b) != add(b, a)) || (mul(a, b) != mul(b, a)))
	report_error(a, b, __LINE__);
      a = rand64(RND2);
      b = rand64(RND2);
      c = rand64(RND2);
      if (((a + b) + c != a + (b + c)) || ((a * b) * c != a * (b * c))
	  || (add(add(a, b), c) != add(a, add(b, c))) || (mul(mul(a, b), c) != mul(a, mul(b, c))))
	report_error(a, b, c, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      if ((((a + Nul) % m) != a % m) || ((a * Un) % m != a % m)
	  || (addmod(a, Nul, m) != a % m) || (mulmod(a, Un, m) != a % m)
	  || (a.addmod(Nul, m) != a % m) || (a.mulmod(Un, m) != a % m))
	report_error(a, b, m, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      if (((a + b) % m != (b + a) % m) || ((a * b) % m != (b * a) % m)
	  || (addmod(a, b, m) != addmod(b, a, m)) || (mulmod(a, b, m) != mulmod(b, a, m)))
	report_error(a, b, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      c = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      if ((((a + b) + c) % m != (a + (b + c)) % m) || (((a * b) * c) % m != (a * (b * c)) % m)
	  || (addmod(addmod(a, b, m), c, m) != addmod(a, addmod(b, c, m), m))
	  || (mulmod(mulmod(a, b, m), c, m) != mulmod(a, mulmod(b, c, m), m)))
	report_error(a, b, c, __LINE__);
    }
}

static void testcancel(int nbretest)
{
  BINT a, b, c, d, e, m;
  int i;

  cout << "Fonction testcancel()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND1);
      b = rand64(RND1);
      c = a + b;
      d = c;
      e = c;
      if ((c - a != b) || (c - b != a)
	  || (sub(c, a) != b) || (sub(c, b) != a)
	  || (d.sub(a) != b) || (e.sub(b) != a))
	report_error(a, b, c, __LINE__);
      a = rand64(RND2);
      do
	b = rand64(RND2);
      while (b == BINT((USHORT)0));
      d = a / b;
      e = a % b;
      if (((b * d + e) != a) || (add(mul(b, d), e) != a))
	report_error(a, b, d, e, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      c = (a + b) % m;
      d = c;
      e = c;
      if ((submod(c, a, m) != b % m) || (submod(c, b, m) != a % m)
	  || (d.submod(a, m) != b % m) || (e.submod(b, m) != a % m))
	report_error(a, b, c, __LINE__);
    }
}
      
      

static void testdist(int nbretest)
{
  BINT a, b, c, m;
  int i;

  cout << "Fonction testdist()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND2);
      b = rand64(RND2);
      c = rand64(RND2);
      if (((a + b) * c != (a * c + b * c)) || (mul(add(a, b), c)) != add(mul(a, c), mul(b, c)))
	report_error(a, b, c, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      c = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      if ((((a + b) * c) % m != (a * c + b * c) % m) || (mulmod(addmod(a, b, m), c, m) != addmod(mulmod(a, c, m), mulmod(b, c, m), m)))
	report_error(a, b, c, __LINE__);
    }
}

static void testexp(int nbretest)
{
  BINT a, r, s, e, m;
  int i;

  cout << "Fonction testexp()..." << endl;

  for (i = 0; i < nbretest; i++)
    {

      //cout << "Encore " << 1000 - i << " tests a attendre..." << endl;

      a = rand64(RND1);
      r = rand64(RND1);
      s = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      e = r * s;
      if (expmod(a, e, m) != expmod(expmod(a, r, m), s, m))
	report_error(a, r, s, m, __LINE__);
      a = rand64(RND1);
      r = rand64(RND1);
      s = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == BINT((USHORT)0));
      e = r + s;
      if (expmod(a, e, m) != mulmod(expmod(a, r, m), expmod(a, s, m), m))
	report_error(a, r, s, m, __LINE__);
    }
}
      


int main()
{
  cout << "Module de test pour la librairie arithmetique C++, " << __FILE__ << endl;

  seed64(time(NULL));
  testadd(10000);
  testsub(10000);
  testmul(10000);
  testsqr(10000);
  testdiv(10000);
  testeqmod(10000);
  testexpmod(500);
  testset(10000);
  testshift(10000);
  testprop(10000);
  testcancel(10000);
  testdist(10000);
  testexp(500);

  cout << "Tous les tests du fichier " << __FILE__ << " effectues." << endl;

  return 0;
}
