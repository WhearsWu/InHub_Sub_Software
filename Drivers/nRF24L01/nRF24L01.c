#include "nRF24L01.h"

uint8_t TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
uint8_t RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ



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
//������uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
//����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uint8_ts��д�����ݵĸ���
//********************************************************************************************************/
void nRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();            //SPIʹ��       
    
	nRF24L01_SendCammand(NRF24L01_WRITE_REG + reg);

    nRF24L01_Muti_Send(pBuf, uint8_ts);


	nRF24L01_CSN_HIGH();           //�ر�SPI
}
//***************************************************************************************************/
//������uint8_t _SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
//����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uint8_ts���������ݵĸ���
//***************************************************************************************************/
void nRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts)
{
	nRF24L01_CSN_LOW();                    		// Set CSN low, init SPI tranaction
    
	nRF24L01_SendCammand(NRF24L01_READ_REG + reg);      		// Select register to write to and read status uint8_t

    nRF24L01_Muti_Get( pBuf,uint8_ts);
    
	nRF24L01_CSN_HIGH();                           
	
}

//***************************************************************************************************/
//���ܣ�NRF24L01��д�Ĵ�������
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
//NRF24L01��ʼ��
///**************************************************************************************/
void nRF24L01_Init(void)
{
    _Delay(3);
 	nRF24L01_CE_LOW();    // chip enable
 	nRF24L01_CSN_HIGH();   // Spi  disable 
	nRF24L01_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	nRF24L01_Write_Buf(RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
    //nRF24L01_Write_Reg(CONFIG, 0x08);
	nRF24L01_Write_Reg(EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	nRF24L01_Write_Reg(EN_RXADDR, 0x01);  //  �������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	nRF24L01_Write_Reg(RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	nRF24L01_Write_Reg(RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	nRF24L01_Write_Reg(RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0d    
}



//***************************************************************************************************/
//������void SetRX_Mode(void)
//���ܣ����ݽ������� 
//***************************************************************************************************/
void nRF24L01_SetRX(void)
{
	nRF24L01_CE_LOW();
	nRF24L01_Write_Reg(CONFIG, 0x0f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������
	nRF24L01_CE_HIGH(); 
	_Delay(4);
}

//*****************************************************************************************************/
//������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//*****************************************************************************************************/
//unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//{
//    unsigned char revale=0;

//	//nRF24L01_State = nRF24L01_Read_Reg(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
////	if(nRF24L01_State|RX_DR)				// �ж��Ƿ���յ�����
////	{
//	    nRF24L01_CE_LOW(); 			//SPIʹ��
//    
//    
//		nRF24L01_Read_Buf(rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
//    
//    
//		revale =1;			//��ȡ������ɱ�־
////	}
////	nRF24L01_Write_Reg(NRF24L01_WRITE_REG+STATUS,nRF24L01_State);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
//	return revale;
//}
//**********************************************************************************************************
//������void nRF24L01_TxPacket(unsigned char * tx_buf)
//���ܣ����� tx_buf������
//*********************************************************************************************************/
//void nRF24L01_TxPacket(unsigned char * tx_buf)
//{
//	nRF24L01_CE_LOW();			//StandBy Iģʽ	
//	nRF24L01_Write_Buf(RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
//    
//    
//	nRF24L01_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
//    
//    
//    
//	nRF24L01_Write_Reg(CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
//	nRF24L01_CE_HIGH();		 //�ø�CE���������ݷ���
//	//_Delay(1);
//}



void EXTI0_1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(nRF24L01_IRQ_Pin);
    //nRF24L01_Updata_State();
    nRF24L01_SetRX();
    nRF24L01_Write_Reg(STATUS,nRF24L01_State); 
}