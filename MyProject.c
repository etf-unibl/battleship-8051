unsigned char red at P0;
unsigned char kolonaCrvena at P2;
unsigned char kolonaZelena at P1;

const int velicinaMatrice = 8;

unsigned char matrica[velicinaMatrice][velicinaMatrice];
unsigned char vrijednostReda[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
unsigned char omoguciMatricu = 0x08;

void initMatrice()
{
     unsigned char i, j;
     for(i = 0; i < velicinaMatrice; i++)
     {
           for(j = 0; j < velicinaMatrice; j++)
           {
                   matrica[i][j] = 0;
           }
     }
     
      for(i = 0; i < velicinaMatrice; i++)
     {
           for(j = 0; j < velicinaMatrice; j++)
           {
                   matrica[i][j] = 8;
           }
     }

}

void prikaziRedMatrice(unsigned char i, unsigned char matrica[velicinaMatrice][velicinaMatrice], unsigned char offset, unsigned char zajednicki)
{
     unsigned char j, kolonaC, kolonaZ;

     kolonaC = 0xff;
     kolonaZ = 0xff;
     for(j = 0; j < velicinaMatrice; j++)
     {
          if(matrica[i][j] == 1 + offset || matrica[i][j] == 1 + offset + zajednicki) {
                  kolonaC &= vrijednostReda[j];
          }
          else if(matrica[i][j] == 2 + offset || matrica[i][j] == 2 + offset + zajednicki) {
                  kolonaZ &= vrijednostReda[j];
          }
          else if(matrica[i][j] == 3 + offset || matrica[i][j] == 3 + offset + zajednicki) {
                  kolonaC &= vrijednostReda[j];
                  kolonaZ &= vrijednostReda[j];
          }
     }

     kolonaCrvena = kolonaC;
     kolonaZelena = kolonaZ;
     Delay_us(1500);

     kolonaCrvena = 0xff;
     kolonaZelena = 0xff;
}

void prikazMatrice()
{
     unsigned char i, j, kolonaC, kolonaZ;
     red = 0;

     for(i = 0; i < velicinaMatrice; i++)
     {
           red = omoguciMatricu + i;
           prikaziRedMatrice(i, matrica, 0, 6);

           red = red << 4;
           Delay_us(1500);
           prikaziRedMatrice(i, matrica, 3, 3);
     }
}

void main()
{
        initMatrice();

        while(1)
        {
                prikazMatrice();

        }
}