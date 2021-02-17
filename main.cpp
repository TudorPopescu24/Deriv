#include <fstream>
#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

ifstream fin("deriv.in");
ofstream fout("deriv.out");

char infix[100], postfixata[100], derivata1[200], derivata2[200];
char sir[50], sir1[50], sir2[50];

struct nod {
    char info;
    nod* urm;
};

struct arbore {
    char info[10];
    arbore* st, * dr;
};

struct nodarbore {
    arbore* info;
    nodarbore* urm;
};

struct stiva {
    nod* varf;
    int nrElemente;
};

struct stivaarbore {
    nodarbore* varf;
    int nrElemente;
};

struct coada {
    nod* prim;
    nod* ultim;
    int nrElemente;
};

coada postfix;

bool esteVida(stiva S) {
    return S.nrElemente == 0;
}

bool esteVida(stivaarbore S) {
    return S.nrElemente == 0;
}

// scoate elementul din varful stivei
void pop(stiva& S) {
    if (!esteVida(S)) {

        nod* varf_nou;
        varf_nou = S.varf->urm;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;
    }
}

// scoate elementul din varful stivei de arbori
void poparbore(stivaarbore& S) {
    if (!esteVida(S)) {
        nodarbore* varf_nou;
        varf_nou = S.varf->urm;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;
    }
}

// adauga un element la stiva, in varf (LIFO)
void push(stiva& S, char element) {
    nod* nod_nou;

    if (esteVida(S)) {
        S.nrElemente = 1;
        S.varf = new nod;
        S.varf->info = element;
        S.varf->urm = NULL;
    }
    else {
        S.nrElemente++;
        nod_nou = new nod;
        nod_nou->info = element;
        nod_nou->urm = S.varf;
        S.varf = nod_nou;
    }
}

// adauga un element la stiva de arbori, in varf (LIFO)
void pusharbore(stivaarbore& S, arbore* element) {
    nodarbore* nod_nou;

    if (esteVida(S)) {
        S.nrElemente = 1;
        S.varf = new nodarbore;
        S.varf->info = element;
        S.varf->urm = NULL;
    }
    else {
        S.nrElemente++;
        nod_nou = new nodarbore;
        nod_nou->info = element;
        nod_nou->urm = S.varf;
        S.varf = nod_nou;
    }
}

void initializeazastiva(stiva& S) {
    S.varf = NULL;
    S.nrElemente = 0;
}

void initializeazastivaarbore(stivaarbore& S) {
    S.varf = NULL;
    S.nrElemente = 0;
}

void initializeazacoada(coada& C) {
    C.prim = NULL;
    C.ultim = NULL;
    C.nrElemente = 0;
}

bool esteVida(coada C) {
    return C.nrElemente == 0;
}

// scoate primul element din coada, din capul cozii
char dequeue(coada& C) {
    char element = C.prim->info;
    nod* primul;
    primul = C.prim;
    C.prim = C.prim->urm;
    delete primul;
    C.nrElemente--;
    return element;
}

// adauga un element la coada, dupa ultimul element (FIFO)
void enqueue(coada& C, char element) {
    nod* nod_nou;

    if (esteVida(C)) {
        C.nrElemente = 1;
        C.prim = new nod;
        C.prim->info = element;
        C.prim->urm = NULL;
        C.ultim = C.prim;
    }
    else {
        C.nrElemente++;
        nod_nou = new nod;
        nod_nou->info = element;
        nod_nou->urm = NULL;
        C.ultim->urm = nod_nou;
        C.ultim = nod_nou;
    }
}

void afiseaza(coada C) {
    nod* nod_curent;
    nod_curent = C.prim;
    int i = 0;

    while (nod_curent != NULL) {
        fout << nod_curent->info;
        postfixata[i++] = nod_curent->info;
        nod_curent = nod_curent->urm;

    }
}

//prioritatea operatorilor
int prioritate(char c) {
    switch (c) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    case 's':
    case 'c':
    case 't':
    case 'g':
    case 'l':
    case 'r':
    case 'e':
    case 'z':
        return 4;
    case '(':
        return 5;
    case ')':
        return 6;
    default:
        return 7;
    }
}

//prioritatea operatorilor (sir de caractere)
int prioritate1(char c[]) {
    if (!strcmp(c, "+"))
        return 1;
    if (!strcmp(c, "-"))
        return 2;
    if (!strcmp(c, "*"))
        return 3;
    if (!strcmp(c, "^"))
        return 4;
    if (!strcmp(c, "/"))
        return 5;
    else
        return 6;
}

//adaugarea elementelor in notatia postfixata
void adaugaincoada(coada& postfix, stiva& S) {
    //verificam daca este functie
    if (S.varf->info == 's') {
        enqueue(postfix, 's');
        enqueue(postfix, 'i');
        enqueue(postfix, 'n');
    }
    else if (S.varf->info == 'c') {
        enqueue(postfix, 'c');
        enqueue(postfix, 'o');
        enqueue(postfix, 's');
    }
    else if (S.varf->info == 't') {
        enqueue(postfix, 't');
        enqueue(postfix, 'g');
    }
    else if (S.varf->info == 'g') {
        enqueue(postfix, 'c');
        enqueue(postfix, 't');
        enqueue(postfix, 'g');
    }
    else if (S.varf->info == 'l') {
        enqueue(postfix, 'l');
        enqueue(postfix, 'o');
        enqueue(postfix, 'g');
    }
    else if (S.varf->info == 'r') {
        enqueue(postfix, 'r');
        enqueue(postfix, 'a');
        enqueue(postfix, 'd');
    }
    else if (S.varf->info == 'e') {
        enqueue(postfix, 'l');
        enqueue(postfix, 'n');
    }
    else if (S.varf->info == 'z') {
        enqueue(postfix, 'l');
        enqueue(postfix, 'g');
    }
    else
        enqueue(postfix, S.varf->info);

    pop(S);
}

//adaugam in stiva operatorii
void adaugainstiva(stiva& S, coada& postfix, char& x, int& i) {

    //verificam daca este functie
    if (x == 's' && infix[i + 1] == 'i' && infix[i + 2] == 'n') {
        push(S, 's');
        i = i + 2;
    }
    else if (x == 'c' && infix[i + 1] == 'o' && infix[i + 2] == 's') {
        push(S, 'c');
        i = i + 2;
    }
    else if (x == 't' && infix[i + 1] == 'g') {
        push(S, 't');
        i = i + 1;
    }
    else if (x == 'c' && infix[i + 1] == 't' && infix[i + 2] == 'g') {
        push(S, 'g');
        i = i + 2;
    }
    else if (x == 'l' && infix[i + 1] == 'o' && infix[i + 2] == 'g') {
        push(S, 'l');
        i = i + 2;
    }
    else if (x == 'r' && infix[i + 1] == 'a' && infix[i + 2] == 'd') {
        push(S, 'r');
        i = i + 2;
    }
    else if (x == 'l' && infix[i + 1] == 'n') {
        push(S, 'e');
        i = i + 1;
    }
    else if (x == 'l' && infix[i + 1] == 'g') {
        push(S, 'z');
        i = i + 1;
    }
    else
        push(S, x); //altfel, este semn
}

bool esteoperator(char infix[], int i) { // construirea notatie postfixate

    if (strchr("+-*/^(),", infix[i])) return 1;
    if (strchr("sctglr", infix[i]) && strchr("iogtan", infix[i + 1]))
        return 1;
    return 0;
}

int esteoperator1(char element[]) {   // construirea arborelui derivatei

    if (strchr("+-*/^", element[0]) && element[1] == NULL) return 2;
    if ((element[0] == 'l') && (element[1] == 'o') && (element[2] == 'g')) return 2;
    if ((element[0] == 'r') && (element[1] == 'a') && (element[2] == 'd')) return 2;
    if (strchr("sctgl", element[0]) && strchr("iogtn", element[1]) && element[1] != NULL)
        return 1;
    return 0;
}

int esteoperator2(char element[]) {    //parcurgerea arborelui

    if (strchr("+-*/^", element[0]) && element[1] == NULL) return 2;
    if ((element[0] == 'l') && (element[1] == 'o') && (element[2] == 'g')) return 1;
    if ((element[0] == 'r') && (element[1] == 'a') && (element[2] == 'd')) return 1;
    if (strchr("sctgl", element[0]) && strchr("iogtn", element[1]) && element[1] != NULL)
        return 1;
    return 0;
}

void convInfix2Postfix(char infix[], coada& postfix) {

    int i, lg = strlen(infix), j = 0;
    char x;
    stiva S;
    initializeazastiva(S);

    // verificam daca primul numar este negativ
    if (infix[j] == '-') {
        enqueue(postfix, '-');
        j = j + 1;
    }

    // parcurgem expresia
    for (i = j; i < lg; i++) {
        x = infix[i];
        if (!esteoperator(infix, i))                       // verificam daca x este operand (ex: a,b,c,d,1,2,3)
            enqueue(postfix, x);
        else {                                              // exista operator(ex: +,-,*,/,^,sin,log)

            if (x != '(' && infix[i - 1] != ')' && !strchr("sctglrez", x))
                enqueue(postfix, ',');

            if (x == '(' && infix[i + 1] == '-') {          //verificam daca numarul este negativ
                enqueue(postfix, '-');
                i++;
            }

            if (x == ')') {
                /* daca elementul este o paranteza dreapta, extragem operatorii din
                                                              stiva si ii adaugam la forma postfixata pana cand varful stivei
                                                              ajunge la o paranteza stanga (care este extrasa, dar nu este adaugata
                                                              la forma postfixata)*/
                while (S.varf->info != '(') {
                    adaugaincoada(postfix, S);
                    enqueue(postfix, ',');
                }

                pop(S);
            }
            else if (x != ',') {
                /* cat timp exista un alt operator O2 în varful stivei si prioritatea
                sa este mai mare sau egala decat cea a operatorului curent O1,
                adauga O2 la notatia postfixata si extrage O2 din stiva*/
                while (!esteVida(S) && (S.varf->info != '(') && (prioritate(S.varf->info) >= prioritate(x))) {
                    adaugaincoada(postfix, S);
                    enqueue(postfix, ',');
                }
                adaugainstiva(S, postfix, x, i);          //adauga O1 in stiva
            }
        }
    }
    if (!esteoperator(infix, i - 1))
        enqueue(postfix, ',');

    while (S.nrElemente > 1) {                              //extragem toate elementele ramase in stiva si le adaugam la forma postfixata
        adaugaincoada(postfix, S);
        enqueue(postfix, ',');
    }

    adaugaincoada(postfix, S);
}


int nrNiveluri(arbore* a) {

    if (a == NULL)return 0;
    else {
        int n1 = nrNiveluri(a->st);
        int n2 = nrNiveluri(a->dr);
        return 1 + max(n1, n2);
    }
}

int nrColoane(arbore* a) {

    if (a == NULL)return 0;
    else {
        int n1 = nrColoane(a->st);
        int n2 = nrColoane(a->dr);
        return 1 + n1 + n2;
    }
}

void deseneazaNod(char elem[], int xc, int yc, int latime, int inaltime) {
    char arr[50];
    sprintf(arr, "%s", elem);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    fillellipse(xc, yc, 23, 23);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    setbkcolor(LIGHTCYAN);
    setcolor(BLACK);
    settextjustify(1, 1);
    settextstyle(3, 0, 2);
    outtextxy(xc, yc + 4, arr);
}

void deseneazaArbore(arbore* a, int niv, int stanga, int latime, int inaltime) {

    if (a != NULL) {
        int n1 = nrColoane(a->st);
        int yc = niv * inaltime - inaltime / 2;
        int xc = stanga + latime * n1 + latime / 2;

        if (a->st != NULL) {
            int xcs = stanga + latime * nrColoane(a->st->st) + latime / 2;
            setcolor(WHITE);
            line(xc, yc, xcs, yc + inaltime);
        }

        if (a->dr != NULL) {
            int xcd = stanga + latime * (n1 + 1) + latime * nrColoane(a->dr->st) + latime / 2;
            setcolor(WHITE);
            line(xc, yc, xcd, yc + inaltime);
        }
        deseneazaArbore(a->st, niv + 1, stanga, latime, inaltime);
        deseneazaArbore(a->dr, niv + 1, stanga + latime * (n1 + 1), latime, inaltime);
        deseneazaNod(a->info, xc, yc, latime, inaltime);
    }
}

//adaugam notatia postfixata in stiva de arbori
void adaugaLaArboreElement(coada C, stivaarbore& S) {
    int i = 0;
    arbore* arbore_nou;
    char element[10];
    memset(element, '\0', 10); // reinitializare sir de caractere

    while (C.prim != NULL) {
        if (C.prim->info == ',') {
            if (!(esteoperator1(element))) { // este operand
                arbore_nou = new arbore;
                strcpy(arbore_nou->info, element);
                arbore_nou->st = NULL;
                arbore_nou->dr = NULL;
                pusharbore(S, arbore_nou);
            }
            else if (esteoperator1(element) == 2) { // este semn (+,-,*,/,^) sau functie binara (log, rad, e)
                arbore_nou = new arbore;
                strcpy(arbore_nou->info, element);
                arbore_nou->dr = S.varf->info;
                poparbore(S);
                arbore_nou->st = S.varf->info;
                poparbore(S);
                pusharbore(S, arbore_nou);
            }
            else { // este functie unara (sin, cos, tg, ctg, ln, lg)
                arbore_nou = new arbore;
                strcpy(arbore_nou->info, element);
                arbore_nou->dr = NULL;
                arbore_nou->st = S.varf->info;
                poparbore(S);
                pusharbore(S, arbore_nou);
            }
            memset(element, '\0', strlen(element));
            i = 0;
        }
        else
            element[i++] = C.prim->info;
        C.prim = C.prim->urm;
    }

    if (esteoperator1(element) == 2) {
        arbore_nou = new arbore;
        strcpy(arbore_nou->info, element);
        arbore_nou->dr = S.varf->info;
        poparbore(S);
        arbore_nou->st = S.varf->info;
        poparbore(S);
        pusharbore(S, arbore_nou);
    }
    else {
        arbore_nou = new arbore;
        strcpy(arbore_nou->info, element);
        arbore_nou->dr = NULL;
        arbore_nou->st = S.varf->info;
        poparbore(S);
        pusharbore(S, arbore_nou);
    }
}

arbore* simplificare(arbore* a) {
    if (!strcmp(a->info, "*")) {
        if (!strcmp(a->st->info, "0") || !strcmp(a->dr->info, "0")) {
            strcpy(a->info, "0");
            a->st = NULL;
            a->dr = NULL;
        }
        else if (!strcmp(a->st->info, "1")) {
            a = a->dr;
        }
        else if (!strcmp(a->dr->info, "1")) {
            a = a->st;
        }
    }

    if (!strcmp(a->info, "/")) {
        if (!strcmp(a->st->info, "0")) {
            strcpy(a->info, "0");
            a->st = NULL;
            a->dr = NULL;
        }
        else if (!strcmp(a->dr->info, "1")) {
            a = a->st;
        }
    }

    if (!strcmp(a->info, "+")) {
        if (!strcmp(a->st->info, "0")) {
            a = a->dr;
        }
        else if (!strcmp(a->dr->info, "0")) {
            a = a->st;
        }
        else if (!strcmp(a->dr->info, "-")) {
            a->dr = a->dr->dr;
            strcpy(a->info, "-");
        }
    }

    if (!strcmp(a->info, "-")) {
        if (!strcmp(a->dr->info, "0")) {
            a = a->st;
        }
        else if (!strcmp(a->dr->info, "-")) {
            a->dr = a->dr->dr;
            strcpy(a->info, "+");
        }
    }

    if (!strcmp(a->info, "^")) {
        if (!strcmp(a->st->info, "0")) {
            strcpy(a->info, "0");
            a->st = NULL;
            a->dr = NULL;
        }
        else if (!strcmp(a->dr->info, "0")) {
            strcpy(a->info, "1");
            a->st = NULL;
            a->dr = NULL;
        }
        else if (!strcmp(a->st->info, "1")) {
            strcpy(a->info, "1");
            a->st = NULL;
            a->dr = NULL;
        }
        else if (!strcmp(a->dr->info, "1")) {
            a = a->st;
        }
    }

    if (!strcmp(a->info, "ln")) {
        if (!strcmp(a->st->info, "e")) {
            strcpy(a->info, "1");
            a->st = NULL;
            a->dr = NULL;
        }
    }

    return a;
}

arbore* deriveaza(arbore* a, arbore* d) {
    arbore* a1, * a2, * a3, * a4, * a5, * a6, * a7, * a8;

    if (strchr("0123456789", a->info[0])) strcpy(d->info, "0"); // numere pozitive
    if ((a->info[0] == '-') && strchr("0123456789", a->info[1])) strcpy(d->info, "0"); //numere negative

    if (a->info[0] == 'x') strcpy(d->info, "1"); // este variabila

    if (((a->info[0] == '+') || (a->info[0] == '-')) && (a->info[1] == NULL)) { // derivam adunarea si scaderea
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        deriveaza(a->st, a1);
        a1 = simplificare(a1);
        d->st = a1;
        deriveaza(a->dr, a2);
        a2 = simplificare(a2);
        d->dr = a2;
        strcpy(d->info, a->info);
        d = simplificare(d);
    }

    if (a->info[0] == '*') { // derivam inmultirea
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        a3 = new arbore;
        a3->st = NULL;
        a3->dr = NULL;
        a4 = new arbore;
        a4->st = NULL;
        a4->dr = NULL;
        deriveaza(a->st, a1);
        a1 = simplificare(a1);
        deriveaza(a->dr, a2);
        a2 = simplificare(a2);
        strcpy(a3->info, "*");
        a3->st = a1;
        a3->dr = a->dr;
        a3 = simplificare(a3);
        strcpy(a4->info, "*");
        a4->st = a->st;
        a4->dr = a2;
        a4 = simplificare(a4);
        strcpy(d->info, "+");
        d->st = a3;
        d->dr = a4;
        d = simplificare(d);
    }

    if (a->info[0] == '/') { // derivam impartirea
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        a3 = new arbore;
        a3->st = NULL;
        a3->dr = NULL;
        a4 = new arbore;
        a4->st = NULL;
        a4->dr = NULL;
        a5 = new arbore;
        a5->st = NULL;
        a5->dr = NULL;
        a6 = new arbore;
        a6->st = NULL;
        a6->dr = NULL;
        a7 = new arbore;
        a7->st = NULL;
        a7->dr = NULL;
        strcpy(a7->info, "2");
        deriveaza(a->st, a1);
        a1 = simplificare(a1);
        deriveaza(a->dr, a2);
        a2 = simplificare(a2);
        strcpy(a3->info, "*");
        a3->st = a1;
        a3->dr = a->dr;
        a3 = simplificare(a3);
        strcpy(a4->info, "*");
        a4->st = a->st;
        a4->dr = a2;
        a4 = simplificare(a4);
        strcpy(a5->info, "-");
        a5->st = a3;
        a5->dr = a4;
        a5 = simplificare(a5);
        strcpy(a6->info, "^");
        a6->st = a->dr;
        a6->dr = a7;
        a6 = simplificare(a6);
        strcpy(d->info, "/");
        d->st = a5;
        d->dr = a6;
        d = simplificare(d);
    }

    if (a->info[0] == '^') { // derivam ridicarea la puterea/exponentiala
        if (strchr("0123456789e", a->st->info[0])) { // derivam exponentiala
            a1 = new arbore;
            a1->st = NULL;
            a1->dr = NULL;
            a2 = new arbore;
            a2->st = a->st;
            a2->dr = NULL;
            strcpy(a2->info, "ln");
            a2 = simplificare(a2);
            a3 = new arbore;
            a3->st = a;
            a3->dr = a2;
            strcpy(a3->info, "*");
            a3 = simplificare(a3);
            deriveaza(a->dr, a1);
            a1 = simplificare(a1);
            strcpy(d->info, "*");
            d->st = a3;
            d->dr = a1;
            d = simplificare(d);
        }
        else { // derivam ridicarea la putere a lui x
            a1 = new arbore;
            a1->st = NULL;
            a1->dr = NULL;
            a2 = new arbore;
            a2->st = NULL;
            a2->dr = NULL;
            strcpy(a2->info, "1");
            a3 = new arbore;
            a3->st = a->dr;
            a3->dr = a2;
            strcpy(a3->info, "-");
            a3 = simplificare(a3);
            a3->info[0] = '0' + a3->st->info[0] - a3->dr->info[0];
            a3->st = NULL;
            a3->dr = NULL;
            a4 = new arbore;
            a4->st = a->st;
            a4->dr = a3;
            strcpy(a4->info, "^");
            a4 = simplificare(a4);
            a5 = new arbore;
            a5->st = a->dr;
            a5->dr = a4;
            strcpy(a5->info, "*");
            a5 = simplificare(a5);
            deriveaza(a->st, a1);
            a1 = simplificare(a1);
            strcpy(d->info, "*");
            d->st = a5;
            d->dr = a1;
            d = simplificare(d);
        }
    }

    if (!strcmp("sin", a->info)) { // derivam sinus
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        a2 = new arbore;
        a2->st = a->st;
        a2->dr = NULL;
        strcpy(a2->info, "cos");
        deriveaza(a->st, a1);
        a1 = simplificare(a1);
        strcpy(d->info, "*");
        d->st = a2;
        d->dr = a1;
        d = simplificare(d);
    }

    if (!strcmp("cos", a->info)) { // derivam cosinus
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        a2 = new arbore;
        a2->st = a->st;
        a2->dr = NULL;
        strcpy(a2->info, "sin");
        a3 = new arbore;
        a3->st = NULL;
        a3->dr = NULL;
        deriveaza(a->st, a1);
        a1 = simplificare(a1);
        strcpy(a3->info, "*");
        a3->st = a2;
        a3->dr = a1;
        a3 = simplificare(a3);
        strcpy(d->info, "-");
        d->st = NULL;
        d->dr = a3;
        d = simplificare(d);
    }

    if (!strcmp("tg", a->info)) { // derivam tangent
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        strcpy(a2->info, "2");
        a3 = new arbore;
        a3->st = a->st;
        a3->dr = NULL;
        strcpy(a3->info, "cos");
        a4 = new arbore;
        a4->st = a3;
        a4->dr = a2;
        strcpy(a4->info, "^");
        a4 = simplificare(a4);
        a5 = new arbore;
        a5->st = a1;
        a5->dr = a4;
        strcpy(a5->info, "/");
        a5 = simplificare(a5);
        a6 = new arbore;
        a6->st = NULL;
        a6->dr = NULL;
        deriveaza(a->st, a6);
        a6 = simplificare(a6);
        strcpy(d->info, "*");
        d->st = a5;
        d->dr = a6;
        d = simplificare(d);
    }

    if (!strcmp("ctg", a->info)) { // derivam cotangent
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        strcpy(a2->info, "2");
        a3 = new arbore;
        a3->st = a->st;
        a3->dr = NULL;
        strcpy(a3->info, "sin");
        a4 = new arbore;
        a4->st = a3;
        a4->dr = a2;
        strcpy(a4->info, "^");
        a4 = simplificare(a4);
        a5 = new arbore;
        a5->st = a1;
        a5->dr = a4;
        strcpy(a5->info, "/");
        a5 = simplificare(a5);
        a6 = new arbore;
        a6->st = NULL;
        a6->dr = NULL;
        deriveaza(a->st, a6);
        a6 = simplificare(a6);
        a8 = new arbore;
        a8->st = a5;
        a8->dr = a6;
        strcpy(a8->info, "*");
        a8 = simplificare(a8);
        strcpy(d->info, "-");
        d->st = NULL;
        d->dr = a8;
        d = simplificare(d);
    }

    if (!strcmp("ln", a->info)) { // derivam logaritm natural
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = a1;
        a2->dr = a->st;
        strcpy(a2->info, "/");
        a2 = simplificare(a2);
        a3 = new arbore;
        a3->st = NULL;
        a3->dr = NULL;
        deriveaza(a->st, a3);
        a3 = simplificare(a3);
        strcpy(d->info, "*");
        d->st = a2;
        d->dr = a3;
        d = simplificare(d);
    }

    if (!strcmp("log", a->info)) { // derivam logaritm
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = a->st;
        a2->dr = NULL;
        strcpy(a2->info, "ln");
        a3 = new arbore;
        a3->st = a->dr;
        a3->dr = a2;
        strcpy(a3->info, "*");
        a3 = simplificare(a3);
        a4 = new arbore;
        a4->st = a1;
        a4->dr = a3;
        strcpy(a4->info, "/");
        a4 = simplificare(a4);
        a5 = new arbore;
        a5->st = NULL;
        a5->dr = NULL;
        deriveaza(a->dr, a5);
        a5 = simplificare(a5);
        strcpy(d->info, "*");
        d->st = a4;
        d->dr = a5;
        d = simplificare(d);
    }

    if (!strcmp("lg", a->info)) { // derivam logaritm zecimal
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = NULL;
        a2->dr = NULL;
        strcpy(a2->info, "10");
        a3 = new arbore;
        a3->st = a2;
        a3->dr = NULL;
        strcpy(a3->info, "ln");
        a4 = new arbore;
        a4->st = a->st;
        a4->dr = a3;
        strcpy(a4->info, "*");
        a4 = simplificare(a4);
        a5 = new arbore;
        a5->st = a1;
        a5->dr = a4;
        strcpy(a5->info, "/");
        a5 = simplificare(a5);
        a6 = new arbore;
        a6->st = NULL;
        a6->dr = NULL;
        deriveaza(a->st, a6);
        a6 = simplificare(a6);
        strcpy(d->info, "*");
        d->st = a5;
        d->dr = a6;
        d = simplificare(d);
    }

    if (!strcmp("rad", a->info)) { // derivam radical
        a1 = new arbore;
        a1->st = NULL;
        a1->dr = NULL;
        strcpy(a1->info, "1");
        a2 = new arbore;
        a2->st = a->st;
        a2->dr = a1;
        strcpy(a2->info, "-");
        a2 = simplificare(a2);
        a2->info[0] = '0' + a2->st->info[0] - a2->dr->info[0];
        a2->st = NULL;
        a2->dr = NULL;
        a3 = new arbore;
        a3->st = a->dr;
        a3->dr = a2;
        strcpy(a3->info, "^");
        a3 = simplificare(a3);
        a4 = new arbore;
        a4->st = a->st;
        a4->dr = a3;
        strcpy(a4->info, "rad");
        a5 = new arbore;
        a5->st = a->st;
        a5->dr = a4;
        strcpy(a5->info, "*");
        a5 = simplificare(a5);
        a6 = new arbore;
        a6->st = a1;
        a6->dr = a5;
        strcpy(a6->info, "/");
        a6 = simplificare(a6);
        a7 = new arbore;
        a7->st = NULL;
        a7->dr = NULL;
        deriveaza(a->dr, a7);
        a7 = simplificare(a7);
        strcpy(d->info, "*");
        d->st = a6;
        d->dr = a7;
        d = simplificare(d);
    }
    return d;
}

// parcurgem arborele si retinem formula intr-un sir de caractere
void parcurgere(arbore* a, char deriv[]) {
    if (a != NULL) {
        if (esteoperator2(a->info) == 1) { // parcurgem in preordine (pentru functii: sin, cos, tg, ctg, ln, lg, log, rad)
            strcat(deriv, a->info);
            strcat(deriv, "(");
            parcurgere(a->st, deriv);
            if (((a->info[0] == 'l') && (a->info[1] == 'o') && (a->info[2] == 'g')) || ((a->info[0] == 'r') && (a->info[1] == 'a') && (a->info[2] == 'd')))
                strcat(deriv, ",");
            parcurgere(a->dr, deriv);
            strcat(deriv, ")");
        }
        else { // parcurgem in inordine (pentru restul)
            parcurgere(a->st, deriv);
            strcat(deriv, a->info);
            if ((a->dr != NULL) && (prioritate1(a->info) > prioritate1(a->dr->info))) {
                strcat(deriv, "(");
                parcurgere(a->dr, deriv);
                strcat(deriv, ")");
            }
            else parcurgere(a->dr, deriv);
        }

    }
}

// REALIZARE MENIU
void titlu(int x, int y, char* n) {
    settextstyle(4, 0, 5);
    setcolor(3);
    setlinestyle(0, 0, 7);
    readimagefile("fundal.jpg", 0, 0, 1200, 1000);
    rectangle(3, 3, 1197, 100);
    setfillstyle(EMPTY_FILL, 2);
    outtextxy(x, y, n);
}

void butoane() {
    rectangle(3, 100, 420, 185);
    settextstyle(4, 0, 5);
    outtextxy(100, 123, "MENIU");

    setlinestyle(0, 0, 3);
    rectangle(0, 185, 422, 249);
    settextstyle(4, 0, 2);
    outtextxy(160, 210, "INFO");

    rectangle(0, 249, 422, 313);
    settextstyle(4, 0, 2);
    outtextxy(25, 262, "INTRODUCETI FORMULA");
    outtextxy(70, 282, "DE LA TASTATURA");

    rectangle(0, 313, 422, 377);
    settextstyle(4, 0, 2);
    outtextxy(25, 326, "INTRODUCETI FORMULA");
    outtextxy(40, 346, "DIN FISIERUL DERIV.IN");

    rectangle(0, 377, 422, 441);
    settextstyle(4, 0, 2);
    outtextxy(43, 398, "NOTATIE POSTFIXATA");

    rectangle(0, 441, 422, 505);
    settextstyle(4, 0, 2);
    outtextxy(43, 462, "ARBORELE FORMULEI");

    rectangle(0, 505, 422, 569);
    settextstyle(4, 0, 2);
    outtextxy(120, 526, "DERIVATA");

    rectangle(0, 569, 422, 633);
    settextstyle(4, 0, 2);
    outtextxy(22, 590, "DERIVATA DE ORDINUL 2");

    rectangle(0, 633, 422, 697);
    settextstyle(4, 0, 2);
    outtextxy(160, 654, "EXIT");

}

// pentru butoane
void Apasa(int& ok) {
    int coordx, coordy;
    clearmouseclick(WM_LBUTTONDOWN);
    while (!ismouseclick(WM_LBUTTONDOWN)) { // verificam daca se apasa mouse-ul
        coordx = mousex();
        coordy = mousey();
    }

    if (coordx > 0 && coordx < 422 && coordy>185 && coordy < 249) //daca se apasa INFO
        ok = 1;
    else if (coordx > 0 && coordx < 422 && coordy>249 && coordy < 313) //daca se apasa INTRODUCETI FORMULA DE LA TASTATURA
        ok = 2;
    else if (coordx > 0 && coordx < 422 && coordy>313 && coordy < 377) //daca se apasa INTRODUCETI FORMULA DIN FISIER
        ok = 3;
    else if (coordx > 0 && coordx < 422 && coordy>377 && coordy < 441) //daca se apasa NOTATIA POSTFIXATA
        ok = 4;
    else if (coordx > 0 && coordx < 422 && coordy>441 && coordy < 505) //daca se apasa ARBORELE FORMULEI
        ok = 5;
    else if (coordx > 0 && coordx < 422 && coordy>505 && coordy < 569) //daca se apasa DERIVATA
        ok = 6;
    else if (coordx > 0 && coordx < 422 && coordy>569 && coordy < 633) //daca se apasa DERIVATA DE ORDIN 2
        ok = 7;
    else if (coordx > 0 && coordx < 422 && coordy>633 && coordy < 697) //daca se apasa EXIT
        ok = 0;
}

void meniu() {
    int ok;
    Apasa(ok);
    clearmouseclick(WM_LBUTTONDOWN);
    switch (ok) {
        Apasa(ok);
    case 0: { // EXIT
        closegraph();
        break;
    }
    case 1: { // INFORMATII PROIECT
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);
        outtextxy(430, 110, "1.   Programul accepta ca operanzi numere intregi si");
        outtextxy(430, 132, "numere rationale (pentru cele rationale se va folosi '.' ca");
        outtextxy(430, 152, "separator), iar ca si variabila, programul accepta 'x'.");
        outtextxy(430, 182, "2.   Programul accepta urmatoarele functii: +, -, *, /, ^,");
        outtextxy(430, 203, "e^(expr), sin(expr), cos(expr), tg(expr), ctg(expr),");
        outtextxy(430, 224, "rad(ordinul radicalului, expr), log(baza logaritmului, expr), ln(expr), lg(expr)");
        outtextxy(430, 244, "ln(expr), lg(expr).");
        outtextxy(430, 274, "3.   Dupa ce introduceti formula de la tastatura, apasati ");
        outtextxy(430, 294, "tasta ENTER pentru a continua.");
        outtextxy(430, 324, "4.   Notatia postfixata, derivata intai si derivata de ordinul");
        outtextxy(430, 344, "al doilea vor fi salvate in fisierul deriv.out.");
        outtextxy(430, 600, "Proiect realizat de:");
        outtextxy(430, 640, "Popescu Tudor-George");
        outtextxy(430, 660, "Bura Stefan-Gabriel");
        outtextxy(890, 600, "Profesor indrumator:");
        outtextxy(890, 640, "Lect. dr. Patrut Bogdan");
        meniu();
        break;
    }
    case 2: { // CITIRE DE LA TASTATURA
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);
        memset(infix, NULL, 100);
        char c;
        int poz = 0;
        outtextxy(430, 110, "Introduceti formula de la tastatura:");
        c = getch();
        while (c != 13) {
            if (c == 8 && poz) {
                poz--;
                infix[poz] = '\0';
                setactivepage(0);
                titlu(250, 30, "DERIV - Derivare formala");
                butoane();
                bar(423, 103, 1200, 697);
                setvisualpage(0);
                outtextxy(430, 110, "Introduceti formula de la tastatura:");
            }
            else if (c != 8) {
                infix[poz] = c;
                poz++;
                infix[poz] = '\0';
            }
            outtextxy(430, 132, infix);
            c = getch();
        }
        meniu();
        break;
    }

    case 3: { // CITIRE DIN FISIER
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);
        memset(infix, NULL, 100);
        fin >> infix;
        outtextxy(430, 110, "Formula introdusa din fisierul deriv.in este:");
        outtextxy(430, 132, infix);
        meniu();
        break;
    }
    case 4: { // NOTATIA POSTFIXATA
        memset(postfixata, NULL, 100);
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);
        initializeazacoada(postfix);
        convInfix2Postfix(infix, postfix);
        fout << "Notatia postfixata a expresiei " << infix << " este: ";
        afiseaza(postfix);
        fout << endl;
        outtextxy(430, 110, "Notatia postfixata a expresiei");
        outtextxy(430, 132, infix);
        outtextxy(430, 154, "este:");
        outtextxy(430, 176, postfixata);
        meniu();
        break;

    }
    case 5: { // ARBORELE FORMULEI
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);

        initializeazacoada(postfix);
        convInfix2Postfix(infix, postfix);
        stivaarbore S;
        initializeazastivaarbore(S);
        adaugaLaArboreElement(postfix, S);

        outtextxy(430, 110, "Arborele expresiei");
        outtextxy(430, 132, infix);
        outtextxy(430, 154, "este:");

        int height = 697, width = 1200;
        int window1 = initwindow(width, height, "Arborele formulei");
        setcurrentwindow(window1);
        setcolor(WHITE);
        setbkcolor(BLACK);
        cleardevice();
        rectangle(1, 1, width - 1, height - 1);
        int latime, inaltime;
        latime = width / nrColoane(S.varf->info);
        inaltime = height / nrNiveluri(S.varf->info);
        deseneazaArbore(S.varf->info, 1, 0, latime, inaltime);
        setcolor(3);
        setlinestyle(0, 0, 2);
        rectangle(1060, 10, 1190, 50);
        settextstyle(4, 0, 2);
        setbkcolor(BLACK);
        outtextxy(1125, 35, "INAPOI");
        int coordx, coordy;
        clearmouseclick(WM_LBUTTONDOWN);
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            coordx = mousex();
            coordy = mousey();
        }
        if (coordx > 1030 && coordx < 1180 && coordy > 20 && coordy < 70)
            closegraph(CURRENT_WINDOW);

        setcurrentwindow(0);
        meniu();
        break;
    }
    case 6: { // DERIVATA INTAI

        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);

        initializeazacoada(postfix);
        convInfix2Postfix(infix, postfix);
        stivaarbore S;
        initializeazastivaarbore(S);
        adaugaLaArboreElement(postfix, S);

        arbore* d;
        d = new arbore;
        d->st = NULL;
        d->dr = NULL;
        d = deriveaza(S.varf->info, d);

        memset(derivata1, NULL, 200);
        parcurgere(d, derivata1);
        outtextxy(430, 110, "Derivata expresiei");
        outtextxy(430, 132, infix);
        outtextxy(430, 154, "este:");
        if (strlen(derivata1) < 50)
            outtextxy(430, 176, derivata1);
        else if (strlen(derivata1) < 100) {
            strncpy(sir, derivata1, 50);
            sir[50] = NULL;
            outtextxy(430, 176, sir);
            outtextxy(430, 198, derivata1 + 50);
        }
        else {
            strncpy(sir1, derivata1, 50);
            sir1[50] = NULL;
            strncpy(sir2, derivata1 + 50, 50);
            sir2[50] = NULL;
            outtextxy(430, 176, sir1);
            outtextxy(430, 198, sir2);
            outtextxy(430, 220, derivata1 + 100);
        }

        setcolor(3);
        setlinestyle(0, 0, 2);
        rectangle(1020, 110, 1190, 160);
        settextstyle(4, 0, 2);
        setbkcolor(BLACK);
        outtextxy(1031, 114, "AFISEAZA");
        outtextxy(1039, 134, "ARBORE");
        int coordx, coordy;
        clearmouseclick(WM_LBUTTONDOWN);
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            coordx = mousex();
            coordy = mousey();
        }
        if (coordx > 1020 && coordx < 1190 && coordy > 110 && coordy < 160) {
            int height = 697, width = 1200;
            int window2 = initwindow(width, height, "Arborele derivatei");
            setcurrentwindow(window2);
            setcolor(WHITE);
            setbkcolor(BLACK);
            cleardevice();
            rectangle(1, 1, width - 1, height - 1);
            int latime, inaltime;
            latime = width / nrColoane(d);
            inaltime = height / nrNiveluri(d);
            deseneazaArbore(d, 1, 0, latime, inaltime);
            setcolor(3);
            setlinestyle(0, 0, 2);
            rectangle(1060, 10, 1190, 50);
            settextstyle(4, 0, 2);
            setbkcolor(BLACK);
            outtextxy(1125, 35, "INAPOI");
            int coordx, coordy;
            clearmouseclick(WM_LBUTTONDOWN);
            while (!ismouseclick(WM_LBUTTONDOWN)) {
                coordx = mousex();
                coordy = mousey();
            }
            if (coordx > 1030 && coordx < 1180 && coordy > 20 && coordy < 70)
                closegraph(CURRENT_WINDOW);
        }

        fout << "Derivata expresiei " << infix << " este: " << derivata1 << endl;

        setcurrentwindow(0);
        meniu();
        break;
    }
    case 7: { // DERIVATA DE ORDINUL 2
        setactivepage(0);
        titlu(250, 30, "DERIV - Derivare formala");
        butoane();
        bar(423, 103, 1200, 697);
        setvisualpage(0);

        initializeazacoada(postfix);
        convInfix2Postfix(infix, postfix);
        stivaarbore S;
        initializeazastivaarbore(S);
        adaugaLaArboreElement(postfix, S);

        arbore* d;
        d = new arbore;
        d->st = NULL;
        d->dr = NULL;
        d = deriveaza(S.varf->info, d);

        arbore* d1;
        d1 = new arbore;
        d1->st = NULL;
        d1->dr = NULL;
        d1 = deriveaza(d, d1);

        memset(derivata2, NULL, 200);
        parcurgere(d1, derivata2);
        outtextxy(430, 110, "Derivata de ordinul 2 a expresiei");
        outtextxy(430, 132, infix);
        outtextxy(430, 154, "este:");
        if (strlen(derivata2) < 50)
            outtextxy(430, 176, derivata2);
        else if (strlen(derivata2) < 100) {
            strncpy(sir, derivata2, 50);
            sir[50] = NULL;
            outtextxy(430, 176, sir);
            outtextxy(430, 198, derivata2 + 50);
        }
        else {
            strncpy(sir1, derivata2, 50);
            sir1[50] = NULL;
            strncpy(sir2, derivata2 + 50, 50);
            sir2[50] = NULL;
            outtextxy(430, 176, sir1);
            outtextxy(430, 198, sir2);
            outtextxy(430, 220, derivata2 + 100);
        }

        setcolor(3);
        setlinestyle(0, 0, 2);
        rectangle(1020, 110, 1190, 160);
        settextstyle(4, 0, 2);
        setbkcolor(BLACK);
        outtextxy(1031, 114, "AFISEAZA");
        outtextxy(1039, 134, "ARBORE");
        int coordx, coordy;
        clearmouseclick(WM_LBUTTONDOWN);
        while (!ismouseclick(WM_LBUTTONDOWN)) {
            coordx = mousex();
            coordy = mousey();
        }
        if (coordx > 1020 && coordx < 1190 && coordy > 110 && coordy < 160) {
            int height = 697, width = 1200;
            int window3 = initwindow(width, height, "Arborele derivatei de ordinul 2");
            setcurrentwindow(window3);
            setcolor(WHITE);
            setbkcolor(BLACK);
            cleardevice();
            rectangle(1, 1, width - 1, height - 1);
            int latime, inaltime;
            latime = width / nrColoane(d1);
            inaltime = height / nrNiveluri(d1);
            deseneazaArbore(d1, 1, 0, latime, inaltime);
            setcolor(3);
            setlinestyle(0, 0, 2);
            rectangle(1060, 10, 1190, 50);
            settextstyle(4, 0, 2);
            setbkcolor(BLACK);
            outtextxy(1125, 35, "INAPOI");
            int coordx, coordy;
            clearmouseclick(WM_LBUTTONDOWN);
            while (!ismouseclick(WM_LBUTTONDOWN)) {
                coordx = mousex();
                coordy = mousey();
            }
            if (coordx > 1030 && coordx < 1180 && coordy > 20 && coordy < 70)
                closegraph(CURRENT_WINDOW);
        }

        fout << "Derivata de ordinul 2 a expresiei " << infix << " este: " << derivata2 << endl;

        setcurrentwindow(0);
        meniu();
        break;
    }

    }
}


int main() {
    initwindow(1200, 697, "DERIV - Derivare formala");

    setactivepage(0);
    titlu(250, 30, "DERIV - Derivare formala");
    butoane();
    bar(423, 103, 1200, 697);
    setvisualpage(0);

    meniu();
    return 0;
}

