/*
 * tk3_grid_declarations.h
 *
 *  Created on: 27 ��. 2020 �.
 *      Author: KozakVF
 */

#ifndef TK_2_SUBPROJECTS_TK3_GRID_DECLARATIONS_H_
#define TK_2_SUBPROJECTS_TK3_GRID_DECLARATIONS_H_

#include "tkConfig.h"
typedef enum{
	Phase_unknown,
	Phase_A,
	Phase_B,
	Phase_C
}phase_t;
typedef enum{
	UnknownTypeLoad,
	ActiveLoad,
	InductiveLoad,
	CapacitiveLoad
} grid_load_t;

typedef enum{
	PhasesUnknownTypeRotation,
	PhasesDirectRotation,
	PhasesReverceRotation,
	PhasesSequenceFault,
	Phase_A_fault,
	Phase_B_fault,
	Phase_C_fault
}phases_rotation_type_t;

typedef struct{
	/* �������� ������� � ������
	 * https://ua.energy/wp-content/uploads/2019/07/SOU-Zabezpechennya-kontrolyu-i-dotrymannya-pokaznykiv-yakosti-e_e-pry-peredachi.pdf
	��������� ���������
	��������                    �� 49,8 �� 50,2 ��,
	�������� ��������� �������� �� 49,2 �� 50,8 ��        */

	hzf_t 		Frequency_Hz;
	/* �������� ������� � �������*/
	voltf_t 	Voltage_V;
	/* ������� ������ � ������� */
	amperf_t 	aAmperage_A;
	amperf_t 	bAmperage_A;
	amperf_t 	cAmperage_A;
	amperf_t	leakAmperage_A;
	/* ������� ������, �� ����� �� �����
 	 * ����� ��������� �� �����, ���������� �� ����� ����� ������,
 	 * �� ���� �� leakAmperage_A, ��� � ��������� �� ����������� ������*/
	procentf_t GrundAmperageDistortion;

	/* ������� ������ ���������� ����������� �����  */
	/* ��� ������������ - ����������� �� ������ */
	grid_load_t LoadType;
	phases_rotation_type_t PhaseRotation;
	/* ������� ��*/
	procentf_t PowerFactor_pro;
	/* ҳ���� ��� ��������������, ���� ������������ �� ������ - �� �� �����
	NEMA (National Electric Manufacturers Associations of the
	USA) Std. (1993) - ���������� ����������� �������:
	�������� ��������� �� ���������� ��������
	https://wp.kntu.ac.ir/tavakoli/pdf/Journal/j.ijepes.2010.12.003.pdf
	������ ��������� - ����� ������� ������� �������              */
	procentf_t UnbalanceDeratingFactor_pro;

    /* ���������� �����������, ���� 99%, �� ���� ������� ����������� �������� ����� ��������
     * ����� �� ���� ���� ��� ��������������  */
	procentf_t HarmonicsDeratingFactor_pro;

	/* ������� ���������� ��������� ����������� �����, ����� ���� ��� �������  */
	procentf_t HmUnbDerating_pro;
	procentf_t fullDerating_pro;

	/* ����i����� �������� ���,  �� ���� �� ���� ��� ��������������									               */
	procentf_t AmperageUnbalance;

	/* ��������� ����������������                                                  */
	kvatt_t	ActivePower_kW;
	kvatt_t	ReactivePower_kW;
	kvatt_t	UnbalanceLostPower_kW;//(���� ��� ��������������)
	kvatt_t	HarmonicLostPower_kW;

	/**********************************************************************************************/

	/*������� ��������� ����������� ����� 					                    */
	/*���������� �������� ���������� �� ������ � ����� ���                      */
	procentf_t aTotalHarmonicDistortion;
	procentf_t bTotalHarmonicDistortion;
	procentf_t cTotalHarmonicDistortion;

	/************************************************************************
	���������� �������� ���������� �� ������ ������������    				*/
	procentf_t THDi_HarmonicAmperageDistortion;
	/*THD� ����� �� ������ ����������� ����� �������� ������.
	THDi ����� 10% ��������� ����������,
	����� ������������� ���������� �������, ����������� �� 6%.

	THDi �� 10% � 50% ����� �� ������ ��������� �����������.
	������ ����������� ��������� �������������, ��������� �����������,
	�� ������, �� ����� �� ��������� ������ ���� ����������������.

	THDi, �� �������� 50%, ����� �� ����� �������� �����������.
	������� ����� ������ ���������� � ����. ���������� ����������� ����� ������
 	 � ������ � �� ��������, ������� �������������
 	 ������� ���������� ���������������� ������.

	���������� ������ ������ ����������:
	(��� ��������� ��������� ���������).
	���� ����������� ������� �������� ����������� � ������� 10%,
	����� ������ ���������� ������ �����������.
	������� �� ���� ��������, ���������� ������ ������:
	32,5% ��� ���������� ��������������
	18% ��� ��������� ��������������
	5% ��� ��������������.
 	 ***************************************************************************
	3, 5, 7, 11 and 13 - �������� ��������� ��� ��������������
 	 */
    /* 1,4,7,10 3k+1 - 䳺 � ���� � �������� ��������� ��� ������, ��� ������ ��������� �������� �������*/

	/* 2, 5, 8, 11   - ���� ���� ����� �������� ���������, �� ���������� �� ������� ��������� �������� �������*/

	/* ������, �� ��������� ������ � ���������� �������*/
	amperf_t aNegativeAmperage_A;
	amperf_t bNegativeAmperage_A;
	amperf_t cNegativeAmperage_A;

	/* 3, 6, 9 .. 3k+3 - ����� �� �������, �� ���������.
	 * ����� ��������� �� ����� �� ��������� �� ������.
	 * �� ����� �� ��������� �� ��������� ����������� ��������
      ������ �������� ������� ������ �� �������������,
      � ����������� ����������� � ���������� ���.*/
	/* ������, �� ����� �������� �������� �� ����� ��� � ��������,
	 * ��������� �������� */
	amperf_t aNeutralAmperage_A;
	amperf_t bNeutralAmperage_A;
	amperf_t cNeutralAmperage_A;
	amperf_t sumNeutralAmperage_A;
	/**/
	amperf_t leakFurie;
	phase_t leakFuriePhase;


	procentf_t MotorHeatProcentage;

	uint16_t mPOPUGAYS;
	uint16_t kPOPUGAYS;
}grid_t;



#endif /* TK_2_SUBPROJECTS_TK3_GRID_DECLARATIONS_H_ */
