/* ----------------------------------------------------------------------- */
/*                                memvideo.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef MEMVIDEO_H
#define MEMVIDEO_H

#include <so1hpub.h\tipos.h>
#include <so1hpub.h\pantalla.h>

extern byte_t cursorF ;

extern byte_t cursorC ;

extern byte_t lineaOriginal1 ;

extern byte_t lineaOriginal2 ;

void inicMemVideo ( void ) ;

int finMemVideo ( void ) ;

void goToXYVideo ( byte_t F, byte_t C ) ;

int printCarVideo ( char car ) ;

#endif /* MEMVIDEO_H */
