/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <tk1_grid.h>
#include <vkButtonsADC.h>
#include <vv300set_factory.h>
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tkConfig.h"
#include "vkDeclarations.h"
#include "../3_Libraries/tkSGL.h"
//"../1_PROJECT/tkConfig.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 /* USER CODE BEGIN 0 */
#ifndef FURIE_TEST

/* Цей масив містить зглажені значення
 * вимірювань входів АЦП
 * Індекс АЦП
    0	 4 кнопка РЕВЕРС,
    1    6-ізоляція,
	2    7-кнопка ПУСК,
	3    8-кнопка STOP,
	4  	10-датчик високого рівень,
	5  	11-датчик низького рівня,
	6  	12-датчик сухого ходу ,
	7 	13-аналоговий датчик,
	8  	14-датчик вологості в насосц/двигуні,
	9  	15-реле температури в насосі/на двигуні*/


//extern uint16_t SensorsAndButtonsADC[10];
//extern void MX_ADC1_Init(void);
/* Прапор, по якому системний таймер повинен запустити передачу відповіді Модбас,
 * щодо дотриматися перервви віж прийомом і передачею 3.5 симв.*/
extern uint32_t EndSysTickDelay;
extern MB_Slave_Transmit(void);
uint32_t tim2_counter =0;
uint32_t tim7_Tick=0;
volatile uint32_t tim7_18_duration =0;
SysTick_afterInit_t SysTick_afterInit = Init_SysTick;
/* Ідентифікатури типу вимірювань АЦП */
enum {
	for_Nothing,
	/* Для перетворення Фурьє, три струми фаз, струм витоку та напруга */
	for_Furie_18,
	/* Протягом одного періоду 20 мс з інтервалом 100 мікросекунд,
	 *  три струми фаз, струм витоку та напруга */
	for_Classic_240,
	/* Для кнопок, через кожні приблизно 5 мсек
	 * для аналогових датчиків - досить разу за період */
	for_slow_BTNs_and_sensors_plus
} adc_meas=for_Nothing;
/* Лічильник завершення вимірювань напруги та струму,
 * передається для скидання в основний цикл tk_Main.c  */
uint32_t MesETCntr = 0;
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint32_t tim6_Cntr=0;
uint32_t tim7_Cntr=0;
volatile uint32_t SysTick_Cntr=0;
uint32_t timClick=0;

volatile uint32_t timDuration =0;
static uint16_t i_conv240 =0;
static uint16_t i_convFurie =0;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

/* USER CODE BEGIN EV */
#ifdef GRD_FURIE
extern volatile adcf_t uFurieC[N_FURIE];
extern volatile adcf_t iFurieA[N_FURIE];
extern volatile adcf_t iFurieB[N_FURIE];
extern volatile adcf_t iFurieC[N_FURIE];



extern volatile adcu16_t iA[240];
extern volatile adcu16_t iB[240];
extern volatile adcu16_t iC[240];
extern volatile adcf_t   uC[240];
extern fSet_t fSet;
//extern uint32_t sbTimeConstants[10];
/* ДМА збирає значення в цей масивs adcGRDValue
							RANK 	Index у масиві
PA0-WKUP------> ADC1_IN0	1		0	Струм Фаза 1
PA1     ------> ADC1_IN1	2		1	Струм Фаза 2
PA2     ------> ADC1_IN2	3		2	Струм Фаза 3
PA3     ------> ADC1_IN3	4		3	Струм витоку
PA5     ------> ADC1_IN5	5       4	Напруга, Фаза 3
PA6     ------> ADC1_IN6	6       5	Ізоляція
PB1     ------> ADC1_IN9	7   	6	Кнопки LCD
PC0     ------> ADC1_IN10	8       7	Високий рівень
PC1     ------> ADC1_IN11	9		8	Низький рівень
PC2     ------> ADC1_IN12	10		9	Сухий хіди
PC3     ------> ADC1_IN13	11		10	Аналоговий датчик тиску
PC4     ------> ADC1_IN14   12		11	Датчик вологості
PC5     ------> ADC1_IN15	13		12	Датчик температури*/
static volatile uint16_t adcGRDValue[4]={0};

#endif //GRD_FURIE
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();

  /* USER CODE BEGIN SysTick_IRQn 1 */
    SysTick_Cntr++;
    if(EndSysTickDelay==SysTick_Cntr){
    	MB_Slave_Transmit();
    }
#ifndef FURIE_TEST


  if (SysTick_afterInit==Loop_SysTick){
	  uint32_t cntr100 = SysTick_Cntr % 100;
	  if (cntr100 == 0){
		  /* Включаємо переривання з розрахунку
		   * 18 переривань на один період напруги мережі*/
		  HAL_TIM_Base_Start_IT(&htim7);
	  }
	  else if(cntr100==25){
		  /* Включаємо переривання через кожні 100мкс*/
		  HAL_TIM_Base_Start_IT(&htim6);
		  /* По кожному з цих переривань запускається АЦП в режим DMA */
	  }
	  else{}
  }
#endif //FURIE_TEST
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
#ifdef GRD_FURIE
  HAL_DMA_IRQHandler(&hdma_adc1);
#endif //GRD_FURIE
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  //https://community.st.com/s/question/0D50X00009XkeH9SAJ/haladcstartdma-acquire-only-a-subset-of-channels
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */

  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt.
  */
void TIM6_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_IRQn 0 */
#ifndef FURIE_TEST
  /* ТІМ6 - це перивання через кожні 100 мікросекунд(не мілі, а мікро),
   * з розрахунку 200 відліків напрурги і струму на період
   * результати складаються в масив adcGRDValue звідки забираються
   * в функції HAL_ADC_ConvCpltCallback, див.нижче                */
  adc_meas=for_Classic_240;
  HAL_ADC_Start_DMA( &hadc1, (uint32_t*)adcGRDValue, 4);
#endif //FURIE_TEST
  /* USER CODE END TIM6_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_IRQn 1 */


  /* USER CODE END TIM6_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
#ifndef FURIE_TEST
/* Частота ТІМ7 з автопідлаштуванням - незалежно від частоти струму
 * завжди точно 18 відліків за період
 * Кожен раз по перериванню запускається АЦП для
 * вимірювання миттєвих напруги і струмів
   результати складаються в масив adcGRDValue звідки забираються
   в функції HAL_ADC_ConvCpltCallback, див.нижче                */
//  if (t7_Cnt == 0) {t7_Tick=HAL_GetTick();}
  adc_meas = for_Furie_18;
  HAL_ADC_Start_DMA( &hadc1, (uint32_t*)adcGRDValue, 4);
#endif //FURIE_TEST
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/*
ADC conversion with transfer by DMA:
Activate the ADC peripheral and start conversions using function
HAL_ADC_Start_DMA()
Wait for ADC conversion completion by call of function
HAL_ADC_ConvCpltCallback() or HAL_ADC_ConvHalfCpltCallback() (these
functions must be implemented in user program)
Conversion results are automatically transferred by DMA into destination
variable address.
Stop conversion and disable the ADC peripheral using function
HAL_ADC_Stop_DMA()*/



uint8_t iuii=0;
uint16_t x1[18]={0};
uint16_t x2[18]={0};

uint32_t it_adc_buttons_pool =0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
	/* ДМА збирає значення в цей масивs adcGRDValue
								RANK 	Index у масиві

	PA6     ------> ADC1_IN6	6       5	Ізоляція
	PB1     ------> ADC1_IN9	7   	6	Кнопки LCD
	PC0     ------> ADC1_IN10	8       7	Високий рівень
	PC1     ------> ADC1_IN11	9		8	Низький рівень
	PC2     ------> ADC1_IN12	10		9	Сухий хіди
	PC3     ------> ADC1_IN13	11		10	Аналоговий датчик тиску
	PC4     ------> ADC1_IN14   12		11	Датчик вологості
	PC5     ------> ADC1_IN15	13		12	Датчик температури*/
#ifndef FURIE_TEST
	if (adc_meas == for_Furie_18){
/*		PA0-WKUP------> ADC1_IN0	1		0	Струм Фаза 1
		PA1     ------> ADC1_IN1	2		1	Струм Фаза 2
		PA2     ------> ADC1_IN2	3		2	Струм Фаза 3
		PA3     ------> ADC1_IN3	4		3	Струм витоку
		PA5     ------> ADC1_IN5	5       4	Напруга, Фаза 3		*/
		/* 20 мс збираємо відліки струмів і напруги*/
		iFurieA[i_convFurie]=(adcf_t)adcGRDValue[0];
		iFurieB[i_convFurie]=(adcf_t)adcGRDValue[1];
		iFurieC[i_convFurie]=(adcf_t)adcGRDValue[2];

		uFurieC[i_convFurie]=(adcf_t)adcGRDValue[3];

		if(i_convFurie ==0){
		}
		HAL_ADC_Stop_DMA(hadc);
		i_convFurie++;
		/* Якщо вже всі 18 відліків - на розрахунок GRID (або пропускаємо) і
		 * переходимо на 240 відліків протягом 20мсек струмів і напруги теплових */
		if (i_convFurie==N_FURIE+3){
			/* зупиняємо переривання для Фурьє*/
			HAL_TIM_Base_Stop_IT(&htim7);
			i_convFurie=0;
			adc_meas = for_Nothing;
		}
	}
	else if (adc_meas == for_Classic_240){
/*		PA0-WKUP------> ADC1_IN0	1		0	Струм Фаза 1
		PA1     ------> ADC1_IN1	2		1	Струм Фаза 2
		PA2     ------> ADC1_IN2	3		2	Струм Фаза 3
		PA5     ------> ADC1_IN5	5       3	Напруга, Фаза 3		*/
		iA[i_conv240]  =(uint16_t)adcGRDValue[0];
		iB[i_conv240]  =(uint16_t)adcGRDValue[1];
		iC[i_conv240]  =(uint16_t)adcGRDValue[2];


		uC[i_conv240] = (adcf_t)adcGRDValue[3];
		HAL_ADC_Stop_DMA(hadc);
		i_conv240++;
		if (i_conv240==240){
			HAL_TIM_Base_Stop_IT(&htim6);
			i_conv240=0;
			MesETCntr++;
			adc_meas = for_Nothing;
			tim7_18_duration=HAL_GetTick()-tim7_Tick;
		}

	}
	else{}

#endif //FURIE_TEST
}
void ADC_ConvInit(void){
	i_convFurie=0;
	i_conv240=0;
	SysTick_Cntr=0;
	MesETCntr=0;
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
