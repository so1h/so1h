/* ----------------------------------------------------------------------- */
/*                                  boot.c                                 */
/* ----------------------------------------------------------------------- */
/*      Sector de arranque en una sola fase para un disquete de 1,44 MB    */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* Carga a partir de la direccion 0x600 los 320 sectores del disquete que  */
/* comienzan en el sector logico 33, cediendo a continuacion el control a  */
/* la direccion CS:IP = 0x0060:0x0000 (direccion lineal 0x0600).           */
/*                                                                         */
/* Este sector de arranque corresponde a un disquete de 3,5 pulgadas con 2 */
/* caras, 18 sectores por pista y 80 cilindos. En consecuencia el disquete */
/* cuenta con un total de 2*18*80 = 2880 sectores. El numero de bytes que  */
/* contiene cada sector es 512 bytes.                                      */
/*                                                                         */
/* El sistema de ficheros del disquete se reporta en su sector de arranque */
/* (sector 0) tratandose de un sistema de ficheros tipo FAT12 con 2 fats,  */
/* (ocupando 9 sectores cada una de ellas) y con un directorio raiz de 224 */
/* entradas, siendo las entradas de 32 bytes. Solo hay un sector reservado */
/* antes de la fat, el cual corresponde al propio sector de arranque. Como */
/*                                                                         */
/*                 1 + 2*9 + 224/(512/32) = 1 + 18 + 14 = 33               */
/*                 ^    ^         ^                                        */
/*               boot fats    dir.raiz                                     */
/*                                                                         */
/* resulta que el sector 33 es el primer sector de datos del disquete.     */
/*                                                                         */
/* Distribucion de los 2880 sectores del disquete:                         */
/*                                                                         */
/* sector          0 : sector   de arranque del disquete                   */
/* sectores  1..  18 : sectores ocupados por las dos fats                  */
/* sectores 19..  32 : sectores para las 224 entradas del directorio raiz  */
/* sectores 33..2879 : sectores para almacenar datos de los ficheros y     */
/*                     entradas de los subdirectorios                      */
/* ----------------------------------------------------------------------- */

#define bytesPorSector 512

#define bootCode startBin                          /* truco para el linker */

unsigned char bootCode [ bytesPorSector ] =
{
    0xeb, 0x3c, 0x90, 0x53, 0x4f, 0x31, 0x20, 0x76, 0x31, 0x2e, 0x30, 0x00, 0x02, 0x01, 0x01, 0x00,
    0x02, 0xe0, 0x00, 0x40, 0x0b, 0xf0, 0x09, 0x00, 0x12, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x40, 0x01, 0x00, 0x00, 0x45, 0x54, 0x49, 0x51, 0x55,
    0x45, 0x54, 0x41, 0x20, 0x20, 0x20, 0x46, 0x41, 0x54, 0x31, 0x32, 0x20, 0x20, 0x20, 0xfa, 0xb8,
    0x00, 0x90, 0x8e, 0xd0, 0x8b, 0xe0, 0x8b, 0xe8, 0xe9, 0x9a, 0x00, 0x55, 0x8b, 0xec, 0x8a, 0x46,
    0x04, 0xb7, 0x07, 0xb4, 0x0e, 0xcd, 0x10, 0x5d, 0xc3, 0xb4, 0x00, 0xcd, 0x13, 0x72, 0x03, 0x33,
    0xc0, 0xc3, 0xb8, 0x01, 0x00, 0xc3, 0xc8, 0x04, 0x00, 0x00, 0x8b, 0x46, 0x04, 0xbb, 0x12, 0x00,
    0x33, 0xd2, 0xf7, 0xf3, 0xfe, 0xc2, 0x88, 0x56, 0xff, 0x8b, 0x46, 0x04, 0xbb, 0x24, 0x00, 0x33,
    0xd2, 0xf7, 0xf3, 0xbb, 0x12, 0x00, 0x52, 0x33, 0xd2, 0x58, 0xf7, 0xf3, 0x88, 0x46, 0xfe, 0x8b,
    0x46, 0x04, 0xbb, 0x24, 0x00, 0x33, 0xd2, 0xf7, 0xf3, 0x89, 0x46, 0xfc, 0xc4, 0x5e, 0x08, 0x8a,
    0x56, 0x06, 0x8a, 0x76, 0xfe, 0x8a, 0x6e, 0xfc, 0x8a, 0x4e, 0xff, 0xb0, 0x01, 0xb4, 0x02, 0xcd,
    0x13, 0x72, 0x04, 0x33, 0xc0, 0xc9, 0xc3, 0xb8, 0x01, 0x00, 0xc9, 0xc3, 0xc8, 0x02, 0x00, 0x00,
    0x8b, 0x5e, 0x04, 0x8b, 0x07, 0x89, 0x46, 0xfe, 0x8b, 0x46, 0xfe, 0x03, 0x46, 0x06, 0x73, 0x08,
    0x8b, 0x5e, 0x04, 0x81, 0x47, 0x02, 0x00, 0x10, 0x89, 0x46, 0xfe, 0x8b, 0x5e, 0x04, 0x8b, 0x46,
    0xfe, 0x89, 0x07, 0xc9, 0xc3, 0xc8, 0x0a, 0x00, 0x00, 0xc7, 0x46, 0xfe, 0xc0, 0x07, 0xc7, 0x46,
    0xfc, 0x00, 0x00, 0xc7, 0x46, 0xfa, 0x00, 0x90, 0xc7, 0x46, 0xf8, 0x00, 0x00, 0xc7, 0x46, 0xf6,
    0x00, 0x00, 0xeb, 0x15, 0xc4, 0x5e, 0xfc, 0x26, 0x8a, 0x07, 0xc4, 0x5e, 0xf8, 0x26, 0x88, 0x07,
    0xff, 0x46, 0xfc, 0xff, 0x46, 0xf8, 0xff, 0x46, 0xf6, 0x81, 0x7e, 0xf6, 0x00, 0x02, 0x7c, 0xe4,
    0x68, 0x00, 0x90, 0x68, 0x29, 0x01, 0xcb, 0xc9, 0xc3, 0xc8, 0x08, 0x00, 0x00, 0xc7, 0x46, 0xfe,
    0x60, 0x00, 0xc7, 0x46, 0xfc, 0x00, 0x00, 0xc7, 0x46, 0xfa, 0x21, 0x00, 0x0e, 0x1f, 0xe8, 0x18,
    0xff, 0xc7, 0x46, 0xf8, 0x00, 0x00, 0xeb, 0x2b, 0xff, 0x76, 0xfe, 0xff, 0x76, 0xfc, 0x6a, 0x00,
    0x8b, 0x46, 0xfa, 0xff, 0x46, 0xfa, 0x50, 0xe8, 0x0c, 0xff, 0x83, 0xc4, 0x08, 0x6a, 0x2e, 0xe8,
    0xe9, 0xfe, 0x59, 0x68, 0x00, 0x02, 0x8d, 0x46, 0xfc, 0x50, 0xe8, 0x4f, 0xff, 0x83, 0xc4, 0x04,
    0xff, 0x46, 0xf8, 0x81, 0x7e, 0xf8, 0x40, 0x01, 0x7c, 0xce, 0x6a, 0x53, 0xe8, 0xcc, 0xfe, 0x59,
    0x6a, 0x4f, 0xe8, 0xc6, 0xfe, 0x59, 0x6a, 0x31, 0xe8, 0xc0, 0xfe, 0x59, 0x6a, 0x0d, 0xe8, 0xba,
    0xfe, 0x59, 0x6a, 0x0a, 0xe8, 0xb4, 0xfe, 0x59, 0x6a, 0x60, 0x6a, 0x00, 0xcb, 0xc9, 0xc3, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xaa
} ;






