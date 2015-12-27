/***********************************************************************
 *                                                                     *
 *       Code source pour la librairie de fonctions d'arithmétique     *
 *       en multi-précision (4096 bits si ARCH32 et 8192 si ARCH64)    *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#include "libmparithm.h"


/* Prototypes des fonctions kernel */

static void add(BIGINT a_l, BIGINT b_l, BIGINTP s_l);
static void sub(BIGINTD a_l, BIGINT b_l, BIGINT d_l);
static void mul(BIGINT a_l, BIGINT b_l, BIGINTD p_l);
static void sqr(BIGINT a_l, BIGINTD p_l);

/* Prototype des fonctions kernel pour Karatsuba */

static void addshift(BIGINT a_l, BIGINT b_l, USHORT dgts, BIGINTD s_l);
static void mulkar(BIGINT a_l, BIGINT b_l, BIGINTD p_l);
static void sqrkar(BIGINT a_l, BIGINTD p_l);

/* Prototype de fonction memset vérifiée */

#ifdef SECURE
static void *guaranteed_memset(void *v, int c, size_t n);
#endif

/* Prototype des fonctions de purge des variables */

#ifdef SECURE
#define PURGEVARS_L(X) purgevars_l X
static void purgevars_l(int numvars, ...);
#ifdef DEBUG_SECURE
#define ISPURGED_L(X) assert(ispurged_l X)
static int ispurged_l(int numvars, ...);
#else
#define ISPURGED_L(X) (void)0
#endif
#else
#define PURGEVARS_L(X) (void)0
#define ISPURGED_L(X) (void)0
#endif

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
  *n_l = --*n_l;
}

void incdigits_l(BIGINTD n_l)
{
  *n_l = ++*n_l;
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
      ++dest_l;
      ++src_l;
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

void setmax_l(BIGINTD n_l)
{
  bigint *ptr_l;
  
  setdigits_l(n_l, (USHORT)BIGINTMAXDIGIT);
  ptr_l = lsdptr_l(n_l);
  while (ptr_l <= msdptr_l(n_l))
    {
      *ptr_l = BMIN1; /*ARCH64*/
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

static void add(BIGINT a_l, BIGINT b_l, BIGINTP s_l)
{
  bigint *msdptra_l, *msdptrb_l;
  register bigint *aptr_l, *bptr_l;
  bigint *sptr_l = lsdptr_l(s_l);
  USHORT carry = 0;
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
      t = (ULONG)*aptr_l + (ULONG)*bptr_l + (ULONG)carry; /*ARCH64*/
      *sptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);/* t div B */
      ++aptr_l;
      ++bptr_l;
      ++sptr_l;
    }
  while (aptr_l <= msdptra_l)
    {
      t = (ULONG)*aptr_l + (ULONG)carry; /*ARCH64*/
      *sptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
      ++sptr_l;
    }
  if (carry == 1)
    {
      *sptr_l = 1;
      setdigits_l(s_l, (USHORT)(digits_l(s_l) + 1));
    }
  rmldzeros_l(s_l);

  PURGEVARS_L((2, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((2, sizeof(carry), &carry, sizeof(t), &t));
}

static void sub(BIGINTD a_l, BIGINT b_l, BIGINTD d_l)
{
  bigint *msdptra_l, *msdptrb_l;
  register bigint *aptr_l, *bptr_l;
  bigint *dptr_l;
  USHORT carry = 1;
  ULONG t = 0L;

  aptr_l = lsdptr_l(a_l);
  bptr_l = lsdptr_l(b_l);
  dptr_l = lsdptr_l(d_l);
  msdptra_l = msdptr_l(a_l);
  msdptrb_l = msdptr_l(b_l);
  setdigits_l(d_l, digits_l(a_l));
  while (bptr_l <= msdptrb_l)
    {
      t = (ULONG)BMIN1 + (ULONG)*aptr_l - (ULONG)*bptr_l + (ULONG)carry; /*ARCH64*/
      *dptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
      ++bptr_l;
      ++dptr_l;
    }
  while (aptr_l <= msdptra_l)
    {
      t = (ULONG)BMIN1 + (ULONG)*aptr_l + (ULONG)carry; /*ARCH64*/
      *dptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
      ++dptr_l;
    }
  rmldzeros_l(d_l);

  PURGEVARS_L((2, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((2, sizeof(carry), &carry, sizeof(t), &t));
}

static void mul(BIGINT a_l, BIGINT b_l, BIGINTD p_l)
{
  register bigint *bptr_l, *pptr_l;
  bigint *aptr_l, *msdptra_l, *msdptrb_l;
  USHORT a0;
  USHORT carry = 0;
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
      t = (ULONG)a0 * (ULONG)(*bptr_l) + (ULONG)carry; /*ARCH64*/
      *pptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++bptr_l;
      ++pptr_l;
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
	  t = (ULONG)a0 * (ULONG)(*bptr_l) + (ULONG)*pptr_l + (ULONG)carry; /*ARCH64*/
	  *pptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++bptr_l;
	  ++pptr_l;
	}
      *pptr_l = carry;
      ++aptr_l;
    }
  rmldzeros_l(p_l);

  PURGEVARS_L((3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0));
  ISPURGED_L((3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0));
}

static void sqr(BIGINT a_l, BIGINTD p_l)             
{
  register bigint *cptr_l, *bptr_l;
  bigint *aptr_l, *csptr_l, *msdptra_l, *msdptrb_l, *msdptrc_l;
  USHORT a0;
  USHORT carry = 0;
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
  setdigits_l(p_l, (USHORT)(2 * digits_l (a_l)));
  while (bptr_l <= msdptrb_l)
    {
      t = (ULONG)a0 * (ULONG)(*bptr_l) + (ULONG)carry; /*ARCH64*/
      *cptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++bptr_l;
      ++cptr_l;
    }
  *cptr_l = (USHORT)(t >> BITPERDGT);

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
	  t = (ULONG)a0 * (ULONG)(*bptr_l) + (ULONG)(*cptr_l) + (ULONG)carry; /*ARCH64*/
	  *cptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++bptr_l;
	  ++cptr_l;
        }
      *cptr_l = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
      ++csptr_l;
      ++csptr_l;
    }

  msdptrc_l = cptr_l;
  carry = 0;
  cptr_l = lsdptr_l(p_l);
  while (cptr_l <= msdptrc_l)
    {
      t = (ULONG)2 * (ULONG)(*cptr_l) + (ULONG)carry; /*ARCH64*/
      *cptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++cptr_l;
    }
  *cptr_l = (USHORT)(t >> BITPERDGT);

  carry = 0;
  bptr_l = lsdptr_l(a_l);
  cptr_l = lsdptr_l(p_l);
  while (bptr_l <= msdptrb_l)
    {
      t = (ULONG)(*bptr_l) * (ULONG)(*bptr_l) + (ULONG)(*cptr_l) + (ULONG)carry; /*ARCH64*/
      *cptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++cptr_l;
      t = (ULONG)(*cptr_l) + (ULONG)carry; /*ARCH64*/
      *cptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++bptr_l;
      ++cptr_l;
    }
  rmldzeros_l(p_l);

  PURGEVARS_L((3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0));
  ISPURGED_L((3, sizeof(carry), &carry, sizeof(t), &t, sizeof(a0), &a0));
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
  *lsdptr_l(u_l) = (USHORT)(ul & BMIN1);
  *(lsdptr_l(u_l) + 1) = (USHORT)(ul >> BITPERDGT);
}

int add_l(BIGINT aa_l, BIGINT bb_l, BIGINT s_l)
{
  BIGINTP ss_l;
  BIGINT a_l, b_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  add(a_l, b_l, ss_l);

  if (digits_l(ss_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(ss_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(ss_l);
      OFL = BIGINT_OFL;
    }
  copy_l(s_l, ss_l);

  PURGEVARS_L((3, sizeof(ss_l), ss_l, sizeof(a_l), a_l, sizeof(b_l), b_l));
  ISPURGED_L((3, sizeof(ss_l), ss_l, sizeof(a_l), a_l, sizeof(b_l), b_l));

  return OFL;
}

int uadd_l(BIGINT a_l, USHORT b, BIGINT s_l)
{
  int OFL;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  OFL = add_l(a_l, t_l, s_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

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

  PURGEVARS_L((4, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l, sizeof(temp_l), temp_l));
  ISPURGED_L((4, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l, sizeof(temp_l), temp_l));

  return UFL;
}

int usub_l(BIGINT a_l, USHORT b, BIGINT d_l)
{
  int UFL;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  UFL = sub_l(a_l, t_l, d_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

  return UFL;
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
  
int mulclass_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l)
{
  BIGINT a_l, b_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  mul(a_l, b_l, pp_l);

  if (digits_l(pp_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  PURGEVARS_L((3, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(b_l), b_l));
  ISPURGED_L((3, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(b_l), b_l));

  return OFL;
}

int umul_l(BIGINT aa_l, USHORT b, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD pp_l;
  register bigint *aptr_l, *pptr_l;
  bigint *msdptra_l;
  USHORT carry = 0;
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

  setdigits_l(pp_l, (USHORT)(digits_l(a_l) + 1));
  
  while (aptr_l <= msdptra_l)
    {
      t = (ULONG)*aptr_l * (ULONG)b + (ULONG)carry; /*ARCH64*/
      *pptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
      ++pptr_l;
    }
  *pptr_l = carry;
  rmldzeros_l(pp_l);
  if (digits_l(pp_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  PURGEVARS_L((4, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((4, sizeof(pp_l), pp_l, sizeof(a_l), a_l, sizeof(carry), &carry, sizeof(t), &t));

  return OFL;
}

int sqrclass_l(BIGINT aa_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);

  sqr(a_l, pp_l);

  if (digits_l(pp_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  PURGEVARS_L((2, sizeof(pp_l), pp_l, sizeof(a_l), a_l));
  ISPURGED_L((2, sizeof(pp_l), pp_l, sizeof(a_l), a_l));

  return OFL;
}

int div_l(BIGINTM aa_l, BIGINT bb_l, BIGINTM quot_l, BIGINT rem_l)
{
  register bigint *rptr_l, *bptr_l;
  BIGINT b_l;
  BIGINTM r_l;
  bigint *qptr_l, *msdptrb_l, *lsdptrr_l, *msdptrr_l;
  USHORT b0, r0, qhat, ri, ri_1, ri_2, bn, bn_1, carry, borrow, sbitsminusd;
  ULONG right, rhat;
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

      PURGEVARS_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));
      ISPURGED_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));

      return BIGINT_DBZ;
    }

  if (eqz_l(r_l))
    {
      setzero_l(quot_l);
      setzero_l(rem_l);

      PURGEVARS_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));
      ISPURGED_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));

      return BIGINT_OK;
    }

  c = comp_l(r_l, b_l);
  if (c == -1)
    {
      copy_l(rem_l, r_l);
      setzero_l(quot_l);

      PURGEVARS_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));
      ISPURGED_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));

      return BIGINT_OK;
    }
  else if (c == 0)
    {
      setone_l(quot_l);
      setzero_l(rem_l);

      PURGEVARS_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));
      ISPURGED_L((2, sizeof(r_l), r_l, sizeof(b_l), b_l));

      return BIGINT_OK;
    }

  if (digits_l(b_l) == 1)
    goto shortdiv;

  setdigits_l(quot_l , (USHORT)(digits_l(r_l) - digits_l(b_l) + 1));
  msdptrb_l = msdptr_l(b_l);
  bn = *msdptrb_l;
  while (bn < (USHORT)BDIV2)
    {
      ++d;
      bn = (bn << 1);
    }
  sbitsminusd = BITPERDGT - d;
  if (d > 0)
    {
      bn = bn + ((*(msdptrb_l - 1)) >> sbitsminusd); /* Report de la mult par 2^d dû à b_{n-2} */ /*ARCH64*/
      if (digits_l(b_l) > 2)
	bn_1 = (USHORT)(((ULONG)(*(msdptrb_l - 1)) << d) + ((ULONG)(*(msdptrb_l - 2)) >> sbitsminusd));/* Au cas où bn_1 serait >= B, on réduit modulo B */ /*ARCH64*/
      else
	bn_1 = (USHORT)((ULONG)(*(msdptrb_l - 1)) << d); /*ARCH64*/
    }
  else
    bn_1 = (USHORT)(*(msdptrb_l - 1)); /*ARCH64*/
  msdptrr_l = msdptr_l(r_l) + 1;
  lsdptrr_l = msdptr_l(r_l) - digits_l(b_l) + 1;
  *msdptrr_l = 0;
  qptr_l = quot_l + digits_l(r_l) - digits_l(b_l) + 1;

  while (lsdptrr_l >= lsdptr_l(r_l))
    {
      ri = (USHORT)(((ULONG)(*msdptrr_l) << d) + ((ULONG)(*(msdptrr_l - 1)) >> sbitsminusd)); /*ARCH64*/
      ri_1 = (USHORT)(((ULONG)(*(msdptrr_l - 1)) << d) + ((ULONG)(*(msdptrr_l - 2)) >> sbitsminusd)); /*ARCH64*/
      if (msdptrr_l - 3 > r_l)
	ri_2 = (USHORT)(((ULONG)(*(msdptrr_l - 2)) << d) + ((ULONG)(*(msdptrr_l - 3)) >> sbitsminusd)); /*ARCH64*/
      else
	ri_2 = (USHORT)((ULONG)(*(msdptrr_l - 2)) << d); /*ARCH64*/
      if (ri != bn)
	{
	  rhat = ((ULONG)ri << BITPERDGT) + (ULONG)ri_1; /*ARCH64*/
	  qhat = (USHORT)(rhat / (ULONG)bn); /*ARCH64*/
	  rhat = rhat % (ULONG)bn; /*ARCH64*/
	  right = (rhat << BITPERDGT) + (ULONG)ri_2; /*ARCH64*/
	  if (((ULONG)bn_1 * qhat) > right)
	    {
	      --qhat;
	      if ((rhat + (ULONG)bn) < B) /*ARCH64*/
		{
		  if (((ULONG)bn_1 * qhat) > (right + ((ULONG)bn << BITPERDGT)))
		    --qhat;
		}
	    }
	}
      else
	{
	  rhat = (ULONG)bn + (ULONG)ri_1; /*ARCH64*/
	  qhat = (USHORT)BMIN1;
	  right = (rhat << BITPERDGT) + (ULONG)ri_2; /*ARCH64*/
	  if (rhat < B)
	    {
	      if (((ULONG)bn_1 * qhat) > right)
		{
		  --qhat;
		  if ((rhat + (ULONG)bn) < B) /*ARCH64*/
		    {
		      if (((ULONG)bn_1 * qhat) > (right + ((ULONG)bn << BITPERDGT)))
			--qhat;
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
	  p = (ULONG)(*bptr_l) * (ULONG)qhat + (ULONG)carry; /*ARCH64*/
	  t = (ULONG)BMIN1 + (ULONG)(*rptr_l) - (ULONG)(p & BMIN1) + (ULONG)borrow;
	  *rptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(p >> BITPERDGT);
	  borrow = (USHORT)(t >> BITPERDGT);
	  ++bptr_l;
	  ++rptr_l;
	}
      t = (ULONG)BMIN1 + (ULONG)(*rptr_l) + (ULONG)borrow - (ULONG)carry;
      *rptr_l = (USHORT)(t & BMIN1);
      borrow = (USHORT)(t >> BITPERDGT);
      if (borrow == 0)
	{
	  carry = 0;
	  bptr_l = lsdptr_l(b_l);
	  rptr_l = lsdptrr_l;
	  while (bptr_l <= msdptrb_l)
	    {
	      t = (ULONG)(*rptr_l) + (ULONG)(*bptr_l) + (ULONG)carry;
	      *rptr_l = (USHORT)(t & BMIN1);
	      carry = (USHORT)(t >> BITPERDGT);
	      ++bptr_l;
	      ++rptr_l;
	    }
	  *rptr_l = *rptr_l + carry;

	  --qhat;
	}

      *qptr_l = qhat;

      --msdptrr_l;
      --lsdptrr_l;
      --qptr_l;
    }

  rmldzeros_l(quot_l);
  setdigits_l(r_l, digits_l(b_l));
  copy_l(rem_l, r_l);
  rmldzeros_l(rem_l);

  PURGEVARS_L((17, sizeof(r_l), r_l, sizeof(b_l), b_l,
	      sizeof(b0), &b0, sizeof(r0), &r0,
	      sizeof(qhat), &qhat, sizeof(ri), &ri,
	      sizeof(ri_1), &ri_1, sizeof(ri_2), &ri_2,
	      sizeof(bn), &bn, sizeof(bn_1), &bn_1,
	      sizeof(right), &right, sizeof(rhat), &rhat,
	      sizeof(borrow), &borrow, sizeof(carry), &carry,
	      sizeof(sbitsminusd), &sbitsminusd, sizeof(t), &t,
	      sizeof(p), &p, sizeof(d), &d, sizeof(c), &c));
  ISPURGED_L((17, sizeof(r_l), r_l, sizeof(b_l), b_l,
			sizeof(b0), &b0, sizeof(r0), &r0,
			sizeof(qhat), &qhat, sizeof(ri), &ri,
			sizeof(ri_1), &ri_1, sizeof(ri_2), &ri_2,
			sizeof(bn), &bn, sizeof(bn_1), &bn_1,
			sizeof(right), &right, sizeof(rhat), &rhat,
			sizeof(borrow), &borrow, sizeof(carry), &carry,
			sizeof(sbitsminusd), &sbitsminusd, sizeof(t), &t,
			sizeof(p), &p, sizeof(d), &d, sizeof(c), &c));

  return BIGINT_OK;

 shortdiv:
  r0 = 0;
  b0 = *lsdptr_l(b_l);
  rptr_l = msdptr_l(r_l);
  setdigits_l(quot_l, digits_l(r_l));
  qptr_l = msdptr_l(quot_l);
  while (rptr_l >= lsdptr_l(r_l))
    {
      t = ((ULONG)r0 << BITPERDGT) + (ULONG)(*rptr_l);
      *qptr_l = (USHORT)(t / b0);
      r0 = (USHORT)(t % b0);
      --qptr_l;
      --rptr_l;
    }
  rmldzeros_l(quot_l);
  u2bigint_l(rem_l, r0);
  rmldzeros_l(rem_l);

  PURGEVARS_L((5, sizeof(r0), &r0, sizeof(b0), &b0, sizeof(b_l), b_l, sizeof(r_l), r_l, sizeof(t), &t));
  ISPURGED_L((5, sizeof(r0), &r0, sizeof(b0), &b0, sizeof(b_l), b_l, sizeof(r_l), r_l, sizeof(t), &t));

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
      t = ((ULONG)r << BITPERDGT) + (ULONG)(*aptr_l); /*ARCH64*/
      *qptr_l = (USHORT)(t / b);
      r = (USHORT)(t % b);
      --qptr_l;
      --aptr_l;
    }
  rmldzeros_l(quot_l);
  u2bigint_l(rem_l, r);

  PURGEVARS_L((3, sizeof(r), &r, sizeof(r), &r, sizeof(a_l), a_l));
  ISPURGED_L((3, sizeof(r), &r, sizeof(r), &r, sizeof(a_l), a_l));

  return BIGINT_OK;
}
      
int mod_l(BIGINTM a_l, BIGINT b_l, BIGINT rem_l)
{
  BIGINTM dummy_l;
  int DBZ;

  DBZ = div_l(a_l, b_l, dummy_l, rem_l);

  PURGEVARS_L((1, sizeof(dummy_l), dummy_l));
  ISPURGED_L((1, sizeof(dummy_l), dummy_l));

  return DBZ;
}

int mod2_l(BIGINT aa_l, ULONG k, BIGINT rem_l)
{
  USHORT u;

  copy_l(rem_l, aa_l);
  if (k > BIGINTMAXBIT)
    return BIGINT_OFL;

  u = (USHORT)(1 + (k >> LDBITPERDGT));
  if (u > digits_l(rem_l))
    return BIGINT_OK;

  rem_l[u] = rem_l[u] & ((1 << (k % BITPERDGT)) - 1);
  setdigits_l(rem_l, u);
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
    return BMIN1;

  if (eqz_l(a_l))
    return 0;

  aptr_l = msdptr_l(a_l);

  while (aptr_l >= lsdptr_l(a_l))
    {
      t = ((ULONG)r << BITPERDGT) + (ULONG)(*aptr_l); /*ARCH64*/
      r = (USHORT)(t % b);
      --aptr_l;
    }

  PURGEVARS_L((2, sizeof(t), &t, sizeof(a_l), a_l));
  ISPURGED_L((2, sizeof(t), &t, sizeof(a_l), a_l));

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

  PURGEVARS_L((2, sizeof(q_l), q_l, sizeof(r_l), r_l));
  ISPURGED_L((2, sizeof(q_l), q_l, sizeof(r_l), r_l));

  return 1;
}

/* Fonction pour Karatsuba */

static void addshift(BIGINT a_l, BIGINT b_l, USHORT dgts, BIGINTD s_l)
{
  register bigint *aptr_l, *bptr_l;
  bigint *sptr_l = lsdptr_l(s_l);
  bigint *msdptra_l, *msdptrb_l;
  USHORT carry = 0;
  ULONG t = 0L;

  aptr_l = lsdptr_l(a_l);
  bptr_l = lsdptr_l(b_l);
  msdptra_l = msdptr_l(a_l);
  msdptrb_l = msdptr_l(b_l);
  setdigits_l(s_l, (USHORT)(digits_l(a_l) + dgts));
  if (msdptrb_l < b_l + dgts)
    {
      while (bptr_l <= msdptrb_l)
	{
	  *sptr_l = *bptr_l;
	  ++sptr_l;
	  ++bptr_l;
	}
    }
  else
    {
      while (bptr_l <= b_l + dgts)
	{
	  *sptr_l = *bptr_l;
	  ++sptr_l;
	  ++bptr_l;
	}
    }
  while (sptr_l <= s_l + dgts)
    {
      *sptr_l = 0;
      ++sptr_l;
    }
  while (bptr_l <= msdptrb_l)
    {
      t = (ULONG)(*aptr_l) + (ULONG)(*bptr_l) + (ULONG)carry; /*ARCH64*/
      *sptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++sptr_l;
      ++aptr_l;
      ++bptr_l;
    }
  while (aptr_l <= msdptra_l)
    {
      t = (ULONG)(*aptr_l) + (ULONG)carry;  /*ARCH64*/
      *sptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++sptr_l;
      ++aptr_l;
    }
  if (carry == 1)
    {
      *sptr_l = 1;
      setdigits_l(s_l, (USHORT)(digits_l(s_l) + 1));
    }
  rmldzeros_l(s_l);

  PURGEVARS_L((2, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((2, sizeof(carry), &carry, sizeof(t), &t));
}
  

static void mulkar(BIGINT a_l, BIGINT b_l, BIGINTD p_l)
{
  BIGINT  a0_l, a1_l, b0_l, b1_l, c0_l, c2_l, ctemp1_l, ctemp2_l;
  BIGINTD temp_l;
  bigint c1_l[1 + (BIGINTMAXDIGIT + 2)];
  USHORT ldiv2, i;

  if ((eqz_l(a_l)) || (eqz_l(b_l)))
    setzero_l(p_l);
  else if ((digits_l(a_l) == digits_l(b_l)) && ((digits_l(a_l) & 1) == 0) && (digits_l(a_l) >= MUL_LIMIT))
    {
      ldiv2 = (USHORT)(digits_l(a_l) / 2);
      setdigits_l(a0_l, ldiv2); 
      setdigits_l(a1_l, ldiv2);
      setdigits_l(b0_l, ldiv2);
      setdigits_l(b1_l, ldiv2);
      for (i = 1; i <= ldiv2; i++)
	{
	  a0_l[i] = a_l[i];
	  a1_l[i] = a_l[i + ldiv2];
	  b0_l[i] = b_l[i];
	  b1_l[i] = b_l[i + ldiv2];
	}
      mulkar(a0_l, b0_l, c0_l);
      mulkar(a1_l, b1_l, c2_l);
      add(a0_l, a1_l, ctemp1_l);
      add(b0_l, b1_l, ctemp2_l);
      mulkar(ctemp1_l, ctemp2_l, c1_l);
      sub(c1_l, c0_l, temp_l);
      sub(temp_l, c2_l, c1_l);
      addshift(c2_l, c1_l, ldiv2, temp_l);
      addshift(temp_l, c0_l, ldiv2, p_l);
    }
  else
    {
      mul(a_l, b_l, p_l);
      rmldzeros_l(p_l);
    }

  PURGEVARS_L((10, sizeof(a0_l), a0_l, sizeof(a1_l), a1_l, sizeof(b0_l), b0_l, sizeof(b1_l), b1_l,
	      sizeof(c0_l), c0_l, sizeof(c1_l), c1_l, sizeof(c2_l), c2_l,
	      sizeof(ctemp1_l), ctemp1_l, sizeof(ctemp2_l), ctemp2_l, sizeof(temp_l), temp_l));
  ISPURGED_L((10, sizeof(a0_l), a0_l, sizeof(a1_l), a1_l, sizeof(b0_l), b0_l, sizeof(b1_l), b1_l,
		    sizeof(c0_l), c0_l, sizeof(c1_l), c1_l, sizeof(c2_l), c2_l,
		    sizeof(ctemp1_l), ctemp1_l, sizeof(ctemp2_l), ctemp2_l, sizeof(temp_l), temp_l));
}

static void sqrkar(BIGINT a_l, BIGINTD p_l)
{
  BIGINT  a0_l, a1_l, c0_l, c2_l, ctemp_l;
  BIGINTD temp_l;
  bigint c1_l[1 + (BIGINTMAXDIGIT + 2)];
  USHORT ldiv2, i;

  if (eqz_l(a_l))
    setzero_l(p_l);
  else  if (((digits_l(a_l) & 1) == 0) && (digits_l(a_l) >= SQR_LIMIT))
    {
      ldiv2 = (USHORT)(digits_l(a_l) / 2);
      setdigits_l(a0_l, ldiv2); 
      setdigits_l(a1_l, ldiv2);
      for (i = 1; i <= ldiv2; i++)
	{
	  a0_l[i] = a_l[i];
	  a1_l[i] = a_l[i + ldiv2];
	}
      sqrkar(a0_l, c0_l);
      sqrkar(a1_l, c2_l);
      add(a0_l, a1_l, ctemp_l);
      sqrkar(ctemp_l, c1_l);
      sub(c1_l, c0_l, temp_l);
      sub(temp_l, c2_l, c1_l);
      addshift(c2_l, c1_l, ldiv2, temp_l);
      addshift(temp_l, c0_l, ldiv2, p_l);
    }
  else
    {
      sqr(a_l, p_l);
      rmldzeros_l(p_l);
    }

  PURGEVARS_L((7, sizeof(a0_l), a0_l, sizeof(a1_l), a1_l, sizeof(c0_l), c0_l, sizeof(c1_l), c1_l,
	      sizeof(c2_l), c2_l, sizeof(ctemp_l), ctemp_l, sizeof(temp_l), temp_l));
  ISPURGED_L((7, sizeof(a0_l), a0_l, sizeof(a1_l), a1_l, sizeof(c0_l), c0_l, sizeof(c1_l), c1_l,
		    sizeof(c2_l), c2_l, sizeof(ctemp_l), ctemp_l, sizeof(temp_l), temp_l));
}

int mulkar_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l)
{
  BIGINT a_l, b_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);
  mulkar(a_l, b_l, pp_l);
  if (digits_l(pp_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(pp_l), pp_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(pp_l), pp_l));

  return OFL;
}

int sqrkar_l(BIGINT aa_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD pp_l;
  int OFL = BIGINT_OK;

  copy_l(a_l, aa_l);
  sqrkar(a_l, pp_l);
  if (digits_l(pp_l) > (USHORT)BIGINTMAXDIGIT)
    {
      setdigits_l(pp_l, (USHORT)BIGINTMAXDIGIT);
      rmldzeros_l(pp_l);
      OFL = BIGINT_OFL;
    }
  copy_l(p_l, pp_l);

  PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(pp_l), pp_l));
  ISPURGED_L((2, sizeof(a_l), a_l, sizeof(pp_l), pp_l));

  return OFL;
}

/* Fonction de multiplication optimisée */

int mul_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l)
{
  return mulkar_l(aa_l, bb_l, p_l);
}

/* Fonction de carré optimisée */

int sqr_l(BIGINT aa_l, BIGINT p_l)
{
  return sqrkar_l(aa_l, p_l);
}

/* Fonctions arithmétiques modulo */

int addmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT s_l)
{
  BIGINT a_l, b_l;
  BIGINTP t_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  add(a_l, b_l , t_l);
  if ((ge_l(a_l, mo_l) == 1) || (ge_l(b_l, mo_l) == 1))
    mod_l(t_l, mo_l, s_l);
  else
    {
      if (ge_l(t_l, mo_l) == 1)
	sub(t_l, mo_l, t_l);
      copy_l(s_l, t_l);
    }

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));

  return BIGINT_OK;
}

int submod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT d_l)
{
  BIGINT a_l, b_l, t_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);

  if (ge_l(a_l, b_l) == 1)
    {
      sub(a_l, b_l, t_l);
      mod_l(t_l, mo_l, d_l);
    }
  else
    {
      sub(b_l, a_l, t_l);
      mod_l(t_l, mo_l, t_l);
      if (gtzero_l(t_l) == 1)
	sub(mo_l, t_l, d_l);
      else
	setzero_l(d_l);
    }

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));

  return BIGINT_OK;
}

int mulmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, b_l;
  BIGINTD t_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  copy_l(b_l, bb_l);
  mul(a_l, b_l, t_l);
  mod_l(t_l, mo_l, p_l);

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));

  return BIGINT_OK;
}

int sqrmod_l(BIGINT aa_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTD t_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;

  copy_l(a_l, aa_l);
  sqr(a_l, t_l);
  mod_l(t_l, mo_l, p_l);

  PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(t_l), t_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(t_l), t_l));

  return BIGINT_OK;
}

int uaddmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT s_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = addmod_l(a_l, t_l, mo_l, s_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

  return err;
}

int usubmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT d_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = submod_l(a_l, t_l, mo_l, d_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

  return err;
}

int umulmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT p_l)
{
  int err;
  BIGINT t_l;

  u2bigint_l(t_l, b);
  err = mulmod_l(a_l, t_l, mo_l, p_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

  return err;
}

int eqmod_l(BIGINT a_l, BIGINT b_l, BIGINT mo_l)
{
  BIGINT t_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  submod_l(a_l, b_l, mo_l, t_l);

  return eqz_l(t_l);
}

/* Fonctions exponentielles */

int uexpmod_l(BIGINT aa_l, USHORT e, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l;
  USHORT mask = (USHORT)BDIV2;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
  mod_l(aa_l, mo_l, a_l);
  while ((e & mask) == 0)
    mask = (mask >> 1);
  mask = (mask >> 1);
  while (mask != 0)
    {
      sqrmod_l(a_l, mo_l, a_l);
      if (e & mask)
	mulmod_l(a_l, aa_l, mo_l, a_l);
      mask = (mask >> 1);
    }
  copy_l(p_l, a_l);

  PURGEVARS_L((1, sizeof(a_l), a_l));
  ISPURGED_L((1, sizeof(a_l), a_l));

  return BIGINT_OK;
}

int wexpmod_l(USHORT a, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, e_l;
  USHORT k, b, w;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
      mod_l(a_l, mo_l, p_l);
      return BIGINT_OK;
    }
  setone_l(a_l);
  copy_l(e_l, ee_l);
  b = (USHORT)(1 << ((numbits_l(e_l) - 1) & (BITPERDGT - 1)));
  w = e_l[digits_l(e_l)];
  while (b > 0)
    {
      sqrmod_l(a_l, mo_l, a_l);
      if ((w & b) > 0)
	umulmod_l(a_l, a, mo_l, a_l);
      b = (b >> 1);
    }
  for (k = (USHORT)(digits_l(e_l) - 1); k > 0; k--)
    {
      w = e_l[k];
      b = (USHORT)BDIV2;
      while (b > 0)
	{
	  sqrmod_l(a_l, mo_l, a_l);
	  if ((w & b) > 0)
	    umulmod_l(a_l, a, mo_l, a_l);
	  b = (b >> 1);
	}
    }
  copy_l(p_l, a_l);

  PURGEVARS_L((5, sizeof(a_l), a_l, sizeof(e_l), e_l,
	      sizeof(b), &b, sizeof(w), &w, sizeof(k), &k));
  ISPURGED_L((5, sizeof(a_l), a_l, sizeof(e_l), e_l,
	      sizeof(b), &b, sizeof(w), &w, sizeof(k), &k));

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

int expmodk_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, a2_l;
  BIGINTP e_l;
  BIGINTD temp_l;
  bigint **pptr_l, *ptr_l = NULL;
  int i, f, tab; /*ARCH64*/
  unsigned int k, lge, pow2k, mask, fk, s, d; /*ARCH64*/
  USHORT digit;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
    --k;
  pow2k = (1 << k);
  mask = pow2k - 1;
  pptr_l = (bigint **)malloc(sizeof(bigint *) * pow2k);
  if (pptr_l  == NULL)
    {

      PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(e_l), e_l));
      ISPURGED_L((2, sizeof(a_l), a_l, sizeof(e_l), e_l));

      return BIGINT_MAL;
    }
  mod_l(a_l, mo_l, a_l);
  pptr_l[1] = a_l;
  if (k > 1)
    {
      ptr_l = (bigint *)malloc(sizeof(BIGINT) * ((pow2k >> 1) - 1));
      if (ptr_l == NULL)
	{
	  free(pptr_l);

	  PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(e_l), e_l));
	  ISPURGED_L((2, sizeof(a_l), a_l, sizeof(e_l), e_l));

	  return BIGINT_MAL;
	}
      pptr_l[2] = a2_l;
      pptr_l[3] = ptr_l;
      for (i = 5; i < (int)pow2k; i += 2) /*ARCH64*/
	pptr_l[i] = pptr_l[i - 2] + BIGINTMAXSHORT;
      sqrmod_l(a_l, mo_l, pptr_l[2]);
      for (i = 3; i < (int)pow2k; i += 2) /*ARCH64*/
	mulmod_l(pptr_l[2], pptr_l[i - 2], mo_l, pptr_l[i]);
    }
  *(msdptr_l(e_l) + 1) = 0;
  f = (int)((lge - 1) / k); /*ARCH64*/
  fk = (unsigned int)f * k;
  s = (fk >> LDBITPERDGT);
  d = fk & (BITPERDGT - 1);
  digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & mask); /*ARCH64*/
  if (digit != 0)
    {
      copy_l(temp_l, pptr_l[oddtab[digit]]);
      tab = twotab[digit];
      for (i = 1; i <= tab; i++) /*ARCH64*/
	sqrmod_l(temp_l, mo_l, temp_l);
    }
  else
    setone_l(temp_l);
  fk = fk - k;
  for (f = f - 1; f >= 0; f--)
    {
      s = (fk >> LDBITPERDGT);
      d = fk & (BITPERDGT - 1);
      digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & mask); /*ARCH64*/
      if (digit != 0)
	{
	  tab = twotab[digit];
	  for (i = 1; i <= ((int)k - tab); i++) /*ARCH64*/
	    sqrmod_l(temp_l, mo_l, temp_l);
	  mulmod_l(temp_l, pptr_l[oddtab[digit]], mo_l, temp_l);
	  for (i = 1; i <= tab; i++) /*ARCH64*/
	    sqrmod_l(temp_l, mo_l, temp_l);
	}
      else
	{
	  for (i = 1; i <= (int)k; i++)
	    sqrmod_l(temp_l, mo_l, temp_l);
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

  PURGEVARS_L((13, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	      sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	      sizeof(tab), &tab, sizeof(k), &k,
	      sizeof(lge), &lge, sizeof(pow2k), &pow2k,
	      sizeof(mask), &mask, sizeof(fk), &fk,
	      sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit));
  ISPURGED_L((13, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
		    sizeof(temp_l), temp_l, sizeof(e_l), e_l,
		    sizeof(tab), &tab, sizeof(k), &k,
		    sizeof(lge), &lge, sizeof(pow2k), &pow2k,
		    sizeof(mask), &mask, sizeof(fk), &fk,
		    sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit));

  return BIGINT_OK;
}

int expmod7_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l;
  BIGINTP e_l;
  BIGINTD temp_l;
  BIGINT a2_l, a3_l, a5_l, a7_l, a9_l, a11_l, a13_l, a15_l, a17_l, a19_l, a21_l, a23_l, a25_l, a27_l, a29_l, a31_l, a33_l, a35_l, a37_l, a39_l, a41_l, a43_l, a45_l, a47_l, a49_l, a51_l, a53_l, a55_l, a57_l, a59_l, a61_l, a63_l, a65_l, a67_l, a69_l, a71_l, a73_l, a75_l, a77_l, a79_l, a81_l, a83_l, a85_l, a87_l, a89_l, a91_l, a93_l, a95_l,  a97_l, a99_l, a101_l, a103_l, a105_l, a107_l, a109_l, a111_l, a113_l, a115_l, a117_l, a119_l, a121_l, a123_l, a125_l, a127_l;
  bigint *pptr_l[128];
  int i, f, tab; /*ARCH64*/
  unsigned int lge, f7, s, d;
  USHORT digit;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
  mod_l(a_l, mo_l, a_l);
  pptr_l[1] = a_l;
  pptr_l[3] = a3_l;
  pptr_l[5] = a5_l;
  pptr_l[7] = a7_l;
  pptr_l[9] = a9_l;
  pptr_l[11] = a11_l;
  pptr_l[13] = a13_l;
  pptr_l[15] = a15_l;
  pptr_l[17] = a17_l;
  pptr_l[19] = a19_l;
  pptr_l[21] = a21_l;
  pptr_l[23] = a23_l;
  pptr_l[25] = a25_l;  
  pptr_l[27] = a27_l;
  pptr_l[29] = a29_l;
  pptr_l[31] = a31_l;
  pptr_l[33] = a33_l;
  pptr_l[35] = a35_l;
  pptr_l[37] = a37_l;
  pptr_l[39] = a39_l;
  pptr_l[41] = a41_l;
  pptr_l[43] = a43_l;
  pptr_l[45] = a45_l;  
  pptr_l[47] = a47_l;
  pptr_l[49] = a49_l;
  pptr_l[51] = a51_l;
  pptr_l[53] = a53_l;
  pptr_l[55] = a55_l;
  pptr_l[57] = a57_l;
  pptr_l[59] = a59_l;
  pptr_l[61] = a61_l;
  pptr_l[63] = a63_l;
  pptr_l[65] = a65_l;  
  pptr_l[67] = a67_l;
  pptr_l[69] = a69_l;
  pptr_l[71] = a71_l;
  pptr_l[73] = a73_l;
  pptr_l[75] = a75_l;
  pptr_l[77] = a77_l;
  pptr_l[79] = a79_l;
  pptr_l[81] = a81_l;
  pptr_l[83] = a83_l;
  pptr_l[85] = a85_l;  
  pptr_l[87] = a87_l;
  pptr_l[89] = a89_l;
  pptr_l[91] = a91_l;
  pptr_l[93] = a93_l;
  pptr_l[95] = a95_l;
  pptr_l[97] = a97_l;
  pptr_l[99] = a99_l;
  pptr_l[101] = a101_l;
  pptr_l[103] = a103_l;
  pptr_l[105] = a105_l;  
  pptr_l[107] = a107_l;
  pptr_l[109] = a109_l;
  pptr_l[111] = a111_l;
  pptr_l[113] = a113_l;
  pptr_l[115] = a115_l;
  pptr_l[117] = a117_l;
  pptr_l[119] = a119_l;
  pptr_l[121] = a121_l;
  pptr_l[123] = a123_l;
  pptr_l[125] = a125_l;  
  pptr_l[127] = a127_l;
  sqrmod_l(a_l, mo_l, a2_l);
  for (i = 3; i < 128; i += 2) /*ARCH64*/
    mulmod_l(a2_l, pptr_l[i - 2], mo_l, pptr_l[i]);
  *(msdptr_l(e_l) + 1) = 0;
  f = (int)((lge - 1) / 7); /*ARCH64*/
  f7 = (unsigned int)f * 7;
  s = (f7 >> LDBITPERDGT);
  d = f7 & (BITPERDGT - 1);
  digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & 0x7f); /*ARCH64*/
  if (digit != 0)
    {
      copy_l(temp_l, pptr_l[oddtab[digit]]);
      tab = twotab[digit];
      for (i = 1; i <= tab; i++) /*ARCH64*/
	sqrmod_l(temp_l, mo_l, temp_l);
    }
  else
    setone_l(temp_l);
  f7 = f7 - 7;
  for (f = f - 1; f >= 0; f--)
    {
      s = (f7 >> LDBITPERDGT);
      d = f7 & (BITPERDGT - 1);
      digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & 0x7f); /*ARCH64*/
      if (digit != 0)
	{
	  tab = twotab[digit];
	  for (i = 1; i <= ((int)7 - tab); i++) /*ARCH64*/
	    sqrmod_l(temp_l, mo_l, temp_l);
	  mulmod_l(temp_l, pptr_l[oddtab[digit]], mo_l, temp_l);
	  for (i = 1; i <= tab; i++) /*ARCH64*/
	    sqrmod_l(temp_l, mo_l, temp_l);
	}
      else
	{
	  for (i = 1; i <= (int)7; i++)
	    sqrmod_l(temp_l, mo_l, temp_l);
	}
      f7 = f7 - 7;
    }
  copy_l(p_l, temp_l);

  PURGEVARS_L((73, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	       sizeof(a3_l), a3_l, sizeof(a5_l), a5_l, 
	       sizeof(a7_l), a7_l, sizeof(a9_l), a9_l,
	       sizeof(a11_l), a11_l, sizeof(a13_l), a13_l,
	       sizeof(a15_l), a15_l, sizeof(a17_l), a17_l, 
	       sizeof(a19_l), a19_l, sizeof(a21_l), a21_l, 
	       sizeof(a23_l), a23_l, sizeof(a25_l), a25_l, 
	       sizeof(a27_l), a27_l, sizeof(a29_l), a29_l, 
	       sizeof(a31_l), a31_l, sizeof(a33_l), a33_l, 
	       sizeof(a35_l), a35_l, sizeof(a37_l), a37_l, 
	       sizeof(a39_l), a39_l, sizeof(a41_l), a41_l, 
	       sizeof(a43_l), a43_l, sizeof(a45_l), a45_l, 
	       sizeof(a47_l), a47_l, sizeof(a49_l), a49_l, 
	       sizeof(a51_l), a51_l, sizeof(a53_l), a53_l, 
	       sizeof(a55_l), a55_l, sizeof(a57_l), a57_l, 
	       sizeof(a59_l), a59_l, sizeof(a61_l), a61_l,
	       sizeof(a63_l), a63_l, sizeof(a65_l), a65_l, 
	       sizeof(a67_l), a67_l, sizeof(a69_l), a69_l,
	       sizeof(a71_l), a71_l, sizeof(a73_l), a73_l,
	       sizeof(a75_l), a75_l, sizeof(a77_l), a77_l, 
	       sizeof(a79_l), a79_l, sizeof(a81_l), a81_l, 
	       sizeof(a83_l), a83_l, sizeof(a85_l), a85_l, 
	       sizeof(a87_l), a87_l, sizeof(a89_l), a89_l, 
	       sizeof(a91_l), a91_l, sizeof(a93_l), a93_l, 
	       sizeof(a95_l), a95_l, sizeof(a97_l), a97_l, 
	       sizeof(a99_l), a99_l, sizeof(a101_l), a101_l, 
	       sizeof(a103_l), a103_l, sizeof(a105_l), a105_l, 
	       sizeof(a107_l), a107_l, sizeof(a109_l), a109_l, 
	       sizeof(a111_l), a111_l, sizeof(a113_l), a113_l, 
	       sizeof(a115_l), a115_l, sizeof(a117_l), a117_l, 
	       sizeof(a119_l), a119_l, sizeof(a121_l), a121_l,
	       sizeof(a123_l), a123_l, sizeof(a125_l), a125_l, 
	       sizeof(a127_l), a27_l,
	       sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	       sizeof(tab), &tab, sizeof(lge), &lge, 
	       sizeof(f7), &f7, sizeof(s), &s, sizeof(d), &d, 
	       sizeof(digit), &digit));
  ISPURGED_L((73, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	      sizeof(a3_l), a3_l, sizeof(a5_l), a5_l, 
	      sizeof(a7_l), a7_l, sizeof(a9_l), a9_l,
	      sizeof(a11_l), a11_l, sizeof(a13_l), a13_l,
	      sizeof(a15_l), a15_l, sizeof(a17_l), a17_l, 
	      sizeof(a19_l), a19_l, sizeof(a21_l), a21_l, 
	      sizeof(a23_l), a23_l, sizeof(a25_l), a25_l, 
	      sizeof(a27_l), a27_l, sizeof(a29_l), a29_l, 
	      sizeof(a31_l), a31_l, sizeof(a33_l), a33_l, 
	      sizeof(a35_l), a35_l, sizeof(a37_l), a37_l, 
	      sizeof(a39_l), a39_l, sizeof(a41_l), a41_l, 
	      sizeof(a43_l), a43_l, sizeof(a45_l), a45_l, 
	      sizeof(a47_l), a47_l, sizeof(a49_l), a49_l, 
	      sizeof(a51_l), a51_l, sizeof(a53_l), a53_l, 
	      sizeof(a55_l), a55_l, sizeof(a57_l), a57_l, 
	      sizeof(a59_l), a59_l, sizeof(a61_l), a61_l,
	      sizeof(a63_l), a63_l, sizeof(a65_l), a65_l, 
	      sizeof(a67_l), a67_l, sizeof(a69_l), a69_l,
	      sizeof(a71_l), a71_l, sizeof(a73_l), a73_l,
	      sizeof(a75_l), a75_l, sizeof(a77_l), a77_l, 
	      sizeof(a79_l), a79_l, sizeof(a81_l), a81_l, 
	      sizeof(a83_l), a83_l, sizeof(a85_l), a85_l, 
	      sizeof(a87_l), a87_l, sizeof(a89_l), a89_l, 
	      sizeof(a91_l), a91_l, sizeof(a93_l), a93_l, 
	      sizeof(a95_l), a95_l, sizeof(a97_l), a97_l, 
	      sizeof(a99_l), a99_l, sizeof(a101_l), a101_l, 
	      sizeof(a103_l), a103_l, sizeof(a105_l), a105_l, 
	      sizeof(a107_l), a107_l, sizeof(a109_l), a109_l, 
	      sizeof(a111_l), a111_l, sizeof(a113_l), a113_l, 
	      sizeof(a115_l), a115_l, sizeof(a117_l), a117_l, 
	      sizeof(a119_l), a119_l, sizeof(a121_l), a121_l,
	      sizeof(a123_l), a123_l, sizeof(a125_l), a125_l, 
	      sizeof(a127_l), a27_l,
	      sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	      sizeof(tab), &tab, sizeof(lge), &lge, 
	      sizeof(f7), &f7, sizeof(s), &s, sizeof(d), &d, 
	      sizeof(digit), &digit));

  return BIGINT_OK;
}

int expmod2k_l(BIGINT aa_l, USHORT k, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l;

  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (k > 0)
    {
      copy_l(a_l, aa_l);
      while (k > 0)
	{
	  sqrmod_l(a_l, mo_l, a_l);
	  --k;
	}
      copy_l(p_l, a_l);
    }
  else
    mod_l(aa_l, mo_l, p_l);

  PURGEVARS_L((1, sizeof(a_l), a_l));
  ISPURGED_L((1, sizeof(a_l), a_l));

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

      PURGEVARS_L((1, sizeof(a_l), a_l));
      ISPURGED_L((1, sizeof(a_l), a_l));
      
      return 0;
    }
  temp = a_l[l];
  l = (unsigned int)(l << LDBITPERDGT);
  while ((temp & BDIV2) == 0)
    {
      temp = (USHORT)(temp << 1);
      --l;
    }

  PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(temp), &temp));
  ISPURGED_L((2, sizeof(a_l), a_l, sizeof(temp), &temp));

  return l;
}

int shleft_l(BIGINT a_l)
{
  bigint *aptr_l, *msdptra_l;
  USHORT carry = 0;
  ULONG t = 0L;
  int err = BIGINT_OK;

  rmldzeros_l(a_l);
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);
  while (aptr_l <= msdptra_l)
    {
      t = ((ULONG)(*aptr_l) << 1) + (ULONG)carry;
      *aptr_l = (USHORT)(t & BMIN1);
      carry = (USHORT)(t >> BITPERDGT);
      ++aptr_l;
    }
  if (carry)
    {
      if (digits_l(a_l) < (USHORT)BIGINTMAXDIGIT)
	{
	  *aptr_l = 1;
	  setdigits_l(a_l, (USHORT)(digits_l(a_l) + 1));
	  err = BIGINT_OK;
	}
      else
	err = BIGINT_OFL;
    }
  rmldzeros_l(a_l);

  PURGEVARS_L((2, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((2, sizeof(carry), &carry, sizeof(t), &t));

  return err;
}

int shright_l(BIGINT a_l)
{
  bigint *aptr_l, *msdptra_l;
  ULONG t = 0L;
  USHORT carry = 0L;
  int err = BIGINT_OK;

  if (eqz_l(a_l))
    err = BIGINT_UFL;
  aptr_l = lsdptr_l(a_l);
  msdptra_l = msdptr_l(a_l);
  while (msdptra_l >= aptr_l)
    {
      t = (ULONG)(*msdptra_l >> 1) + ((ULONG)carry << (BITPERDGT - 1));
      carry = *msdptra_l & 1;
      *msdptra_l = (USHORT)t;
      --msdptra_l;
    }
  rmldzeros_l(a_l);

  PURGEVARS_L((2, sizeof(carry), &carry, sizeof(t), &t));
  ISPURGED_L((2, sizeof(carry), &carry, sizeof(t), &t));

  return err;
}

int shift_l(BIGINT a_l, long int nbrebits)
{
  bigint *aptr_l, *msdptra_l;
  USHORT ints, bits;
  long int res;
  USHORT i;
  int err = BIGINT_OK;

  rmldzeros_l(a_l);
  if (nbrebits < 0)
    {
      ints = (USHORT)((-nbrebits) / BITPERDGT);
      bits = (USHORT)((-nbrebits) % BITPERDGT);
    }
  else
    {
      ints = (USHORT)(nbrebits / BITPERDGT);
      bits = (USHORT)(nbrebits % BITPERDGT);
    }
  res = (long int)numbits_l(a_l) + nbrebits;
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
	  ++aptr_l;
	}
      setdigits_l(a_l, digits_l(a_l) - ints);
      for (i = 0; i < bits; i++)
	shright_l(a_l);
    }
  else
    {
      if (ints < (USHORT)BIGINTMAXDIGIT)
	{
	  if (digits_l(a_l) + ints < (USHORT)BIGINTMAXDIGIT)
	    setdigits_l(a_l, digits_l(a_l) + ints);
	  else
	    setdigits_l(a_l, (USHORT)BIGINTMAXDIGIT);
	  msdptra_l = msdptr_l(a_l);
	  aptr_l = a_l + ints;
	  while (msdptra_l > aptr_l)
	    {
	      *msdptra_l = *(msdptra_l - ints);
	      --msdptra_l;
	    }
	  while (msdptra_l > a_l)
	    {
	      *msdptra_l = 0;
	      --msdptra_l;
	    }
	  for (i = 0; i < bits; i++)
	    shleft_l(a_l);
	} 
      else
	setzero_l(a_l);
    }

  PURGEVARS_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(res), &res));
  ISPURGED_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(res), &res));

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
      ++tptr_l;
      ++aptr_l;
      ++bptr_l;
    }
  copy_l(c_l, t_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));

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
      ++tptr_l;
      ++aptr_l;
      ++bptr_l;
    }
  while (bptr_l <= msdptrb_l)
    {
      *tptr_l = *bptr_l;
      ++tptr_l;
      ++bptr_l;
    }
  copy_l(c_l, t_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));
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
      ++tptr_l;
      ++aptr_l;
      ++bptr_l;
    }
  while (bptr_l <= msdptrb_l)
    {
      *tptr_l = *bptr_l;
      ++tptr_l;
      ++bptr_l;
    }
  copy_l(c_l, t_l);

  PURGEVARS_L((1, sizeof(t_l), t_l));
  ISPURGED_L((1, sizeof(t_l), t_l));
}

int setbit_l(BIGINT a_l, unsigned int pos)
{
  int i;
  USHORT ints = (USHORT)(pos / BITPERDGT);
  USHORT bits = (USHORT)(pos % BITPERDGT);
  USHORT mask = (USHORT)(1 << bits);
  int ret = 0;

  if (pos > BIGINTMAXBIT)
    return BIGINT_OFL;
  if (ints >= digits_l(a_l))
    {
      for (i = digits_l(a_l) + 1; i <= (int)ints + 1; i++)
	a_l[i] = 0;
      setdigits_l(a_l, (USHORT)(ints  + 1));
    }
  if (a_l[ints  + 1] & mask)
    ret = 1;
  a_l[ints + 1] = a_l[ints  + 1] | mask;

  PURGEVARS_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));
  ISPURGED_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));

  return ret;
}

int testbit_l(BIGINT a_l, unsigned int pos)
{
  USHORT ints = (USHORT)(pos / BITPERDGT);
  USHORT bits = (USHORT)(pos % BITPERDGT);
  USHORT mask = (USHORT)(1 << bits);
  int ret = 0;

  if (ints < digits_l(a_l))
    {
      if (a_l[ints + 1] & mask)
	ret = 1;
    }

  PURGEVARS_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));
  ISPURGED_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));

  return ret;
}

int clearbit_l(BIGINT a_l, unsigned int pos)
{
  USHORT ints = (USHORT)(pos / BITPERDGT);
  USHORT bits = (USHORT)(pos % BITPERDGT);
  USHORT mask = (USHORT)(1 << bits);
  int ret = 0;

  if (ints < digits_l(a_l))
    {
      if (a_l[ints + 1] & mask)
	ret = 1;
      a_l[ints + 1] = a_l[ints + 1] & (~mask);
      rmldzeros_l(a_l);
    }

  PURGEVARS_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));
  ISPURGED_L((3, sizeof(ints), &ints, sizeof(bits), &bits, sizeof(mask), &mask));

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
	{
	  ++str;
	  ++str;
	}
    }
  while (isxdigit(*str) || isspace(*str))
    {
      if (!isspace(*str))
	{
	  n = (USHORT)*str;
	  switch (n)
	    {
	    case 'a':
	    case 'b':
	    case 'c':
	    case 'd':
	    case 'e':
	    case 'f':
	      n = (USHORT)(n - (USHORT)'a' + 10);
	      break;
	    default:
	      n = n - (USHORT)'0';
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
      ++str;
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
	  ++i;
	}
      else
	{
	  n[i] = '0';
	  ++i;
	}
    }
  while (gtzero_l(t_l));
  if (showb)
    {
      switch (b)
	{
	case 2:
	  n[i] = 'b';
	  ++i;
	  n[i] = '0';
	  ++i;
	  break;
	case 8:
	  n[i] = '0';
	  ++i;
	  break;
	case 16:
	  n[i] = 'x';
	  ++i;
	  n[i] = '0';
	  ++i;
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
       --end;
    }
  return n;
}

int byte2bigint_l(BIGINT n_l, UCHAR* bytestr, int len)
{
  int i, err = BIGINT_OK;

  if (bytestr == NULL)
    return BIGINT_NPT;
  if ((unsigned int)len > BIGINTMAXBYTE)
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
      ++bytestr;
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
	  bytes[i] = (UCHAR)*lsdptr_l(r_l);
	  ++i;
	}
      else
	{
	  bytes[i] = 0;
	  ++i;
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
      --j;
    }

  PURGEVARS_L((5, sizeof(t_l), t_l, sizeof(r_l), r_l,
	      sizeof(i), &i, sizeof(j), &j,
	      sizeof(temp), &temp));
  ISPURGED_L((5, sizeof(t_l), t_l, sizeof(r_l), r_l,
		    sizeof(i), &i, sizeof(j), &j,
		    sizeof(temp), &temp));

  return bytes;
}

int check_l(BIGINT n_l)
{
  int err = CHECK_OK;
  
  if (n_l == NULL)
    err = CHECK_MEM;
  else
    {
      if (digits_l(n_l) > (USHORT)BIGINTMAXDIGIT)
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

      PURGEVARS_L((2, sizeof(a_l), a_l, sizeof(b_l), b_l));
      ISPURGED_L((2, sizeof(a_l), a_l, sizeof(b_l), b_l));

      return;
    }
  while (iseven_l(a_l) && iseven_l(b_l))
    {
      shright_l(a_l);
      shright_l(b_l);
      ++k;
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
	  shift_l(g_l, (long int)k);

	  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
	  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));

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

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(t_l), t_l));
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

  PURGEVARS_L((2, sizeof(g_l), g_l, sizeof(dummy_l), dummy_l));
  ISPURGED_L((2, sizeof(g_l), g_l, sizeof(dummy_l), dummy_l));

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

      PURGEVARS_L((1, sizeof(v3_l), v3_l));
      ISPURGED_L((1, sizeof(v3_l), v3_l));

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

  PURGEVARS_L((8, sizeof(v1_l), v1_l, sizeof(v3_l), v3_l,
	      sizeof(t3_l), t3_l, sizeof(q_l), q_l,
	      sizeof(t1_l), t1_l, sizeof(temp_l), temp_l,
	      sizeof(utemp_l), utemp_l, sizeof(vtemp_l), vtemp_l));
  ISPURGED_L((8, sizeof(v1_l), v1_l, sizeof(v3_l), v3_l,
		    sizeof(t3_l), t3_l, sizeof(q_l), q_l,
		    sizeof(t1_l), t1_l, sizeof(temp_l), temp_l,
		    sizeof(utemp_l), utemp_l, sizeof(vtemp_l), vtemp_l));

  return;
} 

void invmod_l(BIGINT a_l, BIGINT mo_l, BIGINT g_l, BIGINT i_l)
{
  BIGINT t0_l, u0_l, u1_l, q_l, temp_l, r_l; /* g_l joue le rôle de t1_l (Voir Mignotte) */

  if (eqz_l(a_l))
    {
      if (eqz_l(mo_l))
	{    
	  setzero_l(g_l);
	  setzero_l(i_l);
	  return;
	}
      else
	{
	  copy_l(g_l, mo_l);
	  setzero_l(i_l);
	  return;
	}
    }
  else
    {
      if (eqz_l(mo_l))
	{
	  copy_l(g_l, a_l);
	  setzero_l(i_l);
	  return;
	}
    }
  copy_l(t0_l, a_l);
  copy_l(g_l, mo_l);
  setone_l(u0_l);
  setzero_l(u1_l);
  do
    {
      div_l(t0_l, g_l, q_l, r_l);
      if (gtzero_l(r_l))
	{	  
	  copy_l(temp_l, u1_l);
	  mulmod_l(u1_l, q_l, mo_l, q_l);
	  submod_l(u0_l, q_l, mo_l, u1_l);
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

  PURGEVARS_L((6, sizeof(t0_l), t0_l, sizeof(u0_l), u0_l,
	      sizeof(u1_l), u1_l, sizeof(q_l), q_l,
	      sizeof(temp_l), temp_l, sizeof(r_l), r_l));
  ISPURGED_L((6, sizeof(t0_l), t0_l, sizeof(u0_l), u0_l,
		    sizeof(u1_l), u1_l, sizeof(q_l), q_l,
		    sizeof(temp_l), temp_l, sizeof(r_l), r_l));
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

  PURGEVARS_L((3, sizeof(x_l), x_l, sizeof(y_l), y_l, sizeof(dummy_l), dummy_l));
  ISPURGED_L((3, sizeof(x_l), x_l, sizeof(y_l), y_l, sizeof(dummy_l), dummy_l));
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

  PURGEVARS_L((1, sizeof(y), &y));
  ISPURGED_L((1, sizeof(y), &y));

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

	      PURGEVARS_L((1, sizeof(q_l), q_l));
	      ISPURGED_L((1, sizeof(q_l), q_l));
	      
	      return 1;
	    }
	}
    }
  setzero_l(r_l);

  PURGEVARS_L((1, sizeof(q_l), q_l));
  ISPURGED_L((1, sizeof(q_l), q_l));

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
      ++v;
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
	  ++v;
	}
      if ((v & 1) != 0)
	k = (long int)tab2[*lsdptr_l(b_l) % 8] * k;
      if ((*lsdptr_l(a_l) % 4 == 3) && (*lsdptr_l(b_l) % 4 == 3))
	k = -k;
      copy_l(r_l, a_l);
      mod_l(b_l, r_l, a_l);
      copy_l(b_l, r_l);
    }
  setone_l(r_l);
  if (comp_l(b_l, r_l) == 1)
    k = 0;

  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(r_l), r_l));
  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(b_l), b_l, sizeof(r_l), r_l));

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
      ++r;
    }
  setone_l(n_l);
  shleft_l(n_l);
  while (jacobi_l(n_l, p_l) == 1)
    uadd_l(n_l, 1, n_l);
  expmod_l(n_l, q_l, p_l, y_l);
  usub_l(q_l, 1, q_l);
  shright_l(q_l);
  expmod_l(a_l, q_l, p_l, x_l);
  sqrmod_l(x_l, p_l, temp_l);
  mulmod_l(temp_l, a_l, p_l, z_l);
  mulmod_l(x_l, a_l, p_l, x_l);
  mod_l(z_l, p_l, q_l);
  while (!eq1_l(q_l))
    {
      m = 0;
      do
	{
	  ++m;
	  sqrmod_l(q_l, p_l, q_l);
	}
      while ((!eq1_l(q_l)) && (m < r));
      if (m == r)
	{
	  
	  PURGEVARS_L((8, sizeof(n_l), n_l, sizeof(q_l), q_l,
		      sizeof(t_l), t_l, sizeof(y_l), y_l,
		      sizeof(z_l), z_l, sizeof(temp_l), temp_l,
		      sizeof(r), &r, sizeof(m), &m));
	  ISPURGED_L((8, sizeof(n_l), n_l, sizeof(q_l), q_l,
			    sizeof(t_l), t_l, sizeof(y_l), y_l,
			    sizeof(z_l), z_l, sizeof(temp_l), temp_l,
			    sizeof(r), &r, sizeof(m), &m));

	  return -1;
	}
      expmod2k_l(y_l, (USHORT)(r - m - 1), p_l, t_l);
      sqrmod_l(t_l, p_l, y_l);
      mulmod_l(x_l, t_l, p_l, x_l);
      mulmod_l(z_l, y_l, p_l, z_l);
      copy_l(q_l, z_l);
      r = m;
    }

  PURGEVARS_L((8, sizeof(n_l), n_l, sizeof(q_l), q_l,
	      sizeof(t_l), t_l, sizeof(y_l), y_l,
	      sizeof(z_l), z_l, sizeof(temp_l), temp_l,
	      sizeof(r), &r, sizeof(m), &m));
  ISPURGED_L((8, sizeof(n_l), n_l, sizeof(q_l), q_l,
		    sizeof(t_l), t_l, sizeof(y_l), y_l,
		    sizeof(z_l), z_l, sizeof(temp_l), temp_l,
		    sizeof(r), &r, sizeof(m), &m));

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

      PURGEVARS_L((2, sizeof(xp_l), xp_l, sizeof(xq_l), xq_l));
      ISPURGED_L((2, sizeof(xp_l), xp_l, sizeof(xq_l), xq_l));
      
      return -1;
    }
  mul_l(p_l, q_l, n_l);
  xgcd_l(p_l, q_l, x0_l, u_l, &usign, v_l, &vsign);
  mulmod_l(u_l, p_l, n_l, u_l); /*Attention dépassement possible, la solution est réduite mod NMAX+1*/
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

  PURGEVARS_L((10, sizeof(x0_l), x0_l, sizeof(x1_l), x1_l,
	    sizeof(x2_l), x2_l, sizeof(x3_l), x3_l,
	    sizeof(xp_l), xp_l, sizeof(xq_l), xq_l,
	    sizeof(n_l), n_l, sizeof(xtemp_l), xtemp_l,
	    sizeof(u_l), u_l, sizeof(v_l), v_l));
  ISPURGED_L((10, sizeof(x0_l), x0_l, sizeof(x1_l), x1_l,
		    sizeof(x2_l), x2_l, sizeof(x3_l), x3_l,
		    sizeof(xp_l), xp_l, sizeof(xq_l), xq_l,
		    sizeof(n_l), n_l, sizeof(xtemp_l), xtemp_l,
		    sizeof(u_l), u_l, sizeof(v_l), v_l));

  return 0;
}

int chinrem_l(unsigned int numeq, bigint **coeff_l, BIGINT x_l)
{
  BIGINT g_l, u_l, v_l, mo_l;
  bigint *ai_l = 0, *mi_l = 0;
  unsigned int i;
  int usign, vsign, xsign, err, error = BIGINT_OK;

  if (numeq == 0)
    return 1;
  copy_l(x_l, *coeff_l);
  ++coeff_l;
  copy_l(mo_l, *coeff_l);
  ++coeff_l;
  for (i = 1; i < numeq; i++)
    {
      ai_l = *coeff_l;
      ++coeff_l;
      mi_l = *coeff_l;
      ++coeff_l;
      xgcd_l(mo_l, mi_l, g_l, u_l, &usign, v_l, &vsign);
      if (!eq1_l(g_l))
	{

	  PURGEVARS_L((4, sizeof(g_l), g_l, sizeof(u_l), u_l,
		      sizeof(v_l), v_l, sizeof(mo_l), mo_l));
	  ISPURGED_L((4, sizeof(g_l), g_l, sizeof(u_l), u_l,
			    sizeof(v_l), v_l, sizeof(mo_l), mo_l));

	  return 2;
	}
      err = mul_l(u_l, mo_l, u_l);
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
      err = mul_l(mo_l, mi_l, mo_l);
      if (error == BIGINT_OK)
	error = err;
      smod(x_l, xsign, mo_l, x_l);
    }

  PURGEVARS_L((4, sizeof(g_l), g_l, sizeof(u_l), u_l,
	      sizeof(v_l), v_l, sizeof(mo_l), mo_l));
  ISPURGED_L((4, sizeof(g_l), g_l, sizeof(u_l), u_l,
		    sizeof(v_l), v_l, sizeof(mo_l), mo_l));

  return error;
}

int primelt_l(BIGINT a_l, unsigned int numsprimes, bigint *primes_l[])
{
  BIGINT p_l, temp_l, dummy_l, g_l;
  BIGINT zero_l;
  USHORT a;
  unsigned int i;

  if (iseven_l(primes_l[0]) == 0)
      return -1;
  setzero_l(zero_l);
  copy_l(p_l, primes_l[0]);
  uadd_l(p_l, 1, p_l);
  do
    {
      do
	{
	  a = usrand64_l();
	  u2bigint_l(a_l, a);
	  gcd_l(a_l, p_l, g_l);
	}
      while (eqmod_l(g_l, zero_l, p_l) == 1);
      if (issqr_l(a_l, temp_l) == 1)
	uadd_l(a_l, 1, a_l);
      i = 1;
      do
	{
	  div_l(primes_l[0], primes_l[i], temp_l, dummy_l);
	  expmod_l(a_l, temp_l, p_l, temp_l);
	  ++i;
	}
      while ((i <= numsprimes) && (eq1_l(temp_l) == 0));
    }
  while (eq1_l(temp_l) == 1);
 
  PURGEVARS_L((3, sizeof(p_l), p_l, sizeof(temp_l), temp_l,
	       sizeof(dummy_l), dummy_l, sizeof(a), &a));
  ISPURGED_L((3, sizeof(p_l), p_l, sizeof(temp_l), temp_l,
		    sizeof(dummy_l), dummy_l, sizeof(a), &a));
 
  return BIGINT_OK;
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
    --L;
  f = (char *) malloc (L + 1);
  if (f == NULL)
    {

      PURGEVARS_L((7, sizeof(B0), &B0, sizeof(L), &L,
		  sizeof(i), &i, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(c), &c));
      ISPURGED_L((7, sizeof(B0), &B0, sizeof(L), &L,
			sizeof(i), &i, sizeof(k), &k,
			sizeof(p), &p, sizeof(s), &s,
			sizeof(c), &c));

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
      ++p;
      ++p;
    }
  c = 1;
  for (i = 1; i <= L; i++)
    c = c + f[i];
  primes = (ULONG *)malloc((c + 1) * sizeof(ULONG));
  if (primes == NULL)
    {
      
      PURGEVARS_L((7, sizeof(B0), &B0, sizeof(L), &L,
		  sizeof(i), &i, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(c), &c));
      ISPURGED_L((7, sizeof(B0), &B0, sizeof(L), &L,
			sizeof(i), &i, sizeof(k), &k,
			sizeof(p), &p, sizeof(s), &s,
			sizeof(c), &c));

      return NULL;
    }
  c = 1;
  for (i = 1; i <= L; i++)
    {
      if (f[i] != 0)
	{
	  ++c;
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

  PURGEVARS_L((7, sizeof(B0), &B0, sizeof(L), &L,
	      sizeof(i), &i, sizeof(k), &k,
	      sizeof(p), &p, sizeof(s), &s,
	      sizeof(c), &c));
  ISPURGED_L((7, sizeof(B0), &B0, sizeof(L), &L,
		    sizeof(i), &i, sizeof(k), &k,
		    sizeof(p), &p, sizeof(s), &s,
		    sizeof(c), &c));

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
	  
	  PURGEVARS_L((1, sizeof(n_l), n_l));
	  ISPURGED_L((1, sizeof(n_l), n_l));
	  
	  return 1;
	}
      else
	{

	  PURGEVARS_L((1, sizeof(n_l), n_l));
	  ISPURGED_L((1, sizeof(n_l), n_l));

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
	  t = ((ULONG)r0 << BITPERDGT) + (ULONG)(*aptr_l);
	  q0 = (USHORT)(t / (ULONG)b0);
	  r0 = (USHORT)(t % (ULONG)b0);
	  --aptr_l;
	}
      ++i;
    }
  while ((r0 != 0) && (i <= num_sprimes));
  if (r0 == 0)
    {
      if ((digits_l(a_l) == 1) && (*lsdptr_l(a_l) == b0))
	b0 = 1;
    }
  else
    b0 = 0;

  PURGEVARS_L((5, sizeof(n_l), n_l, sizeof(b0), &b0,
	      sizeof(r0), &r0, sizeof(q0), &q0, sizeof(t), &t));
  ISPURGED_L((5, sizeof(n_l), n_l, sizeof(b0), &b0,
		    sizeof(r0), &r0, sizeof(q0), &q0, sizeof(t), &t));

  return b0;
}

int MRprime_l(BIGINT n_l, unsigned int numsprimes, unsigned int iter)
{
  BIGINT a_l, t_l, b_l;
  USHORT  k, p;
  unsigned int s, i, j;
  int prime = 1;

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
	  ++s;
	}
      p = 2;
      i = 1;
      do
	{
	  p = p + smallprimes[i];
	  ++i;
	  wexpmodmon_l(p, t_l, n_l, b_l);
	  if (!eq1_l(b_l))
	    {
	      j = 0;
	      while ((!eq1_l(b_l)) && (!equ_l(b_l, a_l)) && (j < s - 1))
		{
		  sqrmod_l(b_l, n_l, b_l);
		  ++j;
		}
	      if (!equ_l(b_l, a_l))
		prime = 0;
	    }
	}
      while ((i <= iter) && (prime));

      PURGEVARS_L((8, sizeof(a_l), a_l, sizeof(t_l), t_l,
		  sizeof(b_l), b_l, sizeof(k), &k,
		  sizeof(p), &p, sizeof(s), &s,
		  sizeof(i), &i, sizeof(j), &j));
      ISPURGED_L((8, sizeof(a_l), a_l, sizeof(t_l), t_l,
			sizeof(b_l), b_l, sizeof(k), &k,
			sizeof(p), &p, sizeof(s), &s,
			sizeof(i), &i, sizeof(j), &j));

      return prime;
    }
}

int SSprime_l(BIGINT n_l, unsigned int numsprimes, unsigned iter)
{
  BIGINT t_l, x_l, p_l, j_l;
  USHORT k, p;
  int j;
  unsigned int i;
  int prime = 1;

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
	  ++i;
	  wexpmodmon_l(p, t_l, n_l, x_l);
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

      PURGEVARS_L((8, sizeof(t_l), t_l, sizeof(x_l), x_l,
		  sizeof(p_l), p_l, sizeof(j_l), j_l,
		  sizeof(p), &p, sizeof(k), &k,
		  sizeof(i), &i, sizeof(j), &j));
      ISPURGED_L((8, sizeof(t_l), t_l, sizeof(x_l), x_l,
			sizeof(p_l), p_l, sizeof(j_l), j_l,
			sizeof(p), &p, sizeof(k), &k,
			sizeof(i), &i, sizeof(j), &j));

      return prime;
    }
}

/* Fonctions de génération aléatoire */

static bigint seed64[10];
#ifdef ARCH32
static bigint a64[] = { 4, 32557, 19605, 62509, 22609 };
#endif
#ifdef ARCH64
static bigint a64[] = { 2, 1284865837, 1481765933 };
#endif
static bigint buff64[10];

#ifdef ARCH32
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
  USHORT i;

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
#endif

#ifdef ARCH64
bigint *rand64_l(void)
{
  mul_l(seed64, a64, seed64);
  uadd_l(seed64, 1, seed64);
  if (digits_l(seed64) > 2)
    setdigits_l(seed64, 2);
  return seed64;
}

bigint *seed64_l(BIGINT seed_l)
{
  USHORT i;

  copy_l(buff64, seed64);
  if (digits_l(seed_l) > 2)
    {
      for (i = 0; i <= 2; i++)
	seed64[i] = seed_l[i];
    }
  else
    {
      for (i = 0; i <= digits_l(seed_l); i++)
	seed64[i] = seed_l[i];
    }
  return buff64;
}
#endif

bigint *ulseed64_l(ULONG seed)
{
  copy_l(buff64, seed64);
  setdigits_l(seed64, 2);
  *lsdptr_l(seed64) = (USHORT)(seed & BMIN1);
  *(lsdptr_l(seed64) + 1) = (USHORT)(seed >> BITPERDGT);
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

#ifdef ARCH32
ULONG ulrand64_l(void)
{
  ULONG rand = 0;
  unsigned int d;

  rand64_l();
  d = digits_l(seed64);
  if ((d > 1) && (d <= 4))
    {
      rand = seed64[d - 1];
      rand = rand + ((ULONG)seed64[d] << BITPERDGT);
    }
  else if (d == 1)
    rand = seed64[d];
  else if (d == 0)
    rand = 0;
  return rand;
}
#endif

#ifdef ARCH64
ULONG ulrand64_l(void)
{
  ULONG rand = 0;
  unsigned int d;

  rand64_l();
  d = digits_l(seed64);
  if (d == 2)
    {
      rand = seed64[d - 1];
      rand = rand + ((ULONG)seed64[d] << BITPERDGT);
    }
  else if (d == 1)
    rand = seed64[d];
  else if (d == 0)
    rand = 0;
  return rand;
}
#endif

void rand_l(BIGINT r_l, USHORT l)
{
  USHORT i, j, ls, lr;

  if (l > (USHORT)BIGINTMAXBIT)
    l = (USHORT)BIGINTMAXBIT;
  ls = (USHORT)(l >> LDBITPERDGT);
  lr = (USHORT)(l % BITPERDGT);
  for (i = 1; i <= ls; i++)
    r_l[i] = usrand64_l();
  if (lr > 0)
    {
      ++ls;
      r_l[ls] = usrand64_l();
      j = (USHORT)(1 << (lr - 1));
      r_l[ls] = (r_l[ls] | j) & ((j << 1) - 1);
    }
  else
    r_l[ls] = r_l[ls] | BDIV2;
  setdigits_l(r_l, ls);
}

static BIGINT XBBS;
static BIGINT MODBBS;
static char *MODBBSSTR = "86892f2c1fc36f565babcb113d03673b4d4e2c74c62574eb55250134b7838230166816cfc169814724f55fa0c71752e2046ff4cee53ce5306d2b3931524a37dae0980acb4e6fa58a1e7e45128b1f672b6f9f29ab50552e55058d18b30b5d560ad8f8321cb9995fce89bcf2595964ffa6cd56a68c73d26481aedc32fca45b5184c5d988ab8fd716a688f0a90d9b76f40b139078d45d81a5b391e506821642cc6df629382781ba5050bfe80df1ea30334acb60a8398efe6a6df8df58308bd977c2e7ca5e5c7395e19b642aaa4b42a2906b4e439b418da0bea79e6c196fae1801d708a0001704d0301368464040fdcf1ad6e5c674d1dcda3bf0d13c5ba53484ca7f";
/*static char *MODBBSSTR = "81aa5c97200fb2504c08b92e2f8b7e0805775c72b139b455846028653793ba9d616cd01cef7195b2f952e046ab62d063b048c8f62b21c7fc7fa4e71eef2588aa59b81f355b5539a471ee483b022bdab25b3fb41bc6224d60c17bbcb091294f76cb64c3b6da5504085a958b679d1f1147158189d4fa76ab721c535a3ecfe4452cc61b9466e315ba4b4392db04f686dbb61084b21c9540f972718fc96ed25a40a917a07c668048683ec116219621a301610bfba40e1ab11bd1e13aa8476aa6d37b261228df85c7df67043c51b37a250b27401aaf837101d2db1a55572dd7a79646ff6e5d20a24e4b43c6d8ab5e9a77becd76d5f0355252f4318e2066d3f9c42f25";*/

USHORT randbit_l(void)
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

  PURGEVARS_L((1, sizeof(g_l), g_l));
  ISPURGED_L((1, sizeof(g_l), g_l));

  return 0;
}

int ulseedBBS_l(ULONG seed)
{
  BIGINT seed_l;
  int s;

  setdigits_l(seed_l, 2);
  *lsdptr_l(seed_l) = (USHORT)(seed & BMIN1);
  *(lsdptr_l(seed_l) + 1) = (USHORT)(seed >> BITPERDGT);
  rmldzeros_l(seed_l);
  s = seedBBS_l(seed_l);

  PURGEVARS_L((1, sizeof(seed_l), seed_l));
  ISPURGED_L((1, sizeof(seed_l), seed_l));

  return s;
}

USHORT usrandBBS_l(void)
{
  USHORT i, r = 0;

  for (i = 0; i < (USHORT)(sizeof(USHORT) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

ULONG ulrandBBS_l(void)
{
  ULONG i, r = 0;

  for (i = 0; i < (ULONG)(sizeof(ULONG) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

UCHAR ucrandBBS_l(void)
{
  UCHAR i, r = 0;

  for (i = 0; i < (UCHAR)(sizeof(UCHAR) << 3); i++)
    r = (r << 1) + randbit_l();
  return r;
}

void randBBS_l(BIGINT r_l, USHORT l)
{
  USHORT i, j, ls, lr;

  if (l > (USHORT)BIGINTMAXBIT)
    l = (USHORT)BIGINTMAXBIT;
  ls = (USHORT)(l >> LDBITPERDGT);
  lr = (USHORT)(l % BITPERDGT);
  for (i = 1; i <= ls; i++)
    r_l[i] = usrandBBS_l();
  if (lr > 0)
    {
      ++ls;
      r_l[ls] = usrandBBS_l();
      j = (USHORT)(1 << (lr - 1));
      r_l[ls] = (r_l[ls] | j) & ((j << 1) - 1);
    }
  else
    r_l[ls] = (USHORT)(r_l[ls] | BDIV2);
  setdigits_l(r_l, ls);

  PURGEVARS_L((4, sizeof(i), &i, sizeof(j), &j,
	      sizeof(ls), &ls, sizeof(lr), &lr));
  ISPURGED_L((4, sizeof(i), &i, sizeof(j), &j,
	      sizeof(ls), &ls, sizeof(lr), &lr));
}

/* Fonctions de Montgomery */

void mulmon_l(BIGINT a_l, BIGINT b_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l)
{
  bigint t_l[1 + (BIGINTMAXDIGIT << 1)];
  bigint *tptr_l, *nptr_l, *tiptr_l, *lasttptr_l, *lastnptr_l;
  USHORT carry = 0;
  ULONG mtemp, t = 0L;
  USHORT mi, i;

  mul(a_l, b_l, t_l);
  lasttptr_l = t_l + digits_l(n_l);
  lastnptr_l = msdptr_l(n_l);
  for (i = (USHORT)(digits_l(t_l) + 1); i <= (digits_l(n_l) << 1); i++)
    t_l[i] = 0;
  if (digits_l(t_l) < (digits_l(n_l) << 1))
    setdigits_l(t_l, (digits_l(n_l) << 1));
  else
    setdigits_l(t_l, digits_l(t_l));
  tptr_l = lsdptr_l(t_l);
  while (tptr_l <= lasttptr_l)
    {
      carry = 0;
      mtemp = (ULONG)(*tptr_l) * (ULONG)np; /*ARCH64*/
      mi = (USHORT)(mtemp & BMIN1);
      nptr_l = lsdptr_l(n_l);
      tiptr_l = tptr_l;
      while (nptr_l <= lastnptr_l)
	{
	  t = (ULONG)mi * (ULONG)(*nptr_l) + (ULONG)(*tiptr_l) + (ULONG)carry; /*ARCH64*/
	  *tiptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++nptr_l;
	  ++tiptr_l;
	}
      while ((carry > 0) && (tiptr_l <= msdptr_l(t_l)))
	{
	  t = (ULONG)(*tiptr_l) + (ULONG)carry; /*ARCH64*/
	  *tiptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++tiptr_l;
	}
      if (carry > 0)
	{
	  *tiptr_l = carry;
	  incdigits_l(t_l);
	}
      ++tptr_l;
    }
  tptr_l = t_l + l;
  setdigits_l(tptr_l, digits_l(t_l) - l);
  copy_l(t_l, tptr_l);

  if (ge_l(t_l, n_l))
    sub(t_l, n_l, p_l);/*Il est possible que t_l soit BIGINTP*/
  else
    copy_l(p_l, t_l);

  PURGEVARS_L((4, sizeof(mi), &mi, sizeof(carry), &carry,
	      sizeof(mtemp), &mtemp, sizeof(t_l), t_l));
  ISPURGED_L((4, sizeof(mi), &mi, sizeof(carry), &carry,
		    sizeof(mtemp), &mtemp, sizeof(t_l), t_l));
}

void sqrmon_l(BIGINT a_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l)
{
  bigint t_l[1 + (BIGINTMAXDIGIT << 1)];
  bigint *tptr_l, *nptr_l, *tiptr_l, *lasttptr_l, *lastnptr_l;
  USHORT carry = 0L;
  ULONG mtemp, t = 0L;
  USHORT mi, i;

  sqr(a_l, t_l);
  lasttptr_l = t_l + digits_l(n_l);
  lastnptr_l = msdptr_l(n_l);
  for (i = (USHORT)(digits_l(t_l) + 1); i <= (digits_l(n_l) << 1); i++)
    t_l[i] = 0;
  if (digits_l(t_l) < (digits_l(n_l) << 1))
    setdigits_l(t_l, (digits_l(n_l) << 1));
  else
    setdigits_l(t_l, digits_l(t_l));
  tptr_l = lsdptr_l(t_l);
  while (tptr_l <= lasttptr_l)
    {
      carry = 0;
      mtemp = (ULONG)(*tptr_l) * (ULONG)np; /*ARCH64*/
      mi = (USHORT)(mtemp & BMIN1);
      nptr_l = lsdptr_l(n_l);
      tiptr_l = tptr_l;
      while (nptr_l <= lastnptr_l)
	{
	  t = (ULONG)mi * (ULONG)(*nptr_l) + (ULONG)(*tiptr_l) + (ULONG)carry; /*ARCH64*/
	  *tiptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++nptr_l;
	  ++tiptr_l;
	}
      while ((carry > 0) && (tiptr_l <= msdptr_l(t_l)))
	{
	  t = (ULONG)(*tiptr_l) + (ULONG)carry; /*ARCH64*/
	  *tiptr_l = (USHORT)(t & BMIN1);
	  carry = (USHORT)(t >> BITPERDGT);
	  ++tiptr_l;
	}
      if (carry > 0)
	{
	  *tiptr_l = carry;
	  incdigits_l(t_l);
	}
      ++tptr_l;
    }

  tptr_l = t_l + l;
  setdigits_l(tptr_l, digits_l(t_l) - l);
  copy_l(t_l, tptr_l);
  if (ge_l(t_l, n_l))
    sub(t_l, n_l, p_l);
  else
    copy_l(p_l, t_l);

  PURGEVARS_L((4, sizeof(mi), &mi, sizeof(carry), &carry,
	      sizeof(mtemp), &mtemp, sizeof(t_l), t_l));
  ISPURGED_L((4, sizeof(mi), &mi, sizeof(carry), &carry,
		    sizeof(mtemp), &mtemp, sizeof(t_l), t_l));
}

USHORT invmon_l(BIGINT n_l)
{
  unsigned int i;
  ULONG x = 2, y = 1;

  if (iseven_l(n_l))
    return (USHORT)BIGINT_MOD;
  for (i = 2; i <= BITPERDGT; i++)
    {
      if (x < (((ULONG)(*lsdptr_l(n_l)) * y) & ((x << 1) - 1)))
	y = y + x;
      x = (x << 1);
    }
  return (USHORT)(x - y);
}

int uexpmodmon_l(BIGINT aa_l, USHORT e, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, t_l, md_l;
  BIGINT un_l = { 1, 1 };
  BIGINTP r_l;
  USHORT mask = (USHORT)BDIV2, l, np;

  if (iseven_l(mo_l))
    return BIGINT_MOD;
  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
  copy_l(md_l, mo_l);
  rmldzeros_l(md_l);
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  if (digits_l(r_l) > (USHORT)BIGINTMAXDIGIT)
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

  PURGEVARS_L((6, sizeof(a_l), a_l, sizeof(t_l), t_l,
	      sizeof(r_l), r_l, sizeof(md_l), md_l,
	      sizeof(np), &np, sizeof(l), &l));
  ISPURGED_L((6, sizeof(a_l), a_l, sizeof(t_l), t_l,
		    sizeof(r_l), r_l, sizeof(md_l), md_l,
		    sizeof(np), &np, sizeof(l), &l));

  return BIGINT_OK;
}

int wexpmodmon_l(USHORT a, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, e_l, md_l;
  BIGINT un_l = { 1 , 1 };
  BIGINTP r_l;
  USHORT k, b, w, l, np;

 if (iseven_l(mo_l))
   return BIGINT_MOD;
  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
      mod_l(a_l, mo_l, p_l);
      return BIGINT_OK;
    }
  copy_l(e_l, ee_l);
  copy_l(md_l, mo_l);
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  np = invmon_l(md_l);
  mod_l(r_l, md_l, a_l);
  b = (USHORT)(1 << ((numbits_l(e_l) - 1) & (BITPERDGT - 1)));
  w = e_l[digits_l(e_l)];
  while (b > 0)
    {
      sqrmon_l(a_l, md_l, np, l, a_l);
      if ((w & b) > 0)
	umulmod_l(a_l, a, md_l, a_l);
      b = (b >> 1);
    }
  for (k = (USHORT)(digits_l(e_l) - 1); k > 0; k--)
    {
      w = e_l[k];
      b = (USHORT)BDIV2;
      while (b > 0)
	{
	  sqrmon_l(a_l, md_l, np, l, a_l);
	  if ((w & b) > 0)
	    umulmod_l(a_l, a, md_l, a_l);
	  b = (b >> 1);
	}
    }
  mulmon_l(a_l, un_l, md_l, np, l, p_l);

  PURGEVARS_L((9, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(r_l), r_l,
	      sizeof(md_l), md_l, sizeof(b), &b, sizeof(w), &w,
	      sizeof(np), &np, sizeof(l), &l, sizeof(k), &k));
  ISPURGED_L((9, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(r_l), r_l,
		    sizeof(md_l), md_l, sizeof(b), &b, sizeof(w), &w,
		    sizeof(np), &np, sizeof(l), &l, sizeof(k), &k));

  return BIGINT_OK;
}

int expmodmonk_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  BIGINT a_l, a2_l, md_l;
  BIGINTP e_l, r_l;
  BIGINTD temp_l;
  BIGINT un_l = { 1, 1 };
  bigint **pptr_l, *ptr_l = NULL;
  int i, f, tab;
  unsigned int k, lge, pow2k, mask, fk, s, d; /*ARCH64*/
  USHORT l, np, digit;

  if (iseven_l(mo_l))
    return BIGINT_MOD;
  if (eqz_l(mo_l))
    return BIGINT_DBZ;
  if (eq1_l(mo_l))
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
  copy_l(md_l, mo_l);
  rmldzeros_l(a_l);
  rmldzeros_l(e_l);
  rmldzeros_l(md_l);
  lge = numbits_l(e_l);
  k = 8;
  while (k > 1 && ((k - 1) * (k << ((k - 1) << 1))/((1 << k) - k - 1)) >= lge - 1)
    --k;
  pow2k = (1 << k);
  mask = pow2k - 1;
  pptr_l = (bigint **)malloc(sizeof(bigint *) * pow2k);
  if (pptr_l  == NULL)
    {

      PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l));
      ISPURGED_L((3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l));

      return BIGINT_MAL;
    }
  pptr_l[1] = a_l;
  setzero_l(r_l);
  l = digits_l(md_l);
  setbit_l(r_l, (l << LDBITPERDGT));
  if (digits_l(r_l) > (USHORT)BIGINTMAXDIGIT)
    mod_l(r_l, md_l, r_l);
  np = invmon_l(md_l);
  mulmod_l(a_l, r_l, md_l, a_l);
  if (k > 1)
    {
      ptr_l = (bigint *)malloc(sizeof(BIGINT) * ((pow2k >> 1) - 1));
      if (ptr_l == NULL)
	{
	  free(pptr_l);

	  PURGEVARS_L((3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l));
	  ISPURGED_L((3, sizeof(a_l), a_l, sizeof(e_l), e_l, sizeof(md_l), md_l));

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
  f = (int)((lge - 1)/k);
  fk = (unsigned int)f * k;
  s = (fk >> LDBITPERDGT);
  d = fk & (BITPERDGT - 1);
  digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & mask); /*ARCH64*/
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
      digit = (USHORT)(((((ULONG)e_l[s + 2] << BITPERDGT) + (ULONG)e_l[s + 1]) >> d) & mask); /*ARCH64*/
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

  PURGEVARS_L((17, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
	      sizeof(temp_l), temp_l, sizeof(e_l), e_l,
	      sizeof(md_l), md_l, sizeof(r_l), r_l,
	      sizeof(l), &l, sizeof(np), &np,
	      sizeof(tab), &tab, sizeof(k), &k,
	      sizeof(lge), &lge, sizeof(pow2k), &pow2k,
	      sizeof(mask), &mask, sizeof(fk), &fk,
	      sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit));
  ISPURGED_L((17, sizeof(a_l), a_l, sizeof(a2_l), a2_l,
		    sizeof(temp_l), temp_l, sizeof(e_l), e_l,
		    sizeof(md_l), md_l, sizeof(r_l), r_l,
		    sizeof(l), &l, sizeof(np), &np,
		    sizeof(tab), &tab, sizeof(k), &k,
		    sizeof(lge), &lge, sizeof(pow2k), &pow2k,
		    sizeof(mask), &mask, sizeof(fk), &fk,
		    sizeof(s), &s, sizeof(d), &d, sizeof(digit), &digit));

  return BIGINT_OK;
}

/* Fonction exponentielle optimisée */

int expmod_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l)
{
  if (iseven_l(mo_l))
    return expmodk_l(aa_l, ee_l, mo_l, p_l);
  else
    return expmodmonk_l(aa_l, ee_l, mo_l, p_l);
}

/* Fonctions de purge */

void purge_l(BIGINT n_l)
{
  if (n_l != NULL)
    {
#ifdef ARCH32
#ifdef SECURE
      guaranteed_memset(n_l, 0, BIGINTMAXBYTE + 2);
#else
      memset(n_l, 0, BIGINTMAXBYTE + 2);
#endif
#endif
#ifdef ARCH64
#ifdef SECURE
      guaranteed_memset(n_l, 0, BIGINTMAXBYTE + 4);
#else
      memset(n_l, 0, BIGINTMAXBYTE + 4);
#endif
#endif
    }
}

/* Fonction memset vérifiée */

#ifdef SECURE
static void *guaranteed_memset(void *v, int c, size_t n)
{
  volatile char *p = (volatile char *)v;
  
  while (n--)
    *p++ = c;

  return v;
}
#endif

/* Fonctions internes de purge des variables locales */

#ifdef SECURE
#include <stdarg.h>
#ifdef ARCH32
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
	  assert(size >= BIGINTMAXBYTE);
	  guaranteed_memset(va_arg(arg, char *), 0, size);
	}
    }
  va_end(arg);
}
#endif

#ifdef ARCH64
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
	  *va_arg(arg, int *) = 0;
	  break;
	case 8:
	  *va_arg(arg, long *) = 0;
	  break;
	default:
	  assert(size >= BIGINTMAXBYTE);
	  guaranteed_memset(va_arg(arg, char *), 0, size);
	}
    }
  va_end(arg);
}
#endif

#ifdef DEBUG_SECURE
static int ispurged_l(int numvars, ...)
{
  va_list arg;
  size_t size;
  char *cptr;

  va_start(arg, numvars);
  for (; numvars > 0; --numvars)
    {
      size = va_arg(arg, size_t);
      cptr = va_arg(arg, char *);
      for (; size > 0; size--)
	{
	  if (*cptr != 0)
	    return 0;
	  ++cptr;
	}
    }
  va_end(arg);
  return 1;
}
#endif
#endif

/* Fin de arithm.c */
