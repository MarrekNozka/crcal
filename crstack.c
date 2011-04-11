/*
 * Soubor:  crstack.c
 * Datum:   17.03.2011 21:34
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha:   Zásobníku pomocí pole.
 */
#define _ISOC99_SOURCE
#define _GNU_SOURCE
/***********************************
 *   Hlavičkové soubory
 ***********************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "crstack.h"
/*********************************** 
 *  Globální proměnné a datové struktury
 ***********************************/

Fconvert torad = rad2rad;       // odkaz na funkci, která převádí na radiány
Fconvert radto = rad2rad;       // odkaz na funkci, která převádí na radiány
bool algform;                   // poslední tvar k.č. vrácený pop()

// Jedna paměťová buňka je složena z komplexního čísla a odkazu
// na funkci, která toto číslo tiskne. Tiskne se do složkového 
// nebo exponenciálního tvaru.
typedef struct stackbox {
    double complex number;
    Cmpxprnt prntfnc;
    struct stackbox *next;
} Stackbox;

static Stackbox *stack = NULL;

/*********************************** 
 *  Implementace funkcí
 ***********************************/

/* Tiskne komplexní číslo do řetězce 
 * v algebraickém tvaru tvaru */
void c2algstr(char s[], double complex number)
{
    strncpy(s, "", MSGSEG);
    char bagr[MSGSEG];
    snprintf(bagr, MSGSEG, "%.4g", creal(number));
    strncat(s, bagr, MSGSEG);
    if (cimag(number) > 0) {
        strncat(s, "+j", MSGSEG);
        snprintf(bagr, MSGSEG, "%.4g", cimag(number));
        strncat(s, bagr, MSGSEG);
    }
    if (cimag(number) < 0) {
        strncat(s, "-j", MSGSEG);
        snprintf(bagr, MSGSEG, "%.4g", -cimag(number));
        strncat(s, bagr, MSGSEG);
    }
}

/* Tiskne komplexní číslo do řetězce 
 * v exponenciálním tvaru */
void c2expstr(char s[], double complex number)
{
    strcpy(s, "");
    char bagr[MSGSEG];
    snprintf(bagr,MSGSEG, "%.4g", cabs(number));
    strncat(s, bagr,MSGSEG);
    strncat(s, "L",MSGSEG);
    snprintf(bagr, MSGSEG, "%.4g", (double) radto(carg(number)));
    strncat(s, bagr, MSGSEG);
    if (radto == rad2deg) {
        strncat(s, "°", MSGSEG);
    }
}

Cmpxprnt defform = c2expstr;    // výchozí tvar čísel

/* výchozí tvar komplexních čísel */
void setalg(void)       //%.h   
{
    defform = c2algstr;
}
bool isalg(void)       //%.h   
{
    return (defform == c2algstr)?true:false;
}
void setexp(void)       //%.h   
{
    defform = c2expstr;
}

/* Funkce ukládá číslo do zásobníku,
 * vrací false při přetečení. 
 */
bool push(double complex number, int algform)       //%.h   
{
    unsigned int size = 0;
    Stackbox *newbox;

    if ((newbox = (Stackbox *) malloc(sizeof(Stackbox))) != NULL) {
        newbox->number = number;
        if (algform == EOF) 
            newbox->prntfnc = defform;
        else if (algform == 1) 
            newbox->prntfnc = c2algstr;
        else if (algform == 0) 
            newbox->prntfnc = c2expstr;
        newbox->next = stack;
        stack = newbox;
        while (newbox != NULL) {
            newbox = newbox->next;
            size++;
        }
        return true;
    } else
        return false;
}

/* obal pro push, který přidává číslo v algebraickém tvaru */
int pushalg(double real, double imag)   //%.h   
{
    return push(real + imag * I, true);
}

/* obal pro push, který přidává číslo v exponenciálním tvaru */
int pushexp(double rabs, double rang)   //%.h   
{
    return push(rabs * cos(torad(rang)) + rabs * sin(torad(rang)) * I,
                false);
}

/* Funkce prohodí poslední dva prvky v zásobníku */
bool lastSwitch(void)  //%.h
{
    if (stack != NULL && stack->next != NULL) {
        Stackbox *one=stack;
        Stackbox *three=stack->next->next;

        stack = stack->next;  // two
        stack->next = one;
        one->next = three; 
        return true;
    } else {
        return false;
    }
}

/* Funkce přepne poslední prvek z/do algebraického/exponenciálního tvaru */
void last2alg(void)             //%.h
{
    if (stack != NULL) {
        stack->prntfnc = c2algstr;
    }
}

void last2exp(void)             //%.h
{
    if (stack != NULL) {
        stack->prntfnc = c2expstr;
    }
}

/* Funkce přepne celý stack z/do algebraického/exponenciálního tvaru */
void all2alg(void)              //%.h
{
    Stackbox *aktual = stack;
    while (aktual != NULL) {
        aktual->prntfnc = c2algstr;
        aktual = aktual->next;
    }
}

void all2exp(void)              //%.h
{
    Stackbox *aktual = stack;
    while (aktual != NULL) {
        aktual->prntfnc = c2expstr;
        aktual = aktual->next;
    }
}

/* Funkce vrací číslo, které vyzvedne ze zásobníku.
 * V případě prázdného zásobníku vrátí NAN. */
complex double pop()            //%.h   
{
    Stackbox *forfree;
    complex double ret;
    if (stack != NULL) {
        forfree = stack;
        stack = forfree->next;
        ret = forfree->number;
        algform = (forfree->prntfnc==c2algstr)?true:false;
        free((void *) forfree);
        return ret;
    } else
        return NAN;
}

/* Funkce vrací počet prvků v zásobníku */
/*int gettstackc(void)            //%.h*/
/*{                                    */
/*    return stackc;                   */
/*}                                    */

/* Funkce do řetězce s vytiskne obsah zásobníku */
void spstack(char s[])          //%.h
{
    Stackbox *aktual = stack;
    char bagr[MSGLEN];
    char segment[MSGSEG];

    strncpy(s, "]", MSGLEN);
    while (aktual != NULL) {
        aktual->prntfnc(segment, aktual->number);
        strncpy(bagr,s, MSGLEN);
        snprintf(s,MSGSEG, "(%s) %s",segment,bagr);
        aktual = aktual->next;
    }
    strncpy(bagr, "[ ", MSGLEN);
    strncat(bagr, s, MSGLEN);
    strncpy(s, bagr, MSGLEN);
}

/* Funkce převádí stupně na radiány */
complex double deg2rad(complex double deg)      //%.h
{
    return M_PI * deg / 180;
}

complex double rad2rad(complex double rad)      //%.h
{
    return rad;
}

complex double rad2deg(double complex deg)      //%.h
{
    return 180 * deg / M_PI;
}

/* Funkce přepnou na výpočty ve stupních/radiánech */
void setdeg(void)               //%.h
{
    torad = deg2rad;
    radto = rad2deg;
}
bool isdeg(void)               //%.h
{
    return (torad==deg2rad)?true:false;
}

void setrad(void)               //%.h
{
    torad = rad2rad;
    radto = rad2rad;
}


/* Vymaže celý zásobník */
void stackclear(void)           //%.h
{
    Stackbox *forfree = stack;
    while (stack != NULL) {
        forfree = stack;
        stack = stack->next;
        free((void *) forfree);
    }
}

/*
int main()
{

    char s[MSGLEN];

    setrad();
    pushalg(3, -4);
    pushexp(5, 1);

    setdeg();
    pushexp(6, 1);

    setrad();
    spstack(s);
    printf("#### %s\n", s);

    setdeg();
    spstack(s);
    printf("#### %s\n", s);

    last2alg();
    spstack(s);
    printf("#### %s\n", s);

    all2alg();
    spstack(s);
    printf("#### %s\n", s);

    all2exp();
    spstack(s);
    printf("#### %s\n", s);

    setrad();
    spstack(s);
    printf("#### %s\n", s);



    printf("%g\n", cimag(pop()));

    printf("%g\n", pop());
    printf("   %d\n", push(8, c2algstr));

    stackclear();
}

*/
