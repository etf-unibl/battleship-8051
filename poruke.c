#include "poruke.h"
#include "uart.h"
#include "fazaIgre.h"

char porukaZaSlanje[6];
char porukaZaPrimanje[6];
unsigned char brojSlovaPrimanjePoruke;

extern unsigned char fazaIgre;

static int brojac;
static unsigned char i, j, k;

void initPoruke()
{
    brojSlovaPrimanjePoruke = 0;
    brojac = 0;
}

void posaljiPoruku(char poruka[5])
{
    i = 0;
    while (poruka[i] != '\0')
    {
        porukaZaSlanje[i] = poruka[i];
        i++;
    }
    porukaZaSlanje[i] = 0;

    TI_bit = 1;
}

void posaljiPorukuSaKasnjenjem(char poruka[5])
{
    if (brojac == 0)
    {
        posaljiPoruku(poruka);
        brojac = 2000;
    }
    brojac--;
}

void UartISR()
iv IVT_ADDR_ES
{
    if (UART_IsRXComplete())
    {
        if ((fazaIgre == CEKANJE_ODGOVORA || fazaIgre == PRIHVATANJE_POGOTKA || fazaIgre == USPOSTAVLJANJE_KONEKCIJE || fazaIgre == CEKANJE_SETOVANJA_PROTVNIKA)
            && brojSlovaPrimanjePoruke < 5)
        {
            porukaZaPrimanje[brojSlovaPrimanjePoruke] = UART_GetChar();
            brojSlovaPrimanjePoruke++;
        }

        RI_bit = 0;
    }
    if (UART_IsTXEmpty())
    {
        UART_PutString(porukaZaSlanje);
        TI_bit = 0;
    }
}
