#include "uart.h"
#include "displej.h"
#include "tasteri.h"

unsigned char red at P0;
unsigned char kolonaCrvena at P1;
unsigned char kolonaZelena at P2;

const unsigned char duzineBrodova[8] = { 4, 3, 2, 2, 1, 1, 1, 1 };
unsigned char brodZaSetovanje;

extern unsigned char x;
extern unsigned char y;

static unsigned char i, j, k;
static const int velicinaMatrice = 8;
static unsigned char LCDDisplej[2][2][velicinaMatrice];
static const unsigned char omoguciMatricu = 0x08;

void initDisplej()
{
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < velicinaMatrice; k++)
            {
                LCDDisplej[i][j][k] = 0xff;
            }
        }
    }
    brodZaSetovanje = 0;
}

void prikazNaDisplej()
{
    for (i = 0; i < velicinaMatrice; i++)
    {
        kolonaCrvena = 0xff;
        kolonaZelena = 0xff;
        red = omoguciMatricu + i;

        for (j = 0; j < 2; j++)
        {
            kolonaCrvena = LCDDisplej[j][CRVENA][i];
            kolonaZelena = LCDDisplej[j][ZELENA][i];
            Delay_us(500);

            kolonaCrvena = 0xff;
            kolonaZelena = 0xff;
            red = red << 4;
            Delay_us(500);
        }
    }
}

unsigned char vratiBrod(unsigned char pozicija)
{
    unsigned char brod = 0xff;

    for (i = pozicija; i < pozicija + duzineBrodova[brodZaSetovanje]; i++)
    {
        brod &= ~(1 << i);
    }
    return brod;
}

unsigned char slobodnaPozicija()
{
    unsigned char h;
    j = (y == 0) ? y : (y - 1);
    h = (y == 7) ? y : (y + 1);
    for (; j <= h; j++)
    {
        i = x != 0 ? (x - 1) : x;
        k = x + duzineBrodova[brodZaSetovanje] - 1;
        k = (k == 7) ? k : (k + 1);
        for (; i <= k; i++)
        {
            if (!(LCDDisplej[LIJEVI][ZELENA][j] & (1 << i)))
            {
                return 0;
            }
        }
    }
    return 1;
}

void iscrtajBrod()
{
    red = omoguciMatricu + y;
    kolonaCrvena = 0xff;
    kolonaZelena = vratiBrod(x);
    Delay_ms(1);

    kolonaCrvena = 0xff;
    kolonaZelena = 0xff;

    red = (omoguciMatricu + 3) << 4;
    kolonaZelena = vratiBrod(4 - (duzineBrodova[brodZaSetovanje] / 2));
    Delay_ms(1);
}

void iscrtajKursor()
{
    i = 0xff;
    i &= ~(1 << x);

    red = 0x80 + (y << 4);

    kolonaCrvena = 0xff;
    kolonaZelena = i;
    Delay_ms(1);
}

void prikaziInformaciju(unsigned char boja)
{
    for (i = 3; i < 5; i++)
    {
        red = 0x80 + (i << 4);

        if (boja == CRVENA)
        {
            kolonaCrvena = 0;
            kolonaZelena = 0xff;
        }
        else
        {
            kolonaCrvena = 0xff;
            kolonaZelena = 0;
        }

        Delay_ms(1);
    }
}

void setujDiodu(enum displej d, enum dioda di, unsigned char y, unsigned char x)
{
    LCDDisplej[d][di][y] &= ~(1 << x);
}

void setujRedDioda(enum displej d, enum dioda di, unsigned char y, unsigned char redDioda)
{
    LCDDisplej[d][di][y] &= redDioda;
}

void resetujDiodu(enum displej d, enum dioda di, unsigned char y, unsigned char x)
{
    LCDDisplej[d][di][y] |= (1 << x);
}

unsigned char upaljenaDioda(enum displej d, enum dioda di, unsigned char y, unsigned char x)
{
    return !(LCDDisplej[d][di][y] & (1 << x));
}

unsigned char getVelicinaMatrice()
{
    return velicinaMatrice;
}
