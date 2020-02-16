/*
 * mb_master.h
 *
 *  Created on: 27 ����. 2017 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef MB_MASTER_H_
#define MB_MASTER_H_
#include <stdint.h>

#include "mb.h"



extern volatile modbus_master_tx_msg_t modbus_master_tx;
extern volatile modbus_master_rx_msg_t modbus_master_rx;

extern enum master_state_5_t master_state_5;




enum master_state_5_t {
	Idle,						/* ���������� ���������� ��������	*/
	m_msg_transmitting,
	m_msg_transmitting_ERROR,
	m_msg_transmitting_Cplt,
	Waiting_turnaround_delay,   /* ���������� ���� slaves ��������� �������� ������������ �����������, �������� ����� ���������� �� ����������� 	*/
	Waiting_for_reply,			/* ���������� ������ �� �������� ����������� 			*/
	m_msg_receiving,
	m_msg_receiving_ERROR,
	m_msg_recived_RxIdle,
	Processing_reply,			/* ������� ������				*/
	Processing_error			/* ������� �������/����			*/
};
void Master_RxIdleCallback(UART_HandleTypeDef huart); /* ������� ����������� IDLE / �������� ���� ��������� ����� ��� ���� ����� */
void Master_TxIdleCallback(void);



#endif /* MB_MASTER_H_ */
