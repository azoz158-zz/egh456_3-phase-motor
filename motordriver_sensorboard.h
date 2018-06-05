/* --------------------------------------
 *
 * @File:       motordriver_sensorboard.h
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#ifndef MOTORDRIVER_SENSORBOARD_H_
#define MOTORDRIVER_SENSORBOARD_H_

#include <stdint.h>
#include "motor.h"

#ifndef MOTOR_IDLE
    #define MOTOR_IDLE      1
#endif
#ifndef MOTOR_STARTING
    #define MOTOR_STARTING  2
#endif
#ifndef MOTOR_RUNNING
    #define MOTOR_RUNNING   3
#endif
#ifndef MOTOR_ESTOPPING
    #define MOTOR_ESTOPPING 4
#endif

#ifndef EVENT_SETSPEED
    #define EVENT_SETSPEED 1
#endif
#ifndef EVENT_SETSPEED_TO_ZERO
    #define EVENT_SETSPEED_TO_ZERO 0
#endif
#ifndef EVENT_BRAKE
    #define EVENT_BRAKE 2
#endif
/*
 * Data structure used to keep track of motor attribuites
 *
 * Members:
 * --------
 *      state:  The current sate of the motor
 *      rpm:    Current rpm of the motor
 *      motor:  This object keeps track of internal attributes such as current hall sensor state and duty cycle
 *
 */
typedef struct Motor_Params{
    uint8_t state;
    uint8_t actual_speed;
    uint8_t desired_speed;
    uint8_t average;
    float current_speed;
    uint8_t sample[5];
    Motor motor;
}Motor_Params;

/*
 * This function initialises the motor parameters and calls configureMotor()
 * function that configures and initialises all necessary timers, ports and pins.
 *
 * Arguments:
 * ----------
 *      param:          Pointer to the motor paramterms that keeps track of current state and rpm
 *      system_clock:   Frequency of system clock.
 *      pwm_frequency:  Frequency of PWM signal for the motor.
 *
 */

void SetSpeedAverage(Motor_Params * param);

void MotorInit(Motor_Params * param, uint32_t system_clock, int32_t pwm_frequecy);

void MotorRotate(Motor_Params * param);

void MotorEmergencyStop(Motor_Params * param);

void MotorWakeUp(Motor_Params * param);

void MotorAdjustSpeed(Motor_Params * param);//change motor state here according to speed

uint8_t MotorFaultCondition(Motor_Params * param, char *feedback);

void MotorStateChange(Motor_Params *param, uint8_t event);
/*
void MotorSetSpeed(Motor_Param *motor_param, int32_t s);

void MotorHitBrake(Motor_Param *motor_param);

static void SetMotorNewState(uint8_t *current);
*/



#endif /* MOTORDRIVER_SENSORBOARD_H_ */
