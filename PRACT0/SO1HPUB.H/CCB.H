/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                                  ccb.h                                  */
/* ----------------------------------------------------------------------- */
/*                     colas de funciones de call back                     */
/* ----------------------------------------------------------------------- */

#ifndef CCB_H
#define CCB_H

#include <so1hpub.h\tipos.h>

typedef int ( * callBack_t ) ( void * arg ) ;                 /* call back */

#define cbNulo (callBack_t)0x00000000

typedef struct
{
    word_t num ;                               /* numero actual de callBacks */
    word_t in ;                                /* indice del primer callBack */
    word_t out ;                    /* indice de la primera componente libre */
    word_t max ;                               /* numero m�ximo de callBacks */
    void * arg ;                                       /* argumento devuelto */
    callBack_t callBack [10] ;                /* array circular de callBacks */
//callBack_t callBack [0xFFFF] ;            /* array circular de callBacks */
//callBack_t callBack [ ] ;                 /* array circular de callBacks */
//callBack_t * callBack ;                   /* array circular de callBacks */
} descCcb_t ;

typedef descCcb_t * ccb_t ;

#define ccbNulo (ccb_t)0x00000000

#define tamCcb (sizeof(descCcb_t) + max * sizeof(callBack_t))

int inicCcb ( ccb_t ccb, word_t max ) ;

int encolarCcb ( callBack_t cb, ccb_t ccb ) ;

callBack_t desencolarCcb ( ccb_t ccb ) ;

int eliminarCcb ( callBack_t cb, ccb_t ccb ) ;

int eliminarSegCcb ( word_t segmento, ccb_t ccb ) ;

int vaciarCcb ( ccb_t ccb ) ;

int atenderCcb ( ccb_t ccb ) ;

#endif /* CCB_H */

