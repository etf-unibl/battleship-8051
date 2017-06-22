#ifndef UART_H_
#define UART_H_

void UART_Config(unsigned long);
short UART_IsRXComplete();
short UART_IsTXEmpty();
char UART_GetChar();
void UART_PutChar(char);
void UART_PutString(char*);

#endif