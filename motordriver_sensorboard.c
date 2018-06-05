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
    param->current_speed = 0.0;
    //System_printf("%d %d",param->motor.duty_cycle, param->motor.period);
    param->desired_speed = 0;// ((float)param->motor.duty_cycle/(float)param->motor.period)*MAX_MOTOR_SPEED;
    //MotorRotate(param);
}

void MotorEmergencyStop(Motor_Params * param)
{
    param->current_speed = 0.0;
    param->desired_speed = 0;
    motorEmergencyStop();
}

void MotorWakeUp(Motor_Params * param)
{
    param->desired_speed = 37;
    param->current_speed = 37/2;
    motorWakeup();
}

void SetSpeedAverage(Motor_Params * param)
{
    uint8_t index = 0;
        for (; index < (sizeof(param->sample)/sizeof(uint8_t)); index++){
            param->average += param->sample[index];
        }
        param->average = param->average / (sizeof(param->sample)/sizeof(uint8_t));
}

void MotorRotate(Motor_Params * param)
{
    setPWMSignalDutyCycle((Motor *)&(param->motor));
    //set new speed here;
    uint8_t speed = (uint8_t)param->current_speed;
    if (speed > param->desired_speed)
        param->current_speed -= 0.005;
    else if (speed < param->desired_speed)
        param->current_speed += 0.005;
    else
        MotorStateChange(param, EVENT_SETSPEED);
    //this is after just no delay in sending new pwm signal
}

void MotorAdjustSpeed(Motor_Params * param)
{
    uint8_t error = (uint8_t)param->current_speed - param->actual_speed;
    adjustDutyCycle(&(param->motor), error, (uint8_t)param->current_speed);
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
