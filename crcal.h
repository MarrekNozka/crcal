/*
 * Soubor:  crcal.h
 * Datum:   28.03.2011 22:03
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   hlavičkový soubor pro crcal.c
 */

#ifndef __CRCAL_H__
#define __CRCAL_H__

/***********************************
 *      Symbolické konstanty 
 ***********************************/
#define CMDLEN 128

/***********************************
 *   Hlavičkové soubory
 ***********************************/

/*********************************** 
 *  Globální proměnné a datové struktury
 ***********************************/

typedef double complex(*Oper2par) (double complex, double complex);
typedef double complex(*Oper1par) (double complex);

/*********************************** 
 *  Úplné prototypy funkcí 
 ***********************************/

/* @BEGIN@ automatically generated text from %.c (not edit this) */
int readEnter(char enter[]);
void printStack(void);
void printHelp(void);
void make2wrap(Oper2par function, char operationName[]);
void make1wrap(Oper1par function, char operationName[]);
double complex makePlus(double complex x, double complex y);
double complex makeMinus(double complex x, double complex y);
double complex makeTimes(double complex x, double complex y);
double complex makeDivid(double complex x, double complex y);
double complex makeFlipp(double complex x);
/* @END@ automatically generated text from %.c (not edit this) */


#endif
