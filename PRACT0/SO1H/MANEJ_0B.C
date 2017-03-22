/* ----------------------------------------------------------------------- */
/*                               manej_0b.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 0b         */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX = 0x0b00                                                             */
/* ----------------------------------------------------------------------- */

#include "..\so1hpub.h\tipos.h"
#include "..\so1hpub.h\printvid.h"
#include "..\so1h.h\procesos.h"
#include "..\so1h.h\blockpr.h"


void so1_manejador_0b ( void ) {                     /* ah = 0b ; int SO1H */

    printStrVideo("\n\n estoy en so1_manejador_0b  tramaThread->AX = ") ;
    printHexVideo(tramaThread->AH, 4) ;

    switch (tramaThread->AL) {

        case 0x00 :                             
		            break ;
        case 0x01 : 
		            bloquearThreadActual(rec_hijo) ;            
		            break ;
        case 0x02 : 
                    descThread[indThreadActual].trama = tramaThread ;
                    descThread[indThreadActual].estado = preparado ;
					encolarPC2c(indThreadActual, (ptrC2c_t)&c2cPFR[TPreparados]) ;
					buscarNuevoThreadActual() ;
		            break ;
        case 0x03 :                                      
		            break ;
        default : ;

  }

}
