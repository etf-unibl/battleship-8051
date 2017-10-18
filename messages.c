/*!
 * \file messages.h
 * \brief Function that works with messages send/received through uart
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#include "messages.h"
#include "gameStage.h"
#include "uart.h"

char outgoingMessage[7];
char incomingMessage[7];
unsigned char incomingMessageCharCount;

extern unsigned char gameStage;

static int counter;
static unsigned char i, j, k;

void initMessages()
{
    incomingMessageCharCount = 0;
    counter = 0;
}

void sendMessage(char message[7])
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

void sendDelayedMessage(char message[7])
{
    if (counter <= 0)
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
