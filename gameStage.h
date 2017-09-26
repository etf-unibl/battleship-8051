#ifndef FAZA_IGRE_H_
#define FAZA_IGRE_H_

const enum gameStages
{
    ESTABLISHING_CONNECTION = 0,
    SETTING_SHIPS,
    WAITING_OPPONENTS_SETTING,
    AIMING,
    ACCEPTANCE_OF_SHOT,
    END_WIN,
    END_LOSE,
    WAITING_FOR_RESPONCE
};

void initGameStage();

#endif
