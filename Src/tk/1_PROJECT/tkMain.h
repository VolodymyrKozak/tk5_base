/*
 * tkMain.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKMAIN_H_
#define TKMAIN_H_

//#include "tkTimeDate.h"

typedef enum {                   	/* ������, ���� ���������� ������� �������� 									*/
	tkMAIN_OK,						/* OK, ��� ���������															*/
	tkMAIN_UNKNOWN_Err,			/* ����������� �������*/
	tkMAIN_wo1602gErr,
	tkMAIN_wh1602b3Err,
	tkMAIN_MenuSPV_Err,
	tkMAIN_FLASHErr,
	tkMAIN_I2C_Err,				/* ������� ��������������� ����� �� I2C 										*/
	tkMAIN_BUSY,
	tkMAIN_WRONG_Arg,				/* �� ���� ������� ��������� ���������, ���� � ������� ���� ������ ���������� */
	tkMAIN_NULL_Ptr,				/* �������� �������� (���. - ���������), ������ ���������� ������ 				*/
	tkMAIN_Err						/* ������� ��� ��������� �������, ���� �� ���.. 								*/
} tkMAIN_status_t;




tkMAIN_status_t tkInit(void);
tkMAIN_status_t tkLoop(void);
void tkErrHandler(void);






#endif /* TKMAIN_H_ */
