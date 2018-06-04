/* --------------------------------------
 *
 * @File:       motordriver_sensorboard.c
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */
#include <xdc/std.h>
#include "motordriver_sensorboard.h"

void MotorInit(Motor_Params * param, uint32_t system_clock, int32_t pwm_frequecy)
{
    //configure PWM Signal
    configurePWMSignal();
    //configure Hall Sensor
    configureHallSensor();
    //configure Fault & OTW
    configureFault_OTW();

    initPWMSignal(system_clock, pwm_frequecy, (Motor *)&(param->motor));
    param->state = MOTOR_IDLE;
    param->actual_speed = 0;
    //System_printf("%d %d",param->motor.duty_cycle, param->motor.period);
    param->desired_speed = ((float)param->motor.duty_cycle/(float)param->motor.period)*MAX_MOTOR_SPEED;
    MotorRotate(param);
}

uint8_t MotorRotate(Motor_Params * param)
{
    return setPWMSignalDutyCycle((Motor *)&(param->motor));
    //set new speed here;
    //this is after just no delay in sending new pwm signal
}

void MotorAdjustSpeed(Motor_Params * param)
{
    uint8_t error = param->desired_speed - param->actual_speed;
    adjustDutyCycle(&(param->motor), error, param->desired_speed);
}

uint8_t MotorFaultCondition(Motor_Params *param, char * feedback)
{
    uint8_t motor_condition = checkFaultCondition(&(param->motor), feedback);

    return motor_condition;
}

void MotorStateChange(Motor_Params *param, uint8_t event)
{
    switch(param->state)
    {
        case MOTOR_IDLE:
            if (event == EVENT_SETSPEED)
                param->state = MOTOR_STARTING;
            break;
        case MOTOR_STARTING:
            if (event == EVENT_SETSPEED)
                param->state = MOTOR_RUNNING;
            else if (event == EVENT_BRAKE)
                param->state = MOTOR_ESTOPPING;
            break;
        case MOTOR_RUNNING:
            if (event == EVENT_SETSPEED)
                param->state = MOTOR_RUNNING;
            else if (event == EVENT_BRAKE)
                param->state = MOTOR_ESTOPPING;
            else
                param->state = MOTOR_IDLE;
            break;
        case MOTOR_ESTOPPING:
            if (event == EVENT_BRAKE)
                param->state = MOTOR_IDLE;
            break;
    }

}
