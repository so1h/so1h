/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                               llamadas.h                                */
/* ----------------------------------------------------------------------- */
/*                           llamadas al sistema                           */
/* ----------------------------------------------------------------------- */

#ifndef LLAMADAS_H
#define LLAMADAS_H

#define nVIntSO1H    0x60            /* numero de vector utilizado por SO1 */

void isr_SO1H ( void ) ;

void link_llamadas ( void ) ;

#endif /* LLAMADAS_H */

