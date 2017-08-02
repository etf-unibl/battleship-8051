#include "uart.h"

unsigned char red at P0;
unsigned char kolonaZelena at P1;
unsigned char kolonaCrvena at P2;
unsigned char tasteri at P3;
unsigned char dugmePritisnuto = 0xff;
char porukaZaSlanje[6];
char porukaZaPrimanje[6];
unsigned char brojSlovaPrimanjePoruke = 0;
int brojac = 0;
unsigned char potopljenoBrodova = 0;
unsigned char unistioBrodova = 0;


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

const enum fazeIgranja
{
     USPOSTAVLJANJE_KONEKCIJE = 0,
     SETOVANJE_BRODOVA,
     CEKANJE_SETOVANJA_PROTVNIKA,
     GADJANJE,
     PRIHVATANJE_POGOTKA,
     KRAJ_POBJEDA,
     KRAJ_IZGUBIO,
     CEKANJE_ODGOVORA
};

unsigned char fazaIgre;
const int velicinaMatrice = 8;
unsigned char LCDDisplej[2][2][velicinaMatrice];
const unsigned char omoguciMatricu = 0x08;
unsigned char x = 3;
unsigned char y = 3;
const unsigned char duzineBrodova [8]= {4, 3, 2, 2, 1, 1, 1, 1};
unsigned char brodZaSetovanje = 0;

unsigned char i, j, k;

void initDispleja()
{
     for(i = 0; i < 2; i++)
     {
           for(j = 0; j < 2; j++)
           {
                  for(k = 0; k < velicinaMatrice; k++)
                  {
                        LCDDisplej[i][j][k] = 0xff;
                  }
           }
     }
}

void prikazNaDisplej()
{
     for(i = 0; i < velicinaMatrice; i++) 
     {
             kolonaCrvena = 0xff;
             kolonaZelena = 0xff;
             red = omoguciMatricu + i;
             
             for(j = 0; j < 2; j++)
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

     for(i = pozicija; i < pozicija + duzineBrodova[brodZaSetovanje]; i++)
     {
            brod &= ~(1 << i);
     }
     return brod;
}

unsigned char slobodnaPozicija()
{
       j = (y == 0) ? y : (y - 1);
       brojac = (y == 7) ? y : (y + 1);
       for(;j <= brojac; j++)
       {
             i = x != 0 ? (x - 1) : x;
             k = x + duzineBrodova[brodZaSetovanje] - 1;
             k = (k == 7) ? k : (k + 1);
             for(; i <= k; i++)
             {
                    if(!(LCDDisplej[LIJEVI][ZELENA][j] & (1 << i)))
                    {
                         return 0;
                    }
             }
       }
       return 1;
}

void reagovanjeNaTasterePriSetovanju(unsigned char indexTastera)
{
      if(indexTastera == 2 && y > 0)
      {
             y--;
      }
      else if(indexTastera == 3 && x < 8 - duzineBrodova[brodZaSetovanje])
      {
             x++;
      }
      else if(indexTastera == 4 && x > 0)
      {
             x--;
      }
      else if(indexTastera == 5 && y < 7)
      {
             y++;
      }
      else if(indexTastera == 6)
      {
             if(slobodnaPozicija())
             {
                   LCDDisplej[LIJEVI][ZELENA][y] &= vratiBrod(x);;

                   brodZaSetovanje++;

                   x = 3;
                   y = 3;
                   
                   if (brodZaSetovanje == 8)
                   {
                          fazaIgre = CEKANJE_SETOVANJA_PROTVNIKA;
                   }
             }
      }
}

void reagovanjeNaTasterePriGadjanju(unsigned char indexTastera)
{
      if(indexTastera == 2 && y > 0)
      {
             y--;
      }
      else if(indexTastera == 3 && x < 7)
      {
             x++;
      }
      else if(indexTastera == 4 && x > 0)
      {
             x--;
      }
      else if(indexTastera == 5 && y < 7)
      {
             y++;
      }
      else if(indexTastera == 6)
      {
               if((LCDDisplej[DESNI][CRVENA][y] & (1 << x)))
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
     for(i = 2; i < velicinaMatrice; i++)
     {
          siftovanoI = 1 << i;
          if(tasteri & siftovanoI)
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
      for(i = 3; i < 5; i++)
      {
             red = 0x80 + (i << 4);

             if(boja == CRVENA)
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

void posaljiPoruku(char poruka[5])
{
       if(brojac > 2000)
       {
             i = 0;
             while(poruka[i] != '\0') 
             {
                   porukaZaSlanje[i] = poruka[i];
                   i++;
             }
             porukaZaSlanje[i] = 0;
              
             TI_bit = 1;
             brojac = 0;
       }
       brojac++;
}

void main()
{
         unsigned char xx, yy;
        initDispleja();

        // Initialize UART
        UART_Config(1200);
        
        // Enable interrupts
        ES_bit = 1;
        EA_bit = 1;
        
        fazaIgre = USPOSTAVLJANJE_KONEKCIJE;

        brojac = 0;

        while(1)
        {

                if (fazaIgre == SETOVANJE_BRODOVA)
                {
                        prikazNaDisplej();
                        reagovanjeNaTastere();

                        if(brodZaSetovanje < 8)
                        {
                              iscrtajBrod();
                        }
                }
                else if (fazaIgre == CEKANJE_SETOVANJA_PROTVNIKA)
                {
                        prikaziInformaciju(ZELENA);
                        if(brojSlovaPrimanjePoruke >= 4)
                        {
                             if(porukaZaPrimanje[0] == 'p'
                             && porukaZaPrimanje[1] == 'r'
                             && porukaZaPrimanje[2] == 'v'
                             && porukaZaPrimanje[3] == 'i'
                             ) {
                                   porukaZaSlanje[0] = 'd';
                                   porukaZaSlanje[1] = 'r';
                                   porukaZaSlanje[2] = 'u';
                                   porukaZaSlanje[3] = 'g';
                                   porukaZaSlanje[4] = 'i';
                                   porukaZaSlanje[5] = 0;
                                   
                                   TI_bit = 1;
                                   fazaIgre = PRIHVATANJE_POGOTKA;
                             } else
                             {
                                   fazaIgre = GADJANJE;
                             }
                             brojSlovaPrimanjePoruke = 0;
                        } else {
                              posaljiPoruku("prvi");
                        }
                }
                else if (fazaIgre == USPOSTAVLJANJE_KONEKCIJE)
                {
                        prikaziInformaciju(ZELENA);
                        if(brojSlovaPrimanjePoruke >= 5) {
                             if(porukaZaPrimanje[0] == 's'
                             && porukaZaPrimanje[1] == 't'
                             && porukaZaPrimanje[2] == 'a'
                             && porukaZaPrimanje[3] == 'r'
                             && porukaZaPrimanje[4] == 't'
                             ) {
                                   fazaIgre = SETOVANJE_BRODOVA;
                             }
                             brojSlovaPrimanjePoruke = 0;
                        } else {
                               posaljiPoruku("start");
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
                                if(porukaZaPrimanje[0] == 'D' && porukaZaPrimanje[1] == 'A')
                                {
                                      LCDDisplej[DESNI][CRVENA][y] &= ~(1 << x);
                                      unistioBrodova++;
                                }
                                else
                                {
                                      LCDDisplej[DESNI][CRVENA][y] &= ~(1 << x);
                                      LCDDisplej[DESNI][ZELENA][y] &= ~(1 << x);
                                }
                                brojSlovaPrimanjePoruke = 0;
                                fazaIgre = PRIHVATANJE_POGOTKA;
                                if(potopljenoBrodova == 15)
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
                                if(!(LCDDisplej[LIJEVI][ZELENA][yy] & (1 << xx)))
                                {
                                      LCDDisplej[LIJEVI][CRVENA][yy] &= ~(1 << xx);
                                      LCDDisplej[LIJEVI][ZELENA][yy] |= (1 << xx);
                                      
                                      porukaZaSlanje[0] = 'D';
                                      porukaZaSlanje[1] = 'A';
                                      porukaZaSlanje[2] = 0;
                                      
                                      TI_bit = 1;
                                      potopljenoBrodova++;
                                }
                                else
                                {
                                      LCDDisplej[LIJEVI][CRVENA][yy] &= ~(1 << xx);
                                      LCDDisplej[LIJEVI][ZELENA][yy] &= ~(1 << xx);
                                       
                                      porukaZaSlanje[0] = 'N';
                                      porukaZaSlanje[1] = 'E';
                                      porukaZaSlanje[2] = 0;
                                       
                                      TI_bit = 1;
                                }
                                brojSlovaPrimanjePoruke = 0;
                                fazaIgre = GADJANJE;
                                x = 0;
                                y = 0;

                                if(potopljenoBrodova == 15)
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

void UartISR() iv IVT_ADDR_ES
{
        // put your UART interrupt service routine code here
        if(UART_IsRXComplete()) {
                if ((fazaIgre == CEKANJE_ODGOVORA || fazaIgre == PRIHVATANJE_POGOTKA || fazaIgre == USPOSTAVLJANJE_KONEKCIJE  || fazaIgre == CEKANJE_SETOVANJA_PROTVNIKA
                || fazaIgre == GADJANJE || fazaIgre == SETOVANJE_BRODOVA)
                   && brojSlovaPrimanjePoruke < 5)
                {
                     porukaZaPrimanje[brojSlovaPrimanjePoruke] = UART_GetChar();
                     brojSlovaPrimanjePoruke++;

                }

                RI_bit = 0;
        }
        if(UART_IsTXEmpty()) {
              UART_PutString(porukaZaSlanje);
              TI_bit = 0;
        }
}