/* --------------------------------------
 *
 * @File:       user_interface.h
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */
#ifndef USER_INTERFACE_H_
#define USER_INTERFACE_H_
#include <ti/drivers/GPIO.h>
#include <stdint.h>
#include <stdbool.h>

#include "motor.h"
#include "interface_button.h"
#include "motordriver_sensorboard.h"
#include "current.h"
#include "temperature.h"

#ifndef SCREEN_0
    #define SCREEN_0 0
#endif
#ifndef SCREEN_1
    #define SCREEN_1 1
#endif
#ifndef SCREEN_2
    #define SCREEN_2 2
#endif
#ifndef SCREEN_3
    #define SCREEN_3 3
#endif
#ifndef SCREEN_4
    #define SCREEN_4 4
#endif
#ifndef SCREEN_5
    #define SCREEN_5 5
#endif
#ifndef SCREEN_6
    #define SCREEN_6 6
#endif
#ifndef SCREEN_7
    #define SCREEN_7 7
#endif

#ifndef DEFAULT_SCREEN_OPTION
    #define DEFAULT_SCREEN_OPTION 0
#endif
#ifndef SCREEN_0_OPTION
    #define SCREEN_0_OPTION 1
#endif
#ifndef SCREEN_1_OPTION
    #define SCREEN_1_OPTION 6
#endif
#ifndef SCREEN_2_OPTION
    #define SCREEN_2_OPTION 2
#endif
#ifndef SCREEN_3_OPTION
    #define SCREEN_3_OPTION 2
#endif
#ifndef SCREEN_4_OPTION
    #define SCREEN_4_OPTION 2
#endif

#ifndef DATA_SAMPLE
    #define DATA_SAMPLE 100
#endif


typedef struct Data{
    uint8_t motor_state;
    uint8_t temperature; //buffer to store 100 temperature samples
    uint8_t power; //buffer to store 100 power usage samples
    Speed speed; //struct to store speed for display purpose
}Data;



typedef struct UI_Params{
    uint8_t screen;
    uint8_t screen_option; //keeps track of which option is selected
    Data data;
}UI_Params;

//this function initlises all necessary pheripheral ports and interrupt necessary for user interface
//such as buttons, display, variables and so on.
void UserInterfaceInit(uint32_t systemclock, tContext * sContext, UI_Params * params);
void UserInterfaceDraw(UI_Params * params, tContext * sContext, Motor_Params * motorParams, Current_Params * currentParams, Temp_Params * tempParams);


 /* USER_INTERFACE_H_ */
#endif
