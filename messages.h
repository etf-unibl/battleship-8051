/*!
 * \file messages.h
 * \brief Function that works with messages send/received through uart
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

/*!
 * \brief This function is used to set some default settings for messaging.
 */
void initMessages();

/**
 * \brief We send message to others board with this function.
 * \param message is message with max 6 letters that will be send.
 */
void sendMessage(char message[7]);

/**
 * \brief We send message to others board with this function, bat every 2 seconds.
 * \param message is message with max 6 letters that will be send.
 */
void sendDelayedMessage(char message[7]);

#endif
