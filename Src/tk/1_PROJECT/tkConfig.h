/*
 * tkConfig.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: KozakVF
 */

#ifndef TKCONF_H_
#define TKCONF_H_
#include "stdint.h"

/*****************************************************************************
 * ���Բ����ֲ� �������� �������
 *****************************************************************************
 * 1. ������Ͳ ���������
 * 1.1. � ������, ������������ CubeMX ����������� ������� ������.
 * � ���� main.c ������������ ���� �������
 * 	tkInit() ��� ������� ������� �����볿 ��� ������� �������,
 * 	tkLoop() ��� ������� ������� � ��������� ����.
 * 1.2.�� ������ ����� .h .c ������� � ������:
 *  tk/1_PROJECT
 *  tk/2_SubProgects
 *  tk/3_Libraries
 * 1.3.� ����� 3_PROJECT �������:
 * 	��� ���� ������������ ������� tConfig.h,
 * 	����� tkMain.c �� .h,  �� �����������:
 * 		��������������� ��� ������� �������, ������ �������
 * 	����� tkErr_Handler.c �� .h	������� ��������� ������� � ������� ����������
 * 	��� �������� ������b � ����-������.
 * 1.4.	� ����� 3_Libraries ������� ����� .c �� .h ���������� ������, ����� �������,
 * �� ��������� ��� ����������� � ����� ����� ��������, ��������� - �������� �������� �� ��.
 * 1.5  � ����� 2_SubProjects �������� ������� ������� �������� �������, ��� ������� �������
 * ����������� �������, ������� � ������� ���������� ������ ��������.
 * 1.6. ��������� ������������ ��� ����������� �������� ������� � ������� �������
 * ����������� ��������� � ����� ����.
 * 1.7. � ���������� �������� � ������� ������� �������� ������� � �������,
 * �� �� ���������������� ������ ���������� ��� ����������� � ������������ #ifdef
 *
*/
#define ZERO_INIT_Cplt 0x1254

//#define ENABLE_I2C
/*****************************************************************************************
 *  2. �������
 *  ���������� ��� �������, ��� ���:*/
#ifdef ENABLE_I2C
#define WINSTAR_Display_WH1602B3
//#define WINSTAR_Display_WO1602G
#endif //ENABLE_I2C
/* ������ ��� ���, ������� ������ - ������ �� �� �����, ��� ������ �� ����.. */
#define OFFICEBUTTONS
//#define HOMEBUTTONS
/*****************************************************************************************
 *  3. ������. ������������ �� ������������
 *********************************************************************
 * ���������� ���������� Modbus ��� ������� . ������ �������:
1) ���� ���� Master
2) ���� ���� Slave
3) Master �	 Slave
4) Master �  Master
�����: Slave � Slave �� �����������
�����: ���� � ��� �� UART/RS485 � ������ � ���� � ������
�� ���� ���� � MASTER, i SLAVE */

//#define MASTERRTU_HUART_4
//#define MASTERRTU_HUART_5

#define SLAVERTU_HUART_4
//#define SLAVERTU_HUART_5
#if defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
/*���� � ������ ����������� MODBUS RTU SLAVE, ���������:
  1) ���������� �������� � ����� RTU ������ Modbus 01..125 */
#define SLAVE_NODE_ADDRESS      0x01
/*2)������� ����� �������y��� ��������� �������
 * ����������� (������� MB_Slave_Selective_Handler())
 *  � ��������� ������� (MB_Slave_Broadcast_Handler())
 *  ����������, ���� tkSubProjects/pr_MBRTU_SLAVE.c,
 *  ��������������� �������� (���� tkModbus/mb_slave5.c �� ������ */
/* �������� ���� ���� ������� ���� ���� � �����������   */
//#define MODBUS_APPLICATION_SLAVE_CLASSIC
//#define MODBUS_APPLICATION_SLAVE_asTK2
#endif // defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
/*******************************************************************/
//#define MASTERRTU_HUART_4
//#define MASTERRTU_HUART_5
#if defined (MASTERRTU_HUART_4) || defined (MASTERRTU_HUART_5)
/******************************************************************************
 * �������������� ������, ���� TK4 �������� ������������ ���� ���2 ��������� RS-485
 * ��� ����� ����'������ ������� ���� ��������������� ��������� MASTERRTU_HUART
 * � ����� ����� */
//#define MODBUS_MASTER_for_OVEN_INDICATOR
/*������������� � �������� ������, ���� ���������� ��� �� ������� */
//#define MODBUS_MASTER_for_OVEN_INDICATOR_TESTING
//#define MODBUS_MASTER_for_OVEN_ARCHIVE_MCD200

/* ��������������,���� ������������ �������� ������-���������� ��� ���� ��2*/
//#define DRIVES_REMOTE_CONTROL
#endif //defined (MASTERRTU_HUART_4) || defined (MASTERRTU_HUART_5)
/****************************************************************************************
 * ��˲ ��������� ���� ��
 */

//#define THREE_PHASE_MOTOR_PROTECTION
//#define ONE_PHASE_MOTOR_PROTECTION
//#define THREExONE_PHASE_MOTOR_PROTECTION
//#define BOARD_TK4_TESTING
#define STM32_INTERNAL_CLOCK
#define DEBUGGING_ONLY
#define IT_WAS_ATTEMPT_ONLY


/* ��������� ��� ��������� ����� �������� */
//#ifdef TK2_REMOTE_CTR







/* ***************************************************************************************
 * ���������� ��������� ����� ������������ ������ � ������������� ������������ �����  *
 * ***************************************************************************************
 * �� ����� �������� ����� �������������, ����� 20 ����� ��� �� �����
 * �������,����� ������ ������, �� ������ ������ (��� �������� � ���� ���. ��������������
 * �������������� ��������� ������������:
 * 	���� ������� � ���� � ���,
 * 	�������,
 * 	���� ������ � ����� � ���,
 *  ����� ������ (�������������)
 *  ����������� ������������������ ����(Total Harmonic Current Distortion)
 *  ���Բֲ����:
 *  	������� �� (power factor), ���������� ��������� ��������������
 *  	���������� ��������� �������������� ���������� ����������
 *  	���������� ��������� �������������� ���������� �������� ������
 *
 * */
/* ������ � ����������� � ����������� ��������� ������������*/
#define GRD_FURIE /**/
#ifdef GRD_FURIE
/* ���������� ������������ ������������ ����� ��� ������������ 		*/
//#define FURIE_TEST
/* ����������� ������ � ����������� ���� �ղ���� ���������*/
#define GRID_DISTANCE_DEBUG
/* ʳ������ ����� �� ����� ������/������� ��� ��������� ���'�      */
#define N_FURIE (uint16_t)18
#define N_FURIEBUTTON  N_FURIE/5 + 1
#define FURIE_LIB
#endif //GRD_FURIE

/* ³����������� */
#ifdef DEBUGGING_ONLY
//#define FREQUENCY_CALC_DEBUG0
//#define FREQUENCY_CALC_PRINT0

/* ���� �� ����� ����������� ����� ������ ���� �
 * ����� VC 200 ����� */
//#define TIME_CNTR_DEBUG
//#define AMPERAGE_FURIE_VS_EUGENYT0
//#define COS_FI_CALC_PRINT0
//#define AMPERAGE_UNBALANCE_PRINT0
#endif


//#define FLASH_TO_RESTART /* ����������� ���� ���� tk4 ���� ���� ���������� ������ ����������� ���������*/



#define adcf_t float
#define adcu16_t uint16_t
#define amperf_t float
#define hzf_t  float
#define voltf_t  float
#define vatt_t float
#define kvatt_t float
#define gradf_t  float
#define procentf_t float


#define DISTANE_DEBUG

#endif //TKLIBRARIES_TKCONF_H_
