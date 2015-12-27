/***********************************************************************
 *                                                                     *
 *     Code source pour la librairie de fonctions arithm�tiques C++    *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#include "libmparithm++.hpp"


/* Fonction interne pour elgamalprime */
/**************************************/

static void m_out_of_n(UCHAR *array, int, int);

/* Constructeurs et destructeur */
/********************************/

// Constructeurs

largeint::largeint(void)
{
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 1", 0, __LINE__);
  init = 0;
  status = largeint_OK;
}

largeint::largeint(const largeint& n)
{
  if (n.init == 0)
    panic(largeint_VAL, "Constructeur 2", 1, __LINE__);
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 2", 0, __LINE__);
  copy_l(n_l, n.n_l);
  init = 1;
  status = n.status;
}

largeint::largeint(const char* const str, const int bas)
{
  int err;
  
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 3", 0, __LINE__);
  err = str2bigint_l(n_l, (char*)str, bas);
  switch (err)
    {
    case BIGINT_OK:
      init = 1;
      status = largeint_OK;
      break;
    case BIGINT_NPT:
      status = largeint_NPT;
      panic(largeint_NPT, "Constructeur 3", 1, __LINE__);
      break;
    case BIGINT_BOR:
      status = largeint_BOR;
      panic(largeint_BOR, "Constructeur 3", 2, __LINE__);
      break;
    case BIGINT_OFL:
      init = 1;
      status = largeint_OFL;
      panic(largeint_OFL, "Constructeur 3", 0, __LINE__);
      break;
    default:
      status = largeint_ERR;
      panic(largeint_ERR, "Constructeur 3", err, __LINE__);
    }
}

largeint::largeint(const UCHAR* const bytevec, const int len)
{
  int err;

  if (bytevec == NULL)
    panic(largeint_NEW, "Constructeur 4", 0, __LINE__);
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 4", 0, __LINE__);
  err = byte2bigint_l(n_l, (UCHAR*)bytevec, len);
  switch (err)
    {
    case BIGINT_OK:
      init = 1;
      status = largeint_OK;
      break;
    case BIGINT_NPT:
      status = largeint_NPT;
      panic(largeint_NPT, "Constructeur 4", 1, __LINE__);
      break;
    case BIGINT_OFL:
      init = 1;
      status = largeint_OFL;
      panic(largeint_OFL, "Constructeur 4", 0, __LINE__);
      break;
    default:
      status = largeint_ERR;
      panic(largeint_ERR, "Constructeur 4", err, __LINE__);
    }
}

largeint::largeint(const USHORT u)
{
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 5", 0, __LINE__);
  u2bigint_l(n_l, u);
  init = 1;
  status = largeint_OK;
}

largeint::largeint(const ULONG ul)
{
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 6", 0, __LINE__);
  ul2bigint_l(n_l, ul);
  init = 1;
  status = largeint_OK;
}

largeint::largeint(const BIGINT m_l)
{
  if (check_l((bigint*)m_l) < 0)
    panic(largeint_VAL, "Constructeur 7", 1, __LINE__);
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 7", 0, __LINE__);
  copy_l(n_l, (bigint*)m_l);
  init = 1;
  status = largeint_OK;
}

#ifndef ARCH64
largeint::largeint(const unsigned int i)
{
  ULONG ul;

  n_l = new BIGINT;
  if (n_l == NULL)
    panic(largeint_NEW, "Constructeur 6", 0, __LINE__);
  ul = i;
  ul2bigint_l(n_l, ul);
  init = 1;
  status = largeint_OK;
}
#endif

// Destructeur

largeint::~largeint()
{
  purge_l(n_l);
  delete[] n_l;
}

/* Op�rateur d'assignement */
/***************************/

const largeint& largeint::operator=(const largeint& b)
{
  if (b.init == 0)
    panic(largeint_VAL, "=", 1, __LINE__);
  if (this != &b)
    {
      copy_l(n_l, b.n_l);
      init = 1;
      status = b.status;
    }
  return *this;
}

/* Fonctions membres */
/*********************/

// Fonctions membres d'arithm�tique

const largeint& largeint::add(const largeint& b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "add", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "add", 1, __LINE__);
  err = add_l(n_l, b.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "add", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::add(const USHORT b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "add", 0, __LINE__);
  err = uadd_l(n_l, b, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "add", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::sub(const largeint& b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "sub", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "sub", 1, __LINE__);
  err = sub_l(n_l, b.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_UFL:
      status = largeint_UFL;
      break;
    default:
      panic(largeint_ERR, "sub", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::sub(const USHORT b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "sub", 0, __LINE__);
  err = usub_l(n_l, b, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_UFL:
      status = largeint_UFL;
      break;
    default:
      panic(largeint_ERR, "sub", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::mul(const largeint& b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "mul", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "mul", 1, __LINE__);
  if (this == &b)
    err = sqr_l(n_l, n_l);
  else
    err = mul_l(n_l, b.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "mul", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::mul(const USHORT b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "mul", 0, __LINE__);
  err = umul_l(n_l, b, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "mul", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::sqr(void)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "sqr", 0, __LINE__);
  err = sqr_l(n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "sqr", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::div(const largeint& b, largeint& rem)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "div", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "div", 1, __LINE__);
  err = div_l(n_l, b.n_l, n_l, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = largeint_OK;
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "div", 1, __LINE__);
      break;
    default:
      panic(largeint_ERR, "div", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::div(const USHORT b, largeint& rem)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "div", 0, __LINE__);
  err = udiv_l(n_l, b, n_l, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = largeint_OK;
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "div", 1, __LINE__);
      break;
    default:
      panic(largeint_ERR, "div", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::mod(const largeint& b)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "mod", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "mod", 1, __LINE__);
  err = mod_l(n_l, b.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "mod", 1, __LINE__);
      break;
    default:
      panic(largeint_ERR, "mod", err, __LINE__);
    }
  return *this;
}

const USHORT largeint::mod(const USHORT b)
{
  USHORT rem;
  
  if (init == 0)
    panic(largeint_VAL, "mod", 0, __LINE__);
  rem = umod_l(n_l, b);
  if (rem == 0xffff)
    panic(largeint_DBZ, "mod", 1, __LINE__);
  return rem;
}

const largeint& largeint::mod2(const USHORT u)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "mod2", 0, __LINE__);
  err = mod2_l(n_l, u, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    default:
      panic(largeint_ERR, "mod2", err, __LINE__);
    }
  return *this;
}

// Fonctions membres d'arithm�tique modulo

const largeint& largeint::addmod(const largeint& b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "addmod", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "addmod", 1, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "addmod", 2, __LINE__);
  err = addmod_l(n_l, b.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "addmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "addmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::addmod(const USHORT b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "addmod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "addmod", 2, __LINE__);
  err = uaddmod_l(n_l, b, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "addmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "addmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::submod(const largeint& b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "submod", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "submod", 1, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "submod", 2, __LINE__);
  err = submod_l(n_l, b.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "submod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "submod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::submod(const USHORT b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "submod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "submod", 2, __LINE__);
  err = usubmod_l(n_l, b, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "submod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "submod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::mulmod(const largeint& b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "mulmod", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "mulmod", 1, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "mulmod", 2, __LINE__);
  if (&b == this)
    err = sqrmod_l(n_l, m.n_l, n_l);
  else
    err = mulmod_l(n_l, b.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "mulmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "mulmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::mulmod(const USHORT b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "mulmod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "mulmod", 2, __LINE__);
  err = umulmod_l(n_l, b, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "mulmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "mulmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::sqrmod(const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "sqrmod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "sqrmod", 1, __LINE__);
  err = sqrmod_l(n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "sqrmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "sqrmod", err, __LINE__);
    }
  return *this;
}

const int largeint::eqmod(const largeint& b, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "eqmod", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "eqmod", 1, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "eqmod", 2, __LINE__);
  err = eqmod_l(n_l, b.n_l, m.n_l);
  switch (err)
    {
    case 0:
    case 1:
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "eqmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "eqmod", err, __LINE__);
    }
  return err;
}

// Fonctions membres exponentielles

const largeint& largeint::expmod(const largeint& e, const largeint& m)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "expmod", 0, __LINE__);
  if (e.init == 0)
    panic(largeint_VAL, "expmod", 1, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "expmod", 2, __LINE__);
  err = expmod_l(n_l, e.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "expmod", 2, __LINE__);
      break;
    case BIGINT_MAL:
      panic(largeint_NPT, "expmod", 0, __LINE__);
      break;
    default:
      panic(largeint_ERR, "expmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::expmod(const USHORT e, const largeint& m)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "expmod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "expmod", 2, __LINE__);
  err = uexpmod_l(n_l, e, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "expmod", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "expmod", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::expmod2(const USHORT k, const largeint& m)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "expmod2", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "expmod2", 2, __LINE__);
  err = expmod2k_l(n_l, k, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_DBZ:
      panic(largeint_DBZ, "expmod2", 2, __LINE__);
      break;
    default:
      panic(largeint_ERR, "expmod2", err, __LINE__);
    }
  return *this;
}

// Fonctions membres op�rant sur les bits

const largeint& largeint::shift(const int nbrebits)
{
  int err;
  
  if (init == 0)
    panic(largeint_VAL, "shift", 0, __LINE__);
  err = shift_l(n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    case BIGINT_UFL:
      status = largeint_UFL;
      break;
    default:
      panic(largeint_ERR, "shift", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::setbit(const unsigned int p)
{
  int err;
  
  if (init == 0)
    {
      setzero_l(n_l);
      init = 1;
    }
  err = setbit_l(n_l, p);
  switch (err)
    {
    case 0:
    case 1:
      status = largeint_OK;
      break;
    case BIGINT_OFL:
      status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "setbit", err, __LINE__);
    }
  return *this;
}

const largeint& largeint::clearbit(const unsigned int p)
{
  int err;

  if (init == 0)
    panic(largeint_VAL, "clearbit", 0, __LINE__);
  err = clearbit_l(n_l, p);
  switch (err)
    {
    case 0:
    case 1:
      status = largeint_OK;
      break;
    default:
      panic(largeint_ERR, "setbit", err, __LINE__);
    }
  return *this;
}

const int largeint::testbit(const unsigned int p) const
{
  if (init == 0)
    panic(largeint_VAL, "testbit", 0, __LINE__);
  return (testbit_l(n_l, p));
}

const unsigned int largeint::numbits(void) const
{
  if (init == 0)
    panic(largeint_VAL, "numbits", 0, __LINE__);
  return (numbits_l(n_l));
}

// Fonctions membres de th�orie des nombres

const int largeint::iseven(void) const
{
  if (init == 0)
    panic(largeint_VAL, "iseven", 0, __LINE__);
  return (iseven_l(n_l));
}

const int largeint::MRprime(const unsigned int numsprimes, const unsigned int iter) const
{
  if (init == 0)
    panic(largeint_VAL, "MRprime", 0, __LINE__);
  return (MRprime_l(n_l, numsprimes, iter));
}

const int largeint::SSprime(const unsigned int numsprimes, const unsigned int iter) const
{
  if (init == 0)
    panic(largeint_VAL, "SSprime", 0, __LINE__);
  return (SSprime_l(n_l, numsprimes, iter));
}

const largeint largeint::issqr(void) const
{
  largeint sqroot;
  int sqr;
  
  if (init == 0)
    panic(largeint_VAL, "issqr", 0, __LINE__);
  sqr = issqr_l(n_l, sqroot.n_l);
  switch (sqr)
    {
    case 1:
      sqroot.init = 1;
      sqroot.status = largeint_OK;
      break;
    case 0:
      sqroot.init = 0;
      break;
    default:
      panic(largeint_ERR, "issqr", sqr, __LINE__);
    }
  return sqroot;
}

const largeint largeint::introot(void) const
{
  largeint sqroot;
  
  if (init == 0)
    panic(largeint_VAL, "introot", 0, __LINE__);
  introot_l(n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = largeint_OK;
  return sqroot;
}

const largeint largeint::gcd(const largeint& b) const
{
  largeint g;

  if (init == 0)
    panic(largeint_VAL, "gcd", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "gcd", 1, __LINE__);
  gcd_l(n_l, b.n_l, g.n_l);
  g.init = 1;
  g.status = largeint_OK;
  return g;
}

const largeint largeint::lcm(const largeint& b) const
{
  largeint l;
  int err;

  if (init == 0)
    panic(largeint_VAL, "lcm", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "lcm", 1, __LINE__);
  err =  lcm_l(n_l, b.n_l, l.n_l);
  switch (err)
    {
    case BIGINT_OK:
      l.init = 1;
      l.status = largeint_OK;
      break;
    case BIGINT_OFL:
      l.init = 1;
      l.status = largeint_OFL;
      break;
    default:
      panic(largeint_ERR, "lcm", err, __LINE__);
    }
  return l;
}

const largeint largeint::xgcd(const largeint& b, largeint& u, int& usign, largeint& v, int& vsign) const
{
  largeint g;
  
  if (init == 0)
    panic(largeint_VAL, "xgcd", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "xgcd", 1, __LINE__);
  xgcd_l(n_l, b.n_l, g.n_l, u.n_l, &usign, v.n_l, &vsign);
  g.init = 1;
  u.init = 1;
  v.init = 1;
  g.status = largeint_OK;
  u.status = largeint_OK;
  v.status = largeint_OK;
  return g;
}

const largeint largeint::invmod(const largeint& m) const
{
  largeint inv, g;

  if (init == 0)
    panic(largeint_VAL, "invmod", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "invmod", 1, __LINE__);
  invmod_l(n_l, m.n_l, g.n_l, inv.n_l);
  inv.init = 1;
  inv.status = largeint_OK;
  return inv;
}

const int largeint::jacobi(const largeint& b) const
{
  if (init == 0)
    panic(largeint_VAL, "jacobi", 0, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "jacobi", 1, __LINE__);
  return (jacobi_l(n_l, b.n_l));
}

const largeint largeint::root(const largeint& p) const
{
  largeint x;

   if (init == 0)
    panic(largeint_VAL, "root", 0, __LINE__);
  if (p.init == 0)
    panic(largeint_VAL, "root", 1, __LINE__);
  if (proot_l(n_l, p.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = largeint_OK;
    }
  else
    {
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
    }
  return x;
}

const largeint largeint::root(const largeint& p, const largeint& q) const
{
  largeint x;

  if (init == 0)
    panic(largeint_VAL, "root", 0, __LINE__);
  if (p.init == 0)
    panic(largeint_VAL, "root", 1, __LINE__);
  if (q.init == 0)
    panic(largeint_VAL, "root", 2, __LINE__);
  if (pqroot_l(n_l, p.n_l, q.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = largeint_OK;
    }
  else
    {
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
    }
  return x;
}

const largeint largeint::chinrem(const largeint& m, const largeint& b, const largeint& n) const
{
  largeint x;
  int err;
    
  if (init == 0)
    panic(largeint_VAL, "chinrem", 0, __LINE__);
  if (m.init == 0)
    panic(largeint_VAL, "chinrem", 1, __LINE__);
  if (b.init == 0)
    panic(largeint_VAL, "chinrem", 2, __LINE__);
  if (n.init == 0)
    panic(largeint_VAL, "chinrem", 3, __LINE__);
  bigint* coeff_l[4];
  coeff_l[0] = n_l;
  coeff_l[1] = m.n_l;
  coeff_l[2] = b.n_l;
  coeff_l[3] = n.n_l;
  err = chinrem_l(2, coeff_l, x.n_l);
  switch (err)
    {
    case BIGINT_OK:
      x.init = 1;
      x.status = largeint_OK;
      break;
    case BIGINT_OFL:
      x.init = 1;
      x.status = largeint_OFL;
      break;
    case 1:
    case 2:
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
      break;
    default:
      largeint::panic(largeint_ERR, "chinrem", err, __LINE__);
    }
  return x;
}

// Fonctions membres de conversion

const char* largeint::largeint2str(const USHORT base, const int showbase) const
{
  if (init == 0)
    panic(largeint_VAL, "bint2str", 0, __LINE__);
  return (bigint2str_l(n_l, base, showbase));
}

const UCHAR* largeint::largeint2byte(int* l) const
{
  if (init == 0)
    panic(largeint_VAL, "bint2byte", 0, __LINE__);
  return (bigint2byte_l(n_l, l));
}

// Fonction membre de purge

const void largeint::purge(void)
{
  if (init == 0)
    panic(largeint_VAL, "purge", 0, __LINE__);
  purge_l(n_l);
  setdigits_l(n_l, 0);
}

/* Fonctions amies */
/*******************/

// Fonctions amies d'arithm�tique

const largeint add(const largeint& a, const largeint& b)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "add", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "add", 2, __LINE__);
  err = add_l(a.n_l, b.n_l, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "add", err, __LINE__);
    }
  return sum;
}

const largeint add(const largeint& a, const USHORT b)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "add", 1, __LINE__);
  err = uadd_l(a.n_l, b, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "add", err, __LINE__);
    }
  return sum;
}

const largeint sub(const largeint& a, const largeint& b)
{
  largeint diff;
  int err;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "sub", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "sub", 2, __LINE__);
  err = sub_l(a.n_l, b.n_l, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "sub", err, __LINE__);
    }
  return diff;
}

const largeint sub(const largeint& a, const USHORT b)
{
  largeint diff;
  int err;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "sub", 1, __LINE__);
  err = usub_l(a.n_l, b, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "sub", err, __LINE__);
    }
  return diff;
}

const largeint mul(const largeint& a, const largeint& b)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mul", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "mul", 2, __LINE__);
  if (&a == &b)
    err = sqr_l(a.n_l, prod.n_l);
  else
    err = mul_l(a.n_l, b.n_l, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "mul", err, __LINE__);
    }
  return prod;
}

const largeint mul(const largeint& a, const USHORT b)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mul", 1, __LINE__);
  err = umul_l(a.n_l, b, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "mul", err, __LINE__);
    }
  return prod;
}

const largeint sqr(const largeint& a)
{
  largeint sqr;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "sqr", 1, __LINE__);
  err = sqr_l(a.n_l, sqr.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sqr.init = 1;
      sqr.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sqr.init = 1;
      sqr.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "sqr", err, __LINE__);
    }
  return sqr;
}

const largeint div(const largeint& a, const largeint& b, largeint& r)
{
  largeint quot;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "div", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "div", 2, __LINE__);
  err = div_l(a.n_l, b.n_l, quot.n_l, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      quot.init = 1;
      quot.status = largeint_OK;
      r.init = 1;
      r.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "div", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "div", err, __LINE__);
    }
  return quot;
}

const largeint div(const largeint& a, const USHORT b, largeint& r)
{
  largeint quot;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "div", 1, __LINE__);
  err = udiv_l(a.n_l, b, quot.n_l, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      quot.init = 1;
      quot.status = largeint_OK;
      r.init = 1;
      r.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "div", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "div", err, __LINE__);
    }
  return quot;
}

const largeint mod(const largeint& a, const largeint& b)
{
  largeint rem;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mod", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "mod", 2, __LINE__);
  err = mod_l(a.n_l, b.n_l, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "mod", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_DBZ, "mod", err, __LINE__);
    }
  return rem;
}

const USHORT mod(const largeint& a, const USHORT b)
{
  USHORT rem;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "mod", 1, __LINE__);
  rem = umod_l(a.n_l, b);
  if (rem == 0xffff)
    largeint::panic(largeint_DBZ, "mod", 2, __LINE__);
  return rem;
}

const largeint mod2(const largeint& a, const USHORT k)
{
  largeint rem;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mod2", 1, __LINE__);
  err = mod2_l(a.n_l, k, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = largeint_OK;
      break;
    default:
      largeint::panic(largeint_DBZ, "mod2", err, __LINE__);
    }
  return rem;
}

// Fonctions amies d'arithm�tique modulo

const largeint addmod(const largeint& a, const largeint& b, const largeint& m)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "addmod", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "addmod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "addmod", 3, __LINE__);
  err = addmod_l(a.n_l, b.n_l, m.n_l, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "addmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "addmod", err, __LINE__);
    }
  return sum;
}

const largeint addmod(const largeint& a, const USHORT b, const largeint& m)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "addmod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "addmod", 3, __LINE__);
  err = uaddmod_l(a.n_l, b, m.n_l, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "addmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "addmod", err, __LINE__);
    }
  return sum;
}

const largeint submod(const largeint& a, const largeint& b, const largeint& m)
{
  largeint diff;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "submod", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "submod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "submod", 3, __LINE__);
  err = submod_l(a.n_l, b.n_l, m.n_l, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "submod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "submod", err, __LINE__);
    }
  return diff;
}

const largeint submod(const largeint& a, const USHORT b, const largeint& m)
{
  largeint diff;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "submod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "submod", 3, __LINE__);
  err = usubmod_l(a.n_l, b, m.n_l, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "submod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "submod", err, __LINE__);
    }
  return diff;
}

const largeint mulmod(const largeint& a, const largeint& b, const largeint& m)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mulmod", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "mulmod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "mulmod", 3, __LINE__);
  if (&a == &b)
    err = sqrmod_l(a.n_l, m.n_l, prod.n_l);
  else
    err = mulmod_l(a.n_l, b.n_l, m.n_l, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "mulmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "mulmod", err, __LINE__);
    }
  return prod;
}

const largeint mulmod(const largeint& a, const USHORT b, const largeint& m)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "mulmod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "mulmod", 3, __LINE__);
  err = umulmod_l(a.n_l, b, m.n_l, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "mulmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "mulmod", err, __LINE__);
    }
  return prod;
}

const largeint sqrmod(const largeint& a, const largeint& m)
{
  largeint sqr;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "sqrmod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "sqrmod", 2, __LINE__);
  err = sqrmod_l(a.n_l, m.n_l, sqr.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sqr.init = 1;
      sqr.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "sqrmod", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "sqrmod", err, __LINE__);
    }
  return sqr;
}

const int eqmod(const largeint & a, const largeint& b, const largeint& m)
{
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "eqmod", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "eqmod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "eqmod", 3, __LINE__);
  err = eqmod_l(a.n_l, b.n_l, m.n_l);
  switch (err)
    {
    case 0:
    case 1:
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "eqmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "eqmod", err, __LINE__);
    }
  return err;
}

// Fonctions amies exponentielles

const largeint expmod(const largeint& a, const largeint& e, const largeint& m)
{
  largeint exp;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "expmod", 1, __LINE__);
  if (e.init == 0)
    largeint::panic(largeint_VAL, "expmod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "expmod", 3, __LINE__);
  err = expmod_l(a.n_l, e.n_l, m.n_l, exp.n_l);
  switch (err)
    {
    case BIGINT_OK:
      exp.init = 1;
      exp.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "expmod", 3, __LINE__);
      break;
    case BIGINT_MAL:
      largeint::panic(largeint_NPT, "expmod", 0, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "expmod", err, __LINE__);
    }
  return exp;
}

const largeint expmod(const largeint& a, const USHORT e, const largeint& m)
{
  largeint exp;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "expmod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "expmod", 3, __LINE__);
  err = uexpmod_l(a.n_l, e, m.n_l, exp.n_l);
  switch (err)
    {
    case BIGINT_OK:
      exp.init = 1;
      exp.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "expmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "expmod", err, __LINE__);
    }
  return exp;
}

const largeint expmod(const USHORT a, const largeint& e, const largeint& m)
{
  largeint exp;
  int err;

  if (e.init == 0)
    largeint::panic(largeint_VAL, "expmod", 2, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "expmod", 3, __LINE__);
  err = wexpmod_l(a, e.n_l, m.n_l, exp.n_l);
  switch (err)
    {
    case BIGINT_OK:
      exp.init = 1;
      exp.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "expmod", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "expmod", err, __LINE__);
    }
  return exp;
}

const largeint expmod2(const largeint& a, const USHORT k, const largeint& m)
{
  largeint exp;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "expmod2", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "expmod2", 3, __LINE__);
  err = expmod2k_l(a.n_l, k, m.n_l, exp.n_l);
  switch (err)
    {
    case BIGINT_OK:
      exp.init = 1;
      exp.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "expmod2", 3, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "expmod2", err, __LINE__);
    }
  return exp;
}

// Fonctions amies op�rant sur les bits

const largeint shift(const largeint& a, const int nbrebits)
{
  largeint shft = a;
  int err;

  if (shft.init == 0)
    largeint::panic(largeint_VAL, "shift", 1, __LINE__);
  err = shift_l(shft.n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shft.status = largeint_OK;
      break;
    case BIGINT_OFL:
      shft.status = largeint_OFL;
      break;
    case BIGINT_UFL:
      shft.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "shift", err, __LINE__);
    }
  return shft;
}

const unsigned int numbits(const largeint& a)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "numbits", 1, __LINE__);
  return (numbits_l(a.n_l));
}

// Fonctions amies de th�orie des nombres

const int iseven(const largeint& a)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "iseven", 1, __LINE__);
  return (iseven_l(a.n_l));
}

const USHORT sieve(const largeint& a, const unsigned int numsprimes)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "sieve", 1, __LINE__);
  return (sieve_l(a.n_l, numsprimes));
}

const int MRprime(const largeint& a, const unsigned int numsprimes, const unsigned int iter)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "MRprime", 1, __LINE__);
  return (MRprime_l(a.n_l, numsprimes, iter));
}

const int SSprime(const largeint& a, const unsigned int numsprimes, const unsigned int iter)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "MRprime", 1, __LINE__);
  return (SSprime_l(a.n_l, numsprimes, iter));
}

const largeint issqr(const largeint& a)
{
  largeint sqroot;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "issqr", 1, __LINE__);
  err = issqr_l(a.n_l, sqroot.n_l);
  switch (err)
    {
    case 0:
    case 1:
      sqroot.init = 1;
      sqroot.status = largeint_OK;
      break;

    default:
      largeint::panic(largeint_ERR, "issqr", err, __LINE__);
    }
  return sqroot;
}

const largeint introot(const largeint& a)
{
  largeint sqroot;
  
  if (a.init ==0)
    largeint::panic(largeint_VAL, "introot", 1, __LINE__);
  introot_l(a.n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = largeint_OK;
  return sqroot;
}

const largeint gcd(const largeint& a, const largeint& b)
{
  largeint g;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "gcd", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "gcd", 2, __LINE__);
  gcd_l(a.n_l, b.n_l, g.n_l);
  g.init = 1;
  g.status = largeint_OK;
  return g;
}

const largeint lcm(const largeint& a, const largeint& b)
{
  largeint l;
  int err;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "lcm", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "lcm", 2, __LINE__);
  err = lcm_l(a.n_l, b.n_l, l.n_l);
  switch (err)
    {
    case BIGINT_OK:
      l.init = 1;
      l.status = largeint_OK;
      break;
    case BIGINT_OFL:
      l.init = 1;
      l.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "lcm", err, __LINE__);
    }
  return l;
}

const largeint xgcd(const largeint& a, const largeint& b, largeint& u, int& usign, largeint& v, int& vsign)
{
  largeint g;
  
    if (a.init == 0)
      largeint::panic(largeint_VAL, "xgcd", 1, __LINE__);
    if (b.init == 0)
      largeint::panic(largeint_VAL, "xgcd", 2, __LINE__);
    xgcd_l(a.n_l, b.n_l, g.n_l, u.n_l, &usign, v.n_l, &vsign);
    g.init = 1;
    g.status = largeint_OK;
    u.init = 1;
    u.status = largeint_OK;
    v.init = 1;
    v.status = largeint_OK;
    return g;
}

const largeint invmod(const largeint& a, const largeint& m)
{
  largeint inv, g;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "invmod", 1, __LINE__);
  if (m.init == 0)
    largeint::panic(largeint_VAL, "invmod", 2, __LINE__);
  invmod_l(a.n_l, m.n_l, g.n_l, inv.n_l);
  inv.init = 1;
  inv.status = largeint_OK;
  return inv;
}

const int jacobi(const largeint& a, const largeint& b)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "jacobi", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "jacobi", 2, __LINE__);
  return (jacobi_l(a.n_l, b.n_l));
}

const largeint root(const largeint& a, const largeint& p)
{
  largeint x;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "root", 1, __LINE__);
  if (p.init == 0)
    largeint::panic(largeint_VAL, "root", 2, __LINE__);
  if (proot_l(a.n_l, p.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = largeint_OK;
    }
  else
    {
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
    }
  return x;
}

const largeint root(const largeint& a, const largeint& p, const largeint& q)
{
  largeint x;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "root", 1, __LINE__);
  if (p.init == 0)
    largeint::panic(largeint_VAL, "root", 2, __LINE__);
  if (q.init == 0)
    largeint::panic(largeint_VAL, "root", 3, __LINE__);
  if (pqroot_l(a.n_l, p.n_l, q.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = largeint_OK;
    }
  else
    {
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
    }
  return x;
}

const largeint chinrem(const unsigned int numeq, largeint** coeff)
{
  largeint x;
  bigint** coeff_l = new bigint* [numeq << 1];
  unsigned int i;
  int err;

  if (coeff_l == NULL)
    return largeint((USHORT)0);
  bigint** c_l = coeff_l;
  for (i = 0; i < (numeq << 1); coeff_l++, coeff++, i++)
    {
      if (((*coeff)->init) == 0)
	largeint::panic(largeint_VAL, "chinrem", i, __LINE__);
      *coeff_l = (*coeff)->n_l;
    }
  err = chinrem_l(numeq, c_l, x.n_l);
  switch (err)
    {
    case BIGINT_OK:
      x.init = 1;
      x.status = largeint_OK;
      break;
    case BIGINT_OFL:
      x.init = 1;
      x.status = largeint_OFL;
      break;
    case 1:
    case 2:
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
      break;
    default:
      largeint::panic(largeint_ERR, "chinrem", err, __LINE__);
    }
  delete [] c_l;
  return x;
}

const largeint primelt(const unsigned int numsprimes, largeint** primes)
{
  largeint x;
  bigint** primes_l = new bigint* [numsprimes + 1];
  unsigned int i;

  if (primes_l == NULL)
    largeint::panic(largeint_NEW, "primelt", 0, __LINE__);
  bigint** pr_l = primes_l;
  for (i = 0; i <= numsprimes; primes_l++, primes++, i++)
    {
      if (((*primes)->init) == 0)
	largeint::panic(largeint_VAL, "primelt", i, __LINE__);
      *primes_l = (*primes)->n_l;
    }
  if (primelt_l(x.n_l, numsprimes, pr_l) == 0)
    {
      x.init = 1;
      x.status = largeint_OK;
    }
  else
    {
      x = largeint((USHORT)0);
      x.status = largeint_ERR;
    }
  delete [] pr_l;
  return x;
}

// Fonctions amies de g�n�ration al�atoire

void seed64(const largeint& seed)
{
  if (seed.init == 0)
    largeint::panic(largeint_VAL, "seed64", 1, __LINE__);
  seed64_l(seed.n_l);
}

void seed64(const ULONG seed)
{
  ulseed64_l(seed);
}

largeint rand64(const int l)
{
  largeint ran;
  if (l < (int)BIGINTMAXBIT)
    rand_l(ran.n_l, l);
  else
    rand_l(ran.n_l, BIGINTMAXBIT);
  ran.init = 1;
  ran.status = largeint_OK;
  return ran;
}

largeint rand64(const largeint& min, const largeint& max)
{
  largeint ran, t;
  USHORT l = (numbits(min) + numbits(max)) >> 1;

  if (min.init == 0)
    largeint::panic(largeint_VAL, "rand64", 1, __LINE__);
  if (max.init == 0)
    largeint::panic(largeint_VAL, "rand64", 2, __LINE__);
  if (max < min)
    largeint::panic(largeint_VAL, "rand64", 1, __LINE__);
  t = max - min;
   if (l < BIGINTMAXBIT)
    rand_l(ran.n_l, l);
  else
    rand_l(ran.n_l, BIGINTMAXBIT);
   ran.init = 1;
   ran.status = largeint_OK;
   if (ran < min)
     ran = ran + min;
   if (ran > max)
     ran = min + ran % (t + 1);
   return ran;
}

void seedBBS(const largeint& seed)
{
  if (seed.init == 0)
    largeint::panic(largeint_VAL, "seedBBS", 1, __LINE__);
  seedBBS_l(seed.n_l);
}

void seedBBS(const ULONG seed)
{
  ulseedBBS_l(seed);
}

largeint randBBS(const int l)
{
  largeint ran;
  
  if (l < (int)BIGINTMAXBIT)
    randBBS_l(ran.n_l, l);
  else
    randBBS_l(ran.n_l, BIGINTMAXBIT);
  ran.init = 1;
  ran.status = largeint_OK;
  return ran;
}

largeint randBBS(const largeint& min, const largeint& max)
{
  largeint ran, t;
  USHORT l = (numbits(min) + numbits(max)) >> 1;

  if (min.init == 0)
    largeint::panic(largeint_VAL, "randBBS", 1, __LINE__);
  if (max.init == 0)
    largeint::panic(largeint_VAL, "randBBS", 2, __LINE__);
  if (max < min)
    largeint::panic(largeint_VAL, "randBBS", 1, __LINE__);
  t = max - min;
   if (l < BIGINTMAXBIT)
    randBBS_l(ran.n_l, l);
  else
    randBBS_l(ran.n_l, BIGINTMAXBIT);
   ran.init = 1;
   ran.status = largeint_OK;
   if (ran < min)
     ran = ran + min;
   if (ran > max)
     ran = min + ran % (t + 1);
   return ran;
}

// Fonctions amies de g�n�ration de nombres premiers

const largeint primeBBS(const largeint& min, const largeint& max, const largeint& f)
{
  largeint prim, t;
  largeint one((USHORT)1);
  
  if (min.init == 0)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (max.init == 0)
    largeint::panic(largeint_VAL, "prime", 2, __LINE__);
  if (f.init == 0)
    largeint::panic(largeint_VAL, "prime", 3, __LINE__);
  if (min > max)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (f.iseven() == 1)
    largeint::panic(largeint_VAL, "prime", 3, __LINE__);
  prim = randBBS(min, max);
  t = max - min;
  //if (prim.iseven() == 1)
  //  prim = prim + 1;
  prim.setbit(0);
  if (prim > max)
    prim = min + prim % (t + 1);
  while ((gcd(prim - 1, f) != one) || (prim.MRprime(302, 5) == 0))
    {
      prim = prim + 2;
      while (prim > max)
	{
	  prim = min + prim % (t + 1);
	  //if (prim.iseven() == 1)
	  //  prim = prim + 1;
	  prim.setbit(0);
	}
    }
  return prim;
}

const largeint primeBBS(const USHORT l, const largeint& f)
{
  largeint min((USHORT)0);
  largeint max((USHORT)0);

  if (l == 0)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (l > BIGINTMAXBIT)
    largeint::panic(largeint_OFL, "prime", 1, __LINE__);
  min.setbit(l - 1);
  max.setbit(l);
  max = max - 1;
  return (primeBBS(min, max, f));
}

const largeint primeBBS(const USHORT l)
{
  largeint one((USHORT)1);
  
  return (primeBBS(l, one));
}

const largeint prime64(const largeint& min, const largeint& max, const largeint& f)
{
  largeint prim, t;
  largeint one((USHORT)1);
  
  if (min.init == 0)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (max.init == 0)
    largeint::panic(largeint_VAL, "prime", 2, __LINE__);
  if (f.init == 0)
    largeint::panic(largeint_VAL, "prime", 3, __LINE__);
  if (min > max)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (f.iseven() == 1)
    largeint::panic(largeint_VAL, "prime", 3, __LINE__);
  prim = rand64(min, max);
  t = max - min;
  //if (prim.iseven() == 1)
  //  prim = prim + 1;
  prim.setbit(0);
  if (prim > max)
    prim = min + prim % (t + 1);
  while ((gcd(prim - 1, f) != one) || (prim.MRprime(302, 5) == 0))
    {
      prim = prim + 2;
      while (prim > max)
	{
	  prim = min + prim % (t + 1);
	  //if (prim.iseven() == 1)
	  //  prim = prim + 1;
	  prim.setbit(0);
	}
    }
  return prim;
}

const largeint prime64(const USHORT l, const largeint& f)
{
  largeint min((USHORT)0);
  largeint max((USHORT)0);

  if (l == 0)
    largeint::panic(largeint_VAL, "prime", 1, __LINE__);
  if (l > BIGINTMAXBIT)
    largeint::panic(largeint_OFL, "prime", 1, __LINE__);
  min.setbit(l - 1);
  max.setbit(l);
  max = max - 1;
  return (prime64(min, max, f));
}

const largeint prime64(const USHORT l)
{
  largeint one((USHORT)1);
  
  return (prime64(l, one));
}

USHORT  smallprim[6542] =
{   2, 1, 2, 2, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6, 6, 2, 6, 4, 2,
    6, 4, 6, 8, 4, 2, 4, 2, 4, 14, 4, 6, 2, 10, 2, 6, 6, 4, 6, 6,
    2, 10, 2, 4, 2, 12, 12, 4, 2, 4, 6, 2, 10, 6, 6, 6, 2, 6, 4, 2,
    10, 14, 4, 2, 4, 14, 6, 10, 2, 4, 6, 8, 6, 6, 4, 6, 8, 4, 8, 10,
    2, 10, 2, 6, 4, 6, 8, 4, 2, 4, 12, 8, 4, 8, 4, 6, 12, 2, 18, 6,
    10, 6, 6, 2, 6, 10, 6, 6, 2, 6, 6, 4, 2, 12, 10, 2, 4, 6, 6, 2,
    12, 4, 6, 8, 10, 8, 10, 8, 6, 6, 4, 8, 6, 4, 8, 4, 14, 10, 12, 2,
    10, 2, 4, 2, 10, 14, 4, 2, 4, 14, 4, 2, 4, 20, 4, 8, 10, 8, 4, 6,
    6, 14, 4, 6, 6, 8, 6, 12, 4, 6, 2, 10, 2, 6, 10, 2, 10, 2, 6, 18,
    4, 2, 4, 6, 6, 8, 6, 6, 22, 2, 10, 8, 10, 6, 6, 8, 12, 4, 6, 6,
    2, 6, 12, 10, 18, 2, 4, 6, 2, 6, 4, 2, 4, 12, 2, 6, 34, 6, 6, 8,
    18, 10, 14, 4, 2, 4, 6, 8, 4, 2, 6, 12, 10, 2, 4, 2, 4, 6, 12, 12,
    8, 12, 6, 4, 6, 8, 4, 8, 4, 14, 4, 6, 2, 4, 6, 2, 6, 10, 20, 6,
    4, 2, 24, 4, 2, 10, 12, 2, 10, 8, 6, 6, 6, 18, 6, 4, 2, 12, 10, 12,
    8, 16, 14, 6, 4, 2, 4, 2, 10, 12, 6, 6, 18, 2, 16, 2, 22, 6, 8, 6,
    4, 2, 4, 8, 6, 10, 2, 10, 14, 10, 6, 12, 2, 4, 2, 10, 12, 2, 16, 2,
    6, 4, 2, 10, 8, 18, 24, 4, 6, 8, 16, 2, 4, 8, 16, 2, 4, 8, 6, 6,
    4, 12, 2, 22, 6, 2, 6, 4, 6, 14, 6, 4, 2, 6, 4, 6, 12, 6, 6, 14,
    4, 6, 12, 8, 6, 4, 26, 18, 10, 8, 4, 6, 2, 6, 22, 12, 2, 16, 8, 4,
    12, 14, 10, 2, 4, 8, 6, 6, 4, 2, 4, 6, 8, 4, 2, 6, 10, 2, 10, 8,
    4, 14, 10, 12, 2, 6, 4, 2, 16, 14, 4, 6, 8, 6, 4, 18, 8, 10, 6, 6,
    8, 10, 12, 14, 4, 6, 6, 2, 28, 2, 10, 8, 4, 14, 4, 8, 12, 6, 12, 4,
    6, 20, 10, 2, 16, 26, 4, 2, 12, 6, 4, 12, 6, 8, 4, 8, 22, 2, 4, 2,
    12, 28, 2, 6, 6, 6, 4, 6, 2, 12, 4, 12, 2, 10, 2, 16, 2, 16, 6, 20,
    16, 8, 4, 2, 4, 2, 22, 8, 12, 6, 10, 2, 4, 6, 2, 6, 10, 2, 12, 10,
    2, 10, 14, 6, 4, 6, 8, 6, 6, 16, 12, 2, 4, 14, 6, 4, 8, 10, 8, 6,
    6, 22, 6, 2, 10, 14, 4, 6, 18, 2, 10, 14, 4, 2, 10, 14, 4, 8, 18, 4,
    6, 2, 4, 6, 2, 12, 4, 20, 22, 12, 2, 4, 6, 6, 2, 6, 22, 2, 6, 16,
    6, 12, 2, 6, 12, 16, 2, 4, 6, 14, 4, 2, 18, 24, 10, 6, 2, 10, 2, 10,
    2, 10, 6, 2, 10, 2, 10, 6, 8, 30, 10, 2, 10, 8, 6, 10, 18, 6, 12, 12,
    2, 18, 6, 4, 6, 6, 18, 2, 10, 14, 6, 4, 2, 4, 24, 2, 12, 6, 16, 8,
    6, 6, 18, 16, 2, 4, 6, 2, 6, 6, 10, 6, 12, 12, 18, 2, 6, 4, 18, 8,
    24, 4, 2, 4, 6, 2, 12, 4, 14, 30, 10, 6, 12, 14, 6, 10, 12, 2, 4, 6,
    8, 6, 10, 2, 4, 14, 6, 6, 4, 6, 2, 10, 2, 16, 12, 8, 18, 4, 6, 12,
    2, 6, 6, 6, 28, 6, 14, 4, 8, 10, 8, 12, 18, 4, 2, 4, 24, 12, 6, 2,
    16, 6, 6, 14, 10, 14, 4, 30, 6, 6, 6, 8, 6, 4, 2, 12, 6, 4, 2, 6,
    22, 6, 2, 4, 18, 2, 4, 12, 2, 6, 4, 26, 6, 6, 4, 8, 10, 32, 16, 2,
    6, 4, 2, 4, 2, 10, 14, 6, 4, 8, 10, 6, 20, 4, 2, 6, 30, 4, 8, 10,
    6, 6, 8, 6, 12, 4, 6, 2, 6, 4, 6, 2, 10, 2, 16, 6, 20, 4, 12, 14,
    28, 6, 20, 4, 18, 8, 6, 4, 6, 14, 6, 6, 10, 2, 10, 12, 8, 10, 2, 10,
    8, 12, 10, 24, 2, 4, 8, 6, 4, 8, 18, 10, 6, 6, 2, 6, 10, 12, 2, 10,
    6, 6, 6, 8, 6, 10, 6, 2, 6, 6, 6, 10, 8, 24, 6, 22, 2, 18, 4, 8,
    10, 30, 8, 18, 4, 2, 10, 6, 2, 6, 4, 18, 8, 12, 18, 16, 6, 2, 12, 6,
    10, 2, 10, 2, 6, 10, 14, 4, 24, 2, 16, 2, 10, 2, 10, 20, 4, 2, 4, 8,
    16, 6, 6, 2, 12, 16, 8, 4, 6, 30, 2, 10, 2, 6, 4, 6, 6, 8, 6, 4,
    12, 6, 8, 12, 4, 14, 12, 10, 24, 6, 12, 6, 2, 22, 8, 18, 10, 6, 14, 4,
    2, 6, 10, 8, 6, 4, 6, 30, 14, 10, 2, 12, 10, 2, 16, 2, 18, 24, 18, 6,
    16, 18, 6, 2, 18, 4, 6, 2, 10, 8, 10, 6, 6, 8, 4, 6, 2, 10, 2, 12,
    4, 6, 6, 2, 12, 4, 14, 18, 4, 6, 20, 4, 8, 6, 4, 8, 4, 14, 6, 4,
    14, 12, 4, 2, 30, 4, 24, 6, 6, 12, 12, 14, 6, 4, 2, 4, 18, 6, 12, 8,
    6, 4, 12, 2, 12, 30, 16, 2, 6, 22, 14, 6, 10, 12, 6, 2, 4, 8, 10, 6,
    6, 24, 14, 6, 4, 8, 12, 18, 10, 2, 10, 2, 4, 6, 20, 6, 4, 14, 4, 2,
    4, 14, 6, 12, 24, 10, 6, 8, 10, 2, 30, 4, 6, 2, 12, 4, 14, 6, 34, 12,
    8, 6, 10, 2, 4, 20, 10, 8, 16, 2, 10, 14, 4, 2, 12, 6, 16, 6, 8, 4,
    8, 4, 6, 8, 6, 6, 12, 6, 4, 6, 6, 8, 18, 4, 20, 4, 12, 2, 10, 6,
    2, 10, 12, 2, 4, 20, 6, 30, 6, 4, 8, 10, 12, 6, 2, 28, 2, 6, 4, 2,
    16, 12, 2, 6, 10, 8, 24, 12, 6, 18, 6, 4, 14, 6, 4, 12, 8, 6, 12, 4,
    6, 12, 6, 12, 2, 16, 20, 4, 2, 10, 18, 8, 4, 14, 4, 2, 6, 22, 6, 14,
    6, 6, 10, 6, 2, 10, 2, 4, 2, 22, 2, 4, 6, 6, 12, 6, 14, 10, 12, 6,
    8, 4, 36, 14, 12, 6, 4, 6, 2, 12, 6, 12, 16, 2, 10, 8, 22, 2, 12, 6,
    4, 6, 18, 2, 12, 6, 4, 12, 8, 6, 12, 4, 6, 12, 6, 2, 12, 12, 4, 14,
    6, 16, 6, 2, 10, 8, 18, 6, 34, 2, 28, 2, 22, 6, 2, 10, 12, 2, 6, 4,
    8, 22, 6, 2, 10, 8, 4, 6, 8, 4, 12, 18, 12, 20, 4, 6, 6, 8, 4, 2,
    16, 12, 2, 10, 8, 10, 2, 4, 6, 14, 12, 22, 8, 28, 2, 4, 20, 4, 2, 4,
    14, 10, 12, 2, 12, 16, 2, 28, 8, 22, 8, 4, 6, 6, 14, 4, 8, 12, 6, 6,
    4, 20, 4, 18, 2, 12, 6, 4, 6, 14, 18, 10, 8, 10, 32, 6, 10, 6, 6, 2,
    6, 16, 6, 2, 12, 6, 28, 2, 10, 8, 16, 6, 8, 6, 10, 24, 20, 10, 2, 10,
    2, 12, 4, 6, 20, 4, 2, 12, 18, 10, 2, 10, 2, 4, 20, 16, 26, 4, 8, 6,
    4, 12, 6, 8, 12, 12, 6, 4, 8, 22, 2, 16, 14, 10, 6, 12, 12, 14, 6, 4,
    20, 4, 12, 6, 2, 6, 6, 16, 8, 22, 2, 28, 8, 6, 4, 20, 4, 12, 24, 20,
    4, 8, 10, 2, 16, 2, 12, 12, 34, 2, 4, 6, 12, 6, 6, 8, 6, 4, 2, 6,
    24, 4, 20, 10, 6, 6, 14, 4, 6, 6, 2, 12, 6, 10, 2, 10, 6, 20, 4, 26,
    4, 2, 6, 22, 2, 24, 4, 6, 2, 4, 6, 24, 6, 8, 4, 2, 34, 6, 8, 16,
    12, 2, 10, 2, 10, 6, 8, 4, 8, 12, 22, 6, 14, 4, 26, 4, 2, 12, 10, 8,
    4, 8, 12, 4, 14, 6, 16, 6, 8, 4, 6, 6, 8, 6, 10, 12, 2, 6, 6, 16,
    8, 6, 6, 12, 10, 2, 6, 18, 4, 6, 6, 6, 12, 18, 8, 6, 10, 8, 18, 4,
    14, 6, 18, 10, 8, 10, 12, 2, 6, 12, 12, 36, 4, 6, 8, 4, 6, 2, 4, 18,
    12, 6, 8, 6, 6, 4, 18, 2, 4, 2, 24, 4, 6, 6, 14, 30, 6, 4, 6, 12,
    6, 20, 4, 8, 4, 8, 6, 6, 4, 30, 2, 10, 12, 8, 10, 8, 24, 6, 12, 4,
    14, 4, 6, 2, 28, 14, 16, 2, 12, 6, 4, 20, 10, 6, 6, 6, 8, 10, 12, 14,
    10, 14, 16, 14, 10, 14, 6, 16, 6, 8, 6, 16, 20, 10, 2, 6, 4, 2, 4, 12,
    2, 10, 2, 6, 22, 6, 2, 4, 18, 8, 10, 8, 22, 2, 10, 18, 14, 4, 2, 4,
    18, 2, 4, 6, 8, 10, 2, 30, 4, 30, 2, 10, 2, 18, 4, 18, 6, 14, 10, 2,
    4, 20, 36, 6, 4, 6, 14, 4, 20, 10, 14, 22, 6, 2, 30, 12, 10, 18, 2, 4,
    14, 6, 22, 18, 2, 12, 6, 4, 8, 4, 8, 6, 10, 2, 12, 18, 10, 14, 16, 14,
    4, 6, 6, 2, 6, 4, 2, 28, 2, 28, 6, 2, 4, 6, 14, 4, 12, 14, 16, 14,
    4, 6, 8, 6, 4, 6, 6, 6, 8, 4, 8, 4, 14, 16, 8, 6, 4, 12, 8, 16,
    2, 10, 8, 4, 6, 26, 6, 10, 8, 4, 6, 12, 14, 30, 4, 14, 22, 8, 12, 4,
    6, 8, 10, 6, 14, 10, 6, 2, 10, 12, 12, 14, 6, 6, 18, 10, 6, 8, 18, 4,
    6, 2, 6, 10, 2, 10, 8, 6, 6, 10, 2, 18, 10, 2, 12, 4, 6, 8, 10, 12,
    14, 12, 4, 8, 10, 6, 6, 20, 4, 14, 16, 14, 10, 8, 10, 12, 2, 18, 6, 12,
    10, 12, 2, 4, 2, 12, 6, 4, 8, 4, 44, 4, 2, 4, 2, 10, 12, 6, 6, 14,
    4, 6, 6, 6, 8, 6, 36, 18, 4, 6, 2, 12, 6, 6, 6, 4, 14, 22, 12, 2,
    18, 10, 6, 26, 24, 4, 2, 4, 2, 4, 14, 4, 6, 6, 8, 16, 12, 2, 42, 4,
    2, 4, 24, 6, 6, 2, 18, 4, 14, 6, 28, 18, 14, 6, 10, 12, 2, 6, 12, 30,
    6, 4, 6, 6, 14, 4, 2, 24, 4, 6, 6, 26, 10, 18, 6, 8, 6, 6, 30, 4,
    12, 12, 2, 16, 2, 6, 4, 12, 18, 2, 6, 4, 26, 12, 6, 12, 4, 24, 24, 12,
    6, 2, 12, 28, 8, 4, 6, 12, 2, 18, 6, 4, 6, 6, 20, 16, 2, 6, 6, 18,
    10, 6, 2, 4, 8, 6, 6, 24, 16, 6, 8, 10, 6, 14, 22, 8, 16, 6, 2, 12,
    4, 2, 22, 8, 18, 34, 2, 6, 18, 4, 6, 6, 8, 10, 8, 18, 6, 4, 2, 4,
    8, 16, 2, 12, 12, 6, 18, 4, 6, 6, 6, 2, 6, 12, 10, 20, 12, 18, 4, 6,
    2, 16, 2, 10, 14, 4, 30, 2, 10, 12, 2, 24, 6, 16, 8, 10, 2, 12, 22, 6,
    2, 16, 20, 10, 2, 12, 12, 18, 10, 12, 6, 2, 10, 2, 6, 10, 18, 2, 12, 6,
    4, 6, 2, 24, 28, 2, 4, 2, 10, 2, 16, 12, 8, 22, 2, 6, 4, 2, 10, 6,
    20, 12, 10, 8, 12, 6, 6, 6, 4, 18, 2, 4, 12, 18, 2, 12, 6, 4, 2, 16,
    12, 12, 14, 4, 8, 18, 4, 12, 14, 6, 6, 4, 8, 6, 4, 20, 12, 10, 14, 4,
    2, 16, 2, 12, 30, 4, 6, 24, 20, 24, 10, 8, 12, 10, 12, 6, 12, 12, 6, 8,
    16, 14, 6, 4, 6, 36, 20, 10, 30, 12, 2, 4, 2, 28, 12, 14, 6, 22, 8, 4,
    18, 6, 14, 18, 4, 6, 2, 6, 34, 18, 2, 16, 6, 18, 2, 24, 4, 2, 6, 12,
    6, 12, 10, 8, 6, 16, 12, 8, 10, 14, 40, 6, 2, 6, 4, 12, 14, 4, 2, 4,
    2, 4, 8, 6, 10, 6, 6, 2, 6, 6, 6, 12, 6, 24, 10, 2, 10, 6, 12, 6,
    6, 14, 6, 6, 52, 20, 6, 10, 2, 10, 8, 10, 12, 12, 2, 6, 4, 14, 16, 8,
    12, 6, 22, 2, 10, 8, 6, 22, 2, 22, 6, 8, 10, 12, 12, 2, 10, 6, 12, 2,
    4, 14, 10, 2, 6, 18, 4, 12, 8, 18, 12, 6, 6, 4, 6, 6, 14, 4, 2, 12,
    12, 4, 6, 18, 18, 12, 2, 16, 12, 8, 18, 10, 26, 4, 6, 8, 6, 6, 4, 2,
    10, 20, 4, 6, 8, 4, 20, 10, 2, 34, 2, 4, 24, 2, 12, 12, 10, 6, 2, 12,
    30, 6, 12, 16, 12, 2, 22, 18, 12, 14, 10, 2, 12, 12, 4, 2, 4, 6, 12, 2,
    16, 18, 2, 40, 8, 16, 6, 8, 10, 2, 4, 18, 8, 10, 8, 12, 4, 18, 2, 18,
    10, 2, 4, 2, 4, 8, 28, 2, 6, 22, 12, 6, 14, 18, 4, 6, 8, 6, 6, 10,
    8, 4, 2, 18, 10, 6, 20, 22, 8, 6, 30, 4, 2, 4, 18, 6, 30, 2, 4, 8,
    6, 4, 6, 12, 14, 34, 14, 6, 4, 2, 6, 4, 14, 4, 2, 6, 28, 2, 4, 6,
    8, 10, 2, 10, 2, 10, 2, 4, 30, 2, 12, 12, 10, 18, 12, 14, 10, 2, 12, 6,
    10, 6, 14, 12, 4, 14, 4, 18, 2, 10, 8, 4, 8, 10, 12, 18, 18, 8, 6, 18,
    16, 14, 6, 6, 10, 14, 4, 6, 2, 12, 12, 4, 6, 6, 12, 2, 16, 2, 12, 6,
    4, 14, 6, 4, 2, 12, 18, 4, 36, 18, 12, 12, 2, 4, 2, 4, 8, 12, 4, 36,
    6, 18, 2, 12, 10, 6, 12, 24, 8, 6, 6, 16, 12, 2, 18, 10, 20, 10, 2, 6,
    18, 4, 2, 40, 6, 2, 16, 2, 4, 8, 18, 10, 12, 6, 2, 10, 8, 4, 6, 12,
    2, 10, 18, 8, 6, 4, 20, 4, 6, 36, 6, 2, 10, 6, 24, 6, 14, 16, 6, 18,
    2, 10, 20, 10, 8, 6, 4, 6, 2, 10, 2, 12, 4, 2, 4, 8, 10, 6, 12, 18,
    14, 12, 16, 8, 6, 16, 8, 4, 2, 6, 18, 24, 18, 10, 12, 2, 4, 14, 10, 6,
    6, 6, 18, 12, 2, 28, 18, 14, 16, 12, 14, 24, 12, 22, 6, 2, 10, 8, 4, 2,
    4, 14, 12, 6, 4, 6, 14, 4, 2, 4, 30, 6, 2, 6, 10, 2, 30, 22, 2, 4,
    6, 8, 6, 6, 16, 12, 12, 6, 8, 4, 2, 24, 12, 4, 6, 8, 6, 6, 10, 2,
    6, 12, 28, 14, 6, 4, 12, 8, 6, 12, 4, 6, 14, 6, 12, 10, 6, 6, 8, 6,
    6, 4, 2, 4, 8, 12, 4, 14, 18, 10, 2, 16, 6, 20, 6, 10, 8, 4, 30, 36,
    12, 8, 22, 12, 2, 6, 12, 16, 6, 6, 2, 18, 4, 26, 4, 8, 18, 10, 8, 10,
    6, 14, 4, 20, 22, 18, 12, 8, 28, 12, 6, 6, 8, 6, 12, 24, 16, 14, 4, 14,
    12, 6, 10, 12, 20, 6, 4, 8, 18, 12, 18, 10, 2, 4, 20, 10, 14, 4, 6, 2,
    10, 24, 18, 2, 4, 20, 16, 14, 10, 14, 6, 4, 6, 20, 6, 10, 6, 2, 12, 6,
    30, 10, 8, 6, 4, 6, 8, 40, 2, 4, 2, 12, 18, 4, 6, 8, 10, 6, 18, 18,
    2, 12, 16, 8, 6, 4, 6, 6, 2, 52, 14, 4, 20, 16, 2, 4, 6, 12, 2, 6,
    12, 12, 6, 4, 14, 10, 6, 6, 14, 10, 14, 16, 8, 6, 12, 4, 8, 22, 6, 2,
    18, 22, 6, 2, 18, 6, 16, 14, 10, 6, 12, 2, 6, 4, 8, 18, 12, 16, 2, 4,
    14, 4, 8, 12, 12, 30, 16, 8, 4, 2, 6, 22, 12, 8, 10, 6, 6, 6, 14, 6,
    18, 10, 12, 2, 10, 2, 4, 26, 4, 12, 8, 4, 18, 8, 10, 14, 16, 6, 6, 8,
    10, 6, 8, 6, 12, 10, 20, 10, 8, 4, 12, 26, 18, 4, 12, 18, 6, 30, 6, 8,
    6, 22, 12, 2, 4, 6, 6, 2, 10, 2, 4, 6, 6, 2, 6, 22, 18, 6, 18, 12,
    8, 12, 6, 10, 12, 2, 16, 2, 10, 2, 10, 18, 6, 20, 4, 2, 6, 22, 6, 6,
    18, 6, 14, 12, 16, 2, 6, 6, 4, 14, 12, 4, 2, 18, 16, 36, 12, 6, 14, 28,
    2, 12, 6, 12, 6, 4, 2, 16, 30, 8, 24, 6, 30, 10, 2, 18, 4, 6, 12, 8,
    22, 2, 6, 22, 18, 2, 10, 2, 10, 30, 2, 28, 6, 14, 16, 6, 20, 16, 2, 6,
    4, 32, 4, 2, 4, 6, 2, 12, 4, 6, 6, 12, 2, 6, 4, 6, 8, 6, 4, 20,
    4, 32, 10, 8, 16, 2, 22, 2, 4, 6, 8, 6, 16, 14, 4, 18, 8, 4, 20, 6,
    12, 12, 6, 10, 2, 10, 2, 12, 28, 12, 18, 2, 18, 10, 8, 10, 48, 2, 4, 6,
    8, 10, 2, 10, 30, 2, 36, 6, 10, 6, 2, 18, 4, 6, 8, 16, 14, 16, 6, 14,
    4, 20, 4, 6, 2, 10, 12, 2, 6, 12, 6, 6, 4, 12, 2, 6, 4, 12, 6, 8,
    4, 2, 6, 18, 10, 6, 8, 12, 6, 22, 2, 6, 12, 18, 4, 14, 6, 4, 20, 6,
    16, 8, 4, 8, 22, 8, 12, 6, 6, 16, 12, 18, 30, 8, 4, 2, 4, 6, 26, 4,
    14, 24, 22, 6, 2, 6, 10, 6, 14, 6, 6, 12, 10, 6, 2, 12, 10, 12, 8, 18,
    18, 10, 6, 8, 16, 6, 6, 8, 16, 20, 4, 2, 10, 2, 10, 12, 6, 8, 6, 10,
    20, 10, 18, 26, 4, 6, 30, 2, 4, 8, 6, 12, 12, 18, 4, 8, 22, 6, 2, 12,
    34, 6, 18, 12, 6, 2, 28, 14, 16, 14, 4, 14, 12, 4, 6, 6, 2, 36, 4, 6,
    20, 12, 24, 6, 22, 2, 16, 18, 12, 12, 18, 2, 6, 6, 6, 4, 6, 14, 4, 2,
    22, 8, 12, 6, 10, 6, 8, 12, 18, 12, 6, 10, 2, 22, 14, 6, 6, 4, 18, 6,
    20, 22, 2, 12, 24, 4, 18, 18, 2, 22, 2, 4, 12, 8, 12, 10, 14, 4, 2, 18,
    16, 38, 6, 6, 6, 12, 10, 6, 12, 8, 6, 4, 6, 14, 30, 6, 10, 8, 22, 6,
    8, 12, 10, 2, 10, 2, 6, 10, 2, 10, 12, 18, 20, 6, 4, 8, 22, 6, 6, 30,
    6, 14, 6, 12, 12, 6, 10, 2, 10, 30, 2, 16, 8, 4, 2, 6, 18, 4, 2, 6,
    4, 26, 4, 8, 6, 10, 2, 4, 6, 8, 4, 6, 30, 12, 2, 6, 6, 4, 20, 22,
    8, 4, 2, 4, 72, 8, 4, 8, 22, 2, 4, 14, 10, 2, 4, 20, 6, 10, 18, 6,
    20, 16, 6, 8, 6, 4, 20, 12, 22, 2, 4, 2, 12, 10, 18, 2, 22, 6, 18, 30,
    2, 10, 14, 10, 8, 16, 50, 6, 10, 8, 10, 12, 6, 18, 2, 22, 6, 2, 4, 6,
    8, 6, 6, 10, 18, 2, 22, 2, 16, 14, 10, 6, 2, 12, 10, 20, 4, 14, 6, 4,
    36, 2, 4, 6, 12, 2, 4, 14, 12, 6, 4, 6, 2, 6, 4, 20, 10, 2, 10, 6,
    12, 2, 24, 12, 12, 6, 6, 4, 24, 2, 4, 24, 2, 6, 4, 6, 8, 16, 6, 2,
    10, 12, 14, 6, 34, 6, 14, 6, 4, 2, 30, 22, 8, 4, 6, 8, 4, 2, 28, 2,
    6, 4, 26, 18, 22, 2, 6, 16, 6, 2, 16, 12, 2, 12, 4, 6, 6, 14, 10, 6,
    8, 12, 4, 18, 2, 10, 8, 16, 6, 6, 30, 2, 10, 18, 2, 10, 8, 4, 8, 12,
    24, 40, 2, 12, 10, 6, 12, 2, 12, 4, 2, 4, 6, 18, 14, 12, 6, 4, 14, 30,
    4, 8, 10, 8, 6, 10, 18, 8, 4, 14, 16, 6, 8, 4, 6, 2, 10, 2, 12, 4,
    2, 4, 6, 8, 4, 6, 32, 24, 10, 8, 18, 10, 2, 6, 10, 2, 4, 18, 6, 12,
    2, 16, 2, 22, 6, 6, 8, 18, 4, 18, 12, 8, 6, 4, 20, 6, 30, 22, 12, 2,
    6, 18, 4, 62, 4, 2, 12, 6, 10, 2, 12, 12, 28, 2, 4, 14, 22, 6, 2, 6,
    6, 10, 14, 4, 2, 10, 6, 8, 10, 14, 10, 6, 2, 12, 22, 18, 8, 10, 18, 12,
    2, 12, 4, 12, 2, 10, 2, 6, 18, 6, 6, 34, 6, 2, 12, 4, 6, 18, 18, 2,
    16, 6, 6, 8, 6, 10, 18, 8, 10, 8, 10, 2, 4, 18, 26, 12, 22, 2, 4, 2,
    22, 6, 6, 14, 16, 6, 20, 10, 12, 2, 18, 42, 4, 24, 2, 6, 10, 12, 2, 6,
    10, 8, 4, 6, 12, 12, 8, 4, 6, 12, 30, 20, 6, 24, 6, 10, 12, 2, 10, 20,
    6, 6, 4, 12, 14, 10, 18, 12, 8, 6, 12, 4, 14, 10, 2, 12, 30, 16, 2, 12,
    6, 4, 2, 4, 6, 26, 4, 18, 2, 4, 6, 14, 54, 6, 52, 2, 16, 6, 6, 12,
    26, 4, 2, 6, 22, 6, 2, 12, 12, 6, 10, 18, 2, 12, 12, 10, 18, 12, 6, 8,
    6, 10, 6, 8, 4, 2, 4, 20, 24, 6, 6, 10, 14, 10, 2, 22, 6, 14, 10, 26,
    4, 18, 8, 12, 12, 10, 12, 6, 8, 16, 6, 8, 6, 6, 22, 2, 10, 20, 10, 6,
    44, 18, 6, 10, 2, 4, 6, 14, 4, 26, 4, 2, 12, 10, 8, 4, 8, 12, 4, 12,
    8, 22, 8, 6, 10, 18, 6, 6, 8, 6, 12, 4, 8, 18, 10, 12, 6, 12, 2, 6,
    4, 2, 16, 12, 12, 14, 10, 14, 6, 10, 12, 2, 12, 6, 4, 6, 2, 12, 4, 26,
    6, 18, 6, 10, 6, 2, 18, 10, 8, 4, 26, 10, 20, 6, 16, 20, 12, 10, 8, 10,
    2, 16, 6, 20, 10, 20, 4, 30, 2, 4, 8, 16, 2, 18, 4, 2, 6, 10, 18, 12,
    14, 18, 6, 16, 20, 6, 4, 8, 6, 4, 6, 12, 8, 10, 2, 12, 6, 4, 2, 6,
    10, 2, 16, 12, 14, 10, 6, 8, 6, 28, 2, 6, 18, 30, 34, 2, 16, 12, 2, 18,
    16, 6, 8, 10, 8, 10, 8, 10, 44, 6, 6, 4, 20, 4, 2, 4, 14, 28, 8, 6,
    16, 14, 30, 6, 30, 4, 14, 10, 6, 6, 8, 4, 18, 12, 6, 2, 22, 12, 8, 6,
    12, 4, 14, 4, 6, 2, 4, 18, 20, 6, 16, 38, 16, 2, 4, 6, 2, 40, 42, 14,
    4, 6, 2, 24, 10, 6, 2, 18, 10, 12, 2, 16, 2, 6, 16, 6, 8, 4, 2, 10,
    6, 8, 10, 2, 18, 16, 8, 12, 18, 12, 6, 12, 10, 6, 6, 18, 12, 14, 4, 2,
    10, 20, 6, 12, 6, 16, 26, 4, 18, 2, 4, 32, 10, 8, 6, 4, 6, 6, 14, 6,
    18, 4, 2, 18, 10, 8, 10, 8, 10, 2, 4, 6, 2, 10, 42, 8, 12, 4, 6, 18,
    2, 16, 8, 4, 2, 10, 14, 12, 10, 20, 4, 8, 10, 38, 4, 6, 2, 10, 20, 10,
    12, 6, 12, 26, 12, 4, 8, 28, 8, 4, 8, 24, 6, 10, 8, 6, 16, 12, 8, 10,
    12, 8, 22, 6, 2, 10, 2, 6, 10, 6, 6, 8, 6, 4, 14, 28, 8, 16, 18, 8,
    4, 6, 20, 4, 18, 6, 2, 24, 24, 6, 6, 12, 12, 4, 2, 22, 2, 10, 6, 8,
    12, 4, 20, 18, 6, 4, 12, 24, 6, 6, 54, 8, 6, 4, 26, 36, 4, 2, 4, 26,
    12, 12, 4, 6, 6, 8, 12, 10, 2, 12, 16, 18, 6, 8, 6, 12, 18, 10, 2, 54,
    4, 2, 10, 30, 12, 8, 4, 8, 16, 14, 12, 6, 4, 6, 12, 6, 2, 4, 14, 12,
    4, 14, 6, 24, 6, 6, 10, 12, 12, 20, 18, 6, 6, 16, 8, 4, 6, 20, 4, 32,
    4, 14, 10, 2, 6, 12, 16, 2, 4, 6, 12, 2, 10, 8, 6, 4, 2, 10, 14, 6,
    6, 12, 18, 34, 8, 10, 6, 24, 6, 2, 10, 12, 2, 30, 10, 14, 12, 12, 16, 6,
    6, 2, 18, 4, 6, 30, 14, 4, 6, 6, 2, 6, 4, 6, 14, 6, 4, 8, 10, 12,
    6, 32, 10, 8, 22, 2, 10, 6, 24, 8, 4, 30, 6, 2, 12, 16, 8, 6, 4, 6,
    8, 16, 14, 6, 6, 4, 2, 10, 12, 2, 16, 14, 4, 2, 4, 20, 18, 10, 2, 10,
    6, 12, 30, 8, 18, 12, 10, 2, 6, 6, 4, 12, 12, 2, 4, 12, 18, 24, 2, 10,
    6, 8, 16, 8, 6, 12, 10, 14, 6, 12, 6, 6, 4, 2, 24, 4, 6, 8, 6, 4,
    2, 4, 6, 14, 4, 8, 10, 24, 24, 12, 2, 6, 12, 22, 30, 2, 6, 18, 10, 6,
    6, 8, 4, 2, 6, 10, 8, 10, 6, 8, 16, 6, 14, 6, 4, 24, 8, 10, 2, 12,
    6, 4, 36, 2, 22, 6, 8, 6, 10, 8, 6, 12, 10, 14, 10, 6, 18, 12, 2, 12,
    4, 26, 10, 14, 16, 18, 8, 18, 12, 12, 6, 16, 14, 24, 10, 12, 8, 22, 6, 2,
    10, 60, 6, 2, 4, 8, 16, 14, 10, 6, 24, 6, 12, 18, 24, 2, 30, 4, 2, 12,
    6, 10, 2, 4, 14, 6, 16, 2, 10, 8, 22, 20, 6, 4, 32, 6, 18, 4, 2, 4,
    2, 4, 8, 52, 14, 22, 2, 22, 20, 10, 8, 10, 2, 6, 4, 14, 4, 6, 20, 4,
    6, 2, 12, 12, 6, 12, 16, 2, 12, 10, 8, 4, 6, 2, 28, 12, 8, 10, 12, 2,
    4, 14, 28, 8, 6, 4, 2, 4, 6, 2, 12, 58, 6, 14, 10, 2, 6, 28, 32, 4,
    30, 8, 6, 4, 6, 12, 12, 2, 4, 6, 6, 14, 16, 8, 30, 4, 2, 10, 8, 6,
    4, 6, 26, 4, 12, 2, 10, 18, 12, 12, 18, 2, 4, 12, 8, 12, 10, 20, 4, 8,
    16, 12, 8, 6, 16, 8, 10, 12, 14, 6, 4, 8, 12, 4, 20, 6, 40, 8, 16, 6,
    36, 2, 6, 4, 6, 2, 22, 18, 2, 10, 6, 36, 14, 12, 4, 18, 8, 4, 14, 10,
    2, 10, 8, 4, 2, 18, 16, 12, 14, 10, 14, 6, 6, 42, 10, 6, 6, 20, 10, 8,
    12, 4, 12, 18, 2, 10, 14, 18, 10, 18, 8, 6, 4, 14, 6, 10, 30, 14, 6, 6,
    4, 12, 38, 4, 2, 4, 6, 8, 12, 10, 6, 18, 6, 50, 6, 4, 6, 12, 8, 10,
    32, 6, 22, 2, 10, 12, 18, 2, 6, 4, 30, 8, 6, 6, 18, 10, 2, 4, 12, 20,
    10, 8, 24, 10, 2, 6, 22, 6, 2, 18, 10, 12, 2, 30, 18, 12, 28, 2, 6, 4,
    6, 14, 6, 12, 10, 8, 4, 12, 26, 10, 8, 6, 16, 2, 10, 18, 14, 6, 4, 6,
    14, 16, 2, 6, 4, 12, 20, 4, 20, 4, 6, 12, 2, 36, 4, 6, 2, 10, 2, 22,
    8, 6, 10, 12, 12, 18, 14, 24, 36, 4, 20, 24, 10, 6, 2, 28, 6, 18, 8, 4,
    6, 8, 6, 4, 2, 12, 28, 18, 14, 16, 14, 18, 10, 8, 6, 4, 6, 6, 8, 22,
    12, 2, 10, 18, 6, 2, 18, 10, 2, 12, 10, 18, 32, 6, 4, 6, 6, 8, 6, 6,
    10, 20, 6, 12, 10, 8, 10, 14, 6, 10, 14, 4, 2, 22, 18, 2, 10, 2, 4, 20,
    4, 2, 34, 2, 12, 6, 10, 2, 10, 18, 6, 14, 12, 12, 22, 8, 6, 16, 6, 8,
    4, 12, 6, 8, 4, 36, 6, 6, 20, 24, 6, 12, 18, 10, 2, 10, 26, 6, 16, 8,
    6, 4, 24, 18, 8, 12, 12, 10, 18, 12, 2, 24, 4, 12, 18, 12, 14, 10, 2, 4,
    24, 12, 14, 10, 6, 2, 6, 4, 6, 26, 4, 6, 6, 2, 22, 8, 18, 4, 18, 8,
    4, 24, 2, 12, 12, 4, 2, 52, 2, 18, 6, 4, 6, 12, 2, 6, 12, 10, 8, 4,
    2, 24, 10, 2, 10, 2, 12, 6, 18, 40, 6, 20, 16, 2, 12, 6, 10, 12, 2, 4,
    6, 14, 12, 12, 22, 6, 8, 4, 2, 16, 18, 12, 2, 6, 16, 6, 2, 6, 4, 12,
    30, 8, 16, 2, 18, 10, 24, 2, 6, 24, 4, 2, 22, 2, 16, 2, 6, 12, 4, 18,
    8, 4, 14, 4, 18, 24, 6, 2, 6, 10, 2, 10, 38, 6, 10, 14, 6, 6, 24, 4,
    2, 12, 16, 14, 16, 12, 2, 6, 10, 26, 4, 2, 12, 6, 4, 12, 8, 12, 10, 18,
    6, 14, 28, 2, 6, 10, 2, 4, 14, 34, 2, 6, 22, 2, 10, 14, 4, 2, 16, 8,
    10, 6, 8, 10, 8, 4, 6, 2, 16, 6, 6, 18, 30, 14, 6, 4, 30, 2, 10, 14,
    4, 20, 10, 8, 4, 8, 18, 4, 14, 6, 4, 24, 6, 6, 18, 18, 2, 36, 6, 10,
    14, 12, 4, 6, 2, 30, 6, 4, 2, 6, 28, 20, 4, 20, 12, 24, 16, 18, 12, 14,
    6, 4, 12, 32, 12, 6, 10, 8, 10, 6, 18, 2, 16, 14, 6, 22, 6, 12, 2, 18,
    4, 8, 30, 12, 4, 12, 2, 10, 38, 22, 2, 4, 14, 6, 12, 24, 4, 2, 4, 14,
    12, 10, 2, 16, 6, 20, 4, 20, 22, 12, 2, 4, 2, 12, 22, 24, 6, 6, 2, 6,
    4, 6, 2, 10, 12, 12, 6, 2, 6, 16, 8, 6, 4, 18, 12, 12, 14, 4, 12, 6,
    8, 6, 18, 6, 10, 12, 14, 6, 4, 8, 22, 6, 2, 28, 18, 2, 18, 10, 6, 14,
    10, 2, 10, 14, 6, 10, 2, 22, 6, 8, 6, 16, 12, 8, 22, 2, 4, 14, 18, 12,
    6, 24, 6, 10, 2, 12, 22, 18, 6, 20, 6, 10, 14, 4, 2, 6, 12, 22, 14, 12,
    4, 6, 8, 22, 2, 10, 12, 8, 40, 2, 6, 10, 8, 4, 42, 20, 4, 32, 12, 10,
    6, 12, 12, 2, 10, 8, 6, 4, 8, 4, 26, 18, 4, 8, 28, 6, 18, 6, 12, 2,
    10, 6, 6, 14, 10, 12, 14, 24, 6, 4, 20, 22, 2, 18, 4, 6, 12, 2, 16, 18,
    14, 6, 6, 4, 6, 8, 18, 4, 14, 30, 4, 18, 8, 10, 2, 4, 8, 12, 4, 12,
    18, 2, 12, 10, 2, 16, 8, 4, 30, 2, 6, 28, 2, 10, 2, 18, 10, 14, 4, 26,
    6, 18, 4, 20, 6, 4, 8, 18, 4, 12, 26, 24, 4, 20, 22, 2, 18, 22, 2, 4,
    12, 2, 6, 6, 6, 4, 6, 14, 4, 24, 12, 6, 18, 2, 12, 28, 14, 4, 6, 8,
    22, 6, 12, 18, 8, 4, 20, 6, 4, 6, 2, 18, 6, 4, 12, 12, 8, 28, 6, 8,
    10, 2, 24, 12, 10, 24, 8, 10, 20, 12, 6, 12, 12, 4, 14, 12, 24, 34, 18, 8,
    10, 6, 18, 8, 4, 8, 16, 14, 6, 4, 6, 24, 2, 6, 4, 6, 2, 16, 6, 6,
    20, 24, 4, 2, 4, 14, 4, 18, 2, 6, 12, 4, 14, 4, 2, 18, 16, 6, 6, 2,
    16, 20, 6, 6, 30, 4, 8, 6, 24, 16, 6, 6, 8, 12, 30, 4, 18, 18, 8, 4,
    26, 10, 2, 22, 8, 10, 14, 6, 4, 18, 8, 12, 28, 2, 6, 4, 12, 6, 24, 6,
    8, 10, 20, 16, 8, 30, 6, 6, 4, 2, 10, 14, 6, 10, 32, 22, 18, 2, 4, 2,
    4, 8, 22, 8, 18, 12, 28, 2, 16, 12, 18, 14, 10, 18, 12, 6, 32, 10, 14, 6,
    10, 2, 10, 2, 6, 22, 2, 4, 6, 8, 10, 6, 14, 6, 4, 12, 30, 24, 6, 6,
    8, 6, 4, 2, 4, 6, 8, 6, 6, 22, 18, 8, 4, 2, 18, 6, 4, 2, 16, 18,
    20, 10, 6, 6, 30, 2, 12, 28, 6, 6, 6, 2, 12, 10, 8, 18, 18, 4, 8, 18,
    10, 2, 28, 2, 10, 14, 4, 2, 30, 12, 22, 26, 10, 8, 6, 10, 8, 16, 14, 6,
    6, 10, 14, 6, 4, 2, 10, 12, 2, 6, 10, 8, 4, 2, 10, 26, 22, 6, 2, 12,
    18, 4, 26, 4, 8, 10, 6, 14, 10, 2, 18, 6, 10, 20, 6, 6, 4, 24, 2, 4,
    8, 6, 16, 14, 16, 18, 2, 4, 12, 2, 10, 2, 6, 12, 10, 6, 6, 20, 6, 4,
    6, 38, 4, 6, 12, 14, 4, 12, 8, 10, 12, 12, 8, 4, 6, 14, 10, 6, 12, 2,
    10, 18, 2, 18, 10, 8, 10, 2, 12, 4, 14, 28, 2, 16, 2, 18, 6, 10, 6, 8,
    16, 14, 30, 10, 20, 6, 10, 24, 2, 28, 2, 12, 16, 6, 8, 36, 4, 8, 4, 14,
    12, 10, 8, 12, 4, 6, 8, 4, 6, 14, 22, 8, 6, 4, 2, 10, 6, 20, 10, 8,
    6, 6, 22, 18, 2, 16, 6, 20, 4, 26, 4, 14, 22, 14, 4, 12, 6, 8, 4, 6,
    6, 26, 10, 2, 18, 18, 4, 2, 16, 2, 18, 4, 6, 8, 4, 6, 12, 2, 6, 6,
    28, 38, 4, 8, 16, 26, 4, 2, 10, 12, 2, 10, 8, 6, 10, 12, 2, 10, 2, 24,
    4, 30, 26, 6, 6, 18, 6, 6, 22, 2, 10, 18, 26, 4, 18, 8, 6, 6, 12, 16,
    6, 8, 16, 6, 8, 16, 2, 42, 58, 8, 4, 6, 2, 4, 8, 16, 6, 20, 4, 12,
    12, 6, 12, 2, 10, 2, 6, 22, 2, 10, 6, 8, 6, 10, 14, 6, 6, 4, 18, 8,
    10, 8, 16, 14, 10, 2, 10, 2, 12, 6, 4, 20, 10, 8, 52, 8, 10, 6, 2, 10,
    8, 10, 6, 6, 8, 10, 2, 22, 2, 4, 6, 14, 4, 2, 24, 12, 4, 26, 18, 4,
    6, 14, 30, 6, 4, 6, 2, 22, 8, 4, 6, 2, 22, 6, 8, 16, 6, 14, 4, 6,
    18, 8, 12, 6, 12, 24, 30, 16, 8, 34, 8, 22, 6, 14, 10, 18, 14, 4, 12, 8,
    4, 36, 6, 6, 2, 10, 2, 4, 20, 6, 6, 10, 12, 6, 2, 40, 8, 6, 28, 6,
    2, 12, 18, 4, 24, 14, 6, 6, 10, 20, 10, 14, 16, 14, 16, 6, 8, 36, 4, 12,
    12, 6, 12, 50, 12, 6, 4, 6, 6, 8, 6, 10, 2, 10, 2, 18, 10, 14, 16, 8,
    6, 4, 20, 4, 2, 10, 6, 14, 18, 10, 38, 10, 18, 2, 10, 2, 12, 4, 2, 4,
    14, 6, 10, 8, 40, 6, 20, 4, 12, 8, 6, 34, 8, 22, 8, 12, 10, 2, 16, 42,
    12, 8, 22, 8, 22, 8, 6, 34, 2, 6, 4, 14, 6, 16, 2, 22, 6, 8, 24, 22,
    6, 2, 12, 4, 6, 14, 4, 8, 24, 4, 6, 6, 2, 22, 20, 6, 4, 14, 4, 6,
    6, 8, 6, 10, 6, 8, 6, 16, 14, 6, 6, 22, 6, 24, 32, 6, 18, 6, 18, 10,
    8, 30, 18, 6, 16, 12, 6, 12, 2, 6, 4, 12, 8, 6, 22, 8, 6, 4, 14, 10,
    18, 20, 10, 2, 6, 4, 2, 28, 18, 2, 10, 6, 6, 6, 14, 40, 24, 2, 4, 8,
    12, 4, 20, 4, 32, 18, 16, 6, 36, 8, 6, 4, 6, 14, 4, 6, 26, 6, 10, 14,
    18, 10, 6, 6, 14, 10, 6, 6, 14, 6, 24, 4, 14, 22, 8, 12, 10, 8, 12, 18,
    10, 18, 8, 24, 10, 8, 4, 24, 6, 18, 6, 2, 10, 30, 2, 10, 2, 4, 2, 40,
    2, 28, 8, 6, 6, 18, 6, 10, 14, 4, 18, 30, 18, 2, 12, 30, 6, 30, 4, 18,
    12, 2, 4, 14, 6, 10, 6, 8, 6, 10, 12, 2, 6, 12, 10, 2, 18, 4, 20, 4,
    6, 14, 6, 6, 22, 6, 6, 8, 18, 18, 10, 2, 10, 2, 6, 4, 6, 12, 18, 2,
    10, 8, 4, 18, 2, 6, 6, 6, 10, 8, 10, 6, 18, 12, 8, 12, 6, 4, 6, 14,
    16, 2, 12, 4, 6, 38, 6, 6, 16, 20, 28, 20, 10, 6, 6, 14, 4, 26, 4, 14,
    10, 18, 14, 28, 2, 4, 14, 16, 2, 28, 6, 8, 6, 34, 8, 4, 18, 2, 16, 8,
    6, 40, 8, 18, 4, 30, 6, 12, 2, 30, 6, 10, 14, 40, 14, 10, 2, 12, 10, 8,
    4, 8, 6, 6, 28, 2, 4, 12, 14, 16, 8, 30, 16, 18, 2, 10, 18, 6, 32, 4,
    18, 6, 2, 12, 10, 18, 2, 6, 10, 14, 18, 28, 6, 8, 16, 2, 4, 20, 10, 8,
    18, 10, 2, 10, 8, 4, 6, 12, 6, 20, 4, 2, 6, 4, 20, 10, 26, 18, 10, 2,
    18, 6, 16, 14, 4, 26, 4, 14, 10, 12, 14, 6, 6, 4, 14, 10, 2, 30, 18, 22, 2};

const largeint safeprime(const USHORT l, const unsigned int numsprimes)
{
  largeint min((USHORT)0);
  largeint max((USHORT)0);
  largeint zero((USHORT)0);
  largeint one((USHORT)1);
  largeint q, t, qmin1div2, p;
  USHORT r, rmin1div2, prim = 0;
  unsigned int i;

  if (l == 0)
    largeint::panic(largeint_VAL, "safeprime", 1, __LINE__);
  if (l > BIGINTMAXBIT)
    largeint::panic(largeint_OFL, "safeprime", 1, __LINE__);
  min.setbit(l - 2);
  max.setbit(l - 1);
  max = max - 1;
  t = max - min;
  q = rand64(l - 1);
  q.setbit(0);
  do
    {
      r = 2;
      i = 1;
      do
	{
	  r = r + smallprim[i];
	  rmin1div2 = ((r - 1) >> 1);
	  i = i + 1;
	}
      while ((mod(q - rmin1div2, r) != 0) && (mod(q, r) != 0) && (mod(q - 1, r) != 0) && (i < numsprimes));
      if (i >= numsprimes)
	{
	  if (MRprime(q, 0, 5) == 1)
	    {
	      qmin1div2 = (q - 1) >> 1;
	      if (MRprime(qmin1div2, 302, 5) == 1)
		{
		  prim = 1;
		  return q;
		}
	      p = (q << 1) + 1;
	      if (MRprime(p, 302, 5) == 1)
		{
		  prim = 1;
		  return p;
		}
	    }
	}
      q = q + 2;
      while (q > max)
	{
	  q = min + q % (t + 1);
	  q.setbit(0);
	}
    }
  while (prim == 0);
  return zero;
}

const largeint extendprime(const largeint& pmin, const largeint& pmax, const largeint& q)
{
  largeint p = rand64(pmin, pmax);
  largeint twotimesq = (q << 1);
  largeint t = pmax - pmin;
  largeint w;
  largeint one = largeint((USHORT)1);

  if (pmin.init == 0)
    largeint::panic (largeint_VAL, "extendprime", 1, __LINE__);
  if (pmax.init == 0)
    largeint::panic (largeint_VAL, "extendprime", 2, __LINE__);
  if (q.init == 0)
    largeint::panic (largeint_VAL, "extendprime", 3, __LINE__);
  if (pmin > pmax)
    largeint::panic (largeint_VAL, "extendprime", 1, __LINE__);
  if (q.iseven())
    largeint::panic (largeint_VAL, "extendprime", 3, __LINE__);

  if (p < pmin)
    p = p + pmin;
  w = p % twotimesq;

  p = p + (twotimesq - w) + one;
  if (p.iseven ())
      p = p + q;
  while (p > pmax)
    {
      p = pmin + p % (t + 1);
      w = p % twotimesq;
      p = p + (twotimesq - w) + one;
      if (p.iseven ())
	p = p + q;
    }
  while (p.MRprime(302, 5) == 0)
    {
      p = p + twotimesq;
      while (p > pmax)
        {
          p = p + usrandBBS_l();
          p = pmin + p % (t + 1);
          w = p % twotimesq;
          p = p + (twotimesq - w) + one;
          if (p.iseven())
	    p = p + q;
        }
    }

  return p;
}

const largeint extendprime(const USHORT l, const largeint& q)
{
  if (l > BIGINTMAXBIT)
    largeint::panic(largeint_VAL, "extendprime", 1, __LINE__);

  largeint pmin = largeint((USHORT)0).setbit (l - 1);
  largeint pmax = largeint((USHORT)0).setbit (l);
  pmax = pmax - 1;

  return (extendprime (pmin, pmax, q));
}

const largeint primeprimelt(const USHORT l, largeint& g)
{
  largeint p, q, pmin1;
  largeint **primes = new largeint *[3];
  
  if (l == 0)
    largeint::panic(largeint_VAL, "primeprimelt", 1, __LINE__);
  if (l > BIGINTMAXBIT)
    largeint::panic(largeint_OFL, "primeprimelt", 1, __LINE__);
  p = safeprime(l, 1000);
  pmin1 = p - 1;
  q = pmin1 >> 1;
  primes[0] = &pmin1;
  primes[1] = &q;
  largeint two = largeint((USHORT)2);
  primes[2] = &two;
  g = primelt(2, primes);
  delete [] primes;
  return p;
}

static void m_out_of_n(UCHAR *array, int m, int n)
{
    int i=0, i1=0, j=0, jp=0,  j1=0, k1=0, k2=0;

    if(!m || m >= n)
	return;

    if(m == 1)
      {
	for(i=0; i < n; i++)
	    if(array[i])
	      {
		array[i++] = 0;
		if(i >= n)
		  i = 0;
		array[i] = 1;
		return;
	      }
      }
    for(j=1; j < n; j++)
      {
	if(array[n-1] == array[n-j-1])
	  continue;
	j1 = j;
	break;
      }
    if(m & 1) 
      {
	if(array[n-1])
	  {
	    if(j1 & 1) 
	      {
		k1 = n - j1;
		k2 = k1+2;
		if(k2 > n)
		  k2 = n;
		goto leave;
	      }
	    goto scan;
	  }
	k2 = n - j1 - 1;
	if(k2 == 0)
	  {
	    k1 = i;
	    k2 = n - j1;
	  }
	else if(array[k2] && array[k2-1])
	  k1 = n;
	else
	  k1 = k2 + 1;
    }
    else 
      {      
	if(!array[n-1]) 
	  {
	    k1 = n - j1;
	    k2 = k1 + 1;
	    goto leave;
	  }
	if(!(j1 & 1)) 
	  {
	    k1 = n - j1;
	    k2 = k1+2;
	    if(k2 > n)
	      k2 = n;
	    goto leave;
	  }
      scan:
	jp = n - j1 - 1;
	for(i=1; i <= jp; i++)
	  {
	    i1 = jp + 2 - i;
	    if(array[i1-1]) 
	      {
		if(array[i1-2])
		  {
		    k1 = i1 - 1;
		    k2 = n - j1;
		  }
		else
		  {
		    k1 = i1 - 1;
		    k2 = n + 1 - j1;
		  }
		goto leave;
	      }
	  }
	k1 = 1;
	k2 = n + 1 - m;
      }
 leave:
    array[k1-1] = !array[k1-1];
    array[k2-1] = !array[k2-1];
}

const largeint elgamalprime(const USHORT pbits, const USHORT qbits, largeint& g)
{
  largeint p, q, pmin1, temp;
  largeint zero = largeint((USHORT)0);
  largeint two = largeint((USHORT)2);
  USHORT l, new_qbits, req_qbits = qbits;
  UCHAR *perms = NULL;
  int n, m;
  int i, j;
  int count1, count2;

  for(n = 1; (pbits - qbits - 1) / n  >= qbits; n++)
    ;
  n = n - 1;
  l = (pbits - req_qbits - 1) / n;
  new_qbits = pbits - n * l;
  q = prime64(new_qbits);
  largeint *p_i = new largeint[n];
  m = 3 * n + 5;
  if (m < 25)
    m = 25;
  largeint *pool = new largeint[m];
  count1 = 0;
  count2 = 0;
  do
    {
    next_try:
      if (!perms)
	{
	  perms = new UCHAR[m];
	  for (i = 0; i < m; i++)
	    {
	      do
		temp = prime64(l);
	      while (temp == largeint((USHORT)0));
	      pool[i] = temp;
	      perms[i] = 0;
	    }
	  for (i = 0; i < n; i++)
	    {
	      perms[i] = 1;
	      p_i[i] = pool[i];
	    }
	}
      else
	{
	  m_out_of_n(perms, n, m);
	  for(i = j = 0; (i < m) && (j < n); i++)
	    if(perms[i])
	      {
		p_i[j] = pool[i];
		j = j + 1;
	      }
	}
      p = (q << 1);
      for (i = 0; i < n; i++)
	p = mul(p, p_i[i]);
      p = p + 1;
      if (p.numbits() < pbits)
	{
	  count1 = count1 + 1;
	  if (count1 > 100)
	    {
	      count1 = 0;
	      new_qbits = new_qbits + 1;
	      q = prime64(new_qbits);
	      goto next_try;
	    }
	}
      //else
	//count1 = 0;
      if (p.numbits() > pbits)
	{
	  count2 = count2 + 1;
	  if (count2 > 100)
	    {
	      count2 = 0;
	      new_qbits = new_qbits - 1;
	      q = prime64(new_qbits);
	      goto next_try;
	    }
	}
      //else
	//count2 = 0;
    }
  while ((p.numbits() != pbits) || (p.MRprime(302, 5) == 0));
  largeint **primes = new largeint* [n + 3];
  pmin1 = p - 1;
  primes[0] = &pmin1;
  primes[1] = &two;
  primes[2] = &q;
  for (i = 0;  i < n; i++)
    primes[i + 3] = &p_i[i];
  do
    g = primelt(n + 2, primes);
  while (eqmod(pmin1, zero, g) == 1);
  delete [] perms;
  delete [] pool;
  delete [] p_i;
  delete [] primes;
  return p;
}

// Fonctions amies de conversion

char* largeint2str(const largeint& a, const USHORT base, const int showbase)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "largeint2str", 1, __LINE__);
  return (bigint2str_l(a.n_l, base, showbase));
}

UCHAR* largeint2byte(const largeint& a, int* l)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "largeint2byte", 1, __LINE__);
  return (bigint2byte_l(a.n_l, l));
}

// Fonction amie de purge

const void purge(largeint& a)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "purge", 1, __LINE__);
  purge_l(a.n_l);
  setdigits_l(a.n_l, 0);
}

/* Op�rateurs surcharg�s */
/*************************/

// Op�rateurs de comparaison surcharg�s

const int operator==(const largeint& a, const largeint& b)
{
  int eq;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "==", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "==", 2, __LINE__);
  if (&a == &b)
    return 1;
  else
    {
      eq = equ_l(a.n_l, b.n_l);
      return eq;
    }
}

const int operator!=(const largeint& a, const largeint& b)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "!=", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "!=", 2, __LINE__);
  if (&a == &b)
    return 0;
  if (equ_l(a.n_l, b.n_l) == 0)
    return 1;
  else
    return 0;
}

const int operator<(const largeint& a, const largeint& b)
{
  int cmp;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "<", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "<", 2, __LINE__);
  if (&a == &b)
    return 0;
  else
    {
      cmp = comp_l(a.n_l, b.n_l);
      if (cmp == -1)
	return 1;
      else
	return 0;
    }
}

const int operator>(const largeint& a, const largeint& b)
{
  int cmp;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, ">", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, ">", 2, __LINE__);
  if (&a == &b)
    return 0;
  else
    {
      cmp = comp_l(a.n_l, b.n_l);
      if (cmp == 1)
	return 1;
      else
	return 0;
    }
}

const int operator<=(const largeint& a, const largeint& b)
{
  int cmp;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "<=", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "<=", 2, __LINE__);
  if (&a == &b)
    return 1;
  cmp = comp_l(a.n_l, b.n_l);
  if ((cmp == -1) || (cmp == 0))
    return 1;
  else
    return 0;
}

const int operator>=(const largeint& a, const largeint& b)
{
  int cmp;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, ">=", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, ">=", 2, __LINE__);
  if (&a == &b)
    return 1;
  cmp = comp_l(a.n_l, b.n_l);
  if ((cmp == 1) || (cmp == 0))
    return 1;
  else
    return 0;
}

// Op�rateurs arithm�tiques surcharg�s

const largeint operator+(const largeint& a, const largeint& b)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "+", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "+", 2, __LINE__);
  err = add_l(a.n_l, b.n_l, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "+", err, __LINE__);
    }
  return sum;
}

const largeint operator+(const largeint& a, const USHORT b)
{
  largeint sum;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "+", 1, __LINE__);
  err = uadd_l(a.n_l, b, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "+", err, __LINE__);
    }
  return sum;
}

const largeint operator+(const USHORT a, const largeint& b)
{
  largeint sum;
  int err;

  if (b.init == 0)
    largeint::panic(largeint_VAL, "+", 1, __LINE__);
  err = uadd_l(b.n_l, a, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = largeint_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "+", err, __LINE__);
    }
  return sum;
}

const largeint operator-(const largeint& a, const largeint& b)
{
  largeint diff;
  int err;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "-", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "-", 2, __LINE__);
  err = sub_l(a.n_l, b.n_l, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "-", err, __LINE__);
    }
  return diff;
}

const largeint operator-(const largeint& a, const USHORT b)
{
  largeint diff;
  int err;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "-", 1, __LINE__);
  err = usub_l(a.n_l, b, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = largeint_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "-", err, __LINE__);
    }
  return diff;
}

const largeint operator*(const largeint& a, const largeint& b)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "*", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "*", 2, __LINE__);
  if (&a == &b)
    err = sqr_l(a.n_l, prod.n_l);
  else
    err = mul_l(a.n_l, b.n_l, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "*", err, __LINE__);
    }
  return prod;
}

const largeint operator*(const largeint& a, const USHORT b)
{
  largeint prod;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "*", 1, __LINE__);
  err = umul_l(a.n_l, b, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "*", err, __LINE__);
    }
  return prod;
}

const largeint operator*(const USHORT a, const largeint& b)
{
  largeint prod;
  int err;

  if (b.init == 0)
    largeint::panic(largeint_VAL, "*", 1, __LINE__);
  err = umul_l(b.n_l, a, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = largeint_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = largeint_OFL;
      break;
    default:
      largeint::panic(largeint_ERR, "*", err, __LINE__);
    }
  return prod;
}

const largeint operator/(const largeint& a, const largeint& b)
{
  largeint quot;
  BIGINT dummy_l;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "/", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "/", 2, __LINE__);
  err = div_l(a.n_l, b.n_l, quot.n_l, dummy_l);
  switch (err)
    {
    case BIGINT_OK:
      quot.init = 1;
      quot.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "/", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "/", err, __LINE__);
    }
  setzero_l(dummy_l);
  return quot;
}

const largeint operator/(const largeint& a, const USHORT b)
{
  largeint quot;
  BIGINT dummy_l;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "/", 1, __LINE__);
  err = udiv_l(a.n_l, b, quot.n_l, dummy_l);
  switch (err)
    {
    case BIGINT_OK:
      quot.init = 1;
      quot.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "/", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_ERR, "/", err, __LINE__);
    }
  setzero_l(dummy_l);
  return quot;
}

const largeint operator%(const largeint& a, const largeint& b)
{
  largeint rem;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "%", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "%", 2, __LINE__);
  err = mod_l(a.n_l, b.n_l, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = largeint_OK;
      break;
    case BIGINT_DBZ:
      largeint::panic(largeint_DBZ, "%", 2, __LINE__);
      break;
    default:
      largeint::panic(largeint_DBZ, "%", err, __LINE__);
    }
  return rem;
}

const USHORT operator%(const largeint& a, const USHORT b)
{
  int rem;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "%", 1, __LINE__);
  rem = umod_l(a.n_l, b);
  if (rem == 0xffff)
    largeint::panic(largeint_DBZ, "%", 2, __LINE__);
  return rem;
}

// Op�rateurs bool�ens surcharg�s

const largeint operator&(const largeint& a, const largeint& b)
{
  largeint et;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "&", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "&", 2, __LINE__);
  and_l(a.n_l, b.n_l, et.n_l);
  et.init = 1;
  et.status = largeint_OK;
  return et;
}

const largeint operator|(const largeint& a, const largeint& b)
{
  largeint ou;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "|", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "|", 2, __LINE__);
  or_l(a.n_l, b.n_l, ou.n_l);
  ou.init = 1;
  ou.status = largeint_OK;
  return ou;
}

const largeint operator^(const largeint& a, const largeint& b)
{
  largeint orx;
  
  if (a.init == 0)
    largeint::panic(largeint_VAL, "^", 1, __LINE__);
  if (b.init == 0)
    largeint::panic(largeint_VAL, "^", 2, __LINE__);
  xor_l(a.n_l, b.n_l, orx.n_l);
  orx.init = 1;
  orx.status = largeint_OK;
  return orx;
}

const largeint operator<<(const largeint& a, const int nbrebits)
{
  largeint shft = a;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "<<", 1, __LINE__);
  err = shift_l(shft.n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shft.init = 1;
      shft.status = largeint_OK;
      break;
    case BIGINT_OFL:
      shft.init = 1;
      shft.status = largeint_OFL;
      break;
    case BIGINT_UFL:
      shft.init = 1;
      shft.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, "<<", err, __LINE__);
    }
  return shft;
}

const largeint operator>>(const largeint& a, const int nbrebits)
{
  largeint shft = a;
  int err;

  if (a.init == 0)
    largeint::panic(largeint_VAL, ">>", 1, __LINE__);
  err = shift_l(shft.n_l, -nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shft.init = 1;
      shft.status = largeint_OK;
      break;
    case BIGINT_OFL:
      shft.init = 1;
      shft.status = largeint_OFL;
      break;
    case BIGINT_UFL:
      shft.init = 1;
      shft.status = largeint_UFL;
      break;
    default:
      largeint::panic(largeint_ERR, ">>", err, __LINE__);
    }
  return shft;
}

/* Gestion des erreurs */
/***********************/

// Fonctions membres des classes d'exception

largeint_IOErr::largeint_IOErr(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_IOErr::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Erreur I/O, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_DivBy0::largeint_DivBy0(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_DivBy0::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Division par 0, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_NullNew::largeint_NullNew(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_NullNew::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Erreur avec new, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_OFlow::largeint_OFlow(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_OFlow::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Overflow, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_UFlow::largeint_UFlow(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_UFlow::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Underflow, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_NoInit::largeint_NoInit(const char* const func, const int ar, const int lin)
{
  function = func;
  line = lin;
  arg = ar;
}

void largeint_NoInit::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Variable non initialisee, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line
       << ", argument: " << arg << endl;
}

largeint_Base::largeint_Base(const char* const func, const int lin)
{
  function = func;
  line = lin;
  arg = 0;
}

void largeint_Base::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Base invalide, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line << endl;
}

largeint_NullPtr::largeint_NullPtr(const char* const func, const int ar, const int lin)
{
  function = func;
  line = lin;
  arg = ar;
}

void largeint_NullPtr::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Pointeur NULL, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line
       << ", argument: " << arg << endl;
}

largeint_UnknErr::largeint_UnknErr(const char* const func, const int ar, const int lin)
{
  function = func;
  line = lin;
  arg = ar;
}

void largeint_UnknErr::debugdata(void) const
{
  cerr << "Exception detectee par la classe largeint:" << endl;
  cerr << "Erreur inconnue, dans la fonction " << function << endl;
  cerr << "Module: " << __FILE__ << ", ligne: " << line
       << ", argument: " << arg << endl;
}

// Fonction de gestion des erreurs

void largeint::panic(ERRORS err, const char* const func, const int ar, const int lin)
{
  cerr << "Erreur d'execution detectee par la classe largeint:\n" << endl;
  switch (err)
    {
    case largeint_OK:
      break;
    case largeint_EOF:
      cerr << "Erreur I/O, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_IOErr(func, lin);
#endif
      break;
    case largeint_DBZ:
      cerr << "Division par 0, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_DivBy0(func, lin);
#endif
      break;
    case largeint_NEW:
      cerr << "Erreur dans new, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_NullNew(func, lin);
#endif
      break;
    case largeint_OFL:
      cerr << "Overflow, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_OFlow(func, lin);
#endif
      break;
    case largeint_UFL:
      cerr << "Underflow, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_UFlow(func, lin);
#endif
      break;
    case largeint_VAL:
      cerr << "Argument: " << ar << " dans la fonction: " << func << " pas initialise" << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_NoInit(func, ar, lin);
#endif
      break;
    case largeint_BOR:
      cerr << "Base non-valide de l'argument: " << ar << ", dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_Base(func, lin);
#endif
      break;
    case largeint_NPT:
      cerr << "Argument: " << ar << " dans la fonction: " << func << " est NULL" << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_NullPtr(func, ar, lin);
#endif
      break;
    case largeint_ERR:
      cerr << "Erreur inconnue, dans la fonction: " << func << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << lin << endl;
#ifdef LEXCEPT
      throw largeint_UnknErr(func, ar, lin);
#endif
      break;
    }
}

// Fonction d'extraction du statut

ERRORS largeint::get_status(void)
{
  return status;
}

/* Gestion I/O */
/***************/

// Initialisation

long largeint::flagindex;

largeint_init largeint::setup;

largeint_init::largeint_init(void)
{
  largeint::flagindex = ios::xalloc();
  cout.iword(largeint::flagindex) = cerr.iword(largeint::flagindex) =
    largeint::largeinthex|largeint::largeintshowb;
}

// Formatage des flux

long largeint::flag(void)
{
  return cout.iword(flagindex);
}

long largeint::flag(ostream& s)
{
  return s.iword(flagindex);
}

long largeint::setf(ostream& s, long fl)
{
  long t = s.iword(flagindex);
  
  if (fl & largeint::largeintdec)
    {
      s.iword(flagindex) =
	(t & ~largeint::largeinthex & ~largeint::largeintoct & ~largeint::largeintbin)
	| largeint::largeintdec;
      fl = fl ^ largeint::largeintdec;
    }
  if (fl & largeint::largeinthex)
    {
      s.iword(flagindex) =
	(t & ~largeint::largeintdec & ~largeint::largeintoct & ~largeint::largeintbin)
	| largeint::largeinthex;
      fl = fl ^ largeint::largeinthex;
    }
  if (fl & largeint::largeintoct)
    {
      s.iword(flagindex) =
	(t & ~largeint::largeintdec & ~largeint::largeinthex & ~largeint::largeintbin)
	| largeint::largeintoct;
      fl = fl ^ largeint::largeintoct;
    }
  if (fl & largeint::largeintbin)
    {
      s.iword(flagindex) =
	(t & ~largeint::largeintdec & ~largeint::largeintoct & ~largeint::largeinthex)
	| largeint::largeintbin;
      fl = fl ^ largeint::largeintbin;
    }
  s.iword(flagindex) = s.iword(flagindex) | fl;

  return t;
}

long largeint::setf(long fl)
{
  return largeint::setf(cout, fl);
}

long largeint::unsetf(ostream& s, long fl)
{
  int t = s.iword(flagindex);

  s.iword(flagindex) = (t | fl) ^ fl;
  return t;
}

long largeint::unsetf(long fl)
{
  return largeint::unsetf(cout, fl);
}

long largeint::restoref(ostream& s, long fl)
{
  int t = s.iword(flagindex);

  largeint::unsetf(s, t);
  s.iword(flagindex) = fl;
  return t;
}

long largeint::restoref(long fl)
{
  return largeint::restoref(cout, fl);
}

// Op�rateurs de flux surcharg�s

ostream& operator<<(ostream& s, const largeint& a)
{
  USHORT base = 16;
  long fl = largeint::flag(s);
  char* flargeint;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "ostream <<", 2, __LINE__);
  if (fl & largeint::largeinthex)
    base = 16;
  else
    {
      if (fl & largeint::largeintdec)
	base = 10;
      else
	{
	  if (fl & largeint::largeintoct)
	    base = 8;
	  else
	    {
	      if (fl & largeint::largeintbin)
		base = 2;
	    }
	}
    }
  if (fl & largeint::largeintshowb)
    flargeint = largeint2str(a, base, 1);
  else
    flargeint = largeint2str(a, base, 0);
  s << flargeint << endl;
  s << numbits(a) << "bit(s)" << endl;

  return s;
}

ofstream& operator<<(ofstream& s, const largeint& a)
{
  unsigned int i;

  if (a.init == 0)
    largeint::panic(largeint_VAL, "<<", 2, __LINE__);
  for (i = 0; i <= digits_l(a.n_l); i++)
    {
      if (write_ushort(s, a.n_l[i]) != 0)
	largeint::panic(largeint_EOF, "<<", 1, __LINE__);
    }

  return s;
}

ifstream& operator>>(ifstream& s, largeint& a)
{
  unsigned int i;

  if (read_ushort(s, a.n_l) != 0)
    largeint::panic(largeint_EOF, ">>", 1, __LINE__);
  if (digits_l(a.n_l) < BIGINTMAXSHORT)
    {
      for (i = 1; i <= digits_l(a.n_l); i++)
	{
	  if (read_ushort(s, &a.n_l[i]) != 0)
	    largeint::panic(largeint_EOF, ">>", 1, __LINE__);
	}
    }
  if (check_l(a.n_l) == 0)
    a.init = 1;
  else
    a.init = 0;

  return s;
}

// Manipulateurs

ostream& largeint_Bin(ostream& s)
{
  largeint::setf(s, largeint::largeintbin);
  return s;
}

ostream& largeint_Oct(ostream& s)
{
  largeint::setf(s, largeint::largeintoct);
  return s;
}

ostream& largeint_Dec(ostream& s)
{
  largeint::setf(s, largeint::largeintdec);
  return s;
}

ostream& largeint_Hex(ostream& s)
{
  largeint::setf(s, largeint::largeinthex);
  return s;
}

ostream& largeint_Showb(ostream& s)
{
  largeint::setf(s, largeint::largeintshowb);
  return s;
}

ostream& largeint_NoShowb(ostream& s)
{
  largeint::unsetf(s, largeint::largeintshowb);
  return s;
}

// Mod�les d'�criture et de lecture dans un fichier

int write_ushort(ofstream& s, bigint src)
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

int read_ushort(ifstream& s, bigint* dest)
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




/* Fin de libarithmcpp.cpp */