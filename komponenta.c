#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "komponenta.h"

// Globalni pokazivač
Komponenta* komponente = NULL;

// Broj komponenti
int brojKomponenti = 0;

// Čišćenje buffera
static void clearBuffer()
{
	while (getchar() != '\n');
}

// Funkcija za spremanje komponenti
void spremiKomponente()
{
	// Otvaranje datoteke za pisanje
	FILE* file = fopen(FILE_NAME, "w");

	// Provjera datoteke
	if (file == NULL)
	{
		perror("Greska kod otvaranja datoteke");
		return;
	}

	printf("Spremanje u datoteku...\n");

	// Upis svih komponenti
	for (int i = 0; i < brojKomponenti; i++)
	{
		fprintf(file,
			"%d|%s|%s|%.2f|%d|%d\n",

			komponente[i].id,
			komponente[i].naziv,
			komponente[i].proizvodac,
			komponente[i].cijena,
			komponente[i].kolicina,
			komponente[i].tip
		);
	}

	// Zatvaranje datoteke
	fclose(file);
}

// Funkcija za učitavanje komponenti
void ucitajKomponente()
{
	// Otvaranje datoteke za čitanje
	FILE* file = fopen(FILE_NAME, "r");

	// Ako datoteka ne postoji
	if (file == NULL)
	{
		return;
	}

	// Privremena komponenta
	Komponenta temp;

	// Čitanje komponenti
	while (fscanf(file,
		"%d|%49[^|]|%49[^|]|%f|%d|%d\n",

		&temp.id,
		temp.naziv,
		temp.proizvodac,
		&temp.cijena,
		&temp.kolicina,
		(int*)&temp.tip) == 6)
	{
		// Povećavanje memorije
		Komponenta* novi = realloc(
			komponente,
			(brojKomponenti + 1) * sizeof(Komponenta)
		);

		// Provjera realloc
		if (novi == NULL)
		{
			perror("Greska realloc");

			fclose(file);

			return;
		}

		// Novi pokazivač
		komponente = novi;

		// Dodavanje komponente
		komponente[brojKomponenti] = temp;

		// Povećavanje broja komponenti
		brojKomponenti++;
	}

	// Zatvaranje datoteke
	fclose(file);
}

// Funkcija za dodavanje komponente
void dodajKomponentu()
{
	// Nova komponenta
	Komponenta nova;

	printf("\n=== Dodavanje komponente ===\n");

	// Unos ID-a
	printf("ID: ");
	scanf("%d", &nova.id);

	// Čišćenje buffera
	clearBuffer();

	// Unos naziva
	printf("Naziv: ");

	fgets(nova.naziv, MAX_NAZIV, stdin);

	// Uklanjanje '\n'
	nova.naziv[strcspn(nova.naziv, "\n")] = '\0';

	// Unos proizvođača
	printf("Proizvodac: ");

	fgets(nova.proizvodac, MAX_PROIZVODAC, stdin);

	// Uklanjanje '\n'
	nova.proizvodac[strcspn(nova.proizvodac, "\n")] = '\0';

	// Unos cijene
	printf("Cijena: ");
	scanf("%f", &nova.cijena);

	// Unos količine
	printf("Kolicina: ");
	scanf("%d", &nova.kolicina);

	// Ispis tipova
	printf("\nTip komponente:\n");

	printf("1. CPU\n");
	printf("2. GPU\n");
	printf("3. RAM\n");
	printf("4. MBO\n");
	printf("5. SSD\n");
	printf("6. PSU\n");
	printf("7. VEN\n");
	printf("8. COL\n");
	printf("9. KUC\n");

	// Unos tipa
	printf("Odabir: ");

	scanf("%d", (int*)&nova.tip);

	// Povećavanje memorije
	Komponenta* temp = realloc(
		komponente,
		(brojKomponenti + 1) * sizeof(Komponenta)
	);

	// Provjera realloc
	if (temp == NULL)
	{
		printf("Greska kod alokacije memorije!\n");
		return;
	}

	// Novi pokazivač
	komponente = temp;

	// Dodavanje komponente
	komponente[brojKomponenti] = nova;

	// Povećavanje broja komponenti
	brojKomponenti++;

	// Spremanje u datoteku
	spremiKomponente();

	printf("\nKomponenta uspjesno dodana!\n");
}

// Funkcija za pretvaranje enum u string
const char* getTipKomponente(TipKomponente tip)
{
	switch (tip)
	{
	case CPU:
		return "CPU";

	case GPU:
		return "GPU";

	case RAM:
		return "RAM";

	case MBO:
		return "MBO";

	case SSD:
		return "SSD";

	case PSU:
		return "PSU";

	case VEN:
		return "VENTILATOR";

	case COL:
		return "COOLER";

	case KUC:
		return "KUCISTE";

	default:
		return "NEPOZNATO";
	}
}

// Funkcija za prikaz komponenti
void prikaziKomponente()
{
	// Provjera postoji li komponenti
	if (brojKomponenti == 0)
	{
		printf("\nNema komponenti!\n");
		return;
	}

	printf("\n=== Popis komponenti ===\n");

	// Prolazak kroz sve komponente
	for (int i = 0; i < brojKomponenti; i++)
	{
		printf("\nKomponenta #%d\n", i + 1);

		printf("ID: %d\n", komponente[i].id);

		printf("Naziv: %s\n", komponente[i].naziv);

		printf("Proizvodac: %s\n",
			komponente[i].proizvodac);

		printf("Cijena: %.2f EUR\n",
			komponente[i].cijena);

		printf("Kolicina: %d\n",
			komponente[i].kolicina);

		printf("Tip: %s\n",
			getTipKomponente(komponente[i].tip));
	}
}