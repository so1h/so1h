/* ----------------------------------------------------------------------- */
/*                                matrix.c                                 */
/* ----------------------------------------------------------------------- */
/*                     numero de matricula del alumno                      */
/* ----------------------------------------------------------------------- */

#include <so1h.h\matrix.h>                                /* NUM_MATRICULA */

void matrix ( void )
{
    asm
    (
        " times NUM_MATRICULA dw NUM_MATRICULA"
    ) ;
}

