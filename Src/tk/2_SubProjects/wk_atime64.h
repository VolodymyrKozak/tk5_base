/*
 * wk_time64.h
 *
 *  Created on: 5 ����. 2018 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef DISPLAY_WK_TIME64_H_
#define DISPLAY_WK_TIME64_H_
#include "../3_Libraries/time64/stm32_rtc.h"
#include "../3_Libraries/time64/time64.h"
typedef enum {
	gui_OK,
	gui_UNKNOWN_Err,
	gui_ZERO_ptr,
	gui_wrong_arg
}gui_status_t;


typedef enum {
	clock_empty,
	clock_installed_data_time,
	clock_userti_installed
}clock_installed_t;

extern volatile Time64_T wk_base64_value; /* It's base time, set into device at the facility		 */


/****************************************************************************************************
 *  ��������� ��� � ��������, ��������� 1900.01.01.00.00.00
 *  � ������� ���� ���������� ����������� �������� ���������� ����
 *  ��� �����, ������ 64 ��.
 *  ��� ������������� � ����-����� ���� ������ �������� #include "wk_atime64.h"
 * **************************************************************************************************/
extern volatile Time64_T wk_real64_value; /* It's at the moment time, updatable with MCU every tick.
 	 	 	 	 	 	 	 	  It's user function to set real time at the begining 	 */



/****************************************************************************
 * �������� ��� � �������� ������, ���������� � ���������� �� ���������:
 *  ��� ������������� � ����-����� ���� ������ �������� #include "wk_atime64.h"
��'�	����
int tm_sec;		������� �� ������� ������� (0,59)
int tm_min;		������� �� ������� ������ (0,59)
int tm_hour;	������ �� ������ (0,23)
int tm_mday;	����� ����� (1,31)
int tm_mon;		̳���� ���� ���� (0,11)
int tm_year;	���� � 1900 (� Windows), � 1970 (� Unix)
int tm_wday;	�� � ���� (0,6)
int tm_yday;	�� � ������� ���� (0,365)
int tm_isdst;	������ ������� ����
****************************************************************************/
extern struct tm  wk64_tm;
extern struct tm *p_wk64_tm;
/* ���� ������� ��������� ������ - ��� ��������� ��������� ���� ��������������� �������:
 * struct tm MYTIME = {0};
 * struct tm *pMYTIME =NULL;
 * pMYTIME = localtime64   (&wk_real64_value);*/
gui_status_t  f_wk64_time_Init(void);
gui_status_t  f_wk64_time_Loop(void);
gui_status_t  f_wk64_time_Get(volatile Time64_T *p_Time64toGet);
gui_status_t  f_wk64_time_Set(Time64_T Time64toSet);






#endif /* DISPLAY_WK_TIME64_H_ */
