/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                                ajust_0.h                                */
/* ----------------------------------------------------------------------- */
/*            Ajuste inicial del modelo de programacion de so1h            */
/* ----------------------------------------------------------------------- */

#ifndef AJUST_0_H
#define AJUST_0_H

#include <so1hpub.h\tipos.h>

void startBin ( void ) ;          /* punto de entrada para so1h.bin (boot) */

void startExe ( void ) ;               /* punto de entrada para so1h_0.exe */

#ifndef _DOS

void __start__ ( void ) ;                         /* se llama desde _start */

#endif

modoSO1_t modoSO1 ( void ) ;     /* modo en que se ejecuta SO1H: bin o exe */

byte_t unidadBIOS ( void ) ;       /* numero BIOS de la unidad de arranque */

word_t CSInicial ( void ) ;                                   /* CSInicial */

extern word_t CS_SO1H ;              /* segmento de codigo (text)  de SO1H */

extern word_t RO_SO1H ;              /* segmento de datos (rodata) de SO1H */

extern word_t DS_SO1H ;              /* segmento de datos (data)   de SO1H */

extern word_t BSS_SO1H ;             /* segmento de datos (bss)    de SO1H */

extern word_t SS_SO1H ;              /* segmento de pila           de SO1H */

extern word_t IMRInicial ;     /* mascara de interrupcion inicial del 8259 */

void obtenerMapa ( void ) ;         /* obtiene CS_SO1H, DS_SO1H y BSS_SO1H */

#endif /* AJUSTES_H */



