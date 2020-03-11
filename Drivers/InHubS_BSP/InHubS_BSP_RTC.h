#ifndef INHUBS_BSP_RTC_H
#define INHUBS_BSP_RTC_H

#include "stm32l0xx_hal.h"


void RTC_CalendarConfig(uint16_t year,uint8_t month,uint8_t date,uint8_t hour,uint8_t minute,uint8_t second);
void RTC_CalendarShow(uint16_t* year,uint8_t* month,uint8_t* date,uint8_t* hour,uint8_t* minute,uint8_t* second);
void RTime_Resource_Init(void);

#endif  //INHUBS_BSP_RTC_H

