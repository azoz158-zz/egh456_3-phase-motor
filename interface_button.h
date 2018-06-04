/* --------------------------------------
 *
 * @File:       interface_button.h
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#ifndef INTERFACE_BUTTON_H_
#define INTERFACE_BUTTON_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
//#include "driverlib/rom.h"
//#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
//#include "driverlib/udma.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "drivers/frame.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "drivers/pinout.h"
#include "drivers/touch.h"
#include "drivers/buttons.h"

#ifndef BUTTON_NONE
    #define BUTTON_NONE 0
#endif
#ifndef BUTTON_UP
    #define BUTTON_UP 1
#endif
#ifndef BUTTON_DOWN
    #define BUTTON_DOWN 2
#endif
#ifndef BUTTON_SELECT
    #define BUTTON_SELECT 3
#endif


void initInterfaceButton();

uint8_t validInterfaceButton(uint8_t screen, uint8_t *button_pressed);

uint8_t OnButtonPress();

#endif /* INTERFACE_BUTTON_H_ */
