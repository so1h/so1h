/* ----------------------------------------------------------------------- */
/*                                printgen.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1hpub.h\printgen.h>                              /* printCar_t */
#include <so1hpub.h\caracter.h>                                     /* dig */

int printGenLn ( printCar_t p )
{
    p('\r') ;                                 /* retorno de carro (char)13 */
    p('\n') ;                                 /* salto de linea   (char)10 */
    return(2) ;
}

int printGenStr ( const char * str, printCar_t p )
{
    char car ;
    int cont = 0 ;
    while ((car = *str++) != (char)0)
    {
        if (car == '\n')
        {
            p('\r') ;
            cont++ ;
        }
        p(car) ;
        cont++ ;
    }
    return(cont) ;
}

int printGenStrHasta ( const char * str, word_t n, bool_t lleno, printCar_t p )
{
    char car ;
    word_t i = 0 ;
    while ((i < n) && ((car = *str++) != (char)0))
    {
        if (car == '\n') p('\r') ;
        p(car) ;
        i++ ;
    }
    if (lleno)
        while (i < n)
        {
            p(' ') ;
            i++ ;
        }
    return(i) ;
}

#define tamStrMax 11

static char str[tamStrMax] ;

#define codigoPrintGenDec(tamStr, tamStrM1, tamStrM2) {                      \
/*  char str[tamStr] ; */                                                    \
    word_t i, j ;                                                            \
    int cont = 0 ;                                                           \
    char * ptr ;                                                             \
    if (l == 0) l = 1 ;                                                      \
    if (num == 0) {                                                          \
        for ( i = 0 ; i < l-1 ; i++ ) p(' ') ;                               \
        p('0') ;                                                             \
        return(l) ;                                                          \
    }                                                                        \
    ptr = (char *)&str[tamStrM1] ;                                           \
    *ptr = (char)0 ;                                                         \
    i = 0 ;                                                                  \
    while (num > 0) {                                                        \
        str[tamStrM2-i] = dig[(word_t)(num%10)] ;                            \
        num = num/10 ;                                                       \
        i++ ;                                                                \
    }                                                                        \
    for ( j = i ; j < l ; j++ ) p(' ') ;                                     \
    ptr = (char *)&str[tamStrM2-i+1] ;                                       \
    while (*ptr != (char)0) {                                                \
        p(*ptr) ;                                                            \
        ptr++ ;                                                              \
        cont++ ;                                                             \
    }                                                                        \
    return(l + cont) ;                                                       \
}

int printGenDec ( word_t num, word_t l, printCar_t p )
{
    codigoPrintGenDec(6, 5, 4)
}

int printGenLDec ( dword_t num, word_t l, printCar_t p )
{
    codigoPrintGenDec(11, 10, 9)
}

#define codigoPrintGenInt(tamStr, tamStrM1, tamStrM2) {                      \
/*  char str[tamStr] ; */                                                    \
    word_t i, j ;                                                            \
    char * ptr ;                                                             \
    bool_t negativo = (bool_t)(num < 0) ;                                /**/\
    if (l == 0) l = 1 ;                                                      \
    if (num == 0) {                                                          \
        for ( i = 0 ; i < l-1 ; i++ ) p(' ') ;                               \
        p('0') ;                                                             \
        return(0) ;                                                          \
    }                                                                        \
    if (num < 0) num = -num ;                                            /**/\
    ptr = (char *)&str[tamStrM1] ;                                           \
    *ptr = (char)0 ;                                                         \
    i = 0 ;                                                                  \
    while (num > 0) {                                                        \
        str[tamStrM2-i] = dig[(word_t)(num%10)] ;                            \
        num = num/10 ;                                                       \
        i++ ;                                                                \
    }                                                                        \
    if (negativo) {                                                      /**/\
        for ( j = i ; j < l-1 ; j++ ) p(' ') ;                           /**/\
        p('-') ;                                                         /**/\
    }                                                                    /**/\
    else                                                                 /**/\
        for ( j = i ; j < l ; j++ ) p(' ') ;                                 \
    ptr = (char *)&str[tamStrM2-i+1] ;                                       \
    while (*ptr != (char)0) {                                                \
        p(*ptr) ;                                                            \
        ptr++ ;                                                              \
    }                                                                        \
    return(0) ;                                                              \
}

int printGenInt ( short int num, word_t l, printCar_t p )
{
    codigoPrintGenInt(6, 5, 4)
}

int printGenLInt ( int num, word_t l, printCar_t p )
{
    codigoPrintGenInt(11, 10, 9)
}

int printGenHex ( word_t num, word_t l, printCar_t p )
{
    /*  char str[5] ; */
    word_t i, j ;
    int cont = 0 ;
    char * ptr ;
    if (l == 0) l = 1 ;
    if (num == 0)
    {
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;
        p('0') ;
        return(l) ;
    }
    ptr = &str[4] ;
    *ptr = (char)0 ;
    i = 0 ;
    while (num > 0)
    {
        str[3-i] = dig[num%16] ;
        num = num/16 ;
        i++ ;
    }
    for ( j = i ; j < l ; j++ ) p('0') ;
    ptr = &str[3-i+1] ;
    while (*ptr != (char)0)
    {
        p(*ptr) ;
        ptr++ ;
        cont++ ;
    }
    return(l + cont) ;
}

int printGenLHex ( dword_t num, word_t l, printCar_t p )
{
    /*  char str[9] ; */
    char * ptr ;
    word_t * ptrWord ;
    word_t i, j ;
    if (l == 0) l = 1 ;
    if (num == 0)
    {
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;
        p('0') ;
        return(l) ;
    }
    str[8] = (char)0 ;
    i = 0 ;
    ptrWord = (word_t *)&num ;
    while (num > 0)
    {
        str[7-i] = dig[ptrWord[0]%16] ;
        ptrWord[0] = ptrWord[0]/16 + 0x1000*(ptrWord[1]%16) ;
        ptrWord[1] = ptrWord[1]/16 ;
        i++ ;
    }
    for ( j = i ; j < l ; j++ ) p('0') ;
    ptr = &str[7-i+1] ;

    while (i > 0)
    {
        p(*ptr) ;
        ptr++ ;
        i-- ;
    }
    return(0) ;
}

#define codigoPrintGenBin(es16o32) {                                         \
    word_t i, j ;                                                            \
    word_t pot ;                                                             \
    if (l == 0) l = 1 ;                                                      \
    if (num == 0) {                                                          \
        for ( i = 0 ; i < l-1 ; i++ ) p('0') ;                               \
        p('0') ;                                                             \
        return(0) ;                                                          \
    }                                                                        \
    i = es16o32 ;                                                            \
    pot = 1 << (i-1) ;                                                       \
    while ((pot & num) == 0) {                                               \
        pot = (pot >> 1) ;                                                   \
        i-- ;                                                                \
    }                                                                        \
    if (i > l) l = i ;                                                       \
    else pot = (1 << (l-1)) ;                                                \
    for ( j = 0 ; j < l ; j++ ) {                                            \
        if (pot & num) p('1') ;                                              \
        else p('0') ;                                                        \
        pot = (pot >> 1) ;                                                   \
    }                                                                        \
    return(0) ;                                                              \
}

int printGenBin ( word_t num, word_t l, printCar_t p )
{
    codigoPrintGenBin(16)
}

int printGenLBin ( dword_t num, word_t l, printCar_t p )
{
    codigoPrintGenBin(32)
}

int printGenPtr ( pointer_t ptr, printCar_t p )
{
    printGenHex(seg(ptr), 4, p) ;
    p(':') ;
    printGenHex(off(ptr), 4, p) ;
    return(9) ;
}

//#include <so1pub.h\stdarg.h>                 /* va_arg, va_end, va_start */
//#include <so1pub.h\varargs.h>       /* va_arg, va_end, va_start, va_list */

int printfGen ( const char * formato, printCar_t p, ... )
{
    return(0) ;
}

