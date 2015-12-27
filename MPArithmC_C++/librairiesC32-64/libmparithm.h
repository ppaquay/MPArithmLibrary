/***********************************************************************
 *                                                                     *
 *        Header pour la librairie de fonctions d'arithmétique         *
 *     en multi-précision (4096 bits si ARCH32 et 8192 si ARCH64)      *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/


#ifndef libarithm_H
#define libarithm_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/* Constantes utiles pour la librairie */
/***************************************/

#ifdef ARCH32
/* Nombre de digits pour les BIGINT [0, 2^4096[ */

#define BIGINTMAXDIGIT 256U /*256U*/
#define BIGINTMAXSHORT (BIGINTMAXDIGIT + 1)
#define BIGINTMAXBYTE (BIGINTMAXDIGIT << 1)
#define BIGINTMAXBIT (BIGINTMAXDIGIT << 4)

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

/* Représentation interne des BIGINT */

#define LDBITPERDGT 4U
#define B 0x10000UL
#define BDIV2 0x8000U
#define BMIN1 0xffffU
#define BITPERDGT 16U
#endif

#ifdef ARCH64
/* Nombre de digits pour les BIGINT [0, 2^8192[ */

#define BIGINTMAXDIGIT 256U /*256U*/
#define BIGINTMAXSHORT (BIGINTMAXDIGIT + 1)
#define BIGINTMAXBYTE (BIGINTMAXDIGIT << 2)
#define BIGINTMAXBIT (BIGINTMAXDIGIT << 5)

/* Définitions des types utiles pour les grands entiers */

typedef unsigned int bigint;
typedef unsigned long bigintd;
typedef bigint BIGINT[BIGINTMAXSHORT];
typedef bigint BIGINTP[1 + BIGINTMAXSHORT];
typedef bigint BIGINTD[1 + (BIGINTMAXDIGIT << 1)];
typedef bigint BIGINTM[2 + (BIGINTMAXDIGIT << 1)];
typedef bigint BIGINTQ[1 + (BIGINTMAXDIGIT << 2)];
typedef bigint *BIGINTPTR;
typedef unsigned char UCHAR;
typedef unsigned int USHORT;
typedef unsigned long ULONG;

/* Représentation interne des BIGINT */

#define LDBITPERDGT 5U
#define B 0x100000000UL
#define BDIV2 0x80000000U
#define BMIN1 0xffffffffU
#define BITPERDGT 32U
#endif

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

/* Nombre de digits efficaces pour Karatsuba */

#ifdef ARCH32
#define MUL_LIMIT 128
#define SQR_LIMIT 128
#endif

#ifdef ARCH64
#define MUL_LIMIT 64
#define SQR_LIMIT 64
#endif

/* Prototypes */
/**************/

/* Prototypes des fonctions opérant sur les digits */

extern USHORT digits_l(BIGINTD n_l);
/* Renvoie le nbre de digits de n_l */

extern bigint *lsdptr_l(BIGINTD n_l);
/* Renvoie un pointeur vers le digit le moins significatif de n_l */

extern bigint *msdptr_l(BIGINTD n_l);
/* Renvoie un pointeur vers le digit le plus significatif de n_l */

extern void setdigits_l(BIGINTD n_l, USHORT l);
/* Fixe le nombre de digits de n_l à l */

extern void decdigits_l(BIGINTD n_l);
/* Diminue de 1 le nombre de digits de n_l */

extern void incdigits_l(BIGINTD n_l);
/* Augmente de 1 le nombre de digits de n_l */

extern void rmldzeros_l(BIGINTD n_l);
/* Enlève les 0 devant un BIGINTD */


/* Prototypes des fonctions d'initialisation et de comparaison */

extern void copy_l(BIGINTM dest_l, BIGINTM src_l);
/* Copie les contenu de src_l dans dest_l */

extern void setzero_l(BIGINTM n_l);
/* Fixe à 0 la valeur de n_l */

extern void setone_l(BIGINTM n_l);
/* Fixe à 1 la valeur de n_l */

extern void settwo_l(BIGINT n_l);
/* Fixe à 2 la valeur de n_l */

extern void setmax_l(BIGINTD n_l);
/* Fixe à 2^4096 - 1 la valeur de n_l */

extern int comp_l(BIGINTM a_l, BIGINTM b_l);
/* Compare a_l et b_l, renvoie 1 si a_l>b_l, -1 si a_l<b_l et 0 si a_l = b_l */

extern int equ_l(BIGINTM a_l, BIGINTM b_l);
/* Renvoie 1 si a_l = b_l et 0 sinon */

extern int eqz_l(BIGINTM a_l);
/* Renvoie 1 si a_l = 0 et 0 sinon */

extern int eq1_l(BIGINT a_l);
/* Renvoie 1 si a_l = 1 et 0 sinon */

extern int ge_l(BIGINTM a_l, BIGINTM b_l);
/* Renvoie 1 si a_l>=b_l et 0 sinon */

extern int gtzero_l(BIGINTM a_l);
/* Renvoie 1 si a_l>0 et 0 sinon */

extern int iseven_l(BIGINT n_l);
/* Renvoie 1 si n_l est pair et 0 sinon */


/* Prototypes des fonctions arithmétiques élémentaires */

extern void u2bigint_l(BIGINT u_l, USHORT u);
/* Convertit u en u_l */

extern void ul2bigint_l(BIGINT u_l, ULONG ul);
/* Convertit ul en u_l */

extern int add_l(BIGINT aa_l, BIGINT bb_l, BIGINT s_l);
/* Additionne aa_l et bb_l dans s_l, renvoie BIGINT_OFL si overflow (s_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int uadd_l(BIGINT a_l, USHORT b, BIGINT s_l);
/* Additionne a_l et b dans s_l, renvoie BIGINT_OFL si overflow (s_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int sub_l(BIGINT aa_l, BIGINT bb_l, BIGINT d_l);
/* Soustrait aa_l et bb_l dans d_l, renvoie BIGINT_UFL si underflow (d_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int usub_l(BIGINT a_l, USHORT b, BIGINT d_l);
/* Soustrait a_l et b dans d_l, renvoie BIGINT_UFL si underflow (d_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int sadd(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP s_l);
/* Additionne a_l et b_l en tenant compte des signes asign et bsign  */

extern int ssub(BIGINT a_l, int asign, BIGINT b_l, int bsign, BIGINTP d_l);
/* Soustrait a_l et b_l en tenant compte des signes asign et bsign  */

extern int mulclass_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l);
/* Multiplie aa_l et bb_l dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int umul_l(BIGINT aa_l, USHORT b, BIGINT p_l);
/* Multiplie aa_l et b dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int sqrclass_l(BIGINT aa_l, BIGINT p_l);
/* Elève aa_l au carré dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int div_l(BIGINTM aa_l, BIGINT bb_l, BIGINTM quot_l, BIGINT rem_l);
/* Divise aa_l par bb_l dans quot_l, le reste est dans rem_l, renvoie BIGINT_DBZ si bb_l = 0 et BIGINT_OK sinon */

extern int udiv_l(BIGINT aa_l, USHORT b, BIGINT quot_l, BIGINT rem_l);
/* Divise aa_l par b dans quot_l, le reste est dans rem_l, renvoie BIGINT_DBZ si b = 0 et BIGINT_OK sinon */

extern int mod_l(BIGINTM a_l, BIGINT b_l, BIGINT rem_l);
/* Donne le reste de la division de  a_l par b_l dans rem_l, renvoie BIGINT_DBZ si bb_l = 0 et BIGINT_OK sinon */

extern int mod2_l(BIGINT aa_l, ULONG k, BIGINT rem_l);
/* Donne le reste de la division de aa_l par 2^k dans rem_l, renvoie BIGINT_OK */
extern USHORT umod_l(BIGINT aa_l, USHORT b);
/* Renvoie le reste de la division de aa_l par b et 0xffff sinon */

extern int smod(BIGINT a_l, int asign, BIGINT b_l, BIGINT rem_l);
/* Donne le reste de a_l par b_l avec le signe asign, renvoie BIGINT_DBZ si b_l = 0 et BIGINT_OK sinon */


/* Prototypes des fonctions pour Karatsuba */

extern int mulkar_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l);
/* Multiplie aa_l et bb_l par Karatsuba dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */

extern int sqrkar_l(BIGINT aa_l, BIGINT p_l);
/* Elève aa_l au carré par Karatsuba dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */


/* Prototype de la fonction de multiplication optimisée */

extern int mul_l(BIGINT aa_l, BIGINT bb_l, BIGINT p_l);
/* Multiplie aa_l et bb_l dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */


/* Prototype de la fonction de carré optimisée */

extern int sqr_l(BIGINT aa_l, BIGINT p_l);
/* Elève aa_l au carré dans p_l, renvoie BIGINT_OFL si overflow (p_l est réduit mod 2^4096) et BIGINT_OK sinon */


/* Prototypes des fonctions arithmétiques modulo */

extern int addmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT s_l);
/* Additionne aa_l et bb_l mod mo_l dans s_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int submod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT d_l);
/* Soustrait aa_l et bb_l mod mo_l dans d_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int mulmod_l(BIGINT aa_l, BIGINT bb_l, BIGINT mo_l, BIGINT p_l);
/* Multiplie aa_l et bb_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int sqrmod_l(BIGINT aa_l, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l au carré mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int uaddmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT s_l);
/* Additionne a_l et b mod mo_l dans s_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int usubmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT d_l);
/* Soustrait a_l et b mod mo_l dans d_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int umulmod_l(BIGINT a_l, USHORT b, BIGINT mo_l, BIGINT p_l);
/* Multiplie a_l et b mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int eqmod_l(BIGINT a_l, BIGINT b_l, BIGINT mo_l);
/* Renvoie 1 si a_l = b_l mod mo_l et 0 sinon */


/* Prototypes des fonctions exponentielles */

extern int uexpmod_l(BIGINT aa_l, USHORT e, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par e mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int wexpmod_l(USHORT a, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève a par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int expmodk_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0, BIGINT_MAL si erreur de malloc() et BIGINT_OK sinon */

extern int expmod7_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */

extern int expmod2k_l(BIGINT aa_l, USHORT k, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par 2^k mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0 et BIGINT_OK sinon */


/* Prototypes des fonctions opérant sur les bits */

extern unsigned int numbits_l(BIGINT aa_l);
/* Renvoie le nombre de bits significatifs de aa_l */

extern int shleft_l(BIGINT a_l);
/* Shifte a_l vers la gauche, renvoie BIGINT_OFL si overflow et BIGINT_OK sinon */


extern int shright_l(BIGINT a_l);
/* Shifte a_l vers la droite, renvoie BIGINT_UFL si underflow et BIGINT_OK sinon */

extern int shift_l(BIGINT a_l, long int nbrebits);
/* Shifte a_l vers la gauche ou droite suivant nbrebits, renvoie BIGINT_OFL si overflow, BIGINT_UFL si underflow et BIGINT_OK sinon */

extern void and_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
/* Donne le Et logique entre a_l et b_l dans c_l */

extern void or_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
/* Donne le Ou logique entre a_l et b_l dans c_l */

extern void xor_l(BIGINT a_l, BIGINT b_l, BIGINT c_l);
/* Donne le Ou exclusif logique entre a_l et b_l dans c_l */

extern int setbit_l(BIGINT a_l, unsigned int pos);
/* Met à 1 la valeur du bit à la position pos, renvoie 1 si le bit était déjà à 1, 0 si le bit était à 0 et BIGINT_OFL si overflow */

extern int testbit_l(BIGINT a_l, unsigned int pos);
/* Renvoie 1 si le bit à la position pos était à 1 et 0 sinon */

extern int clearbit_l(BIGINT a_l, unsigned int pos);
/* Mat à 0 la valeur du bit à la position pos, renvoie 1 si le bit était à 1 et 0 sinon */


/* Prototypes des fonctions d'entrée-sortie */

extern int str2bigint_l(BIGINT n_l, char *str, USHORT b);
/* Convertit une chaîne de caractères et un BIGINT, renvoie BIGINT_BOR si la base est mal choisie, BIGINT_NPT si le pointeur NULL est passé, BIGINT_OFL si overflow et BIGINT_OK sinon */

extern char *bigint2str_l(BIGINT n_l, USHORT b, int showb);
/* Convertit un BIGINT en une chaîne de caractères, renvoie un pointeur vers la chaîne */

extern int byte2bigint_l(BIGINT n_l, UCHAR* bytestr, int len);
/* Convertit une chaîne de bytes en un BIGINT, renvoie BIGINT_NPT si le pointeur NULL est passé, BIGINT_OFL si overflow et BIGINT_OK sinon */

extern UCHAR *bigint2byte_l(BIGINT n_l, int *len);
/* Convertit un BIGINT en une chaîne de bytes, renvoie un pointeur vers la chaîne */ 

extern int check_l(BIGINT n_l);
/* Teste le format numérique de n_l, renvoie CHECK_LDZ si il y a des 0 devant, CHECK_MEM si le pointeur NULL est passé, CHECK_OFL si overflow et CHECK_OK sinon */


/* Prototypes des fonctions de théorie des nombres */

extern void gcd_l(BIGINT aa_l, BIGINT bb_l, BIGINT g_l);
/* Calcule le pgcd entre aa_l et bb_l dans g_l */

extern int lcm_l(BIGINT a_l, BIGINT b_l, BIGINT l_l);
/* Calcule le ppcm entre a_l et b_l dans l_l, renvoie BIGINT_OFL si overflow et BIGINT_OK sinon */

extern void xgcd_l(BIGINT a_l, BIGINT b_l, BIGINT g_l, BIGINT u_l, int *usign, BIGINT v_l, int *vsign);
/* Calcule le pgcd entre a_l et b_l dans g_l et trouve les coefficients u_l et v_l tels que g_l = u_l*a_l + v_l*b_l */

extern void invmod_l(BIGINT a_l, BIGINT mo_l, BIGINT g_l, BIGINT i_l);
/* Calcule l'inverse de a_l mod mo_l dans i_l avec le pgcd g_l, renvoie i_l = 0 si g_l != 1 */

extern void introot_l(BIGINT n_l, BIGINT r_l);
/* Donne la partie entière de la racine carrée de n_l */

extern ULONG uintroot_l(ULONG n);
/* Donne la partie entière de la racine carrée de n */

extern unsigned int issqr_l(BIGINT n_l, BIGINT r_l);
/* Renvoie 1 si n_l est un carré parfait et 0 sinon */

extern int jacobi_l(BIGINT aa_l, BIGINT bb_l);
/* Calcule le symbole de Jacobi de a_l et p_l dans x_l */

extern int proot_l(BIGINT a_l, BIGINT p_l, BIGINT x_l);
/* Calcule la racine carrée de a_l mod p_l dans x_l, renvoie 0 si a_l est un résidu quadratique mod p_l et -1 sinon */

extern int pqroot_l(BIGINT a_l, BIGINT p_l, BIGINT q_l, BIGINT x_l);
/* Calcule la racine carrée de a_l mod p_l*q_l dans x_l, renvoie 0 si a_l est un résidu quadratique mod p_l*q_l et -1 sinon */

extern int chinrem_l(unsigned int numeq, bigint **coeff_l, BIGINT x_l);
/* Donne la solution d'un système de congruences dans x_l, renvoie BIGINT_OFL si overflow, 1 si numeq est 0, 2 si les modulos ne sont pas premiers relatifs et BIGINT_OK sinon */

extern int primelt_l(BIGINT a_l, unsigned int numsprimes, bigint *primes_l[]);
/* Génère un élément primitif modulo p (p>2) dans a_l, renvoie -1 si p pas premier supérieur à 2 et BIGINT_OK sinon */

/* Prototypes des fonctions de test de primalité */

extern ULONG *primesgen(ULONG N);
/* Renvoie un pointeur vers un vecteur de nombre premiers <= N et NULL si erreur avec malloc() */

extern USHORT sieve_l(BIGINT a_l, unsigned int numsprimes);
/* Revoie un facteur premier ou 1 si a_l est premier et 0 si aucun facteur n'est trouvé */

extern int MRprime_l(BIGINT n_l, unsigned int numsprimes, unsigned int iter);
/* Test de primalité Miller-Rabin, renvoie 1 si n_l est probablement premier et 0 sinon */

extern int SSprime_l(BIGINT n_l, unsigned int numsprimes, unsigned iter);
/* Test de primalité Solovay-Strassen, renvoie 1 si n_l est probablement premier et 0 sinon */


/* Prototypes des fonctions de génération aléatoire */

extern bigint *rand64_l(void);
/* Générateur de congruence linéaire, renvoie un pointeur vers SEED64 */

extern bigint *seed64_l(BIGINT seed_l);
/* Fixe une valeur initiale pour rand_l, renvoie un pointeur vers BUFF64 */

extern bigint *ulseed64_l(ULONG seed);
/* Fixe une valeur initiale pour rand_l, renvoie un pointeur vers BUFF64 */

extern UCHAR ucrand64_l(void);
/* Renvoie un nombre aléatoire UCHAR */

extern USHORT usrand64_l(void);
/* Renvoie un nombre aléatoire USHORT */

extern ULONG ulrand64_l(void);
/* Renvoie un nombre aléatoire ULONG */

extern void rand_l(BIGINT r_l, USHORT l);
/* Renvoie un nombre aléatoire dans l'intervalle [2^(l-1),2^l-1] */

extern USHORT randbit_l(void);
/* Génère un bit aléatoire avec Blum-Blum-Shub */

extern int seedBBS_l(BIGINT seed_l);
/* Fixe une valeur initiale pour randbit_l, renvoie -1 si seed_l et MODBBS ne sont pas premiers relatifs et 0 sinon */

extern int ulseedBBS_l(ULONG seed);
/* Fixe une valeur initiale pour randbit_l, renvoie -1 si seed et MODBBS ne sont pas premiers relatifs et 0 sinon */

extern USHORT usrandBBS_l(void);
/* Renvoie un nombre aléatoire USHORT */

extern ULONG ulrandBBS_l(void);
/* Renvoie un nombre aléatoire ULONG */

extern UCHAR ucrandBBS_l(void);
/* Renvoie un nombre aléatoire UCHAR */

extern void randBBS_l(BIGINT r_l, USHORT l);
/* Renvoie un nombre aléatoire dans l'intervalle [2^(l-1),2^l-1] */


/* Prototypes des fonctions de Montgomery */

extern void mulmon_l(BIGINT a_l, BIGINT b_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l);
/* Effectue le produit de Montgomery de a_l et b_l dans p_l */

extern void sqrmon_l(BIGINT a_l, BIGINT n_l, USHORT np, USHORT l, BIGINT p_l);
/* Effectue la mise au carré de Montgomery de a_l */

extern USHORT invmon_l(BIGINT n_l);
/* Calcule l'inverse de Montgomery */

extern int uexpmodmon_l(BIGINT aa_l, USHORT e, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par e mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0, BIGINT_MOD si mo_l est pair et BIGINT_OK sinon */

extern int wexpmodmon_l(USHORT a, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève a par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0, BIGINT_MOD si mo_l est pair et BIGINT_OK sinon */

extern int expmodmonk_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0, BIGINT_MAL si erreur malloc(), BIGINT_MOD si mo_l est pair et BIGINT_OK sinon */


/* Prototype de la fonction exponentielle optimisée */

extern int expmod_l(BIGINT aa_l, BIGINT ee_l, BIGINT mo_l, BIGINT p_l);
/* Elève aa_l par ee_l mod mo_l dans p_l, renvoie BIGINT_DBZ si mo_l = 0, BIGINT_MAL si erreur malloc() et BIGINT_OK sinon */


/* Prototypes des fonctions de purge */

extern void purge_l(BIGINT n_l);
/* Remplit de 0 n_l */




/* Fin de libarithm.h */


#endif
