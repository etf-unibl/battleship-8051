#include "tasteri.h"
#include "displej.h"
#include "poruke.h"
#include "fazaIgre.h"

unsigned char tasteri at P3;
unsigned char x;
unsigned char y;

extern unsigned char brojSlovaPrimanjePoruke;
extern const unsigned char duzineBrodova[8] = { 4, 3, 2, 2, 1, 1, 1, 1 };
extern unsigned char brodZaSetovanje;
extern char porukaZaSlanje[6];
extern char porukaZaPrimanje[6];
extern unsigned char fazaIgre;

static unsigned char i, j, k;
static unsigned char dugmePritisnuto;

void initTasteri()
{
    x = 2;
    y = 3;
    dugmePritisnuto = 0xff;

    ES_bit = 1;
    EA_bit = 1;
}

void reagovanjeNaTasterePriSetovanju(unsigned char indexTastera)
{
    if (indexTastera == GORE && y > 0)
    {
        y--;
    }
    else if (indexTastera == DESNO && x < 8 - duzineBrodova[brodZaSetovanje])
    {
        x++;
    }
    else if (indexTastera == LIJEVO && x > 0)
    {
        x--;
    }
    else if (indexTastera == DOLJE && y < 7)
    {
        y++;
    }
    else if (indexTastera == ENTER)
    {
        if (slobodnaPozicija())
        {
            setujRedDioda(LIJEVI, ZELENA, y, vratiBrod(x));

            brodZaSetovanje++;

            x = 3;
            y = 3;

            if (brodZaSetovanje == 8)
            {
                fazaIgre = CEKANJE_SETOVANJA_PROTVNIKA;
                brojSlovaPrimanjePoruke = 0;
            }
        }
    }
}

void reagovanjeNaTasterePriGadjanju(unsigned char indexTastera)
{
    if (indexTastera == GORE && y > 0)
    {
        y--;
    }
    else if (indexTastera == DESNO && x < 7)
    {
        x++;
    }
    else if (indexTastera == LIJEVO && x > 0)
    {
        x--;
    }
    else if (indexTastera == DOLJE && y < 7)
    {
        y++;
    }
    else if (indexTastera == ENTER)
    {
        if (!upaljenaDioda(DESNI, CRVENA, y, x))
        {
            porukaZaSlanje[0] = 'h';
            porukaZaSlanje[1] = 'i';
            porukaZaSlanje[2] = 't';
            porukaZaSlanje[3] = x + 0x30;
            porukaZaSlanje[4] = y + 0x30;
            porukaZaSlanje[5] = 0;

            TI_bit = 1;

            fazaIgre = CEKANJE_ODGOVORA;
            brojSlovaPrimanjePoruke = 0;
        }
    }
}

void reagovanjeNaTastere()
{
    unsigned char siftovanoI;
    for (i = 2; i < getVelicinaMatrice(); i++)
    {
        siftovanoI = 1 << i;
        if (tasteri & siftovanoI)
        {
            if (!(dugmePritisnuto & siftovanoI))
            {
                if (fazaIgre == SETOVANJE_BRODOVA)
                {
                    reagovanjeNaTasterePriSetovanju(i);
                }
                else if (fazaIgre == GADJANJE)
                {
                    reagovanjeNaTasterePriGadjanju(i);
                }

                dugmePritisnuto |= siftovanoI;
            }
        }
        else
        {
            dugmePritisnuto &= ~siftovanoI;
        }
    }
}
