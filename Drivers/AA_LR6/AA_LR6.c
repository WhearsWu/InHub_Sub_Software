#include "AA_LR6.h"

uint8_t AA_LR6_Life(void)
{
    uint16_t votage;
    uint8_t per;
    votage = _AA_LR6_Votage();
    per = (votage - MinVotage)/(MaxVotage - MinVotage)*100;
    return per;
}

