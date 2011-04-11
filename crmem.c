/*
 * Soubor:  crmem.c
 * Datum:   30.03.2011 11:54
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha: 
 * Popis:   
 */

/***********************************
 *      Symbolické konstanty 
 ***********************************/

/***********************************
 *   Hlavičkové soubory
 ***********************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

#include "crmem.h"

/*********************************** 
 *  Globální proměnné a datové struktury
 ***********************************/
typedef struct membox {
    double complex number;
    char name[MEMSTR];
    char form;
    struct membox *next;
} Membox;

static Membox *mem = NULL;



/*********************************** 
 *  Implementace funkcí
 ***********************************/


bool memput(double complex number, char name[], int form)       //%.h   
{
    Membox *newbox;
    Membox *aktual = mem;

    while (aktual != NULL) {
        if (strcmp(name, aktual->name) == 0) {
            aktual->form =form ;
            aktual->number = number;
            return true;
        }
        aktual = aktual->next;
    }
    if ((newbox = (Membox *) malloc(sizeof(Membox))) != NULL) {
        newbox->number = number;
        newbox->form = form;
        strncpy(newbox->name, name, MEMSTR);
        newbox->next = mem;
        mem = newbox;
        return true;
    } else
        return false;
}

complex double memget(char name[],char *form)      //%.h   
{
    Membox *aktual=mem;
    while (aktual != NULL) {
        if (strcmp(name, aktual->name) == 0) {
            *form = aktual->form;
            return aktual->number;
        }
        aktual = aktual->next;
    }
    return NAN;
}

/* Vymaže celý seznam */
void memclear(void)           //%.h
{
    Membox *forfree = mem;
    while (mem != NULL) {
        forfree = mem;
/*        printf("%g\n",forfree->number);*/
        mem = mem->next;
        free((void *) forfree);
    }
}

