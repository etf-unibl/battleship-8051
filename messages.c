#include "messages.h"

#include "gameStage.h"
#include "uart.h"

char outgoingMessage[6];
char incomingMessage[6];
unsigned char incomingMessageCharCount;

extern unsigned char gameStage;

static int counter;
static unsigned char i, j, k;

void initMessages()
{
    incomingMessageCharCount = 0;
    counter = 0;
}

void sendMessage(char message[5])
{
    i = 0;
    while (message[i] != '\0')
    {
        outgoingMessage[i] = message[i];
        i++;
    }
    outgoingMessage[i] = 0;

    TI_bit = 1;
}

void sendDelayedMessage(char message[5])
{
    if (counter == 0)
    {
        sendMessage(message);
        counter = 2000;
    }
    counter--;
}

void UartISR() iv IVT_ADDR_ES
{
    if (UART_IsRXComplete())
    {
        if (
                        (gameStage == WAITING_FOR_RESPONCE
                        || gameStage == ACCEPTANCE_OF_SHOT
                        || gameStage == ESTABLISHING_CONNECTION
                        || gameStage == WAITING_OPPONENTS_SETTING)
            && incomingMessageCharCount < 5)
        {
            incomingMessage[incomingMessageCharCount] = UART_GetChar();
            incomingMessageCharCount++;
        }

        RI_bit = 0;
    }
    if (UART_IsTXEmpty())
    {
        UART_PutString(outgoingMessage);
        TI_bit = 0;
    }
}