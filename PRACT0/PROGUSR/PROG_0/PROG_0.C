/* ----------------------------------------------------------------------- */
/*                                 prog_0.c                                */
/* ----------------------------------------------------------------------- */
/*                      un primer programa de usuario                      */
/* ----------------------------------------------------------------------- */

#include <so1hpub.h\ll_s_thr.h>

int main ( void )
{
	while (TRUE) ;
	asm(" dd 0x12345675") ;
    return(thread_self()) ;
}

