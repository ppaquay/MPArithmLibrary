/***********************************************************************
 *                                                                     *
 *      Test global de la librairie de fonctions arithmétiques C++     *
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

static void report_error(largeint&, largeint&, largeint&, largeint&, int);
static void report_error(largeint&, largeint&, largeint&, int);
static void report_error(largeint&, largeint&, int);
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


static void report_error(largeint& a, largeint& b, largeint& c, largeint& d, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  cerr << "d = " << d << endl;
  abort();
}

static void report_error(largeint& a, largeint& b, largeint& c, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  abort();
}

static void report_error(largeint& a, largeint& b, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  abort();
}

static void report_error(largeint& a, USHORT b, largeint& c, USHORT d, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  cerr << "d = " << d << endl;
  abort();
}

static void report_error(largeint& a, USHORT b, largeint& c, int line)
{
  cerr << "Erreur dans la ligne " << line << endl;
  cerr << "a = " << a << endl;
  cerr << "b = " << b << endl;
  cerr << "c = " << c << endl;
  abort();
}

static void testadd(int nbretest)
{
  largeint a, b, c, d, e, m;
  USHORT ds, bs;
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
      ds = bs = usrand64_l();
      e = a + bs;
      if ((e != c.add(bs)) || (e != add(a, bs)))
	report_error(a, bs, c, ds, __LINE__);
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      e = a + bs;
      if ((e != c.add(bs)) || (e != add(a, bs)))
	report_error(a, bs, c, ds, __LINE__);
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
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
      while (m == largeint((USHORT)0));
      e = addmod(a, b, m);
      if ((e != c.addmod(b, m)) || (e != d.addmod(a, m))
	  || (e != addmod(a, b, m)) || (e != addmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND1);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
      e = (a + bs) % m;
      if ((e != c.addmod(bs, m)) || (e != addmod(a, bs, m)))
	report_error(a, bs, c, ds, __LINE__);
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      e = (a + bs) % m;
      if ((e != c.addmod(bs, m)) || (e != addmod(a, bs, m)))
	report_error(a, bs, c, ds, __LINE__);
    }
}

static void testsub(int nbretest)
{
  largeint a, b, c, d, e, m;
  USHORT bs, ds;
  int i;

  cout << "Fonction testsub()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      e = a - b;
      if ((e != largeint((USHORT)0) - (b - a)) || (e != c.sub(b)) 
	  || (e != sub(a, b)))
	{
	  cerr << "a - b = " << a - b;
	  cerr << "0 - (b - a) = " << largeint((USHORT)0) - (b - a);
	  cerr << "sub(a, b) = " << sub(a, b);
	  cerr << "a.sub(b) = " << a.sub(b);
	  report_error(a, b, c, d, __LINE__);
	}
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      e = a - b;
      if ((e != largeint((USHORT)0) - (b - a)) || (e != c.sub(b)) 
	  || (e != sub(a, b)))
	{
	  cerr << "a - b = " << a - b;
	  cerr << "0 - (b - a) = " << largeint((USHORT)0) - (b - a);
	  cerr << "sub(a, b) = " << sub(a, b);
	  cerr << "a.sub(b) = " << a.sub(b);
	  report_error(a, b, c, d, __LINE__);
	}
      c = a = rand64(RND1);
      ds = bs = usrand64_l();
      e = a - bs;
      if ((e != c.sub(bs)) || (e != sub(a, bs)))
	{
	  cerr << "a - b = " << a - bs;
	  cerr << "sub(a, bs) = " << sub(a, bs);
	  cerr << "a.sub(bs) = " << a.sub(bs);
	  report_error(a, bs, c, ds, __LINE__);
	}
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      e = a - bs;
      if ((e != c.sub(bs)) || (e != sub(a, bs)))
	{
	  cerr << "a - b = " << a - bs;
	  cerr << "sub(a, bs) = " << sub(a, bs);
	  cerr << "a.sub(bs) = " << a.sub(bs);
	  report_error(a, bs, c, ds, __LINE__);
	}
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
      if (c.submod(b, m) != submod(a, b, m))
	report_error(a, b, c, __LINE__);
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      if (c.submod(b, m) != submod(a, b, m))
	report_error(a, b, c, __LINE__);
      c = a = rand64(RND1);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
      if (c.submod(bs, m) != submod(a, bs, m))
	report_error(a, bs, c, __LINE__);
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      if (c.submod(bs, m) != submod(a, bs, m))
	report_error(a, bs, c, __LINE__);
    }
}

static void testmul(int nbretest)
{
  largeint a, b, c, d, e, m;
  USHORT bs, ds;
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
      ds = bs = usrand64_l();
      e = a * bs;
      if ((e != c.mul(bs)) || (e != mul(a, bs)))
	report_error(a, bs, c, ds, __LINE__);
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      e = a * bs;
      if ((e != c.mul(bs)) || (e != mul(a, bs)))
	report_error(a, bs, c, ds, __LINE__);
      c = a = rand64(RND1);
      d = b = rand64(RND1);
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
      e = (a * b) % m;
      if ((e != (b * a) % m) || (e != c.mulmod(b, m)) || (e != d.mulmod(a, m))
	  || (e != mulmod(a, b, m)) || (e != mulmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND1);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND1);
	}
      while (m == largeint((USHORT)0));
      e = (a * bs) % m;
      if ((e != c.mulmod(bs, m)) || (e != mulmod(a, bs, m)))
	report_error(a, bs, c, ds, __LINE__);  
      c = a = rand64(RND2);
      d = b = rand64(RND2);
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      e = mulmod(a, b, m);
      if ((e != c.mulmod(b, m)) || (e != d.mulmod(a, m))
	  || (e != mulmod(a, b, m)) || (e != mulmod(b, a, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      ds = bs = usrand64_l();
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      e = mulmod(a, bs, m);
      if ((e != c.mulmod(bs, m)) || (e != mulmod(a, bs, m)))
	report_error(a, bs, c, ds, __LINE__);  
    }
}

static void testsqr(int nbretest)
{
  largeint a, b, m;
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
      while (m == largeint((USHORT)0));
      b = sqrmod(b, m);
      if ((b != a.sqrmod(m)))
	report_error(a, b, __LINE__);
      a = rand64(RND2);
      b = a;
      do
	{
	  m = rand64(RND2);
	}
      while (m == largeint((USHORT)0));
      b = sqrmod(b, m);
      if ((b != a.sqrmod(m)))
	report_error(a, b, __LINE__);
    }
}

static void testdiv(int nbretest)
{
  largeint a, b, c, d, e, r, r1, r2;
  USHORT bs, ds;
  int i;

  cout << "Fonction testdiv()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      c = a = rand64(RND2);
      do
	d = b = rand64(RND2);
      while (b == largeint((USHORT)0));
      e = a / b;
      if ((e != div(a, b, r)) || (e != c.div(b, r)))
	report_error(a, b, c, __LINE__);
      c = a = rand64(RND2);
      do
	d = b = rand64(RND2);
      while (b == largeint((USHORT)0));
      div(a, b, r1);
      a.div(b, r2);
      e = c % d;
      if ((e != r1) || (e != r2))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      do
	ds = bs = usrand64_l();
      while (bs == 0);
      e = a / bs;
      if ((e != div(a, bs, r)) || (e != c.div(bs, r)))
	report_error(a, bs, c, __LINE__);
      c = a = rand64(RND2);
      do
	d = b = usrand64_l();
      while (bs == 0);
      div(a, bs, r1);
      a.div(bs, r2);
      e = c % ds;
      if ((e != r1) || (e != r2))
	report_error(a, bs, c, ds, __LINE__);
    }
}

static void testeqmod(int nbretest)
{
  largeint a, b, k, m;
  int i;

  cout << "Fonction testeqmod()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND2);
      b = rand64(RND2);
      do
	m = rand64(RND2);
      while (m == largeint((USHORT)0));
      if ((eqmod(a, b, m) && !(a % m == b % m)) || (!eqmod(a, b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
      b = rand64(RND2);
      do
	{
	  k = rand64(RND1);
	  m = rand64(RND1);
	}
      while ((m == largeint((USHORT)0)) || (k == largeint((USHORT)0)));
      a = k * m + b;
      if ((eqmod(a, b, m) && !(a % m == b % m)) || (!eqmod(a, b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
      if ((a.eqmod(b, m) && !(a % m == b % m)) || (!a.eqmod(b, m) && (a % m == b % m)))
	report_error(a, b, m, __LINE__);
    }
}

static void testexpmod(int nbretest)
{
  largeint a, b, c, d, e, f, g, m;
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
      while (m == largeint((USHORT)0));
      d = expmod(a, b, m);
      if ((d != a.expmod(b, m)) || (d != c.expmod(b, m))
	  || (d != expmod(f, b, m)) || (d != g.expmod(e, m)))
	report_error(a, b, c, d, __LINE__);
      c = a = rand64(RND2);
      e = u = usrand64_l();
      d = expmod(u, a, m);
      if ((d != expmod(e, a, m)) || (d != e.expmod(c, m)))
	report_error(a, e, m, d, __LINE__);
      c = u = usrand64_l();
      e = b = rand64(RND2);
      d = expmod(u, e, m);
      if (d != expmod(c, b, m))
	report_error(a, e, m, d, __LINE__);
    }
}

static void testset(int nbretest)
{
  largeint s, t;
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
  largeint s((USHORT)0), t;
  unsigned long pos, shift;
  int i;

  cout << "Fonction testshift()..." << endl;

  s.setbit(BIGINTMAXBIT - 1);
  t = s;
  t = (t >> (BIGINTMAXBIT - 1));
  if ((t != largeint((USHORT)1)) || (t != (s >> (BIGINTMAXBIT - 1))))
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
      s = largeint((USHORT)0);
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
  largeint a, b, c, m;
  largeint Nul = largeint((USHORT)0), Un = largeint((USHORT)1);
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
      while (m == largeint((USHORT)0));
      if ((((a + Nul) % m) != a % m) || ((a * Un) % m != a % m)
	  || (addmod(a, Nul, m) != a % m) || (mulmod(a, Un, m) != a % m)
	  || (a.addmod(Nul, m) != a % m) || (a.mulmod(Un, m) != a % m))
	report_error(a, b, m, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == largeint((USHORT)0));
      if (((a + b) % m != (b + a) % m) || ((a * b) % m != (b * a) % m)
	  || (addmod(a, b, m) != addmod(b, a, m)) || (mulmod(a, b, m) != mulmod(b, a, m)))
	report_error(a, b, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      c = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == largeint((USHORT)0));
      if ((((a + b) + c) % m != (a + (b + c)) % m) || (((a * b) * c) % m != (a * (b * c)) % m)
	  || (addmod(addmod(a, b, m), c, m) != addmod(a, addmod(b, c, m), m))
	  || (mulmod(mulmod(a, b, m), c, m) != mulmod(a, mulmod(b, c, m), m)))
	report_error(a, b, c, __LINE__);
    }
}

static void testcancel(int nbretest)
{
  largeint a, b, c, d, e, m;
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
      while (b == largeint((USHORT)0));
      d = a / b;
      e = a % b;
      if (((b * d + e) != a) || (add(mul(b, d), e) != a))
	report_error(a, b, d, e, __LINE__);
      a = rand64(RND1);
      b = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == largeint((USHORT)0));
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
  largeint a, b, c, m;
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
      while (m == largeint((USHORT)0));
      if ((((a + b) * c) % m != (a * c + b * c) % m) || (mulmod(addmod(a, b, m), c, m) != addmod(mulmod(a, c, m), mulmod(b, c, m), m)))
	report_error(a, b, c, __LINE__);
    }
}

static void testexp(int nbretest)
{
  largeint a, r, s, e, m;
  int i;

  cout << "Fonction testexp()..." << endl;

  for (i = 0; i < nbretest; i++)
    {
      a = rand64(RND1);
      r = rand64(RND1);
      s = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == largeint((USHORT)0));
      e = r * s;
      if (expmod(a, e, m) != expmod(expmod(a, r, m), s, m))
	report_error(a, r, s, m, __LINE__);
      a = rand64(RND1);
      r = rand64(RND1);
      s = rand64(RND1);
      do
	m = rand64(RND1);
      while (m == largeint((USHORT)0));
      e = r + s;
      if (expmod(a, e, m) != mulmod(expmod(a, r, m), expmod(a, s, m), m))
	report_error(a, r, s, m, __LINE__);
    }
}
      


int main()
{
  cout << "Module de test pour la librairie arithmétique C++, " << __FILE__ << endl;

  seed64(time(NULL));

#ifndef SECURE
  testadd(1000);
  testsub(1000);
  testmul(1000);
  testsqr(1000);
  testdiv(1000);
  testeqmod(1000);
  testexpmod(50);
  testset(1000);
  testshift(1000);
  testprop(1000);
  testcancel(1000);
  testdist(1000);
  testexp(100);
#else
  testadd(100);
  testsub(100);
  testmul(100);
  testsqr(100);
  testdiv(100);
  testeqmod(100);
  testexpmod(5);
  testset(100);
  testshift(100);
  testprop(100);
  testcancel(100);
  testdist(100);
  testexp(10);
#endif

  cout << "Tous les tests du fichier " << __FILE__ << " effectués." << endl;

  return 0;
}
