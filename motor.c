/* --------------------------------------
 *
 * @File:       motor.c
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#include <string.h>

#include "motor.h"

#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

//HALL sequence H1 H2 H3
const uint8_t HALL[6][3] =  {{0, 0, 1},
                             {0, 1, 0},
                             {0, 1, 1},
                             {1, 0, 0},
                             {1, 0, 1},
                             {1, 1, 0}};

//PWM sequence PWM_A PWM_B PWM_C
const uint8_t PWM[6][3] =   {{1, 1, 0},
                             {0, 1, 1},
                             {1, 1, 0},
                             {1, 0, 1},
                             {1, 0, 1},
                             {0, 1, 1}};

//RESET sequence RESET_A RESET_B RESET_C

const uint8_t RESET[6][3] =  {{1, 0, 1},
                             {1, 1, 0},
                             {0, 1, 1},
                             {1, 0, 0},
                             {1, 0, 1},
                             {1, 1, 0}};


static void initHallSensorState(Motor *motor)
{
    if (GPIOPinRead(GPIO_PORTL_BASE, GPIO_PIN_3))
        motor->hall_sensor_state[0] = 1;
    else
        motor->hall_sensor_state[0] = 0;

    if (GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_4))
        motor->hall_sensor_state[1] = 1;
    else
        motor->hall_sensor_state[1] = 0;

    if (GPIOPinRead(GPIO_PORTP_BASE, GPIO_PIN_5))
        motor->hall_sensor_state[2] = 1;
    else
        motor->hall_sensor_state[2] = 0;
}


static uint8_t determineHallStateIndex(uint8_t * hall_sensor)
{
    uint8_t index;
    for(index=0; index<STATES; index++)
    {
        if (HALL[index][0] == hall_sensor[0] &&
            HALL[index][1] == hall_sensor[1] &&
            HALL[index][2] == hall_sensor[2])
            return index;
    }
    return OUT_OF_RANGE_INDEX;
}



void configurePWMSignal()
{
    //Peripheral enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);    //PWM_A & PWM_B & PWM_C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);    //RESET_B & RESET_C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);    //RESET_A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);   //PM0 & PM1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);   //PM2

    //Configure peripheral
    GPIOPinConfigure(GPIO_PM2_T3CCP0);//Timer3 PM2
    GPIOPinConfigure(GPIO_PM1_T2CCP1);//Timer2 PM1
    GPIOPinConfigure(GPIO_PM0_T2CCP0);//Timer2 PM0

    //PWM
    GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0);
    //RESET
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);

}


void configureHallSensor()
{
    //Peripheral enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);    //H1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);    //H2 & H3

    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);//H1
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5);//H2 & H3

    //Interrupt for hall sensor pins PL3 & PP4 & PP5
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntDisable(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOIntDisable(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOIntClear(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOIntClear(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOIntTypeSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_BOTH_EDGES);
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTL_BASE, GPIO_PIN_3);
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_PIN_4|GPIO_PIN_5);
    IntMasterEnable();
}


void configureFault_OTW()
{
    //Peripheral enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);    //OTW
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);    //FAULT

    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_2); //OTW
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6); //FAULT
}


void initPWMSignal(uint32_t system_clock, int32_t pwm_frequecy, Motor *motor)
{
    if (!pwm_frequecy) pwm_frequecy = PWM_FREQUENCY; //set default if 0

    motor->period = system_clock/pwm_frequecy;
    motor->duty_cycle = (uint32_t) DEFAULT_DUTYCYCLE * motor->period * 1/100;

    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, motor->period);

    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER2_BASE, TIMER_A, motor->period);
    TimerLoadSet(TIMER2_BASE, TIMER_B, motor->period);
    TimerControlLevel(TIMER2_BASE,TIMER_A, true);
    TimerControlLevel(TIMER3_BASE,TIMER_A, false);
    TimerControlLevel(TIMER2_BASE,TIMER_B, false);

    TimerEnable(TIMER3_BASE, TIMER_A);
    TimerEnable(TIMER2_BASE, TIMER_A|TIMER_B);

    TimerSynchronize(TIMER0_BASE, TIMER_2A_SYNC|TIMER_2B_SYNC|TIMER_3A_SYNC);
    initHallSensorState(motor);
}

void adjustDutyCycle(Motor *motor, uint8_t error, uint8_t desired)
{
    int16_t output = Kp *error + Ki * error;
    if (output < 0) output = 0;
    else if (output > MAX_ERROR) output = MAX_ERROR;
    motor->duty_cycle = desired * (float)(motor->period / MAX_MOTOR_SPEED); //adjust the duty cycle range
}



uint8_t checkFaultCondition(Motor * motor, char *feedback)
{

    if (HWREG(GPIO_PORTC_BASE | (GPIO_PIN_6 << 2)))
        motor->fault[0]=0x01;
    else
        motor->fault[0]=0x00;
    if (HWREG(GPIO_PORTL_BASE | (GPIO_PIN_2 << 2)))
        motor->fault[1]=0x01;
    else
        motor->fault[1]=0x00;

    if (!motor->fault[0] && !motor->fault[1])
    {
        strcpy(feedback, "Overtemperature warning and (overtemperature or overcurrent or undervoltage protection) occured");
        return FAULT_1;
    }
    else if (!motor->fault[0] && motor->fault[1])
    {
        strcpy(feedback, "Overcurrent shut-down or GVDD undervoltage protection occured");
        return FAULT_2;
    }
    else if (motor->fault[0] && !motor->fault[1])
    {
        strcpy(feedback, "Overtemperature warning");
        return FAULT_3;
    }

    return NO_FAULT;
}


uint8_t setPWMSignalDutyCycle(Motor *motor)
{
    uint8_t index = determineHallStateIndex((uint8_t *) &(motor->hall_sensor_state));

    if (index != OUT_OF_RANGE_INDEX)
    {
        HWREG(TIMER3_BASE|TIMER_O_TAV)=0;
        HWREG(TIMER2_BASE|TIMER_O_TBV)=0;
        HWREG(TIMER2_BASE|TIMER_O_TAV)=0;

        TimerMatchSet(TIMER3_BASE, TIMER_A, (PWM[index][0])? motor->duty_cycle : 0);//PWM_A
        TimerMatchSet(TIMER2_BASE, TIMER_B, (PWM[index][1])? motor->duty_cycle : 0);//PWM_B
       TimerMatchSet(TIMER2_BASE, TIMER_A, (PWM[index][2])? motor->duty_cycle : 0);//PWM_C
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, (RESET[index][0])? 0xff:0);
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_5, (RESET[index][1])? 0xff:0);
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_4, (RESET[index][2])? 0xff:0);
        return 1;
    }
    return 0;
}

void initInterfaceSpeed(Speed * speed, uint8_t sample)
{
    uint8_t i = 0;
    speed->value_full = 0;
    speed->value_index = 0;
    speed->start_index = 0;

    for (;i<sample;i++)
    {
        speed->data[i] = 0;
    }
}

void setInterfaceSpeed(Speed * speed, int16_t newspeed)
{
    speed->data[speed->value_index] = newspeed;
    speed->value_index += 1;
    if (speed->value_index >= 100)
    {
        speed->value_full += 1;
        speed->value_index = 0;
    }
    if (speed->value_full)
    {
        speed->start_index += 1;
        if (speed->start_index >= 100)
             speed->start_index = 0;
    }
}

