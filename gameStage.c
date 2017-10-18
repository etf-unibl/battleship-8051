/*!
 * \file gameStage.c
 * \brief Function that memorize game stage when plays
 *  COPYRIGHT: College of electrical engineering, www.etfbl.net
 *  This software was written in accordance to the guidelines at https://github.com/knezicm/battleship-8051/blob/master/LICENSE
 *  VERSION:	1.0, PF 2-OCT-17
 */

#include "gameStage.h"

unsigned char gameStage;

void initGameStage()
{
    gameStage = ESTABLISHING_CONNECTION;
}
