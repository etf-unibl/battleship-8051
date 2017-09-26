#include "display.h"

#include "buttons.h"
#include "uart.h"

unsigned char row at P0;
//unsigned char columnRed at P1;
//unsigned char columnGreen at P2;
unsigned char columnRed;
unsigned char columnGreen;

const unsigned char shipSize[8] = { 4, 3, 2, 2, 1, 1, 1, 1 };
unsigned char shipIndex;

extern unsigned char x;
extern unsigned char y;

static unsigned char i, j, k;
static const int matrixSize = 8;
static unsigned char LCD[2][2][matrixSize];
static const unsigned char enableLeftMatrix = 0x08;

void initDisplay()
{
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < matrixSize; k++)
            {
                LCD[i][j][k] = 0xff;
            }
        }
    }
    shipIndex = 0;
}

void showDisplay()
{
    for (i = 0; i < matrixSize; i++)
    {
        columnRed = 0xff;
        columnGreen = 0xff;
        row = enableLeftMatrix + i;

        for (j = 0; j < 2; j++)
        {
            columnRed = LCD[j][DIODE_RED][i];
            columnGreen = LCD[j][DIODE_GREEN][i];
            Delay_us(500);

            columnRed = 0xff;
            columnGreen = 0xff;
            row = row << 4;
            Delay_us(500);
        }
    }
}

unsigned char returnShip(unsigned char position)
{
    unsigned char ship = 0xff;

    for (i = position; i < position + shipSize[shipIndex]; i++)
    {
        ship &= ~(1 << i);
    }
    return ship;
}

unsigned char availablePosition()
{
    unsigned char h;
    j = (y == 0) ? y : (y - 1);
    h = (y == 7) ? y : (y + 1);
    for (; j <= h; j++)
    {
        i = x != 0 ? (x - 1) : x;
        k = x + shipSize[shipIndex] - 1;
        k = (k == 7) ? k : (k + 1);
        for (; i <= k; i++)
        {
            if (!(LCD[DISPLAY_LEFT][DIODE_GREEN][j] & (1 << i)))
            {
                return 0;
            }
        }
    }
    return 1;
}

void drawShip()
{
    row = enableLeftMatrix + y;
    columnRed = 0xff;
    columnGreen = returnShip(x);
    Delay_ms(1);

    columnRed = 0xff;
    columnGreen = 0xff;

    row = (enableLeftMatrix + 3) << 4;
    columnGreen = returnShip(4 - (shipSize[shipIndex] / 2));
    Delay_ms(1);
}

void drawCursor()
{
    i = 0xff;
    i &= ~(1 << x);

    row = 0x80 + (y << 4);

    columnRed = 0xff;
    columnGreen = i;
    Delay_ms(1);
}

void showInfo(unsigned char color)
{
    for (i = 3; i < 5; i++)
    {
        row = 0x80 + (i << 4);

        if (color == DIODE_RED)
        {
            columnRed = 0;
            columnGreen = 0xff;
        }
        else
        {
            columnRed = 0xff;
            columnGreen = 0;
        }

        Delay_ms(1);
    }
}

void setDiodes(enum display d, enum diode di, unsigned char y, unsigned char x)
{
    LCD[d][di][y] &= ~(1 << x);
}

void setDiodesRow(enum display d, enum diode di, unsigned char y, unsigned char redDioda)
{
    LCD[d][di][y] &= redDioda;
}

void resetDiode(enum display d, enum diode di, unsigned char y, unsigned char x)
{
    LCD[d][di][y] |= (1 << x);
}

unsigned char diodeOn(enum display d, enum diode di, unsigned char y, unsigned char x)
{
    return !(LCD[d][di][y] & (1 << x));
}