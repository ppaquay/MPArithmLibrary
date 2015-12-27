/***********************************************************************
 *                                                                     *
 *          Test  pour la librairie de fonctions d'arithmétique        *
 *        élémentaire pour les grands entiers (4096 et 8192 bits)      *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <math.h>

#include "libmparithm.h"

#define MAXTESTLEN BIGINTMAXBIT
#define RND (ulrand64_l() % (MAXTESTLEN + 1))
#define RNDDIGIT (ulrand64_l() % (BIGINTMAXDIGIT + 1))

static void verif_l(BIGINT a_l, BIGINT b_l, int line);
static void addcomm_l(unsigned int nbretest);
static void addass_l(unsigned int nbretest);
static void addsub_l(unsigned int nbretest);
static void addu_l(unsigned int nbretest);
static void addofl_l(void);
static void subneu_l(unsigned int nbretest);
static void subadd_l(unsigned int nbretest);
static void subass_l(unsigned int nbretest);
static void subu_l(unsigned int nbretest);
static void subufl_l(void);
static void mulclasscomm_l(unsigned int nbretest);
static void mulcomm_l(unsigned int nbretest);
static void mulkarcomm_l(unsigned int nbretest);
static void mulclassadd_l(unsigned int nbretest);
static void muladd_l(unsigned int nbretest);
static void mulkaradd_l(unsigned int nbretest);
static void mulclassass_l(unsigned int nbretest);
static void mulass_l(unsigned int nbretest);
static void mulkarass_l(unsigned int nbretest);
static void mulu_l(unsigned int nbretest);
static void mulclassdis_l(unsigned int nbretest);
static void muldis_l(unsigned int nbretest);
static void mulkardis_l(unsigned int nbretest);
static void mulclassofl_l(void);
static void mulofl_l(void);
static void mulkarofl_l(void);
static void sqrneu_l(void);
static void sqrmul_l(unsigned int nbretest);
static void sqrpot_l(unsigned int nbretest);
static void sqrbin_l(unsigned int nbretest);
static void sqrdist_l(unsigned int nbretest);
static void sqrofl_l(void);
static void sqrkarneu_l(void);
static void sqrkarmul_l(unsigned int nbretest);
static void sqrkarpot_l(unsigned int nbretest);
static void sqrkarbin_l(unsigned int nbretest);
static void sqrkardist_l(unsigned int nbretest);
static void sqrkarofl_l(void);
static void divneu_l(unsigned int nbretest);
static void divsmall_l(unsigned int nbretest);
static void divsub_l(unsigned int nbretest);
static void divs5_l(void);
static void divrem_l(unsigned int nbretest);
static void divmod_l(unsigned int nbretest);
static void addmodcom_l(unsigned int nbretest);
static void addmodass_l(unsigned int nbrestest);
static void addmodsub_l(unsigned int nbretest);
static void addmodu_l(unsigned int nbretest);
static void submodneu_l(unsigned int nbretest);
static void submodadd_l(unsigned int nbretest);
static void submodass_l(unsigned int nbretest);
static void submodu_l(unsigned int nbretest);
static void mulmodcom_l(unsigned int nbretest);
static void mulmodadd_l(unsigned int nbretest);
static void mulmodass_l(unsigned int nbretest);
static void mulmodu_l(unsigned int nbretest);
static void mulmoddis_l(unsigned int nbretest);
static void sqrmodneu_l(unsigned int nbretest);
static void sqrmodmul_l(unsigned int nbretest);
static void sqrmodpot_l(unsigned int nbretest);
static void sqrmodbin_l(unsigned int nbretest);
static void sqrmoddis_l(unsigned int nbretest);
static void expmodtriv_l(void);
static void expmodmontriv_l(void);
static void expmodsqr_l(unsigned int nbretest);
static void expmodmonsqr_l(unsigned int nbretest);
static void expmodprop_l(unsigned int nbretest);
static void expmodmonprop_l(unsigned int nbretest);
static void expprop_l(unsigned int nbretest);
static void expmodprop2_l(unsigned int nbretest);
static void expmodmonprop2_l(unsigned int nbretest);
static void expprop2_l(unsigned int nbretest);
static void expmodacc_l(void);
static void expmodmonacc_l(void);
static void expacc_l(void);

/*void write_bigint(BIGINT a_l)
{
  bigint *aptr_l = msdptr_l(a_l);
  int i;

  printf("Nombre = ");
  for (i = digits_l(a_l); i > 0; i--)
    {
      printf("%ld, ", *aptr_l);
      --aptr_l;
    }
  printf("\n");
}*/

static void verif_l(BIGINT a_l, BIGINT b_l, int line)
{
  if (check_l(a_l))
    {
      printf("Erreur dans check_l() dans la ligne %d\n", line);
      printf("check_l() = %d\n", check_l(a_l));
      printf("a_l = %s\n", bigint2str_l(a_l, 10, 0));
      abort();
    }
  if (equ_l(a_l, b_l) == 0)
    {
      printf("Erreur dans la ligne %d\n", line);
      /*write_bigint(a_l);
      write_bigint(b_l);*/
      printf("a_l = %s\n", bigint2str_l(a_l, 10, 0));
      printf("b_l = %s\n", bigint2str_l(b_l, 10, 0));
      abort();
    }
}

static void addcomm_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, s1_l, s2_l;
  BIGINT nul_l = { 0 };
  int i;

  printf("Fonction addcom_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  add_l(a_l, b_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  setmax_l(b_l);
  add_l(a_l, b_l, s1_l);
  add_l(b_l, a_l, s2_l);
  verif_l(s1_l, b_l, __LINE__);
  verif_l(s2_l, b_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      add_l(b_l, a_l, s2_l);
      verif_l(s1_l, b_l, __LINE__);
      verif_l(s2_l, b_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      add_l(a_l, b_l, s1_l);
      add_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, a_l);
      add_l(a_l, b_l, a_l);
      add_l(d_l, b_l, c_l);
      verif_l(a_l, c_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, b_l);
      add_l(a_l, b_l, b_l);
      add_l(a_l, d_l, c_l);
      verif_l(b_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      add_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void addass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction addass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      add_l(a_l, b_l, s1_l);
      add_l(s1_l, c_l, s2_l);

      add_l(b_l, c_l, s3_l);
      add_l(a_l, s3_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void addsub_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l;
  int i;

  printf("Fonction addsub_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      sub_l(s1_l, a_l, s2_l);
      sub_l(s1_l, b_l, s3_l);
      verif_l(s2_l, b_l, __LINE__);
      verif_l(s3_l, a_l, __LINE__);
    }
}

static void addu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, u_l;
  USHORT u;
  int i;

  printf("Fonction addu_l()...\n");

  rand_l(a_l, RND);
  u = 0;
  uadd_l(a_l, u, s1_l);
  verif_l(s1_l, a_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      uadd_l(a_l, u, s1_l);
      add_l(b_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      uadd_l(a_l, u, s1_l);
      add_l(a_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void addofl_l(void)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 }, nul_l = { 0 };

  printf("Fonction addofl_l()...\n");
  
  setmax_l(a_l);
  sub_l(a_l, un_l, b_l);
  if (add_l(a_l, un_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur dans add_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, nul_l) == 0)
    {
      printf("Erreur dans add_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s1_l = %s\n", bigint2str_l(s1_l, 10, 0));
      abort();
    }
  if (add_l(a_l, a_l, s2_l) != BIGINT_OFL)
    {
      printf("Erreur dans add_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s2_l, b_l) == 0)
    {
      printf("Erreur dans add_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
}

static void subneu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l;
  BIGINT nul_l = {0 }, un_l = { 1, 1 };
  int i;

  printf("Fonction subneu_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  sub_l(a_l, b_l, s1_l);
  sub_l(b_l, a_l, s2_l);
  verif_l(s1_l, nul_l, __LINE__);
  verif_l(s2_l, nul_l, __LINE__);

  setzero_l(a_l);
  sub_l(a_l, un_l, s1_l);
  sub_l(b_l, s1_l, s3_l);
  verif_l(s3_l, un_l, __LINE__);

  setzero_l(b_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      sub_l(a_l, b_l, s1_l);
      verif_l(s1_l, a_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      sub_l(a_l, a_l, a_l);
      verif_l(a_l, nul_l, __LINE__);
      
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(c_l, a_l);
      sub_l(a_l, b_l, a_l);
      sub_l(c_l, b_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(c_l, b_l);
      sub_l(a_l, b_l, b_l);
      sub_l(a_l, c_l, s1_l);
      verif_l(b_l, s1_l, __LINE__);
    }
}

static void subadd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l;
  int i;

  printf("Fonction subadd_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      if (ge_l(a_l, b_l))
	{
	  sub_l(a_l, b_l, s1_l);
	  add_l(s1_l, b_l, s1_l);
	  verif_l(s1_l, a_l, __LINE__);
	}
    }
}

static void subass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction subass_l()...\n");

 for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      add_l(b_l, c_l, s1_l);
      add_l(s1_l, a_l, s2_l);
      sub_l(s2_l, c_l, s3_l);
      sub_l(s3_l, b_l, s4_l);
      sub_l(s2_l, s1_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);
    }
}

static void subu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, u_l;
  USHORT u;
  int i, k;

  printf("Fonction subu_l()...\n");

  rand_l(a_l, RND);
  u = 0;
  usub_l(a_l, u, s1_l);
  verif_l(s1_l, a_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      u = usrand64_l();
      u2bigint_l(u_l, u);
      k = RND;
      if ((numbits_l(u_l) + 1) > k)
	rand_l(a_l, numbits_l(u_l) + 1);
      else
	rand_l(a_l, k);
      copy_l(b_l, a_l);
      usub_l(a_l, u, s1_l);
      sub_l(b_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      usub_l(a_l, u, s1_l);
      sub_l(a_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void subufl_l(void)
{
  BIGINT a_l, s1_l, s2_l, s3_l;
  BIGINT un_l = { 1, 1 }, nul_l = { 0 }, deux_l = { 1, 2 };

  printf("Fonction subufl_l()...\n");
  
  setmax_l(a_l);
  if (sub_l(nul_l, un_l, s1_l) != BIGINT_UFL)
    {
      printf("Erreur dans sub_l: UFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(a_l, s1_l) == 0)
    {
      printf("Erreur dans sub_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s1_l = %s\n", bigint2str_l(s1_l, 10, 0));
      abort();
    }
  if (sub_l(un_l, deux_l, s2_l) != BIGINT_UFL)
    {
      printf("Erreur dans sub_l: UFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s2_l, a_l) == 0)
    {
      printf("Erreur dans sub_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
  if (sub_l(nul_l, a_l, s3_l) != BIGINT_UFL)
    {
      printf("Erreur dans sub_l: UFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s3_l, un_l) == 0)
    {
      printf("Erreur dans sub_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
}

static void mulclasscomm_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, s1_l, s2_l;
  BIGINT nul_l = { 0 }, un_l = { 1, 1 };
  int i;

  printf("Fonction mulclasscomm_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  mulclass_l(a_l, b_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mulclass_l(a_l, b_l, s1_l);
      mulclass_l(b_l, a_l, s2_l);
      verif_l(s1_l, nul_l, __LINE__);
      verif_l(s2_l, nul_l, __LINE__);
    }

  setone_l(a_l);
  setone_l(b_l);
  mulclass_l(a_l, b_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);

  setone_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mulclass_l(a_l, b_l, s1_l);
      mulclass_l(b_l, a_l, s2_l);
      verif_l(s1_l, b_l, __LINE__);
      verif_l(s2_l, b_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      mulclass_l(a_l, b_l, s1_l);
      mulclass_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, a_l);
      mulclass_l(a_l, b_l, a_l);
      mulclass_l(d_l, b_l, c_l);
      verif_l(a_l, c_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, b_l);
      mulclass_l(a_l, b_l, b_l);
      mulclass_l(a_l, d_l, c_l);
      verif_l(b_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      mulclass_l(a_l, b_l, s1_l);
      mulclass_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulcomm_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, s1_l, s2_l;
  BIGINT nul_l = { 0 }, un_l = { 1, 1 };
  int i;

  printf("Fonction mulcomm_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  mul_l(a_l, b_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mul_l(a_l, b_l, s1_l);
      mul_l(b_l, a_l, s2_l);
      verif_l(s1_l, nul_l, __LINE__);
      verif_l(s2_l, nul_l, __LINE__);
    }

  setone_l(a_l);
  setone_l(b_l);
  mul_l(a_l, b_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);

  setone_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mul_l(a_l, b_l, s1_l);
      mul_l(b_l, a_l, s2_l);
      verif_l(s1_l, b_l, __LINE__);
      verif_l(s2_l, b_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      mul_l(a_l, b_l, s1_l);
      mul_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, a_l);
      mul_l(a_l, b_l, a_l);
      mul_l(d_l, b_l, c_l);
      verif_l(a_l, c_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, b_l);
      mul_l(a_l, b_l, b_l);
      mul_l(a_l, d_l, c_l);
      verif_l(b_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      mul_l(a_l, b_l, s1_l);
      mul_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulkarcomm_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, s1_l, s2_l;
  BIGINT nul_l = { 0 }, un_l = { 1, 1 };
  int i;

  printf("Fonction mulkarcomm_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  mulkar_l(a_l, b_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mulkar_l(a_l, b_l, s1_l);
      mulkar_l(b_l, a_l, s2_l);
      verif_l(s1_l, nul_l, __LINE__);
      verif_l(s2_l, nul_l, __LINE__);
    }

  setone_l(a_l);
  setone_l(b_l);
  mulkar_l(a_l, b_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);

  setone_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      mulkar_l(a_l, b_l, s1_l);
      mulkar_l(b_l, a_l, s2_l);
      verif_l(s1_l, b_l, __LINE__);
      verif_l(s2_l, b_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      mulkar_l(a_l, b_l, s1_l);
      mulkar_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, a_l);
      mulkar_l(a_l, b_l, a_l);
      mulkar_l(d_l, b_l, c_l);
      verif_l(a_l, c_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(d_l, b_l);
      mulkar_l(a_l, b_l, b_l);
      mulkar_l(a_l, d_l, c_l);
      verif_l(b_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      mulkar_l(a_l, b_l, s1_l);
      mulkar_l(b_l, a_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulclassadd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l;
  int i, j, n;

  printf("Fonction mulclassadd_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	n = usrand64_l() % 1001;
      while (n == 0);
      u2bigint_l(b_l, n);
      mulclass_l(a_l, b_l, s1_l);
      copy_l(b_l, a_l);
      for (j = 1; j < n; j++)
	add_l(a_l, b_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void muladd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l;
  int i, j, n;

  printf("Fonction muladd_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	n = usrand64_l() % 1001;
      while (n == 0);
      u2bigint_l(b_l, n);
      mul_l(a_l, b_l, s1_l);
      copy_l(b_l, a_l);
      for (j = 1; j < n; j++)
	add_l(a_l, b_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void mulkaradd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l;
  int i, j, n;

  printf("Fonction mulkaradd_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	n = usrand64_l() % 1001;
      while (n == 0);
      u2bigint_l(b_l, n);
      mulkar_l(a_l, b_l, s1_l);
      copy_l(b_l, a_l);
      for (j = 1; j < n; j++)
	add_l(a_l, b_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void mulclassass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction mulclassass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      mulclass_l(a_l, b_l, s1_l);
      mulclass_l(s1_l, c_l, s2_l);

      mulclass_l(b_l, c_l, s3_l);
      mulclass_l(a_l, s3_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void mulass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction mulass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      mul_l(a_l, b_l, s1_l);
      mul_l(s1_l, c_l, s2_l);

      mul_l(b_l, c_l, s3_l);
      mul_l(a_l, s3_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void mulkarass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction mulkarass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      mulkar_l(a_l, b_l, s1_l);
      mulkar_l(s1_l, c_l, s2_l);

      mulkar_l(b_l, c_l, s3_l);
      mulkar_l(a_l, s3_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void mulu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, u_l;
  BIGINT nul_l = { 0 };
  USHORT u;
  int i;

  printf("Fonction mulu_l()...\n");

  rand_l(a_l, RND);
  u = 0;
  umul_l(a_l, u, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      umul_l(a_l, u, s1_l);
      mul_l(b_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      umul_l(a_l, u, s1_l);
      mul_l(a_l, u_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulclassdis_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction mulclassdis_l()...\n");
  
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      add_l(a_l, b_l, s1_l);
      mulclass_l(s1_l, c_l, s2_l);

      mulclass_l(a_l, c_l, s3_l);
      mulclass_l(b_l, c_l, s4_l);
      add_l(s3_l, s4_l, s5_l);
      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void muldis_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction muldis_l()...\n");
  
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      add_l(a_l, b_l, s1_l);
      mul_l(s1_l, c_l, s2_l);

      mul_l(a_l, c_l, s3_l);
      mul_l(b_l, c_l, s4_l);
      add_l(s3_l, s4_l, s5_l);
      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void mulkardis_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction mulkardis_l()...\n");
  
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      add_l(a_l, b_l, s1_l);
      mulkar_l(s1_l, c_l, s2_l);

      mulkar_l(a_l, c_l, s3_l);
      mulkar_l(b_l, c_l, s4_l);
      add_l(s3_l, s4_l, s5_l);
      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void mulclassofl_l(void)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 }, deux_l = { 1, 2 };

  printf("Fonction mulclassofl_l()...\n");
  
  setmax_l(a_l);
  sub_l(a_l, un_l, b_l);
  if (mulclass_l(a_l, deux_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur dans mul_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, b_l) == 0)
    {
      printf("Erreur dans mul_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s1_l = %s\n", bigint2str_l(s1_l, 10, 0));
      abort();
    }
  if (mulclass_l(a_l, a_l, s2_l) != BIGINT_OFL)
    {
      printf("Erreur dans mul_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s2_l, un_l) == 0)
    {
      printf("Erreur dans mul_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
}

static void mulofl_l(void)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 }, deux_l = { 1, 2 };

  printf("Fonction mulofl_l()...\n");
  
  setmax_l(a_l);
  sub_l(a_l, un_l, b_l);
  if (mul_l(a_l, deux_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur dans mul_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, b_l) == 0)
    {
      printf("Erreur dans mul_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s1_l = %s\n", bigint2str_l(s1_l, 10, 0));
      abort();
    }
  if (mul_l(a_l, a_l, s2_l) != BIGINT_OFL)
    {
      printf("Erreur dans mul_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s2_l, un_l) == 0)
    {
      printf("Erreur dans mul_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
}

static void mulkarofl_l(void)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 }, deux_l = { 1, 2 };

  printf("Fonction mulkarofl_l()...\n");
  
  setmax_l(a_l);
  sub_l(a_l, un_l, b_l);
  if (mulkar_l(a_l, deux_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur dans mulkar_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, b_l) == 0)
    {
      printf("Erreur dans mulkar_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s1_l = %s\n", bigint2str_l(s1_l, 10, 0));
      abort();
    }
  if (mulkar_l(a_l, a_l, s2_l) != BIGINT_OFL)
    {
      printf("Erreur dans mulkar_l: OFL non détecté dans la ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s2_l, un_l) == 0)
    {
      printf("Erreur dans mulkar_l: réduction mod 2^BIGINTMAXBIT dans la ligne %d\n", __LINE__);
      printf("s2_l = %s\n", bigint2str_l(s2_l, 10, 0));
      abort();
    }
}

static void sqrclassneu_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 };

  printf("Fonction sqrclassneu_l()...\n");

  setzero_l(a_l);
  sqrclass_l(a_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setone_l(a_l);
  sqrclass_l(a_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
}

static void sqrneu_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 };

  printf("Fonction sqrneu_l()...\n");

  setzero_l(a_l);
  sqr_l(a_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setone_l(a_l);
  sqr_l(a_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
}

static void sqrkarneu_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 };

  printf("Fonction sqrkarneu_l()...\n");

  setzero_l(a_l);
  sqrkar_l(a_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setone_l(a_l);
  sqrkar_l(a_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
}

static void sqrclassmul_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  int i;

  printf("Fonction sqrclassmul_l()...\n");

  setmax_l(a_l);
  sqrclass_l(a_l, s1_l);
  mulclass_l(a_l, a_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      sqrclass_l(a_l, a_l);
      mulclass_l(b_l, b_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void sqrmul_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  int i;

  printf("Fonction sqrmul_l()...\n");

  setmax_l(a_l);
  sqr_l(a_l, s1_l);
  mulclass_l(a_l, a_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      sqr_l(a_l, a_l);
      mulclass_l(b_l, b_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void sqrkarmul_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l;
  int i;

  printf("Fonction sqrkarmul_l()...\n");

  setmax_l(a_l);
  sqrkar_l(a_l, s1_l);
  mulclass_l(a_l, a_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      sqrkar_l(a_l, a_l);
      mulclass_l(b_l, b_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void sqrclasspot_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction sqrclasspot_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      sqrclass_l(a_l, s1_l);
      sqrclass_l(b_l, s2_l);
      mulclass_l(s1_l, s2_l, s3_l);
      mulclass_l(a_l, b_l, s4_l);
      sqrclass_l(s4_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);
    }
}

static void sqrpot_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction sqrpot_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      sqr_l(a_l, s1_l);
      sqr_l(b_l, s2_l);
      mul_l(s1_l, s2_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      sqr_l(s4_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);
    }
}

static void sqrkarpot_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction sqrkarpot_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      sqrkar_l(a_l, s1_l);
      sqrkar_l(b_l, s2_l);
      mul_l(s1_l, s2_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      sqrkar_l(s4_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);
    }
}

static void sqrclassbin_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  int i;

  printf("Fonction sqrclassbin_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      sqrclass_l(s1_l, s1_l);

      sqrclass_l(a_l, s2_l);
      sqrclass_l(b_l, s3_l);
      mulclass_l(a_l, b_l, s4_l);
      mulclass_l(s4_l, deux_l, s4_l);
      add_l(s2_l, s3_l, s2_l);
      add_l(s2_l, s4_l, s2_l);

      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void sqrbin_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  int i;

  printf("Fonction sqrbin_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      sqr_l(s1_l, s1_l);

      sqr_l(a_l, s2_l);
      sqr_l(b_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      mul_l(s4_l, deux_l, s4_l);
      add_l(s2_l, s3_l, s2_l);
      add_l(s2_l, s4_l, s2_l);

      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void sqrkarbin_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  int i;

  printf("Fonction sqrkarbin_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      sqrkar_l(s1_l, s1_l);

      sqrkar_l(a_l, s2_l);
      sqrkar_l(b_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      mul_l(s4_l, deux_l, s4_l);
      add_l(s2_l, s3_l, s2_l);
      add_l(s2_l, s4_l, s2_l);

      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void sqrclassdist_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction sqrclassdist_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      mulclass_l(a_l, s1_l, s2_l);

      sqrclass_l(a_l, s3_l);
      mulclass_l(a_l, b_l, s4_l);
      add_l(s3_l, s4_l, s5_l);

      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void sqrdist_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction sqrdist_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      mul_l(a_l, s1_l, s2_l);

      sqr_l(a_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      add_l(s3_l, s4_l, s5_l);

      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void sqrkardist_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction sqrkardist_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      add_l(a_l, b_l, s1_l);
      mul_l(a_l, s1_l, s2_l);

      sqrkar_l(a_l, s3_l);
      mul_l(a_l, b_l, s4_l);
      add_l(s3_l, s4_l, s5_l);

      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void sqrclassofl_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT un_l = { 1 , 1 };

  printf("Fonction sqrclassofl_l()...\n");

  setmax_l(a_l);
  if (sqrclass_l(a_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur: OFL non détecté, ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, un_l) == 0)
    {
      printf("Erreur dans la réduction mod 2^4096, ligne %d\n", __LINE__);
      abort();
    }
}

static void sqrofl_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT un_l = { 1 , 1 };

  printf("Fonction sqrofl_l()...\n");

  setmax_l(a_l);
  if (sqr_l(a_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur: OFL non détecté, ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, un_l) == 0)
    {
      printf("Erreur dans la réduction mod 2^4096, ligne %d\n", __LINE__);
      abort();
    }
}

static void sqrkarofl_l(void)
{
  BIGINT a_l, s1_l;
  BIGINT un_l = { 1 , 1 };

  printf("Fonction sqrkarofl_l()...\n");

  setmax_l(a_l);
  if (sqrkar_l(a_l, s1_l) != BIGINT_OFL)
    {
      printf("Erreur: OFL non détecté, ligne %d\n", __LINE__);
      abort();
    }
  if (equ_l(s1_l, un_l) == 0)
    {
      printf("Erreur dans la réduction mod 2^4096, ligne %d\n", __LINE__);
      abort();
    }
}

static void divneu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, r_l, s1_l;
  BIGINT nul_l = { 0 };
  int i;

  printf("Fonction divneu_l()...\n");

  rand_l(a_l, RND);
  setzero_l(b_l);
  if (div_l(a_l, b_l, s1_l, r_l) == 0)
    {
      printf("Erreur div par 0 non détectée, ligne %d\n", __LINE__);
      abort();
    }

  setzero_l(a_l);
  do
    rand_l(b_l, RND);
  while (eqz_l(b_l));
  div_l(a_l, b_l, s1_l, r_l);
  verif_l(r_l, nul_l, __LINE__);
  verif_l(s1_l, nul_l, __LINE__);

  setone_l(b_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      div_l(a_l, b_l, s1_l, r_l);
      verif_l(s1_l, a_l, __LINE__);
      verif_l(r_l, nul_l, __LINE__);
    }
}

static void divsmall_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, r_l, r1_l, s1_l, s2_l, s3_l, s4_l;
  int i, n;

  printf("Fonction divsmall_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	u2bigint_l(b_l, usrand64_l());
      while (eqz_l(b_l));
      div_l(a_l, b_l, s1_l, r_l);
      mul_l(b_l, s1_l, s2_l);
      add_l(s2_l, r_l, s3_l);
      verif_l(s3_l, a_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      copy_l(c_l, a_l);
      do
	n = usrand64_l();
      while (n == 0);
      u2bigint_l(d_l, n);
      udiv_l(a_l, n, a_l, s1_l);
      mul_l(d_l, a_l, s2_l);
      add_l(s2_l, s1_l, s2_l);
      verif_l(s2_l, c_l, __LINE__);

      udiv_l(b_l, n, s1_l, b_l);
      mul_l(d_l, s1_l, s1_l);
      add_l(s1_l, b_l, s2_l);
      verif_l(s2_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	u2bigint_l(b_l, usrand64_l());
      while (eqz_l(b_l));
      do
	u2bigint_l(c_l, usrand64_l());
      while (eqz_l(c_l));
      div_l(a_l, b_l, s1_l, r_l);
      div_l(b_l, c_l, s2_l, r1_l);
      mul_l(c_l, s2_l, s3_l);
      mul_l(s3_l, s1_l, s3_l);
      mul_l(r1_l, s1_l, s4_l);
      add_l(s3_l, s4_l, s4_l);
      add_l(s4_l, r_l, s4_l);
      verif_l(s4_l, a_l, __LINE__);
    }
}

static void divsub_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, r_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 };
  int i;

  printf("Fonction divsub_l()...\n");   

  for (i = 1; i <= nbretest; i++)
    {
      do
	{
	  do
	    rand_l(a_l, RND);
	  while (eqz_l(a_l));
	  rand_l(b_l, (digits_l(a_l) * BITPERDGT) - (ucrand64_l() % 11));
	}
      while (eqz_l(b_l));

      /*write_bigint(a_l);
      write_bigint(b_l);*/

      div_l(a_l, b_l, s1_l, r_l);
      setzero_l(s2_l);
      while (ge_l(a_l, b_l))
	{
	  sub_l(a_l, b_l, a_l);
	  add_l(s2_l, un_l, s2_l);
	}
      verif_l(s1_l, s2_l, __LINE__);
      verif_l(r_l, a_l, __LINE__);
    }
}

static void divs5_l(void)
{
  BIGINT a_l, b_l, c_l, d_l, r_l, s1_l;

  printf("Fonction divs5_l()...\n");   

  str2bigint_l(a_l, "e37d3abc904baba7a2ac4b6d8f782b2bf84919d2917347690d9e93dcdd2b91cee9983c564cf1312206c91e74d80ba479064c8f42bd70aaaa689f80d435afc997ce853b465703c8edca", 16);
  str2bigint_l(b_l, "80b0987b72c1667c30c9156a6674c2e73e61a1fd527d4e78b3f1505603c566658459b83ccfd587ba9b5fcbdc0ad09152e0ac265", 16);
  str2bigint_l(c_l, "1c48a1c798541ae0b9eb2c6327b1fffff4fe5c0e2723", 16);
  str2bigint_l(d_l, "ca2312fbb3f4c23add7655e94c3410b15c6064bd48a4e5fcc33ddf553e7cb829bf66fbfd61b4667f5ed6b387ec47c5272cf6fb", 16);
  div_l(a_l, b_l, s1_l, r_l);
  verif_l(s1_l, c_l, __LINE__);
  verif_l(r_l, d_l, __LINE__);
  
  str2bigint_l(a_l, "1f1bf7045d135fbc248134dcb1bb953a548d16e3578b7300", 16);
  str2bigint_l(b_l, "07c6fdc11744d7ef09204d372c6ee54ed37552c7978d7131", 16);
  str2bigint_l(c_l, "7c6fdc11744d7ef09204d372c6ee54dda2d1e8c90e31f6d", 16);
  str2bigint_l(d_l, "3", 16);
  div_l(a_l, b_l, s1_l, r_l);
  verif_l(s1_l, d_l, __LINE__);
  verif_l(r_l, c_l, __LINE__);
}

static void divrem_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, r_l, r1_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction divrem_l()...\n");

  setmax_l(a_l);
  rand_l(b_l, BIGINTMAXBIT);
  div_l(a_l, b_l, s1_l, r_l);
  mul_l(b_l, s1_l, s2_l);
  add_l(s2_l, r_l, s3_l);
  verif_l(s3_l, a_l, __LINE__);

  rand_l(d_l, BIGINTMAXBIT << 1);
  rand_l(b_l, BIGINTMAXBIT);
  div_l(d_l, b_l, s1_l, r_l);
  mul_l(b_l, s1_l, s2_l);
  add_l(s2_l, r_l, s3_l);
  verif_l(s3_l, d_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(c_l, a_l);
      do
	rand_l(b_l, RND);
      while (eqz_l(b_l));
      copy_l(d_l, b_l);
      div_l(a_l, b_l, a_l, b_l);
      mul_l(d_l, a_l, s1_l);
      add_l(s1_l, b_l, s2_l);
      verif_l(s2_l, c_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(b_l, RND);
      while (eqz_l(b_l));
      div_l(a_l, b_l, s1_l, r_l);
      mul_l(b_l, s1_l, s2_l);
      add_l(s2_l, r_l, s3_l);
      verif_l(s3_l, a_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
       rand_l(a_l, RND);
      do
	rand_l(b_l, RND);
      while (eqz_l(b_l));
      do
	rand_l(c_l, RND);
      while (eqz_l(c_l));
      div_l(a_l, b_l, s1_l, r_l);
      div_l(b_l, c_l, s2_l, r1_l);

      mul_l(c_l, s2_l, s3_l);
      mul_l(s3_l, s1_l, s3_l);
      mul_l(r1_l, s1_l, s4_l);
      add_l(s3_l, s4_l, s4_l);
      add_l(s4_l, r_l, s4_l);
      verif_l(s4_l, a_l, __LINE__);
    }
}

static void divmod_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, r_l, s1_l, s2_l;
  BIGINT un_l = { 1, 1 }, nul_l = { 0 }, deux_l = { 1, 2 };
  int i, j;
  USHORT n;
  unsigned long k;

  printf("Fonction divmod_l()...\n");

  if (mod_l(un_l, nul_l, s1_l) == 0)
    {
      printf("Erreur div par 0 non détecté, ligne %d\n", __LINE__);
      abort();
    }

  rand_l(a_l, RND);
  mod_l(a_l, un_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(b_l, RND);
      while (eqz_l(b_l));
      div_l(a_l, b_l, s1_l, r_l);
      mod_l(a_l, b_l, s2_l);
      verif_l(r_l, s2_l, __LINE__);
    }

  mod2_l(nul_l, 0, s1_l);
  mod_l(nul_l, un_l, s2_l);
  verif_l(s1_l, nul_l, __LINE__);
  verif_l(s2_l, nul_l, __LINE__);

  k = 0;
  rand_l(a_l, RND);
  copy_l(b_l, a_l);
  copy_l(c_l, un_l);
  mod2_l(a_l, k, a_l);
  mod_l(b_l, un_l, s1_l);
  verif_l(a_l, s1_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      k = ulrand64_l() % MAXTESTLEN;
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      copy_l(c_l, un_l);
      for (j = 1; j <= k; j++)
	mul_l(c_l, deux_l, c_l);
      mod2_l(a_l, k, a_l);
      mod_l(b_l, c_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  if (umod_l(un_l, 0) != BMIN1)
    {
      printf("Erreur: réduction par 0 non détectée, ligne %d\n", __LINE__);
      abort();
    }
  if (umod_l(deux_l, 1) != 0)
    {
      printf("Erreur: réduction par 1 fausse, ligne %d\n", __LINE__);
      abort();
    }
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      n = usrand64_l();
      u2bigint_l(b_l, n);
      u2bigint_l(c_l, umod_l(a_l, n));
      if (eqmod_l(a_l, c_l, b_l) == 0)
	{
	  printf("Erreur dans umod_l, ligne %d\n", __LINE__);
	  abort();
	}
    }
}

static void addmodcom_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l;
  BIGINT nul_l = { 0 };
  int i;

  printf("Fonction addmodcomm_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  if (addmod_l(a_l, b_l, a_l, s1_l) == 0)
    {
      printf("Erreur div par 0 non détecté, ligne %d\n", __LINE__);
      abort();
    }

  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  addmod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  setmax_l(b_l);
  addmod_l(a_l, b_l, m_l, s1_l);
  addmod_l(b_l, a_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      addmod_l(b_l, a_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setone_l(m_l);
  addmod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      copy_l(c_l, a_l);
      rand_l(d_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(b_l, d_l, m_l, b_l);
      addmod_l(d_l, a_l, m_l, a_l);
      verif_l(b_l, a_l, __LINE__);
      addmod_l(c_l, d_l, m_l, m_l);
      verif_l(b_l, m_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      addmod_l(b_l, a_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void addmodass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction addmodass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      addmod_l(s1_l, c_l, m_l, s2_l);

      addmod_l(b_l, c_l, m_l, s3_l);
      addmod_l(a_l, s3_l, m_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void addmodsub_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l;
  int i;

  printf("Fonction addmodsub_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  addmod_l(a_l, b_l, m_l, s1_l);
  submod_l(s1_l, b_l, m_l, s2_l);
  submod_l(s1_l, a_l, m_l, s3_l);
  mod_l(a_l, m_l, a_l);
  mod_l(b_l, m_l, b_l);
  verif_l(a_l, s2_l, __LINE__);
  verif_l(b_l, s3_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      submod_l(s1_l, b_l, m_l, s2_l);
      submod_l(s1_l, a_l, m_l, s3_l);
      mod_l(a_l, m_l, a_l);
      mod_l(b_l, m_l, b_l);
      verif_l(a_l, s2_l, __LINE__);
      verif_l(b_l, s3_l, __LINE__);
    }
}

static void addmodu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, u_l;
  BIGINT nul_l = { 0 };
  USHORT u;
  int i;

  printf("Fonction addmodu_l()...\n");

  rand_l(a_l, RND);
  if (uaddmod_l(a_l, 2, nul_l, a_l) == 0)
    {
      printf("Erreur dans uaddmod_l(): div par 0, ligne %d\n", __LINE__);
      abort();
    }

  rand_l(a_l, RND);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  u = 0;
  uaddmod_l(a_l, u, m_l, s1_l);
  mod_l(a_l, m_l, a_l);
  verif_l(s1_l, a_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      uaddmod_l(a_l, u, m_l, a_l);
      addmod_l(b_l, u_l, m_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      u = usrand64_l();
      u2bigint_l(u_l, u);
      uaddmod_l(a_l, u, m_l, s1_l);
      addmod_l(a_l, u_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void submodneu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l;
  BIGINT nul_l = {0 };
  int i;

  printf("Fonction submodneu_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  if (submod_l(a_l, b_l, a_l, a_l) == 0)
    {
      printf("Erreur dans submod, div par 0, ligne %d\n", __LINE__);
      abort();
    }

  setzero_l(a_l);
  setzero_l(b_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  submod_l(a_l, b_l, m_l, s1_l);
  submod_l(b_l, a_l, m_l, s2_l);
  verif_l(s1_l, nul_l, __LINE__);
  verif_l(s2_l, nul_l, __LINE__);

  setmax_l(a_l);
  setzero_l(b_l);
  submod_l(a_l, b_l, m_l, s1_l);
  mod_l(a_l, m_l, a_l);
  verif_l(s1_l, a_l, __LINE__);

  setzero_l(b_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      submod_l(a_l, b_l, m_l, s1_l);
      mod_l(a_l, m_l, a_l);
      verif_l(s1_l, a_l, __LINE__);
    }
      
  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setone_l(m_l);
  submod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
}

static void submodadd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l;
  int i;

  printf("Fonction submodadd_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      rand_l(c_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      submod_l(a_l, c_l, m_l, a_l);
      addmod_l(a_l, c_l, m_l, s1_l);
      mod_l(b_l, m_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      copy_l(c_l, b_l);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      submod_l(a_l, b_l, m_l, b_l);
      addmod_l(b_l, c_l, m_l, s1_l);
      mod_l(a_l, m_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(c_l, m_l);
      submod_l(a_l, b_l, m_l, m_l);
      addmod_l(m_l, b_l, c_l, s1_l);
      mod_l(a_l, c_l, a_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  submod_l(a_l, b_l, m_l, s1_l);
  addmod_l(s1_l, b_l, m_l, s1_l);
  mod_l(a_l, m_l, a_l);
  verif_l(s1_l, a_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      submod_l(a_l, b_l, m_l, s1_l);
      addmod_l(s1_l, b_l, m_l, s1_l);
      mod_l(a_l, m_l, a_l);
      verif_l(s1_l, a_l, __LINE__);
    }
}

static void submodass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction submodass_l()...\n");

 for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(b_l, c_l, m_l, s1_l);
      addmod_l(s1_l, a_l, m_l, s2_l);
      submod_l(s2_l, c_l, m_l, s3_l);
      submod_l(s3_l, b_l, m_l, s4_l);
      submod_l(s2_l, s1_l, m_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);
    }
}

static void submodu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, u_l;
  BIGINT nul_l = { 0 };
  USHORT u;
  int i, k;

  printf("Fonction submodu_l()...\n");

  rand_l(a_l, RND);
  if (usubmod_l(a_l, 2, nul_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  rand_l(a_l, RND);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  u = 0;
  usubmod_l(a_l, u, m_l, s1_l);
  mod_l(a_l, m_l, a_l);
  verif_l(s1_l, a_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      u = usrand64_l();
      u2bigint_l(u_l, u);
      k = RND;
      if ((numbits_l(u_l) + 1) > k)
	rand_l(a_l, numbits_l(u_l) + 1);
      else
	rand_l(a_l, k);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(b_l, a_l);
      usubmod_l(a_l, u, m_l, a_l);
      submod_l(b_l, u_l, m_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }


  for (i = 1; i <= nbretest; i++)
    {
      u = usrand64_l();
      u2bigint_l(u_l, u);
      k = RND;
      if ((numbits_l(u_l) + 1) > k)
	rand_l(a_l, numbits_l(u_l) + 1);
      else
	rand_l(a_l, k);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(b_l, m_l);
      usubmod_l(a_l, u, m_l, m_l);
      submod_l(a_l, u_l, b_l, s1_l);
      verif_l(m_l, s1_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      u = usrand64_l();
      u2bigint_l(u_l, u);
      usubmod_l(a_l, u, m_l, s1_l);
      submod_l(a_l, u_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulmodcom_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l;
  BIGINT nul_l = { 0 }, un_l = { 1, 1 };
  int i;

  printf("Fonction mulmodcom_l()...\n");

  setzero_l(a_l);
  setzero_l(b_l);
  if (mulmod_l(a_l, b_l, a_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  setzero_l(a_l);
  setzero_l(b_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  mulmod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setzero_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mulmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(b_l, a_l, m_l, s2_l);
      verif_l(s1_l, nul_l, __LINE__);
      verif_l(s2_l, nul_l, __LINE__);
    }

  setone_l(a_l);
  setone_l(b_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  mulmod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);

  setone_l(a_l);
  setmax_l(b_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  mulmod_l(a_l, b_l, m_l, s1_l);
  mulmod_l(b_l, a_l, m_l, s2_l);
  mod_l(b_l, m_l, b_l);
  verif_l(s1_l, b_l, __LINE__);
  verif_l(s2_l, b_l, __LINE__);

  setone_l(a_l);
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mulmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(b_l, a_l, m_l, s2_l);
      mod_l(b_l, m_l, b_l);
      verif_l(s1_l, b_l, __LINE__);
      verif_l(s2_l, b_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      copy_l(b_l, a_l);
      copy_l(c_l, a_l);
      rand_l(d_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mulmod_l(b_l, d_l, m_l, b_l);
      mulmod_l(d_l, a_l, m_l, a_l);
      verif_l(a_l, b_l, __LINE__);
      mulmod_l(c_l, d_l, m_l, m_l);
      verif_l(b_l, m_l, __LINE__);
    }

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setone_l(m_l);
  mulmod_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mulmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(b_l, a_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulmodadd_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l;
  int i, j, n;

  printf("Fonction mulmodadd_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  do
    n = usrand64_l() % 1001;
  while (n == 0);
  u2bigint_l(b_l, n);
  mulmod_l(a_l, b_l, m_l, s1_l);
  mod_l(a_l, m_l, s2_l);
  for (j = 1; j < n; j++)
    addmod_l(a_l, s2_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      do
	n = usrand64_l() % 1001;
      while (n == 0);
      u2bigint_l(b_l, n);
      mulmod_l(a_l, b_l, m_l, s1_l);
      mod_l(a_l, m_l, s2_l);
      for (j = 1; j < n; j++)
	addmod_l(a_l, s2_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulmodass_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction mulmodass_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mulmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(s1_l, c_l, m_l, s2_l);

      mulmod_l(b_l, c_l, m_l, s3_l);
      mulmod_l(a_l, s3_l, m_l, s4_l);

      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void mulmodu_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, u_l;
  BIGINT nul_l = { 0 };
  USHORT u;
  int i;

  printf("Fonction mulmodu_l()...\n");

  rand_l(a_l, RND);
  if (umulmod_l(a_l, 2, nul_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  rand_l(a_l, RND);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  u = 0;
  umulmod_l(a_l, u, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(b_l, a_l);
      u = usrand64_l();
      u2bigint_l(u_l, u);
      umulmod_l(a_l, u, m_l, a_l);
      mulmod_l(b_l, u_l, m_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      u = usrand64_l();
      u2bigint_l(u_l, u);
      umulmod_l(a_l, u, m_l, s1_l);
      mulmod_l(a_l, u_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void mulmoddis_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction mulmoddis_l()...\n");
  
  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      rand_l(c_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(s1_l, c_l, m_l, s2_l);

      mulmod_l(a_l, c_l, m_l, s3_l);
      mulmod_l(b_l, c_l, m_l, s4_l);
      addmod_l(s3_l, s4_l, m_l, s5_l);
      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void sqrmodneu_l(unsigned int nbretest)
{
  BIGINT a_l, m_l, s1_l, s2_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 };

  printf("Fonction sqrmodneu_l()...\n");

  setzero_l(a_l);
  if (sqrmod_l(a_l, a_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  setzero_l(a_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  sqrmod_l(a_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);

  setone_l(a_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  sqrmod_l(a_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);

  setmax_l(a_l);
  sqrmod_l(a_l, m_l, s1_l);
  mulmod_l(a_l, a_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);

  rand_l(a_l, RND);
  setone_l(m_l);
  sqrmod_l(a_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
}

static void sqrmodmul_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l;
  int i;

  printf("Fonction sqrmodmul_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(b_l, m_l);
      sqrmod_l(a_l, m_l, m_l);
      mulmod_l(a_l, a_l, b_l, s1_l);
      verif_l(m_l, s1_l, __LINE__);

      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      copy_l(b_l, a_l);
      sqrmod_l(a_l, m_l, a_l);
      mulmod_l(b_l, b_l, m_l, s1_l);
      verif_l(a_l, s1_l, __LINE__);
    }
}

static void sqrmodpot_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l, s4_l;
  int i;

  printf("Fonction sqrmodpot_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, BIGINTMAXBIT);
      rand_l(b_l, BIGINTMAXBIT);
      do
	rand_l(m_l, BIGINTMAXBIT);
      while (eqz_l(m_l));
      sqrmod_l(a_l, m_l, s1_l);
      sqrmod_l(b_l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      mulmod_l(a_l, b_l, m_l, s4_l);
      sqrmod_l(s4_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);

      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      sqrmod_l(a_l, m_l, s1_l);
      sqrmod_l(b_l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      mulmod_l(a_l, b_l, m_l, s4_l);
      sqrmod_l(s4_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);
    }
}

static void sqrmodbin_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  int i;

  printf("Fonction sqrmodbin_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      sqrmod_l(s1_l, m_l, s1_l);

      sqrmod_l(a_l, m_l, s2_l);
      sqrmod_l(b_l, m_l, s3_l);
      mulmod_l(a_l, b_l, m_l, s4_l);
      mulmod_l(s4_l, deux_l, m_l, s4_l);
      addmod_l(s2_l, s3_l, m_l, s2_l);
      addmod_l(s2_l, s4_l, m_l, s2_l);

      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void sqrmoddis_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  int i;

  printf("Fonction sqrmoddis_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      addmod_l(a_l, b_l, m_l, s1_l);
      mulmod_l(a_l, s1_l, m_l, s2_l);

      sqrmod_l(a_l, m_l, s3_l);
      mulmod_l(a_l, b_l, m_l, s4_l);
      addmod_l(s3_l, s4_l, m_l, s5_l);

      verif_l(s2_l, s5_l, __LINE__);
    }
}

static void expmodtriv_l(void)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 }, deux_l = { 1, 2 };
  USHORT k;

  printf("Fonction expmodtriv_l()...\n");

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setzero_l(m_l);
  if (expmodk_l(a_l, b_l, m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (expmod7_l(a_l, b_l, m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (uexpmod_l(a_l, usrand64_l(), m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (wexpmod_l(usrand64_l(), b_l, m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (expmod2k_l(a_l, usrand64_l(), m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  expmodk_l(nul_l, nul_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
  uexpmod_l(nul_l, 0, m_l, s2_l);
  verif_l(s2_l, un_l, __LINE__);
  wexpmod_l(0, nul_l, m_l, s3_l);
  verif_l(s3_l, un_l, __LINE__);
  expmod2k_l(nul_l, 0, m_l, s4_l);
  verif_l(s4_l, nul_l, __LINE__);
  expmod7_l(nul_l, nul_l, m_l, s5_l);
  verif_l(s5_l, un_l, __LINE__);

  setzero_l(a_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  expmodk_l(a_l, deux_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmod_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmod_l(0, deux_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);
  expmod2k_l(nul_l, 1, m_l, s4_l);
  verif_l(s4_l, nul_l, __LINE__);
  expmod7_l(a_l, deux_l, m_l, s5_l);
  verif_l(s5_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  setzero_l(b_l);
  do
    rand_l(m_l, RND);
  while (eqz_l(m_l));
  expmodk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
  uexpmod_l(a_l, 0, m_l, s2_l);
  verif_l(s2_l, un_l, __LINE__);
  wexpmod_l(usrand64_l(), b_l, m_l, s3_l);
  verif_l(s3_l, un_l, __LINE__);
  expmod2k_l(a_l, 0, m_l, s4_l);
  mod_l(a_l, m_l, a_l);
  verif_l(s4_l, a_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s5_l);
  verif_l(s5_l, un_l, __LINE__);

  do
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
    }
  while(comp_l(a_l, m_l) == -1);
  setone_l(b_l);
  mod_l(a_l, m_l, s1_l);
  expmodk_l(a_l, b_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);
  uexpmod_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, s1_l, __LINE__);
  expmod2k_l(a_l, 0, m_l, s2_l);
  verif_l(s2_l, s1_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);
  do
    k = usrand64_l();
  while (k < 3);
  u2bigint_l(m_l, (k - 1));
  wexpmod_l(k, b_l, m_l, s2_l);
  u2bigint_l(s1_l, k);
  mod_l(s1_l, m_l, s1_l);
  verif_l(s1_l, s2_l, __LINE__);

  rand_l(a_l, RND);
  setzero_l(b_l);
  setone_l(m_l);
  expmodk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmod_l(a_l, 0, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmod_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s5_l);
  verif_l(s5_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  setone_l(b_l);
  setone_l(m_l);
  expmodk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmod_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  uexpmod_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmod_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);
  expmod2k_l(a_l, 0, m_l, s4_l);
  verif_l(s4_l, nul_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s5_l);
  verif_l(s5_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setone_l(m_l);
  expmodk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmod_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  uexpmod_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmod_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);
  expmod2k_l(a_l, 0, m_l, s4_l);
  verif_l(s4_l, nul_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s5_l);
  verif_l(s5_l, nul_l, __LINE__);

  setone_l(b_l);
  u2bigint_l(m_l, 3);
  expmodk_l(deux_l, b_l, m_l, s1_l);
  verif_l(s1_l, deux_l, __LINE__);
  uexpmod_l(deux_l, 1, m_l, s2_l);
  verif_l(s2_l, deux_l, __LINE__);
  wexpmod_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, deux_l, __LINE__);
  expmod2k_l(deux_l, 0, m_l, s4_l);
  verif_l(s4_l, deux_l, __LINE__);
  expmod7_l(deux_l, b_l, m_l, s5_l);
  verif_l(s5_l, deux_l, __LINE__);

  u2bigint_l(b_l, 3);
  u2bigint_l(m_l, 3);
  expmodk_l(deux_l, b_l, m_l, s1_l);
  verif_l(s1_l, deux_l, __LINE__);
  uexpmod_l(deux_l, 3, m_l, s2_l);
  verif_l(s2_l, deux_l, __LINE__);
  wexpmod_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, deux_l, __LINE__);
  expmod7_l(deux_l, b_l, m_l, s5_l);
  verif_l(s5_l, deux_l, __LINE__);

  u2bigint_l(a_l, 100);
  u2bigint_l(b_l, 3);
  u2bigint_l(m_l, 10);
  expmodk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmod_l(a_l, 3, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmod_l(100, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s5_l);
  verif_l(s5_l, nul_l, __LINE__);
}

static void expmodmontriv_l(void)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l, s3_l;
  BIGINT nul_l = { 0 }, un_l = { 1 , 1 }, deux_l = { 1, 2 };

  printf("Fonction expmodmontriv_l()...\n");

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setzero_l(m_l);
  if (expmodmonk_l(a_l, b_l, m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (uexpmodmon_l(a_l, usrand64_l(), m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }
  if (wexpmodmon_l(usrand64_l(), b_l, m_l, a_l) == 0)
    {
      printf("Erreur div par 0, ligne %d\n", __LINE__);
      abort();
    }

  do
    rand_l(m_l, RND);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  expmodmonk_l(nul_l, nul_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
  uexpmodmon_l(nul_l, 0, m_l, s2_l);
  verif_l(s2_l, un_l, __LINE__);
  wexpmodmon_l(0, nul_l, m_l, s3_l);
  verif_l(s3_l, un_l, __LINE__);

  setzero_l(a_l);
  do
    rand_l(m_l, RND);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  expmodmonk_l(a_l, deux_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmodmon_l(0, deux_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  setzero_l(b_l);
  do
    rand_l(m_l, RND);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  expmodmonk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, un_l, __LINE__);
  uexpmodmon_l(a_l, 0, m_l, s2_l);
  verif_l(s2_l, un_l, __LINE__);
  wexpmodmon_l(usrand64_l(), b_l, m_l, s3_l);
  verif_l(s3_l, un_l, __LINE__);

  do
    {
      rand_l(a_l, RND);
      do
	rand_l(m_l, RND);
      while ((eqz_l(m_l)) || (iseven_l(m_l)));
    }
  while(comp_l(a_l, m_l) == -1);
  setone_l(b_l);
  mod_l(a_l, m_l, s1_l);
  expmodmonk_l(a_l, b_l, m_l, s2_l);
  verif_l(s1_l, s2_l, __LINE__);
  uexpmodmon_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, s1_l, __LINE__);

  rand_l(a_l, RND);
  setzero_l(b_l);
  setone_l(m_l);
  expmodmonk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 0, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmodmon_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  setone_l(b_l);
  setone_l(m_l);
  expmodmonk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmodmon_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);

  rand_l(a_l, RND);
  rand_l(b_l, RND);
  setone_l(m_l);
  expmodmonk_l(a_l, b_l, m_l, s1_l);
  verif_l(s1_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 1, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  uexpmodmon_l(a_l, 2, m_l, s2_l);
  verif_l(s2_l, nul_l, __LINE__);
  wexpmodmon_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, nul_l, __LINE__);

  setone_l(b_l);
  u2bigint_l(m_l, 3);
  expmodmonk_l(deux_l, b_l, m_l, s1_l);
  verif_l(s1_l, deux_l, __LINE__);
  uexpmodmon_l(deux_l, 1, m_l, s2_l);
  verif_l(s2_l, deux_l, __LINE__);
  wexpmodmon_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, deux_l, __LINE__);

  u2bigint_l(b_l, 3);
  u2bigint_l(m_l, 3);
  expmodmonk_l(deux_l, b_l, m_l, s1_l);
  verif_l(s1_l, deux_l, __LINE__);
  uexpmodmon_l(deux_l, 3, m_l, s2_l);
  verif_l(s2_l, deux_l, __LINE__);
  wexpmodmon_l(2, b_l, m_l, s3_l);
  verif_l(s3_l, deux_l, __LINE__);
}

static void expmodsqr_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k;
  int i;

  printf("Fonction expmodsqr_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      sqrmod_l(a_l, m_l, s1_l);
      expmodk_l(a_l, deux_l, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      expmod7_l(a_l, deux_l, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      uexpmod_l(a_l, 2, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      expmod2k_l(a_l, 1, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      k = usrand64_l();
      u2bigint_l(a_l, k);
      sqrmod_l(a_l, m_l, s1_l);
      wexpmod_l(k, deux_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void expmodmonsqr_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, m_l, s1_l, s2_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k;
  int i;

  printf("Fonction expmodmonsqr_l()...\n");

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND);
      do
	rand_l(m_l, RND);
      while ((eqz_l(m_l)) || (iseven_l(m_l)));
      sqrmod_l(a_l, m_l, s1_l);
      expmodmonk_l(a_l, deux_l, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      uexpmodmon_l(a_l, 2, m_l, s2_l);
      verif_l(s2_l, s1_l, __LINE__);
      k = usrand64_l();
      u2bigint_l(a_l, k);
      sqrmod_l(a_l, m_l, s1_l);
      wexpmodmon_l(k, deux_l, m_l, s2_l);
      verif_l(s1_l, s2_l, __LINE__);
    }
}

static void expmodprop_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k, l, n;
  int i;

  printf("Fonction expmodprop_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT - 1);
  rand_l(c_l, BIGINTMAXBIT - 1);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  add_l(b_l, c_l, s1_l);
  expmodk_l(a_l, b_l, m_l, s2_l);
  expmodk_l(a_l, c_l, m_l, s3_l);
  mulmod_l(s2_l, s3_l, m_l, s4_l);
  expmodk_l(a_l, s1_l, m_l, s5_l);
  verif_l(s4_l, s5_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s2_l);
  expmod7_l(a_l, c_l, m_l, s3_l);
  mulmod_l(s2_l, s3_l, m_l, s4_l);
  expmod7_l(a_l, s1_l, m_l, s5_l);
  verif_l(s4_l, s5_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND - 1);
      rand_l(c_l, RND - 1);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      add_l(b_l, c_l, s1_l);
      expmodk_l(a_l, b_l, m_l, s2_l);
      expmodk_l(a_l, c_l, m_l, s3_l);
      mulmod_l(s2_l, s3_l, m_l, s4_l);
      expmodk_l(a_l, s1_l, m_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);
      expmod7_l(a_l, b_l, m_l, s2_l);
      expmod7_l(a_l, c_l, m_l, s3_l);
      mulmod_l(s2_l, s3_l, m_l, s4_l);
      expmod7_l(a_l, s1_l, m_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);

      do
	{
	  n = 1 + (usrand64_l() % (BIGINTMAXBIT - 1));
	  l = usrand64_l() % n;
	  k = n - l;
	}
      while (k == 0);
      uexpmod_l(a_l, k, m_l, s1_l);
      uexpmod_l(a_l, l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      uexpmod_l(a_l, n, m_l, s4_l);
      verif_l(s4_l, s3_l, __LINE__);
      
      wexpmod_l(n, b_l, m_l, s1_l);
      wexpmod_l(n, c_l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      add_l(b_l, c_l, s4_l);
      wexpmod_l(n, s4_l, m_l, s5_l);
      verif_l(s3_l, s5_l, __LINE__);

      expmod2k_l(a_l, k, m_l, s1_l);
      expmod2k_l(a_l, l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      setmax_l(d_l);
      uexpmod_l(deux_l, k, d_l, s4_l);
      uexpmod_l(deux_l, l, d_l, s5_l);
      add_l(s4_l, s5_l, s4_l);
      expmodk_l(a_l, s4_l, m_l, s5_l);
      verif_l(s5_l, s3_l, __LINE__);
    }
}

static void expmodmonprop_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  USHORT k, l, n;
  int i;

  printf("Fonction expmodmonprop_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT - 1);
  rand_l(c_l, BIGINTMAXBIT - 1);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  add_l(b_l, c_l, s1_l);
  expmodmonk_l(a_l, b_l, m_l, s2_l);
  expmodmonk_l(a_l, c_l, m_l, s3_l);
  mulmod_l(s2_l, s3_l, m_l, s4_l);
  expmodmonk_l(a_l, s1_l, m_l, s5_l);
  verif_l(s4_l, s5_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND - 1);
      rand_l(c_l, RND - 1);
      do
	rand_l(m_l, RND);
      while ((eqz_l(m_l)) || (iseven_l(m_l)));
      add_l(b_l, c_l, s1_l);
      expmodmonk_l(a_l, b_l, m_l, s2_l);
      expmodmonk_l(a_l, c_l, m_l, s3_l);
      mulmod_l(s2_l, s3_l, m_l, s4_l);
      expmodmonk_l(a_l, s1_l, m_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);

      do
	{
	  n = 1 + (usrand64_l() % (BIGINTMAXBIT - 1));
	  l = usrand64_l() % n;
	  k = n - l;
	}
      while (k == 0);
      uexpmodmon_l(a_l, k, m_l, s1_l);
      uexpmodmon_l(a_l, l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      uexpmodmon_l(a_l, n, m_l, s4_l);
      verif_l(s4_l, s3_l, __LINE__);
      
      wexpmodmon_l(n, b_l, m_l, s1_l);
      wexpmodmon_l(n, c_l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      add_l(b_l, c_l, s4_l);
      wexpmodmon_l(n, s4_l, m_l, s5_l);
      verif_l(s3_l, s5_l, __LINE__);
    }
}

static void expprop_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l, s3_l, s4_l, s5_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k, l, n;
  int i;

  printf("Fonction expprop_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT - 1);
  rand_l(c_l, BIGINTMAXBIT - 1);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  add_l(b_l, c_l, s1_l);
  expmod_l(a_l, b_l, m_l, s2_l);
  expmod_l(a_l, c_l, m_l, s3_l);
  mulmod_l(s2_l, s3_l, m_l, s4_l);
  expmod_l(a_l, s1_l, m_l, s5_l);
  verif_l(s4_l, s5_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, RND - 1);
      rand_l(c_l, RND - 1);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      add_l(b_l, c_l, s1_l);
      expmod_l(a_l, b_l, m_l, s2_l);
      expmod_l(a_l, c_l, m_l, s3_l);
      mulmod_l(s2_l, s3_l, m_l, s4_l);
      expmod_l(a_l, s1_l, m_l, s5_l);
      verif_l(s4_l, s5_l, __LINE__);

      do
	{
	  n = 1 + (usrand64_l() % (BIGINTMAXBIT - 1));
	  l = usrand64_l() % n;
	  k = n - l;
	}
      while (k == 0);
      uexpmod_l(a_l, k, m_l, s1_l);
      uexpmod_l(a_l, l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      uexpmod_l(a_l, n, m_l, s4_l);
      verif_l(s4_l, s3_l, __LINE__);
      
      wexpmod_l(n, b_l, m_l, s1_l);
      wexpmod_l(n, c_l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      add_l(b_l, c_l, s4_l);
      wexpmod_l(n, s4_l, m_l, s5_l);
      verif_l(s3_l, s5_l, __LINE__);

      expmod2k_l(a_l, k, m_l, s1_l);
      expmod2k_l(a_l, l, m_l, s2_l);
      mulmod_l(s1_l, s2_l, m_l, s3_l);
      setmax_l(d_l);
      uexpmod_l(deux_l, k, d_l, s4_l);
      uexpmod_l(deux_l, l, d_l, s5_l);
      add_l(s4_l, s5_l, s4_l);
      expmod_l(a_l, s4_l, m_l, s5_l);
      verif_l(s5_l, s3_l, __LINE__);
    }
}

static void expmodprop2_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k, l, n;
  int i;

  printf("Fonction expmodprop2_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT / 2);
  rand_l(c_l, BIGINTMAXBIT / 2);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  mul_l(b_l, c_l, s1_l);
  expmodk_l(a_l, b_l, m_l, s2_l);
  expmodk_l(s2_l, c_l, m_l, s3_l);
  expmodk_l(a_l, s1_l, m_l, s4_l);
  verif_l(s3_l, s4_l, __LINE__);
  expmod7_l(a_l, b_l, m_l, s2_l);
  expmod7_l(s2_l, c_l, m_l, s3_l);
  expmod7_l(a_l, s1_l, m_l, s4_l);
  verif_l(s3_l, s4_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, usrand64_l() % BIGINTMAXBIT / 2);
      rand_l(c_l, usrand64_l() % BIGINTMAXBIT / 2);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mul_l(b_l, c_l, s1_l);
      expmodk_l(a_l, b_l, m_l, s2_l);
      expmodk_l(s2_l, c_l, m_l, s3_l);
      expmodk_l(a_l, s1_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);
      expmod7_l(a_l, b_l, m_l, s2_l);
      expmod7_l(s2_l, c_l, m_l, s3_l);
      expmod7_l(a_l, s1_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);

      k = usrand64_l() % 256;
      l = usrand64_l() % 256;
      uexpmod_l(a_l, k, m_l, s1_l);
      uexpmod_l(s1_l, l, m_l, s2_l);
      uexpmod_l(a_l, k * l, m_l, s3_l);
      verif_l(s2_l, s3_l, __LINE__);

      n = k + l;
      wexpmod_l(n, b_l, m_l, s1_l);
      expmodk_l(s1_l, c_l, m_l, s2_l);
      mul_l(b_l, c_l, s3_l);
      wexpmod_l(n, s3_l, m_l, s4_l);
      verif_l(s2_l, s4_l, __LINE__);
      
      k = usrand64_l() % BIGINTMAXBIT / 2;
      l = usrand64_l() % BIGINTMAXBIT / 2;
      expmod2k_l(a_l, k, m_l, s1_l);
      expmod2k_l(s1_l, l, m_l, s2_l);
      setmax_l(d_l);
      uexpmod_l(deux_l, k + l, d_l, s3_l);
      expmodk_l(a_l, s3_l, m_l, s4_l);
      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void expmodmonprop2_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, m_l, s1_l, s2_l, s3_l, s4_l;
  USHORT k, l, n;
  int i;

  printf("Fonction expmodmonprop2_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT / 2);
  rand_l(c_l, BIGINTMAXBIT / 2);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  mul_l(b_l, c_l, s1_l);
  expmodmonk_l(a_l, b_l, m_l, s2_l);
  expmodmonk_l(s2_l, c_l, m_l, s3_l);
  expmodmonk_l(a_l, s1_l, m_l, s4_l);
  verif_l(s3_l, s4_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, usrand64_l() % BIGINTMAXBIT / 2);
      rand_l(c_l, usrand64_l() % BIGINTMAXBIT / 2);
      do
	rand_l(m_l, RND);
      while ((eqz_l(m_l)) || (iseven_l(m_l)));
      mul_l(b_l, c_l, s1_l);
      expmodmonk_l(a_l, b_l, m_l, s2_l);
      expmodmonk_l(s2_l, c_l, m_l, s3_l);
      expmodmonk_l(a_l, s1_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);

      k = usrand64_l() % 256;
      l = usrand64_l() % 256;
      uexpmodmon_l(a_l, k, m_l, s1_l);
      uexpmodmon_l(s1_l, l, m_l, s2_l);
      uexpmodmon_l(a_l, k * l, m_l, s3_l);
      verif_l(s2_l, s3_l, __LINE__);

      n = k + l;
      wexpmodmon_l(n, b_l, m_l, s1_l);
      expmodmonk_l(s1_l, c_l, m_l, s2_l);
      mul_l(b_l, c_l, s3_l);
      wexpmodmon_l(n, s3_l, m_l, s4_l);
      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void expprop2_l(unsigned int nbretest)
{
  BIGINT a_l, b_l, c_l, d_l, m_l, s1_l, s2_l, s3_l, s4_l;
  BIGINT deux_l = { 1, 2 };
  USHORT k, l, n;
  int i;

  printf("Fonction expprop2_l()...\n");

  rand_l(a_l, BIGINTMAXBIT);
  rand_l(b_l, BIGINTMAXBIT / 2);
  rand_l(c_l, BIGINTMAXBIT / 2);
  do
    rand_l(m_l, BIGINTMAXBIT);
  while (eqz_l(m_l));
  mul_l(b_l, c_l, s1_l);
  expmod_l(a_l, b_l, m_l, s2_l);
  expmod_l(s2_l, c_l, m_l, s3_l);
  expmod_l(a_l, s1_l, m_l, s4_l);
  verif_l(s3_l, s4_l, __LINE__);

  for (i = 1; i <= nbretest; i++)
    {
      rand_l(a_l, RND);
      rand_l(b_l, usrand64_l() % BIGINTMAXBIT / 2);
      rand_l(c_l, usrand64_l() % BIGINTMAXBIT / 2);
      do
	rand_l(m_l, RND);
      while (eqz_l(m_l));
      mul_l(b_l, c_l, s1_l);
      expmod_l(a_l, b_l, m_l, s2_l);
      expmod_l(s2_l, c_l, m_l, s3_l);
      expmod_l(a_l, s1_l, m_l, s4_l);
      verif_l(s3_l, s4_l, __LINE__);

      k = usrand64_l() % 256;
      l = usrand64_l() % 256;
      uexpmod_l(a_l, k, m_l, s1_l);
      uexpmod_l(s1_l, l, m_l, s2_l);
      uexpmod_l(a_l, k * l, m_l, s3_l);
      verif_l(s2_l, s3_l, __LINE__);

      n = k + l;
      wexpmod_l(n, b_l, m_l, s1_l);
      expmod_l(s1_l, c_l, m_l, s2_l);
      mul_l(b_l, c_l, s3_l);
      wexpmod_l(n, s3_l, m_l, s4_l);
      verif_l(s2_l, s4_l, __LINE__);
      
      k = usrand64_l() % BIGINTMAXBIT / 2;
      l = usrand64_l() % BIGINTMAXBIT / 2;
      expmod2k_l(a_l, k, m_l, s1_l);
      expmod2k_l(s1_l, l, m_l, s2_l);
      setmax_l(d_l);
      uexpmod_l(deux_l, k + l, d_l, s3_l);
      expmod_l(a_l, s3_l, m_l, s4_l);
      verif_l(s2_l, s4_l, __LINE__);
    }
}

static void expmodacc_l(void)
{
  BIGINT a_l, b_l, c_l, m_l;
  USHORT k;

  printf("Fonction expmodacc_l()...\n");

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  copy_l(c_l, a_l);
  expmodk_l(a_l, a_l, m_l, a_l);
  expmodk_l(b_l, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  copy_l(c_l, a_l);
  expmod7_l(a_l, a_l, m_l, a_l);
  expmod7_l(b_l, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  k = usrand64_l();
  uexpmod_l(a_l, k, m_l, a_l);
  uexpmod_l(b_l, k, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  k = usrand64_l();
  wexpmod_l(k, a_l, m_l, a_l);
  wexpmod_l(k, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  k = usrand64_l() % 768;
  expmod2k_l(a_l, k, m_l, a_l);
  expmod2k_l(b_l, k, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);
}

static void expmodmonacc_l(void)
{
  BIGINT a_l, b_l, c_l, m_l;
  USHORT k;

  printf("Fonction expmodmonacc_l()...\n");

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  copy_l(b_l, a_l);
  copy_l(c_l, a_l);
  expmodmonk_l(a_l, a_l, m_l, a_l);
  expmodmonk_l(b_l, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  copy_l(b_l, a_l);
  k = usrand64_l();
  uexpmod_l(a_l, k, m_l, a_l);
  uexpmod_l(b_l, k, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while ((eqz_l(m_l)) || (iseven_l(m_l)));
  copy_l(b_l, a_l);
  k = usrand64_l();
  wexpmodmon_l(k, a_l, m_l, a_l);
  wexpmodmon_l(k, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);
}  

static void expacc_l(void)
{
  BIGINT a_l, b_l, c_l, m_l;

  printf("Fonction expacc_l()...\n");

  rand_l(a_l, 768);
  do
    rand_l(m_l, 1024);
  while (eqz_l(m_l));
  copy_l(b_l, a_l);
  copy_l(c_l, a_l);
  expmod_l(a_l, a_l, m_l, a_l);
  expmod_l(b_l, b_l, m_l, m_l);
  verif_l(a_l, m_l, __LINE__);
}

int main()
{
  printf("Module de test %s pour la librairie arithmétique en C\n\n", __FILE__);
  clock_t t, debut, fin;
  t = 0;
  debut = clock();
  ulseed64_l(time(0));

#ifndef SECURE
  addcomm_l(10000);
  addass_l(10000);
  addsub_l(10000);
  addu_l(10000);
  addofl_l();
  subneu_l(10000);
  subadd_l(10000);
  subass_l(10000);
  subu_l(10000);
  subufl_l();
  mulclasscomm_l(10000);
  mulclassadd_l(10000);
  mulclassass_l(10000);
  mulclassdis_l(10000);
  mulclassofl_l();
  mulkarcomm_l(10000);
  mulkaradd_l(10000);
  mulkarass_l(10000);
  mulkardis_l(10000);
  mulkarofl_l();
  mulcomm_l(10000);
  muladd_l(10000);
  mulass_l(10000);
  mulu_l(10000);
  muldis_l(10000);
  mulofl_l();
  sqrclassneu_l();
  sqrclassmul_l(10000);
  sqrclasspot_l(10000);
  sqrclassbin_l(10000);
  sqrclassdist_l(10000);
  sqrclassofl_l();
  sqrkarneu_l();
  sqrkarmul_l(10000);
  sqrkarpot_l(10000);
  sqrkarbin_l(10000);
  sqrkardist_l(10000);
  sqrkarofl_l();
  sqrneu_l();
  sqrmul_l(10000);
  sqrpot_l(10000);
  sqrbin_l(10000);
  sqrdist_l(10000);
  sqrofl_l();
  divneu_l(10000);
  divsmall_l(10000);
  divs5_l();
  divsub_l(10000);
  divrem_l(10000);
  divmod_l(10000);
  addmodcom_l(10000);
  addmodass_l(10000);
  addmodsub_l(10000);
  addmodu_l(10000);
  submodneu_l(10000);
  submodadd_l(10000);
  submodass_l(10000);
  submodu_l(10000);
  mulmodcom_l(10000);
  mulmodadd_l(10000);
  mulmodass_l(10000);
  mulmodu_l(10000);
  mulmoddis_l(10000);
  sqrmodneu_l(10000);
  sqrmodmul_l(10000);
  sqrmodpot_l(10000);
  sqrmodbin_l(10000);
  sqrmoddis_l(10000);
  expmodtriv_l();
  expmodsqr_l(50);
  expmodprop_l(50);
  expmodprop2_l(50);
  expmodacc_l();
  expmodmontriv_l();
  expmodmonsqr_l(50);
  expmodmonprop_l(50);
  expmodmonprop2_l(50);
  expmodmonacc_l();
  expprop_l(50);
  expprop2_l(50);
  expacc_l();
#else
  addcomm_l(100);
  addass_l(100);
  addsub_l(100);
  addu_l(100);
  addofl_l();
  subneu_l(100);
  subadd_l(100);
  subass_l(100);
  subu_l(100);
  subufl_l();
  mulclasscomm_l(100);
  mulclassadd_l(100);
  mulclassass_l(100);
  mulclassdis_l(100);
  mulclassofl_l();
  mulkarcomm_l(100);
  mulkaradd_l(100);
  mulkarass_l(100);
  mulkardis_l(100);
  mulkarofl_l();
  mulcomm_l(100);
  muladd_l(100);
  mulass_l(100);
  mulu_l(100);
  muldis_l(100);
  mulofl_l();
  sqrclassneu_l();
  sqrclassmul_l(100);
  sqrclasspot_l(100);
  sqrclassbin_l(100);
  sqrclassdist_l(100);
  sqrclassofl_l();
  sqrkarneu_l();
  sqrkarmul_l(100);
  sqrkarpot_l(100);
  sqrkarbin_l(100);
  sqrkardist_l(100);
  sqrkarofl_l();
  sqrneu_l();
  sqrmul_l(100);
  sqrpot_l(100);
  sqrbin_l(100);
  sqrdist_l(100);
  sqrofl_l();
  divneu_l(100);
  divsmall_l(100);
  divs5_l();
  divsub_l(100);
  divrem_l(100);
  divmod_l(100);
  addmodcom_l(100);
  addmodass_l(100);
  addmodsub_l(100);
  addmodu_l(100);
  submodneu_l(100);
  submodadd_l(100);
  submodass_l(100);
  submodu_l(100);
  mulmodcom_l(100);
  mulmodadd_l(100);
  mulmodass_l(100);
  mulmodu_l(100);
  mulmoddis_l(100);
  sqrmodneu_l(100);
  sqrmodmul_l(100);
  sqrmodpot_l(100);
  sqrmodbin_l(100);
  sqrmoddis_l(100);
  expmodtriv_l();
  expmodsqr_l(5);
  expmodprop_l(5);
  expmodprop2_l(5);
  expmodacc_l();
  expmodmontriv_l();
  expmodmonsqr_l(5);
  expmodmonprop_l(5);
  expmodmonprop2_l(5);
  expmodmonacc_l();
  expprop_l(5);
  expprop2_l(5);
  expacc_l();
#endif

  fin = clock();
  t = t + (fin - debut);

  printf("Temps pour %s    = %ld\n", __FILE__, t);
  printf("\nTous les tests du fichier %s effectués.\n", __FILE__);

  return 0;
}
