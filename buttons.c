#include "buttons.h"

#include "display.h"
#include "gameStage.h"
#include "messages.h"

unsigned char buttons at P3;
unsigned char x;
unsigned char y;

extern unsigned char incomingMessageCharCount;
extern const unsigned char shipSize[8];
extern unsigned char shipIndex;
extern char outgoingMessage[6];
extern char incomingMessage[6];
extern unsigned char gameStage;

static unsigned char i, j, k;
static unsigned char buttonPressed;

void initButtons()
{
    x = 2;
    y = 3;
    buttonPressed = 0xff;

    ES_bit = 1;
    EA_bit = 1;
}

void buttonResponceSetting(unsigned char buttonIndex)
{
    if (buttonIndex == BUTTON_UP && y > 0)
    {
        y--;
    }
    else if (buttonIndex == BUTTON_RIGHT && x < 8 - shipSize[shipIndex])
    {
        x++;
    }
    else if (buttonIndex == BUTTON_LEFT && x > 0)
    {
        x--;
    }
    else if (buttonIndex == BUTTON_DOWN && y < 7)
    {
        y++;
    }
    else if (buttonIndex == BUTTON_ENTER)
    {
        if (availablePosition())
        {
            setDiodesRow(DISPLAY_LEFT, DIODE_GREEN, y, returnShip(x));

            shipIndex++;

            x = 3;
            y = 3;

            if (shipIndex == 8)
            {
                gameStage = WAITING_OPPONENTS_SETTING;
                incomingMessageCharCount = 0;
            }
        }
    }
}

void buttonResponceShooting(unsigned char buttonIndex)
{
    if (buttonIndex == BUTTON_UP && y > 0)
    {
        y--;
    }
    else if (buttonIndex == BUTTON_RIGHT && x < 7)
    {
        x++;
    }
    else if (buttonIndex == BUTTON_LEFT && x > 0)
    {
        x--;
    }
    else if (buttonIndex == BUTTON_DOWN && y < 7)
    {
        y++;
    }
    else if (buttonIndex == BUTTON_ENTER)
    {
        if (!diodeOn(DISPLAY_RIGHT, DIODE_RED, y, x))
        {
            outgoingMessage[0] = 'h';
            outgoingMessage[1] = 'i';
            outgoingMessage[2] = 't';
            outgoingMessage[3] = x + 0x30;
            outgoingMessage[4] = y + 0x30;
            outgoingMessage[5] = 0;

            TI_bit = 1;

            gameStage = WAITING_FOR_RESPONCE;
            incomingMessageCharCount = 0;
        }
    }
}

void buttonResponce()
{
    unsigned char shiftedI;
    for (i = 2; i < 8; i++)
    {
        shiftedI = 1 << i;
        if (buttons & shiftedI)
        {
            if (!(buttonPressed & shiftedI))
            {
                if (gameStage == SETTING_SHIPS)
                {
                    buttonResponceSetting(i);
                }
                else if (gameStage == AIMING)
                {
                    buttonResponceShooting(i);
                }

                buttonPressed |= shiftedI;
            }
        }
        else
        {
            buttonPressed &= ~shiftedI;
        }
    }
}