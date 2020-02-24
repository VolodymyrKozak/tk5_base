/*
 * tkFreq.h
 *
 *  Created on: 27 ����. 2018 �.
 *      Author: KozakVF
 */

#ifndef TKLIBRARIES_TKMOTOR_TKFREQ_H_
#define TKLIBRARIES_TKMOTOR_TKFREQ_H_
#include "wk_atime64.h"
#include "../1_PROJECT/tkConfig.h"

#include "vv300set_factory.h"
#include "vv300set_user.h"

#include "tk3_grid_declarations.h"
/**/





/* ����������:
 * 		������� �� 46�� �� 54��,
 * 		������� (���� �),
 * 		�������������������� ������ ��� �BC (��������),
 * 		�������������������� ������ ����� �������� ��� ���.
 *
 * 	����������:
 * 		������� ���������, ��������� ��������� �� ����������� ��������� (�os fi),
 * 				������ ��� �� ������� �� ��������, ���� �,
 * 		����������� ������ ��������� ����� �������� ������� ������,
 * 				�� ����� ����������� �������,
 * 		���������� ������ ��������� ����� �������� ������,
 * 				�� ����� ����������� �������.
 *
 * 	³������ �� ���� �� ������ ���:
 * 		������� ������� �����������㳿 (���.���),
 * 		������� ������� ��������� �����������㳿 (����.���),
 * 		������� �����������㳿, �������� ����� �������� ������� (����.���),
 * 		������� �����������㳿, �������� �� ��������������� ����� �������� (���.���).
 *
 * 	���������:
 * 		������� ����� ��������� �� ���� ���������,
 * 		���������� ����,
 * 		����������� ����������� ���,
 * 		������ ������ �� ����� ���� �����������.
 *
 * */
typedef enum{

GRID_OK										= 0,
GRID_OverLoadProtection						= 1,
GRID_UnderLoadProtection					= 2,
GRID_VoltageFault							= 4,
GRID_AmperageUnbalanceProtection			= 8,
GRID_GroundProtection						= 0x10,

GRID_StartNumberLimit						= 0x20,
GRID_JummedProtection						= 0x40,

GRID_Phase_A_OverLoadProtection				= 0x80,
GRID_Phase_B_OverLoadProtection				= 0x100,
GRID_Phase_C_OverLoadProtection				= 0x200,

GRID_FREQUENCY_FAULT                        = 0x400,
GRID_PHASE_SEQUENCE_FAULT                   = 0x800,
GRID_UnknownFault							=0x80000000
} grid_status_t;



typedef enum {
	GRID_clc_OK,
	GRID_clc_FreqOverLimit,
	GRID_clc_FreqUnderLimit,
	GRID_clc_Impossible,
	GRID_clc_ZERO_PTR,
	GRID_clc_UnKnown
}grid_calc_status_t;

typedef	struct {
	Time64_T elipsed_time;
	uint32_t gstatus;
	uint32_t crc;
}grec_t;

union gr{
	grec_t 		gr_rec;
	uint32_t 	grfl[4];
};
extern union gr gr;

typedef struct {
	double Active;
	double Reactive;
	double Harmonic;
	double Unbalance;
	double Negative;
	double Neutral;
}power_t;


/******************************************************************************************
 * GLOBAL FUNCTIONS DECLARATIONS
 * ****************************************************************************************/

void f_GRD_Events(void);

int16_t f_GRD_Init(fSet_t *hfs, user_settings_t *hus);
grid_calc_status_t f_GRD_Calc(grid_t *hg);

//void f_GRID_Power_Init(void);
//void f_GRID_Power(void);
//
//void f_GRID_RaportHR(uint16_t* grid16);
//void f_GRD_Print(void);
#endif /* TKLIBRARIES_TKMOTOR_TKFREQ_H_ */
/* ����������:
 * 0		uint16_t, 0.01��		������� �� 46�� �� 54��,
 * 1		uint16_t  �						������� (���� �),
 * 2		uint16_t  0.1A, max 6553A �������������������� ������ ��� �BC (��������),
 * 3		uint16_t  0.1A 		�������������������� ������ ����� �������� ��� ���.
 * 4		uint16_t  0.1A
 * 	����������:
 * 5		uint16_t  0.1���    ������� ���������,
 * 6		uint16_t  0.1��� 	��������� ��������� ��
 * 7		int16_t   %		    ����������� ��������� (�os fi),
 * 8		int16_t   ����      ������ ��� �� ������� �� ��������, ���� �,
 *
 * 9		uint8_t   %			����������� ������ ��������� ����� �������� ������� ������,
 * 			uint8_t	  %         �� ����� ����������� �������,
 *
 * 	A		uint8_t	  % 		���������� ������ ��������� ����� �������� ������,
 * 	B		uint8_t	  % 		�� ����� ����������� �������.
 *  C
 *  D
 *  E
 *  F
 *
 *
 * 	³������ �� ���� �� ������ ���:
 * 0x10..0x2F	uint16_t 1..31    �� �������� �����
 *
 * ���������� � ����
 * 0x30..0x70		4 � uint32_t 0..11(1..12) �� ����� ����
 *
 * 		������� ������� �����������㳿 (���.���),
 * 		������� ������� ��������� �����������㳿 (����.���),
 * 		������� �����������㳿, �������� ����� �������� ������� (����.���),
 * 		������� �����������㳿, �������� �� ��������������� ����� �������� (���.���).
 *
 * 	���������:
 * 		0000 0001 ������� ����� ���������,
 * 		0000 0010 �� ���� ���������,
 * 		0000 0100 ������� ����� ���������,
 * 		0000 1000 ������� ���� ���������,
 * 		0001 0000 ���������� ����,
 * 		0010 0000 ����������� ����������� ���,
 * 		0100 0000 ������ ������ �� ����� ���� �����������
 * 		1000 0000 ������
 *

 *		Read Holding Registers 03
 *		Read Exception status  07
 *		Read Coils 01
 * */

#ifdef FURIE_TEST
void GreedModel(void);
#endif
#ifdef GRID_DISTANCE_DEBUG
void GridDistanceDebug(void);
void GridDistanceDebugInit(void);
#endif
