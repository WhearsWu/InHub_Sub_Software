#include "Procedure.h"

Packet_TypeDef PacketTX;
Packet_TypeDef PacketRX;

void Procedure(void)
{
    PacketTX.Battery = AA_LR6_Life();
    
    _RTime_Get(PacketTX.Time);
    
    nRF24L01_TxPacket(&(PacketTX.Time.year),8);
    
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    
//    switch (nRF24L01_State)
//    {
//        case RX_DR:
//            nRF24L01_RxPacket(&(PacketRX.Time.year));
//        break;
//        
//        case TX_DS:
//            
//        break;
//        
//        case MAX_RT:
//            
//        break;
//        
//        case RX_P_NO:
//            
//        break;
//        
//        case TX_FULL:
//            
//        break;
//   
//    }
    
}
