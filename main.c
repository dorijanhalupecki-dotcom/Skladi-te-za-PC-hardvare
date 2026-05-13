#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "komponenta.h"
#include "izbornik.h"

// Enum za opcije izbornika
typedef enum {

	IZLAZ,
	DODAJ = 1,
	PRIKAZI

} IzbornikOpcije;

int main()
{
	// Učitavanje komponenti iz datoteke
	ucitajKomponente();

	// Varijabla za odabir izbornika
	int izbor = -1;

	// Glavna petlja programa
	do
	{
		// Prikaz izbornika
		prikaziIzbornik();

		// Unos korisničkog odabira
		scanf("%d", &izbor);

		// Obrada odabira
		switch (izbor)
		{
		case DODAJ:

			dodajKomponentu();

			break;

		case PRIKAZI:

			prikaziKomponente();

			break;

		case IZLAZ:

			printf("\nIzlaz iz programa...\n");

			break;

		default:

			printf("\nNeispravan odabir!\n");
		}

	} while (izbor != IZLAZ);

	// Oslobađanje memorije
	free(komponente);

	// Anuliranje pokazivača
	komponente = NULL;

	return 0;
}