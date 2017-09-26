#include "buttons.h"
#include "display.h"
#include "gameStage.h"
#include "messages.h"
#include "uart.h"

extern unsigned char gameStage;
extern unsigned char incomingMessageCharCount = 0;
extern const unsigned char shipSize[8] = { 4, 3, 2, 2, 1, 1, 1, 1 };
extern unsigned char shipIndex = 0;
extern char outgoingMessage[6];
extern char incomingMessage[6];
extern unsigned char x = 3;
extern unsigned char y = 3;

static unsigned char sunkShips = 0;
static unsigned char destroyedShips = 0;

void main()
{
    unsigned char xx, yy;

    initDisplay();
    UART_Config(1200);
    initButtons();
    initMessages();

    initGameStage();

    while (1)
    {

        if (gameStage == SETTING_SHIPS)
        {
            showDisplay();
            buttonResponce();

            if (shipIndex < 8)
            {
                drawShip();
            }
        }
        else if (gameStage == WAITING_OPPONENTS_SETTING)
        {
            showInfo(DIODE_GREEN);
            if (incomingMessageCharCount >= 4)
            {
                if (incomingMessage[0] == 'p' && incomingMessage[1] == 'r' && incomingMessage[2] == 'v'
                    && incomingMessage[3] == 'i')
                {
                    sendMessage("drugi");
                    gameStage = ACCEPTANCE_OF_SHOT;
                }
                else
                {
                    gameStage = AIMING;
                }
                incomingMessageCharCount = 0;
            }
            else
            {
                sendDelayedMessage("prvi");
            }
        }
        else if (gameStage == ESTABLISHING_CONNECTION)
        {
            showInfo(DIODE_GREEN);
            if (incomingMessageCharCount >= 5)
            {
                if (incomingMessage[0] == 's' && incomingMessage[1] == 't' && incomingMessage[2] == 'a'
                    && incomingMessage[3] == 'r' && incomingMessage[4] == 't')
                {
                    gameStage = SETTING_SHIPS;
                    sendMessage("start");
                }
                incomingMessageCharCount = 0;
            }
            else
            {
                sendDelayedMessage("start");
            }
        }
        else if (gameStage == AIMING)
        {
            showDisplay();
            buttonResponce();
            drawCursor();
        }
        else if (gameStage == WAITING_FOR_RESPONCE)
        {
            showDisplay();
            if (incomingMessageCharCount >= 2)
            {
                if (incomingMessage[0] == 'D' && incomingMessage[1] == 'A')
                {
                    setDiodes(DISPLAY_RIGHT, DIODE_RED, y, x);
                    destroyedShips++;
                }
                else
                {
                    setDiodes(DISPLAY_RIGHT, DIODE_RED, y, x);
                    setDiodes(DISPLAY_RIGHT, DIODE_GREEN, y, x);
                }
                incomingMessageCharCount = 0;
                gameStage = ACCEPTANCE_OF_SHOT;
                if (destroyedShips == 15)
                {
                    gameStage = END_WIN;
                }
            }
        }
        else if (gameStage == ACCEPTANCE_OF_SHOT)
        {
            showDisplay();
            if (incomingMessageCharCount >= 5)
            {
                xx = incomingMessage[3] - 0x30;
                yy = incomingMessage[4] - 0x30;
                if (diodeOn(DISPLAY_LEFT, DIODE_GREEN, yy, xx))
                {
                    setDiodes(DISPLAY_LEFT, DIODE_RED, yy, xx);
                    resetDiode(DISPLAY_LEFT, DIODE_GREEN, yy, xx);

                    sendMessage("DA");
                    sunkShips++;
                }
                else
                {
                    setDiodes(DISPLAY_LEFT, DIODE_RED, yy, xx);
                    setDiodes(DISPLAY_LEFT, DIODE_GREEN, yy, xx);

                    sendMessage("NE");
                }
                incomingMessageCharCount = 0;
                gameStage = AIMING;
                x = 0;
                y = 0;

                if (sunkShips == 15)
                {
                    gameStage = END_LOSE;
                }
            }
        }
        else if (gameStage == END_WIN)
        {
            showInfo(DIODE_GREEN);
        }
        else if (gameStage == END_LOSE)
        {
            showInfo(DIODE_RED);
        }
    }
}