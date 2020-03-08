#include "nRF24L01.h"

uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址



uint8_t nRF24L01_State;
uint8_t ignore;


void nRF24L01_SendCammand(uint8_t cmd)
{
    _SPI_ReadWrite(&cmd,&nRF24L01_State);
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
//函数：uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
//功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uint8_ts：写入数据的个数
//********************************************************************************************************/
void nRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();            //SPI使能       
    
	nRF24L01_SendCammand(NRF24L01_WRITE_REG + reg);

    nRF24L01_Muti_Send(pBuf, uint8_ts);


	nRF24L01_CSN_HIGH();           //关闭SPI
}
//***************************************************************************************************/
//函数：uint8_t _SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
//功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uint8_ts：读出数据的个数
//***************************************************************************************************/
void nRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
    
	nRF24L01_SendCammand(NRF24L01_READ_REG + reg);      		// Select register to write to and read status uint8_t

    nRF24L01_Muti_Get( pBuf,uint8_ts);
    
	nRF24L01_CSN_HIGH();                           
	
}

//***************************************************************************************************/
//功能：NRF24L01读写寄存器函数
//***************************************************************************************************/
void nRF24L01_Write_Reg(uint8_t reg, uint8_t value)
{
    nRF24L01_Write_Buf(reg, &value, 1);
}


uint8_t nRF24L01_Read_Reg(uint8_t reg)
{    
    uint8_t read;
    nRF24L01_Read_Buf(reg, &read, 1);
    return read;
}


void nRF24L01_Updata_State()
{
    nRF24L01_CSN_LOW();
    nRF24L01_SendCammand(NOP);
    nRF24L01_CSN_HIGH();
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
    
    nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
	nRF24L01_SendCammand(WR_TX_PLOAD);      		// Select register to write to and read status uint8_t

	nRF24L01_Muti_Send( pBuf,NoB);
    
    nRF24L01_Write_Reg(CONFIG, 0x0e);
    
	nRF24L01_CSN_HIGH();
    _Delay(1);
}





///***************************************************************************************
//NRF24L01初始化
///**************************************************************************************/
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



//***************************************************************************************************/
//函数：void SetRX_Mode(void)
//功能：数据接收配置 
//***************************************************************************************************/
void nRF24L01_SetRX(void)
{
	nRF24L01_CE_LOW();
	nRF24L01_Write_Reg(CONFIG, 0x0f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	nRF24L01_CE_HIGH(); 
	_Delay(4);
}

//*****************************************************************************************************/
//函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//功能：数据读取后放如rx_buf接收缓冲区中
//*****************************************************************************************************/
//unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//{
//    unsigned char revale=0;

//	//nRF24L01_State = nRF24L01_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
////	if(nRF24L01_State|RX_DR)				// 判断是否接收到数据
////	{
//	    nRF24L01_CE_LOW(); 			//SPI使能
//    
//    
//		nRF24L01_Read_Buf(rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
//    
//    
//		revale =1;			//读取数据完成标志
////	}
////	nRF24L01_Write_Reg(NRF24L01_WRITE_REG+STATUS,nRF24L01_State);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
//	return revale;
//}
//**********************************************************************************************************
//函数：void nRF24L01_TxPacket(unsigned char * tx_buf)
//功能：发送 tx_buf中数据
//*********************************************************************************************************/
//void nRF24L01_TxPacket(unsigned char * tx_buf)
//{
//	nRF24L01_CE_LOW();			//StandBy I模式	
//	nRF24L01_Write_Buf(RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
//    
//    
//	nRF24L01_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
//    
//    
//    
//	nRF24L01_Write_Reg(CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
//	nRF24L01_CE_HIGH();		 //置高CE，激发数据发送
//	//_Delay(1);
//}



void EXTI0_1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(nRF24L01_IRQ_Pin);
    //nRF24L01_Updata_State();
    nRF24L01_SetRX();
    nRF24L01_Write_Reg(STATUS,nRF24L01_State); 
}
