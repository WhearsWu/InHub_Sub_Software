#ifndef _PROCEDURE_H
#define _PROCEDURE_H

#include "nRF24L01.h"
#include "AA_LR6.h"
#include "RTime.h"


typedef struct
{
    RTimt_Typedef Time;
    uint8_t Battery;
} Packet_TypeDef;


void Procedure(void);

#endif //_PROCEDURE_H

