/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                               manej_04.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 04         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\copia.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\ll_s_so1.h>
#include <so1.h\dbgword.h>                                 /* debugWord */
#include <so1.h\ajustsp.h>
#include <so1.h\ajustes.h>
#include <so1.h\gm.h>                      /* listaLibres, tamBloqueMax */
#include <so1.h\procesos.h>
#include <so1.h\sf.h>
#include <so1.h\llamadas.h>

static void copiarDescProcesos ( bloquePFR_t far * ptrPFR ) {
  int i ;
  descProceso_t far * dP = ptrPFR->descProceso ;
  for ( i = 0 ; i < maxProcesos ; i++ ) {
    copia((pointer_t)&descProceso[i],
          (pointer_t)(&(dP[i])),
          sizeof(descProceso_t)
    ) ;
    if (descProceso[i].estado != libre)
      dP[i].c2cHijos.e = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2Hijos) ;
  }
}

static void copiarDescFicheros ( bloquePFR_t far * ptrPFR ) {
  int i ;
  descFichero_t far * dF = ptrPFR->descFichero ;
  for ( i = 0 ; i < dfsMax ; i++ )
    copia((pointer_t)&descFichero[i],
          (pointer_t)(&(dF[i])),
          sizeof(descFichero_t)
    ) ;
}

static void copiarDescRecursos ( bloquePFR_t far * ptrPFR ) {
  int i ;
  descRecurso_t far * dR = ptrPFR->descRecurso ;
  for ( i = 0 ; i < maxRecursos ; i++ ) {
    copia((pointer_t)&descRecurso[i],
          (pointer_t)(&(dR[i])),
          sizeof(descRecurso_t)
    ) ;
    if (descRecurso[i].tipo != rLibre)
      dR[i].c2cFichRec.e = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2FichRec) ;
  }
}

static void transferirBPFR ( bloquePFR_t far * ptrPFR, byte_t regAL ) {

  copia((pointer_t)&e2PFR, (pointer_t)(ptrPFR->e2PFR), sizeof(e2PFR_t)) ;
  copia((pointer_t)&c2cPFR, (pointer_t)(ptrPFR->c2cPFR), numColasPFR*sizeof(c2c_t)) ;

  copiarDescProcesos(ptrPFR) ;                           /* requiere e2PFR */
  if (regAL == 0x02) {
    copiarDescFicheros(ptrPFR) ;                         /* requiere e2PFR */
    copiarDescRecursos(ptrPFR) ;                         /* requiere e2PFR */
  }

  ptrPFR->c2cPFR[DPLibres].e    = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescProceso) ;
  ptrPFR->c2cPFR[DPOcupados].e  = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescProceso) ;
  ptrPFR->c2cPFR[PPreparados].e = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2Preparados) ;
  ptrPFR->c2cPFR[PUrgentes].e   = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2Urgentes) ;
  ptrPFR->c2cPFR[POrdenados].e  = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2POrdenados) ;
  ptrPFR->c2cPFR[PDormidos].e   = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2PDormidos) ;
  ptrPFR->c2cPFR[DFLibres].e    = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescFichero) ;
  ptrPFR->c2cPFR[DFOcupados].e  = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescFichero) ;
  ptrPFR->c2cPFR[DRLibres].e    = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescRecurso) ;
  ptrPFR->c2cPFR[DROcupados].e  = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2DescRecurso) ;

  if (regAL == 0x01) {
    tramaProceso->AX = seg((pointer_t)listaLibres) ;
    tramaProceso->DX = tamBloqueMax ;
  }
}

void so1_manejador_03 ( void ) {                       /* ah = 3 ; int SO1 */

  switch (tramaProceso->AL) {

    case 0x00 :                                /* obtenInfoPS  *//* 0x0400 */
    case 0x01 :                                /* obtenInfoMEM *//* 0x0401 */
    case 0x02 : {                              /* obtenInfoPFR *//* 0x0402 */
      bloquePFR_t far * ptrPFR ;
      ptrPFR = (bloquePFR_t far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
      transferirBPFR(ptrPFR, tramaProceso->AL) ;
      break ;
    }

    case 0x03 : {                                                /* 0x0403 */
      descriptor_de_fichero_t far * df ;
      df = (descriptor_de_fichero_t far *)                 /* obtenInfoFAB */
              pointer(tramaProceso->ES, tramaProceso->BX) ;
      if (df != (descriptor_de_fichero_t far *)NULL)
        copia((pointer_t)&tablaFichAbiertos,
              (pointer_t)df,
              sizeof(tablaFichAbiertos)) ;
      else
        inicTablaFichAbiertos() ;             /* borramos la tabla de f.a. */
      break ;
    }

    case 0x04 : {                                                /* 0x0404 */
      info_t far * info ;
      cabecera_t far * cabecera ;
      cabecera = (cabecera_t far *)pointer(CS_SO1, desplCab()) ;
      info = (info_t far *)                               /* obtenInfoINFO */
              pointer(tramaProceso->ES, tramaProceso->BX) ;
/*    info->ticsPorRodaja = ticsPorRodaja ; */
      info->SP0 = cabecera->SP0 ;               /* info->SP0 = SPInicial ; */
      info->SP0_So1 = SP0_SO1 ;
      info->IMR = IMRInicial ;
      info->modoSO1 = modoSO1() ;
      info->ptrDebugWord = (word_t far *)pointer(_DS, (word_t)&debugWord) ;
      break ;
    }

    case 0x05 :                                                  /* 0x0405 */
      tramaProceso->AX = (word_t)unidadLogicaActual ;           /* getdisk */
      break ;

#if (FALSE)
    case 0x05 :                                                  /* 0x0405 */
      tramaProceso->AX = (word_t)                     /* unidadLogicaLista */
        unidadLogicaListaSo1(tramaProceso->BL) ;
      break ;
#endif

    case 0x06 :                                                  /* 0x0406 */
      tramaProceso->AX = (word_t)
        findFirstSo1(                                      /* findFirstSo1 */
          tramaProceso->BL,
          (ffblk_t far *)(pointer(tramaProceso->DS, tramaProceso->DX))
        ) ;
      break ;

    case 0x07 :                                                  /* 0x0407 */
      tramaProceso->AX = (word_t)
        findNextSo1(                                        /* findNextSo1 */
          (ffblk_t far *)(pointer(tramaProceso->DS, tramaProceso->DX))
        ) ;
      break ;

    default: ;

  }
}
