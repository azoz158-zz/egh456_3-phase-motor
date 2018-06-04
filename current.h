/* --------------------------------------
 *
 * @File:       current.h
 * @Author:     Abdulaziz Alahmadi
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#ifndef CURRENT_H_
#define CURRENT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"

#ifndef MAXIMUM_BYTES_SIZE
    #define MAXIMUM_BYTES_SIZE 4096.0
#endif
#ifndef VOLTAGE
    #define VOLTAGE 3.3
#endif
#ifndef REFERENCE_VOLTAGE
    #define REFERENCE_VOLTAGE 2.5
#endif
#ifndef VOLTAGE_OFFSET
    #define VOLTAGE_OFFSET 0.5
#endif

typedef struct Current_sensor{
    //used for storing data from ADC FIFO, must be as large as the FIFO for sequencer in use. Sequencer 1 has FIFO depth of 4
    volatile uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32CurrentValueHolder;
    volatile uint32_t avg;
    volatile uint32_t sample[5];
}Current_sensor;

typedef struct Current_Params{
   uint32_t current;
   uint32_t upper_limit;
   Current_sensor currentsensor;
}Current_Params;

typedef struct Power{
    uint8_t value_full;
    uint8_t value_index;
    uint8_t start_index;
    int16_t data[100];
}Power;

void CurrentInit(Current_Params * params);

void SetCurrentUpperLimit(Current_Params * params, int16_t limit);

void SetCurrentAverage(Current_Params * params); //call this every time you get 5 samples to update avg

bool isCurrentWithinLimit(Current_Params * params);

void initInterfacePower(Power * power, uint8_t sample);

void setInterfacePower(Power * power, int16_t newpower);//calculate and send new_power_consumption

#endif /* CURRENT_H_ */
