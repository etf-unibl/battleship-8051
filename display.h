#ifndef DISPLAY_H_
#define DISPLAY_H_

const enum display
{
    DISPLAY_LEFT = 0,
    DISPLAY_RIGHT
};

const enum diode
{
    DIODE_RED = 0,
    DIODE_GREEN
};

void initDisplay();
void showDisplay();
unsigned char returnShip(unsigned char position);
unsigned char availablePosition();
void drawShip();
void drawCursor();
void showInfo(unsigned char color);
void setDiodes(enum display d, enum diode di, unsigned char y, unsigned char x);
void setDiodesRow(enum display d, enum diode di, unsigned char y, unsigned char row);
void resetDiode(enum display d, enum diode di, unsigned char y, unsigned char x);
unsigned char diodeOn(enum display d, enum diode di, unsigned char y, unsigned char x);

#endif
