/*
 * temperature.h
 *
 *  Created on: May,2018
 *      Author: n9118462
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <math.h>
#include <string.h>
/* XDCtools Header files*/
#include <xdc/runtime/System.h>

/* TI-RTOS Header files*/
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

/* Example/Board Header files*/
#include "Board.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"

//unsigned int    i;


#define RAM_1 0x4000
#define RAM_2 0x4001
#define RAM_3 0x4002
#define RAM_4 0x4003
#define RAM_5 0x4004
#define RAM_6 0x4005
#define RAM_7 0x4006
#define RAM_8 0x4007
#define RAM_9 0x4008

//The default I2C address for the MLX90632 on the SparkX breakout is 0x3B. 0x3A is also possible.
#define MLX90632_DEFAULT_ADDRESS 0x3B

//Registers
#define EE_VERSION 0x240B

//32 bit constants
#define EE_P_R 0x240C
#define EE_P_G 0x240E
#define EE_P_T 0x2410
#define EE_P_O 0x2412
#define EE_Aa 0x2414
#define EE_Ab 0x2416
#define EE_Ba 0x2418
#define EE_Bb 0x241A
#define EE_Ca 0x241C
#define EE_Cb 0x241E
#define EE_Da 0x2420
#define EE_Db 0x2422
#define EE_Ea 0x2424
#define EE_Eb 0x2426
#define EE_Fa 0x2428
#define EE_Fb 0x242A
#define EE_Ga 0x242C

//16 bit constants
#define EE_Ha 0x2481
#define EE_Hb 0x2482
#define EE_Gb 0x242E
#define EE_Ka 0x242F
#define EE_Kb 0x2430

//Control registers
#define EE_CONTROL 0x24D4
#define EE_I2C_ADDRESS 0x24D5
#define REG_I2C_ADDRESS 0x3000
#define REG_CONTROL 0x3001
#define REG_STATUS 0x3FFF

//Three measurement modes available
#define MODE_SLEEP 0b01
#define MODE_STEP 0b10
#define MODE_CONTINUOUS 0b11

//REG_STATUS bits
#define BIT_DEVICE_BUSY 10
#define BIT_EEPROM_BUSY 9
#define BIT_BROWN_OUT 8
#define BIT_CYCLE_POS 2 //6:2 = 5 bits
#define BIT_NEW_DATA 0

//REG_CONTROL bits
#define BIT_SOC 3
#define BIT_MODE 1 //2:1 = 2 bits

#define I2C_SPEED_STANDARD        100000
#define I2C_SPEED_FAST            400000

double P_R;
double P_G;
double P_T;
double P_O;
double Ea;
double Eb;
double Fa;
double Fb;
double Ga;
double Gb;
double Ka;
double Ha;
double Hb;

static uint32_t EE_PR;
static uint32_t EE_PG;
static uint32_t EE_PT;
static uint32_t EE_PO;
static uint32_t EEEa;
static uint32_t EEEb;
static uint32_t EEFb;
static uint32_t EEGa;
static uint32_t EEFa;
static uint32_t EEGb;
static uint32_t EEHa;
static uint32_t EEHb;
static uint32_t EEKa;

static uint32_t outputPointerEE_P_R;

//double TOdut = 25.0; //Assume 25C for first iteration
//double TO0 = 25.0; //object temp from previous calculation
//double TA0 = 25.0; //ambient temp from previous calculation
static double sensorTemp; //Internal temp of the MLX sensor

static uint16_t        temperature;
static uint8_t         txBuffer[2];
static uint8_t         rxBuffer[2];
static I2C_Handle      i2c;
static I2C_Params      i2cParams;
static I2C_Transaction i2cTransaction;

typedef struct Temp_Params{
   float current_temp;
   float avg;
   int16_t upper_limit;
   float sample[5];
}Temp_Params;


// Functions used in the Program
uint16_t EEPROM_Read16(uint16_t address);
uint32_t EEPROM_Read32(uint16_t address);
void writeRegister16(uint16_t address, uint16_t val);
uint8_t getMode();
void setSOC();
uint16_t getStatus();
void clearNewData();
void all32 ();
void all16();
void conversion();
uint8_t getCyclePosition();
float getObjTemp();
void i2c_setup();
void i2c_init();

void TemperatureInit(Temp_Params * params);
void SetTemperatureUpperLimit(Temp_Params * params, int16_t limit);
void SetTEmperatureAverage(Temp_Params * params);







#endif /* TEMPERATURE_H_ */
