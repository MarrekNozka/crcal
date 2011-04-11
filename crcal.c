/*
 * Soubor:  posfix-kalkul.c
 * Datum:   17.03.2011
 * Autor:   Marek Nožka, nozka <@t> spseol <d.t> cz
 * Licence: GNU/GPL 
 * Úloha:   Kalkulátor s reverzní polskou notací
 ****************************************************/
#define _ISOC99_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

/* ************************************************ */
#include "crstack.h"
#include "crmem.h"

/* ***************    Makra    ******************** */

/* *********** Prototypy  funkcí ****************** */

#include "crcal.h"

/* *********** Globální proměnné ****************** */

/****************************************************
 *               Hlavní program.
 ****************************************************/
int main(void)
{
    double rnumber;             // číslo se kterým se má počítat
    double inumber;             // číslo se kterým se má počítat
    char enter[CMDLEN];         // vstup od uživatele

    setrad();
    /* Čte řádky až do konce souboru */
    while (readEnter(enter) != EOF) {
        if (sscanf(enter, "%lg,%lg", &rnumber, &inumber) == 2) {
            pushalg(rnumber, inumber);
        } else if (sscanf(enter, "%lgL%lg", &rnumber, &inumber) == 2) {
            pushexp(rnumber, inumber);
        } else if (sscanf(enter, ",%lg", &inumber) == 1) {
            pushalg(0, inumber);
        } else if (sscanf(enter, "%lg", &rnumber) == 1) {
            pushalg(rnumber, 0);
        } else if (strcmp(enter, "pi") == 0) {
            pushalg(M_PI, 0);
        } else if (strcmp(enter, "e") == 0) {
            pushalg(M_E, 0);
        } else if (strcmp(enter, "+") == 0) {
            make2wrap(makePlus, enter);
        } else if (strcmp(enter, "-") == 0) {
            make2wrap(makeMinus, enter);
        } else if (strcmp(enter, "*") == 0) {
            make2wrap(makeTimes, enter);
        } else if (strcmp(enter, "/") == 0) {
            make2wrap(makeDivid, enter);
        } else if (strcmp(enter, "**") == 0) {
            /* n-tá mocnina */
            make2wrap(cpow, enter);
        } else if (strcmp(enter, "^") == 0) {
            /* převrácená hodnota */
            make1wrap(makeFlipp, enter);
        } else if (strcmp(enter, "sqrt") == 0 || strcmp(enter, "t") == 0) {
            make1wrap(csqrt, enter);
        } else if (strcmp(enter, "exp") == 0 || strcmp(enter, "x") == 0) {
            /* exponenciální funkce */
            make1wrap(cexp, enter);
        } else if (strcmp(enter, "ln") == 0) {
            make1wrap(clog, enter);
        } else if (strcmp(enter, "log") == 0) {
            make1wrap(clog10, enter);
        } else if (strcmp(enter, "r") == 0) {
            /* přepne kalkulačku do radiánů */
            setrad();
        } else if (strcmp(enter, "d") == 0) {
            /* přepne kalkulačku do stupňů */
            setdeg();
        } else if (strcmp(enter, "la") == 0) {
            /* poslední číslo převede do algebraickém tvaru */
            last2alg();
            printStack();
        } else if (strcmp(enter, "le") == 0) {
            /* poslední číslo převede exponenciálním tvaru */
            last2exp();
            printStack();
        } else if (strcmp(enter, "aa") == 0) {
            /* všechny čísla převede do algebraickém tvaru */
            all2alg();
            printStack();
        } else if (strcmp(enter, "ae") == 0) {
            /* všechny čísla převede do exponenciálním tvaru */
            all2exp();
            printStack();
        } else if (strcmp(enter, "da") == 0) {
            setalg();
        } else if (strcmp(enter, "de") == 0) {
            setexp();
        } else if (strcmp(enter, "switch") == 0 || strcmp(enter, "s") == 0) {
            if (!lastSwitch())
                fprintf(stderr, " error > Nelze prohodit\n");
        } else if (strcmp(enter, "deg") == 0) {
            make1wrap(rad2deg, enter);
        } else if (strcmp(enter, "rad") == 0) {
            make1wrap(deg2rad, enter);
            /* Goniometrické funkce */
        } else if (strcmp(enter, "sin") == 0) {
            make1wrap(csin, enter);
        } else if (strcmp(enter, "cos") == 0) {
            make1wrap(ccos, enter);
        } else if (strcmp(enter, "tan") == 0 || strcmp(enter, "tg") == 0) {
            make1wrap(ctan, enter);
        } else if (strcmp(enter, "asin") == 0) {
            make1wrap(casin, enter);
        } else if (strcmp(enter, "acos") == 0) {
            make1wrap(cacos, enter);
        } else if (strcmp(enter, "atan") == 0 || strcmp(enter, "atg") == 0) {
            make1wrap(catan, enter);
        } else if (enter[0] == '>') {
            /* ukládá do proměnné */
            complex double number = pop();
            memput(number, enter, algform ? ALGFORM : EXPFORM);
            push(number, algform);
        } else if (enter[0] == '<') {
            /* vybírá obsah proměnné */
            enter[0] = '>';
            char form;
            complex double number = memget(enter, &form);
            push(number, form == ALGFORM ? true : false);
        } else if (strcmp(enter, "rem") == 0 || strcmp(enter, "m") == 0) {
            pop();
            printStack();
        } else if (strcmp(enter, "clr") == 0 || strcmp(enter, "c") == 0) {
            stackclear();       // vymaže celý zásobník
        } else if (strcmp(enter, "p") == 0 || strcmp(enter, "print") == 0) {    // jen tiskne zásobník
            printStack();
        } else if (strcmp(enter, "help") == 0 || strcmp(enter, "h") == 0) {
            printHelp();
        } else if (strcmp(enter, "quit") == 0 || strcmp(enter, "q") == 0) {
            break;
        } else {
            fprintf(stderr, " error > %s\n", enter);
            fprintf(stderr, " error > Neplatný vstup\n");
        }
    }
    stackclear();               // maže zásobník
    memclear();               // maže zásobník
    return 0;
}

/****************************************************
 *                 Funkce
 ****************************************************/

/* čte vstup a tiskne prompt */
int readEnter(char enter[])     //%.h
{
    static bool printPrompt = true;
    int c, ret;

    if (printPrompt) {
        printf("{%s}{%s} > ",
               isalg()? "alg" : "exp", isdeg()? "deg" : "rad");
        printPrompt = false;
    }

    while (((c = getchar()) == ' ') || c == '\t' || c == '\n') {
        if (c=='\n') {
            printf("{%s}{%s} > ",
                   isalg()? "alg" : "exp", isdeg()? "deg" : "rad");
        }
    }
    if (c == EOF) {
        return EOF;
    }
    ungetc(c, stdin);

    ret = scanf("%s", enter);

    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') {
        printPrompt = true;
    } else {
        ungetc(c, stdin);
    }

    return ret;
}

/*  tiskne obsah zásobníku */
void printStack(void)           //%.h
{
    char message[MSGLEN];
    spstack(message);           // zjistím, co je v zásobníku
    printf(">>> %s\n", message);        // ... a vytisknu to 
}

/* Tiskne malou nápovědu */
void printHelp(void)            //%.h
{
    printf
        ("# Kalkulátor s polskou reverzní notací. Pořadí matematických\n# "
         "operací je dáno přímo zápisem. Například: \n# \n# "
         "(5 * (4 + 3) ) / (10-3) odpovídá 5 4 3 + * 10 3 - /\n# "
         "\n# Kompl. čís. se zadávají ve složkovém tvaru: 3,4"
         "... nebo v exponenciální tvaru: 5L0.927 nebo d 5L53.1\n# "
         "\n# Podporované matematické operace a funkce:\n# \n# " "+ - * /\n# "
         "**,  sqrt|q, ^ mocnina, druhá odmocnina, převrácená hodnota\n# "
         "sin, cos, tg|tan, asin, acos, atg|atan, exp|x, ln, log\n# "
         "\n# Příkazy:\n# "
         "d r      přepne pro počítání ve stupních/radiánech\n# "
         "le la    přepne poslední číslo z/do algeb./expon. tvaru\n# "
         "ae aa    přepne všechny čísla z/do algeb./expon. tvaru\n# "
         "de da    přepne přepne výchozí tvar čísel z/do algeb./expon.\n# "
         "print|p  vytiskne zásobník\n# "
         "switch|s prohodí poslední dvě čísla\n# "
         "rem|m    vymaže poslední číslo ze zásobníku\n# "
         "clr|c    vymaže celý zásobník\n# "
         "quit|q    ukončí program\n# "
         "\n# Paměťové registry/proměnné:\n# "
         ">U1      uloží poslední číslo v zásobníku do registru a \n# "
         "<Io      uloží číslo v registru a do zásobníku \n# " "\n");
}

/* Funkce vyzvedne ze zásobníku 2 čísla, provede s nimi
 * matematickou operace danou parametrem function a výsledek
 * uloží zpět do zásobníku.
 * Funkce vrací FALSE pokud operaci nelze provést.
 */
bool make2param(Oper2par function)
{
    complex double x, y;
    if (isnan(y = pop())) {
        return false;
    }
    if (isnan(x = pop())) {
        push(y, EOF);
        return false;
    }
    push(function(x, y), EOF);
    return true;
}

/* Funkce vyzvedne ze zásobníku číslo, provede s nimi
 * matematickou operace danou parametrem function a výsledek
 * uloží zpět do zásobníku.
 * Funkce vrací FALSE pokud operaci nelze provést.
 */
bool make1param(Oper1par function)
{
    complex double x;
    if (isnan(x = pop())) {
        return false;
    }
    if (function == csin || function == ccos || function == ctan) {
        push(function(torad(x)), EOF);
    } else if (function == casin || function == cacos || function == catan) {
        push(radto(function(x)), EOF);
    } else {
        push(function(x), EOF);
    }
    return true;
}

/* Tato funkce jen vhodně obaluje make2param a komunikuje 
 * s uživatelem*/
void make2wrap(Oper2par function, char operationName[]) //%.h
{
    if (!make2param(function)) {
        fprintf(stderr, " error > %s vyžaduje dva parametry\n",
                operationName);
    }
    printStack();
}

/* Tato funkce jen vhodně obaluje make1param a komunikuje 
 * s uživatelem*/
void make1wrap(Oper1par function, char operationName[]) //%.h
{
    if (!make1param(function)) {
        fprintf(stderr, " error > %s vyžaduje jeden parametr\n",
                operationName);
    }
    printStack();

}

/*    plus      */
double complex makePlus(double complex x, double complex y)     //%.h
{
    return x + y;
}

/*    minus      */
double complex makeMinus(double complex x, double complex y)    //%.h
{
    return x - y;
}

/*    krát      */
double complex makeTimes(double complex x, double complex y)    //%.h
{
    return x * y;
}

/*    děleno      */
double complex makeDivid(double complex x, double complex y)    //%.h
{
    return x / y;
}

/*    1/x     */
double complex makeFlipp(double complex x)      //%.h
{
    return 1 / x;
}
