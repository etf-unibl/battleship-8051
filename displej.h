#ifndef DISPLEJ_H_
#define DISPLEJ_H_

const enum displej
{
    LIJEVI = 0,
    DESNI
};

const enum dioda
{
    CRVENA = 0,
    ZELENA
};

void initDisplej();
void prikazNaDisplej();
unsigned char vratiBrod(unsigned char pozicija);
unsigned char slobodnaPozicija();
void iscrtajBrod();
void iscrtajKursor();
void prikaziInformaciju(unsigned char boja);
void setujDiodu(enum displej d, enum dioda di, unsigned char y, unsigned char x);
void setujRedDioda(enum displej d, enum dioda di, unsigned char y, unsigned char redDioda);
void resetujDiodu(enum displej d, enum dioda di, unsigned char y, unsigned char x);
unsigned char upaljenaDioda(enum displej d, enum dioda di, unsigned char y, unsigned char x);
unsigned char getVelicinaMatrice();

#endif
