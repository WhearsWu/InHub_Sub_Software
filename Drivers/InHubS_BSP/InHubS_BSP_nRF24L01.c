#include "InHubS_BSP_nRF24L01.h"



static SPI_HandleTypeDef SpiHandle;
static void nRF24L01_SPIx_Error (void);

/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* SPI Routines *********************************/
/**
  * @brief  SPIx Bus initialization
  * @param  None
  * @retval None
  */
void nRF24L01_SPIx_Init(void)
{
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {

      SpiHandle.Instance = SPI1;
      SpiHandle.Init.Mode = SPI_MODE_MASTER;
      SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
      SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
      SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
      SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
      SpiHandle.Init.NSS = SPI_NSS_SOFT;
      SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
      SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
      SpiHandle.Init.TIMode = SPI_TIMODE_DISABLE;
      SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
      SpiHandle.Init.CRCPolynomial = 7;
      if (HAL_SPI_Init(&SpiHandle) != HAL_OK)
      {
        nRF24L01_SPIx_Error();
      }
  }
}

/**
  * @brief  SPI Read 4 bytes from device.
  * @param  ReadSize Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */
uint8_t nRF24L01_SPIx_ReadWrite(uint8_t* TValue,uint8_t* RValue,uint8_t NOB)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_SPI_TransmitReceive(&SpiHandle,  TValue, RValue, NOB, SPIx_TIMEOUT_MAX);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the BUS */
    nRF24L01_SPIx_Error();
      return 0x0;
  }

  return 0x01;
}








/**
  * @brief  SPI Write a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
//static void SPIx_Write(uint8_t Value)
//{
//  HAL_StatusTypeDef status = HAL_OK;

//  status = HAL_SPI_Transmit(&SpiHandle, (uint8_t*) &Value, 1, SPIx_TIMEOUT_MAX);

//  /* Check the communication status */
//  if(status != HAL_OK)
//  {
//    /* Re-Initiaize the BUS */
//    nRF24L01_SPIx_Error();
//  }
//}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void nRF24L01_SPIx_Error (void)
{
  /* De-Initialize the SPI comunication BUS */
  HAL_SPI_DeInit(&SpiHandle);

  /* Re-Initiaize the SPI comunication BUS */
  nRF24L01_SPIx_Init();
}


void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
} 


void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}



void nRF24L01_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, Switch_Pin|nRF24L01_CE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(nRF24L01_CSN_GPIO_Port, nRF24L01_CSN_Pin, GPIO_PIN_SET);

//  /*Configure GPIO pin : PC15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//  /*Configure GPIO pins : PAPin PAPin */
//  GPIO_InitStruct.Pin = Detect_Pin|DS18B20_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//  /*Configure GPIO pin : PtPin */
//  GPIO_InitStruct.Pin = Switch_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(Switch_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = nRF24L01_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(nRF24L01_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = nRF24L01_CE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(nRF24L01_CE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = nRF24L01_CSN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(nRF24L01_CSN_GPIO_Port, &GPIO_InitStruct);

}
void nRF24L01_EXTI_Init(void)
{
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}




void nRF24L01_Resource_Init(void)
{
    nRF24L01_SPIx_Init();
    nRF24L01_GPIO_Init();
    nRF24L01_EXTI_Init();
}




//void nRF24L01_Delay (uint32_t Delay)
//{
//  HAL_Delay (Delay);
//}
