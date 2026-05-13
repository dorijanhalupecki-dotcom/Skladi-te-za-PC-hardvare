#ifndef KOMPONENTA_H
#define KOMPONENTA_H

// Maksimalna duljina stringova
#define MAX_NAZIV 50
#define MAX_PROIZVODAC 50

// Naziv tekstualne datoteke
#define FILE_NAME "komponente.txt"

// Enum tipova komponenti
typedef enum {

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

// Struktura komponente
typedef struct {

	int id;

	char naziv[MAX_NAZIV];

	char proizvodac[MAX_PROIZVODAC];

	float cijena;

	int kolicina;

	TipKomponente tip;

} Komponenta;

// Globalni pokazivač
extern Komponenta* komponente;

// Broj komponenti
extern int brojKomponenti;

// Funkcije
void dodajKomponentu();

void prikaziKomponente();

void spremiKomponente();

void ucitajKomponente();

const char* getTipKomponente(TipKomponente tip);

#endif