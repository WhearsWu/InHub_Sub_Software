#include "InHubS_BSP_RTC.h"

RTC_HandleTypeDef hrtc;

void RTC_Error_Handler(void)
{
    while(1);
}    
/* RTC init function */
void RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    RTC_Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    RTC_Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    RTC_Error_Handler();
  }
  /** Enable the WakeUp 
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 60, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
    RTC_Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
} 

void RTC_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_IRQn 0 */

  /* USER CODE END RTC_IRQn 0 */
  HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_IRQn 1 */

  /* USER CODE END RTC_IRQn 1 */
}

void RTC_CalendarConfig(uint16_t year,uint8_t month,uint8_t date,uint8_t hour,uint8_t minute,uint8_t second)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;


    sdatestructure.Year = year;
    sdatestructure.Month = month;
    sdatestructure.Date = date;
    //sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;

    if(HAL_RTC_SetDate(&hrtc,&sdatestructure,RTC_FORMAT_BCD
    ) != HAL_OK)
    {
        /* Error */
        RTC_Error_Handler(); 
    } 

    /*##-2- Configure the Time #################################################*/
    /* Set Time: 02:00:00 */
    stimestructure.Hours = hour;
    stimestructure.Minutes = minute;
    stimestructure.Seconds = second;
    stimestructure.TimeFormat = RTC_HOURFORMAT_24;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

    if(HAL_RTC_SetTime(&hrtc,&stimestructure,RTC_FORMAT_BCD
    ) != HAL_OK)
    {
        /* Error */
        RTC_Error_Handler(); 
    }

    /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);  
}\

void RTC_CalendarShow(uint16_t* year,uint8_t* month,uint8_t* date,uint8_t* hour,uint8_t* minute,uint8_t* second)
{
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;

    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BCD);
    *hour=stimestructureget.Hours ;
    *minute=stimestructureget.Minutes;
    *second=stimestructureget.Seconds;


    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BCD);
    *year = sdatestructureget.Year;
    *month = sdatestructureget.Month;
    *date= sdatestructureget.Date; 
    /* Display time Format : hh:mm:ss */
  
   
}
void RTime_Resource_Init(void)
{
    RTC_Init();
    
}
