/*
 * mb_slave.c
 *
 *  Created on: 10 ����. 2017 �.
 *      Author: Volodymyr.Kozak
 */


/*
 * tkModbus_slave7.c
 *
 *  Created on: 27 ���. 2017 �.
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
/* ˳������� �������� */
extern volatile uint32_t SysTick_Cntr;
/* ������, �� ����� ��������� ������ ������� ��������� �������� ������ ������,
 * ���� ����������� �������� �� �������� � ��������� 3.5 ����.*/
uint32_t EndSysTickDelay = 0;
/* ������� �ղ����� ��²�������� MODBUS_RTU
 *
// * �������  RX_Init_modbus_slave (void) ����������� ��� ����������� slave-����������
 *  ������� ����� ���� � ����� modbus_rtu RS-485->uart,
 *  ��� ���� ������ �� �������� �� ����������� � �����.
 *  ���� �����������
 *  	���������� ����� slave
 *  	��� �� ���������� ����������� ��� ��� slaves � �����
 *  � ���� ������� CRC � ���������� � ����������  CRC
 *  ����������� ���������� �� �������, �� ���� ����������� ����������� ��������������.
 *  ����� ����������� Modbus � ����� ����� �� ������������.
 *
 *  �� ��� ��������� ������ ���� modbus_rtu RS-485->uart ���������.
 *  ϳ���������� ��� Modbus-master ������� �������� � ��� ������,
 *  �� ���������������� ������ ������ Modbus-master �� ����� ������� �RC
 *  � ����������� � ������ uart ->RS-485->modbus_rtu
 *  ϳ��� �������� ��������� ����������� ����������� ����������� ������� RX_Init_modbus_slave (void)
 *  ��� ����������� ���������� ����� ����� modbus_rtu RS-485->uart � ����� ��������� ����������,
 *  �� ��������� ��� ����� slave
 *
 ***********************************************************************************************************************************
 *                                   ���˲�Ͳ ���������� �� �������² �����������
 */
modbus_slave_tx_msg_t mbTxS ={0};
/* ̳� ����������� ������� �������� ����������� � �������� �������� ������
 * �� ������ �� ����� ���� �������� 3.5 ������� */
static volatile uint32_t RxDxDelay=0;
extern void fTK4_Slave1(
		modbus_slave_rx_msg_t *rx,
		modbus_slave_tx_msg_t *tx
		);
/* �������� ������ ������, ��������� ����� ��� �������� �������� � �������� ����*/
modbus_status_t MB_SlaveStatus = MODBUS_ERR_UNKNOWN;
static modbus_slave_rx_msg_t mbRxS ={0};
/* ����������� ����� ��� ��� ��������� �� ������� ����������� ������*/
static modbus_status_t f_MB_Slave_Receive_Init (modbus_slave_rx_msg_t *pmbRxS );
/* ����������� � ����������� ���� ������� ����������� ������ */
static modbus_status_t f_Slave_RxIdleCpltCallback(modbus_slave_rx_msg_t *pmbRxS);
/* ����������� � ���������� ������� ��� �������� ������ ������� ����� �볺���/�������*/
static modbus_status_t f_MB_Slave_Transmit (modbus_slave_tx_msg_t *pmbTxS);


/* ����������� ���� ��� ����������� ��4,
 * ������� ������� ���������� �������� ������ */
void MB_Slave_Receive_Init (void){
	/* ����������� ��������� ��� � ������������ �� �������� ������ ���� ���������� �������
	 * �� ��������� - 3.5 ���� �������� ������ �������(�����) ����������� */
	float t3_5 = (float)10000 / /* ��� �������� ��� �������� �� ������ ���, � ������ �� (8+�����+����)*/
			(float)SLAVE_HUART.Init.BaudRate * 3.5f
			+2.0f;/* ������ ���� ����������� ��� ������������ */
	/* ���������� �������� �  ����������  */
	RxDxDelay=(uint16_t)t3_5;
	MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
}

/* ����������� �������� void HAL_UART_RxIdleCallback(UART_HandleTypeDef *UartHandle) ����� mb.c,
 * c������ ��� ������������� slave ��/��� master
 * This procedure could be used for aborting any ongoing transfer started in Interrupt or DMA mode.
 *  This procedure performs following operations :
		Disable PPP Interrupts
		Disable the DMA transfer in the peripheral register (if enabled)
		Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
		Set handle State to READY
		At abort completion, call user abort complete callback
		This procedure is executed in Interrupt mode, meaning that abort procedure could be considered as completed only
		when user abort complete callback is executed (not when exiting function).
		�������� ������� HAL_UART_RxIdleCallback(UART_HandleTypeDef *UartHandle) ��������� � ���� stm32f1xx_hal_uart.h
*/
void Slave_RxIdleCpltCallback(void){
	MB_SlaveStatus = f_Slave_RxIdleCpltCallback(&mbRxS);
}



/* ����������� ����� ��� ��� ��������� �� ������� ����������� ������*/
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
/* ����������� � ����������� ���� ������� ����������� ������ */
static modbus_status_t f_Slave_RxIdleCpltCallback(modbus_slave_rx_msg_t *pmbRxS){
	modbus_status_t mscb = MODBUS_ERR_UNKNOWN;
	while (mscb == MODBUS_ERR_UNKNOWN){
//
		/* ������� ����������� RTU */
		uint16_t msgS_length=SLAVE_HUART.RxXferSize - SLAVE_HUART.RxXferCount;
		__HAL_UART_DISABLE_IT(&SLAVE_HUART, UART_IT_IDLE);
		SLAVE_HUART.RxState = HAL_UART_STATE_BUSY;
		/* � ����������� ����� ��������� �������*/
		if (msgS_length>=RX_MAX_SLAVE_MSG_LENGTH){mscb=MODBUS_ERR_BYTE_COUNT;break;}
		/* ���� ����������� ������� �� 4 �����, 			*/
		/* ����������, ����� ������� �����������			*/
		if (msgS_length<4) {mscb=MODBUS_ERR_BYTE_COUNT;break;}
	    mbRxS.length=msgS_length;
		/* ���� ����������� �� ��� ��� � �� ����������
		 *����������	*/
		if (!((mbRxS.msg[0]==SLAVE_BROADCAST_ADDRESS)||
					(mbRxS.msg[0]==SLAVE_NODE_ADDRESS))){mscb=MODBUS_ERR_SLAVE_ADDR;break;}
		mscb=f_check_CRCs(&mbRxS);
		if(  mscb!= MODBUS_SUCCESS){break;}
		/* �������� ��� ����������� ������� ��������������� ������ */
		if (mbRxS.msg[0]==SLAVE_BROADCAST_ADDRESS){mscb=MODBUS_ERR_SLAVE_ADDR;break;}

        /* ��������. ������� ��� �������*/
		RxDxDelayCntr=HAL_GetTick();

		/* ϳ�������� ������ �� ����������� ���
		 * - ��������� ������ � ������� ����������� mbTxS */
		fTK5_Slave1(&mbRxS,&mbTxS);
		mscb=f_add_CRCs(&mbTxS);
		if( mscb != MODBUS_SUCCESS){break;}
		uint32_t tk5Slave1_duration = HAL_GetTick()-RxDxDelayCntr;
		/* ��������� ��� ������� �������� ������ */
		if(RxDxDelay>tk5Slave1_duration){
			EndSysTickDelay=SysTick_Cntr+(RxDxDelay-tk5Slave1_duration);

		}
		/* �� ��������� �������� 3.5 ����� �� ����������� ������� � ������� ��������
		 * �� ��������� �������� ������� EndSysTickDelay, ���� � ��������� ��������� ����������
		 * ������� ������� f_MB_Slave_Transmit (&mbTxS) ��� �������� ������ ;*/
		//mscb=f_MB_Slave_Transmit (&mbTxS);
	}/* ����� � ����� ������� �������� ����������� */
/*********************************************************************************************/
	if(mscb !=MODBUS_SUCCESS){
		/* ���� ���� �� ��� - ������������ ������� �� ������ ���������� ����������� */
		MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
	}
	/* ���� ������� ����������� ����������� ������,
	 * ������ �������� ����������� ���� �������� �����������, �������� �� �..*/
	return mscb;

}//else if (msg_received_length!=0){

void MB_Slave_Transmit(void){
	modbus_status_t msst = MODBUS_ERR_UNKNOWN;
	msst=f_MB_Slave_Transmit (&mbTxS);
}
/* ����������� � ���������� ������� ��� �������� ������ ������� ����� �볺���/�������*/
static modbus_status_t f_MB_Slave_Transmit (modbus_slave_tx_msg_t *pmbTxS){

 /* The board sends the message and expects to receive it back */
 /*Configure GPIO pin Output Level to turn on RS485 TRANSMITT REGIME */
 HAL_GPIO_WritePin( SLAVE_RS485_SWITCH_PORT, SLAVE_RS485_SWITCH_PIN, GPIO_PIN_SET);
  /*##-2- Start the transmission process #####################################*/
 HAL_StatusTypeDef tr = HAL_ERROR;
 tr=HAL_UART_Transmit_IT(&SLAVE_HUART, pmbTxS->msg, pmbTxS->length );
 if(tr!=HAL_OK){
		MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);
		/* ���� �������� ���������� ������, ������� ����������� � �����������
		 * �� ��������� �������� */
 }
 return tr;
}

/*����������� �������� HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) ����� mb.c,
 �� ����������� ���� ���������� ��������, ������� ������ ���������� �����������  * */
void Slave_TxCpltCallback(void){
	/*Configure GPIO pin Output Level to turn on RS485 RECIEVE REGIME */
	HAL_GPIO_WritePin(SLAVE_RS485_SWITCH_PORT, SLAVE_RS485_SWITCH_PIN, GPIO_PIN_RESET);
	/* Set transmission flag: transfer complete */
	/* ��������� ������ ���������� ����������� */
	MB_SlaveStatus = f_MB_Slave_Receive_Init (&mbRxS);

}


#endif //defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)



/**********************************************************************************************************************************************************
*  ���˲��ֲ� - ������, ���� ���� �������. ����� �� ���.
*  ������� ����� � ���������� Modbus RTU slave �� ��� ������� ������� ���� � �������� ����������,
*  ���������� ������������ ���� ����.
*  ������ ���� ���������� ���� ����������� slave-����������, � ����� ����������� HAL_UART_Receive_IT
*  � ��� �������� ��������� �����, �� ����������� HAL_UART_RxCpltCallback, ������� HAL_UART_Receive_IT ����������� �����, � �.�. �� ����.
*  � ��� ������� ��������� ����� �� ����������� HAL_UART_ErrorCallback, ������� HAL_UART_Receive_IT ����������� �����.
*  ����� ������� ���������� �����  modbus_rtu RS-485->uart ����������� ���� �������� ������.
*  ���������, ��� ����� ��������������� ������ ���������.
*
*  ʳ���� �����������, ����� � ���������� Modbus RTU, ����������� ������ �������� ����, ����������� ��� ��������/������� �� ����� 3.5 �����,
*  � ��� ��������� ����� ����� 1.5 �����, ��������� ����������� ������� �����������.

*/
/* USER CODE END PFP */


/* ������������ ����������� IDDLE:
 * ���.https://community.st.com/thread/41068-cubemx-feature-request-add-usart-rx-idle-handling
 * https://www.riuson.com/blog/post/stm32-hal-uart-dma-rx-variable-length
 *
 * 1. ���� usart.c, ������� void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
 * ������ ����    HAL_NVIC_EnableIRQ(UART5_IRQn);
   	   	   	   	   USER CODE BEGIN UART5_MspInit 1
*******************************************************************************************
           __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
*******************************************************************************************
�� �� ����, ��:

         	 huart5.Instance->CR1 |= USART_CR1_IDLEIE;
********************************************************************
 	 	 	 	 	 USER CODE END UART5_MspInit 1
��� �� �������� ���������� IDDLE

 * 2. ���� stm32f1xx_hal_uart.h ������
 * void HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart)
 *
 * 3. ���� stm32f1xx_hal_uart.c, �������   HAL_UART_IRQHandler(UART_HandleTypeDef *huart):
 * ***********************************************************************************
  if(((isrflags & USART_SR_IDLE) != RESET) && ((cr1its & USART_CR1_IDLEIE) != RESET))
  {
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    HAL_UART_RxIdleCallback(huart);
    return;
  }
  ************************************************************************************
  *************************************************************************************/




