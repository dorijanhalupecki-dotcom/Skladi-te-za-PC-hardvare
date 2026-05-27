#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "komponenta.h"

Komponenta* komponente = NULL;
int brojKomponenti = 0;

static void clearBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int postojiKomponenta(const char* id)
{
    for (int i = 0; i < brojKomponenti; i++)
    {
        if (strcmp(komponente[i].id, id) == 0)
            return i;
    }
    return -1;
}

void spremiKomponente()
{
    FILE* file = fopen(FILE_NAME, "w");
    if (!file) return;

    for (int i = 0; i < brojKomponenti; i++)
    {
        fprintf(file, "%s|%s|%s|%.2f|%d|%d|%d.%d.%d\n",
            komponente[i].id,
            komponente[i].naziv,
            komponente[i].proizvodac,
            komponente[i].cijena,
            komponente[i].kolicina,
            komponente[i].tip,
            komponente[i].datumDolaska.dan,
            komponente[i].datumDolaska.mjesec,
            komponente[i].datumDolaska.godina);
    }

    fclose(file);
}

void ucitajKomponente()
{
    FILE* file = fopen(FILE_NAME, "r");
    if (!file) return;

    Komponenta temp;

    while (fscanf(file,
        "%29[^|]|%49[^|]|%49[^|]|%f|%d|%d|%d.%d.%d\n",
        temp.id,
        temp.naziv,
        temp.proizvodac,
        &temp.cijena,
        &temp.kolicina,
        (int*)&temp.tip,
        &temp.datumDolaska.dan,
        &temp.datumDolaska.mjesec,
        &temp.datumDolaska.godina) == 9)
    {
        Komponenta* novi = realloc(komponente, (brojKomponenti + 1) * sizeof(Komponenta));
        if (!novi) return;

        komponente = novi;
        komponente[brojKomponenti++] = temp;
    }

    fclose(file);
}

void dodajKomponentu()
{
    Komponenta nova;

    clearBuffer();

    printf("\n=== Dodavanje ===\n");

    printf("ID: ");
    fgets(nova.id, MAX_ID, stdin);
    nova.id[strcspn(nova.id, "\n")] = 0;

    int idx = postojiKomponenta(nova.id);

    if (idx != -1)
    {
        printf("Postoji, dodaj kolicinu: ");
        int k;
        scanf("%d", &k);

        komponente[idx].kolicina += k;
        spremiKomponente();
        return;
    }

    clearBuffer();

    printf("Naziv: ");
    fgets(nova.naziv, MAX_NAZIV, stdin);
    nova.naziv[strcspn(nova.naziv, "\n")] = 0;

    printf("Proizvodac: ");
    fgets(nova.proizvodac, MAX_PROIZVODAC, stdin);
    nova.proizvodac[strcspn(nova.proizvodac, "\n")] = 0;

    printf("Cijena: ");
    scanf("%f", &nova.cijena);

    printf("Kolicina: ");
    scanf("%d", &nova.kolicina);

    printf("Tip (1-9): ");
    scanf("%d", (int*)&nova.tip);

    Komponenta* tmp = realloc(komponente, (brojKomponenti + 1) * sizeof(Komponenta));
    if (!tmp) return;

    komponente = tmp;
    komponente[brojKomponenti++] = nova;

    spremiKomponente();

    printf("Dodano!\n");
}

void prikaziKomponente()
{
    if (brojKomponenti == 0)
    {
        printf("Nema!\n");
        return;
    }

    char buffer[10];
    int sort = 0;

    printf("\nSortiraj po cijeni? (1=da, 0=ne): ");
    fgets(buffer, sizeof(buffer), stdin);
    sort = atoi(buffer);

    Komponenta* kopija = malloc(brojKomponenti * sizeof(Komponenta));
    if (!kopija)
    {
        printf("Greska u memoriji!\n");
        return;
    }

    for (int i = 0; i < brojKomponenti; i++)
        kopija[i] = komponente[i];

    if (sort == 1)
    {
        for (int i = 0; i < brojKomponenti - 1; i++)
        {
            for (int j = 0; j < brojKomponenti - i - 1; j++)
            {
                if (kopija[j].cijena > kopija[j + 1].cijena)
                {
                    Komponenta t = kopija[j];
                    kopija[j] = kopija[j + 1];
                    kopija[j + 1] = t;
                }
            }
        }
    }

    printf("\n=== POPIS KOMPONENTI ===\n");

    for (int i = 0; i < brojKomponenti; i++)
    {
        printf("%s | %.2f EUR | %d kom\n",
            kopija[i].id,
            kopija[i].cijena,
            kopija[i].kolicina);
    }

    free(kopija);
}

void prikaziKomponenteFiltrirano()
{
    int t;
    printf("Tip (0 sve): ");
    scanf("%d", &t);

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (t == 0 || komponente[i].tip == t)
        {
            printf("%s | %.2f\n",
                komponente[i].id,
                komponente[i].cijena);
        }
    }
}

void azurirajKomponentu()
{
    char id[MAX_ID];
    clearBuffer();

    printf("ID: ");
    fgets(id, MAX_ID, stdin);
    id[strcspn(id, "\n")] = 0;

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (strcmp(komponente[i].id, id) == 0)
        {
            printf("Nova cijena: ");
            scanf("%f", &komponente[i].cijena);

            printf("Nova kolicina: ");
            scanf("%d", &komponente[i].kolicina);

            spremiKomponente();
            return;
        }
    }

    printf("Nije nadeno!\n");
}

void obrisiKomponentu()
{
    char id[MAX_ID];
    clearBuffer();

    printf("ID za brisanje: ");
    fgets(id, MAX_ID, stdin);
    id[strcspn(id, "\n")] = 0;

    for (int i = 0; i < brojKomponenti; i++)
    {
        if (strcmp(komponente[i].id, id) == 0)
        {
            for (int j = i; j < brojKomponenti - 1; j++)
                komponente[j] = komponente[j + 1];

            brojKomponenti--;
            spremiKomponente();

            printf("Obrisano!\n");
            return;
        }
    }

    printf("Nije nadeno!\n");
}

const char* getTipKomponente(TipKomponente tip)
{
    switch (tip)
    {
    case CPU: return "CPU";
    case GPU: return "GPU";
    case RAM: return "RAM";
    case MBO: return "MBO";
    case SSD: return "SSD";
    case PSU: return "PSU";
    case VEN: return "VEN";
    case COL: return "COL";
    case KUC: return "KUC";
    default: return "??";
    }
}