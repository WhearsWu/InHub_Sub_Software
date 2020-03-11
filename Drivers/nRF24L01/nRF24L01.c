#include "nRF24L01.h"

uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
uint8_t nRF24L01_RXBuffer[RX_PLOAD_WIDTH];
uint8_t nRF24L01_TXBuffer[RX_PLOAD_WIDTH];

uint8_t nRF24L01_State;
uint8_t ignore;

//*****************************************************************************************************
void nRF24L01_SendCammand(uint8_t cmd)
{
    _SPI_ReadWrite(&cmd,&nRF24L01_State);
}

void nRF24L01_Updata_State()
{
    nRF24L01_CSN_LOW();
    nRF24L01_SendCammand(NOP);
    nRF24L01_CSN_HIGH();
}

void nRF24L01_SendValue(uint8_t* val)
{
    _SPI_ReadWrite(val,&ignore);
}
void nRF24L01_GetdValue(uint8_t* buf)
{
    static uint8_t nop=NOP;
    
    _SPI_ReadWrite(&nop,buf);

}
void nRF24L01_Muti_Send(uint8_t* pBuf, uint8_t times)   
{
    uint8_t Send_counter;
    for(Send_counter=0; Send_counter!=times; Send_counter++) //
		nRF24L01_SendValue(pBuf+Send_counter);
}
void nRF24L01_Muti_Get(uint8_t* pBuf, uint8_t times)   
{
    uint8_t Get_counter;
    for(Get_counter=0; Get_counter!=times; Get_counter++) //
		nRF24L01_GetdValue(pBuf+Get_counter);
}

//********************************************************************************************************

void nRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();            //SPI使能       
    
	nRF24L01_SendCammand(NRF24L01_WRITE_REG + reg);

    nRF24L01_Muti_Send(pBuf, uint8_ts);


	nRF24L01_CSN_HIGH();           //关闭SPI
    nRF24L01_Updata_State();
}

void nRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
    
	nRF24L01_SendCammand(NRF24L01_READ_REG + reg);      		// Select register to write to and read status uint8_t

    nRF24L01_Muti_Get( pBuf,uint8_ts);
    
	nRF24L01_CSN_HIGH();                           
	nRF24L01_Updata_State();
}


void nRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    //nRF24L01_Write_Buf(reg, &value, 1);
    nRF24L01_CSN_LOW();            //SPI使能       
    
	nRF24L01_SendCammand(NRF24L01_WRITE_REG + reg);

    nRF24L01_SendValue(&value);


	nRF24L01_CSN_HIGH();           //关闭SPI
    nRF24L01_Updata_State();
    
}


uint8_t nRF24L01_Read_Reg(uint8_t reg)
{    
    uint8_t read;
    //nRF24L01_Read_Buf(reg, &read, 1);
    nRF24L01_CSN_LOW();            //SPI使能       
    
	nRF24L01_SendCammand(NRF24L01_WRITE_REG + reg);

    nRF24L01_GetdValue(&read);

	nRF24L01_CSN_HIGH();           //关闭SPI
    
    nRF24L01_Updata_State();
    
    return read;
}



void nRF24L01_RxPacket(uint8_t *pBuf)
{
    uint8_t NoB;
    NoB = nRF24L01_Read_Reg(RX_PW_P0);
    
    nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
	nRF24L01_SendCammand(RD_RX_PLOAD);      		// Select register to write to and read status uint8_t

	nRF24L01_Muti_Get( pBuf,NoB);
    
	nRF24L01_CSN_HIGH();
}

void nRF24L01_TxPacket(uint8_t *pBuf,uint8_t NoB)
{
//    uint8_t RegVal;
    
    nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
	nRF24L01_SendCammand(WR_TX_PLOAD);      		// Select register to write to and read status uint8_t

	nRF24L01_Muti_Send( pBuf,NoB);
        
	nRF24L01_CSN_HIGH();
    
    //RegVal = nRF24L01_Read_Reg(CONFIG);
    
    nRF24L01_Write_Reg(CONFIG, 0x0e);
    
    nRF24L01_CE_HIGH();    
    _Delay(1);
    nRF24L01_CE_LOW();
}

void nRF24L01_SetRX(void)
{
	nRF24L01_CE_LOW();
	nRF24L01_Write_Reg(CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	//nRF24L01_CE_HIGH(); 
	_Delay(1);
}

void nRF24L01_Flush_TX(void)
{
    nRF24L01_CSN_LOW();
    nRF24L01_SendCammand(FLUSH_TX);
    nRF24L01_CSN_HIGH();
}

//********************************************************************************************************

void nRF24L01_Init(void)
{
    _Delay(3);
 	nRF24L01_CE_LOW();    // chip enable
 	nRF24L01_CSN_HIGH();   // Spi  disable 
	nRF24L01_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	nRF24L01_Write_Buf(RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
    //nRF24L01_Write_Reg(CONFIG, 0x08);
	nRF24L01_Write_Reg(EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	nRF24L01_Write_Reg(EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	nRF24L01_Write_Reg(RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	nRF24L01_Write_Reg(RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	nRF24L01_Write_Reg(RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0d    
}

void _nRF24L01_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(nRF24L01_IRQ_Pin);
    nRF24L01_Updata_State();
    switch (nRF24L01_State&(~RX_P_NO))
    {
        case RX_DR:
            nRF24L01_RxPacket(nRF24L01_RXBuffer);
            
        break;
        
        case TX_DS:
            
        break;
        
        case MAX_RT:
            
        break;
        
        
        case TX_FULL:
            nRF24L01_Flush_TX();
        break;
   
    }
    
    nRF24L01_Write_Reg(STATUS,nRF24L01_State&(~RX_P_NO)); 
}
