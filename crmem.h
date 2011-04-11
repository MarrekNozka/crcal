/*
 * Soubor:  crmem.h
 * Datum:   30.03.2011 11:54
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   hlavičkový soubor pro crmem.c
 */

#ifndef __CRMEM_H__
#define __CRMEM_H__

/***********************************
 *      Symbolické konstanty 
 ***********************************/
#define ALGFORM 1
#define EXPFORM 0
#define MEMSTR 32

/***********************************
 *   Hlavičkové soubory
 ***********************************/
#include <stdbool.h>

/*********************************** 
 *  Globální proměnné a datové struktury
 ***********************************/


/*********************************** 
 *  Úplné prototypy funkcí 
 ***********************************/

/* @BEGIN@ automatically generated text from %.c (not edit this) */
bool memput(double complex number, char name[], int form);   
complex double memget(char name[],char *form);   
void memclear(void);
/* @END@ automatically generated text from %.c (not edit this) */


#endif
