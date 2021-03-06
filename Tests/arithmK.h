/***********************************************************************
 *                                                                     *
 *         Header pour la librairie de fonctions d'arithmétique        *
 *           élémentaire pour les grands entiers (4096 bits)           *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#ifndef arithm_H
#define arithm_H

/* Constantes utiles pour la librairie */
/***************************************/

/* Nombre de digits pour les BIGINT [0, 2^4096[ */

#define BIGINTMAXDIGIT 256U /* 256U */
#define BIGINTMAXSHORT (BIGINTMAXDIGIT + 1)
#define BIGINTMAXBYTE (BIGINTMAXDIGIT << 1)
#define BIGINTMAXBIT (BIGINTMAXDIGIT << 4)

/* Représentation interne des BIGINT */

#define LDBITPERDGT 4U 
#define B 0x10000UL
#define BDIV2 B / 2
#define BMIN1 B - 1
#define BITPERDGT 16UL

/* Codes d'erreurs */

#define BIGINT_OK 0
#define BIGINT_DBZ -1
#define BIGINT_OFL -2
#define BIGINT_UFL -3
#define BIGINT_MAL -4
#define BIGINT_NOR -5
#define BIGINT_BOR -6
#define BIGINT_NPT -7
#define BIGINT_MOD -8
#define CHECK_OK 0
#define CHECK_LDZ 1
#define CHECK_MEM -1
#define CHECK_OFL -2
#define NUMSPRIMES 6542

/* Définitions des types utiles pour les grands entiers */

typedef unsigned short bigint;
typedef unsigned long bigintd;
typedef bigint BIGINT[BIGINTMAXSHORT];
typedef bigint BIGINTP[1 + BIGINTMAXSHORT];
typedef bigint BIGINTD[1 + (BIGINTMAXDIGIT << 1)];
typedef bigint BIGINTM[2 + (BIGINTMAXDIGIT << 1)];
typedef bigint BIGINTQ[1 + (BIGINTMAXDIGIT << 2)];
typedef bigint *BIGINTPTR;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

/* Prototypes */
/**************/

/* Prototypes des fonctions opérant sur les digits */

extern USHORT digits_l(BIGINTD n_l);
extern bigint *lsdptr_l(BIGINTD n_l);
extern bigint *msdptr_l(BIGINTD n_l);
extern void setdigits_l(BIGINTD n_l, USHORT l);
extern void decdigits_l(BIGINTD n_l);
extern void incdigits_l(BIGINTD n_l);
extern void rmldzeros_l(BIGINTD n_l);

/* Prototypes des fonctions d'initialisation et de comparaison */

extern void copy_l(BIGINTM dest_l, BIGINTM src_l);
extern void setzero_l(BIGINTM n_l);
extern void setone_l(BIGINTM n_l);
extern void settwo_l(BIGINT n_l);
extern void setmax_l(BIGINT n_l);
extern int comp_l(BIGINTM a_l, BIGINTM b_l); /* = 1 si a > b, 0 si a = b et -1 si a < b*/
extern int equ_l(BIGINTM a_l, BIGINTM b_l);
extern int eqz_l(BIGINTM a_l);
extern int eq1_l(BIGINT a_l);
extern int ge_l(BIGINTM a_l, BIGINTM b_l);
extern int gtzero_l(BIGINTM a_l);
extern int iseven_l(BIGINT n_l);

/* Prototypes des fonctions kernel */

extern void add(BIGINT a_l, BIGINT b_l, BIGINTP s_l);
extern void sub(BIGINT a_l, BIGINT b_l, BIGINT d_l);
extern void mul(BIGINT a_l, BIGINT b_l, BIGINTD p_l);
extern void umul(BIGINT a_l, USHORT b, BIGINTP p_l);
extern void sqr(BIGINT a_l, BIGINTD p_l);
extern int sadd(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP s_l);
extern int ssub(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP d_l);

/* Prototypes des fonctions arithmétiques élémentaires */

extern void u2bigint_l(BIGINT u_l, USHORT u);
extern void ul2bigint_l(BIGINT u_l, ULONG ul);
extern int add_l(BIGINT aa_l, BIGINT bb_l, BIGINT s_l);
extern int uadd_l(BIGINT a_l, USHORT b, BIGINT s_l);
extern int sub_l(BIGINT aa_l, BIGINT bb_l, BIGINT d_l);
extern int usub_l(BIGINT a_l, USHORT b, BIGINT d_l);
extern int mul_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l);
extern int umul_l(BIGINT aa_l, USHORT b, BIGINT p_l);
extern int sqr_l(BIGINT aa_l, BIGINT p_l);
extern int div_l(BIGINTM aa_l, BIGINT bb_l, BIGINTM quot_l, BIGINT rem_l);
extern int udiv_l(BIGINT aa_l, USHORT b, BIGINT quot_l, BIGINT rem_l);
extern int mod_l(BIGINTM a_l, BIGINT b_l, BIGINT rem_l);
extern int mod2_l(BIGINT aa_l, ULONG k, BIGINT rem_l);
extern USHORT umod_l(BIGINT aa_l, USHORT b);
extern int smod(BIGINT a_l, int asign, BIGINT b_l, BIGINT rem_l);

/* Prototypes des fonctions arithmétiques modulo */

extern int addmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT s_l);
extern int submod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT d_l);
extern int mulmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT m_l, BIGINT p_l);
extern int sqrmod_l(BIGINT aa_l, BIGINT m_l, BIGINT p_l);
extern int uaddmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT s_l);
extern int usubmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT d_l);
extern int umulmod_l(BIGINT a_l, USHORT b, BIGINT m_l, BIGINT p_l);
extern int eqmod_l(BIGINT a_l, BIGINT b_l, BIGINT m_l);

/* Prototypes des fonctions exponentielles */

extern int uexpmod_l(BIGINT aa_l, USHORT e, BIGINT m_l, BIGINT p_l);
extern int wexpmod_l(USHORT a, BIGINT ee_l, BIGINT m_l, BIGINT p_l);
extern int expmodk_l(BIGINT aa_l, BIGINT ee_l, BIGINT m_l, BIGINT p_l);
extern int expmod2k_l(BIGINT aa_l, USHORT k, BIGINT m_l, BIGINT p_l);

/* Prototypes des fonctions opérant sur les bits */

extern unsigned int numbits_l(BIGINT aa_l);
extern int shleft_l(BIGINT a_l);
extern int shright_l(BIGINT a_l);
extern int shift_l(BIGINT a_l, long int nbrebits);
extern void and_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
extern void or_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
extern void xor_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
extern int setbit_l(BIGINT a_l, unsigned int pos);
extern int testbit_l(BIGINT a_l, unsigned int pos);
extern int clearbit_l(BIGINT a_l, unsigned int pos);

/* Prototypes des fonctions d'entrée-sortie */

extern int str2bigint_l(BIGINT n_l, char *str, USHORT b);
extern char *bigint2str_l(BIGINT n_l, USHORT b, int showb);
extern int byte2bigint_l(BIGINT n_l, UCHAR* bytestr, int len);
extern UCHAR *bigint2byte_l(BIGINT n_l, int *len);
extern int check_l(BIGINT n_l);

/* Prototypes des fonctions de théorie des nombres */

extern void gcd_l(BIGINT aa_l, BIGINT bb_l, BIGINT g_l);
extern int lcm_l(BIGINT a_l, BIGINT b_l, BIGINT l_l);
extern void xgcd_l(BIGINT a_l, BIGINT b_l, BIGINT g_l, BIGINT u_l, int *usign, BIGINT v_l, int *vsign);
extern void invmod_l(BIGINT a_l, BIGINT m_l, BIGINT g_l, BIGINT i_l);
extern void introot_l(BIGINT n_l, BIGINT r_l);
extern ULONG uintroot_l(ULONG n);
extern unsigned int issqr_l(BIGINT n_l, BIGINT r_l);
extern int jacobi_l(BIGINT aa_l, BIGINT bb_l);
extern int proot_l(BIGINT a_l, BIGINT p_l, BIGINT x_l);
extern int pqroot_l(BIGINT a_l, BIGINT p_l, BIGINT q_l, BIGINT x_l);
extern int chinrem_l(unsigned int numeq, bigint **coeff_l, BIGINT x_l);

/* Prototypes des fonctions de test de primalité */

extern ULONG *primesgen(ULONG N);
extern USHORT sieve_l(BIGINT a_l, unsigned int numsprimes);
extern int MRprime_l(BIGINT n_l, unsigned int numsprimes, unsigned int iter);
extern int SSprime_l(BIGINT n_l, unsigned int numsprimes, unsigned iter);

/* Prototypes des fonctions de génération aléatoire */

extern bigint *rand64_l(void);
extern bigint *seed64_l(BIGINT seed_l);
extern bigint *ulseed64_l(ULONG seed);
extern UCHAR ucrand64_l(void);
extern USHORT usrand64_l(void);
extern ULONG ulrand64_l(void);
extern void rand_l(BIGINT r_l, int l);
extern int randbit_l(void);
extern int seedBBS_l(BIGINT seed_l);
extern int ulseedBBS_l(ULONG seed);
extern USHORT usrandBBS_l(void);
extern ULONG ulrandBBS_l(void);
extern UCHAR ucrandBBS_l(void);
extern void randBBS_l(BIGINT r_l, int l);

/* Prototypes des fonctions de Montgomery */

extern void mulmon_l(BIGINT a_l, BIGINT b_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l);
extern void sqrmon_l(BIGINT a_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l);
extern USHORT invmon_l(BIGINT n_l);
extern int uexpmodmon_l(BIGINT aa_l, USHORT e, BIGINT m_l, BIGINT p_l);
extern int wexpmodmon_l(USHORT a, BIGINT ee_l, BIGINT m_l, BIGINT p_l);
extern int expmodmonk_l(BIGINT aa_l, BIGINT ee_l, BIGINT m_l, BIGINT p_l);

/* Prototypes des fonctions de purge */

extern void purge_l(BIGINT n_l);


/* Fin de arithm.h */


#endif
