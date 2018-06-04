/* --------------------------------------
 *
 * @File:       interface_button.c
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#include "interface_button.h"


static unsigned char ucDelta = 0;
static unsigned char ua = 0;
static unsigned char ucState =0;


static const uint8_t valid_button[5][3] = {{1,1,1}, //screen 1 //home
                                           {0,0,1}, //screen 2 //temperatu
                                           {0,0,1}, //screen 3 //power
                                           {1,1,1}, //screen 4 //speed
                                           {0,0,1}};//screen 5 //motor state


void initInterfaceButton()
{
    ButtonsInit(ALL_BUTTONS);

}

uint8_t validInterfaceButton(uint8_t screen, uint8_t *button_pressed)
{
    if (valid_button[screen][(*button_pressed)-1])
    {
        return 1;
    }
    //critical section
    *button_pressed = BUTTON_NONE;

    return 0;
}

uint8_t OnButtonPress()
{
    ucState = ButtonsPoll(&ucDelta, &ua);
    if (BUTTON_PRESSED(UP_BUTTON, ucState, ucDelta) )
        return BUTTON_UP;
    if (BUTTON_PRESSED(DOWN_BUTTON, ucState, ucDelta) )
            return BUTTON_DOWN;
    if (BUTTON_PRESSED(SELECT_BUTTON, ucState, ucDelta) )
            return BUTTON_SELECT;
    return BUTTON_NONE;
}

