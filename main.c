#include <stdio.h>
#include <stdlib.h>

#include "komponenta.h"
#include "izbornik.h"

int main()
{
    ucitajKomponente();

    int izbor;

    do
    {
        prikaziIzbornik();
        scanf("%d", &izbor);

        switch (izbor)
        {
        case 1: dodajKomponentu(); break;
        case 2: prikaziKomponente(); break;
        case 3: sortirajPoCijeni(); break;
        case 4: azurirajKomponentu(); break;
        case 5: obrisiKomponentu(); break;
        case 0: printf("Izlaz\n"); break;
        default: printf("Greska\n");
        }

    } while (izbor != 0);

    free(komponente);

    return 0;
}