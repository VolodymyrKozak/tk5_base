/*
 * vv300_set.h
 *
 *  Created on: 9 ���. 2019 �.
 *      Author: KozakVF
 */

#ifndef TKSUBPROJECTS_VV300SET_H_
#define TKSUBPROJECTS_VV300SET_H_
#include <stdbool.h>
//#include "tkConfig.h"
#include "vv300Flash.h"
#include "u16set.h"

#define N_USET  2
#define N_FCSET 4



//extern uint32_t us32set[N_USET / 2 +2];
extern uint16_t *uset;
extern uint32_t fc32set[N_FCSET / 2 +2];
extern uint16_t *fcset;




typedef struct {
	float amperageAclbr; 	// 0�2000
	float clbr_iA;			// 0�2002

	float amperageBclbr;	// 0�2004
	float clbr_iB;			// 0�2006

	float amperageCclbr;    // 0�2008
	float clbr_iC;	        // 0�200A

	float voltageCclbr;		// 0�200C
	float clbr_uC;			// 0�200E

	uint16_t DeviceMode;     //0x1010 ��������� ���������� 1...5
	uint16_t Faznost;		 //0x1011

	uint32_t xx12;       	 //0x1012

	uint32_t xx14;			 //0x1014
	uint32_t xx16;			 //0x1016

	uint32_t xx18;			//0x1018
	uint32_t xx1A;			//0x101A

	uint32_t xx1C;			//0x101C
	uint32_t xCRCA;			//0x101E
}fSet_t;
extern fSet_t fSet;

void f_FSsetInit(void);
void f_UsetInit(void);

flash_status_t f_VV300_FactorySets_Read(void);
flash_status_t f_VV300_FactorySets_Write(void);
flash_status_t f_VV300_UserSets_Read(void);
flash_status_t f_VV300_UserSets_Write(void);
bool  f_rated_amperage_test(float a);
#endif /* TKSUBPROJECTS_VV300SET_H_ */
