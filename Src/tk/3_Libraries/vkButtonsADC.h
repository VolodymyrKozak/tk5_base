/*
 * 	tkButtonsADC.h
 *  Created on: 1 ���. 2018 �.
 *  Author: Volodymyr.Kozak
    ���������� ������ ̲������������� (�������)
 *  (���������� ����� ������ ����������� ����� ���� ���� ���, ��� ����� ������ - �� ���� ��� ����� ��� �������)
 *  ��� ���������� ������ ��������� ����������� ���������
 *  ����������� �������� - 2 ���������� � �������
 */
#ifndef TKBUTTONSADC_H_
#define TKBUTTONSADC_H_

#include "stdint.h"
#include "../1_PROJECT/tkConfig.h"
/********************************************************************************************************************
 *  ���˲�Ͳ ����������
 ********************************************************************************************************************/

typedef enum {            /*������������ ���� ��� �������� ������������� ������� ���������� ������ ������� ������� button_pool() */
	NONE_btn,
	DELAY_btn,

	ESC_btn,
	UP_btn,
	DOWN_btn,
	ENTER_btn,

	ESCLong_btn,
	UPLong_btn,
	DOWNLong_btn,
	ENTERLong_btn

} button_output_t;
extern button_output_t BTN;
extern uint32_t BTN_DelayTick;
extern uint32_t BTN_Delay;

extern uint16_t iBTNdiv;
typedef enum {
	pool_btw_btn_state,
	pool5s_before_start_state,
	pool_started_state,
	pool_Btn_state,
	pool5s_before_LongPressed_state,
	pool_LongPressed_state,

	pool_calm_down,
	pool_calm_down_wait,

	pool_exit_state
} btn_pool_state_t;

typedef struct {
	uint8_t  iReset;
	uint8_t  iYes;
	uint32_t iBtw;
	uint8_t  iNo;
	uint8_t  xLongBP;
	btn_pool_state_t s;
	button_output_t sPool;
} i_btn_t;

/* �����: �������� ���������� ���������� ������ - 100 ����,
 *         �������� ������� ���������� ������    - 500����
 * ������ ����������� ����� ���� ��������� 2..7 ���� ��, �� 100���� - ���� 17-19 ��������� ������
 * ϳ��� ������������ ���������� ������ ������� ������ */
typedef struct {
	uint32_t LBtw;  		/* ������ ��� ���� ������������ ���������� ������, �� ����� �������� */
							/* � ��� ����������� �������� ���� ���� ����������� ���������� ������ ����
							 * ��������������� BTN_Delay*/
	uint8_t  LBeforeStart;  /* ������� ��� �� ������� �������� ���;*/
	uint8_t  LYes; 			/* ������� ��� �� ������ ��� �������� PRESSED */
	uint8_t  LNo;			/* ������� � ���� ���.. */
	uint8_t  LReset;		/* ������� � ��� �������� ������� ��������� ��������           */
    uint8_t  xLYes;         /* �������� LYes ��� ����������� ��������� �� PRESSED �� LONGPRESSED*/
    						/* LYes x xLYyes - ������� Yes �� ��������� ������ PRESSED �� LONGPRESSED */
    						/* 100���� - �� 20 �����, 500���� - �� 100 �����*/
    						/* ����  xLYes=5 LYes=10, - �� 50 Yes, ����� ��������� 250����*/
    						/* ����  xLYes=10 - �� 250 ����
    						 * xLYes �������� � ��������� �� ������������ */
} btn_limit_t;




 /* */
/********************************************************************************************************************
 *  ������ͳ ����������
 ********************************************************************************************************************/


/********************************************************************************************************************
 *  ���˲��� ����ֲ� ���������� ������
 ********************************************************************************************************************/
int f_BTNs_Pool_Init(void);
button_output_t f_BTN_Pool (uint32_t adc_buttons_pool);
button_output_t buttonsADCpool(uint32_t adc_buttons_pool);
void f_btn_debug (button_output_t btn);

int f_BTNs_Pool5ms_Init(void);
button_output_t f_BTN_Pool5ms (uint32_t adc_buttons_pool);


int f_BTNs_PoolSmooth_Init(void);
void f_BTN_PoolSmooth (uint32_t adc_buttons_pool);

#endif /* TKBUTTONSADC_H_ */
