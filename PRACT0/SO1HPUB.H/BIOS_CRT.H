/* ----------------------------------------------------------------------- */
/*                                bios_crt.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOS_CRT_H
#define BIOS_CRT_H

#include <so1hpub.h\tipos.h>
#include <so1hpub.h\pantalla.h>

typedef enum
{
    _NONE, _MDA, _CGA,
    _EGACOLOR = 0x04, _EGAMONO,
    _VGACOLOR = 0x07, _VGAMONO,
    _MCGACOLOR = 0x0a, _MCGAMONO,
    _MDS_GENIUS = 0x30
} tipoAdaptador_t ;

typedef enum
{
    _FONT08 =  8,
    _FONT14 = 14,
    _FONT15,                                                       /* GENIUS */
    _FONT16
} fontSize_t ;

extern byte_t numFilas ;

extern byte_t numColumnas ;

extern pantalla_t * ptrPant ;

extern tipoAdaptador_t tipoAdaptador ;

extern bool_t redimensionable ;

extern byte_t fontSize ;

extern byte_t visibilidad ;        /* 0 invisible, 1 por defecto, 2 grande */

void inicBiosCrt ( void ) ;

void finBiosCrt ( void ) ;

fontSize_t getFontSize ( void ) ;

byte_t getFilas ( void ) ;

int comprobarAdaptador ( tipoAdaptador_t tipoAdaptador ) ;

tipoAdaptador_t tipoDeAdaptador ( void ) ;

byte_t setCursorVisibilidad ( int valorDeVisibilidad ) ;

void setFontSize ( fontSize_t size ) ;

void set25x80 ( void ) ;

void redimensionar ( byte_t nFilas, byte_t nCols ) ;

#endif /* BIOS_CRT_H */
