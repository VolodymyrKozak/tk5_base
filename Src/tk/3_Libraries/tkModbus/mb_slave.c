/*
 * mb_slave.c
 *
 *  Created on: 10 жовт. 2017 р.
 *      Author: Volodymyr.Kozak
 */


/*
 * tkModbus_slave7.c
 *
 *  Created on: 27 вер. 2017 р.
 *      Author: Volodymyr.Kozak
 */

#include "mb_slave.h"

#if defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "usart.h"
#include "stm32f1xx_hal_uart.h"
#include "gpio.h"
#include "stm32f1xx_it.h"




#include "../tkDebugThings.h"

#include "tkCRC.h"

static volatile uint32_t  RxDxDelayCntr =0;
/* Лічильник мілісекунд */
extern volatile uint32_t SysTick_Cntr;
/* Прапор, по якому системний таймер повинен запустити передачу відповіді Модбас,
 * щодо дотриматися перервви віж прийомом і передачею 3.5 симв.*/
uint32_t EndSysTickDelay = 0;
/* ОБРОБКА ВХІДНОГО ПОВІДОМЛЕННЯ MODBUS_RTU
 *
// * Функція  RX_Init_modbus_slave (void) викликається при ініціалізації slave-контролера
 *  постійно сканує вхід з мережі modbus_rtu RS-485->uart,
 *  для чого приймає та перевіряє всі повідомлення в мережі.
 *  Якщо повідомлення
 *  	адресовано цьому slave
 *  	або це циркулярне повідомлення для всіх slaves у мережі
 *  і якщо співпадає CRC у повідомленні з обчисленим  CRC
 *  повідомлення передається на обробку, всі інші повідомлення автоматично відбраковуються.
 *  Логіка повідомлення Modbus у цьому модулі не перевіряється.
 *
 *  На час підготовки відповіді вхід modbus_rtu RS-485->uart блокується.
 *  Підготовлена для Modbus-master відповідь подається в цей модуль,
 *  де укомплектовується байтом адреси Modbus-master та двома байтами СRC
 *  і надсилається в мережу uart ->RS-485->modbus_rtu
 *  Після відправки вихідного повідомлення автоматично запускаєстья функція RX_Init_modbus_slave (void)
 *  для продовження сканування входу мережі modbus_rtu RS-485->uart з метою виявлення повідомлень,
 *  що призначені для цього slave
 *
 ***********************************************************************************************************************************
 *                                   ПУБЛІЧНІ ОГОЛОШЕННЯ ТА ПОЧАТКОВІ ПРИЗНАЧЕННЯ
 */
modbus_slave_tx_msg_t mbTxS ={0};
/* Між завершенням прийому вхідного повідомлення і початком передачі відповіді
 * має пройти не менше часу передачі 3.5 символів */
static volatile uint32_t RxDxDelay=0;
extern void fTK4_Slave1(
		modbus_slave_rx_msg_t *rx,
		modbus_slave_tx_msg_t *tx
		);
/* Поточний статус Модбас, глабальна змінна для передачі значення в основний цикл*/
modbus_status_t MB_SlaveStatus = MODBUS_ERR_UNKNOWN;
static modbus_slave_rx_msg_t mbRxS ={0};
/* Викликається кожен раз при готовності до прийому повідомлення Модбас*/
static modbus_status_t f_MB_Slave_Receive_Init (modbus_slave_rx_msg_t *pmbRxS );
/* Викликається з переривання після прийому повідомлення Модбас */
static modbus_status_t f_Slave_RxIdleCpltCallback(modbus_slave_rx_msg_t *pmbRxS);
/* Викликається з попередньої функції для передачі відповіді сервера Мобас Клієнту/майстру*/
static modbus_status_t f_MB_Slave_Transmit (modbus_slave_tx_msg_t *pmbTxS);


/* Викликається лише при ініціалізації ТК4,
 * запускає постійне опитування вхідного Модбас */
void MB_Slave_Receive_Init (void){
	/* Розраховуємо мінімальний час в мікросекундах до передачі відповіді після завершення прийому
	 * По стандарту - 3.5 часу передачі одного символа(байта) повідомлення */
	float t3_5 = (float)10000 / /* нас цікавить час передачі не одного біта, а десяти біт (8+Старт+стоп)*/
			(float)SLAVE_HUART.Init.BaudRate * 3.5f
			+2.0f;/* Додаємо одну мікросекунду для дурнестійкості */
	/* Обчислюємо затримку в  мілісекундах  */
	RxDxDelay=(uint16_t)t3_5;
	MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
}

/* Викликається функцією void HAL_UART_RxIdleCallback(UART_HandleTypeDef *UartHandle) файлу mb.c,
 * cпільною при функціонуванні slave та/або master
 * This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
 *  This procedure performs following operations :
		Disable PPP Interrupts
		Disable the DMA transfer in the peripheral register (if enabled)
		Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
		Set handle State to READY
		At abort completion, call user abort complete callback
		This procedure is executed in Interrupt mode, meaning that abort procedure could be considered as completed only
		when user abort complete callback is executed (not when exiting function).
		Прототип функції HAL_UART_RxIdleCallback(UART_HandleTypeDef *UartHandle) оголошено в файлі stm32f1xx_hal_uart.h
*/
void Slave_RxIdleCpltCallback(void){
	MB_SlaveStatus = f_Slave_RxIdleCpltCallback(&mbRxS);
}



/* Викликається кожен раз при готовності до прийому повідомлення Модбас*/
static modbus_status_t f_MB_Slave_Receive_Init (modbus_slave_rx_msg_t *pmbRxS ){

/* The board receives the message */
  /*##-1- Put UART peripheral in reception process ###########################*/
  memset(pmbRxS,0,sizeof(modbus_slave_rx_msg_t));
//  __HAL_UART_CLEAR_IDLEFLAG(&SLAVE_HUART);

  __HAL_UART_ENABLE_IT(&SLAVE_HUART, UART_IT_IDLE);
  SLAVE_HUART.RxState = HAL_UART_STATE_READY;
  if (HAL_UART_Receive_IT(
		  &SLAVE_HUART,
		  pmbRxS->msg,
		  RX_MAX_SLAVE_MSG_LENGTH  /*tx_length*/
	    )!=HAL_OK){
	    	return MODBUS_ERR_RX;
  }
  else{
	  return MODBUS_SUCCESS;
  }
}
/* Викликається з переривання після прийому повідомлення Модбас */
static modbus_status_t f_Slave_RxIdleCpltCallback(modbus_slave_rx_msg_t *pmbRxS){
	modbus_status_t mscb = MODBUS_ERR_UNKNOWN;
	while (mscb == MODBUS_ERR_UNKNOWN){
//
		/* довжина повідомлення RTU */
		uint16_t msgS_length=SLAVE_HUART.RxXferSize - SLAVE_HUART.RxXferCount;
		__HAL_UART_DISABLE_IT(&SLAVE_HUART, UART_IT_IDLE);
		SLAVE_HUART.RxState = HAL_UART_STATE_BUSY;
		/* Є повідомлення якоїсь прийнятної довжини*/
		if (msgS_length>=RX_MAX_SLAVE_MSG_LENGTH){mscb=MODBUS_ERR_BYTE_COUNT;break;}
		/* якщо повідомлення коротше ніж 4 байти, 			*/
		/* відбраковка, надто коротке повідомлення			*/
		if (msgS_length<4) {mscb=MODBUS_ERR_BYTE_COUNT;break;}
	    mbRxS.length=msgS_length;
		/* Якщо повідомлення не для нас і не циркулярне
		 *відбраковка	*/
		if (!((mbRxS.msg[0]==SLAVE_BROADCAST_ADDRESS)||
					(mbRxS.msg[0]==SLAVE_NODE_ADDRESS))){mscb=MODBUS_ERR_SLAVE_ADDR;break;}
		mscb=f_check_CRCs(&mbRxS);
		if(  mscb!= MODBUS_SUCCESS){break;}
		/* Заглушка для дописування обробки широкоформатних команд */
		if (mbRxS.msg[0]==SLAVE_BROADCAST_ADDRESS){mscb=MODBUS_ERR_SLAVE_ADDR;break;}

        /* Прийняли. засікаємо час прийому*/
		RxDxDelayCntr=HAL_GetTick();

		/* Підготовка відповіді на прикладному рівні
		 * - підготовка відповіді в вихидне повідомлення mbTxS */
		fTK5_Slave1(&mbRxS,&mbTxS);
		mscb=f_add_CRCs(&mbTxS);
		if( mscb != MODBUS_SUCCESS){break;}
		uint32_t tk5Slave1_duration = HAL_GetTick()-RxDxDelayCntr;
		/* Визначаємо час початку передачі відповіді */
		if(RxDxDelay>tk5Slave1_duration){
			EndSysTickDelay=SysTick_Cntr+(RxDxDelay-tk5Slave1_duration);

		}
		/* Що витримати інтервал 3.5 байта між завершенням прийому і початку передачі
		 * ми визначили значення прапора EndSysTickDelay, який з обробника системних переривань
		 * викличе функцію f_MB_Slave_Transmit (&mbTxS) для передачі відповіді ;*/
		//mscb=f_MB_Slave_Transmit (&mbTxS);
	}/* Вихід з циклу обробки вхідного повідомлення */
/*********************************************************************************************/
	if(mscb !=MODBUS_SUCCESS){
		/* Якщо щось не так - перезаряжаємо приймач на прийом наступного повідомлення */
		MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
	}
	/* Якщо обробка переривання завершилася успішно,
	 * запуск приймача здійснюється після відправки повідомлення, успішного чи ні..*/
	return mscb;

}//else if (msg_received_length!=0){

void MB_Slave_Transmit(void){
	modbus_status_t msst = MODBUS_ERR_UNKNOWN;
	msst=f_MB_Slave_Transmit (&mbTxS);
}
/* Викликається з попередньої функції для передачі відповіді сервера Мобас Клієнту/майстру*/
static modbus_status_t f_MB_Slave_Transmit (modbus_slave_tx_msg_t *pmbTxS){

 /* The board sends the message and expects to receive it back */
 /*Configure GPIO pin Output Level to turn on RS485 TRANSMITT REGIME */
 HAL_GPIO_WritePin( SLAVE_RS485_SWITCH_PORT, SLAVE_RS485_SWITCH_PIN, GPIO_PIN_SET);
  /*##-2- Start the transmission process #####################################*/
 HAL_StatusTypeDef tr = HAL_ERROR;
 tr=HAL_UART_Transmit_IT(&SLAVE_HUART, pmbTxS->msg, pmbTxS->length );
 if(tr!=HAL_OK){
		MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
		/* Якщо передача стартувала успішно, приймач запуститься з переривання
		 * по завершенні передачі */
 }
 return tr;
}

/*Викликається функцією HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) файлу mb.c,
 по перериванню після завершення передачі, запускає прийом наступного повідомлення  * */
void Slave_TxCpltCallback(void){
	/*Configure GPIO pin Output Level to turn on RS485 RECIEVE REGIME */
	HAL_GPIO_WritePin(SLAVE_RS485_SWITCH_PORT, SLAVE_RS485_SWITCH_PIN, GPIO_PIN_RESET);
	/* Set transmission flag: transfer complete */
	/* запускаємо прийом наступного повідомлення */
	MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);

}


#endif //defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)



/**********************************************************************************************************************************************************
*  РЕАЛІЗАЦІЯ - варіант, якщо буде потрібно. Зараз не так.
*  Оскільки згідно з стандартом Modbus RTU slave не знає наперед кількість байт у вхідному повідомленні,
*  приймається безперервний потік байт.
*  Перший байт приймається після ініціалізації slave-контролера, в режимі переривання HAL_UART_Receive_IT
*  в разі успішного прийняття байта, по перериванню HAL_UART_RxCpltCallback, функція HAL_UART_Receive_IT запускається знову, і т.д. по колу.
*  в разі помилки прийняття байта по перериванню HAL_UART_ErrorCallback, функція HAL_UART_Receive_IT запускається знову.
*  Тобто постійне скарування входу  modbus_rtu RS-485->uart здійснюється поза основним циклом.
*  Безумовно, при цьому використовується ресурс процесора.
*
*  Кінець повідомлення, згідно з стандартом Modbus RTU, позначається паузою протягом часу, необхідного для передачі/прийому не менше 3.5 байта,
*  в разі виявлення паузи більше 1.5 байта, приймання повідомлення повинно припинятися.

*/
/* USER CODE END PFP */


/* НАЛАШТУВАННЯ ПЕРЕРИВАННЯ IDDLE:
 * див.https://community.st.com/thread/41068-cubemx-feature-request-add-usart-rx-idle-handling
 * https://www.riuson.com/blog/post/stm32-hal-uart-dma-rx-variable-length
 *
 * 1. файл usart.c, функція void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
 * додати після    HAL_NVIC_EnableIRQ(UART5_IRQn);
   	   	   	   	   USER CODE BEGIN UART5_MspInit 1
*******************************************************************************************
           __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
*******************************************************************************************
це те саме, що:

         	 huart5.Instance->CR1 |= USART_CR1_IDLEIE;
********************************************************************
 	 	 	 	 	 USER CODE END UART5_MspInit 1
цим ви включити перериванн IDDLE

 * 2. файл stm32f1xx_hal_uart.h додати
 * void HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart)
 *
 * 3. файл stm32f1xx_hal_uart.c, функція   HAL_UART_IRQHandler(UART_HandleTypeDef *huart):
 * ***********************************************************************************
  if(((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    HAL_UART_RxIdleCallback(huart);
    return;
  }
  ************************************************************************************
  *************************************************************************************/




