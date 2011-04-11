/*
 * Soubor:  dynmstack.h
 * Datum:   17.03.2011 21:37
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   hlavičkový soubor pro dynmstack.c
 */

#ifndef __CRSTACK_H__
#define __CRSTACK_H__

/***********************************
 *      Symbolické konstanty 
 ***********************************/
// maximální počet znaků pro tisk obsahu zásobníku
#define MSGLEN 2048
// maximální počet znaků v pro jedno číslo
#define MSGSEG 64
/***********************************
 *   Hlavičkové soubory
 ***********************************/
#include <stdbool.h>
#include <math.h>
#include <complex.h>

/*********************************** 
 *  Globální proměnné a datové struktury
 ***********************************/
// Odkaz na funkci, která bude sloužit k tisku komplexního 
// čísla do řetězce.
typedef void (*Cmpxprnt) (char *, complex double);

typedef complex double (*Fconvert) (complex double);
extern Fconvert torad, radto;   // odkaz na funkci, která převádí na radiány
extern bool algform;            // poslední tvar k.č. vrácený pop()

/*********************************** 
 *  Úplné prototypy funkcí 
 ***********************************/

/* @BEGIN@ automatically generated text from %.c (not edit this) */
void setalg(void);   
bool isalg(void);   
void setexp(void);   
bool push(double complex number, int algform);   
int pushalg(double real, double imag);   
int pushexp(double rabs, double rang);   
bool lastSwitch(void);
void last2alg(void);
void last2exp(void);
void all2alg(void);
void all2exp(void);
complex double pop();   
void spstack(char s[]);
complex double deg2rad(complex double deg);
complex double rad2rad(complex double rad);
complex double rad2deg(double complex deg);
void setdeg(void);
bool isdeg(void);
void setrad(void);
void stackclear(void);
/* @END@ automatically generated text from %.c (not edit this) */


#endif
