/*
 * uList_set.h
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKLIBRARIES_TKMENU_ULIST_SET_H_
#define TKLIBRARIES_TKMENU_ULIST_SET_H_
#include "stdint.h"
typedef enum{
	LISTmenu_unknown_state,
	LISTmenu_input,
	LISTmenu_inputBTN,
	LISTmenu_Next,
	LISTmenu_Prev,
	LISTmenu_setting,
	LISTmenu_escape,
	LISTmenu_valuePreSet,
	LISTmenu_valueSet,
	LISTmenu_WrongData
}LISTmenu_state_t;

void f_listmenu_Init(void);

LISTmenu_state_t f_listmenu(    int8_t *uName,            /* �������� �� ����� ���������*/
								int8_t *uList,            /* �������� �� ������ �����*/
								uint8_t number_of_records,/* ������� �������� � ������*/
								uint8_t start_number,	  /* ���������� �������*/
								uint8_t *p_out_number);	  /* �������� �� �������� �������� */






#endif /* TKLIBRARIES_TKMENU_ULIST_SET_H_ */
