#ifndef _RTIME_H
#define _RTIME_H

#include "InHubS_BSP_RTC.h"

#define _RTime_Set(T)    RTC_CalendarConfig(T.year,T.month,T.date,T.hour,T.minute,T.second)
#define _RTime_Get(T)   RTC_CalendarShow(&(T.year),&(T.month),&(T.date),&(T.hour),&(T.minute),&(T.second))



typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t date;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    
} RTimt_Typedef;



#endif //_RTIME_H

