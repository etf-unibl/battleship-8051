#ifndef TASTERI_H_
#define TASTERI_H_

const enum TASTER
{
    GORE = 2,
    DESNO = 3,
    LIJEVO = 4,
    DOLJE = 5,
    ENTER = 6
};

void initTasteri();
void reagovanjeNaTasterePriSetovanju(unsigned char indexTastera);
void reagovanjeNaTasterePriGadjanju(unsigned char indexTastera);
void reagovanjeNaTastere();

#endif
