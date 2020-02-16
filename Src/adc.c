/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "tkConfig.h"
#include "string.h"
#include "vkDeclarations.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/*
 * https://blog.radiotech.kz/perevod/adc-perevod-iz-knigi-mastering-stm32/
 *
Tconv = Tsmt + 12,5 циклов,
 где Tsmt – это программно заданное время выборки.
 Например, частота ADCCLK задана равной 12MHz
 Если время выборки – 1,5 цикла.
 Тогда Tconv = =1,5 + 12,5 = 14 циклов * 1/12MHz= 1.17us.
 Якщо час вибірки 239.5циків, тоді
 Tconv=239.5+12.5=252 цикли  / 12MHz = 21us. Тобто 14 каналів може бути опитано за 293uс
 Tconv=71.5+12.5=84 цикли  / 12MHz = 7us. Тобто 14 каналів може бути опитано за 98uс
 Tconv=55.5+12.5=68 цикли  / 12MHz = 5.7us. Тобто 14 каналів може бути опитано за 80uс
// Якщо залишаємо кількість відліків АЦП без змін - кожні 100us (200 відліків за 20мс для 50Гц),
 то обгрунтована тривалість циклів - 55.5

 Якщо кількість відліків - кожні 200us, тобто 100 відліків для періоду струму/напруги,
 то можна перейти на 71.5.
*/
/* ADC1 init function */
void MX_ADC1_Init(void){

  ADC_ChannelConfTypeDef sConfig = {0};
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  memset(&sConfig,0,sizeof(sConfig));

  hadc1.Init.NbrOfConversion = 13;
  if (HAL_ADC_Init(&hadc1) != HAL_OK){
	Error_Handler();
  }
	  /**Configure Regular Channel
   	   струм iA
   	   PA0-WKUP------> ADC1_IN0	1 		Струм Фаза 1  */

	  sConfig.Channel = ADC_CHANNEL_0;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		  Error_Handler();
	  }
	  /**Configure Regular Channel
  	  струм іВ
  	  PA1     ------> ADC1_IN1	2		Струм Фаза 2*/
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_2;
	  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		  Error_Handler();
	  }
	  /**Configure Regular Channel
   	   струм іС-
	   PA2     ------> ADC1_IN2	3		Струм Фаза 3   	   */
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = ADC_REGULAR_RANK_3;
	  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		  Error_Handler();
	  }
	  /**Configure Regular Channel
  	  Voltage!!!!
  	  PA5     ------> ADC1_IN5	4        Напруга, Фаза 3*/
	  sConfig.Channel = ADC_CHANNEL_5;
	  sConfig.Rank = ADC_REGULAR_RANK_4;
	  sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK){
		  Error_Handler();
	  }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13*/
/***************************************/
/*    PA0-WKUP------> ADC1_IN0   iA    */
/*    PA1     ------> ADC1_IN1 	 iB	   */
/*    PA2     ------> ADC1_IN2 	 iC	   */
/*    								   */	/*PA3     ------> ADC1_IN3*/
/*    								   */	/*PA4     ------> ADC1_IN4*/
/*    PA5     ------> ADC1_IN5   uC    */
/***************************************/
/*
    PA6     ------> ADC1_IN6
    PC4     ------> ADC1_IN14
    PC5     ------> ADC1_IN15
    PB0     ------> ADC1_IN8
    PB1     ------> ADC1_IN9 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_NORMAL;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC0     ------> ADC1_IN10       Високий рівень
    PC1     ------> ADC1_IN11		Низький рівень
    PC2     ------> ADC1_IN12		Сухий хід
    PC3     ------> ADC1_IN13		Аналоговий датчик тиску
    PA0-WKUP------> ADC1_IN0 		Струм Фаза 1
    PA1     ------> ADC1_IN1		Струм Фаза 2
    PA2     ------> ADC1_IN2		Струм Фаза 3
    PA3     ------> ADC1_IN3		Струм витоку
    PA4     ------> ADC1_IN4        -
    PA5     ------> ADC1_IN5        Напруга, Фаза 3
    PA6     ------> ADC1_IN6        Ізоляція

    PC4     ------> ADC1_IN14       -
    PC5     ------> ADC1_IN15       -
    PB0     ------> ADC1_IN8        -
    PB1     ------> ADC1_IN9   		Кнопки LCD
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
} 






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
