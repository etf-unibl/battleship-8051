/*!
 * \file uart.h
 * \brief Functions for UART communication
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#ifndef UART_H_
#define UART_H_

/**
 * \brief This function is used to set default settings for uart communication.
 * \param baudrate Is speed of uart communication
 */
void UART_Config(unsigned long baudrate);

/**
 * \brief This function check is all bites received.
 * \return 1 if everything received, otherwise 0.
 */
short UART_IsRXComplete();

/**
 * \brief This function check is all bites sent.
 * \return 1 if everything sent, otherwise 0.
 */
short UART_IsTXEmpty();

/**
 * \brief This function receive 8 bits trough uart.
 * \return bites received.
 */
char UART_GetChar();

/**
 * \brief This function send 8 bits trough uart
 * \param _data Data to be sent.
 */
void UART_PutChar(char _data);

/**
 * \brief This function send all buffer trough uart
 * \param buffer Data that will be sent.
 */
void UART_PutString(char* buffer);

#endif
