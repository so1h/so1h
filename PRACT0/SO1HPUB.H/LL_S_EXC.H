/*
  Copyright (c) 2017, Pedro Pablo Lopez Rodriguez & Julio Lozano Del Pozo
  2-clause BSD license.
*/

/* ----------------------------------------------------------------------- */
/*                               ll_s_exc.h                                */
/* ----------------------------------------------------------------------- */
/* Subconjunto de rutinas de interfaz de las llamadas al sistema que se    */
/* utilizan desde SO1H al ser necesarias para la creación del proceso      */
/* inicial INIC. La idea es no tener que enlazar el resto de rutinas de    */
/* interfaz contenidas en ll_s_so1.c para obtener SO1H.BIN, de manera que  */
/* se reduzca asi el tamaño de SO1H.BIN.                                   */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_EXC_H
#define LL_S_EXC_H

#include <so1hpub.h\tipos.h>                                     /* word_t */
#include <so1hpub.h\def_proc.h>           /* pid_t, uid_t, gid_t, modoAp_t */

/* ----------------------------------------------------------------------- */
/* -                                                                     - */
/* -     Llamadas al sistema implementada mediante traps (int 0x60)      - */
/* -     ==========================================================      - */
/* ----------------------------------------------------------------------- */

/* Numero de vector de interrupcion utilizado por el sistema SO1.          */

#define nVIntSO1H    0x60

pid_t createProcess ( const char * nombre,
                      const char * comando ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema toma como primer parametro una cadena de        */
/* caracteres nombre. Si esa cadena de caracteres corresponde a un fichero */
/* ejecutable (SO1) existente en el directorio raiz de la unidad de la que */
/* se ha arrancado, y si existe memoria libre suficiente, se crea un nuevo */
/* proceso que ejecuta el fichero una vez cargado en la nueva particion de */
/* memoria creada. El proceso creado hereda todos los ficheros que tenia   */
/* abiertos el proceso padre. El segundo parametro str es una cadena de    */
/* caracteres de la cual pueden extraerse los argumentos con los que se    */
/* ejecuta el programa.                                                    */
/* ----------------------------------------------------------------------- */


int exec ( const char * nombre,
           const char * comando ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema toma como primer parametro una cadena de        */
/* caracteres nombre. Si esa cadena de caracteres corresponde a un fichero */
/* ejecutable (SO1) existente en el directorio raiz de la unidad de la que */
/* se ha arrancado, y si existe memoria suficiente para cargar el fichero  */
/* con su codigo, datos y pila, el proceso que ejecuta esta llamada (exec) */
/* deja de ejecutar su anterior codigo, datos y pila para pasar a ejecutar */
/* el nuevo codigo, datos y pila desde el principio. El segundo parametro  */
/* str es una cadena de caracteres de la cual pueden extraerse los         */
/* argumentos con los que se el proceso pasa a ejecutar el nuevo codigo,   */
/* datros y pila.                                                          */
/* ----------------------------------------------------------------------- */


pid_t waitpid ( pid_t pid, int * statloc ) ;

/* ----------------------------------------------------------------------- */
/* El proceso que ejecuta esta llamada queda bloqueado hasta que el        */
/* proceso hijo con el pid indicado termine. Si pid = -1 el bloqueo es     */
/* tan solo hasta que termine cualquiera de sus hijos. La llamada retorna  */
/* el pid del proceso hijo terminado.                                      */
/* ----------------------------------------------------------------------- */

int kill ( int pid ) ;                         /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Si pid > 0 y corresponde al identificador de un proceso del sistema,    */
/* esta llamada destruye al proceso cuyo identificador de proceso es pid y */
/* retorna como resultado un 0.                                            */
/* Si pid = -1 esta llamada destruye todos los procesos menos el proceso   */
/* con pid 0 que corresponde a la consola del sistema SO1.                 */
/* Si pid = 0 la llamada simplemente retorna el valor -1, ya que no se     */
/* permite matar al proceso consola.                                       */
/* En cualquier otro caso la llamada al sistema retorna el valor -2.       */
/* ----------------------------------------------------------------------- */

void exit ( int status ) ;

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema termina el proceso (en ejecucion) que la        */
/* ejecuta, tratando de devolver al proceso padre el codigo de terminacion */
/* status. Si el proceso padre esta esperando a recibir ese codigo de      */
/* terminacion habiéndose bloqueado con waitpid el proceso termina         */
/* inmediatamente, la ventana correspondiente al proceso se cierra y la    */
/* particion de memoria que utilizaba se libera, devolviendose dicha       */
/* memoria al sistema. Sie proceso padre no esta bloqueado en waitpid, el  */
/* proceso que hace el exit se queda bloqueado (zombie) a la espera de que */
/* el padre haga el waitpid correspondiente.                               */
/* ----------------------------------------------------------------------- */

int setuid ( uid_t uid ) ;                     /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el uid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int setgid ( gid_t gid ) ;                     /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Esta llamada al sistema establece el gid del proceso que la ejecuta.    */
/* ----------------------------------------------------------------------- */

int kill ( int pid ) ;                         /* declarada en ll_s_exec.h */

/* ----------------------------------------------------------------------- */
/* Si pid > 0 y corresponde al identificador de un proceso del sistema,    */
/* esta llamada destruye al proceso cuyo identificador de proceso es pid y */
/* retorna como resultado un 0.                                            */
/* Si pid = -1 esta llamada destruye todos los procesos menos el proceso   */
/* con pid 0 que corresponde a la consola del sistema SO1.                 */
/* Si pid = 0 la llamada simplemente retorna el valor -1, ya que no se     */
/* permite matar al proceso consola.                                       */
/* En cualquier otro caso la llamada al sistema retorna el valor -2.       */
/* ----------------------------------------------------------------------- */

int open ( const char * nombre, modoAp_t modoAp ) ;

int close ( int df ) ;

int write ( int df, pointer_t dir, word_t nbytes ) ;

#endif /* LL_S_EXC_H */
