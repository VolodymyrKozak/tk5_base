/*
 * pr_OVEN_cmi.h
 *
 *  Created on: 24 ����. 2018 �.
 *      Author: KozakVF
 */

#ifndef TKSUBPROJECTS_PR_OVEN_CMI_H_
#define TKSUBPROJECTS_PR_OVEN_CMI_H_

#ifdef MODBUS_MASTER_for_OVEN_INDICATOR

/* ��� ������ ������ ������� ������� */

#ifdef MODBUS_MASTER_for_OVEN_INDICATOR_TESTING

#include "tk_oven_smi2.h"


void f_oventest_Init(void);
/* ������������ ��������� ����������� ������� ���� ���
 * ҳ ��� ����� ���������� � �� ���� ��� � �� �������.
1. 5 ������ �������� �������� ����� � ������ � ����� �����,
��������� - �� ���� �� �� ������ �������
(������� f_ovenSMI2_DisplayFloat(..)
2. 5 ������ �������� ����� ���� ����������� ������� �� ��
���� ���
(������� f_ovenSMI2_DisplayBroadCast(..)
3. ������� ������� �� ������ ������
4. �������� ������� �� ������ ������
*/
void f_oventest(uint32_t ov_counter);
/*1. 5 ������ �������� �������� ����� � ������ � ����� �����,
��������� - �� ���� �� �� ������ �������
(������� f_ovenSMI2_DisplayFloat(..)*/

void f_oventest1(void);
/* 2. 5 ������ �������� ����� ���� ����������� ������� �� ��
���� ��� */
void f_oventest2(void);
#endif// MODBUS_MASTER_for_OVEN_INDICATOR
#endif //MODBUS_MASTER_for_OVEN_INDICATOR_TESTING

#endif /* TKSUBPROJECTS_PR_OVEN_CMI_H_ */
