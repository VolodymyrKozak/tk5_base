/*
 * wk_motor_ctrl.c
 *
 *  Created on: 25 ����. 2019 �.
 *      Author: KozakVF
 */

#include "wk_motor_ctrl.h"
#include "../1_PROJECT/tkConfig.h"
#include "../3_Libraries/vkDeclarations.h"

/* ������� �������� ��� ������.
 * ���� ���� ��������� ������ START � ������
 * �� �����.������� �� �������  - oN_
 * ���� ���� ��������� ������ STOP
 * �� ������� ������� �� ������� - oFF_*/
on_off_t LastCmndForMotor  = oFF_;
/* �� ������� �������� ����� �� ����������, � �� � �� �����������,
 * �� �������� ����, ���� ����������� �� ������� �������� ������ */
motor_state_t MotorState  = oFF_;
uint32_t MotorOperationalLife=0;


