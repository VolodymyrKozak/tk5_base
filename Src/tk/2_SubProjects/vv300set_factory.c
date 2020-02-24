/*
 * vv300factory_set.c
 *
 *  Created on: 9 ���. 2019 �.
 *      Author: KozakVF
 */

#include "vv300set_factory.h"

char X_x;  //���������� �������, X_x=1..5
char Y_y;  //���������� �������  Y_y=0..8


typedef struct{
	uint16_t iMin;
	uint16_t iMax;
	uint16_t a;
	uint16_t b;
	uint16_t c;
} ki_t;
uint32_t fc32set[N_FCSET / 2 +2];
ki_t ET_AmpSens[6]={
	{0,		0,                0,  0,  0 },
//  ��������� � ������������ �������� ���� x=1..5
//  �����/10   ����.   �   �   �
    {20, 	240		/* x=1 */, 26, 26, 26},
    {100,	750		/* x=2 */, 26, 26, 26},
    {300, 	4000	/* x=3 */, 26, 26, 26},
    {1000,  5500	/* x=4 */, 26, 26, 26},
    {0,		330		/* x=5 */, 26 ,26, 26}
};

typedef struct{
	uint16_t imin;
	uint16_t imax;
} i412limit_t;

/* ������ ������ � ��������� �� ���������� ������� 412 */
i412limit_t i412limit[9]={
//����.  ����.
//����	 ����
//���/10 ���/10
{20,   	 170}, /* ��������� ���� 412 0*/
{60,	 240}, /*                     1*/
{100,	 620}, /*                     2*/
{200,	 900}, /*                     3*/
{300,	1100}, /*                     4*/
{500,	2500}, /*                     5*/
{500,	3800}, /*                     6*/
{1000,	5700}, /*                     7*/
{999,	999}   /*                     8*/
};
////������� ������� ���� ��������� � ������������ ������� Y=0..8                    // ���  06 04 2017
//     170/*Inmax,Y=0*/,240/*Inmax,Y=1*/,620/*Inmax,Y=2*/,900/*Inmax,Y=3*/,1100/*Inmax,Y=4*/,
//   //100,             101,             102,             103,             104,
//     2500/*Inmax,Y=5*/,3800/*Inmax,Y=6*/,5700/*Inmax,Y=7*/,999/*Inmax,Y=8*/,110/*���*/,
//   //105,              106,              107,              108,             109,
//      //������� ������� ���� ...   Y=0..8                                               // ���  06 04 2017
//       0/*���.*/, 0/*���.*/, 2/*Tzv_pn*/,  3/*Pmin_d ��������*/, 15/*Pmax_d ��������*/,
//   //110,      111,       112,           113,                 114,
//    1000/*UstDV*/,100/*UstTR*/,0/*J_jok*/,0/*ToNEDf*/,
//   //115,         116,        117,       118,
//      //������ ������� ���� ��������� � ������������ ������� Y=0..8                    // ���  06 04 2017
//     20/*Inmin,Y=0*/, 60/*Inmin,Y=1*/,100/*Inmin,Y=2*/,200/*Inmin,Y=3*/,300/*Inmin,Y=4*/,
//   //119,             120,            121,             122,             123,
//     500/*Inmin,Y=5*/,500/*Inmin,Y=6*/,1000/*Inmin,Y=7*/,999/*Inmin,Y=8*/,110/*���*/,
//   //124,             125,             126,             127,             128,
//      //������ ������� ���� ...   Y=0..8






/* 2) ��������� ����� �������� u16menu_orn_t, �� �������� ��� ����� �������:*/

//u16menu_orn_t fc[N_FCSET]={
//        				 { "koef uC", 	"k_uC",		"",	1000,  1, 1},
//    					 { "koef iA", 	"k_iA",		"",	1000,  1, 1},
//						 { "koef iB", 	"k_iB",		"",	1000,  1, 1},
//						 { "koef iC", 	"k_iC",		"",	1000,  1, 1}
//};
     /*  ���������� �������� ���������*/



fSet_t fSet= {
		123.2,//A	float amperageAclbr; 	// 0�0820
		0.021,//	float clbr_iA;			// 0�0822
		//
		123.2,//A	float amperageBclbr;	// 0�0824
		0.021,//	float clbr_iB;			// 0�0826
		//
		123.2,//A	float amperageCclbr;    // 0�0828
		0.021,//	float clbr_iC;	        // 0�082A
		//
		223.0,//V	float voltageCclbr;		// 0�082c
		4.021, //	float clbr_uC;			// 0�082e
		//
		//uint16_t DeviceMode;     //0x0830 ��������� ���������� 1...5
		1,
		//uint16_t Faznost;		     //0x0834
		3,

		0x08360837,
		//
		0x08380839,
		0x083A083B,

		0x083C083D,
		0x00000000//crc

};

/* �������� ������������ ������ �������������� �� ���������� ����
 * ���������� �������  */
bool  f_rated_amperage_test(float a){
	uint16_t typesize=fSet.DeviceMode;
	uint16_t min=ET_AmpSens[typesize].iMin;
	uint16_t max=ET_AmpSens[typesize].iMax;
	if (
			(a>((float)min/10.0f)) &&
		    (a<((float)max/10.0f))){
		return true;
	}
	else{
		return false;
	}
}



uint16_t *fcset =(uint16_t*)(fc32set);






flash_status_t f_VV300_FactorySets_Read(void){
	uint32_t size = sizeof(fSet_t);
	uint32_t *p32fset = NULL;
	p32fset=(uint32_t*)&fSet;
	return f_VV_Sets_Read(
			p32fset,
			size,
			FLASH_PAGE_FACILILY_SETS_A,
			FLASH_PAGE_FACILILY_SETS_B
			);
}
flash_status_t f_VV300_FactorySets_Write(void){
	uint32_t size = sizeof(fSet_t);
	uint32_t *p32fset = NULL;
	p32fset=(uint32_t*)&fSet;
	return f_VV_Sets_Write(
			p32fset,
			size,
			FLASH_PAGE_FACILILY_SETS_A,
			FLASH_PAGE_FACILILY_SETS_B
			);
}
