#ifndef _AA_LR6_H
#define _AA_LR6_H

#include "InHubS_BSP_Battery.h"

#define serial              1
#define MaxVotage           1500*serial
#define MinVotage           800*serial

#define _AA_LR6_Votage()    Battrey_mV()



uint8_t AA_LR6_Life(void);

#endif //_AA_LR6_H

