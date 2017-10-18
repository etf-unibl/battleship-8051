/*!
 * \file gameStage.h
 * \brief Function that memorize game stage when plays
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#ifndef FAZA_IGRE_H_
#define FAZA_IGRE_H_

/*!
 * \brief We use this enum to identify stages in the game
 *  There is 8 stage levels.
 */
const enum gameStages
{
    ESTABLISHING_CONNECTION = 0,
    SETTING_SHIPS,
    WAITING_OPPONENTS_SETTING,
    AIMING,
    ACCEPTANCE_OF_SHOT,
    END_WIN,
    END_LOSE,
    WAITING_FOR_RESPONCE,
    RESET_GAME
};

/*!
 * \brief This function is used to set default stage. it is ESTABLISHING_CONNECTION.
 */
void initGameStage();

#endif
