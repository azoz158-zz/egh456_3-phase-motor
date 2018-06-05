/*
 * temperature.c
 *
 *  Created on: 26May,2018
 *      Author: n9003096
 */
#include "temperature.h"

void i2c_init(){
    /* I2C2 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PL1_I2C2SCL);
    GPIOPinConfigure(GPIO_PL0_I2C2SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTL_BASE, GPIO_PIN_1);
    GPIOPinTypeI2C(GPIO_PORTL_BASE, GPIO_PIN_0);
}

// Takes 16 bit register address and returns the value in the register
uint16_t EEPROM_Read16(uint16_t address){

    uint8_t         txBuffer[2];
    uint8_t         rxBuffer[2];

    txBuffer[0] = (address>> 8);
    txBuffer[1] = (address & 0xFF);
    i2cTransaction.slaveAddress = 0x3A;
    i2cTransaction.writeBuf = txBuffer; /* Buffer to be written */
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    I2C_transfer(i2c, &i2cTransaction);

    uint8_t msb0 = rxBuffer[0];
    uint8_t lsb0 = rxBuffer[1];


    uint16_t lower = (uint16_t)msb0 << 8 | lsb0;

    return lower;


}
// Takes 32 bit register address and returns the value in the register

uint32_t EEPROM_Read32(uint16_t address){

    uint8_t         txBuffer[2];
    uint8_t         rxBuffer[4];

    txBuffer[0] = (address>> 8);
    txBuffer[1] = (address & 0xFF);
    i2cTransaction.slaveAddress = 0x3A;
    i2cTransaction.writeBuf = txBuffer; /* Buffer to be written */
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 4;

    I2C_transfer(i2c, &i2cTransaction);

    uint8_t msb0 = rxBuffer[0];
    uint8_t lsb0 = rxBuffer[1];
    uint8_t msb1 = rxBuffer[2];
    uint8_t lsb1 = rxBuffer[3];

    uint16_t lower = (uint16_t)msb0 << 8 | lsb0;
    uint16_t upper = (uint16_t)msb1 << 8 | lsb1;

    return outputPointerEE_P_R = (uint32_t)upper << 16 | lower;
}

void writeRegister16(uint16_t address, uint16_t val){
     uint8_t         txBuffer[4];
    // uint8_t         rxBuffer[4];

     txBuffer[0] = (address>> 8);
     txBuffer[1] = (address & 0xFF);
     txBuffer[0] = (val>> 8);
     txBuffer[1] = (val & 0xFF);
     i2cTransaction.slaveAddress = 0x3A;
     i2cTransaction.writeBuf = txBuffer; /* Buffer to be written */
     i2cTransaction.writeCount = 4;
     i2cTransaction.readBuf = NULL;
     i2cTransaction.readCount = 0;

     I2C_transfer(i2c, &i2cTransaction);

}

uint8_t getMode(){
    uint16_t mode;

    mode = EEPROM_Read16 (REG_CONTROL);
    mode = (mode >> 1) & 0x0003;
    return mode;
}
void setSOC(){
    uint16_t reg;

    reg = EEPROM_Read16(REG_CONTROL);
    reg |= (1 << 3);
    writeRegister16(REG_CONTROL,reg);
}

uint16_t getStatus(){
    uint16_t deviceStatus;

    deviceStatus = EEPROM_Read16(REG_STATUS);
    return deviceStatus;
}

void clearNewData()
{
  uint16_t reg = getStatus(); //Get current bits
  reg &= ~(1 << BIT_NEW_DATA); //Clear the bit
  writeRegister16(REG_STATUS, reg); //Set the mode bits
}
// Reads from 32 bit registers in the sensor
void all32 (){
    EE_PR = EEPROM_Read32(EE_P_R);
    EE_PG = EEPROM_Read32(EE_P_G);
    EE_PT = EEPROM_Read32(EE_P_T);
    EE_PO = EEPROM_Read32(EE_P_O);
    EEEa = EEPROM_Read32(EE_Ea);
    EEEb = EEPROM_Read32(EE_Eb);
    EEFa = EEPROM_Read32(EE_Fa);
    EEFb = EEPROM_Read32(EE_Fb);
    EEGa = EEPROM_Read32(EE_Ga);

}

// Reads from 16 bit registers in the sensor
void all16(){
        EEGb = EEPROM_Read16(EE_Gb);
        EEHa = EEPROM_Read16(EE_Ha);
        EEHb = EEPROM_Read16(EE_Hb);
        EEKa = EEPROM_Read16(EE_Ka);
}


// conversion to use in formula

void conversion(){
    P_R = (double)EE_PR * pow(2, -8);
    P_G = (double)EE_PG * pow(2, -20);
    P_T = (double)EE_PT * pow(2, -44);
    P_O = (double)EE_PO * pow(2, -8);
    Ea  = (double)EEEa * pow(2, -16);
   Eb = (double)EEEb * pow(2, -8);
   Fa = (double)EEFa * pow(2, -46);
   Fb = (double)EEFb * pow(2, -36);
   Ga = (double)EEGa * pow(2, -36);
   Gb = (double)EEGb * pow(2, -10);
   Ka = (double)EEKa * pow(2, -10);
   Ha = (double)EEHa * pow(2, -14);
  Hb = (double)EEHb * pow(2, -14);
}

uint8_t getCyclePosition(){

    uint16_t status = getStatus()>> BIT_CYCLE_POS;
    status &= 0x1F; //Get lower 5 bits.
    return (status);
}

float getObjTemp(){
    double TOdut = 25.0; //Assume 25C for first iteration
    double TO0 = 25.0; //object temp from previous calculation
    double TA0 = 25.0; //ambient temp from previous calculation
    if(getMode() != MODE_CONTINUOUS) setSOC();

    clearNewData();


    int16_t lowerRAM = 0;
    int16_t upperRAM = 0;

    int16_t sixRAM = EEPROM_Read16(RAM_6);
    //System_printf("%f",sixRAM);
    int16_t nineRAM = EEPROM_Read16(RAM_9);

    int cyclePosition = getCyclePosition();

    if (cyclePosition == 1 ){
        lowerRAM = EEPROM_Read16(RAM_4);
        upperRAM = EEPROM_Read16(RAM_5);
    }
    else if (cyclePosition == 2){
        lowerRAM = EEPROM_Read16(RAM_7);
        upperRAM = EEPROM_Read16(RAM_8);
    }
    uint8_t i ;
    for (i= 0 ; i < 3 ; i++){
        double VRta = nineRAM + Gb * (sixRAM / 12.0);

        double AMB = (sixRAM / 12.0) / VRta * pow(2, 19);

        double sensorTemp = P_O + (AMB - P_R) / P_G + P_T * pow((AMB - P_R), 2);

        float S = (float)(lowerRAM + upperRAM) / 2.0;
        double VRto = nineRAM + Ka * (sixRAM / 12.0);
        double Sto = (S / 12.0) / VRto * (double)pow(2, 19);

        double TAdut = (AMB - Eb) / Ea + 25.0;

        double ambientTempK = TAdut + 273.15;

        double bigFraction = Sto / (1 * Fa * Ha * (1 + Ga * (TOdut - TO0) + Fb * (TAdut - TA0)));

        double objectTemp = bigFraction + pow(ambientTempK, 4);
        objectTemp = pow(objectTemp, 0.25); //Take 4th root
        objectTemp = objectTemp - 273.15 - Hb;

        TO0 = objectTemp;
      }
    return TO0;
}


void i2c_setup(){
    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
//    i2cParams.transferMode = I2C_MODE_CALLBACK;
//    i2cParams.transferCallbackFxn = UserCallbackFxn;
    i2c = I2C_open(DK_TM4C129X_I2C2, &i2cParams);
    if (i2c == NULL) {
        System_abort("Error Initializing I2C\n");
    }
    else {
        System_printf("I2C Initialized!\n");
    }

    /* Point to the T ambient register and read its 2 bytes */

    txBuffer[0] = (EE_VERSION >> 8);
    txBuffer[1] = (EE_VERSION & 0xFF);
    i2cTransaction.slaveAddress = 0x3A;
    i2cTransaction.writeBuf = txBuffer; /* Buffer to be written */
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 2;

    /* Take 20 samples and print them out onto the console */
     I2C_transfer(i2c, &i2cTransaction);
     //System_printf("Sample %u: %d  and %d (C)\n", i,rxBuffer[0], rxBuffer[1]);

     all32();
     all16();
     conversion();
}

void TemperatureInit(Temp_Params * params)
{
    i2c_setup();
    memset(params, 0, sizeof(Temp_Params));
    params->upper_limit = 40;
}

bool isTemperatureWithinLimit(Temp_Params * params){
    bool withinLimit = false;

    if(params->avg <= params->upper_limit){
        withinLimit = true;
    }

    return withinLimit;
}

void SetTemperatureUpperLimit(Temp_Params * params, int16_t limit)
{
    params->upper_limit = limit;
}


void SetTemperatureAverage(Temp_Params * params)
{
    uint8_t index = 0;
    params->avg = 0;
    for (; index < (sizeof(params->sample)/sizeof(float)); index++){
        params->avg += params->sample[index];
    }
    params->avg = params->avg / (sizeof(params->sample)/sizeof(float));
}



