/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                                def_proc.h                               */
/* ----------------------------------------------------------------------- */
/*                      Gestion de procesos y threads                      */
/* ----------------------------------------------------------------------- */

#ifndef DEF_PROC_H
#define DEF_PROC_H

#include <so1hpub.h\tipos.h>
#include <so1hpub.h\const.h>            /* maxProcesos, dfMax, maxRecursos */
#include <so1hpub.h\fcntl.h>                                   /* modoAp_t */
#include <so1hpub.h\def_trm.h>                                  /* trama_t */
#include <so1hpub.h\def_mem.h>
#include <so1hpub.h\ptrc2c.h>
#include <so1hpub.h\ccb.h>

#define STDIN  0                                       /* entrada estandar */
#define STDOUT 1                                       /* salida estandar  */
#define STDERR 2                                       /* error estandar   */

#define SEEK_SET   0           /* lseek posiciona desde el principio       */
#define SEEK_CUR   1           /* lseek posiciona desde la posicion actual */
#define SEEK_END   2           /* lseek posiciona desde el final           */

typedef enum
{
    libre, preparado, ejecutandose, bloqueado        /* estados de un thread */
} estado_t ;

typedef struct
{
    modoAp_t modoAp ;                                    /* modo de apertura */
    dfs_t    dfs ;                      /* descriptor de fichero del sistema */
    dword_t  pos ;                                               /* posicion */
} dfa_t ;                  /* descriptor de fichero abierto por un proceso */

#define camposPubThread        /* campos publicos del descriptor de thread */\
  tid_t        tid ;                                                         \
  estado_t     estado ;                                                      \
  rindx_t      esperandoPor ;      /* recurso por el que se esta esperando */\
  trama_t      * trama ;                                  /* trama = SS:SP */\
  bool_t       noStatus ;              /* TRUE => puede morir directamente */\
  int          status ;  /* estado de terminacion si queda zombie (thread) */\
  tindx_t      ptindx ;                          /* tindx del thread padre */\
  tindx_t      htindx ;   /* tindx del thread por el que se esta esperando */\
  pindx_t      pindx ;               /* pindx del proceso al que pertenece */\
                                                         /*----Memoria---- */\
  word_t       SSThread ;                              /* segmento de pila */\
  word_t       SP0 ;                            /* puntero de pila inicial */\
                                                                             \
///* dword_t      tCPU ; */   /* tiempo de CPU consumido (en tics*(2**16)) */

#define camposPubProceso      /* campos publicos del descriptor de proceso */\
                                                         /*------CPU------ */\
  pid_t        pid ;                                                         \
  bool_t       noStatus ;              /* TRUE => puede morir directamente */\
  int          status ;           /* estado de terminacion si queda zombie */\
  pindx_t      ppindx ;                         /* pindx del proceso padre */\
  pindx_t      hpindx ;     /* pindx del hijo por el que se esta esperando */\
  c2c_t        c2cHijos ;                        /* cola de procesos hijos */\
                                                              /* (threads) */\
  c2c_t        c2cThreads ;                             /* cola de threads */\
                                                         /*----Memoria---- */\
  word_t       CSProc ;     /* segmento de codigo = inicio de la particion */\
  word_t       tamCodigo ;                   /* tama�o del codigo en bytes */\
  word_t       desplBSS ;               /* CS:desplBSS primer byte del BSS */\
  word_t       desplPila ;            /* Pila: DS:desplPila a DS:SPInicial */\
  word_t       tam ;              /* tamanio de la particion en paragrafos */\
  dword_t      tamFichero ;      /* tamanio del fichero ejecutable cargado */\
  char         programa[12] ;     /* nombre del fichero ejecutable cargado */\
  char         comando[tamComando] ;                            /* comando */\
                                                         /*------E/S------ */\
  word_t       nfa ;         /* numero de ficheros abiertos por el proceso */\
  dfa_t        tfa [dfMax] ;   /* tabla de ficheros abiertos (del proceso) */\
                                                                             \
  uid_t        uid ;                                                         \
  gid_t        gid ;                                                         \
                                                                             \
///* dword_t      tCPU ; */   /* tiempo de CPU consumido (en tics*(2**16)) */

typedef struct
{
    camposPubProceso
} descProceso_t ;                                 /* descriptor de proceso */

typedef struct
{
    camposPubThread
} descThread_t ;                                   /* descriptor de thread */

typedef enum
{
    flibre, fRegular, fedBloques, fedCaracteres, tuberia
} tipoFichero_t ;

typedef struct
{
    tipoFichero_t  tipo ;
    char           nombre[9] ;
    rindx_t        rindx ;
    int            menor ;                                   /* numero menor */
    word_t         shareMode ;
    int            contAp_L ;                    /* contador de aperturas L  */
    int            contAp_E ;                    /* contador de aperturas E  */
} descFichero_t ;

typedef enum
{
    rLibre,
    rDCaracteres, rDBloques,
    rTuberia,
    rGP,
    rGM,
    rSF,
    rOtro
} tipoRecurso_t ;

typedef int      ( * open_t ) ( int dfs, modoAp_t modo ) ;
typedef int      ( * release_t ) ( int dfs ) ;
typedef int      ( * read_t ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      ( * aio_read_t ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      ( * write_t ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      ( * aio_write_t ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef long int ( * lseek_t ) ( int dfs, long int pos, word_t whence ) ;
typedef int      ( * fcntl_t ) ( int dfs, word_t cmd, word_t arg ) ;
typedef int      ( * ioctl_t ) ( int dfs, word_t request, word_t arg ) ;

typedef int      ( * eliminar_t ) ( pindx_t pindx ) ;

typedef struct
{

    char          nombre[12] ;                         /* nombre del recurso */
    tipoRecurso_t tipo ;
    ccb_t         ccb ;                    /* cola de funciones de call back */
    tindx_t       tindx ;                 /* identificador del thread driver */
    c2c_t         c2cFichRec ;    /* cola de ficheros que dan paso al driver */
    byte_t        numVI ;             /* numero de vectores de int. 0, 1 o 2 */
    byte_t        nVInt[2] ;           /* tabla de numeros de vector de int. */
    byte_t        irq[2] ;  /* tabla irqs. 0 <= irq[i] < 16 => int. hardware */
    isr_t         isr[2] ;   /* tabla de rutinas de servicio de interrupcion */

    open_t        open ;
    release_t     release ;
    read_t        read ;
    aio_read_t    aio_read ;
    write_t       write ;
    aio_write_t   aio_write ;
    lseek_t       lseek ;
    fcntl_t       fcntl ;
    ioctl_t       ioctl ;

    eliminar_t    eliminar ;

} descRecurso_t ;

/* recursos (simples) por los que se espera bloqueado */

#define rec_hijo            (rindx_t)(- 1)       /* terminacion de un hijo */
#define rec_zombie          (rindx_t)(- 2)
#define rec_desinstalacion  (rindx_t)(- 3)
#define rec_thr_hijo        (rindx_t)(- 4)     /* terminacion de un thread */
#define rec_thr_zombie      (rindx_t)(- 5)     /* terminacion de un thread */

typedef struct                                      /* informacion variada */
{
    /* word_t ticsPorRodaja ; */
    word_t SP0 ;
    word_t SP0_So1 ;
    word_t IMR ;
    word_t modoSO1 ;                         /* modoSO1_(Cmd,Bin,Com,Exe,Bs) */
    word_t * ptrDebugWord ;         /* puntero a debugWord (so1.h\dbgword.h) */
} info_t ;

typedef struct
{
    word_t  signatura ;
    word_t  bytesUltSector ;
    word_t  sectores ;
    word_t  numDirReub ;
    word_t  numParCabecera ;
    word_t  minAlloc ;
    word_t  maxAlloc ;
    word_t  SS0 ;
    word_t  SP0 ;
    word_t  checkSum ;
    word_t  IP0 ;
    word_t  CS0 ;
    word_t  offTablaReub ;
    word_t  numOverlay ;
    dword_t relleno ;
} cabecera_t ;

typedef struct
{
    union
    {
        dobleEnlace_t Libres [ maxProcesos + 1 ] ;                     /* Pila */
        dobleEnlace_t Ocupados [maxProcesos + 2 ] ;         /* Pila + Eliminar */
    } e2DescProceso ;
    union
    {
        dobleEnlace_t Libres [ maxThreads + 1 ] ;                      /* Pila */
        dobleEnlace_t Ocupados [maxThreads + 2 ] ;          /* Pila + Eliminar */
    } e2DescThread ;
    union
    {
        dobleEnlace_t Libres [ dfsMax + 1 ] ;                             /* P */
        dobleEnlace_t Ocupados [ dfsMax + 2 ] ;                       /* P + E */
    } e2DescFichero ;
    union
    {
        dobleEnlace_t Libres [ maxRecursos + 1 ] ;                        /* P */
        dobleEnlace_t Ocupados [ maxRecursos + 2 ] ;                  /* P + E */
    } e2DescRecurso ;
    dobleEnlace_t e2Hijos [ maxProcesos + maxProcesos ] ;    /* P+E de hijos */
    dobleEnlace_t e2Threads [ maxThreads + maxProcesos ] ; /* P+E de threads */
    dobleEnlace_t e2Preparados [ maxThreads + 1 ] ;    /* Cola de preparados */  /* unir en una sola cola */
    dobleEnlace_t e2Urgentes [ maxThreads + 1 ] ;  /* Desbloqueados urgentes */  /* ver prepararDesbloqueadosUrgentes */
    dobleEnlace_t e2POrdenados [ maxProcesos + 1 ] ;          /* Particiones */
    dobleEnlace_t e2TDormidos [ maxThreads + 1 ] ;               /* Dormidos */
    dobleEnlace_t e2FichRec [ dfsMax + maxRecursos ] ;      /* Fich. de Rec. */
} e2PFR_t ;

typedef enum
{
    DPLibres, DPOcupados, DTLibres, DTOcupados, TPreparados, TUrgentes, POrdenados, TDormidos,
    DFLibres, DFOcupados,
    DRLibres, DROcupados,
    numColasPFR
} cPFR_t ;                       /* colas de Procesos, Ficheros y Recursos */ /* igual mejor cTPFR_t */

typedef tindx_t ( * sigThread_t ) ( ) ;

typedef void ( * activarThread_t ) ( tindx_t tindx ) ;

typedef void ( * buscarNuevoThreadActual_t ) ( void ) ;

typedef void ( * bloquearThreadActual_t ) ( rindx_t rindx ) ;

#define dirDescSO1H ((descSO1H_t *)&ptrIndProcesoActual)

/* en la siguiente macro (camposDescSO1H) modo = volatile, extern o static */            

#define camposDescSO1H(modo)                                                 \
modo   pindx_t                 * ptrIndProcesoActual ;                       \
modo   tindx_t                 * ptrIndThreadActual ;                        \
modo   trama_t *               * ptrTramaThread ;                            \
modo   bool_t                  * ptrActivarAlEpilogo ;                       \
modo   descProceso_t           * ptrDescProceso ;     /* tabla de procesos */\
modo   word_t                    tamDescProceso ;                            \
modo   descThread_t            * ptrDescThread ;       /* tabla de threads */\
modo   word_t                    tamDescThread ;                             \
modo   descFichero_t           * ptrDescFichero ;     /* tabla de ficheros */\
modo   descRecurso_t           * ptrDescRecurso ;     /* tabla de recursos */\
modo   c2c_t                   * ptrC2cPFR ;                                 \
modo   e2PFR_t                 * ptrE2PFR ;                                  \
modo   int                     * ptrNivelActivacionSO1H ;                    \
modo   bool_t                  * ptrEnHalt ;                                 \
modo   bool_t                  * ptrHayTic ;                                 \
modo   ccb_t                   * ptrCcbAlEpilogo ;                           \
modo   word_t                  * ptrSS_Thread ;                              \
modo   word_t                  * ptrSP_Thread ;                              \
modo   word_t                  * ptrSS_Kernel ;                              \
modo   word_t                  * ptrSP0_Kernel ;                             \
modo   word_t                    SP0_SO1H ;                                  \
modo   dword_t                 * ptrContRodajas ;                       /* */\
modo   int                     * ptrContTicsRodaja ;                    /* */\
modo   rti_t                   * ptrVIOrg ;                                  \
modo   sigThread_t               sigThread ;                                 \
modo   activarThread_t           activarThread ;                             \
modo   buscarNuevoThreadActual_t buscarNuevoThreadActual ;                   \
modo   bloquearThreadActual_t    bloquearThreadActual ;      /* no retorna */\
modo   ptrBloque_t             * ptrListaLibres ;  /* lista bloques libres */\
modo   word_t                  * ptrTamBloqueMax ;	
	

typedef struct
{
    camposDescSO1H(volatile) 
} descSO1H_t ;
	
#if (0) 
	
typedef struct
{
    pindx_t                  * ptrIndProcesoActual ;
    tindx_t                  * ptrIndThreadActual ;
    trama_t *                * ptrTramaThread ;
    bool_t                   * ptrActivarAlEpilogo ;
    descProceso_t            * ptrDescProceso ;       /* tabla de procesos */
    word_t                     tamDescProceso ;
    descThread_t             * ptrDescThread ;         /* tabla de threads */
    word_t                     tamDescThread ;
    descFichero_t            * ptrDescFichero ;       /* tabla de ficheros */
    descRecurso_t            * ptrDescRecurso ;       /* tabla de recursos */
    c2c_t                    * ptrC2cPFR ;
//  e2cPFR_t                 * ptrE2PFR ;
    int                      * ptrNivelActivacionSO1H ;
    bool_t                   * ptrEnHalt ;
    bool_t                   * ptrHayTic ;
    ccb_t                    * ptrCcbAlEpilogo ;
    word_t                   * ptrSS_Thread ;
    word_t                   * ptrSP_Thread ;
    word_t                     SP0_So1h ;     /* SP0 del sistema operativo */
    dword_t                  * ptrContRodajas ;
    int                      * ptrContTicsRodaja ;
    rti_t                    * ptrVIOrg ;

    sigThread_t                sigThread ;
    activarThread_t            activarThread ;
    buscarNuevoThreadActual_t  buscarNuevoThreadActual ;
    bloquearThreadActual_t     bloquearThreadActual ;        /* no retorna */
	
	ptrBloque_t              * ptrListaLibres ; /* lista de bloques libres */
    word_t                   * ptrTamBloqueMax ;	
	
} descSO1H_t ;                              /* descriptor del sistema SO1H */

#endif

#endif /* DEF_PROC_H */
