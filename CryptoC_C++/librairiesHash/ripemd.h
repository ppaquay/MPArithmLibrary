/***********************************************************************
 *                                                                     *
 *     Header pour la librairie de fonctions de hachage RIPEMD160      *
 *     en multi-précision (4096 bits si ARCH32 et 8192 si ARCH64)      *
 *                                                                     *
 *                                                                     *
 *                             Pierre Paquay                           *
 *                                                                     *
 ***********************************************************************/

#ifndef ripemd_H
#define ripemd_H

#include "libmparithm.h"

#define RMDVER 160
#define BIGINT_RMD -20

#ifdef ARCH32
struct ripemd_stat {
  unsigned long stomach[5];
  unsigned long total[2];
};

typedef struct ripemd_stat RMDSTAT;

extern void ripeinit(RMDSTAT*);
extern int ripehash(RMDSTAT*, UCHAR*, ULONG);
extern UCHAR* ripemd160(UCHAR*, ULONG);
#endif

#ifdef ARCH64
struct ripemd_stat {
  unsigned int stomach[5];
  unsigned int total[2];
};

typedef struct ripemd_stat RMDSTAT;

extern void ripeinit(RMDSTAT*);
extern int ripehash(RMDSTAT*, UCHAR*, unsigned int);
extern UCHAR* ripemd160(UCHAR*, unsigned int);
#endif


/* Fin de ripemd.h */

#endif
