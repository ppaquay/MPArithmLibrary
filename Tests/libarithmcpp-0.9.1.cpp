/***********************************************************************
 *                                                                     *
 *     Code source pour la librairie de fonctions arithmétiques C++    *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "libarithmcpp-0.9.1.hpp"


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

// Destructeur

largeint::~largeint()
{
  purge_l(n_l);
  delete[] n_l;
}

/* Opérateur d'assignement */
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

// Fonctions membres d'arithmétique

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

// Fonctions membres d'arithmétique modulo

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

// Fonctions membres opérant sur les bits

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

// Fonctions membres de théorie des nombres

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

// Fonctions amies d'arithmétique

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

// Fonctions amies d'arithmétique modulo

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

// Fonctions amies opérant sur les bits

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

// Fonctions amies de théorie des nombres

const int iseven(const largeint& a)
{
  if (a.init == 0)
    largeint::panic(largeint_VAL, "iseven", 1, __LINE__);
  return (iseven_l(a.n_l));
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

// Fonctions amies de génération aléatoire

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

// Fonctions amies de génération de nombres premiers

const largeint prime(const largeint& min, const largeint& max, const largeint& f)
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
  if (prim.iseven() == 1)
    prim = prim + 1;
  if (prim > max)
    prim = min + prim % (t + 1);
  while ((gcd(prim - 1, f) != one) || (prim.MRprime(302, 5) == 0))
    {
      prim = prim + 2;
      while (prim > max)
	{
	  prim = min + prim % (t + 1);
	  if (prim.iseven() == 1)
	    prim = prim + 1;
	}
    }
  return prim;
}

const largeint prime(const USHORT l, const largeint& f)
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
  return (prime(min, max, f));
}

const largeint prime(const USHORT l)
{
  largeint one((USHORT)1);
  
  return (prime(l, one));
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

/* Opérateurs surchargés */
/*************************/

// Opérateurs de comparaison surchargés

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

// Opérateurs arithmétiques surchargés

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

// Opérateurs booléens surchargés

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

// Opérateurs de flux surchargés

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
  int i;

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
  int i;

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

// Modèles d'écriture et de lecture dans un fichier

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
