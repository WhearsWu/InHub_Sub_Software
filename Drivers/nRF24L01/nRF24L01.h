#ifndef _NRF24L01_H
#define _NRF24L01_H

#include "stdint.h"

/*******************************Configure *********************************/
#include "stm32l0xx_hal.h"
#include "InHubS_BSP_nRF24L01.h"

#define         _Delay(s)                       HAL_Delay(s)
#define         _SPI_ReadWrite(Tchar,Rchar)       nRF24L01_SPIx_ReadWrite((uint8_t*)Tchar,(uint8_t*)Rchar)
//#define         _SPI_Read()                   SPIx_Read()
#define         nRF24L01_CSN_LOW()              HAL_GPIO_WritePin(nRF24L01_CSN_GPIO_Port, nRF24L01_CSN_Pin, GPIO_PIN_RESET)
#define         nRF24L01_CSN_HIGH()             HAL_GPIO_WritePin(nRF24L01_CSN_GPIO_Port, nRF24L01_CSN_Pin, GPIO_PIN_SET)
#define         nRF24L01_CE_LOW()               HAL_GPIO_WritePin(nRF24L01_CE_GPIO_Port, nRF24L01_CE_Pin, GPIO_PIN_RESET)
#define         nRF24L01_CE_HIGH()              HAL_GPIO_WritePin(nRF24L01_CE_GPIO_Port, nRF24L01_CE_Pin, GPIO_PIN_SET)

#define         _nRF24L01_IRQHandler              EXTI0_1_IRQHandler
/******************************* Configure *********************************/


#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  20  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  20  	// 20 uints TX payload









//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define NRF24L01_READ_REG        0x00  	// ���Ĵ���ָ��
#define NRF24L01_WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// 
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
    #define PRIM_RX     0x01
    #define PWR_UP      0x02
    #define CRCO        0x04
    #define EN_CRC      0x08
    #define MASK_MAX_RT 0x10
    #define MASK_TX_DS  0x20
    #define MASK_RX_DR  0x40
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
    #define TX_FULL     0x01
    #define RX_P_NO     0x0e
    #define MAX_RT      0x10
    #define TX_DS       0x20
    #define RX_DR       0x40
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//**************************************************************************************
#define nRF24L01_TX_DS                (nRF24L01_State&TX_DS)
#define nRG24L01_RX_DR                (nRF24L01_State&RX_DR)
//typedef struct
//{
//    uint8_t TX_FULL;
//    uint8_t RX_P_NO;
//    uint8_t MAX_RT;
//    uint8_t TX_DS;
//    uint8_t RX_DR;
//    
//} nRF24L01_StatusTypDef;



extern uint8_t nRF24L01_State;
extern uint8_t nRF24L01_RXBuffer[RX_PLOAD_WIDTH];
extern uint8_t nRF24L01_TXBuffer[RX_PLOAD_WIDTH];


void nRF24L01_Init(void);

void nRF24L01_SetRX(void);
void nRF24L01_SetTX(void);
void nRF24L01_RxPacket(uint8_t *pBuf);
void nRF24L01_TxPacket(uint8_t *pBuf,uint8_t NoB);
void nRF24L01_Flush_TX(void);
void nRF24L01_Flush_RX(void);




#endif /*_NRF24L01_H*/
