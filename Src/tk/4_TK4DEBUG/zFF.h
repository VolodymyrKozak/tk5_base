/*
 * zFF.h
 *
 *  Created on: 24 ���. 2019 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKLIBRARIES_TKMENU_ZFF_H_
#define TKLIBRARIES_TKMENU_ZFF_H_

#include "../1_PROJECT/tkConfig.h"
#include "stdint.h"

#include "stdlib.h"
#include "string.h"
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "stdint.h"

#include "tk_wh1602b3.h"
#include "tk_wo1602g.h"
#include "../3_Libraries/vkButtonsADC.h"

typedef enum {
	dsp_OK,						/* OK, ��� ���������															*/
	dsp_UNKNOWN_Err,			/* ����������� �������*/
	dsp_wo1602gErr,
	dsp_FLASHErr,
	dsp_I2C_Err,				/* ������� ��������������� ����� �� I2C 										*/
	dsp_BUSY,
	dsp_WRONG_Arg,				/* �� ���� ������� ��������� ���������, ���� � ������� ���� ������ ���������� */
	dsp_NULL_Ptr,				/* �������� �������� (���. - ���������), ������ ���������� ������ 				*/
	dsp_Err						/* ������� ��� ��������� �������, ���� �� ���..*/
}dsp_status_t;


extern int8_t strHigh_menu[17];
extern int8_t strLow_menu[17];
extern int8_t strLow0_menu[17];

dsp_status_t tkDisplay_Init(void);
dsp_status_t tkDisplay_ClearScreen(void);
dsp_status_t tkDisplay_PutHighStr(int8_t *hstr);
dsp_status_t tkDisplay_PutLowStr(int8_t *hstr);


#endif /* TKLIBRARIES_TKMENU_ZFF_H_ */
