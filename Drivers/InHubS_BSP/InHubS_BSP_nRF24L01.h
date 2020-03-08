#ifndef _INHUB_BSP_NRF24L01_H
#define	_INHUB_BSP_NRF24L01_H
#include "stm32l0xx_hal.h"

#define SPIx_TIMEOUT_MAX              ((uint32_t)0x1000)




//#define Detect_Pin GPIO_PIN_2
//#define Detect_GPIO_Port GPIOA

//#define Switch_Pin GPIO_PIN_3
//#define Switch_GPIO_Port GPIOA

//#define DS18B20_Pin GPIO_PIN_4
//#define DS18B20_GPIO_Port GPIOA

#define nRF24L01_IRQ_Pin GPIO_PIN_1
#define nRF24L01_IRQ_GPIO_Port GPIOB

#define nRF24L01_CE_Pin GPIO_PIN_9
#define nRF24L01_CE_GPIO_Port GPIOA

#define nRF24L01_CSN_Pin GPIO_PIN_10
#define nRF24L01_CSN_GPIO_Port GPIOA


//void                    nRF24L01_SPIx_Init(void);
uint8_t                   nRF24L01_SPIx_ReadWrite(uint8_t* TValue,uint8_t* RValue);
//static uint32_t          SPIx_Read(void);
void                      nRF24L01_Resource_Init(void);
//void                    nRF24L01_GPIO_Init(void);
#endif /*_INHUB_BSP_NRF24L01_H*/
