#ifndef BUTTONS_H_
#define BUTTONS_H_

const enum button
{
    BUTTON_UP = 2,
    BUTTON_RIGHT = 3,
    BUTTON_LEFT = 4,
    BUTTON_DOWN = 5,
    BUTTON_ENTER = 6
};

void initButtons();
void buttonResponceSetting(unsigned char buttonIndex);
void buttonResponceShooting(unsigned char buttonIndex);
void buttonResponce();

#endif
