/* --------------------------------------
 *
 * @File:       motor.h
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef PWM_FREQUENCY
    #define PWM_FREQUENCY               200000  //default PWM Frequency 20KHz
#endif
#ifndef NUM_MOTOR_PAIR_POLES
    #define NUM_MOTOR_PAIR_POLES             4
#endif
#ifndef MAX_MOTOR_SPEED
    #define MAX_MOTOR_SPEED       75 //in revs/s
#endif

#ifndef DEFAULT_DUTYCYCLE
    #define DEFAULT_DUTYCYCLE    50
#endif
#ifndef Kp
    #define Kp 5
#endif
#ifndef Ki
    #define Ki 1
#endif
#ifndef MAX_ERROR
    #define MAX_ERROR MAX_MOTOR_SPEED * Kp + MAX_MOTOR_SPEED * Ki
#endif

#ifndef STATES
    #define STATES                6
#endif
#ifndef OUT_OF_RANGE_INDEX
    #define OUT_OF_RANGE_INDEX    6
#endif

#ifndef FAULT_1
    #define FAULT_1               1
#endif
#ifndef FAULT_2
    #define FAULT_2               2
#endif
#ifndef FAULT_3
    #define FAULT_3               3
#endif
#ifndef NO_FAULT
    #define NO_FAULT              0
#endif

/*
 * Data structure used to keep track of motor internal attribuites
 *
 * Members:
 * --------
 *      hall_sensor_state:  Current state of hall sensors H1 H2 H3
 *      fault:              Current fault condition of the motor
 *      duty_cycle:         Current PWM signals duty cycle (not in %)
 *
 */
typedef struct Motor{
    uint8_t hall_sensor_state[3];
    uint8_t fault[2];
    uint32_t duty_cycle;
    uint32_t period;
}Motor;

typedef struct Speed{
    uint8_t value_full;
    uint8_t value_index;
    uint8_t start_index;
    int16_t data[100];
}Speed;

void motorWakeup();

void motorEmergencyStop();

void configurePWMSignal();

void configureHallSensor();

void configureFault_OTW();

void initPWMSignal(uint32_t system_clock, int32_t pwm_frequecy, Motor *motor);

void adjustDutyCycle(Motor *motor, uint8_t error, uint8_t desired);

uint8_t checkFaultCondition(Motor *motor, char *feedback);

uint8_t setPWMSignalDutyCycle(Motor *motor);

void initInterfaceSpeed(Speed * speed, uint8_t sample);

void setInterfaceSpeed(Speed * speed, int16_t newspeed);

#endif /* MOTOR_H_ */
