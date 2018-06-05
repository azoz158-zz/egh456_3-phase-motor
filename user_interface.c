/* --------------------------------------
 *
 * @File:       user_interface.c
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "user_interface.h"

extern tCanvasWidget g_sBackground;
extern tCanvasWidget g_sMenuPage;
extern tCanvasWidget g_sSettingPage1;
extern tCanvasWidget g_sSettingPage2;
extern tCanvasWidget g_sSettingPage3;
extern tCanvasWidget g_sGraphPage1;
extern tCanvasWidget g_sGraphPage2;
extern tCanvasWidget g_sGraphPage3;
extern tPushButtonWidget g_sMotorState;
extern tPushButtonWidget g_sMotorStartStop;
extern tPushButtonWidget g_sMenu;
extern tPushButtonWidget g_sSave1;
extern tPushButtonWidget g_sSave2;
extern tPushButtonWidget g_sSave3;
extern tPushButtonWidget g_sAddition1;
extern tPushButtonWidget g_sAddition2;
extern tPushButtonWidget g_sAddition3;
extern tPushButtonWidget g_sSubtraction1;
extern tPushButtonWidget g_sSubtraction2;
extern tPushButtonWidget g_sSubtraction3;
extern tPushButtonWidget g_sBack1;
extern tPushButtonWidget g_sBack2;
extern tPushButtonWidget g_sBack3;
extern tPushButtonWidget g_sBack4;
extern tPushButtonWidget g_sSetTemp;
extern tPushButtonWidget g_sSetSpeed;
extern tPushButtonWidget g_sSetCurrent;
extern tPushButtonWidget g_sGraphTemp;
extern tPushButtonWidget g_sGraphSpeed;
extern tPushButtonWidget g_sGraphCurrent;

static uint8_t motor_button = 1;

static char * getCurrentDateTime()
{
    static char t[30];
    struct tm * timeinfo;
    time_t t1 = time (NULL);
    timeinfo = localtime ( &t1 );
    strcpy(t,asctime(timeinfo));
    return t;
}

static void litLED(uint8_t speed, uint8_t status)
{
    if (speed > 0){
        GPIO_write(Board_LED0, Board_LED_ON);
        GPIO_write(Board_LED1, Board_LED_OFF);
        GPIO_write(Board_LED2, Board_LED_OFF);
    }
    else{
        if (status == MOTOR_IDLE){
            GPIO_write(Board_LED0, Board_LED_OFF);
            GPIO_write(Board_LED1, Board_LED_ON);
            GPIO_write(Board_LED2, Board_LED_OFF);
        }
        else{
            GPIO_write(Board_LED0, Board_LED_OFF);
            GPIO_write(Board_LED1, Board_LED_OFF);
            GPIO_write(Board_LED2, Board_LED_ON);
        }
    }
}

static void ChangeMotorState(uint8_t newstate, uint8_t * state)
{
    switch(newstate)
    {
        case MOTOR_IDLE:
            PushButtonTextSet(&g_sMotorState, "    Motor is in idle state    ");
            break;
        case MOTOR_STARTING:
            PushButtonTextSet(&g_sMotorState, "  Motor is in Starting state  ");
            break;
        case MOTOR_RUNNING:
            PushButtonTextSet(&g_sMotorState, "   Motor is in Running state  ");
            break;
        case MOTOR_ESTOPPING:
            PushButtonTextSet(&g_sMotorState, " Motor is in E-Stopping state ");
            break;
    }
    *state = newstate;
    WidgetPaint(WIDGET_ROOT);
}

static void updateHomeMenuOptions(uint8_t menu)
{
    switch(menu)
    {
    case 0:
        PushButtonFillColorSet(&g_sMotorStartStop, ClrWhite);
        PushButtonTextColorSet(&g_sMotorStartStop, ClrBlack);
        PushButtonFillColorSet(&g_sMenu, ClrBlack);
        PushButtonTextColorSet(&g_sMenu, ClrWhite);
        break;
    case 1:
        PushButtonFillColorSet(&g_sMotorStartStop, ClrBlack);
        PushButtonTextColorSet(&g_sMotorStartStop, ClrWhite);
        PushButtonFillColorSet(&g_sMenu, ClrWhite);
        PushButtonTextColorSet(&g_sMenu, ClrBlack);
        break;
    }
    WidgetPaint(WIDGET_ROOT);
}

static void updateMainMenuOptions(uint8_t menu)
{
    PushButtonFillColorSet(&g_sSetTemp, ClrBlack);
    PushButtonTextColorSet(&g_sSetTemp, ClrWhite);
    PushButtonFillColorSet(&g_sSetSpeed, ClrBlack);
    PushButtonTextColorSet(&g_sSetSpeed, ClrWhite);
    PushButtonFillColorSet(&g_sSetCurrent, ClrBlack);
    PushButtonTextColorSet(&g_sSetCurrent, ClrWhite);
    PushButtonFillColorSet(&g_sGraphTemp, ClrBlack);
    PushButtonTextColorSet(&g_sGraphTemp, ClrWhite);
    PushButtonFillColorSet(&g_sGraphSpeed, ClrBlack);
    PushButtonTextColorSet(&g_sGraphSpeed, ClrWhite);
    PushButtonFillColorSet(&g_sGraphCurrent, ClrBlack);
    PushButtonTextColorSet(&g_sGraphCurrent, ClrWhite);
    PushButtonFillColorSet(&g_sBack1, ClrBlack);
    PushButtonTextColorSet(&g_sBack1, ClrWhite);
    switch(menu)
    {
    case 0:
        PushButtonFillColorSet(&g_sSetTemp, ClrWhite);
        PushButtonTextColorSet(&g_sSetTemp, ClrBlack);
        break;
    case 1:
        PushButtonFillColorSet(&g_sSetSpeed, ClrWhite);
        PushButtonTextColorSet(&g_sSetSpeed, ClrBlack);
        break;
    case 2:
        PushButtonFillColorSet(&g_sSetCurrent, ClrWhite);
        PushButtonTextColorSet(&g_sSetCurrent, ClrBlack);
        break;
    case 3:
        PushButtonFillColorSet(&g_sGraphTemp, ClrWhite);
        PushButtonTextColorSet(&g_sGraphTemp, ClrBlack);
        break;
    case 4:
        PushButtonFillColorSet(&g_sGraphSpeed, ClrWhite);
        PushButtonTextColorSet(&g_sGraphSpeed, ClrBlack);
        break;
    case 5:
        PushButtonFillColorSet(&g_sGraphCurrent, ClrWhite);
        PushButtonTextColorSet(&g_sGraphCurrent, ClrBlack);
        break;
    case 6:
        PushButtonFillColorSet(&g_sBack1, ClrWhite);
        PushButtonTextColorSet(&g_sBack1, ClrBlack);
        break;
    }
    WidgetPaint((tWidget *) &g_sMenuPage);
}

static void updateSettingOptions(uint8_t menu, uint8_t screen)
{
    switch(screen)
    {
        case SCREEN_2:
            PushButtonFillColorSet(&g_sAddition1, ClrBlack);
            PushButtonTextColorSet(&g_sAddition1, ClrWhite);
            PushButtonFillColorSet(&g_sSubtraction1, ClrBlack);
            PushButtonTextColorSet(&g_sSubtraction1, ClrWhite);
            PushButtonFillColorSet(&g_sSave1, ClrBlack);
            PushButtonTextColorSet(&g_sSave1, ClrWhite);
            switch(menu)
            {
                case 0:
                    PushButtonFillColorSet(&g_sAddition1, ClrWhite);
                    PushButtonTextColorSet(&g_sAddition1, ClrBlack);
                    break;
                case 1:
                    PushButtonFillColorSet(&g_sSubtraction1, ClrWhite);
                    PushButtonTextColorSet(&g_sSubtraction1, ClrBlack);
                    break;
                case 2:
                    PushButtonFillColorSet(&g_sSave1, ClrWhite);
                    PushButtonTextColorSet(&g_sSave1, ClrBlack);
                    break;
            }
            WidgetPaint((tWidget *) &g_sSettingPage1);
            break;
        case SCREEN_3:
            PushButtonFillColorSet(&g_sAddition2, ClrBlack);
            PushButtonTextColorSet(&g_sAddition2, ClrWhite);
            PushButtonFillColorSet(&g_sSubtraction2, ClrBlack);
            PushButtonTextColorSet(&g_sSubtraction2, ClrWhite);
            PushButtonFillColorSet(&g_sSave2, ClrBlack);
            PushButtonTextColorSet(&g_sSave2, ClrWhite);
            switch(menu)
            {
                case 0:
                    PushButtonFillColorSet(&g_sAddition2, ClrWhite);
                    PushButtonTextColorSet(&g_sAddition2, ClrBlack);
                    break;
                case 1:
                    PushButtonFillColorSet(&g_sSubtraction2, ClrWhite);
                    PushButtonTextColorSet(&g_sSubtraction2, ClrBlack);
                    break;
                case 2:
                    PushButtonFillColorSet(&g_sSave2, ClrWhite);
                    PushButtonTextColorSet(&g_sSave2, ClrBlack);
                    break;
            }
            WidgetPaint((tWidget *) &g_sSettingPage2);
            break;
        case SCREEN_4:
            PushButtonFillColorSet(&g_sAddition3, ClrBlack);
            PushButtonTextColorSet(&g_sAddition3, ClrWhite);
            PushButtonFillColorSet(&g_sSubtraction3, ClrBlack);
            PushButtonTextColorSet(&g_sSubtraction3, ClrWhite);
            PushButtonFillColorSet(&g_sSave3, ClrBlack);
            PushButtonTextColorSet(&g_sSave3, ClrWhite);
            switch(menu)
            {
                case 0:
                    PushButtonFillColorSet(&g_sAddition3, ClrWhite);
                    PushButtonTextColorSet(&g_sAddition3, ClrBlack);
                    break;
                case 1:
                    PushButtonFillColorSet(&g_sSubtraction3, ClrWhite);
                    PushButtonTextColorSet(&g_sSubtraction3, ClrBlack);
                    break;
                case 2:
                    PushButtonFillColorSet(&g_sSave3, ClrWhite);
                    PushButtonTextColorSet(&g_sSave3, ClrBlack);
                    break;
            }
            WidgetPaint((tWidget *) &g_sSettingPage3);
            break;
    }
}


static void DrawHomeScreen(tContext *sContext, UI_Params *params, Motor_Params *motorParams, Current_Params *currentParams, Temp_Params *tempParams)
{
    FrameDraw(sContext, "Home Page");
    WidgetPaint(WIDGET_ROOT);
    uint8_t button = OnButtonPress();
    char buffer[32];
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_UP && params->screen_option  > SCREEN_0)
            params->screen_option -= 1;
        else if (button == BUTTON_DOWN && params->screen_option < SCREEN_0_OPTION)
            params->screen_option += 1;
        if (button == BUTTON_UP || button == BUTTON_DOWN)
            updateHomeMenuOptions(params->screen_option);
        else if (button == BUTTON_SELECT)
        {
            if (params->screen_option == 0)
            {
                if (motor_button)
                {

                    MotorWakeUp(motorParams);

                    litLED(motorParams->current_speed, motorParams->state);
                    MotorStateChange(motorParams, EVENT_SETSPEED);
                    //MotorRotate(motorParams);
                    PushButtonTextSet(&g_sMotorStartStop, "Stop Motor");
                    motor_button = 0;
                    MotorRotate(motorParams);
                }
                else{
                    if (motorParams->state == MOTOR_ESTOPPING){
                        motor_button = 1;
                        PushButtonTextSet(&g_sMotorStartStop, "Start Motor");

                    }
                    MotorEmergencyStop(motorParams);
                    MotorStateChange(motorParams, EVENT_BRAKE);
                    litLED(motorParams->current_speed, motorParams->state);
                }
                WidgetPaint(WIDGET_ROOT);
            }
            else
            {
                params->screen = SCREEN_1;
                params->screen_option = DEFAULT_SCREEN_OPTION;
                break;
            }
        }
        WidgetMessageQueueProcess();
        GrStringDraw(sContext, getCurrentDateTime(), 24, 65, 25, 1);
        if (motorParams->state != params->data.motor_state)
            ChangeMotorState(motorParams->state, &(params->data.motor_state));
                sprintf(buffer,"  Desired Speed: %02d revs/s   ", motorParams->desired_speed);
        GrStringDraw(sContext, buffer, 31, 35, 75, 1);
                sprintf(buffer,"  Actual Speed: %02d revs/s    ", motorParams->average);
        GrStringDraw(sContext, buffer, 31, 35, 100, 1);
        sprintf(buffer,"Temperature: %.2f degree Celsius", tempParams->avg);
        GrStringDraw(sContext, buffer, 31, 35, 125, 1);
                sprintf(buffer,"       Current: %05d mA        ", currentParams->current);
        GrStringDraw(sContext, buffer, 31, 35, 150, 1);

    }
}

static void DrawMainMenuScreen(tContext * sContext, UI_Params * params,Temp_Params *tempParams)
{
    FrameDraw(sContext, "Main Menu");
    WidgetPaint((tWidget *) &g_sMenuPage);
    uint8_t button = OnButtonPress();
    updateMainMenuOptions(params->screen_option);
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_UP && params->screen_option  > SCREEN_0)
            params->screen_option -= 1;
        else if (button == BUTTON_DOWN && params->screen_option < SCREEN_1_OPTION)
            params->screen_option += 1;
        if (button == BUTTON_UP || button == BUTTON_DOWN)
            updateMainMenuOptions(params->screen_option);
        else if (button == BUTTON_SELECT)
        {
            if (params->screen_option == 6){
                params->screen_option = 1;
                params->screen = SCREEN_0;
            }
            else{
                params->screen = params->screen_option + 2;
                params->screen_option = SCREEN_0;
            }
            break;
        }
        WidgetMessageQueueProcess();
    }
}

static void DrawSetting1Screen(tContext * sContext, UI_Params * params,Temp_Params *tempParams)
{
    FrameDraw(sContext, "Set Temperature Limit");
    WidgetPaint((tWidget *) &g_sSettingPage1);
    uint8_t button = OnButtonPress();
    updateSettingOptions(params->screen_option, SCREEN_2);
    char s[20];
    int16_t temp_limit=tempParams->upper_limit;
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_UP && params->screen_option  > SCREEN_0)
            params->screen_option -= 1;
        else if (button == BUTTON_DOWN && params->screen_option < SCREEN_2_OPTION)
            params->screen_option += 1;
        if (button == BUTTON_UP || button == BUTTON_DOWN)
            updateSettingOptions(params->screen_option, SCREEN_2);
        else if (button == BUTTON_SELECT)
        {
            switch (params->screen_option)
            {
                case 0:
                    if (temp_limit < 85)
                        temp_limit++;
                    break;
                case 1:
                    if (temp_limit > 10)
                        temp_limit--;
                    break;
                case 2:
                    //save temp
                    params->screen = SCREEN_1;
                    params->screen_option = DEFAULT_SCREEN_OPTION;
                    break;
            }
        }
        sprintf(s, "%04d degree Celsius",temp_limit);
        if (params->screen == SCREEN_1)
            break;
        WidgetMessageQueueProcess();
        GrStringDraw(sContext, s, 19, 90, 90, 1);
    }
}

static void DrawSetting2Screen(tContext * sContext, UI_Params * params, Motor_Params * motorParams,Temp_Params *tempParams)
{
    FrameDraw(sContext, "Set Speed");
    WidgetPaint((tWidget *) &g_sSettingPage2);
    uint8_t button = OnButtonPress();
    updateSettingOptions(params->screen_option, SCREEN_3);
    char s[16];
    int32_t speed= motorParams->desired_speed;
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_UP && params->screen_option  > SCREEN_0)
            params->screen_option -= 1;
        else if (button == BUTTON_DOWN && params->screen_option < SCREEN_3_OPTION)
            params->screen_option += 1;
        if (button == BUTTON_UP || button == BUTTON_DOWN)
            updateSettingOptions(params->screen_option, SCREEN_3);
        else if (button == BUTTON_SELECT)
        {
            switch (params->screen_option)
            {
                case 0:
                    if (speed < MAX_MOTOR_SPEED)
                    speed++;
                    break;
                case 1:
                    if (speed > 0)
                        speed--;
                    break;
                case 2:
                    //save speed to desired
                    //
                    params->screen = SCREEN_1;
                    params->screen_option = 1;
                    if (speed != motorParams->desired_speed)
                    {
                        if (motorParams->state == MOTOR_IDLE)
                        {
                            MotorRotate(motorParams);
                            motor_button = 0;
                            PushButtonTextSet(&g_sMotorStartStop, "Stop Motor");
                        }
                        MotorStateChange(motorParams, EVENT_SETSPEED);

                    }
                    if (speed == 0)
                    {
                        motor_button = 1;
                        PushButtonTextSet(&g_sMotorStartStop, "Start Motor");
                        MotorStateChange(motorParams, EVENT_SETSPEED_TO_ZERO);
                        //set desired speed = 0
                    }
                    motorParams->desired_speed = speed;
                    break;
            }
        }
        sprintf(s, "%02d revs/s",speed);
        if (params->screen == SCREEN_1)
            break;
        WidgetMessageQueueProcess();
        GrStringDraw(sContext, s, 15, 116, 90, 1);
    }
}

static void DrawSetting3Screen(tContext * sContext, UI_Params * params,Temp_Params *tempParams, Current_Params *currentParams)
{
    FrameDraw(sContext, "Set Current Limit");
    WidgetPaint((tWidget *) &g_sSettingPage3);
    uint8_t button = OnButtonPress();
    updateSettingOptions(params->screen_option, SCREEN_4);
    char s[10];
    int16_t current_limit=currentParams->upper_limit;
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_UP && params->screen_option  > SCREEN_0)
            params->screen_option -= 1;
        else if (button == BUTTON_DOWN && params->screen_option < SCREEN_4_OPTION)
            params->screen_option += 1;
        if (button == BUTTON_UP || button == BUTTON_DOWN)
            updateSettingOptions(params->screen_option, SCREEN_4);
        else if (button == BUTTON_SELECT)
        {
            switch (params->screen_option)
            {
                case 0:
                    if (current_limit < 5000)
                        current_limit++;
                    break;
                case 1:
                    if (current_limit > 0)
                        current_limit--;
                    break;
                case 2:
                    //save current
                    params->screen = SCREEN_1;
                    params->screen_option = 2;
                    break;
            }
        }
        sprintf(s, "%05d mA",current_limit);
        if (params->screen == SCREEN_1)
            break;
        WidgetMessageQueueProcess();
        GrStringDraw(sContext, s, 10, 130, 90, 1);
    }
}

static void DrawGraph1Screen(tContext * sContext, UI_Params * params, Temp_Params *tempParams)
{
    FrameDraw(sContext, "Temperature Graph");
    WidgetPaint((tWidget *) &g_sGraphPage1);
    uint8_t button = OnButtonPress();
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_SELECT)
        {
            params->screen = SCREEN_1;
            params->screen_option = 3;
            break;
        }
        WidgetMessageQueueProcess();
    }
}

static void DrawGraph2Screen(tContext * sContext, UI_Params * params, Temp_Params *tempParams)
{
    FrameDraw(sContext, "Speed Graph");
    WidgetPaint((tWidget *) &g_sGraphPage2);
    uint8_t button = OnButtonPress();
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_SELECT)
        {
            params->screen = SCREEN_1;
            params->screen_option = 4;
            break;
        }
        WidgetMessageQueueProcess();
    }
}

static void DrawGraph3Screen(tContext * sContext, UI_Params * params, Temp_Params *tempParams)
{
    FrameDraw(sContext, "Current Graph");
    WidgetPaint((tWidget *) &g_sGraphPage3);
    uint8_t button = OnButtonPress();
    while(1)
    {
        tempParams->current_temp = getObjTemp();
        button = OnButtonPress();
        if (button == BUTTON_SELECT)
        {
            params->screen = SCREEN_1;
            params->screen_option = 5;
            break;
        }
        WidgetMessageQueueProcess();
    }
}


void UserInterfaceInit(uint32_t systemclock, tContext * sContext, UI_Params * params)
{
    PinoutSet();
    Kentec320x240x16_SSD2119Init(systemclock);
    GrContextInit(sContext, &g_sKentec320x240x16_SSD2119);
    params->screen = SCREEN_0;
    params->screen_option = SCREEN_0;
    params->data.motor_state = MOTOR_IDLE;
    initInterfaceButton();
    initInterfaceSpeed(&(params->data.speed), DATA_SAMPLE);
    //root
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sBackground);
    WidgetAdd((tWidget *)&g_sBackground, (tWidget *)&g_sMotorState);
    WidgetAdd((tWidget *)&g_sBackground, (tWidget *)&g_sMotorStartStop);
    WidgetAdd((tWidget *)&g_sBackground, (tWidget *)&g_sMenu);

    //main menu
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sSetTemp);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sSetSpeed);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sSetCurrent);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sGraphTemp);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sGraphSpeed);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sGraphCurrent);
    WidgetAdd((tWidget *)&g_sMenuPage, (tWidget *) &g_sBack1);

    //set Temp Limit
    WidgetAdd((tWidget *)&g_sSettingPage1, (tWidget *) &g_sAddition1);
    WidgetAdd((tWidget *)&g_sSettingPage1, (tWidget *) &g_sSubtraction1);
    WidgetAdd((tWidget *)&g_sSettingPage1, (tWidget *) &g_sSave1);
    //WidgetAdd((tWidget *)&g_sSettingPage1, (tWidget *) &g_sBack2);

    //set Speed
    WidgetAdd((tWidget *)&g_sSettingPage2, (tWidget *) &g_sAddition2);
    WidgetAdd((tWidget *)&g_sSettingPage2, (tWidget *) &g_sSubtraction2);
    WidgetAdd((tWidget *)&g_sSettingPage2, (tWidget *) &g_sSave2);
    //WidgetAdd((tWidget *)&g_sSettingPage2, (tWidget *) &g_sBack3);

    //set Current Limit
    WidgetAdd((tWidget *)&g_sSettingPage3, (tWidget *) &g_sAddition3);
    WidgetAdd((tWidget *)&g_sSettingPage3, (tWidget *) &g_sSubtraction3);
    WidgetAdd((tWidget *)&g_sSettingPage3, (tWidget *) &g_sSave3);
    //WidgetAdd((tWidget *)&g_sSettingPage3, (tWidget *) &g_sBack4);

    //graph temp
    WidgetAdd((tWidget *)&g_sGraphPage1, (tWidget *) &g_sBack2);
    //graph speed
    WidgetAdd((tWidget *)&g_sGraphPage2, (tWidget *) &g_sBack3);
    //graph current
    WidgetAdd((tWidget *)&g_sGraphPage3, (tWidget *) &g_sBack4);
}

void UserInterfaceDraw(UI_Params * params, tContext * sContext, Motor_Params * motorParams, Current_Params * currentParams, Temp_Params * tempParams)
{
    switch(params->screen)
    {
        case SCREEN_0:
            DrawHomeScreen( sContext, params, motorParams, currentParams, tempParams);
            break;
        case SCREEN_1:
            DrawMainMenuScreen(sContext, params,tempParams);
            break;
        case SCREEN_2:
            DrawSetting1Screen(sContext, params, tempParams);
            break;
        case SCREEN_3:
            DrawSetting2Screen(sContext, params, motorParams, tempParams);
            break;
        case SCREEN_4:
            DrawSetting3Screen(sContext, params, tempParams, currentParams);
            break;
        case SCREEN_5:
            DrawGraph1Screen(sContext, params, tempParams);
            break;
        case SCREEN_6:
            DrawGraph2Screen(sContext, params, tempParams);
            break;
        case SCREEN_7:
            DrawGraph3Screen(sContext, params, tempParams);
            break;

    }
}







