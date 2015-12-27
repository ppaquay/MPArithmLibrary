/***********************************************************************
 *                                                                     *
 *     Code source pour la librairie de fonctions arithmétiques C++    *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include "arithmcpp.hpp"
#include <stdlib.h>
#include <string.h>


/* Constructeurs */

BINT::BINT(void)
{
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 1", 0, __LINE__);
  maxlen = BIGINTMAXDIGIT;
  init = 0;
  status = BINT_OK;
  base = 16;
  showbase = 1;
}

BINT::BINT(const BINT& n)
{
  if (n.init == 0)
    panic(BINT_VAL, "Constructeur 2", 1, __LINE__);
  n_l = new bigint[n.maxlen + 1];
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 2", 0, __LINE__);
  copy_l(n_l, n.n_l);
  maxlen = n.maxlen;
  init = 1;
  status = n.status;
  base = n.base;
  showbase = n.showbase;
}

BINT::BINT(const char* const str, const int bas)
{
  int err;
  
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 3", 0, __LINE__);
  err = str2bigint_l(n_l, (char*)str, bas);
  switch (err)
    {
    case BIGINT_OK:
      maxlen = BIGINTMAXDIGIT;
      init = 1;
      status = BINT_OK;
      base = 16;
      showbase = 1;
      break;
    case BIGINT_NPT:
      status = BINT_NPT;
      panic(BINT_NPT, "Constructeur 3", 1, __LINE__);
      break;
    case BIGINT_OFL:
      init = 1;
      status = BINT_OFL;
      panic(BINT_OFL, "Constructeur 3", 0, __LINE__);
      break;
    default:
      status = BINT_ERR;
      panic(BINT_ERR, "Constructeur 3", err, __LINE__);
    }
}

BINT::BINT(const UCHAR* const bytevec, const int len)
{
  int err;

  if (bytevec == NULL)
    panic(BINT_NHP, "Constructeur 4", 0, __LINE__);
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 4", 0, __LINE__);
  err = byte2bigint_l(n_l, (UCHAR*)bytevec, len);
  switch (err)
    {
    case BIGINT_OK:
      maxlen = BIGINTMAXDIGIT;
      init = 1;
      status = BINT_OK;
      base = 16;
      showbase = 1;
      break;
    case BIGINT_NPT:
      status = BINT_NPT;
      panic(BINT_NPT, "Constructeur 4", 1, __LINE__);
      break;
    case BIGINT_OFL:
      init = 1;
      status = BINT_OFL;
      panic(BINT_OFL, "Constructeur 4", 0, __LINE__);
      break;
    default:
      status = BINT_ERR;
      panic(BINT_ERR, "Constructeur 4", err, __LINE__);
    }
}

BINT::BINT(const USHORT u)
{
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 5", 0, __LINE__);
  u2bigint_l(n_l, u);
  maxlen = BIGINTMAXDIGIT;
  init = 1;
  status = BINT_OK;
  base = 16;
  showbase = 1;
}

BINT::BINT(const BIGINT m_l)
{
  if (check_l((bigint*)m_l) < 0)
    panic(BINT_VAL, "Constructeur 6", 1, __LINE__);
  n_l = new BIGINT;
  if (n_l == NULL)
    panic(BINT_NHP, "Constructeur 6", 0, __LINE__);
  copy_l(n_l, (bigint*)m_l);
  maxlen = BIGINTMAXDIGIT;
  init = 1;
  status = BINT_OK;
  base = 16;
  showbase = 1;
}

/* Destructeur */

BINT::~BINT()
{
  delete [] n_l;
}

/* Opérateur assignement */

const BINT& BINT::operator=(const BINT& n)
{
  if (n.init == 0)
    panic(BINT_VAL, "=", 1, __LINE__);
  if (maxlen < digits_l(n.n_l))
    panic(BINT_OFL, "=", 1, __LINE__);
  if (this != &n)
    {
      copy_l(n_l, n.n_l);
      init = 1;
      status = n.status;
    }
  return *this;
}

/* Opérateurs logiques surchargés amis */

const int operator==(const BINT& m, const BINT& n)
{
  int equ;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "==", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "==", 2, __LINE__);
  if (&n == &m)
    return 1;
  else
    {
      equ = equ_l(m.n_l, n.n_l);
      return equ;
    }
}

const int operator!=(const BINT& m, const BINT& n)
{
  if (m.init == 0)
    BINT::panic(BINT_VAL, "!=", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "!=", 2, __LINE__);
  if (&m == &n)
    return 0;
  return (equ_l(m.n_l, n.n_l) == 0);
}

const int operator<(const BINT& m, const BINT& n)
{
  int cmp;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "<", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "<", 2, __LINE__);
  if (&n == &m)
    return 0;
  else
    {
      cmp = comp_l(m.n_l, n.n_l);
      if (cmp == -1)
	return 1;
      else
	return 0;
    }
}

const int operator>(const BINT& m, const BINT& n)
{
  int cmp;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, ">", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, ">", 2, __LINE__);
  if (&n == &m)
    return 0;
  else
    {
      cmp = comp_l(m.n_l, n.n_l);
      if (cmp == 1)
	return 1;
      else
	return 0;
    }
}

const int operator<=(const BINT& m, const BINT& n)
{
  int cmp;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "<=", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "<=", 2, __LINE__);
  if (&n == &m)
    return 1;
  cmp = comp_l(m.n_l, n.n_l);
  if ((cmp == -1) || (cmp == 0))
    return 1;
  else
    return 0;
}

const int operator>=(const BINT& m, const BINT& n)
{
  int cmp;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, ">=", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, ">=", 2, __LINE__);
  if (&n == &m)
    return 1;
  cmp = comp_l(m.n_l, n.n_l);
  if ((cmp == 1) || (cmp == 0))
    return 1;
  else
    return 0;
}

/* Opérateurs booléens surchargés */

const BINT operator&(const BINT& m, const BINT& n)
{
  BINT et;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "&", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "&", 2, __LINE__);
  and_l(m.n_l, n.n_l, et.n_l);
  et.init = 1;
  et.status = BINT_OK;
  return et;
}

const BINT operator|(const BINT& m, const BINT& n)
{
  BINT ou;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "|", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "|", 2, __LINE__);
  or_l(m.n_l, n.n_l, ou.n_l);
  ou.init = 1;
  ou.status = BINT_OK;
  return ou;
}

const BINT operator^(const BINT& m, const BINT& n)
{
  BINT oux;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "^", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "^", 2, __LINE__);
  xor_l(m.n_l, n.n_l, oux.n_l);
  oux.init = 1;
  oux.status = BINT_OK;
  return oux;
}

const BINT operator<<(const BINT& m, const int nbrebits)
{
  BINT shft = m;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "<<", 1, __LINE__);
  err = shift_l(shft.n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shft.init = 1;
      shft.status = BINT_OK;
      break;
    case BIGINT_OFL:
      shft.init = 1;
      shft.status = BINT_OFL;
      break;
    case BIGINT_UFL:
      shft.init = 1;
      shft.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, "<<", err, __LINE__);
    }
  return shft;
}

const BINT operator>>(const BINT& m, const int nbrebits)
{
  BINT shft = m;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, ">>", 1, __LINE__);
  err = shift_l(shft.n_l, -nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shft.init = 1;
      shft.status = BINT_OK;
      break;
    case BIGINT_OFL:
      shft.init = 1;
      shft.status = BINT_OFL;
      break;
    case BIGINT_UFL:
      shft.init = 1;
      shft.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, ">>", err, __LINE__);
    }
  return shft;
}

/* Opérateurs arithmétiques surchargés amis */

const BINT operator+(const BINT& m, const BINT& n)
{
  BINT sum;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "+", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "+", 2, __LINE__);
  err = add_l(m.n_l, n.n_l, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = BINT_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "+", err, __LINE__);
    }
  return sum;
}

const BINT operator+(const BINT& m, const USHORT n)
{
  BINT sum;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "+", 1, __LINE__);
  err = uadd_l(m.n_l, n, sum.n_l);
  switch (err)
    {
    case BIGINT_OK:
      sum.init = 1;
      sum.status = BINT_OK;
      break;
    case BIGINT_OFL:
      sum.init = 1;
      sum.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "+", err, __LINE__);
    }
  return sum;
}

const BINT operator-(const BINT& m, const BINT& n)
{
  BINT diff;
  int err;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "+", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "+", 2, __LINE__);
  err = sub_l(m.n_l, n.n_l, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = BINT_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, "+", err, __LINE__);
    }
  return diff;
}

const BINT operator-(const BINT& m, const USHORT n)
{
  BINT diff;
  int err;
  
  if (m.init == 0)
    BINT::panic(BINT_VAL, "+", 1, __LINE__);
  err = usub_l(m.n_l, n, diff.n_l);
  switch (err)
    {
    case BIGINT_OK:
      diff.init = 1;
      diff.status = BINT_OK;
      break;
    case BIGINT_UFL:
      diff.init = 1;
      diff.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, "+", err, __LINE__);
    }
  return diff;
}

const BINT operator*(const BINT& m, const BINT& n)
{
  BINT prod;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "*", 1, __LINE__);
  if (!n.init)
    BINT::panic(BINT_VAL, "*", 2, __LINE__);
  if (&m == &n)
    err = sqr_l(m.n_l, prod.n_l);
  else
    err = mul_l(m.n_l, n.n_l, prod.n_l);
  switch (err)
    {
    case BIGINT_OK:
      prod.init = 1;
      prod.status = BINT_OK;
      break;
    case BIGINT_OFL:
      prod.init = 1;
      prod.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "*", err, __LINE__);
    }
  return prod;
}

const BINT operator/(const BINT& m, const BINT& n)
{
  BINT quot;
  BIGINT dummy_l;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "/", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "/", 2, __LINE__);
  err = div_l(m.n_l, n.n_l, quot.n_l, dummy_l);
  switch (err)
    {
    case BIGINT_OK:
      quot.init = 1;
      quot.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "/", 2, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "/", err, __LINE__);
    }
  setzero_l(dummy_l);
  return quot;
}

const BINT operator%(const BINT& m, const BINT& n)
{
  BINT rem;
  int err;

  if (m.init == 0)
    BINT::panic(BINT_VAL, "%", 1, __LINE__);
  if (n.init == 0)
    BINT::panic(BINT_VAL, "%", 2, __LINE__);
  err = mod_l(m.n_l, n.n_l, rem.n_l);
  switch (err)
    {
    case BIGINT_OK:
      rem.init = 1;
      rem.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "%", 2, __LINE__);
      break;
    default:
      BINT::panic(BINT_DBZ, "%", err, __LINE__);
    }
  return rem;
}

/* Fonctions membres d'arithmétique */

const BINT& BINT::add(const BINT& s)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "add", 0, __LINE__);
  if (s.init == 0)
    panic(BINT_VAL, "add", 1, __LINE__);
  err = add_l(n_l, s.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_OFL:
      status = BINT_OFL;
      break;
    default:
      panic(BINT_ERR, "add", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::sub(const BINT& d)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "sub", 0, __LINE__);
  if (d.init == 0)
    panic(BINT_VAL, "sub", 1, __LINE__);
  err = sub_l(n_l, d.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_UFL:
      status = BINT_UFL;
      break;
    default:
      panic(BINT_ERR, "sub", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::mul(const BINT& p)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "mul", 0, __LINE__);
  if (p.init == 0)
    panic(BINT_VAL, "mul", 1, __LINE__);
  if (this == &p)
    err = sqr_l(n_l, n_l);
  else
    err = mul_l(n_l, p.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_OFL:
      status = BINT_OFL;
      break;
    default:
      panic(BINT_ERR, "mul", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::sqr(void)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "sqr", 0, __LINE__);
  err = sqr_l(n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_OFL:
      status = BINT_OFL;
      break;
    default:
      panic(BINT_ERR, "sqr", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::div(const BINT& d, BINT& r)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "div", 0, __LINE__);
  if (d.init == 0)
    panic(BINT_VAL, "div", 1, __LINE__);
  err = div_l(n_l, d.n_l, n_l, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      r.init = 1;
      r.status = BINT_OK;
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "div", 1, __LINE__);
      break;
    default:
      panic(BINT_DBZ, "div", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::mod(const BINT& d)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "mod", 0, __LINE__);
  if (d.init == 0)
    panic(BINT_VAL, "mod", 1, __LINE__);
  err = mod_l(n_l, d.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "mod", 1, __LINE__);
      break;
    default:
      panic(BINT_ERR, "mod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::mod2(const USHORT u)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "mod2", 0, __LINE__);
  err = mod2_l(n_l, u, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    default:
      panic(BINT_ERR, "mod2", err, __LINE__);
    }
  return *this;
}

/* Fonctions membres d'arithmétique modulaire */

const BINT& BINT::addmod(const BINT& s, const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "addmod", 0, __LINE__);
  if (s.init == 0)
    panic(BINT_VAL, "addmod", 1, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "addmod", 2, __LINE__);
  err = addmod_l(n_l, s.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "addmod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "addmod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::submod(const BINT& d, const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "submod", 0, __LINE__);
  if (d.init == 0)
    panic(BINT_VAL, "submod", 1, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "submod", 2, __LINE__);
  err = submod_l(n_l, d.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "submod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "submod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::mulmod(const BINT& p, const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "mulmod", 0, __LINE__);
  if (p.init == 0)
    panic(BINT_VAL, "mulmod", 1, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "mulmod", 2, __LINE__);
  if (&p == this)
    err = sqrmod_l(n_l, m.n_l, n_l);
  else
    err = mulmod_l(n_l, p.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "mulmod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "mulmod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::sqrmod(const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "sqrmod", 0, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "sqrmod", 1, __LINE__);
  err = sqrmod_l(n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "sqrmod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "sqrmod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::expmod(const BINT& e, const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "expmod", 0, __LINE__);
  if (e.init == 0)
    panic(BINT_VAL, "expmod", 1, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "expmod", 2, __LINE__);
  err = expmodk_l(n_l, e.n_l, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "expmod", 2, __LINE__);
      break;
    case BIGINT_MAL:
      panic(BINT_NPT, "expmod", 0, __LINE__);
      break;
    default:
      panic(BINT_ERR, "expmod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::expmod(const USHORT e, const BINT& m)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "expmod", 0, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "expmod", 2, __LINE__);
  err = uexpmod_l(n_l, e, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "expmod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "expmod", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::expmod2(const USHORT k, const BINT& m)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "expmod2", 0, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "expmod2", 2, __LINE__);
  err = expmod2k_l(n_l, k, m.n_l, n_l);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "expmod2", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "expmod2", err, __LINE__);
    }
  return *this;
}

const int BINT::eqmod(const BINT& b, const BINT& m)
{
  int err;

  if (init == 0)
    panic(BINT_VAL, "eqmod", 0, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "eqmod", 1, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "eqmod", 2, __LINE__);
  err = eqmod_l(n_l, b.n_l, m.n_l);
  switch (err)
    {
    case 0:
    case 1:
      break;
    case BIGINT_DBZ:
      panic(BINT_DBZ, "eqmod", 2, __LINE__);
      break;
    default:
      panic(BINT_ERR, "eqmod", err, __LINE__);
    }
  return err;
}

const BINT& BINT::shift(const int nbrebits)
{
  int err;
  
  if (init == 0)
    panic(BINT_VAL, "shift", 0, __LINE__);
  err = shift_l(n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      status = BINT_OK;
      break;
    case BIGINT_OFL:
      status = BINT_OFL;
      break;
    case BIGINT_UFL:
      status = BINT_UFL;
      break;
    default:
      panic(BINT_ERR, "shift", err, __LINE__);
    }
  return *this;
}

const BINT& BINT::setbit(const unsigned int p)
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
    case BIGINT_OFL:
      status = BINT_OFL;
      break;
    default:
      status = BINT_OK;
    }
  return *this;
}

const BINT& BINT::clearbit(const unsigned int p)
{
  if (init == 0)
    panic(BINT_VAL, "clearbit", 0, __LINE__);
  clearbit_l(n_l, p);
  status = BINT_OK;
  return *this;
}

const int BINT::testbit(const unsigned int p)
{
  if (init == 0)
    panic(BINT_VAL, "testbit", 0, __LINE__);
  return (testbit_l(n_l, p));
}

/* Fonctions membres de théorie des nombres */

const unsigned int BINT::numbits(void)
{
  if (init == 0)
    panic(BINT_VAL, "numbits", 0, __LINE__);
  return (numbits_l(n_l));
}

const int BINT::iseven(void)
{
  if (init == 0)
    panic(BINT_VAL, "iseven", 0, __LINE__);
  return (iseven_l(n_l));
}

const int BINT::prime(void)
{
  if (init == 0)
    panic(BINT_VAL, "prime", 0, __LINE__);
  return (MRprime_l(n_l, 302, 5));
}

const BINT BINT::issqr(void)
{
  BINT sqroot;
  
  if (init == 0)
    panic(BINT_VAL, "issqr", 0, __LINE__);
  issqr_l(n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = BINT_OK;
  return sqroot;
}

const BINT BINT::introot(void)
{
  BINT sqroot;
  
  if (init == 0)
    panic(BINT_VAL, "introot", 0, __LINE__);
  introot_l(n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = BINT_OK;
  return sqroot;
}

const BINT BINT::gcd(const BINT& b)
{
  BINT g;

  if (init == 0)
    panic(BINT_VAL, "gcd", 0, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "gcd", 1, __LINE__);
  gcd_l(n_l, b.n_l, g.n_l);
  g.init = 1;
  g.status = BINT_OK;
  return g;
}

const BINT BINT::lcm(const BINT& b)
{
  BINT l;
  int err;

  if (init == 0)
    panic(BINT_VAL, "lcm", 0, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "lcm", 1, __LINE__);
  err =  lcm_l(n_l, b.n_l, l.n_l);
  switch (err)
    {
    case BIGINT_OK:
      l.init = 1;
      l.status = BINT_OK;
      break;
    case BIGINT_OFL:
      l.init = 1;
      l.status = BINT_OFL;
      break;
    default:
      panic(BINT_ERR, "lcm", err, __LINE__);
    }
  return l;
}

const BINT BINT::xgcd(const BINT& b, BINT& u, int& usign, BINT& v, int& vsign)
{
  BINT g;
  
  if (init == 0)
    panic(BINT_VAL, "xgcd", 0, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "xgcd", 1, __LINE__);
  xgcd_l(n_l, b.n_l, g.n_l, u.n_l, &usign, v.n_l, &vsign);
  g.init = 1;
  u.init = 1;
  v.init = 1;
  g.status = BINT_OK;
  u.status = BINT_OK;
  v.status = BINT_OK;
  return g;
}

const BINT BINT::invmod(const BINT& m)
{
  BINT inv, g;

  if (init == 0)
    panic(BINT_VAL, "invmod", 0, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "invmod", 1, __LINE__);
  invmod_l(n_l, m.n_l, g.n_l, inv.n_l);
  inv.init = 1;
  inv.status = BINT_OK;
  return inv;
}

const int BINT::jacobi(const BINT& b)
{
  if (init == 0)
    panic(BINT_VAL, "jacobi", 0, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "jacobi", 1, __LINE__);
  return (jacobi_l(n_l, b.n_l));
}

const BINT BINT::root(const BINT& p)
{
  BINT x;

   if (init == 0)
    panic(BINT_VAL, "root", 0, __LINE__);
  if (p.init == 0)
    panic(BINT_VAL, "root", 1, __LINE__);
  if (proot_l(n_l, p.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = BINT_OK;
    }
  else
    {
      x.init = 0;
      x.status = BINT_ERR;
    }
  return x;
}

const BINT BINT::root(const BINT& p, const BINT& q)
{
  BINT x;

  if (init == 0)
    panic(BINT_VAL, "root", 0, __LINE__);
  if (p.init == 0)
    panic(BINT_VAL, "root", 1, __LINE__);
  if (q.init == 0)
    panic(BINT_VAL, "root", 2, __LINE__);
  if (pqroot_l(n_l, p.n_l, q.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = BINT_OK;
    }
  else
    {
      x.init = 0;
      x.status = BINT_ERR;
    }
  return x;
}

const BINT BINT::chinrem(const BINT& m, const BINT& b, const BINT& n)
{
  BINT x;
    
  if (init == 0)
    panic(BINT_VAL, "chinrem", 0, __LINE__);
  if (m.init == 0)
    panic(BINT_VAL, "chinrem", 1, __LINE__);
  if (b.init == 0)
    panic(BINT_VAL, "chinrem", 2, __LINE__);
  if (n.init == 0)
    panic(BINT_VAL, "chinrem", 3, __LINE__);
  bigint* coeff_l[4];
  coeff_l[0] = n_l;
  coeff_l[1] = m.n_l;
  coeff_l[2] = b.n_l;
  coeff_l[3] = n.n_l;
  if (chinrem_l(2, coeff_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = BINT_OK;
    }
  else
    {
      x.init = 0;
      x.status = BINT_ERR;
    }
  return x;
}

/* Fonctions membres de conversion */

char* BINT::bint2str(const USHORT base, const int showb)
{
  if (init == 0)
    panic(BINT_VAL, "bint2str", 0, __LINE__);
  return (bigint2str_l(n_l, base, showb));
}

UCHAR* BINT::bint2byte(int* len)
{
  if (init == 0)
    panic(BINT_VAL, "bint2byte", 0, __LINE__);
  return (bigint2byte_l(n_l, len));
}

/* Fonctions d'arithmétique amies */

const BINT add(const BINT& a, const BINT& b)
{
  BINT s;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "add", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "add", 2, __LINE__);
  err = add_l(a.n_l, b.n_l, s.n_l);
  switch (err)
    {
    case BIGINT_OK:
      s.init = 1;
      s.status = BINT_OK;
      break;
    case BIGINT_OFL:
      s.init = 1;
      s.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "add", err, __LINE__);
    }
  return s;
}

const BINT sub(const BINT& a, const BINT& b)
{
  BINT d;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "sub", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "sub", 1, __LINE__);
  err = sub_l(a.n_l, b.n_l, d.n_l);
  switch (err)
    {
    case BIGINT_OK:
      d.init = 1;
      d.status = BINT_OK;
      break;
    case BIGINT_UFL:
      d.init = 1;
      d.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, "sub", err, __LINE__);
    }
  return d;
}

const BINT mul(const BINT& a, const BINT& b)
{
  BINT p;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "mul", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "mul", 2, __LINE__);
  if (&a == &b)
    err = sqr_l(a.n_l, p.n_l);
  else
    err = mul_l(a.n_l, b.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_OFL:
      p.init = 1;
      p.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "mul", err, __LINE__);
    }
  return p;
}

const BINT sqr(const BINT& a)
{
  BINT p;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "sqr", 1, __LINE__);
  err = sqr_l(a.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_OFL:
      p.init = 1;
      p.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "sqr", err, __LINE__);
    }
  return p;
}

const BINT div(const BINT& a, const BINT& b, BINT& r)
{
  BINT q;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "div", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "div", 2, __LINE__);
  err = div_l(a.n_l, b.n_l, q.n_l, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      q.init = 1;
      q.status = BINT_OK;
      r.init = 1;
      r.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "div", 2, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "div", err, __LINE__);
    }
  return q;
}

const BINT mod(const BINT& a, const BINT& b)
{
  BINT r;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "mod", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "mod", 2, __LINE__);
  err = mod_l(a.n_l, b.n_l, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      r.init = 1;
      r.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "mod", 2, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "mod", err, __LINE__);
    }
  return r;
}

const BINT mod2(const BINT& a, const USHORT u)
{
  BINT r;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "mod2", 1, __LINE__);
  err = mod2_l(a.n_l, u, r.n_l);
  switch (err)
    {
    case BIGINT_OK:
      r.init = 1;
      r.status = BINT_OK;
      break;
    default:
      BINT::panic(BINT_ERR, "mod2", err, __LINE__);
    }
  return r;
}

/* Fonctions d'arithmétique modulaire amies */

const BINT addmod(const BINT& a, const BINT& b, const BINT& m)
{
  BINT s;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "addmod", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "addmod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "addmod", 3, __LINE__);
  err = addmod_l(a.n_l, b.n_l, m.n_l, s.n_l);
  switch (err)
    {
    case BIGINT_OK:
      s.init = 1;
      s.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "addmod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "addmod", err, __LINE__);
    }
  return s;
}

const BINT submod(const BINT& a, const BINT& b, const BINT& m)
{
  BINT d;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "submod", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "submod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "submod", 3, __LINE__);
  err = submod_l(a.n_l, b.n_l, m.n_l, d.n_l);
  switch (err)
    {
    case BIGINT_OK:
      d.init = 1;
      d.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "submod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "submod", err, __LINE__);
    }
  return d;
}

const BINT mulmod(const BINT& a, const BINT& b, const BINT& m)
{
  BINT p;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "mulmod", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "mulmod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "mulmod", 3, __LINE__);
  if (&a == &b)
    err = sqrmod_l(a.n_l, m.n_l, p.n_l);
  else
    err = mulmod_l(a.n_l, b.n_l, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "mulmod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "mulmod", err, __LINE__);
    }
  return p;
}

const BINT sqrmod(const BINT& a, const BINT& m)
{
  BINT p;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "sqrmod", 1, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "sqrmod", 2, __LINE__);
  err = sqrmod_l(a.n_l, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "sqrmod", 2, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "sqrmod", err, __LINE__);
    }
  return p;
}

const BINT expmod(const BINT& a, const BINT& e, const BINT& m)
{
  BINT p;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "expmod", 1, __LINE__);
  if (e.init == 0)
    BINT::panic(BINT_VAL, "expmod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "expmod", 3, __LINE__);
  err = expmodk_l(a.n_l, e.n_l, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "expmod", 3, __LINE__);
      break;
    case BIGINT_MAL:
      BINT::panic(BINT_NPT, "expmod", 0, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "expmod", err, __LINE__);
    }
  return p;
}

const BINT expmod(const BINT& a, const USHORT e, const BINT& m)
{
  BINT p;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "expmod", 1, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "expmod", 3, __LINE__);
  err = uexpmod_l(a.n_l, e, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "expmod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "expmod", err, __LINE__);
    }
  return p;
}

const BINT expmod(const USHORT a, const BINT& e, const BINT& m)
{
  BINT p;
  int err;
  
  if (e.init == 0)
    BINT::panic(BINT_VAL, "expmod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "expmod", 3, __LINE__);
  err = wexpmod_l(a, e.n_l, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "expmod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "expmod", err, __LINE__);
    }
  return p;
}

const BINT expmod2(const BINT& a, const USHORT k, const BINT& m)
{
  BINT p;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "expmod2", 1, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "expmod2", 3, __LINE__);
  err = expmod2k_l(a.n_l, k, m.n_l, p.n_l);
  switch (err)
    {
    case BIGINT_OK:
      p.init = 1;
      p.status = BINT_OK;
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_DBZ, "expmod2", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_ERR, "expmod2", err, __LINE__);
    }
  return p;
}

const int eqmod(const BINT& a, const BINT& b, const BINT& m)
{
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "eqmod", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "eqmod", 2, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "eqmod", 3, __LINE__);
  err = eqmod_l(a.n_l, b.n_l, m.n_l);
  switch (err)
    {
    case 0:
    case 1:
      break;
    case BIGINT_DBZ:
      BINT::panic(BINT_VAL, "eqmod", 3, __LINE__);
      break;
    default:
      BINT::panic(BINT_VAL, "eqmod", err, __LINE__);
    }
  return err;
}

const BINT shift(const BINT& a, const int nbrebits)
{
  BINT shift = a;
  int err;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "shift", 1, __LINE__);
  err = shift_l(shift.n_l, nbrebits);
  switch (err)
    {
    case BIGINT_OK:
      shift.init = 1;
      shift.status = BINT_OK;
      break;
    case BIGINT_OFL:
      shift.init = 1;
      shift.status = BINT_OFL;
      break;
    case BIGINT_UFL:
      shift.init = 1;
      shift.status = BINT_UFL;
      break;
    default:
      BINT::panic(BINT_ERR, "shift", err, __LINE__);
    }
  return shift;
}

/* Fonctions de théorie des nombres amies */

const unsigned int numbits(const BINT& a)
{
  if (a.init == 0)
    BINT::panic(BINT_VAL, "numbits", 1, __LINE__);
  return (numbits_l(a.n_l));
}

const int iseven(const BINT& a)
{
  if (a.init == 0)
    BINT::panic(BINT_VAL, "iseven", 1, __LINE__);
  return (iseven_l(a.n_l));
}

const int prime(const BINT& a)
{
  if (a.init == 0)
    BINT::panic(BINT_VAL, "prime", 1, __LINE__);
  return (MRprime_l(a.n_l, 302, 5));
}

const BINT issqr(const BINT& a)
{
  BINT sqroot;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "issqr", 1, __LINE__);
  issqr_l(a.n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = BINT_OK;
  return sqroot;
}

const BINT introot(const BINT& a)
{
  BINT sqroot;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "introot", 1, __LINE__);
  introot_l(a.n_l, sqroot.n_l);
  sqroot.init = 1;
  sqroot.status = BINT_OK;
  return sqroot;
}

const BINT gcd(const BINT& a, const BINT& b)
{
  BINT g;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "gcd", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "gcd", 2, __LINE__);
  gcd_l(a.n_l, b.n_l, g.n_l);
  g.init = 1;
  g.status = BINT_OK;
  return g;
}

const BINT lcm(const BINT& a, const BINT& b)
{
  BINT l;
  int err;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "lcm", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "lcm", 2, __LINE__);
  err =  lcm_l(a.n_l, b.n_l, l.n_l);
  switch (err)
    {
    case BIGINT_OK:
      l.init = 1;
      l.status = BINT_OK;
      break;
    case BIGINT_OFL:
      l.init = 1;
      l.status = BINT_OFL;
      break;
    default:
      BINT::panic(BINT_ERR, "lcm", err, __LINE__);
    }
  return l;
}

const BINT xgcd(const BINT& a, const BINT& b, BINT& u, int& usign, BINT& v, int& vsign)
{
  BINT g;
  
  if (a.init == 0)
    BINT::panic(BINT_VAL, "xgcd", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "xgcd", 2, __LINE__);
  xgcd_l(a.n_l, b.n_l, g.n_l, u.n_l, &usign, v.n_l, &vsign);
  g.init = 1;
  u.init = 1;
  v.init = 1;
  g.status = BINT_OK;
  u.status = BINT_OK;
  v.status = BINT_OK;
  return g;
}

const BINT invmod(const BINT& a, const BINT& m)
{
  BINT inv, g;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "invmod", 1, __LINE__);
  if (m.init == 0)
    BINT::panic(BINT_VAL, "invmod", 2, __LINE__);
  invmod_l(a.n_l, m.n_l, g.n_l, inv.n_l);
  inv.init = 1;
  inv.status = BINT_OK;
  return inv;
}

const int jacobi(const BINT& a, const BINT& b)
{
  if (a.init == 0)
    BINT::panic(BINT_VAL, "jacobi", 1, __LINE__);
  if (b.init == 0)
    BINT::panic(BINT_VAL, "jacobi", 2, __LINE__);
  return (jacobi_l(a.n_l, b.n_l));
}

const BINT root(const BINT& a, const BINT& p)
{
  BINT x;

   if (a.init == 0)
    BINT::panic(BINT_VAL, "root", 1, __LINE__);
  if (p.init == 0)
    BINT::panic(BINT_VAL, "root", 2, __LINE__);
  if (proot_l(a.n_l, p.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = BINT_OK;
    }
  else
    {
      x.init = 0;
      x.status = BINT_ERR;
    }
  return x;
}

const BINT root(const BINT& a, const BINT& p, const BINT& q)
{
  BINT x;

  if (a.init == 0)
    BINT::panic(BINT_VAL, "root", 1, __LINE__);
  if (p.init == 0)
    BINT::panic(BINT_VAL, "root", 2, __LINE__);
  if (q.init == 0)
    BINT::panic(BINT_VAL, "root", 3, __LINE__);
  if (pqroot_l(a.n_l, p.n_l, q.n_l, x.n_l) == 0)
    {
      x.init = 1;
      x.status = BINT_OK;
    }
  else
    {
      x.init = 0;
      x.status = BINT_ERR;
    }
  return x;
}

/* Fonctions de conversion amies */

char* bint2str(const BINT& n, const USHORT base, const int showb)
{
  if (n.init == 0)
    BINT::panic(BINT_VAL, "bint2str", 1, __LINE__);
  return (bigint2str_l(n.n_l, base, showb));
}

UCHAR* bint2byte(const BINT& n, int* len)
{
  if (n.init == 0)
    BINT::panic(BINT_VAL, "bint2byte", 1, __LINE__);
  return (bigint2byte_l(n.n_l, len));
}

/* Fonctions de génération de nombres pseudo-aléatoires */

void seed64(const ULONG seed)
{
  ulseed64_l(seed);
}

void seed64(const BINT& seed)
{
  if (seed.init == 0)
    BINT::panic(BINT_VAL, "seed64", 1, __LINE__);
  else
    seed64_l(seed.n_l);
}

BINT rand64(const int l)
{
  BINT rand;

  if (l > (int)BIGINTMAXBIT)
    rand_l(rand.n_l, BIGINTMAXBIT);
  else
    rand_l(rand.n_l, l);
  rand.init = 1;
  rand.status = BINT_OK;
  return rand;
}

BINT rand64(const BINT& min, const BINT& max)
{
  BINT rand;
  BINT temp = max - min;
  USHORT l = (numbits(min) + numbits(max)) >> 1; 

  if (min.init == 0)
    BINT::panic(BINT_VAL, "rand", 1, __LINE__);
  if (max.init == 0)
    BINT::panic(BINT_VAL, "rand", 2, __LINE__);
  if (max < min)
    BINT::panic(BINT_VAL, "rand", 1, __LINE__);
  if (l > (int)BIGINTMAXBIT)
    rand_l(rand.n_l, BIGINTMAXBIT);
  else
    rand_l(rand.n_l, l);
  rand.init = 1;
  rand.status = BINT_OK;
  if (rand < min)
    rand = min + rand;
  if (rand > max)
    rand = min + rand % (temp + 1);
  return rand;
}

int seedBBS(const ULONG seed)
{
  return (ulseedBBS_l(seed));
}

int seedBBS(const BINT& seed)
{
  if (seed.init == 0)
    BINT::panic(BINT_VAL, "seedBBS", 1, __LINE__);
  return (seedBBS_l(seed.n_l));
}

BINT randBBS(const int l)
{
  BINT rand;
  
  if (l > (int)BIGINTMAXBIT)
    randBBS_l(rand.n_l, BIGINTMAXBIT);
  else
    randBBS_l(rand.n_l, l);
  rand.init = 1;
  rand.status = BINT_OK;
  return rand;
}

BINT randBBS(const BINT& min, const BINT& max)
{
  BINT rand;
  BINT temp = max - min;
  USHORT l = (numbits(min) + numbits(max)) >> 1; 

  if (min.init == 0)
    BINT::panic(BINT_VAL, "randBBS", 1, __LINE__);
  if (max.init == 0)
    BINT::panic(BINT_VAL, "randBBS", 2, __LINE__);
  if (max < min)
    BINT::panic(BINT_VAL, "randBBS", 1, __LINE__);
  if (l > (int)BIGINTMAXBIT)
    randBBS_l(rand.n_l, BIGINTMAXBIT);
  else
    randBBS_l(rand.n_l, l);
  rand.init = 1;
  rand.status = BINT_OK;
  if (rand < min)
    rand = min + rand;
  if (rand > max)
    rand = min + rand % (temp + 1);
  return rand;
}

const BINT findprime(const BINT& min, const BINT& max, const BINT& f)
{
  BINT prim = randBBS(min, max);
  BINT t = max - min;

  if (min.init == 0)
    BINT::panic(BINT_VAL, "findprime", 1, __LINE__);
  if (max.init == 0)
    BINT::panic(BINT_VAL, "findprime", 2, __LINE__);
  if (f.init == 0)
    BINT::panic(BINT_VAL, "findprime", 3, __LINE__);
  if (min > max)
    BINT::panic(BINT_VAL, "findprime", 1, __LINE__);
  if (((BINT)f).iseven())
    BINT::panic(BINT_VAL, "findprime", 3, __LINE__);
  if (prim < min)
    prim = prim + min;
  if (prim.iseven())
    prim = prim + 1;
  if (prim > max)
    prim = min + prim % (t + 1);
  while ((gcd(prim - 1, f) != 1) || (prim.prime() == 0))
    {
      prim = prim + 2;
      while (prim > max)
	{
	  prim = min + prim % (t + 1);
	  if (prim.iseven())
	    prim = prim + 1;
	}
    }
  return prim;
}

const BINT findprime(const USHORT l, const BINT& f)
{
  BINT min((USHORT)0);
  BINT max((USHORT)0);
  
  if ((l == 0) || (l > BIGINTMAXBIT))
    BINT::panic(BINT_OFL, "findprime", 1, __LINE__);
  min.setbit(l - 1);
  max.setbit(l);
  max = max - 1;
  return (findprime(min, max, f));
}

/* Formatage des flux */

BINT& BINT::write_format(const int b, const int s)
{
  if ((b == 16) || (b == 10) || (b == 8) || (b == 2))
    base = b;
  else
    panic(BINT_BOR, "write_format", 1, __LINE__);
  if ((s == 0) || (s == 1))
    showbase = s;
  return *this;
}

/* Opérateurs de flux surchargés */

ostream& operator<<(ostream& s, const BINT& n)
{
  if (n.init == 0)
    BINT::panic(BINT_VAL, "<<", 2, __LINE__);
  s << bigint2str_l(n.n_l, n.base, n.showbase) << endl;
  s << numbits(n) << "bits" << endl;
  return s;
}

ofstream& operator<<(ofstream& s, const BINT& n)
{
  int i;

  if (n.init == 0)
    BINT::panic(BINT_VAL, "<<", 2, __LINE__);
  for (i = 0; i <= digits_l(n.n_l); i++)
    {
      if (write_ushort(s, n.n_l[i]))
	BINT::panic(BINT_EOF, "<<", 1, __LINE__);
    }
  return s;
}

ifstream& operator>>(ifstream& s, BINT& n)
{
  int i;

  if (read_ushort(s, n.n_l))
    BINT::panic(BINT_EOF, ">>", 1, __LINE__);
  if (digits_l(n.n_l) < BIGINTMAXSHORT)
    {
      for (i = 1; i <= digits_l(n.n_l); i++)
	{
	  if (read_ushort(s, &n.n_l[i]))
	    BINT::panic(BINT_EOF, ">>", 1, __LINE__);
	}
    }
  if (check_l(n.n_l) == 0)
    n.init = 1;
  else
    n.init = 0;
  return s;
}

/* Fonction de gestion des erreurs */

void BINT::panic(BINT_ERRORZ err, const char* const f, const int arg, const int line)
{
  cerr << "Erreur d'execution detectee par la classe BINT:\n" << endl;
  switch (err)
    {
    case BINT_OK:
      break;
    case BINT_EOF:
      cerr << "Erreur I/O, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_DBZ:
      cerr << "Division par 0, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_NHP:
      cerr << "Erreur dans new, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_OFL:
      cerr << "Overflow, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_UFL:
      cerr << "Underflow, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_VAL:
      cerr << "Argument: " << arg << " dans la fonction: " << f << " pas initialise" << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_BOR:
      cerr << "Base non-valide de l'argument: " << arg << ", dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_NPT:
      cerr << "Argument: " << arg << " dans la fonction: " << f << " est NULL" << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    case BINT_ERR:
      cerr << "Erreur inconnue, dans la fonction: " << f << endl;
      cerr << "Dans le module: " << __FILE__ << ", ligne: " << line << endl;
      break;
    }
  abort();
}

/* Fonction membre de purge */

void BINT::purge(void)
{
  if (init == 0)
    panic(BINT_VAL, "purge", 0, __LINE__);
  purge_l(n_l);
  setdigits_l(n_l, 0);
}

/* Fonction d'extraction du statut */

BINT_ERRORZ BINT::get_status(void)
{
  return status;
}


/* Fin de arithmcpp.cpp */
