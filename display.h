/*!
 * \file display.h
 * \brief Functions that work with LEDS(two displays)
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

/*!
 * \brief We use this enum to identify display that will be enabled
 *  There are two displays, left for setting ships and right for shooting enemy.
 */
const enum display {
	DISPLAY_LEFT = 0,
	DISPLAY_RIGHT
};

/*!
 * \brief We use this enum to identify what color on display will be enabled
 *  There are two colors, red and green. The display also has a blue color but it is not used for the project.
 */
const enum diode {
	DIODE_RED = 0,
	DIODE_GREEN
};

/*!
 * \brief This function is used to set some default settings for drawing on display.
 */
void initDisplay();

/*!
 * \brief This function is used to show set ships on display.
 */
void showDisplay();

/**
 * \brief This function return ship for position passed as argument
 * \param position Is a position where need to show ship
 * \return Ship that need to be displayed. if there is no ship nothing will be shown, function return 0xff.
 */
unsigned char returnShip(unsigned char position);

/**
 * \brief This function return info is (x,y) position free and can we set ship at that position.
 *  "x" and "y" variable are global variable and used to save current position of cursor.
 * \return 1 if we can set current ship at position, otherwise it return 0.
 */
unsigned char availablePosition();

/**
 * \brief This function is used to display ship on right display that will be set on battlefield.
 *  On left display show current position of ship that will be set.
 */
void drawShip();

/**
 * \brief This function is used to display current position of cursor when we want to shout enemy.
 *  It used global x and y variable.
 */
void drawCursor();

/**
 * \brief This function is used to display info on right display with red or green color.
 *  When we call this function fourth and fifth row are bright in that color.
 */
void showInfo(unsigned char color);

/**
 * \brief This function enable one diode on display, and save that state.
 * \param d Left or right display
 * \param di Red or green color of diodes
 * \param y is row in matrix
 * \param x is column in matrix
 */
void setDiodes(enum display d, enum diode di, unsigned char y, unsigned char x);

/**
 * \brief This function enable diodes in one row for one display, and save that state.
 * \param d Left or right display
 * \param di Red or green color of diodes
 * \param y is row in matrix
 * \param row diodes in one row that need to be set
 */
void setDiodesRow(enum display d, enum diode di, unsigned char y,
		unsigned char row);

/**
 * \brief This function disable one diode in display, and save that state.
 * \param d Left or right display
 * \param di Red or green color of diodes
 * \param y is row in matrix
 * \param x is column in matrix
 */
void resetDiode(enum display d, enum diode di, unsigned char y,
		unsigned char x);

/**
 * \brief This function check is diode on (x,y) position enabled
 * \param d Left or right display
 * \param di Red or green color of diodes
 * \param y is row in matrix
 * \param x is column in matrix
 * \return 1 if diode enabled, otherwise it return 0.
 */
unsigned char diodeOn(enum display d, enum diode di, unsigned char y,
		unsigned char x);

#endif
