/* --------------------------------------
 *
 * @File:       current.c
 * @Author:     Abdulaziz Alahmadi
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#include "current.h"

void CurrentInit(Current_Params * params){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    //enable ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //configure number of samples to be averaged
    ADCHardwareOversampleConfigure(ADC0_BASE, 1);
    //configure ADC sequencer - use ADC 0, sample sequencer 0, want the processor to trigger sequence, use highest priority
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    //final sequencer step: need to sample current sensor(ADC_CTL_CH0), configure interrupt flag(ADC_CTL_IE) to be set when sample is done
    //tell ADC logic that this is the last conversion on sequencer (ADC_CTL_END)
    ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,0);
    IntMasterEnable();
    ADCIntClear(ADC0_BASE, 0);
    ADCIntEnable(ADC0_BASE,0);
    memset(params, 0, sizeof(Current_Params));
}

void SetCurrentUpperLimit(Current_Params * params, int16_t limit){
    params->upper_limit = limit;
}

void SetCurrentAverage(Current_Params * params){
    uint8_t index = 0;
    for (; index < (sizeof(params->currentsensor.sample)/sizeof(uint32_t)); index++){
        params->currentsensor.avg += params->currentsensor.sample[index];
    }
    params->currentsensor.avg = params->currentsensor.avg / (sizeof(params->currentsensor.sample)/sizeof(uint32_t));
    params->current = (uint32_t)(((((((float)params->currentsensor.avg )/ MAXIMUM_BYTES_SIZE)*VOLTAGE)-REFERENCE_VOLTAGE)/VOLTAGE_OFFSET)*1000.0) ;
}

bool isCurrentWithinLimit(Current_Params * params){
    bool withinLimit = false;

    if(params->current <= params->upper_limit){
        withinLimit = true;
    }

    return withinLimit;
}

void initInterfacePower(Power * power, uint8_t sample){
    uint8_t i = 0;
    power->value_full = 0;
    power->value_index = 0;
    power->start_index = 0;

    for (;i<sample;i++)
    {
        power->data[i] = 0;
    }
}

void setInterfacePower(Power * power, int16_t newpower){
    power->data[power->value_index] = newpower;
    power->value_index += 1;
    if (power->value_index >= 100)
    {
        power->value_full += 1;
        power->value_index = 0;
    }
    if (power->value_full)
    {
        power->start_index += 1;
        if (power->start_index >= 100)
             power->start_index = 0;
    }
}
