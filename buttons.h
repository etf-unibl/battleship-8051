/*!
 * \file buttons.h
 * \brief Functions that work with buttons
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

/*!
 * \brief We use this enum to identify pressed button on keyboard
 *  Have just five buttons that we use, other are ignored.
 */
const enum button {
	BUTTON_UP = 2,
	BUTTON_RIGHT = 3,
	BUTTON_LEFT = 4,
	BUTTON_DOWN = 5,
	BUTTON_ENTER = 6
};

/*!
 * \brief This function is used to set some default settings for handling buttons
 *  ES_bit and EA_bit variables are set here
 */
void initButtons();

/*!
 * \brief
 *  This function is used to response when user press button. Depending of gameStage it calls proper handling functions.
 *  Globals variable: shipIndex, outgoingMessage, gameStage and incomingMessageCharCount are set here for proper button index
 */
void buttonResponce();

#endif
