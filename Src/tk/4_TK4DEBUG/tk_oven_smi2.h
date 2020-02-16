/*
 * tk_oven_smi2.h
 *
 *  Created on: 23 ����. 2018 �.
 *      Author: KozakVF
 */

#ifndef TK_OVEN_SMI2_H_
#define TK_OVEN_SMI2_H_

#include <tkConfig.h>

#ifdef   MODBUS_MASTER_for_OVEN_INDICATOR
#include <stdio.h>
#include "usart.h"
#define SMI1_DEFAULT_MODBUSS_ADRESS_1 0x20
#define SMI1_DEFAULT_MODBUSS_ADRESS_2 0x21


/* ���� ������������ ����������������� ������ ����������
 * �������� f_ovenSMI2_DisplayBroadCast
 * �� ����� �������� ������������
 * �������������� ������ ���� ��������� � ��������
 * ���������/������ ���������� � ��������� ������.
 * ���� f_ovenSMI2_DisplayBroadCast �� ������������ - �� �������� ��������	*/
typedef struct {
	float value[31];
}vector_of_float_t;

/*  ���� �������, ����� ������������ ������������ ����� ��������� ��������
 *  - ��� ������ 															*/
typedef enum {
	oven_OK,
	oven_UnknownERR,
	oven_Modbus_ERR,
	oven_WrongDevice,
	oven_WrongeFunction,
	oven_WrongeRegime,
	oven_WrongeTask,
	oven_NULL_POINTER,
	oven_AnswerLOST
} oven_session_status_t;


/* �������������. ����� ������� ����������� ��������� �������
 * ��� ������������ ������������� OVEN SMI2,
 * ���� �������� ����� - ��������� ������������� ������� �������..
 * �� ��������� ������� ���������.
 * ovenSMI_ADDRESS - ������ ������ �������*/
oven_session_status_t f_ovenSMI2_Init (UART_HandleTypeDef *phuart,
									   uint8_t ovenSMI_ADDRESS);

/**********************************************************************************************************************
 * ����������������� ������ �� ��������� ����������� ����������
 * 1. ������-������ ���������� (�� 31, �� �����, ����������� ���2 ������ ���� ������,
 * �������� 0�20, 0�21, ... , 0�51. ����������� ���� ����� ������� �������� � ����������� ���������.
 * 2. ������ ��������� ����� ������ ���� �������� � ���������.
 * 3. ������ ��������� ����� ��������� ��� � � ������� f_ovenSMI2_DisplayFloat
 **********************************************************************************************************************/

oven_session_status_t f_ovenSMI2_DisplayBroadCast (	UART_HandleTypeDef *phuart,
													uint8_t fist_ovenSMI_ADDRESS,
													uint8_t number_of_values,
													vector_of_float_t *pv);
/*������ �� ��������� ��������� - ����� � ��������� ������
 * ���� value
 * 				>  999.0 		��������� 'Err '
 * 				< -999.0 		��������� '-Err'
 * ���� -999.0 ...+999.0  		��������� ��������, ����� ����� ������������ �������������
 * ���� -0.01  ... 0.001		��������� '0000'
 * */
oven_session_status_t f_ovenSMI2_DisplayFloat (	UART_HandleTypeDef *phuart,
												uint8_t ovenSMI_ADDRESS,
												float value
												);
/* �������� ������� */
oven_session_status_t f_ovenSMI2_DisplayBlinkingON (UART_HandleTypeDef *phuart,
													uint8_t ovenSMI_ADDRESS);
/* ��������� ������� */
oven_session_status_t f_ovenSMI2_DisplayBlinkingOFF (UART_HandleTypeDef *phuart,
													 uint8_t ovenSMI_ADDRESS);



oven_session_status_t f_ovenSMI2_Init_String (	UART_HandleTypeDef *phuart,
												uint8_t ovenSMI_ADDRESS);
/*************************************************************************************/
/* ��������� �������. �� ����� - �� �������. ����� - ����������� �����
 * � �����. � ������������ �� ������������ ���� ���2 ��������� RS-485 ����.421449.004��																				*/
/* ������ ���������� ��������� */
oven_session_status_t f_ovenSMI2_TASKS_16 (UART_HandleTypeDef *phuart,
										   uint8_t ovenSMI_ADDRESS,
									       uint8_t register_to_BEGIN,
										   uint8_t N_of_registers,
										   uint8_t N2_of_bytes,
										   uint8_t *str_to_oven);
/* ������ ������ �������� */
oven_session_status_t f_ovenSMI2_TASKS_06 (UART_HandleTypeDef *phuart,
										   uint8_t ovenSMI_ADDRESS,
										   uint16_t register_to_PRESET,
										   uint16_t register_value);
/* ������ �������� */
#ifdef MODBUS_MASTER_for_OVEN_INDICATOR_TESTING
oven_session_status_t f_ovenSMI2_TASKS_03 ( UART_HandleTypeDef *phuart,
											uint8_t ovenSMI_ADDRESS,
											uint16_t register_to_READ,
											uint16_t *ptr_received_values);
#endif //MODBUS_MASTER_for_OVEN_INDICATOR_TESTING
/* ������ ����� ������� � ������ ���������															*/
oven_session_status_t f_ovenSMI2_TASKS_17 (	UART_HandleTypeDef *phuart,
											uint8_t ovenSMI_ADDRESS);
#endif /*  MODBUS_MASTER_for_OVEN_INDICATOR */

#endif /* TK_OVEN_SMI2_H_*/

