/* ----------------------------------------------------------------------- */
/*                               pantalla.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1hpub.h\pantalla.h>

word_t tamPantalla ( byte_t numFilas )
{
    return(numFilas*maxColumnas*sizeof(pos_t)) ;
}

void pantallazo ( pantalla_t * pantalla,
                  word_t numFilas,
                  char car, char atr,
                  byte_t esqSupDeF, byte_t esqSupDeC,
                  byte_t esqInfIzF, byte_t esqInfIzC )
{
    int F, C ;
    pos_t pos ;

    if (esqInfIzF >= numFilas) return ;

    if (atr != (char)0)                   /* si se modifican los atributos */
    {
        pos.car = car ;
        pos.atr = atr ;
        for ( F = esqSupDeF ; F <= esqInfIzF ; F++ )
            for ( C = esqSupDeC ; C <= esqInfIzC ; C++ )
                pantalla->t[F][C] = pos ;
    }
    else                                  /* no se modifican los atributos */
    {
        for ( F = esqSupDeF ; F <= esqInfIzF ; F++ )
            for ( C = esqSupDeC ; C <= esqInfIzC ; C++ )
                pantalla->t[F][C].car = car ;
    }
}

void borrarPantalla ( pantalla_t * pantalla,
                      word_t numFilas )
{
    pantallazo(pantalla, numFilas, ' ', atrNormal, 0, 0, numFilas-1, maxColumnas-1) ;
}

void borrarCPantalla ( pantalla_t * pantalla,
                       word_t numFilas )
{
    pantallazo(pantalla, numFilas, ' ', (char)0, 0, 0, numFilas-1, maxColumnas-1) ;
}

void copiarPantalla ( pantalla_t * p1,
                      pantalla_t * p2,
                      word_t numFilas )
{
    byte_t F, C ;
    for ( F = 0 ; F < numFilas ; F++ )
        for ( C = 0 ; C < maxColumnas ; C++ )
            p2->t[F][C] = p1->t[F][C] ;
}

void copiarCPantalla ( pantalla_t * p1,
                       pantalla_t * p2,
                       word_t numFilas )
{
    byte_t F, C ;
    for ( F = 0 ; F < numFilas ; F++ )
        for ( C = 0 ; C < maxColumnas ; C++ )
            p2->t[F][C].car = p1->t[F][C].car ;
}

void scrollPantalla ( pantalla_t * pantalla,
                      word_t numFilas )
{
    byte_t C ;
    pos_t pos ;
    pos.car = ' ' ;
    pos.atr = atrNormal ;

    numFilas-- ;

    copiarPantalla((pantalla_t *)&pantalla->t[1][0], pantalla, numFilas) ;

    for ( C = 0 ; C < maxColumnas ; C++ ) pantalla->t[numFilas][C] = pos ;

}

void scrollCPantalla ( pantalla_t * pantalla,
                       word_t numFilas )
{
    byte_t C ;

    numFilas-- ;

    copiarCPantalla((pantalla_t *)&pantalla->t[1][0], pantalla, numFilas) ;

    for ( C = 0 ; C < maxColumnas ; C++ ) pantalla->t[numFilas][C].car = ' ' ;

}

