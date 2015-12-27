/***********************************************************************
 *                                                                     *
 *       Code source pour la librairie de fonctions d'arithmétique     *
 *           élémentaire pour les grands entiers (4096 bits)           *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "arithm.h"


/* Fonction de purge des variables */

static void purgevars_l(int numvars, ...);

/* Fonctions opérant sur les digits */

USHORT digits_l(BIGINTD n_l)
{
  return *n_l;
}

USHORT *lsdptr_l(BIGINTD n_l)
{
  return (n_l + 1);
}

USHORT *msdptr_l(BIGINTD n_l)
{
  return (n_l + digits_l(n_l));
}

void setdigits_l(BIGINTD n_l, USHORT l)
{
  *n_l = l;
}

void decdigits_l(BIGINTD n_l)
{
  *n_l = *n_l - 1;
}

void incdigits_l(BIGINTD n_l)
{
  *n_l = *n_l + 1;
}

void rmldzeros_l(BIGINTD n_l)
{
  while ((digits_l(n_l) > 0) && (*msdptr_l(n_l) == 0))
    decdigits_l(n_l);
}

/* Fonctions d'initialisation et de comparaison */

void copy_l(BIGINTM dest_l, BIGINTM src_l)
{
  bigint *lastsrc_l = msdptr_l(src_l);
  
  *dest_l = *src_l;
  while ((*lastsrc_l == 0) && (*dest_l > 0))
    {
      --lastsrc_l;
      --*dest_l;
    }
  while (src_l < lastsrc_l)
    {
      dest_l++;
      src_l++;
      *dest_l = *src_l;
    }
}

void setzero_l(BIGINTM n_l)
{
  *n_l = 0;
}

void setone_l(BIGINTM n_l)
{
  *n_l = 1;
  *(n_l + 1) = 1;
}

void settwo_l(BIGINTM n_l)
{
  *n_l = 1;
  *(n_l + 1) = 2;
}

void setmax_l(BIGINT n_l)
{
  bigint *ptr_l;
  
  setdigits_l(n_l, BIGINTMAXDIGIT);
  ptr_l = lsdptr_l(n_l);
  while (ptr_l <= msdptr_l(n_l))
    {
      *ptr_l = 0xffff;
      ptr_l++;
    }
}

int comp_l(BIGINTM a_l, BIGINTM b_l)
{
  bigint *msdptra_l, *msdptrb_l;
  int c = 0;
  
  rmldzeros_l(a_l);
  rmldzeros_l(b_l);
  if (digits_l(a_l) > digits_l(b_l))
    c = 1;
  else if (digits_l(a_l) < digits_l(b_l))
    c = -1;
  else if (digits_l(a_l) == digits_l(b_l))
    {
      msdptra_l = msdptr_l(a_l);
      msdptrb_l = msdptr_l(b_l);
      while ((*msdptra_l == *msdptrb_l) && (msdptra_l > lsdptr_l(a_l)))
    {
  msdptra_l--;
  msdptrb_l--;
    }
      if (*msdptra_l > *msdptrb_l)
        c = 1;
      if (*msdptra_l < *msdptrb_l)
        c = -1;
      if (*msdptra_l == *msdptrb_l)
        c = 0;
    }
  return c;
}

int equ_l(BIGINTM a_l, BIGINTM b_l)
{
  bigint *msdptra_l, *msdptrb_l;

  rmldzeros_l(a_l);
  rmldzeros_l(b_l);
  if (digits_l(a_l) == 0 && digits_l(b_l) == 0)
    return 1;
  if (digits_l(a_l) != digits_l(b_l))
    return 0;
  else
    {
      msdptra_l = msdptr_l(a_l);
      msdptrb_l = msdptr_l(b_l);
      while ((*msdptra_l == *msdptrb_l) && (msdptra_l > a_l))
    {
      msdptra_l--;
      msdptrb_l--;
    }
      if (msdptra_l == a_l)
        return 1;
      else
        return 0;
    }
}

int eqz_l(BIGINTM a_l)
{
  rmldzeros_l(a_l);
  if (*a_l == 0)
    return 1;
  else
    return 0;
}

int eq1_l(BIGINT a_l)
{
  rmldzeros_l(a_l);
  if ((digits_l(a_l) == 1) && (*lsdptr_l(a_l) == 1))
    return 1;
  else
    return 0;
}

int ge_l(BIGINTM a_l, BIGINTM b_l)
{
  int c;

  c = comp_l(a_l, b_l);
  if ((c == 1) || (c == 0))
    return 1;
  else
    return 0;
}

int gtzero_l(BIGINTM a_l)
{
  BIGINT z_l;
  int c;
  
  setzero_l(z_l);
  c = comp_l(a_l, z_l);
  if (c == 1)
    return 1;
  else
    return 0;
}

int iseven_l(BIGINT n_l)
{
  if (digits_l(n_l) == 0 || (digits_l(n_l) > 0 && (*lsdptr_l(n_l) & 1) == 0))
    return 1;
  else
    return 0;
}

/* Fonctions kernel */

void add(BIGINT a_l, BIGINT b_l, BIGINTP s_l)
{
  bigint *msdptra_l, *msdptrb_l;
  register bigint *aptr_l, *bptr_l;
  bigint *sptr_l = lsdptr_l(s_l);
  ULONG carry = 0L;
  ULONG t = 0L;

  if (digits_l(a_l) < digits_l(b_l))
    {
      aptr_l = lsdptr_l(b_l);
      bptr_l = lsdptr_l(a_l);
      msdptra_l = msdptr_l(b_l);
      msdptrb_l = msdptr_l(a_l);
      setdigits_l(s_l, digits_l(b_l));
    }
  else
    {
      aptr_l = lsdptr_l(a_l);
      bptr_l = lsdptr_l(b_l);
      msdptra_l = msdptr_l(a_l);
      msdptrb_l = msdptr_l(b_l);
      setdigits_l(s_l, digits_l(a_l));
    }
  while (bptr_l <= msdptrb_l)
    {
      t = *aptr_l + *bptr_l + carry;
      *sptr_l = t & (B - 1);/* t mod B */
      carry = (t >> BITPERDGT);/* t div B */
      aptr_l = aptr_l + 1;
      bptr_l = bptr_l + 1;
      sptr_l = sptr_l + 1;
    }
  while (aptr_l <= msdptra_l)
    {
      t = *aptr_l + carry;
      *sptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      sptr_l = sptr_l + 1;
    }
  if (carry == 1)
    {
      *sptr_l = 1;
      setdigits_l(s_l, digits_l(s_l) + 1);
    }
  rmldzeros_l(s_l);

  purgevars_l(2, sizeof(carry), &carry, sizeof(t), &t);

}

void sub(BIGINT a_l, BIGINT b_l, BIGINT d_l)
{
  bigint *msdptra_l, *msdptrb_l;
  register bigint *aptr_l, *bptr_l;
  bigint *dptr_l;
  ULONG carry = 1L;
  ULONG t = 0L;

  aptr_l = lsdptr_l(a_l);
  bptr_l = lsdptr_l(b_l);
  dptr_l = lsdptr_l(d_l);
  msdptra_l = msdptr_l(a_l);
  msdptrb_l = msdptr_l(b_l);
  setdigits_l(d_l, digits_l(a_l));
  while (bptr_l <= msdptrb_l)
    {
      if (carry == 1)
        t = B + *aptr_l - *bptr_l;
      else
        t = B - 1 + *aptr_l - *bptr_l;
      *dptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      bptr_l = bptr_l + 1;
      dptr_l = dptr_l + 1;
    }
  while (aptr_l <= msdptra_l)
    {
      if (carry == 1)
        t = B + *aptr_l;
      else
        t = B - 1 + *aptr_l;
      *dptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      dptr_l = dptr_l + 1;
    }
  rmldzeros_l(d_l);

  purgevars_l(2, sizeof(carry), &carry, sizeof(t), &t);

}

void mul(BIGINT a_l, BIGINT b_l, BIGINTD p_l)
{
  register bigint *bptr_l, *pptr_l;
  bigint *aptr_l, *msdptra_l, *msdptrb_l;
  USHORT a0;
  ULONG carry = 0L;
  ULONG t = 0L;

  if (eqz_l(a_l) || eqz_l(b_l))
    {
      setzero_l(p_l);
      return;
    }

  pptr_l = lsdptr_l(p_l);
  aptr_l = lsdptr_l(a_l) + 1;
  bptr_l = lsdptr_l(b_l);
  msdptra_l = msdptr_l(a_l);
  msdptrb_l = msdptr_l(b_l);
 
  setdigits_l(p_l, digits_l(a_l) + digits_l(b_l));
 
  a0 = *lsdptr_l(a_l);
  while (bptr_l <= msdptrb_l)
    {
      t = a0 * (*bptr_l) + carry;
      *pptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      bptr_l = bptr_l + 1;
      pptr_l = pptr_l + 1;
    }
  *pptr_l = carry;

  while (aptr_l <= msdptra_l)
    {
      carry = 0;
      a0 = *aptr_l;
      bptr_l = lsdptr_l(b_l);
      pptr_l = lsdptr_l(p_l) + (aptr_l - lsdptr_l(a_l));
      while (bptr_l <= msdptrb_l)
        {
          t = a0 * (*bptr_l) + *pptr_l + carry;
          *pptr_l = t & (B - 1);
          carry = (t >> BITPERDGT);
          bptr_l = bptr_l + 1;
          pptr_l = pptr_l + 1;
        }
      *pptr_l = carry;
      aptr_l = aptr_l + 1;
    }
  rmldzeros_l(p_l);

  purgevars_l(3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0);

}

void umul(BIGINT a_l, USHORT b, BIGINTP p_l)
{
  register bigint *aptr_l, *pptr_l;
  bigint *msdptra_l;
  ULONG carry = 0L;
  ULONG t = 0L;
  
  if (eqz_l(a_l) || b == 0)
    {
      setzero_l(p_l);
      return;
    }

  pptr_l = lsdptr_l(p_l);
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);

  setdigits_l(p_l, digits_l(a_l) + 1);
  
  while (aptr_l <= msdptra_l)
    {
      t = *aptr_l * b + carry;
      *pptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      pptr_l = pptr_l + 1;
    }
  *pptr_l = carry;
  rmldzeros_l(p_l);

  purgevars_l(2, sizeof(carry), &carry, sizeof(t), &t);

}

void sqr(BIGINT a_l, BIGINTD p_l)             
{
  register bigint *cptr_l, *bptr_l;
  bigint *aptr_l, *csptr_l, *msdptra_l, *msdptrb_l, *msdptrc_l;
  USHORT a0;
  ULONG carry = 0L;
  ULONG t = 0L;

  if (eqz_l (a_l))
    {
      setzero_l (p_l);
      return;
    }

  msdptrb_l = msdptr_l(a_l);
  msdptra_l = msdptrb_l - 1;
  *lsdptr_l(p_l) = 0;
  a0 = *lsdptr_l(a_l);
  bptr_l = lsdptr_l(a_l) + 1;
  cptr_l = lsdptr_l(p_l) + 1;
  setdigits_l(p_l, 2 * digits_l (a_l));
  while (bptr_l <= msdptrb_l)
    {
      t = a0 * (*bptr_l) + carry;
      *cptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      bptr_l = bptr_l + 1;
      cptr_l = cptr_l + 1;
    }
  *cptr_l = (t >> BITPERDGT);

  aptr_l = lsdptr_l(a_l) + 1;
  csptr_l = lsdptr_l(p_l) + 3;
  while (aptr_l <= msdptra_l)
    {
      carry = 0;
      a0 = *aptr_l;
      bptr_l = aptr_l + 1;
      cptr_l = csptr_l;
      while (bptr_l <= msdptrb_l)
        {
          t = a0 * (*bptr_l) + *cptr_l + carry;
          *cptr_l = t & (B - 1);
          carry = (t >> BITPERDGT);
          bptr_l = bptr_l + 1;
          cptr_l = cptr_l + 1;
        }
      *cptr_l = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      csptr_l = csptr_l + 2;
    }

  msdptrc_l = cptr_l;
  carry = 0;
  cptr_l = lsdptr_l(p_l);
  while (cptr_l <= msdptrc_l)
    {
      t = 2 * (*cptr_l) + carry;
      *cptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      cptr_l = cptr_l + 1;
    }
  *cptr_l = (t >> BITPERDGT);

  carry = 0;
  bptr_l = lsdptr_l(a_l);
  cptr_l = lsdptr_l(p_l);
  while (bptr_l <= msdptrb_l)
    {
      t = *bptr_l * (*bptr_l) + *cptr_l + carry;
      *cptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      cptr_l = cptr_l + 1;
      t = *cptr_l + carry;
      *cptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      bptr_l = bptr_l + 1;
      cptr_l = cptr_l + 1;
    }
  rmldzeros_l(p_l);

  purgevars_l(3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0);

}

int sadd(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP s_l)
{
  int ssign;

  if (asign == 1)
    {
      if (bsign == 1)
        {
          add(a_l, b_l, s_l);
          ssign = 1;
        }
      else
      {
        if (comp_l(a_l, b_l) == -1)
          {
            sub(b_l, a_l, s_l);
            ssign = -1;
          }
          else
          {
            sub(a_l, b_l, s_l);
            ssign = 1;
          }
       }
    }
  else
    {
      if (bsign == 1)
        {
          if (comp_l(a_l, b_l) == 1)
            {
              sub(a_l, b_l, s_l);
              ssign = -1;
            }
            else
            {
              sub(b_l, a_l, s_l);
              ssign = 1;
            }
        }
      else
        {
          add(a_l, b_l, s_l);
          ssign = -1;
        }
    }
  return ssign;
}

int ssub(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP d_l)
{
  int ssign;

  if (asign == 1)
    {
      if (bsign == 1)
        {
          if (comp_l(a_l, b_l) == -1)
            {
              sub(b_l, a_l, d_l);
              ssign = -1;
            }
          else
            {
              sub(a_l, b_l, d_l);
              ssign = 1;
            }
        }
      else
        {
          add(a_l, b_l, d_l);
          ssign = 1;
        }
    }
  else
    {
      if (bsign == 1)
        {
          add(a_l, b_l, d_l);
          ssign = -1;
        }
      else
        {
          if (comp_l(a_l, b_l) == 1)
            {
              sub(a_l, b_l, d_l);
              ssign = -1;
            }
          else
            {
              sub(b_l, a_l, d_l);
              ssign = 1;
            }
        }
    }
  return ssign;
}

/* Fonctions arithmétiques élémentaires */

void u2bigint_l(BIGINT u_l, USHORT u)
{
  setdigits_l(u_l, 1);
  *lsdptr_l(u_l) = u;
}

void ul2bigint_l(BIGINT u_l, ULONG ul)
{
  setdigits_l(u_l, 2);
  *lsdptr_l(u_l) = ul & (B - 1);
  *(lsdptr_l(u_l) + 1) = (ul >> BITPERDGT);
}

int add_l(BIGINT aa_l, BIGINT bb_l, BIGINT s_l)
{
  BIGINTP ss_l;
  BIGINT a_l, b_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  add(a_l, b_l, ss_l);

  if (digits_l(ss_l) > BIGINTMAXDIGIT)
    {
      setdigits_l(ss_l, BIGINTMAXDIGIT);
      rmldzeros_l(ss_l);
      OFL = BIGINT_OFL;
    }
  copy_l(s_l, ss_l);

  purgevars_l(3, sizeof(ss_l), ss_l, sizeof(a_l), a_l, sizeof(b_l), b_l);

  return OFL;
}

int uadd_l(BIGINT a_l, USHORT b, BIGINT s_l)
{
  int OFL;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  OFL = add_l(a_l, t_l, s_l);

  purgevars_l(1, sizeof(t_l), t_l);

  return OFL;
}

int sub_l(BIGINT aa_l, BIGINT bb_l, BIGINT d_l)
{
  BIGINT  b_l;
  BIGINTP a_l, t_l, temp_l;
  int UFL = BIGINT_OK;

  copy_l(b_l, bb_l);
  
  if (comp_l(aa_l, bb_l) == -1)
    {
      setmax_l(a_l);
      copy_l(t_l, aa_l);
      UFL = BIGINT_UFL;
    }
  else
    copy_l(a_l, aa_l);

  sub(a_l, b_l, temp_l);

  if (UFL)
    {
      add_l(temp_l, t_l, d_l);
      uadd_l(d_l, 1, d_l);
    }
  else
    copy_l(d_l, temp_l);

  purgevars_l(4, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l, sizeof(temp_l), temp_l);

  return UFL;
}

int usub_l(BIGINT a_l, USHORT b, BIGINT d_l)
{
  int UFL;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  UFL = sub_l(a_l, t_l, d_l);

  purgevars_l(1, sizeof(t_l), t_l);

  return UFL;
}
  
int mul_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l)
{
  BIGINT a_l, b_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  mul(a_l, b_l, pp_l);

  if (digits_l(pp_l) > BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  purgevars_l(3, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(b_l), b_l);

  return OFL;
}

int umul_l(BIGINT aa_l, USHORT b, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD pp_l;
  register bigint *aptr_l, *pptr_l;
  bigint *msdptra_l;
  ULONG carry = 0L;
  ULONG t;
  int OFL = BIGINT_OK;
  
  if (eqz_l(aa_l) || b == 0)
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }

  copy_l(a_l, aa_l);
  pptr_l = lsdptr_l(pp_l);
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);

  setdigits_l(pp_l, digits_l(a_l) + 1);
  
  while (aptr_l <= msdptra_l)
    {
      t = *aptr_l * b + carry;
      *pptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
      pptr_l = pptr_l + 1;
    }
  *pptr_l = carry;
  rmldzeros_l(pp_l);
  if (digits_l(pp_l) > BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  purgevars_l(4, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(carry), &carry, sizeof(t), &t);

  return OFL;
}

int sqr_l(BIGINT aa_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);

  sqr(a_l, pp_l);

  if (digits_l(pp_l) > BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  purgevars_l(2, sizeof(pp_l), pp_l, sizeof(a_l), a_l);

  return OFL;
}

int div_l(BIGINTM aa_l, BIGINT bb_l, BIGINTM quot_l, BIGINT rem_l)
{
  register bigint *rptr_l, *bptr_l;
  BIGINT b_l;
  BIGINTM r_l;
  bigint *qptr_l, *msdptrb_l, *lsdptrr_l, *msdptrr_l;
  USHORT b0, r0, qhat, ri, ri_1, ri_2, bn, bn_1;
  ULONG right, rhat, borrow, carry, sbitsminusd;
  ULONG t = 0L;
  ULONG p = 0L;
  unsigned int d = 0;
  int c;

  copy_l(r_l, aa_l);
  rmldzeros_l(r_l);
  copy_l(b_l, bb_l);
  rmldzeros_l(b_l);

  if (eqz_l(b_l))
    {
      purgevars_l(2, sizeof(r_l), r_l, sizeof(b_l), b_l);

      return BIGINT_DBZ;
    }

  if (eqz_l(r_l))
    {
      setzero_l(quot_l);
      setzero_l(rem_l);

      purgevars_l(2, sizeof(r_l), r_l, sizeof(b_l), b_l);

      return BIGINT_OK;
    }

  c = comp_l(r_l, b_l);
  if (c == -1)
    {
      copy_l(rem_l, r_l);
      setzero_l(quot_l);

      purgevars_l(2, sizeof(r_l), r_l, sizeof(b_l), b_l);

      return BIGINT_OK;
    }
  else if (c == 0)
    {
      setone_l(quot_l);
      setzero_l(rem_l);

      purgevars_l(2, sizeof(r_l), r_l, sizeof(b_l), b_l);

      return BIGINT_OK;
    }

  if (digits_l(b_l) == 1)
    goto shortdiv;

  setdigits_l(quot_l , digits_l(r_l) - digits_l(b_l) + 1);
  msdptrb_l = msdptr_l(b_l);
  bn = *msdptrb_l;
  while (bn < BDIV2)
    {
      d = d + 1;
      bn = (bn << 1);
    }
  sbitsminusd = BITPERDGT - d;

  if (d > 0)
    {
      bn = bn + (*(msdptrb_l - 1) >> sbitsminusd); /* Report de la mult par 2^d dû à b_{n-2} */
      if (digits_l(b_l) > 2)
        bn_1 = (*(msdptrb_l - 1) << d) + (*(msdptrb_l - 2) >> sbitsminusd);
      else
        bn_1 = *(msdptrb_l - 1) << d;
    }
  else
    bn_1 = *(msdptrb_l - 1);

  msdptrr_l = msdptr_l(r_l) + 1;
  lsdptrr_l = msdptr_l(r_l) - digits_l(b_l) + 1;
  *msdptrr_l = 0;
  qptr_l = quot_l + digits_l(r_l) - digits_l(b_l) + 1;

  while (lsdptrr_l >= lsdptr_l(r_l))
    {
      ri = (*msdptrr_l << d) + (*(msdptrr_l - 1) >> sbitsminusd);
      ri_1 = (*(msdptrr_l - 1) << d) + (*(msdptrr_l - 2) >> sbitsminusd);
      if (msdptrr_l - 3 > r_l)
        ri_2 = (*(msdptrr_l - 2) << d) + (*(msdptrr_l - 3) >> sbitsminusd);
      else
        ri_2 = (*(msdptrr_l - 2) << d);

      if (ri != bn)
        {
          rhat = (ri << BITPERDGT) + ri_1;
          qhat = rhat / bn;
          rhat = rhat % bn;
          right = (rhat << BITPERDGT) + ri_2;
          if (((ULONG)bn_1 * qhat) > right)
            {
              qhat = qhat - 1;
              if ((rhat + bn) < B)
                {
                  if (((ULONG)bn_1 * qhat) > (right + (bn << BITPERDGT)))
                    qhat = qhat - 1;
                }
            }
        }
      else
        {
          rhat = bn + ri_1;
          qhat = B - 1;
          right = (rhat << BITPERDGT) + ri_2;
          if (rhat < B)
            {
              if (((ULONG)bn_1 * qhat) > right)
                {
                  qhat = qhat - 1;
                  if ((rhat + bn) < B)
                    {
                      if (((ULONG)bn_1 * qhat) > (right + (bn << BITPERDGT)))
                        qhat = qhat - 1;
                    }
                }
             }
          }

      borrow = 1;
      carry = 0;
      bptr_l = lsdptr_l(b_l);
      rptr_l = lsdptrr_l;
      while (bptr_l <= msdptrb_l)
        {
          p = *bptr_l * qhat + carry;
          if (borrow == 1)
            t = B + *rptr_l - (p & (B - 1));
           else
             t = B - 1 + *rptr_l - (p & (B - 1));
           *rptr_l = t & (B - 1);
           carry = (p >> BITPERDGT);
           borrow = (t >> BITPERDGT);
           bptr_l = bptr_l + 1;
           rptr_l = rptr_l + 1;
         }
      if (borrow == 1)
        t = B + *rptr_l - carry;
      else
        t = B - 1 + *rptr_l - carry;
      *rptr_l = t & (B - 1);
      borrow = (t >> BITPERDGT);
      if (borrow == 0)
        {
          carry = 0;
          bptr_l = lsdptr_l(b_l);
          rptr_l = lsdptrr_l;
          while (bptr_l <= msdptrb_l)
            {
              t = *rptr_l + *bptr_l + carry;
              *rptr_l = t & (B - 1);
              carry = (t >> BITPERDGT);
              bptr_l = bptr_l + 1;
              rptr_l = rptr_l + 1;
            }
          *rptr_l = *rptr_l + carry;

          qhat = qhat - 1;
        }

      *qptr_l = qhat;

      msdptrr_l = msdptrr_l - 1;
      lsdptrr_l = lsdptrr_l - 1;
      qptr_l = qptr_l - 1;
    }

  rmldzeros_l(quot_l);
  setdigits_l(r_l, digits_l(b_l));
  copy_l(rem_l, r_l);
  rmldzeros_l(rem_l);

  purgevars_l(17, sizeof(r_l), r_l, sizeof(b_l), b_l,
              sizeof(b0), &b0, sizeof(r0), &r0,
              sizeof(qhat), &qhat, sizeof(ri), &ri,
              sizeof(ri_1), &ri_1, sizeof(ri_2), &ri_2,
              sizeof(bn), &bn, sizeof(bn_1), &bn_1,
              sizeof(right), &right, sizeof(rhat), &rhat,
              sizeof(borrow), &borrow, sizeof(carry), &carry,
              sizeof(sbitsminusd), &sbitsminusd, sizeof(t), &t,
              sizeof(p), &p, sizeof(d), &d, sizeof(c), &c);

  return BIGINT_OK;

 shortdiv:
  r0 = 0;
  b0 = *lsdptr_l(b_l);
  rptr_l = msdptr_l(r_l);
  setdigits_l(quot_l, digits_l(r_l));
  qptr_l = msdptr_l(quot_l);
  while (rptr_l >= lsdptr_l(r_l))
    {
      t = (r0 << BITPERDGT) + (*rptr_l);
      *qptr_l = t / b0;
      r0 = t % b0;
      qptr_l = qptr_l - 1;
      rptr_l = rptr_l - 1;
    }
  rmldzeros_l(quot_l);
  u2bigint_l(rem_l, r0);
  rmldzeros_l(rem_l);

  purgevars_l(5, sizeof(r0), &r0, sizeof(b0), &b0, sizeof(b_l), b_l, sizeof(r_l), r_l, sizeof(t), &t);

  return BIGINT_OK;
}

int udiv_l(BIGINT aa_l, USHORT b, BIGINT quot_l, BIGINT rem_l)
{
  BIGINT a_l;
  register bigint *aptr_l, *qptr_l;
  USHORT r = 0;
  ULONG t;

  copy_l(a_l, aa_l);
  rmldzeros_l(a_l);
  if (b == 0)
    return BIGINT_DBZ;

  if (eqz_l(a_l))
    {
      setzero_l(quot_l);
      setzero_l(rem_l);
      return BIGINT_OK;
    }

  setdigits_l(quot_l, digits_l(a_l));
  aptr_l = msdptr_l(a_l);
  qptr_l = msdptr_l(quot_l);

  while (aptr_l >= lsdptr_l(a_l))
    {
      t = (r << BITPERDGT) + (*aptr_l);
      *qptr_l = t / b;
      r = t % b;
      qptr_l = qptr_l - 1;
      aptr_l = aptr_l - 1;
    }
  rmldzeros_l(quot_l);
  u2bigint_l(rem_l, r);

  purgevars_l(3, sizeof(r), &r, sizeof(r), &r, sizeof(a_l), a_l);

  return BIGINT_OK;
}
      
int mod_l(BIGINTM a_l, BIGINT b_l, BIGINT rem_l)
{
  BIGINTM dummy_l;
  int DBZ;

  DBZ = div_l(a_l, b_l, dummy_l, rem_l);

  purgevars_l(1, sizeof(dummy_l), dummy_l);

  return DBZ;
}

int mod2_l(BIGINT aa_l, ULONG k, BIGINT rem_l)
{
  int i;

  copy_l(rem_l, aa_l);
  if (k > BIGINTMAXBIT)
    return BIGINT_OK;

  i = 1 + (k >> LDBITPERDGT);
  if (i > digits_l(rem_l))
    return BIGINT_OK;

  rem_l[i] = rem_l[i] & ((1 << (k % BITPERDGT)) - 1);
  setdigits_l(rem_l, i);
  rmldzeros_l(rem_l);

  return BIGINT_OK;
}

USHORT umod_l(BIGINT aa_l, USHORT b)
{
  BIGINT a_l;
  bigint *aptr_l;
  USHORT r = 0;
  ULONG t;

  copy_l(a_l, aa_l);
  rmldzeros_l(a_l);
  if (b == 0)
    return 0xffff;

  if (eqz_l(a_l))
    return 0;

  aptr_l = msdptr_l(a_l);

  while (aptr_l >= lsdptr_l(a_l))
    {
      t = (r << BITPERDGT) + (*aptr_l);
      r = t % b;
      aptr_l = aptr_l - 1;
    }

  purgevars_l(2, sizeof(t), &t, sizeof(a_l), a_l);

  return r;
}

int smod(BIGINT a_l, int asign, BIGINT b_l, BIGINT rem_l)
{
  BIGINT q_l, r_l;

  if (eqz_l(b_l))
    return BIGINT_DBZ;
  div_l(a_l, b_l, q_l, r_l);
  if ((asign == -1) && gtzero_l(r_l))
    sub_l(b_l, r_l, r_l);
  copy_l(rem_l, r_l);

  purgevars_l(2, sizeof(q_l), q_l, sizeof(r_l), r_l);

  return 1;
}

/* Fonctions arithmétiques modulo */

int addmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT s_l)
{
  BIGINT a_l, b_l;
  BIGINTP t_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  add(a_l, b_l , t_l);
  if ((ge_l(a_l, m_l) == 1) || (ge_l(b_l, m_l) == 1))
    mod_l(t_l, m_l, s_l);
  else
    {
      if (ge_l(t_l, m_l) == 1)
        sub(t_l, m_l, t_l);
      copy_l(s_l, t_l);
    }

  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l);

  return BIGINT_OK;
}

int submod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT d_l)
{
  BIGINT a_l, b_l, t_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  if (ge_l(a_l, b_l) == 1)
    {
      sub(a_l, b_l, t_l);
      mod_l(t_l, m_l, d_l);
    }
  else
    {
      sub(b_l, a_l, t_l);
      mod_l(t_l, m_l, t_l);
      if (gtzero_l(t_l) == 1)
        sub(m_l, t_l, d_l);
      else
        setzero_l(d_l);
    }

  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l);

  return BIGINT_OK;
}

int mulmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, b_l;
  BIGINTD t_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);
  mul(a_l, b_l, t_l);
  mod_l(t_l, m_l, p_l);

  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l);

  return BIGINT_OK;
}

int sqrmod_l(BIGINT aa_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD t_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  sqr(a_l, t_l);
  mod_l(t_l, m_l, p_l);

  purgevars_l(2, sizeof(a_l), a_l, sizeof(t_l), t_l);

  return BIGINT_OK;
}

int uaddmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT s_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = addmod_l(a_l, t_l, m_l, s_l);

  purgevars_l(1, sizeof(t_l), t_l);

  return err;
}

int usubmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT d_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = submod_l(a_l, t_l, m_l, d_l);

  purgevars_l(1, sizeof(t_l), t_l);

  return err;
}

int umulmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT p_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = mulmod_l(a_l, t_l, m_l, p_l);

  purgevars_l(1, sizeof(t_l), t_l);

  return err;
}

int eqmod_l(BIGINT a_l, BIGINT b_l, BIGINT m_l)
{
  BIGINT t_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;
  submod_l(a_l, b_l, m_l, t_l);

  return eqz_l(t_l);
}

/* Fonctions exponentielles */

int uexpmod_l(BIGINT aa_l, USHORT e, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l;
  USHORT mask = BDIV2;

  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (e == 0)
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(aa_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  mod_l(aa_l, m_l, a_l);
  while ((e & mask) == 0)
    mask = (mask >> 1);
  mask = (mask >> 1);
  while (mask != 0)
    {
      sqrmod_l(a_l, m_l, a_l);
      if (e & mask)
        mulmod_l(a_l, aa_l, m_l, a_l);
      mask = (mask >> 1);
    }
  copy_l(p_l, a_l);

  purgevars_l(1, sizeof(a_l), a_l);

  return BIGINT_OK;
}

int wexpmod_l(USHORT a, BIGINT ee_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, e_l;
  USHORT k, b, w;

  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(ee_l))
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (a == 0)
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eq1_l(ee_l))
    {
      u2bigint_l(a_l, a);
      mod_l(a_l, m_l, p_l);
      return BIGINT_OK;
    }
  setone_l(a_l);
  copy_l(e_l, ee_l);
  b = (1 << ((numbits_l(e_l) - 1) & (BITPERDGT - 1)));
  w = e_l[digits_l(e_l)];
  while (b > 0)
    {
      sqrmod_l(a_l, m_l, a_l);
      if ((w & b) > 0)
	umulmod_l(a_l, a, m_l, a_l);
      b = (b >> 1);
    }
  for (k = digits_l(e_l) - 1; k > 0; k--)
    {
      w = e_l[k];
      b = BDIV2;
      while (b > 0)
        {
          sqrmod_l(a_l, m_l, a_l);
          if ((w & b) > 0)
            umulmod_l(a_l, a, m_l, a_l);
          b = (b >> 1);
        }
    }
  copy_l(p_l, a_l);

  purgevars_l(5, sizeof(a_l), a_l, sizeof(e_l), e_l,
              sizeof(b), &b, sizeof(w), &w, sizeof(k), &k);

  return BIGINT_OK;
}

static int twotab[] =
{0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0,
 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0,
 3, 0, 1, 0, 2, 0, 1, 0, 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 6, 0, 1, 0, 2, 0, 1, 0,
 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

static USHORT oddtab[] =
{0, 1, 1, 3, 1, 5, 3, 7, 1, 9, 5, 11, 3, 13, 7, 15, 1, 17, 9, 19, 5, 21, 11, 23, 3, 25, 13, 27, 7, 29, 15, 31, 1,
 33, 17, 35, 9, 37, 19, 39, 5, 41, 21, 43, 11, 45, 23, 47, 3, 49, 25, 51, 13, 53, 27, 55, 7, 57, 29, 59, 15,
 61, 31, 63, 1, 65, 33, 67, 17, 69, 35, 71, 9, 73, 37, 75, 19, 77, 39, 79, 5, 81, 41, 83, 21, 85, 43, 87, 11,
 89, 45, 91, 23, 93, 47, 95, 3, 97, 49, 99, 25, 101, 51, 103, 13, 105, 53, 107, 27, 109, 55, 111, 7, 113,
 57, 115, 29, 117, 59, 119, 15, 121, 61, 123, 31, 125, 63, 127, 1, 129, 65, 131, 33, 133, 67, 135, 17,
 137, 69, 139, 35, 141, 71, 143, 9, 145, 73, 147, 37, 149, 75, 151, 19, 153, 77, 155, 39, 157, 79, 159,
 5, 161, 81, 163, 41, 165, 83, 167, 21, 169, 85, 171, 43, 173, 87, 175, 11, 177, 89, 179, 45, 181, 91,
 183, 23, 185, 93, 187, 47, 189, 95, 191, 3, 193, 97, 195, 49, 197, 99, 199, 25, 201, 101, 203, 51, 205,
 103, 207, 13, 209, 105, 211, 53, 213, 107, 215, 27, 217, 109, 219, 55, 221, 111, 223, 7, 225, 113,
 227, 57, 229, 115, 231, 29, 233, 117, 235, 59, 237, 119, 239, 15, 241, 121, 243, 61, 245, 123, 247, 31,
 249, 125, 251, 63, 253, 127, 255};

int expmodk_l(BIGINT aa_l, BIGINT ee_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, a2_l;
  BIGINTP e_l;
  BIGINTD temp_l;
  bigint **pptr_l, *ptr_l = NULL;
  int i, f, tab;
  unsigned int k, lge, pow2k, mask, fk, s, d, digit;

  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(ee_l))
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(aa_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  copy_l(a_l, aa_l);
  copy_l(e_l, ee_l);
  rmldzeros_l(a_l);
  rmldzeros_l(e_l);
  lge = numbits_l(e_l);
  k = 8;
  while (k > 1 && ((k - 1) * (k << ((k - 1) << 1))/((1 << k) - k - 1)) >= lge - 1)
    k = k - 1;
  pow2k = (1 << k);
  mask = pow2k - 1;
  pptr_l = (bigint **)malloc(sizeof(bigint *) * pow2k);
  if (pptr_l  == NULL)
    {
      purgevars_l(2, sizeof(a_l), a_l, sizeof(e_l), e_l);
      return BIGINT_MAL;
    }
  mod_l(a_l, m_l, a_l);
  pptr_l[1] = a_l;
  if (k > 1)
    {
      ptr_l = (bigint *)malloc(sizeof(BIGINT) * ((pow2k >> 1) - 1));
      if (ptr_l == NULL)
        {
          free(pptr_l);
          purgevars_l(2, sizeof(a_l), a_l, sizeof(e_l), e_l);
          return BIGINT_MAL;
        }
      pptr_l[2] = a2_l;
      pptr_l[3] = ptr_l;
      for (i = 5; i < (int)pow2k; i+=2)
        pptr_l[i] = pptr_l[i - 2] + BIGINTMAXSHORT;
      sqrmod_l(a_l, m_l, pptr_l[2]);
      for (i = 3; i < (int)pow2k; i+=2)
        mulmod_l(pptr_l[2], pptr_l[i - 2], m_l, pptr_l[i]);
      }
  *(msdptr_l(e_l) + 1) = 0;
  f = (lge - 1)/k;
  fk = f * k;
  s = (fk >> LDBITPERDGT);
  d = fk & (BITPERDGT - 1);
  digit = (((e_l[s + 2] << BITPERDGT) + e_l[s + 1]) >> d) & mask;
  if (digit != 0)
    {
      copy_l(temp_l, pptr_l[oddtab[digit]]);
      tab = twotab[digit];
      for (i = 1; i <= tab; i++)
        sqrmod_l(temp_l, m_l, temp_l);
    }
  else
    setone_l(temp_l);
  fk = fk - k;
  for (f = f - 1; f >= 0; f--)
    {
      s = (fk >> LDBITPERDGT);
      d = fk & (BITPERDGT - 1);
      digit = (((e_l[s + 2] << BITPERDGT) + e_l[s + 1]) >> d) & mask;
      if (digit != 0)
        {
	  tab = twotab[digit];
	  for (i = 1; i <= ((int)k - tab); i++)
	    sqrmod_l(temp_l, m_l, temp_l);
	  mulmod_l(temp_l, pptr_l[oddtab[digit]], m_l, temp_l);
	  for (i = 1; i <= tab; i++)
	    sqrmod_l(temp_l, m_l, temp_l);
	}
      else
	{
	  for (i = 1; i <= (int)k; i++)
	    sqrmod_l(temp_l, m_l, temp_l);
	}
      fk = fk - k;
    }
  copy_l(p_l, temp_l);
  free(pptr_l);
  if (ptr_l != NULL)
    {
      memset(ptr_l, 0, sizeof(BIGINT) * ((pow2k >> 1) - 1));
      free(ptr_l);
    }

  purgevars_l(13, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	      sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	      sizeof(tab), &tab, sizeof(k), &k,
	      sizeof(lge), &lge, sizeof(pow2k), &pow2k,
	      sizeof(mask), &mask, sizeof(fk), &fk,
	      sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit);

  return BIGINT_OK;
}

int expmod2k_l(BIGINT aa_l, USHORT k, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l;

  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (k > 0)
    {
      copy_l(a_l, aa_l);
      while (k > 0)
	{
	  sqrmod_l(a_l, m_l, a_l);
	  k = k - 1;
	}
      copy_l(p_l, a_l);
    }
  else
    mod_l(aa_l, m_l, p_l);

  purgevars_l(1, sizeof(a_l), a_l);

  return BIGINT_OK;
}

/* Fonctions opérant sur les bits */

unsigned int numbits_l(BIGINT aa_l)
{
  BIGINT a_l;
  unsigned int l;
  USHORT temp;

  copy_l(a_l, aa_l);
  rmldzeros_l(a_l);
  l = digits_l(a_l);
  if (l == 0)
    {
      purgevars_l(1, sizeof(a_l), a_l);
      
      return 0;
    }
  temp = a_l[l];
  l = (l << LDBITPERDGT);
  while ((temp & BDIV2) == 0)
    {
      temp = (temp << 1);
      l = l - 1;
    }

  purgevars_l(2, sizeof(a_l), a_l, sizeof(temp), &temp);

  return l;
}

int shleft_l(BIGINT a_l)
{
  bigint *aptr_l, *msdptra_l;
  ULONG carry = 0L;
  ULONG t = 0L;
  int err = BIGINT_OK;

  rmldzeros_l(a_l);
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);
  while (aptr_l <= msdptra_l)
    {
      t = (*aptr_l << 1) + carry;
      *aptr_l = t & (B - 1);
      carry = (t >> BITPERDGT);
      aptr_l = aptr_l + 1;
    }
  if (carry)
    {
      if (digits_l(a_l) < BIGINTMAXDIGIT)
	{
	  *aptr_l = 1;
	  setdigits_l(a_l, digits_l(a_l) + 1);
	  err = BIGINT_OK;
	}
      else
	err = BIGINT_OFL;
    }
  rmldzeros_l(a_l);

  purgevars_l(2, sizeof(carry), &carry, sizeof(t), &t);

  return err;
}

int shright_l(BIGINT a_l)
{
  bigint *aptr_l, *msdptra_l;
  ULONG t = 0L;
  ULONG carry = 0L;
  int err = BIGINT_OK;

  if (eqz_l(a_l))
    err = BIGINT_UFL;
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);
  while (msdptra_l >= aptr_l)
    {
      t = (*msdptra_l >> 1) + (carry << (BITPERDGT - 1));
      carry = *msdptra_l & 1;
      *msdptra_l = t;
      msdptra_l = msdptra_l - 1;
    }
  rmldzeros_l(a_l);

  purgevars_l(2, sizeof(carry), &carry, sizeof(t), &t);

  return err;
}

int shift_l(BIGINT a_l, long int nbrebits)
{
  bigint *aptr_l, *msdptra_l;
  USHORT ints, bits;
  long int res;
  int i;
  int err = BIGINT_OK;

  rmldzeros_l(a_l);
  if (nbrebits < 0)
    {
      ints = (-nbrebits) / BITPERDGT;
      bits = (-nbrebits) % BITPERDGT;
    }
  else
    {
      ints = nbrebits / BITPERDGT;
      bits = nbrebits % BITPERDGT;
    }
  res = numbits_l(a_l) + nbrebits;
  if (digits_l(a_l) == 0)
    {
      if (res < 0)
	err = BIGINT_UFL;
      return err;
    }
  if (nbrebits == 0)
    return err;
  if ((res < 0) || (res > (long)BIGINTMAXBIT))
    {
      if (res < 0)
	err = BIGINT_UFL;
      else
	err = BIGINT_OFL;
    }
  if (nbrebits < 0)
    {
      if (digits_l(a_l) < ints)
	ints  = digits_l(a_l);
      msdptra_l = msdptr_l(a_l) - ints;
      aptr_l = lsdptr_l(a_l);
      while (aptr_l <= msdptra_l)
	{
	  *aptr_l = *(aptr_l + ints);
	  aptr_l = aptr_l + 1;
	}
      setdigits_l(a_l, digits_l(a_l) - ints);
      for (i = 0; i < bits; i++)
	shright_l(a_l);
    }
  else
    {
      if (ints < BIGINTMAXDIGIT)
	{
	  if (digits_l(a_l) + ints < (short)BIGINTMAXDIGIT)
	    setdigits_l(a_l, digits_l(a_l) + ints);
	  else
	    setdigits_l(a_l, BIGINTMAXDIGIT);
	  msdptra_l = msdptr_l(a_l);
	  aptr_l = a_l + ints;
	  while (msdptra_l > aptr_l)
	    {
	      *msdptra_l = *(msdptra_l - ints);
	      msdptra_l = msdptra_l - 1;
	    }
	  while (msdptra_l > a_l)
	    {
	      *msdptra_l = 0;
	      msdptra_l = msdptra_l - 1;
	    }
	  for (i = 0; i < bits; i++)
	    shleft_l(a_l);
	} 
      else
	setzero_l(a_l);
    }

  purgevars_l(3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(res), &res);

  return err;
}

void and_l(BIGINT a_l, BIGINT b_l, BIGINT c_l)
{
  BIGINT t_l;
  bigint *aptr_l, *bptr_l, *tptr_l, *msdptra_l;

  if (digits_l(a_l) < digits_l(b_l))
    {
      aptr_l = lsdptr_l(a_l);
      bptr_l = lsdptr_l(b_l);
      msdptra_l = msdptr_l(a_l);
    }
  else
    {
      aptr_l = lsdptr_l(b_l);
      bptr_l = lsdptr_l(a_l);
      msdptra_l = msdptr_l(b_l);
    }
  setdigits_l(t_l, *(aptr_l - 1));
  tptr_l = lsdptr_l(t_l);
  while (aptr_l <= msdptra_l)
    {
      *tptr_l = *aptr_l & *bptr_l;
      tptr_l = tptr_l + 1;
      aptr_l = aptr_l + 1;
      bptr_l = bptr_l + 1;
    }
  copy_l(c_l, t_l);

  purgevars_l(1, sizeof(t_l), t_l);

}

void or_l(BIGINT a_l, BIGINT b_l, BIGINT c_l)
{
  BIGINT t_l;
  bigint *aptr_l, *bptr_l, *tptr_l, *msdptra_l, *msdptrb_l;

  if (digits_l(a_l) < digits_l(b_l))
    {
      aptr_l = lsdptr_l(a_l);
      bptr_l = lsdptr_l(b_l);
      msdptra_l = msdptr_l(a_l);
      msdptrb_l = msdptr_l(b_l);
    }
  else
    {
      aptr_l = lsdptr_l(b_l);
      bptr_l = lsdptr_l(a_l);
      msdptra_l = msdptr_l(b_l);
      msdptrb_l = msdptr_l(a_l);
    }
  setdigits_l(t_l, *(bptr_l - 1));
  tptr_l = lsdptr_l(t_l);
  while (aptr_l <= msdptra_l)
    {
      *tptr_l = *aptr_l | *bptr_l;
      tptr_l = tptr_l + 1;
      aptr_l = aptr_l + 1;
      bptr_l = bptr_l + 1;
    }
  while (bptr_l <= msdptrb_l)
    {
      *tptr_l = *bptr_l;
      tptr_l = tptr_l + 1;
      bptr_l = bptr_l + 1;
    }
  copy_l(c_l, t_l);

  purgevars_l(1, sizeof(t_l), t_l);

}

void xor_l(BIGINT a_l, BIGINT b_l, BIGINT c_l)
{
  BIGINT t_l;
  bigint *aptr_l, *bptr_l, *tptr_l, *msdptra_l, *msdptrb_l;

  if (digits_l(a_l) < digits_l(b_l))
    {
      aptr_l = lsdptr_l(a_l);
      bptr_l = lsdptr_l(b_l);
      msdptra_l = msdptr_l(a_l);
      msdptrb_l = msdptr_l(b_l);
    }
  else
    {
      aptr_l = lsdptr_l(b_l);
      bptr_l = lsdptr_l(a_l);
      msdptra_l = msdptr_l(b_l);
      msdptrb_l = msdptr_l(a_l);
    }
  setdigits_l(t_l, *(bptr_l - 1));
  tptr_l = lsdptr_l(t_l);
  while (aptr_l <= msdptra_l)
    {
      *tptr_l = *aptr_l ^ *bptr_l;
      tptr_l = tptr_l + 1;
      aptr_l = aptr_l + 1;
      bptr_l = bptr_l + 1;
    }
  while (bptr_l <= msdptrb_l)
    {
      *tptr_l = *bptr_l;
      tptr_l = tptr_l + 1;
      bptr_l = bptr_l + 1;
    }
  copy_l(c_l, t_l);

  purgevars_l(1, sizeof(t_l), t_l);

}

int setbit_l(BIGINT a_l, unsigned int pos)
{
  int i;
  USHORT ints = (pos / BITPERDGT);
  USHORT bits = (pos % BITPERDGT);
  USHORT mask = (1 << bits);
  int ret = 0;

  if (pos > BIGINTMAXBIT)
    return BIGINT_OFL;
  if (ints >= digits_l(a_l))
    {
      for (i = digits_l(a_l) + 1; i <= ints + 1; i++)
	a_l[i] = 0;
      setdigits_l(a_l, ints  + 1);
    }
  if (a_l[ints  + 1] & mask)
    ret = 1;
  a_l[ints + 1] = a_l[ints  + 1] | mask;

  purgevars_l(3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask);

  return ret;
}

int testbit_l(BIGINT a_l, unsigned int pos)
{
  USHORT ints = (pos / BITPERDGT);
  USHORT bits = (pos % BITPERDGT);
  USHORT mask = (1 << bits);
  int ret = 0;

  if (ints < digits_l(a_l))
    {
      if (a_l[ints + 1] & mask)
	ret = 1;
    }

  purgevars_l(3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask);

  return ret;
}

int clearbit_l(BIGINT a_l, unsigned int pos)
{
  USHORT ints = (pos / BITPERDGT);
  USHORT bits = (pos % BITPERDGT);
  USHORT mask = (1 << bits);
  int ret = 0;

  if (ints < digits_l(a_l))
    {
      if (a_l[ints + 1] & mask)
	ret = 1;
      a_l[ints + 1] = a_l[ints + 1] & (~mask);
      rmldzeros_l(a_l);
    }

  purgevars_l(3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask);

  return ret;
}

/* Fonctions d'entrée-sortie */

int str2bigint_l(BIGINT n_l, char *str, USHORT b)
{
  bigint b_l[10];
  USHORT n;
  int err = BIGINT_OK;
  
  if (str == NULL)
    return BIGINT_NPT;
  if (b < 2 || b > 16)
    return BIGINT_BOR;
  u2bigint_l(b_l, b);
  setzero_l(n_l);
  if (*str == '0')
    {
      if ((*(str + 1) == 'x') || (*(str + 1) == 'b'))
	str = str + 2;
    }
  while (isxdigit(*str) || isspace(*str))
    {
      if (!isspace(*str))
	{
	  n = *str;
	  switch (n)
	    {
	    case 'a':
	    case 'b':
	    case 'c':
	    case 'd':
	    case 'e':
	    case 'f':
	      n = n - 'a' + 10;
	      break;
	    default:
	      n = n - '0';
	    }
	  if (n >= b)
	    {
	      err = BIGINT_BOR;
	      break;
	    }
	  err = mul_l(n_l, b_l, n_l);
	  if (err != BIGINT_OK)
	    break;
	  err = uadd_l(n_l, n, n_l);
	  if (err != BIGINT_OK)
	    break;
	}
      str = str + 1;
    }
  return err;
}

static char ntable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

char *bigint2str_l(BIGINT n_l, USHORT b, int showb)
{
  BIGINT t_l, r_l;
  bigint b_l[10];
  int i = 0;
  static char n[BIGINTMAXBIT + 3];
  char temp, *begin, *end;

  if (b < 2 || b > 16)
    return NULL;
  u2bigint_l(b_l, b);
  copy_l(t_l, n_l);
  do
    {
      div_l(t_l, b_l, t_l, r_l);
      if (gtzero_l(r_l))
	{
	  n[i] = ntable[*lsdptr_l(r_l)];
	  i = i + 1;
	}
      else
	{
	  n[i] = '0';
	  i = i + 1;
	}
    }
  while (gtzero_l(t_l));
  if (showb)
    {
      switch (b)
	{
	case 2:
	  n[i] = 'b';
	  i = i + 1;
	  n[i] = '0';
	  i = i + 1;
	  break;
	case 8:
	  n[i] = '0';
	  i = i + 1;
	  break;
	case 16:
	  n[i] = 'x';
	  i = i + 1;
	  n[i] = '0';
	  i = i + 1;
	  break;
	}
    }
  n[i] = '\0';
  begin = n;
  end = n + strlen(n) - 1;
  for (; end > begin; begin++)
    {
       temp = *begin;
       *begin = *end;
       *end = temp;
       end = end - 1;
    }
  return n;
}

int byte2bigint_l(BIGINT n_l, UCHAR* bytestr, int len)
{
  int i, err = BIGINT_OK;

  if (bytestr == NULL)
    return BIGINT_NPT;
  if ((unsigned)len > BIGINTMAXBYTE)
    return BIGINT_OFL;
  setzero_l(n_l);
  for (i = 0; i < len; i++)
    {
      err = umul_l(n_l, 256, n_l);
      if (err != BIGINT_OK)
	return err;
      err = uadd_l(n_l, *bytestr, n_l);
      if (err != BIGINT_OK)
	return err;
      bytestr = bytestr + 1;
    }
  return err;
}

UCHAR *bigint2byte_l(BIGINT n_l, int *len)
{
  BIGINT t_l, r_l;
  int i = 0, j;
  static UCHAR bytes[BIGINTMAXBYTE];
  UCHAR temp;

  if (len == NULL)
    return NULL;
  copy_l(t_l, n_l);
  do
    {
      udiv_l(t_l, 256, t_l, r_l);
      if (gtzero_l(r_l))
	{
	  bytes[i] = (unsigned char)*lsdptr_l(r_l);
	  i = i + 1;
	}
      else
	{
	  bytes[i] = 0;
	  i = i + 1;
	}
    }
  while (gtzero_l(t_l));
  *len = i;
  j = *len - 1;
  for (i = 0; i < j; i++)
    {
      temp = bytes[i];
      bytes[i] = bytes[j];
      bytes[j] = temp;
      j = j - 1;
    }

  purgevars_l(5, sizeof(t_l), t_l, sizeof(r_l), r_l,
	      sizeof(i), &i, sizeof(j), &j,
	      sizeof(temp), &temp);

  return bytes;
}

int check_l(BIGINT n_l)
{
  int err = CHECK_OK;
  
  if (n_l == NULL)
    err = CHECK_MEM;
  else
    {
      if (digits_l(n_l) > BIGINTMAXDIGIT)
	err = CHECK_OFL;
      else
	{
	  if ((digits_l(n_l) > 0) && (*msdptr_l(n_l) == 0))
	    err = CHECK_LDZ;
	}
    }
  return err;
}

/* Fonctions de théorie des nombres */

void gcd_l(BIGINT aa_l, BIGINT bb_l, BIGINT g_l)
{
  BIGINT a_l, b_l, t_l;
  unsigned int k = 0;
  int sign;
  
  if (comp_l(aa_l, bb_l) == -1)
    {
      copy_l(a_l, bb_l);
      copy_l(b_l, aa_l);
    }
  else
    {
      copy_l(a_l, aa_l);
      copy_l(b_l, bb_l);
    }
  if (eqz_l(b_l))
    {
      copy_l(g_l, a_l);

      purgevars_l(2, sizeof(a_l), a_l, sizeof(b_l), b_l);

      return;
    }
  while (iseven_l(a_l) && iseven_l(b_l))
    {
      shright_l(a_l);
      shright_l(b_l);
      k = k + 1;
    }
  while (iseven_l(a_l))
    shright_l(a_l);
  while (iseven_l(b_l))
    shright_l(b_l);
  do
    {
      if (ge_l(a_l, b_l))
	{
	  sub_l(a_l, b_l, t_l);
	  sign = 1;
	}
      else
	{
	  sub_l(b_l, a_l, t_l);
	  sign = -1;
	}
      if (eqz_l(t_l))
	{
	  copy_l(g_l, a_l);
	  shift_l(g_l, k);

	  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l);

	  return;
	}
      while (iseven_l(t_l))
	shright_l(t_l);
      if (sign == 1)
	copy_l(a_l, t_l);
      else if (sign == -1)
	copy_l(b_l, t_l);
    }
  while (eqz_l(t_l) == 0);

  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l);

}

int lcm_l(BIGINT a_l, BIGINT b_l, BIGINT l_l)
{
  BIGINT g_l, dummy_l;
  int err = BIGINT_OK;

  if (eqz_l(a_l) || eqz_l(b_l))
    {
      setzero_l(l_l);
      return err;
    }
  gcd_l(a_l, b_l, g_l);
  div_l(a_l, g_l, g_l, dummy_l);
  err = mul_l(b_l, g_l, l_l);

  purgevars_l(2, sizeof(g_l), g_l, sizeof(dummy_l), dummy_l);

  return err;
}

void xgcd_l(BIGINT a_l, BIGINT b_l, BIGINT g_l, BIGINTD u_l, int *usign, BIGINTD v_l, int *vsign)
{
  BIGINT v1_l, v3_l, t3_l, q_l;
  BIGINTP t1_l;
  BIGINTD temp_l, utemp_l, vtemp_l;
  int v1sign, t1sign;

  copy_l(g_l, a_l);
  copy_l(v3_l, b_l);
  if (eqz_l(v3_l))
    {
      setone_l(u_l);
      setzero_l(v_l);
      *usign = 1;
      *vsign = 1;

      purgevars_l(1, sizeof(v3_l), v3_l);

      return;
    }
  setone_l(utemp_l);
  *usign = 1;
  setzero_l(v1_l);
  v1sign = 1;
  while (gtzero_l(v3_l))
    {
      div_l(g_l, v3_l, q_l, t3_l);
      mul_l(v1_l, q_l, q_l);
      t1sign = ssub(utemp_l, *usign, q_l, v1sign, t1_l);
      copy_l(utemp_l, v1_l);
      *usign = v1sign;
      copy_l(g_l, v3_l);
      copy_l(v1_l, t1_l);
      v1sign = t1sign;
      copy_l(v3_l, t3_l);
    }
  mul(a_l, utemp_l, temp_l);
  *vsign = ssub(g_l, 1, temp_l, *usign, temp_l);
  div_l(temp_l, b_l, vtemp_l, temp_l);
  copy_l(u_l, utemp_l);
  copy_l(v_l, vtemp_l);

  purgevars_l(8, sizeof(v1_l), v1_l, sizeof(v3_l), v3_l,
	      sizeof(t3_l), t3_l, sizeof(q_l), q_l,
	      sizeof(t1_l), t1_l, sizeof(temp_l), temp_l,
	      sizeof(utemp_l), utemp_l, sizeof(vtemp_l), vtemp_l);

  return;
} 

void invmod_l(BIGINT a_l, BIGINT m_l, BIGINT g_l, BIGINT i_l)
{
  BIGINT t0_l, u0_l, u1_l, q_l, temp_l, r_l; /* g_l joue le rôle de t1_l (Voir Mignotte) */

  if (eqz_l(a_l))
    {
      if (eqz_l(m_l))
	{    
	  setzero_l(g_l);
	  setzero_l(i_l);
	  return;
	}
      else
	{
	  copy_l(g_l, m_l);
	  setzero_l(i_l);
	  return;
	}
    }
  else
    {
      if (eqz_l(m_l))
	{
	  copy_l(g_l, a_l);
	  setzero_l(i_l);
	  return;
	}
    }
  copy_l(t0_l, a_l);
  copy_l(g_l, m_l);
  setone_l(u0_l);
  setzero_l(u1_l);
  do
    {
      div_l(t0_l, g_l, q_l, r_l);
      if (gtzero_l(r_l))
	{	  
	  copy_l(temp_l, u1_l);
	  mulmod_l(u1_l, q_l, m_l, q_l);
	  submod_l(u0_l, q_l, m_l, u1_l);
	  copy_l(u0_l, temp_l);
	  copy_l(t0_l, g_l);
	  copy_l(g_l, r_l);
	}
    }
  while (gtzero_l(r_l));
  if (eq1_l(g_l))
    copy_l(i_l, u1_l);
  else
    setzero_l(i_l);

  purgevars_l(6, sizeof(t0_l), t0_l, sizeof(u0_l), u0_l,
	      sizeof(u1_l), u1_l, sizeof(q_l), q_l,
	      sizeof(temp_l), temp_l, sizeof(r_l), r_l);

}

void introot_l(BIGINT n_l, BIGINT r_l)
{
  BIGINT x_l, y_l, dummy_l;
  unsigned int l;
  int c;

  l = (numbits_l(n_l) + 1) >> 1; /* numbits = log + 1 */
  setzero_l(x_l);
  setbit_l(x_l, l);
  do
    {
      div_l(n_l, x_l, y_l, dummy_l);
      add_l(y_l, x_l, y_l);
      shright_l(y_l);
      c = comp_l(y_l, x_l);
      if (c == -1)
	copy_l(x_l, y_l);
    }
  while (c == -1);
  copy_l(r_l, x_l);

  purgevars_l(3, sizeof(x_l), x_l, sizeof(y_l), y_l, sizeof(dummy_l), dummy_l);

}

ULONG uintroot_l(ULONG n)
{
  ULONG x, y;

  if (n == 0)
    return 0;
  x = 1;
  y = 0;
  while (y <= n)
    {
      x = (x << 1);
      y = x * x;
    }
  do
    {
      y = x;
      x = (x * x + n) / (2 * x);
    }
  while (x < y);

  purgevars_l(1, sizeof(y), &y);

  return x;
}

static const UCHAR q11[11]=
  {1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0};

static const UCHAR q63[63]=
  {1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1,
   0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
   0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};

static const UCHAR q64[64]=
  {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
   0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
   0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

static const UCHAR q65[65]=
  {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
   1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
   0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1};

unsigned int issqr_l(BIGINT n_l, BIGINT r_l)
{
  BIGINT q_l;
  USHORT r;

  if (eqz_l(n_l))
    {
      setzero_l(r_l);
      return 1;
    }
  if (q64[*lsdptr_l(n_l) % 64] == 1)
    {
      r = umod_l(n_l, 45045);
      if ((q63[r % 63] == 1) && (q65[r % 65] == 1) && (q11[r % 11] == 1))
	{
	  introot_l(n_l, r_l);
	  sqr_l(r_l, q_l);
	  if (equ_l(n_l, q_l))
	    {

	      purgevars_l(1, sizeof(q_l), q_l);
	      
	      return 1;
	    }
	}
    }
  setzero_l(r_l);

  purgevars_l(1, sizeof(q_l), q_l);

  return 0;
}

static int tab2[] = {0, 1, 0, -1, 0, -1, 0, 1};

int jacobi_l(BIGINT aa_l, BIGINT bb_l)
{
  BIGINT a_l, b_l, r_l;
  long int k, v;

  if (eqz_l(bb_l))
    {
      if (eq1_l(aa_l))
	return 1;
      else
	return 0;
    }
  if (iseven_l(aa_l) && iseven_l(bb_l))
    return 0;
  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);
  v = 0;
  while(iseven_l(b_l))
    {
      shright_l(b_l);
      v = v + 1;
    }
  if ((v & 1) == 0)
    k = 1;
  else
    k = tab2[*lsdptr_l(a_l) % 8]; /* *lsdptr_l(a_l) & 7 == a_l % 8 */
  while (gtzero_l(a_l))
    {
      v = 0;
      while(iseven_l(a_l))
	{
	  shright_l(a_l);
	  v = v + 1;
	}
      if ((v & 1) != 0)
	k = tab2[*lsdptr_l(b_l) % 8] * k;
      if ((*lsdptr_l(a_l) % 4 == 3) && (*lsdptr_l(b_l) % 4 == 3))
	k = -k;
      copy_l(r_l, a_l);
      mod_l(b_l, r_l, a_l);
      copy_l(b_l, r_l);
    }
  setone_l(r_l);
  if (comp_l(b_l, r_l) == 1)
    k = 0;

  purgevars_l(3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(r_l), r_l);

  return k;
}

int proot_l(BIGINT a_l, BIGINT p_l, BIGINT x_l)
{
  BIGINT n_l, q_l, t_l, y_l, z_l, temp_l;
  int r, m;

  if (eqz_l(p_l) || iseven_l(p_l))
    return -1;
  if (eqz_l(a_l))
    {
      setzero_l(a_l);
      return 0;
    }
  copy_l(q_l, p_l);
  usub_l(q_l, 1, q_l);
  r = 0;
  while (iseven_l(q_l))
    {
      shright_l(q_l);
      r = r + 1;
    }
  setone_l(n_l);
  shleft_l(n_l);
  while (jacobi_l(n_l, p_l) == 1)
    uadd_l(n_l, 1, n_l);
  expmodk_l(n_l, q_l, p_l, y_l);
  usub_l(q_l, 1, q_l);
  shright_l(q_l);
  expmodk_l(a_l, q_l, p_l, x_l);
  sqrmod_l(x_l, p_l, temp_l);
  mulmod_l(temp_l, a_l, p_l, z_l);
  mulmod_l(x_l, a_l, p_l, x_l);
  mod_l(z_l, p_l, q_l);
  while (!eq1_l(q_l))
    {
      m = 0;
      do
	{
	  m = m + 1;
	  sqrmod_l(q_l, p_l, q_l);
	}
      while (!eq1_l(q_l));
      if (m == r)
	{
	  
	  purgevars_l(8, sizeof(n_l), n_l, sizeof(q_l), q_l,
		      sizeof(t_l), t_l, sizeof(y_l), y_l,
		      sizeof(z_l), z_l, sizeof(temp_l), temp_l,
		      sizeof(r), &r, sizeof(m), &m);

	  return -1;
	}
      expmod2k_l(y_l, r - m - 1, p_l, t_l);
      sqrmod_l(t_l, p_l, y_l);
      mulmod_l(x_l, t_l, p_l, x_l);
      mulmod_l(z_l, y_l, p_l, z_l);
      copy_l(q_l, z_l);
      r = m;
    }

  purgevars_l(8, sizeof(n_l), n_l, sizeof(q_l), q_l,
	      sizeof(t_l), t_l, sizeof(y_l), y_l,
	      sizeof(z_l), z_l, sizeof(temp_l), temp_l,
	      sizeof(r), &r, sizeof(m), &m);

  return 0;
}

int pqroot_l(BIGINT a_l, BIGINT p_l, BIGINT q_l, BIGINT x_l)
{
  BIGINT x0_l, x1_l, x2_l, x3_l, xp_l, xq_l, n_l, xtemp_l;
  BIGINTD u_l, v_l;
  int usign, vsign;

  if (eqz_l(a_l))
    {
      setzero_l(x_l);
      return 0;
    }
  if ((proot_l(a_l, p_l, xp_l) != 0) || (proot_l(a_l, q_l, xq_l) != 0))
    {

      purgevars_l(2, sizeof(xp_l), xp_l, sizeof(xq_l), xq_l);
      
      return -1;
    }
  mul_l(p_l, q_l, n_l);
  xgcd_l(p_l, q_l, x0_l, u_l, &usign, v_l, &vsign);
  mulmod_l(u_l, p_l, n_l, u_l); /*Attention dépassement possible, la solution est réduite mod 2^4096*/
  mulmod_l(u_l, xq_l, n_l, u_l);
  mulmod_l(v_l, q_l, n_l, v_l);
  mulmod_l(v_l, xp_l, n_l, v_l);
  usign = sadd(u_l, usign, v_l, vsign, x0_l);
  smod(x0_l, usign, n_l, x0_l);
  sub_l(n_l, x0_l, x1_l);
  submod_l(u_l, v_l, n_l, x2_l);
  sub_l(n_l, x2_l, x3_l);
  if (comp_l(x0_l, x1_l) == -1)
    copy_l(xtemp_l, x0_l);
  else
    copy_l(xtemp_l, x1_l);
  if (comp_l(x2_l, xtemp_l) == -1)
    copy_l(xtemp_l, x2_l);
  if (comp_l(x3_l, xtemp_l) == -1)
    copy_l(xtemp_l, x3_l);
  copy_l(x_l, xtemp_l);

  purgevars_l(10, sizeof(x0_l), x0_l, sizeof(x1_l), x1_l,
	    sizeof(x2_l), x2_l, sizeof(x3_l), x3_l,
	    sizeof(xp_l), xp_l, sizeof(xq_l), xq_l,
	    sizeof(n_l), n_l, sizeof(xtemp_l), xtemp_l,
	    sizeof(u_l), u_l, sizeof(v_l), v_l);

  return 0;
}

int chinrem_l(unsigned int numeq, bigint **coeff_l, BIGINT x_l)
{
  BIGINT g_l, u_l, v_l, m_l;
  bigint *ai_l = 0, *mi_l = 0;
  unsigned int i;
  int usign, vsign, xsign, err, error = BIGINT_OK;
  
  if (numeq == 0)
    return 1;
  copy_l(x_l, *(coeff_l));
  coeff_l = coeff_l + 1;
  copy_l(m_l, *(coeff_l));
  coeff_l = coeff_l + 1;
  for (i = 1; i < numeq; i++)
    {
      copy_l(ai_l, *(coeff_l));
      coeff_l = coeff_l + 1;
      copy_l(mi_l, *(coeff_l));
      coeff_l = coeff_l + 1;
      xgcd_l(m_l, mi_l, g_l, u_l, &usign, v_l, &vsign);
      if (!eq1_l(g_l))
	{
	  purgevars_l(4, sizeof(g_l), g_l, sizeof(u_l), u_l,
		      sizeof(v_l), v_l, sizeof(m_l), m_l);
	  return 2;
	}
      err = mul_l(u_l, m_l, u_l);
      if (error == BIGINT_OK)
	error = err;
      err = mul_l(u_l, ai_l, u_l);
      if (error == BIGINT_OK)
	error = err;
      err = mul_l(v_l, mi_l, v_l);
      if (error == BIGINT_OK)
	error = err;
      err = mul_l(v_l, x_l, v_l);
      if (error == BIGINT_OK)
	error = err;
      xsign = sadd(u_l, usign, v_l, vsign, x_l);
      err = mul_l(m_l, mi_l, m_l);
      if (error == BIGINT_OK)
	error = err;
      smod(x_l, xsign, m_l, x_l);
    }

  purgevars_l(4, sizeof(g_l), g_l, sizeof(u_l), u_l,
	      sizeof(v_l), v_l, sizeof(m_l), m_l);

  return error;
}

/* Fonctions de test de primalité */

ULONG *primesgen(ULONG N)
{
  ULONG B0, L;
  unsigned long i, k, p, s, c;
  ULONG *primes;
  char *f;

  B0 = (1 + uintroot_l(N)) >> 1;
  L = (N >> 1);
  if (((N & 1) == 0) && (N > 0))
    L = L - 1;
  f = (char *) malloc (L + 1);
  if (f == NULL)
    {

      purgevars_l(7, sizeof(B0), &B0, sizeof(L), &L,
		  sizeof(i), &i, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(c), &c);
      
      return NULL;
    }
  for (i = 1; i <= L; i++)
    f[i] = 1;
  p = 3;
  s = 4;
  for (i = 1; i <= B0; i++)
    {
      if (f[i] != 0)
	{
	  for (k = s; k <= L; k = k + p)
	    f[k] = 0;
	}
      s = s + 2 * p + 2;
      p = p + 2;
    }
  c = 1;
  for (i = 1; i <= L; i++)
    c = c + f[i];
  primes = (ULONG *)malloc((c + 1) * sizeof(ULONG));
  if (primes == NULL)
    {
      
      purgevars_l(7, sizeof(B0), &B0, sizeof(L), &L,
		  sizeof(i), &i, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(c), &c);

      return NULL;
    }
  c = 1;
  for (i = 1; i <= L; i++)
    {
      if (f[i] != 0)
	{
	  c = c + 1;
	  primes[c] = (i << 1) + 1;
	}
    }
  if (N < 2)
    primes[0] = 0;
  else
    {
      primes[0] = c;
      primes[1] = 2;
    }
  free(f);

  purgevars_l(7, sizeof(B0), &B0, sizeof(L), &L,
	      sizeof(i), &i, sizeof(k), &k,
	      sizeof(p), &p, sizeof(s), &s,
	      sizeof(c), &c);

  return primes;
}

USHORT smallprimes[NUMSPRIMES] =
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

USHORT sieve_l(BIGINT a_l, unsigned int num_sprimes)
{
  BIGINT n_l;
  bigint *aptr_l;
  USHORT b0, r0, q0;
  ULONG t = 0L;
  unsigned int i = 1;

  if (iseven_l(a_l))
    {
      setone_l(n_l);
      shleft_l(n_l);
      if (equ_l(a_l, n_l))
	{
	  
	  purgevars_l(1, sizeof(n_l), n_l);
	  
	  return 1;
	}
      else
	{

	  purgevars_l(1, sizeof(n_l), n_l);

	  return 2;
	}
    }
  b0 = 2;
  do
    {
      r0 = 0;
      b0 = b0 + smallprimes[i];
      aptr_l = msdptr_l(a_l);
      while (aptr_l >= lsdptr_l(a_l))
	{
	  t = (r0 << BITPERDGT) + (*aptr_l);
	  q0 = t / b0;
	  r0 = t % b0;
	  aptr_l = aptr_l - 1;
	}
      i = i + 1;
    }
  while ((r0 != 0) && (i <= num_sprimes));
  if (r0 == 0)
    {
      if ((digits_l(a_l) == 1) && (*lsdptr_l(a_l) == b0))
	b0 = 1;
    }
  else
    b0 = 0;

  purgevars_l(5, sizeof(n_l), n_l, sizeof(b0), &b0,
	      sizeof(r0), &r0, sizeof(q0), &q0, sizeof(t), &t);

  return b0;
}

int MRprime_l(BIGINT n_l, unsigned int numsprimes, unsigned int iter)
{
  BIGINT a_l, t_l, b_l;
  USHORT  k, p;
  unsigned int s, i, j, prime = 1;

  if (eq1_l(n_l))
    return 0;
  k = sieve_l(n_l, numsprimes);
  if (k == 1)
    return 1;
  if (k > 1)
    return 0;
  else
    {
      copy_l(a_l, n_l);
      usub_l(a_l, 1, a_l);
      s = 0;
      copy_l(t_l, a_l);
      while (iseven_l(t_l))
	{
	  shright_l(t_l);
	  s = s + 1;
	}
      p = 2;
      i = 1;
      do
	{
	  p = p + smallprimes[i];
	  i = i + 1;
	  wexpmod_l(p, t_l, n_l, b_l);
	  if (!eq1_l(b_l))
	    {
	      j = 0;
	      while ((!eq1_l(b_l)) && (!equ_l(b_l, a_l)) && (j < s - 1))
		{
		  sqrmod_l(b_l, n_l, b_l);
		  j = j + 1;
		}
	      if (!equ_l(b_l, a_l))
		prime = 0;
	    }
	}
      while ((i <= iter) && (prime));

      purgevars_l(8, sizeof(a_l), a_l, sizeof(t_l), t_l,
		  sizeof(b_l), b_l, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(i), &i, sizeof(j), &j);

      return prime;
    }
}

int SSprime_l(BIGINT n_l, unsigned int numsprimes, unsigned iter)
{
  BIGINT t_l, x_l, p_l, j_l;
  USHORT k, p;
  int j;
  unsigned int i, prime = 1;

  if (eq1_l(n_l))
    return 0;
  k = sieve_l(n_l, numsprimes);
  if (k == 1)
    return 1;
  if (k > 1)
    return 0;
  else
    {
      copy_l(t_l, n_l);
      usub_l(t_l, 1, t_l);
      shright_l(t_l);
      p = 2;
      i = 1;
      do
	{
	  p = p + smallprimes[i];
	  i = i + 1;
	  wexpmod_l(p, t_l, n_l, x_l);
	  u2bigint_l(p_l, p);
	  j = jacobi_l(p_l, n_l);
	  if (j < 0)
	    usub_l(n_l, 1, j_l);
	  else if (j == 0)
	    setzero_l(j_l);
	  else
	    setone_l(j_l);
	  if (!equ_l(x_l, j_l))
	    prime = 0;
	}
      while ((i <= iter) && (prime));

      purgevars_l(8, sizeof(t_l), t_l, sizeof(x_l), x_l,
		  sizeof(p_l), p_l, sizeof(j_l), j_l,
		  sizeof(p), &p, sizeof(k), &k,
		  sizeof(i), &i, sizeof(j), &j);

      return prime;
    }
}

/* Fonctions de génération aléatoire */

static bigint seed64[10];
static bigint a64[] = { 4, 32557, 19605, 62509, 22609 };
static bigint buff64[10];

bigint *rand64_l(void)
{
  mul_l(seed64, a64, seed64);
  uadd_l(seed64, 1, seed64);
  if (digits_l(seed64) > 4)
    setdigits_l(seed64, 4);
  return seed64;
}

bigint *seed64_l(BIGINT seed_l)
{
  int i;

  copy_l(buff64, seed64);
  if (digits_l(seed_l) > 4)
    {
      for (i = 0; i <= 4; i++)
	seed64[i] = seed_l[i];
    }
  else
    {
      for (i = 0; i <= digits_l(seed_l); i++)
	seed64[i] = seed_l[i];
    }
  return buff64;
}

bigint *ulseed64_l(ULONG seed)
{
  copy_l(buff64, seed64);
  setdigits_l(seed64, 2);
  *lsdptr_l(seed64) = (seed & (B - 1));
  *(lsdptr_l(seed64) + 1) = (seed >> BITPERDGT);
  rmldzeros_l(seed64);
  return buff64;
}

UCHAR ucrand64_l(void)
{
  rand64_l();
  return (UCHAR)(seed64[seed64[0]] >> (BITPERDGT - 8));
}

USHORT usrand64_l(void)
{
  rand64_l();
  return seed64[digits_l(seed64)];
}

ULONG ulrand64_l(void)
{
  ULONG rand = 0;
  unsigned int d;

  rand64_l();
  d = digits_l(seed64);
  if ((d > 1) && (d <= 4))
    {
      rand = seed64[d - 1];
      rand = rand + (seed64[d] << BITPERDGT);
    }
  else if (d == 1)
    rand = seed64[d];
  else if (d == 0)
    rand = 0;
  return rand;
}

void rand_l(BIGINT r_l, int l)
{
  unsigned int i, j;
  USHORT ls, lr;

  if (l > (short)BIGINTMAXBIT)
    l = BIGINTMAXBIT;
  ls = (l >> LDBITPERDGT);
  lr = l % BITPERDGT;
  for (i = 1; i <= ls; i++)
    r_l[i] = usrand64_l();
  if (lr > 0)
    {
      ls = ls + 1;
      r_l[ls] = usrand64_l();
      j = (1 << (lr - 1));
      r_l[ls] = (r_l[ls] | j) & ((j << 1) - 1);
    }
  else
    r_l[ls] = (r_l[ls] | BDIV2);
  setdigits_l(r_l, ls);
}

static BIGINT XBBS;
static BIGINT MODBBS;
static char *MODBBSSTR = "81aa5c97200fb2504c08b92e2f8b7e0805775c72b139b455846028653793ba9d616cd01cef7195b2f952e046ab62d063b048c8f62b21c7fc7fa4e71eef2588aa59b81f355b5539a471ee483b022bdab25b3fb41bc6224d60c17bbcb091294f76cb64c3b6da5504085a958b679d1f1147158189d4fa76ab721c535a3ecfe4452cc61b9466e315ba4b4392db04f686dbb61084b21c9540f972718fc96ed25a40a917a07c668048683ec116219621a301610bfba40e1ab11bd1e13aa8476aa6d37b261228df85c7df67043c51b37a250b27401aaf837101d2db1a55572dd7a79646ff6e5d20a24e4b43c6d8ab5e9a77becd76d5f0355252f4318e2066d3f9c42f25";

int randbit_l(void)
{
  sqrmod_l(XBBS, MODBBS, XBBS);
  return (*lsdptr_l(XBBS) & 1);
}

int seedBBS_l(BIGINT seed_l)
{
  BIGINT g_l;
  
  str2bigint_l(MODBBS, MODBBSSTR, 16);
  gcd_l(seed_l, MODBBS, g_l);
  if (!eq1_l(g_l))
    return -1;
  sqrmod_l(seed_l, MODBBS, XBBS);
  return 0;
}

int ulseedBBS_l(ULONG seed)
{
  BIGINT seed_l;
  int s;

  setdigits_l(seed_l, 2);
  *lsdptr_l(seed_l) = (seed & (B - 1));
  *(lsdptr_l(seed_l) + 1) = (seed >> BITPERDGT);
  rmldzeros_l(seed_l);
  s = seedBBS_l(seed_l);
  return s;
}

USHORT usrandBBS_l(void)
{
  unsigned int i, r = 0;

  for (i = 0; i < (sizeof(USHORT) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

ULONG ulrandBBS_l(void)
{
  unsigned int i, r = 0;

  for (i = 0; i < (sizeof(ULONG) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

UCHAR ucrandBBS_l(void)
{
  unsigned int i;
  UCHAR r = 0;

  for (i = 0; i < (sizeof(UCHAR) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

void randBBS_l(BIGINT r_l, int l)
{
  unsigned int i, j;
  USHORT ls, lr;

  if (l > (short)BIGINTMAXBIT)
    l = BIGINTMAXBIT;
  ls = (l >> LDBITPERDGT);
  lr = l % BITPERDGT;
  for (i = 1; i <= ls; i++)
    r_l[i] = usrandBBS_l();
  if (lr > 0)
    {
      ls = ls + 1;
      r_l[ls] = usrandBBS_l();
      j = (1 << (lr - 1));
      r_l[ls] = (r_l[ls] | j) & ((j << 1) - 1);
    }
  else
    r_l[ls] = (r_l[ls] | BDIV2);
  setdigits_l(r_l, ls);

  purgevars_l(4, sizeof(i), &i, sizeof(j), &j,
	      sizeof(ls), &ls, sizeof(lr), &lr);
}

/* Fonctions de Montgomery */

void mulmon_l(BIGINT a_l, BIGINT b_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l)
{
  bigint t_l[1 + (BIGINTMAXDIGIT << 1)];
  bigint *tptr_l, *nptr_l, *tiptr_l, *lasttptr_l, *lastnptr_l;
  ULONG carry = 0L;
  ULONG mtemp, t = 0L;
  USHORT mi;
  int i;

  mul(a_l, b_l, t_l);
  lasttptr_l = t_l + digits_l(n_l);
  lastnptr_l = msdptr_l(n_l);
  for (i = digits_l(t_l) + 1; i <= (digits_l(n_l) << 1); i++)
    t_l[i] = 0;
  if (digits_l(t_l) < (digits_l(n_l) << 1))
    setdigits_l(t_l, (digits_l(n_l) << 1));
  else
    setdigits_l(t_l, digits_l(t_l));
  tptr_l = lsdptr_l(t_l);
  while (tptr_l <= lasttptr_l)
    {
      carry = 0;
      mtemp = (*tptr_l) * np;
      mi = mtemp & (B - 1);
      nptr_l = lsdptr_l(n_l);
      tiptr_l = tptr_l;
      while (nptr_l <= lastnptr_l)
	{
	  t = mi * (*nptr_l) + *tiptr_l + carry;
	  *tiptr_l = t & (B - 1);
	  carry = (t >> BITPERDGT);
	  nptr_l = nptr_l + 1;
	  tiptr_l = tiptr_l + 1;
	}
      while ((carry > 0) && (tiptr_l <= msdptr_l(t_l)))
	{
	  t = *tiptr_l + carry;
	  *tiptr_l = t & (B - 1);
	  carry = (t >> BITPERDGT);
	  tiptr_l = tiptr_l + 1;
	}
      if (carry > 0)
	{
	  *tiptr_l = carry;
	  incdigits_l(t_l);
	}
      tptr_l = tptr_l + 1;
    }
  tptr_l = t_l + l;
  setdigits_l(tptr_l, digits_l(t_l) - l);
  copy_l(t_l, tptr_l);

  if (ge_l(t_l, n_l))
    sub_l(t_l, n_l, p_l);/*Il est possible que t_l soit BIGINTP*/
  else
    copy_l(p_l, t_l);

  purgevars_l(4, sizeof(mi), &mi, sizeof(carry), &carry,
	      sizeof(mtemp), &mtemp, sizeof(t_l), t_l);
}

void sqrmon_l(BIGINT a_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l)
{
  bigint t_l[1 + (BIGINTMAXDIGIT << 1)];
  bigint *tptr_l, *nptr_l, *tiptr_l, *lasttptr_l, *lastnptr_l;
  ULONG carry = 0L;
  ULONG mtemp, t = 0L;
  USHORT mi;
  int i;

  sqr(a_l, t_l);
  lasttptr_l = t_l + digits_l(n_l);
  lastnptr_l = msdptr_l(n_l);
  for (i = digits_l(t_l) + 1; i <= (digits_l(n_l) << 1); i++)
    t_l[i] = 0;
  if (digits_l(t_l) < (digits_l(n_l) << 1))
    setdigits_l(t_l, (digits_l(n_l) << 1));
  else
    setdigits_l(t_l, digits_l(t_l));
  tptr_l = lsdptr_l(t_l);
  while (tptr_l <= lasttptr_l)
    {
      carry = 0;
      mtemp = (*tptr_l) * np;
      mi = mtemp & (B - 1);
      nptr_l = lsdptr_l(n_l);
      tiptr_l = tptr_l;
      while (nptr_l <= lastnptr_l)
	{
	  t = mi * (*nptr_l) + *tiptr_l + carry;
	  *tiptr_l = t & (B - 1);
	  carry = (t >> BITPERDGT);
	  nptr_l = nptr_l + 1;
	  tiptr_l = tiptr_l + 1;
	}
      while ((carry > 0) && (tiptr_l <= msdptr_l(t_l)))
	{
	  t = *tiptr_l + carry;
	  *tiptr_l = t & (B - 1);
	  carry = (t >> BITPERDGT);
	  tiptr_l = tiptr_l + 1;
	}
      if (carry > 0)
	{
	  *tiptr_l = carry;
	  incdigits_l(t_l);
	}
      tptr_l = tptr_l + 1;
    }

  tptr_l = t_l + l;
  setdigits_l(tptr_l, digits_l(t_l) - l);
  copy_l(t_l, tptr_l);
  if (ge_l(t_l, n_l))
    sub_l(t_l, n_l, p_l);
  else
    copy_l(p_l, t_l);

  purgevars_l(4, sizeof(mi), &mi, sizeof(carry), &carry,
	      sizeof(mtemp), &mtemp, sizeof(t_l), t_l);
}

USHORT invmon_l(BIGINT n_l)
{
  unsigned int i;
  ULONG x = 2, y = 1;

  if (iseven_l(n_l))
    return BIGINT_MOD;
  for (i = 2; i <= BITPERDGT; i++)
    {
      if (x < ((*lsdptr_l(n_l) * y) & ((x << 1) - 1)))
	y = y + x;
      x = (x << 1);
    }
  return (x - y);
}

int uexpmodmon_l(BIGINT aa_l, USHORT e, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, t_l, md_l;
  BIGINT un_l = { 1, 1 };
  BIGINTP r_l;
  USHORT mask = BDIV2, l, np;

  if (iseven_l(m_l))
    return BIGINT_MOD;
  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (e == 0)
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(aa_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  copy_l(md_l, m_l);
  rmldzeros_l(md_l);
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  if (digits_l(r_l) > BIGINTMAXDIGIT)
    mod_l(r_l, md_l, r_l);
  np = invmon_l(md_l);
  mulmod_l(aa_l, r_l, md_l, a_l);
  copy_l(t_l, a_l);
  while ((e & mask) == 0)
    mask = (mask >> 1);
  mask = (mask >> 1);
  while (mask != 0)
    {
      sqrmon_l(a_l, md_l, np, l, a_l);
      if (e & mask)
	mulmon_l(a_l, t_l, md_l, np, l, a_l);
      mask = (mask >> 1);
    }
  mulmon_l(a_l, un_l, md_l, np, l, p_l);

  purgevars_l(6, sizeof(a_l), a_l, sizeof(t_l), t_l,
	      sizeof(r_l), r_l, sizeof(md_l), md_l,
	      sizeof(np), &np, sizeof(l), &l);

  return BIGINT_OK;
}

int wexpmodmon_l(USHORT a, BIGINT ee_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, e_l, md_l;
  BIGINT un_l = { 1 , 1 };
  BIGINTP r_l;
  USHORT k, b, w, l, np;

 if (iseven_l(m_l))
   return BIGINT_MOD;
  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(ee_l))
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (a == 0)
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eq1_l(ee_l))
    {
      u2bigint_l(a_l, a);
      mod_l(a_l, m_l, p_l);
      return BIGINT_OK;
    }
  copy_l(e_l, ee_l);
  copy_l(md_l, m_l);
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  np = invmon_l(md_l);
  mod_l(r_l, md_l, a_l);
  b = (1 << ((numbits_l(e_l) - 1) & (BITPERDGT - 1)));
  w = e_l[digits_l(e_l)];
  while (b > 0)
    {
      sqrmon_l(a_l, md_l, np, l, a_l);
      if ((w & b) > 0)
	umulmod_l(a_l, a, md_l, a_l);
      b = (b >> 1);
    }
  for (k = digits_l(e_l) - 1; k > 0; k--)
    {
      w = e_l[k];
      b = BDIV2;
      while (b > 0)
	{
	  sqrmon_l(a_l, md_l, np, l, a_l);
	  if ((w & b) > 0)
	    umulmod_l(a_l, a, md_l, a_l);
	  b = (b >> 1);
	}
    }
  mulmon_l(a_l, un_l, md_l, np, l, p_l);

  purgevars_l(9, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(r_l), r_l,
	      sizeof(md_l), md_l, sizeof(b), &b, sizeof(w), &w,
	      sizeof(np), &np, sizeof(l), &l, sizeof(k), &k);

  return BIGINT_OK;
}

int expmodmonk_l(BIGINT aa_l, BIGINT ee_l, BIGINT m_l, BIGINT p_l)
{
  BIGINT a_l, a2_l, md_l;
  BIGINTP e_l, r_l;
  BIGINTD temp_l;
  BIGINT un_l = { 1, 1 };
  bigint **pptr_l, *ptr_l = NULL;
  int i, f, tab;
  unsigned int k, lge, pow2k, mask, fk, s, d, digit;
  USHORT l, np;

  if (iseven_l(m_l))
    return BIGINT_MOD;
  if (eqz_l(m_l))
    return BIGINT_DBZ;
  if (eq1_l(m_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(ee_l))
    {
      setone_l(p_l);
      return BIGINT_OK;
    }
  if (eqz_l(aa_l))
    {
      setzero_l(p_l);
      return BIGINT_OK;
    }
  copy_l(a_l, aa_l);
  copy_l(e_l, ee_l);
  copy_l(md_l, m_l);
  rmldzeros_l(a_l);
  rmldzeros_l(e_l);
  rmldzeros_l(md_l);
  lge = numbits_l(e_l);
  k = 8;
  while (k > 1 && ((k - 1) * (k << ((k - 1) << 1))/((1 << k) - k - 1)) >= lge - 1)
    k = k - 1;
  pow2k = (1 << k);
  mask = pow2k - 1;
  pptr_l = (bigint **)malloc(sizeof(bigint *) * pow2k);
  if (pptr_l  == NULL)
    {
      purgevars_l(3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l);
      return BIGINT_MAL;
    }
  pptr_l[1] = a_l;
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  if (digits_l(r_l) > BIGINTMAXDIGIT)
    mod_l(r_l, md_l, r_l);
  np = invmon_l(md_l);
  mulmod_l(a_l, r_l, md_l, a_l);
  if (k > 1)
    {
      ptr_l = (bigint *)malloc(sizeof(BIGINT) * ((pow2k >> 1) - 1));
      if (ptr_l == NULL)
	{
	  free(pptr_l);
	  purgevars_l(3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l);
	  return BIGINT_MAL;
	}
      pptr_l[2] = a2_l;
      pptr_l[3] = ptr_l;
      for (i = 5; i < (int)pow2k; i+=2)
	pptr_l[i] = pptr_l[i - 2] + BIGINTMAXSHORT;
      sqrmon_l(a_l, md_l, np, l, pptr_l[2]);
      for (i = 3; i < (int)pow2k; i+=2)
	mulmon_l(pptr_l[2], pptr_l[i - 2], md_l, np, l, pptr_l[i]);
    }
  *(msdptr_l(e_l) + 1) = 0;
  f = (lge - 1)/k;
  fk = f * k;
  s = (fk >> LDBITPERDGT);
  d = fk & (BITPERDGT - 1);
  digit = (((e_l[s + 2] << BITPERDGT) + e_l[s + 1]) >> d) & mask;
  if (digit != 0)
    {
      copy_l(temp_l, pptr_l[oddtab[digit]]);
      tab = twotab[digit];
      for (i = 1; i <= tab; i++)
	sqrmon_l(temp_l, md_l, np, l, temp_l);
    }
  else
    mod_l(r_l, md_l, temp_l);
  fk = fk - k;
  for (f = f - 1; f >= 0; f--)
    {
      s = (fk >> LDBITPERDGT);
      d = fk & (BITPERDGT - 1);
      digit = (((e_l[s + 2] << BITPERDGT) + e_l[s + 1]) >> d) & mask;
      if (digit != 0)
	{
	  tab = twotab[digit];
	  for (i = 1; i <= ((int)k - tab); i++)
	    sqrmon_l(temp_l, md_l, np, l, temp_l);
	  mulmon_l(temp_l, pptr_l[oddtab[digit]], md_l, np, l, temp_l);
	  for (i = 1; i <= tab; i++)
	    sqrmon_l(temp_l, md_l, np, l, temp_l);
	}
      else
	{
	  for (i = 1; i <= (int)k; i++)
	    sqrmon_l(temp_l, md_l, np, l, temp_l);
	}
      fk = fk - k;
    }
  mulmon_l(temp_l, un_l, md_l, np, l, p_l);
  free(pptr_l);
  if (ptr_l != NULL)
    {
      memset(ptr_l, 0, sizeof(BIGINT) * ((pow2k >> 1) - 1));
      free(ptr_l);
    }

  purgevars_l(17, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	      sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	      sizeof(md_l), md_l, sizeof(r_l), r_l,
	      sizeof(l), &l, sizeof(np), &np,
	      sizeof(tab), &tab, sizeof(k), &k,
	      sizeof(lge), &lge, sizeof(pow2k), &pow2k,
	      sizeof(mask), &mask, sizeof(fk), &fk,
	      sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit);

  return BIGINT_OK;
}

/* Fonctions de purge */

void purge_l(BIGINT n_l)
{
  if (n_l != NULL)
    memset(n_l, 0, BIGINTMAXBYTE + 2);
}

/* Fonction interne de purge des variables locales */

static void purgevars_l(int numvars, ...)
{
  va_list arg;
  size_t size;

  va_start(arg, numvars);
  for (; numvars > 0; --numvars)
    {
      size = va_arg(arg, size_t);
      switch (size)
	{
	case 1:
	  *va_arg(arg, char *) = 0;
	  break;
	case 2:
	  *va_arg(arg, short *) = 0;
	  break;
	case 4:
	  *va_arg(arg, long *) = 0;
	  break;
	default:
	  assert(size >= BIGINTMAXBYTE + 2);
	  memset(va_arg(arg, char *), 0, size);
	}
    }
  va_end(arg);
}


/* Fin de arithm.c */
