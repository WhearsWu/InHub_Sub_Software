#ifndef _INHUBS_BSP_BATTERY_H
#define _INHUBS_BSP_BATTERY_H

#include "stm32l0xx_hal.h"

#define VDD_APPLI                      ((uint32_t)3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_12BITS                   ((uint32_t)4095)    /* Max value with a full range of 12 bits */

#define COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( (ADC_DATA) * VDD_APPLI / RANGE_12BITS)

uint16_t Battrey_mV(void);
void Battrey_Resource_Init(void);


#endif //_INHUBS_BSP_BATTERY_H
