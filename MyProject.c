#include "uart.h"
#include "displej.h"
#include "tasteri.h"
#include "poruke.h"
#include "fazaIgre.h"

extern unsigned char fazaIgre;
extern unsigned char brojSlovaPrimanjePoruke = 0;
extern const unsigned char duzineBrodova[8] = { 4, 3, 2, 2, 1, 1, 1, 1 };
extern unsigned char brodZaSetovanje = 0;
extern char porukaZaSlanje[6];
extern char porukaZaPrimanje[6];
extern unsigned char x = 3;
extern unsigned char y = 3;

static unsigned char potopljenoBrodova = 0;
static unsigned char unistioBrodova = 0;

void main()
{
    unsigned char xx, yy;

    initDisplej();
    UART_Config(1200);
    initTasteri();
    initPoruke();

    initFazeIgre();

    while (1)
    {

        if (fazaIgre == SETOVANJE_BRODOVA)
        {
            prikazNaDisplej();
            reagovanjeNaTastere();

            if (brodZaSetovanje < 8)
            {
                iscrtajBrod();
            }
        }
        else if (fazaIgre == CEKANJE_SETOVANJA_PROTVNIKA)
        {
            prikaziInformaciju(ZELENA);
            if (brojSlovaPrimanjePoruke >= 4)
            {
                if (porukaZaPrimanje[0] == 'p' && porukaZaPrimanje[1] == 'r' && porukaZaPrimanje[2] == 'v'
                    && porukaZaPrimanje[3] == 'i')
                {
                    posaljiPoruku("drugi");
                    fazaIgre = PRIHVATANJE_POGOTKA;
                }
                else
                {
                    fazaIgre = GADJANJE;
                }
                brojSlovaPrimanjePoruke = 0;
            }
            else
            {
                posaljiPorukuSaKasnjenjem("prvi");
            }
        }
        else if (fazaIgre == USPOSTAVLJANJE_KONEKCIJE)
        {
            prikaziInformaciju(ZELENA);
            if (brojSlovaPrimanjePoruke >= 5)
            {
                if (porukaZaPrimanje[0] == 's' && porukaZaPrimanje[1] == 't' && porukaZaPrimanje[2] == 'a'
                    && porukaZaPrimanje[3] == 'r' && porukaZaPrimanje[4] == 't')
                {
                    fazaIgre = SETOVANJE_BRODOVA;
                    posaljiPoruku("start");
                }
                brojSlovaPrimanjePoruke = 0;
            }
            else
            {
                posaljiPorukuSaKasnjenjem("start");
            }
        }
        else if (fazaIgre == GADJANJE)
        {
            prikazNaDisplej();
            reagovanjeNaTastere();
            iscrtajKursor();
        }
        else if (fazaIgre == CEKANJE_ODGOVORA)
        {
            prikazNaDisplej();
            if (brojSlovaPrimanjePoruke >= 2)
            {
                if (porukaZaPrimanje[0] == 'D' && porukaZaPrimanje[1] == 'A')
                {
                    setujDiodu(DESNI, CRVENA, y, x);
                    unistioBrodova++;
                }
                else
                {
                    setujDiodu(DESNI, CRVENA, y, x);
                    setujDiodu(DESNI, ZELENA, y, x);
                }
                brojSlovaPrimanjePoruke = 0;
                fazaIgre = PRIHVATANJE_POGOTKA;
                if (unistioBrodova == 15)
                {
                    fazaIgre = KRAJ_POBJEDA;
                }
            }
        }
        else if (fazaIgre == PRIHVATANJE_POGOTKA)
        {
            prikazNaDisplej();
            if (brojSlovaPrimanjePoruke >= 5)
            {
                xx = porukaZaPrimanje[3] - 0x30;
                yy = porukaZaPrimanje[4] - 0x30;
                if (upaljenaDioda(LIJEVI, ZELENA, yy, xx))
                {
                    setujDiodu(LIJEVI, CRVENA, yy, xx);
                    resetujDiodu(LIJEVI, ZELENA, yy, xx);

                    posaljiPoruku("DA");
                    potopljenoBrodova++;
                }
                else
                {
                    setujDiodu(LIJEVI, CRVENA, yy, xx);
                    setujDiodu(LIJEVI, ZELENA, yy, xx);

                    posaljiPoruku("NE");
                }
                brojSlovaPrimanjePoruke = 0;
                fazaIgre = GADJANJE;
                x = 0;
                y = 0;

                if (potopljenoBrodova == 15)
                {
                    fazaIgre = KRAJ_IZGUBIO;
                }
            }
        }
        else if (fazaIgre == KRAJ_POBJEDA)
        {
            prikaziInformaciju(ZELENA);
        }
        else if (fazaIgre == KRAJ_IZGUBIO)
        {
            prikaziInformaciju(CRVENA);
        }
    }
}
