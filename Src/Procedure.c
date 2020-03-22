#include "Procedure.h"

RTimt_Typedef Time;

//uint8_t PacketTX[8];
//uint8_t PacketRX[8];
void RxExecute(void)
{
    nRF24L01_RxPacket(nRF24L01_RXBuffer);
    if(nRF24L01_RXBuffer[0]==TimeSync)
    {
    Time.year = 0x2000|nRF24L01_RXBuffer[1];
    Time.month = nRF24L01_RXBuffer[2];
    Time.date = nRF24L01_RXBuffer[3];
    Time.hour = nRF24L01_RXBuffer[4];
    Time.minute = nRF24L01_RXBuffer[5];
    Time.second = nRF24L01_RXBuffer[6];
    _RTime_Set(Time);
    }
    else
        nRF24L01_Flush_RX();
}


//void Procedure(void)
//{
//    
//    _RTime_Get(Time);
//    
//    nRF24L01_TXBuffer[0] = (Time.year)&0xff;
//    nRF24L01_TXBuffer[1] = Time.month;
//    nRF24L01_TXBuffer[2] = Time.date;
//    nRF24L01_TXBuffer[3] = Time.hour;
//    nRF24L01_TXBuffer[4] = Time.minute;
//    nRF24L01_TXBuffer[5] = Time.second;
//    nRF24L01_TXBuffer[6] = AA_LR6_Life();
//    nRF24L01_SetTX();
//    nRF24L01_Flush_TX();
//    nRF24L01_TxPacket(nRF24L01_TXBuffer,7);
//    while(!nRF24L01_TX_DS);
//    nRF24L01_SetRX();
//    
//    RTC_WakeUp_Reset();
//    
//    
//    HAL_SuspendTick();
//    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//    HAL_ResumeTick();
//    
//    if(nRG24L01_RX_DR)
//        RxExecute();  
//}
void Procedure(void)
{
    
    
    nRF24L01_TXBuffer[0] = 0;
    nRF24L01_TXBuffer[1] = 1;
    nRF24L01_TXBuffer[2] = 2;
    nRF24L01_TXBuffer[3] = 3;
    nRF24L01_TXBuffer[4] = 4;
    nRF24L01_TXBuffer[5] = 5;
    nRF24L01_TXBuffer[6] = 6;
    nRF24L01_SetTX();
    nRF24L01_Flush_TX();
    nRF24L01_TxPacket(nRF24L01_TXBuffer,7);
    while(!nRF24L01_TX_DS);
    nRF24L01_SetRX();
    
    HAL_Delay(2000);
    

    
    if(nRG24L01_RX_DR)
        RxExecute();  
}

