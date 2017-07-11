#include "uart.h"

unsigned char red at P0;
unsigned char kolonaZelena at P1;
unsigned char kolonaCrvena at P2;
unsigned char tasteri at P3;
unsigned char dugmePritisnuto = 0xff;
char porukaUarta[5];
unsigned char brojSlovaPoruke = 0;


enum displej
{
     LIJEVI = 0,
     DESNI
};

enum dioda
{
     CRVENA = 0,
     ZELENA
};

enum fazeIgranja
{
     USPOSTAVLJANJE_KONEKCIJE = 0,
     SETOVANJE_BRODOVA,
     CEKANJE_SETOVANJA_PROTVNIKA,
     GADJANJE,
     PRIHVATANJE_POGOTKA,
     KRAJ,
     CEKANJE_ODGOVORA
};

unsigned char fazaIgre = SETOVANJE_BRODOVA;
const int velicinaMatrice = 8;
unsigned char LCDDisplej[2][2][velicinaMatrice];
const unsigned char omoguciMatricu = 0x08;
unsigned char x = 3;
unsigned char y = 3;
const unsigned char duzineBrodova [8]= {4, 3, 2, 2, 1, 1, 1, 1};
unsigned char brodZaSetovanje = 0;


void initDispleja()
{
     unsigned char i, j, k;
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
     unsigned char i, j;
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
     unsigned char i, brod = 0xff;

     for(i = pozicija; i < pozicija + duzineBrodova[brodZaSetovanje]; i++)
     {
            brod &= ~(1 << i);
     }
     return brod;
}

void reagovanjeNaTasterePriSetovanju(unsigned char indexTastera)
{
      unsigned char brod;
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
             if (brodZaSetovanje < 8)
             {
                   brod = vratiBrod(x);
                   LCDDisplej[0][ZELENA][y] &= brod;

                   brodZaSetovanje++;
             }
             if (brodZaSetovanje == 8)
             {
                    fazaIgre = GADJANJE;
             }
             x = 3;
             y = 3;
      }
}

void reagovanjeNaTasterePriGadjanju(unsigned char indexTastera)
{
//      char hit[6];
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
//              sprintf(hit, "hit%c%c", x + 0x30, y + 0x30);
//              UART_PutString(hit);
              
              UART_PutChar('h');
              UART_PutChar('i');
              UART_PutChar('t');
              UART_PutChar(x + 0x30);
              UART_PutChar(y + 0x30);

              fazaIgre = CEKANJE_ODGOVORA;
      }
}

void reagovanjeNaTastere()
{
     unsigned char i, siftovanoI;
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
     unsigned char i, brod;
     
     brod = vratiBrod(x);
     red = omoguciMatricu + y;
     kolonaCrvena = 0xff;
     kolonaZelena = brod;
     Delay_ms(1);
     
     brod = vratiBrod(3);
     red = (omoguciMatricu + 3) << 4;
     kolonaZelena = brod;
     Delay_ms(1);
}

void iscrtajKursor()
{
       unsigned char pozicija = 0xff;
       pozicija &= ~(1 << x);

       red = 0x80 + (y << 4);
       
       kolonaCrvena = 0xff;
       kolonaZelena = pozicija;
       Delay_ms(1);
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
                else if (fazaIgre == GADJANJE)
                {
                        prikazNaDisplej();
                        reagovanjeNaTastere();
                        iscrtajKursor();
                }
                else if (fazaIgre == CEKANJE_ODGOVORA)
                {
                       prikazNaDisplej();
                       if (brojSlovaPoruke >= 2)
                        {
                                if(porukaUarta[0] == 'D' && porukaUarta[1] == 'A')
                                {
                                      LCDDisplej[DESNI][CRVENA][y] &= ~(1 << x);
                                }
                                else
                                {
                                      LCDDisplej[DESNI][CRVENA][y] &= ~(1 << x);
                                      LCDDisplej[DESNI][ZELENA][y] &= ~(1 << x);
                                }
                                brojSlovaPoruke = 0;
                                fazaIgre = PRIHVATANJE_POGOTKA;
                        }
                }
                else if (fazaIgre == PRIHVATANJE_POGOTKA)
                {
                        prikazNaDisplej();
                        if (brojSlovaPoruke >= 5)
                        {
                                xx = porukaUarta[3] - 0x30;
                                yy = porukaUarta[4] - 0x30;
                                if(LCDDisplej[LIJEVI][ZELENA][yy] & (1 << xx))
                                {
                                      LCDDisplej[LIJEVI][CRVENA][yy] &= ~(1 << xx);
                                      LCDDisplej[LIJEVI][ZELENA][yy] &= ~(1 << xx);
                                      UART_PutChar('D');
                                      UART_PutChar('A');
                                }
                                else
                                {
                                      LCDDisplej[LIJEVI][CRVENA][yy] &= ~(1 << xx);
                                      LCDDisplej[LIJEVI][ZELENA][yy] |= (1 << xx);
                                      UART_PutChar('N');
                                      UART_PutChar('E');
                                }
                                brojSlovaPoruke = 0;
                                fazaIgre = GADJANJE;
                        }
                }
        }
}

void UartISR() iv IVT_ADDR_ES
{
        // put your UART interrupt service routine code here
        if(UART_IsRXComplete()) {
                if (fazaIgre == CEKANJE_ODGOVORA || fazaIgre == PRIHVATANJE_POGOTKA)
                {
                     porukaUarta[brojSlovaPoruke] = UART_GetChar();
                     brojSlovaPoruke++;
                }

                RI_bit = 0;
        }
}