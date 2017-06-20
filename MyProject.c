unsigned char red at P0;
unsigned char kolonaZelena at P1;
unsigned char kolonaCrvena at P2;

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

void main()
{
        initDispleja();

        while(1)
        {
                prikazNaDisplej();
        }
}