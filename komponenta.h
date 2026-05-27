#ifndef KOMPONENTA_H
#define KOMPONENTA_H

#include <stdio.h>

#define MAX_ID 30
#define MAX_NAZIV 50
#define MAX_PROIZVODAC 50
#define FILE_NAME "komponente.txt"

typedef struct
{
    int dan;
    int mjesec;
    int godina;
} Datum;

typedef enum
{
    CPU = 1,
    GPU,
    RAM,
    MBO,
    SSD,
    PSU,
    VEN,
    COL,
    KUC
} TipKomponente;

typedef struct
{
    char id[MAX_ID];
    char naziv[MAX_NAZIV];
    char proizvodac[MAX_PROIZVODAC];
    float cijena;
    int kolicina;
    TipKomponente tip;
    Datum datumDolaska;
} Komponenta;

extern Komponenta* komponente;
extern int brojKomponenti;

void ucitajKomponente();
void spremiKomponente();

void dodajKomponentu();
void prikaziKomponente();
void prikaziKomponenteFiltrirano();

void azurirajKomponentu();
void obrisiKomponentu();

void sortirajPoCijeni();

const char* getTipKomponente(TipKomponente tip);

#endif