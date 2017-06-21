unsigned char red at P0;
unsigned char kolonaZelena at P1;
unsigned char kolonaCrvena at P2;
unsigned char tasteri at P3;
unsigned char dugmePritisnuto = 0xff;

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

const int velicinaMatrice = 8;
unsigned char LCDDisplej[2][2][velicinaMatrice];
unsigned char omoguciMatricu = 0x08;
unsigned char x = 3;
unsigned char y = 3;
unsigned char duzineBrodova [8]= {4, 3, 2, 2, 1, 1, 1, 1};
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
             red = omoguciMatricu + i;
             for(j = 0; j < 2; j++)
             {
                     kolonaCrvena = 0xff;
                     kolonaZelena = 0xff;

                     kolonaCrvena = LCDDisplej[j][CRVENA][i];
                     kolonaZelena = LCDDisplej[j][ZELENA][i];
                     Delay_ms(1);

                     red = red << 4;
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

void reagovanjeNaTastere()
{
     unsigned char i, siftovanoI, brod;
     for(i = 2; i < velicinaMatrice; i++)
     {
          siftovanoI = 1 << i;
          if(tasteri & siftovanoI)
          {
          	if (!(dugmePritisnuto & siftovanoI))
          	{
                        if(i == 2 && y > 0)
                        {
                               y--;
                        }
                        else if(i == 3 && x < 8 - duzineBrodova[brodZaSetovanje])
                        {
                               x++;
                        }
                        else if(i == 4 && x > 0)
                        {
                               x--;
                        }
                        else if(i == 5 && y < 7)
                        {
                               y++;
                        }
                        else if(i == 6 && brodZaSetovanje < 8)
                        {
                               brod = vratiBrod(x);
                               LCDDisplej[0][CRVENA][y] &= brod;
                               
                               x = 3;
                               y = 3;
                               brodZaSetovanje++;
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

void main()
{
        initDispleja();

        while(1)
        {
                prikazNaDisplej();
                reagovanjeNaTastere();
                
                if(brodZaSetovanje < 8)
                {
                      iscrtajBrod();
                }
        }
}
