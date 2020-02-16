/*
 * tkFreq.c
 *  Created on: 27 ����. 2018 �.
 *      Author: KozakVF
 */
#include "tkConfig.h"

#include "tk1_grid.h"
#include "math.h"
#if defined GRD_FURIE || defined FURIE_TEST

#include "stdint.h"
#include "gpio.h"
#include "tkSGL.h"
#include "adc.h"
#include "tim.h"
#include "float.h"
#include "tkFurie.h"
#include "wk_atime64.h"
#include "string.h"
#include "tk_wh1602b3.h"
#include "vkDeclarations.h"
#include "vkButtonsADC.h"

#include "wk_motor_ctrl.h"

#include "zFF.h"

#include "d0_debug_main.h"


extern void ADC_ConvInit(void);
//float ph[100]={0};
//float phd[100] = {0};
//uint16_t i_phd=0;

#define U_KOEFF0  0.1 /* �������� �� ����������� �.�.*/
/* ������������ ���������� */
user_settings_t us;
/* ������������ ��������� */
fSet_t fs;
/**********************************************************************************************
 * �������Ͳ �̲�Ͳ. �ղ�Ͳ ��� ������
 **********************************************************************************************/
volatile uint16_t peru16 = 2000;
/* ³���� ��� ��� ���'� */
volatile adcf_t uFurieC[N_FURIE+4]={0};
volatile adcf_t iFurieA[N_FURIE+4]={0};
volatile adcf_t iFurieB[N_FURIE+4]={0};
volatile adcf_t iFurieC[N_FURIE+4]={0};
volatile adcf_t iFurieLeak[N_FURIE+4]={0};

/* �������� ������ ��� ��������� ����� ������ �� �������
 * ��������� � ���� stm32f1_it.c (��� ����������� - mdGRD_test.�
 * ���������������� ���� � ����� ���� */
volatile adcu16_t iA[240]={0};
volatile adcu16_t iB[240]={0};
volatile adcu16_t iC[240]={0};
volatile adcu16_t iDf[240]={0};
volatile adcf_t   uC[240]={0};

volatile adcu16_t iLeak[240]={0};



/**********************************************************************************************
 * �������Ͳ �̲�Ͳ. ��ղ�Ͳ ��� ������
 **********************************************************************************************/


/* �������� ������� � ������ 	  */
volatile hzf_t 	Frequency 	= 50.0;
/* �������� ������� � �������*/
volatile voltf_t 	Voltage		=220.0;
/* ������� ������ � ������� */
volatile amperf_t 	aAmperage	=0.0;
volatile amperf_t 	bAmperage	=0.0;
volatile amperf_t 	cAmperage	=0.0;
volatile amperf_t 	leakAmperage	=0.0;
/* ������� ������ ���������� ����������� �����  */
procentf_t PowerFactor =100.0;
procentf_t UnbalanceDeratingFactor=100.0;
procentf_t HarmonicsDeratingFactor=100.0;

//procentf_t LeakDeratingFactor=100.0;
//procentf_t GrundDeratingFactor=100.0;
//procentf_t NeutralDeratingFactor=100.0;

/* ������� ���������� ��������� ����������� ����� */
procentf_t HmUnbDerating =100.0;
procentf_t fullDerating =100.0;
/* ��������� ����������������                               */
vatt_t	ActivePower 		= 	0;
vatt_t	ReactivePower 		=   0;
vatt_t  ReactivePowerInductiv=  0;
vatt_t  ReactivePowerCapacitive=0;
vatt_t	UnbalanceLostPower 	=	0;
vatt_t	HarmonicLostPower 	= 	0;
vatt_t	NeutralHarmonicLostPower 	= 	0;
vatt_t	NegativeHarmonicLostPower 	= 	0;
/**********************************************************************************************/

/*������� ��������� ����������� ����� 					*/
/*���������� �������� ���������� �� ������ � ����� ���  */
procentf_t aTotalHarmonicDistortion = 0;
procentf_t bTotalHarmonicDistortion = 0;
procentf_t cTotalHarmonicDistortion = 0;

/************************************************************************
���������� �������� ���������� �� ������ ������������    				*/
procentf_t THDi_HarmonicAmperageDistortion = 0;
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


/* ����i����� �������� ��� 									*/
volatile procentf_t AmperageUnbalance = 0;


/* ������, �� ��������� ������ � ���������� �������*/
static adcf_t aNegativeAmperage_adc=0;
static adcf_t bNegativeAmperage_adc=0;
static adcf_t cNegativeAmperage_adc=0;
static adcf_t sumNegativeAmperage_adc=0;
/* ������, �� ����� �������� �������� �� ����� ��� � ��������,
 * ��������� �������� */
static adcf_t aNeutralAmperage_adc=0;
static adcf_t bNeutralAmperage_adc=0;
static adcf_t cNeutralAmperage_adc=0;
static adcf_t sumNeutralAmperage_adc=0;
/*����� � �������� �� ���� � �������� (���������)
 * �������� � ������� ���� ���������� */
volatile static ifurie_out_t iAfurie_out = {0};
volatile static ifurie_out_t iBfurie_out = {0};
volatile static ifurie_out_t iCfurie_out = {0};
volatile ufurie_out_t uCfurie_out = {0};
/*����� � ��������, ���������� �� 200 ����� � ������� ���� */
volatile iheat_out_t  iAheat_out = {0};
volatile iheat_out_t  iBheat_out = {0};
volatile iheat_out_t  iCheat_out = {0};

/* ���������� ����� �������������� (���� �� �����������)
 * ���� ������� ��� ������� �� �������������� �� ������� �����
 * ������������ � f_GRID_Amperage_Calc()*/
float ShareOfRated_0_1=1.0f;	/* ����������� */
/* */
procentf_t procOfRated =100.0f;  	/* ���������  */
/* �� ������� �������� ����� �� ����������, � �� � �� �����������,
 * �� �������� ����, ���� ����������� �� ������� �������� ������ */
extern on_off_t MotorState;

volatile static ifurie_out_t iLEAKfurie_out = {0};
volatile static iheat_out_t  iLEAKheat_out = {0};


/* ������� �������� ���� ��� ��� ��� ������ */
volatile static adcf_t aAdc0Amperage=2048;
volatile static adcf_t bAdc0Amperage=2048;
volatile static adcf_t cAdc0Amperage=2048;
volatile static adcf_t leakAdc0Amperage=2048;

/* ������� ������ ������������������ ������ ��� � �������� */
volatile static adcf_t aAdcAmperage=0;  /* 362.0*/
volatile static adcf_t bAdcAmperage=0;
volatile static adcf_t cAdcAmperage=0;
/* ������ �������� ��������� ������ ������ � ��������       */
volatile static adcf_t aveAdcAmperage=0;
volatile static adcf_t leakAdcAmperage=0;
volatile static adcf_t grundAdcAmperage=0;
volatile static adcf_t neutralAdcAmperage=0;
volatile static adcf_t negativeAdcAmperage=0;



/* ������� ������������������ ������ ������� (�����) �������� ��� � �������� */
volatile static adcf_t aAdc1Amperage=0;  /* 362.0*/
volatile static adcf_t bAdc1Amperage=0;
volatile static adcf_t cAdc1Amperage=0;
volatile static adcf_t leakAdc1Amperage=0;
/*****************************************************************************/
/* ������� ���� � ��������                                                  */
static gradf_t u_i_Phi =0;
static gradf_t a_b_Phi =120.0;
static gradf_t b_c_Phi =120.0;
static gradf_t c_a_Phi =120.0;


/* �������, �������Ͳ �� ����� ��� */
/* ����������� ����� ������� � ������� ���  */
volatile  adcf_t ratedAdcAmperage =  423.8f;
/* ̳�������� �����, % �� ������������, ��� ����� ���������� ������
 * �������, �� ������ ������, ���� ��������� ����� ����� 7% �����������*/
volatile static procentf_t lLimitRms 	=   20.0f;
/* ���������� ���� � �������� ������������� � ������ ���
� ��� ������������ �������������� �������������� (���) �� ���*/

volatile static procentf_t LimitLeak_adc =  7.0f;

volatile static oper_mode_t grid_oper_mode=LowVoltageInductionMotorsProtection;


volatile static adcf_t rated220AdcVoltage = 547.1f;
/* ����������� ��� �������� ������� */
volatile static procentf_t lLimitVoltage =  60.0f;
volatile static procentf_t hLimitVoltage =  150.0f;
/* ����������� ��� ���� ������ �� �����
 * 1) � ��� ������������ �������������� �������������� (���)*/


/*********************************************************************************************
 * ���� ������� ��� ���������� ���������
 *********************************************************************************************/

/* ������� ���� ��� ��������� */

static uint32_t fr_counter=0;
static float oldPhase = 0;

static float PhaseDiff = 0;
volatile static float currentPhaseDiff =FLT_MIN;
/* ����� ��� �������� ���� ������� �����, � ���� � ������� Ҳ�7*/
 static uint16_t old_peru16=2000;
#ifdef		FREQUENCY_CALC_DEBUG0
/* ���� ��� ��������������/���������� ���������� ������� */
static volatile float phd[100]={0};
static volatile uint32_t i_phd=0;
static volatile float ph[100]={0};
static volatile uint32_t j_phd=0;
#endif
/* ���� ��� �������� ����/��������� ��� ������������� */
static uint32_t iclk=0;
static volatile uint32_t iDuration =0;

/* ���� ��� ������ ��������� �������� ��� ������������� */
uint32_t grd_tick =0;
uint32_t grd_ti =0;
volatile uint32_t grd_duration=0;
volatile uint32_t grd_mins=0;
volatile uint32_t phd_duration=0;
uint32_t grd_phd = 0;
//static uint32_t uclk=0;
//static uint32_t uDuration =0;
//volatile static irms[200]={0};
//volatile static iphase[200]={0};
//uint16_t i200 =0;
/******************************************************************************************
 * STATIC FUNCTIONS DECLARATIONS
 * ****************************************************************************************/
static grid_calc_status_t f_grid_frequency_calc(grid_t *hg);
static grid_calc_status_t f_grid_voltage_calc(grid_t *hg);
static grid_calc_status_t f_grid_amperage_calc(grid_t *hg);
static grid_calc_status_t f_grid_reactive_calc(grid_t *hg);
static grid_calc_status_t f_grid_leak_furie_calc(grid_t *hg);
static grid_calc_status_t f_grid_unbalance_factor_calc(
		adcf_t iAsgl, adcf_t iBsgl, adcf_t iCsgl,
		grid_t *hg);


static grid_calc_status_t f_grid_harmonics_calc(grid_t *hg);
static grid_calc_status_t f_grid_harmonics2_calc(grid_t *hg);
static grid_calc_status_t f_grid_power_calc(grid_t *hg);

static grid_calc_status_t f_grid_power_calc(grid_t *hg);

static grid_calc_status_t f_ThermalModel_calc(grid_t *hg);
static int16_t f_tim7_PeriodAutoTuning(hzf_t frequency);
static phases_rotation_type_t f_tk2_phase_rotation(
		volatile adcu16_t *p200a_array,
		volatile adcu16_t *p200b_array,
		volatile adcu16_t *p200c_array,
		uint16_t Nn
		);

/******************************************************************************************
 * GLOBAL FUNCTIONS DEFINITIONS
 * ****************************************************************************************/
int16_t f_GRD_Init(fSet_t *hfs, user_settings_t *hus){
	if((hfs==NULL)||(hus==NULL))return -1;
	fs.DeviceMode=hfs->DeviceMode;
	fs.Faznost   =hfs->Faznost;
	us.oper_mode =hus->oper_mode;
		/*��������� �� ������ � ������� ������� ������� */
	memset ((void*)&uFurieC,0,sizeof(uFurieC));
	memset ((void*)&iFurieC,0,sizeof(iFurieC));

	memset ((void*)&uC,0,sizeof(uC));
	memset ((void*)&iC,0,sizeof(iC));
	/* ���������� ������� �� ������������*/
	fs.clbr_uC = 0.896f*hfs->clbr_uC;	//4.021f;
	fs.clbr_iC = 1.209f*hfs->clbr_iC;  //0.021f
	/* ��������� ������� � �������� */
	if((fs.clbr_uC*U_KOEFF0)>0){
		rated220AdcVoltage=220.0f/(fs.clbr_uC*U_KOEFF0);
	}

	if(fs.Faznost==1){
		/* ���������� ����� � �������� ��� ��� ���������� �������  */
		if(fs.clbr_iC>0){
			ratedAdcAmperage=hus->rated_amperage_set/fs.clbr_iC;
		}

	}
	else if (fs.Faznost==2){}
	else if (fs.Faznost==3){
		/*��������� �� ������ � ������� ������� ������� */

		memset ((void*)&iFurieA,0,sizeof(iFurieA));
		memset ((void*)&iFurieB,0,sizeof(iFurieB));
		memset ((void*)&iFurieLeak,0,sizeof(iFurieLeak));

		memset ((void*)&iA,0,sizeof(iA));
		memset ((void*)&iB,0,sizeof(iB));
		memset ((void*)&iLeak,0,sizeof(iLeak));

		fs.clbr_iA = 1.209f*hfs->clbr_iA;	//0.021f;
		fs.clbr_iB = 1.209f*hfs->clbr_iB;	//0.021f;

		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			/* ���������� ����� � �������� ��� ��� ���������� �������  */
			if((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)>0){
				ratedAdcAmperage=hus->rated_amperage_set/((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)/3.0f);

			}
		}

	}
	else{}

	/* ��������� ���������, �� ���������������� ��� ��������� ������ �����
	 * ������� � ���� stm32f1xx_it.c*/
	ADC_ConvInit();
	/* ���������� ������ ��� ����� ����� */
	MX_TIM7_Init();
	/* ���������� �� ����-��� ������� ������������, ���� �� �������� ������/�������� ����� ���
	 * ��� ����������� � ����� ���������� ������� ������ 								*/
	f_uiFurie_Init();

	//float thermal_model_TimeConstant = 700;
	iclk=HAL_GetTick();
	return 0;
}



volatile uint32_t grid_event_status = 0x0;
//static uint32_t GRDCalcCntr=0;
grid_calc_status_t f_GRD_Calc(grid_t *hg){
//  GRDCalcCntr++;
    grid_calc_status_t gcs=GRID_clc_UnKnown;
    while(gcs==GRID_clc_UnKnown){

    	f_tim7_PeriodAutoTuning(Frequency);
        gcs=f_grid_frequency_calc(hg);
	    if(gcs != GRID_clc_OK){break;}
	    gcs=f_grid_voltage_calc(hg);
	    if(gcs != GRID_clc_OK){break;}
	    gcs=f_grid_amperage_calc(hg);
	    if(gcs != GRID_clc_OK){break;}

	    gcs=f_grid_reactive_calc(hg);
	    if(!((gcs == GRID_clc_OK) || (gcs == GRID_clc_Impossible))){break;}
	    if((fs.Faznost==3)&&(us.oper_mode ==LowVoltageInductionMotorsProtection)){
	    	gcs= f_grid_leak_furie_calc(hg);
	    }
	    if(fs.Faznost==3){
	    gcs=f_grid_unbalance_factor_calc(
	    		aAdcAmperage,bAdcAmperage,cAdcAmperage,
				hg
			);
	    }
	    if(!((gcs == GRID_clc_OK) || (gcs == GRID_clc_Impossible))){break;}

	    gcs=f_grid_harmonics_calc(hg);
	    if(!((gcs == GRID_clc_OK) || (gcs == GRID_clc_Impossible))){break;}
	    gcs=f_grid_harmonics2_calc(hg);
	    if(!((gcs == GRID_clc_OK) || (gcs == GRID_clc_Impossible))){break;}
	    gcs= f_grid_power_calc(hg);
	    if(gcs!=GRID_clc_OK){break;};
	    gcs= f_ThermalModel_calc(hg);


	    float Nh = 	(uint16_t)(1.0f/(Frequency*0.1f*0.001f));
	    uint8_t N=(uint8_t)Nh;
	    if((fs.Faznost==3)&&(us.oper_mode ==LowVoltageInductionMotorsProtection)){
	    	f_tk2_phase_rotation(
	    		iA,
				iB,
				iC,
	    		N
	    		);
	    }
	    /* ������� ����� */
	    memset ((void*)&uFurieC,0,sizeof(uFurieC));
	    memset ((void*)&iFurieA,0,sizeof(iFurieA));
	    memset ((void*)&iFurieB,0,sizeof(iFurieB));
	    memset ((void*)&iFurieC,0,sizeof(iFurieC));
	    memset ((void*)&iFurieLeak,0,sizeof(iFurieLeak));

	    memset ((void*)&uC,0,sizeof(uC));
	    memset ((void*)&iA,0,sizeof(iA));
	    memset ((void*)&iB,0,sizeof(iB));
	    memset ((void*)&iC,0,sizeof(iC));
	    memset ((void*)&iLeak,0,sizeof(iLeak));

    }
	return grid_event_status;
}

/******************************************************************************************
 * STATIC FUNCTIONS DEFINITIONS
 * ****************************************************************************************/

/* �� ������������ �����-������������ ��� ������ ����� ������� (18 �����),
 * ���� �
 * .rms - ������ ������������������� �������� �������  ��
 * .phase - ���� � �������� ������� �������� �������
 * ��������� ������� ����� ����������������.
 * � ��� ���� ��������� ������� ����� �����������������
 * > 54�� ��� < 46�� ������� ������� ����� �������*/

static grid_calc_status_t f_grid_frequency_calc(grid_t *hg){

	if(hg==NULL){return GRID_clc_ZERO_PTR;}
	grid_calc_status_t fc_status = GRID_clc_UnKnown;
	memset ((void*)&uCfurie_out,0,sizeof(uCfurie_out));
    /* ���������� ��� ��������� */
	f_ufurie(uFurieC, &uCfurie_out);
	/* �������� ���������� ������� ���� ������� ���� */
	if (fr_counter>20){
		/* ��������� �������� ���� 0..360 */
		if (uCfurie_out.u_phase<0){uCfurie_out.u_phase = uCfurie_out.u_phase +360.0;}
		/*������ ���� ������� �� ���������� �� �'��� ������ �������   */
		currentPhaseDiff=uCfurie_out.u_phase-oldPhase;
		/*��������� ������ ���� -180..+180 � 0..360  */
		if (currentPhaseDiff>180.0f){currentPhaseDiff-=360.0f;}
		if (currentPhaseDiff<-180.0f){currentPhaseDiff+=360.0f;}
#ifdef		FREQUENCY_CALC_DEBUG0
        /*******************************************************/
		/*   ²�����������                                     */
		/*******************************************************/
			ph[i_phd]=uCfurie_out.u_phase;
			phd[i_phd]=currentPhaseDiff;
			/* ������������������� ��������� */
			i_phd++;
			j_phd++;
			if ((i_phd%100)==0){
				i_phd=0;
				if ((j_phd%200)==0){
					j_phd=0;
//					HAL_Delay(10);
					/* ���������� ��������������������� ��������� */
					float sum_phdxphd=0;
					for (uint8_t i=0;i<100;i++){
						float dDiff=PhaseDiff-phd[i_phd];
						sum_phdxphd+=dDiff*dDiff;
					} /* 0.5 ... 0.8 ����.*/
					volatile float diffStandardDeviation = sqrtf(sum_phdxphd/100);
					/**/
					phd_duration=HAL_GetTick()-grd_phd;
					grd_phd = HAL_GetTick();
				}
				memset ((void*)&ph,0,sizeof(ph));
				memset ((void*)&phd,0,sizeof(phd));
			}
	    /*******************************************************/
		/*   ʲ���� ²�����������                                     */
		/*******************************************************/
#endif //FREQUENCY_CALC_DEBUG0
		/* ���� ������ ��� ����� ������ - �� ���������, ����������  	   */
		if(fabsf(currentPhaseDiff)<29.0f){
			/*(100.0+newPhaseDiff*100)/360.0) ��������� � ���������� �'��� ������ ������� */
			float newPhaseDiff =0;
			/* ���������..*/
			f_ExponentialSmoothing  (
							currentPhaseDiff,
							PhaseDiff,
							20.0,
						   	10.0,
							&newPhaseDiff
						   	);
			PhaseDiff=newPhaseDiff;
			float flpd=fabsf(PhaseDiff);
				if(flpd<36.0f){/* ���� ��������� - 100��, ����� �'��� ����� ������� ������ ����� ����������������,
				                  ���������� ������ ����� � ���� �������� ��������� � ����� +- 72����.
				                  ������ ��������*/
					float oneperiodPhaseDiff = PhaseDiff/5.0f;
					float jhy = (360.0f+oneperiodPhaseDiff)/360.0f;
					Frequency = 50.0f * jhy;

					fc_status=GRID_OK;
#ifdef FREQUENCY_CALC_PRINT0
	if(print_debug0==teFREQUENCY_CALC_PRINT0){
					int8_t str[17]={0};
					int ic = 0;
					ic=(uint32_t)(Voltage*10.0f);
					snprintf((char*)str,17,  "u= %03d.%03d ", ic/10, ic%10);
					tkDisplay_PutHighStr(str);
					memset(str,0,sizeof(str));
					ic=(uint32_t)(Frequency*1000.0f);
					snprintf((char*)str,17,  "f= %03d.%03d ", ic/1000, ic%1000);
					tkDisplay_PutLowStr(str);
	}
#endif
			}
			else if (PhaseDiff>=36){/*45hz=36grad*/
				/* ����� �������   */
				Frequency=45.0f;
				fc_status=GRID_clc_FreqUnderLimit;
			}
			else if (PhaseDiff<=-36){/*55hz = 36grad*/
				Frequency=55.0f;
				fc_status=GRID_clc_FreqOverLimit;
			}
			else{}
			hg->Frequency_Hz=Frequency;
			fc_status=GRID_clc_OK;
		}

	}
	else{
		fc_status=GRID_clc_OK;
	}
	fr_counter++;
	oldPhase=uCfurie_out.u_phase;
	iDuration=HAL_GetTick()-iclk;
	return fc_status;
}

/* �� ������� �������� ������� � ������� , ������������ ��������
 * ��� ������� � ������������ ���'�
 * �� ���������� ������� �� ������ ������� � ������� */
static grid_calc_status_t f_grid_voltage_calc(grid_t *hg){
	/* �� �������� �������� 1-� �������� �������.
	 * �������, �� �������� 1-� �������� ������� ����������� ������������������� ������ */
	voltf_t iVoltage = uCfurie_out.u1harm_ampl*fs.clbr_uC*U_KOEFF0;
	voltf_t newFlatVoltage = 0;
	f_ExponentialSmoothing  (
							iVoltage,
							Voltage,
			    			20.0,
			   				10.0,
							&newFlatVoltage
			   				);


	Voltage = newFlatVoltage;
	hg->Voltage_V=Voltage;
	return GRID_OK;
}

/* �� �������:
   1) �������� ������ ����� ��� ������ iAfurie_out, i�furie_out, i�furie_out (�� 18 �����)
		  ����� iFurieLeak ������������ �� ��������� ���� ����� ��� ����� ������ ������
		  ֳ 4 ������ ������������ � ���.������������ ���'�, �� ����������:
		  ��������� iAfurie_out, iBfurie_out, iCfurie_out, iLeakfurie_out,
   1.3)	  ����� ��������� ������:
		rms_fundamental -    			root_mean-square, ����� ����� ������� ��������, ����� ���
	    gradf_t phase_fundamental-   	���� ������� ��������,  �������
		adcf_t	rmsZero-			 	����� ����� ������ �������, ������� �����
	    adcf_t  rmsNegative-            ����� ����� �������, ������� 3*�+2
   2) �������� ������ ����� ��� ������ ��, ��, ��, �Leak (�� 200..210 ����� ����� 100 ���, ������� �� �������
	  ���������� rms ����� ���� �������� ������������ ������
   3) ������ ��� ������ ��� ���
   4) �������� ������ rms, ������� ������ ������, � ��������*/
static grid_calc_status_t f_grid_amperage_calc(grid_t *hg){
	/* ���������� ������ ����� 18 */
//	grid_calc_status_t	ac_status=GRID_clc_UnKnown;


	/* ���������� ����� ����� 18 ��� ���� ����   */
	memset ((void*)(&iCfurie_out),0,sizeof(iCfurie_out));
	f_ifurie(	iFurieC,   &iCfurie_out);

	/* ���������� ����� ����� 200 ��� ���� ����  */
	/* ����� �������� �����                        */
	float Nh = 	(uint16_t)(1.0f/(Frequency*0.1f*0.001f));
	memset ((void*)(&iCheat_out),0,sizeof(iCheat_out));
	f_iheat(	iC,
				cAdc0Amperage,
				Nh,
				&iCheat_out);
	/* �������� ��� ������ � �������� ���  ���  C */
	float newSglPermComp=FLT_MIN;
	newSglPermComp=0;
	f_ExponentialSmoothing  (
								iCheat_out.permanent_component,
					   			cAdc0Amperage,
					    		20.0f,
					   			10.0f,
								&newSglPermComp
					   			);
	cAdc0Amperage=newSglPermComp;

	/* ���������� ������� ������ �������/����� �������� � � �������
	 * ������� - ��������� ����������� ������� ��� 18 � 200*/
	float new1Amperage=FLT_MIN;
	/* �������� ������ ����� �������� ���� ���� � ��������*/
	f_ExponentialSmoothing  (
							iCfurie_out.rms_fundamental,
				   			cAdc1Amperage,
				    		2.0f,
				   			10.0f,
							&new1Amperage
				   			);
	cAdc1Amperage=new1Amperage;
	/* �������� ������ ������ ���� ���� � �������� */
	float newAmperage=0;
	f_ExponentialSmoothing  (
								iCheat_out.rms_heat,
					   			cAdcAmperage,
					    		2.0f,
					   			10.0f,
								&newAmperage
					   			);
	cAdcAmperage=newAmperage;

	/* �������� ����� ���� ���� � ������� */
	cAmperage	= fs.clbr_iC*cAdcAmperage;
	hg->cAmperage_A=cAmperage;
	hg->mPOPUGAYS=(uint16_t)(cAdcAmperage*1.41f);
	hg->kPOPUGAYS=(uint16_t)(cAdc1Amperage*1.41f);
	if(fs.Faznost==1){
		/* ���������� ������ ��������� ��� ����������� �������������� */
		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			/* ���������� ���������� ����� - ������ �� ������������ ��� ������� ����� */
			ShareOfRated_0_1 = iAheat_out.rms_heat / ratedAdcAmperage;
			procOfRated=(cAdcAmperage/ratedAdcAmperage)*100.0f;
		}
	}


	if(fs.Faznost==3){
		/* ���������� ����� ������ 18 ��� ��� � � �     */
		memset ((void*)(&iAfurie_out),0,sizeof(iAfurie_out));
		f_ifurie(	iFurieA,   &iAfurie_out);

		memset ((void*)(&iBfurie_out),0,sizeof(iBfurie_out));
		f_ifurie(	iFurieB,   &iBfurie_out);
		/* ���������� ����� ������ 200 ��� ��� � � �*/
		memset ((void*)(&iAheat_out),0,sizeof(iAheat_out));
		f_iheat(	iA,
					aAdc0Amperage,
					Nh,
					&iAheat_out);

		memset ((void*)(&iBheat_out),0,sizeof(iBheat_out));
		f_iheat(	iB,
				    bAdc0Amperage,
					Nh,
					&iBheat_out);
		/* �������� ��� ������ � �������� ���  ���  A & B*/
		float newSglPermComp=FLT_MIN;
		f_ExponentialSmoothing  (
									iAheat_out.permanent_component,
						   			aAdc0Amperage,
						    		20.0f,
						   			10.0f,
									&newSglPermComp
						   			);
		aAdc0Amperage=newSglPermComp;
		newSglPermComp=0;
		f_ExponentialSmoothing  (
									iBheat_out.permanent_component,
						   			bAdc0Amperage,
						    		20.0f,
						   			10.0f,
									&newSglPermComp
						   			);
		bAdc0Amperage=newSglPermComp;
		/* ���������� ������� ������ �������/����� �������� � � �������
		 * ������� - ��������� ����������� ������� ��� 18 � 200*/
		float new1Amperage=FLT_MIN;
		f_ExponentialSmoothing  (
								iAfurie_out.rms_fundamental,
					   			aAdc1Amperage,
					    		2.0f,
					   			10.0f,
								&new1Amperage
					   			);
		aAdc1Amperage=new1Amperage;
		f_ExponentialSmoothing  (
								iBfurie_out.rms_fundamental,
					   			bAdc1Amperage,
					    		2.0f,
					   			10.0f,
								&new1Amperage
					   			);
		bAdc1Amperage=new1Amperage;
		/* ���������� ������� ������ ������ � �������� */
		float newAmperage=FLT_MIN;
		f_ExponentialSmoothing  (
									iAheat_out.rms_heat,
						   			aAdcAmperage,
						    		2.0f,
						   			10.0f,
									&newAmperage
						   			);
		aAdcAmperage=newAmperage;

		newAmperage=0;
		f_ExponentialSmoothing  (
									iBheat_out.rms_heat,
						   			bAdcAmperage,
						    		2.0f,
						   			10.0f,
									&newAmperage
						   			);
		bAdcAmperage=newAmperage;

		aAmperage	= fs.clbr_iA*aAdcAmperage;
		bAmperage	= fs.clbr_iB*bAdcAmperage;
		hg->aAmperage_A=aAmperage;
		hg->bAmperage_A=bAmperage;

		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			/* ���������� ���������� ����� - ������ �� ������������ ��� ������� �����*/
			ShareOfRated_0_1 = ((iAheat_out.rms_heat+iBheat_out.rms_heat+iCheat_out.rms_heat) / 3.0f) / ratedAdcAmperage;

			procOfRated=(((aAdcAmperage+bAdcAmperage+cAdcAmperage)/3.0f)/ratedAdcAmperage)*100.0f;
			memset ((void*)(&iLEAKheat_out),0,sizeof(iLEAKheat_out));
			/* ���������� ����� �������� ��������� ������, �� ����� �� �����,
			 *  �� ���� ������� ������� ������ ������ ���*/
			/* ���������� ����� �������� ��������� ������, �� ����� �� �����,
			 *  �� ���� ������� ������� ������ ������ ���*/
			memset ((void*)(&iLeak),0,sizeof(iLeak));
			/* ������� ����� ���� ����� 200 */
			for (uint8_t i=0;i<240;i++){
				float iL=   ((float)iA[i]-aAdc0Amperage)
						    +((float)iB[i]-bAdc0Amperage)
							+((float)iC[i]-cAdc0Amperage);

				iLeak[i]= (uint16_t)iL+2048;
			}
			/* ���������� ����� ���� ����� 200, ����� ������ ������  */
			f_iheat(
				iLeak,
				2048.0f,
				Nh,
				&iLEAKheat_out);
			/* ���������� ��������� �������� ����� ������ */
			float newAmperage=0;
			f_ExponentialSmoothing  (
				iLEAKheat_out.rms_heat,
				leakAdcAmperage,
				2.0f,
				10.0f,
				&newAmperage
			);
			leakAdcAmperage=newAmperage;
			/* ���������� ����� ������ � �������								 */
			leakAmperage= ((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)/3.0f)*leakAdcAmperage;
		}
		hg->leakAmperage_A=leakAmperage;
		if(Voltage>lLimitRms*220.0f/100.0f){;
			float ave_adc_amperage=(aAdcAmperage+bAdcAmperage+cAdcAmperage)/3.0f;
			/* ����� �� ����� ��������   */
			if(ave_adc_amperage>lLimitRms*ratedAdcAmperage/100.0f){
				hg->GrundAmperageDistortion=100.0f*leakAdcAmperage/ave_adc_amperage;
			}
			else{hg->GrundAmperageDistortion=0.0f;}
		}
	}



#ifdef	AMPERAGE_FURIE_VS_EUGENYT0
	if((GRDCalcCntr % 10)==0){
		int8_t str[17]={0};
		int ic = 0;
		if(print_debug0==teADC_FURIE_VS_EUGENYT0){
			ic=(uint32_t)(iCheat_out.rms_heat*1000.0f);
			snprintf((char*)str,17,  "ET %03d.%03d ���", ic/1000, ic%1000);
	    	tkDisplay_PutHighStr(str);
			memset(&str,0,sizeof(str));
			ic=(uint32_t)(iCfurie_out.rms_fundamental*1000.0f);
			snprintf((char*)str,17,  "VK %03d.%03d a��", ic/1000, ic%1000);
			tkDisplay_PutLowStr(str);
		}
		if(print_debug0==teAMPERAGE_FURIE_VS_EUGENYT0){
			ic=(uint32_t)(iCheat_out.rms_heat*fs.clbr_iC*1000.0f);
			snprintf((char*)str,17,  "ET %03d.%03d A", ic/1000, ic%1000);
	    	tkDisplay_PutHighStr(str);
			memset(&str,0,sizeof(str));
			ic=(uint32_t)(iCfurie_out.rms_fundamental*fs.clbr_iC*1000.0f);
			snprintf((char*)str,17,  "VK %03d.%03d A", ic/1000, ic%1000);
			tkDisplay_PutLowStr(str);
		}

	}
#endif

	return GRID_OK;

}


//static grid_calc_status_t f_grid_leak_200_calc(grid_t *hg){
//	return GRID_OK;
//}
phases_rotation_type_t 	phrotation		= PhasesUnknownTypeRotation;
//#define U_I_PHI_DEBUG
#ifdef  U_I_PHI_DEBUG
	float u_ph[100]	= {0};
	float i_ph[100]	= {0};
	float ui_ph[100]  = {0};
	uint16_t i_phd  = 0;
#endif //U_I_PHI_DEBUG
static grid_calc_status_t f_grid_reactive_calc(grid_t *hg){
	grid_calc_status_t 		grc_status 		= GRID_clc_UnKnown;
	grid_load_t 			gloadtype		= UnknownTypeLoad;
	phases_rotation_type_t 	phrotation		= PhasesUnknownTypeRotation;

	while(grc_status==GRID_clc_UnKnown){
		/* ����� ��� ��������, ���� �������� ������� � ������ �� ����� ������� */
		bool ub=(uCfurie_out.u1harm_ampl>(lLimitRms*rated220AdcVoltage/100.0f));
		/* �������� ����� �������� ������ ���� � �� ����� ������   */
		bool ib=(iCfurie_out.rms_fundamental>(lLimitRms*ratedAdcAmperage/100.0f));
		bool ui=ub&&ib;
		if (!ui){
			PowerFactor=100.0f;

			grc_status=GRID_clc_Impossible;

			break;
		}
		/* ���� -180..+180 ���� ������������ � ���� 0..360 ����*/
		if (uCfurie_out.u_phase<0){uCfurie_out.u_phase=uCfurie_out.u_phase+360.0;}
		if (iCfurie_out.phase_fundamental<0){iCfurie_out.phase_fundamental=iCfurie_out.phase_fundamental+360.0;}
		gradf_t deltaPHI_iCuC=0;
		deltaPHI_iCuC=uCfurie_out.u_phase-iCfurie_out.phase_fundamental;
		if (deltaPHI_iCuC< -180.0){deltaPHI_iCuC=deltaPHI_iCuC+360.0;}
		if (deltaPHI_iCuC>  180.0){deltaPHI_iCuC=deltaPHI_iCuC-360.0;}
		procentf_t new_u_i_Phi=FLT_MIN;
		f_ExponentialSmoothing  (
						deltaPHI_iCuC,
						u_i_Phi,
		    			20.0f,
		   				01.0f,
						&new_u_i_Phi
		   				);
		u_i_Phi=new_u_i_Phi;


		if(fabs(u_i_Phi)>5.0f){
			gloadtype=ActiveLoad;
		}
		else if(u_i_Phi>=5.0f){
			gloadtype=InductiveLoad;
		}
		else if(u_i_Phi<=-5.0f){
			gloadtype=CapacitiveLoad;
		}
		else{}
		PowerFactor = 100.0f*cosf(M_PI * u_i_Phi/180.0f);
		hg->LoadType		= gloadtype;
		hg->PowerFactor_pro = PowerFactor;


		/* ��� ����������� ������ � �������������� ���������� ����������� ��� */
		if((fs.Faznost==3)&&(us.oper_mode ==LowVoltageInductionMotorsProtection)){

			if(iAfurie_out.rms_fundamental < (lLimitRms*ratedAdcAmperage/100.0f)){
				phrotation=Phase_A_fault;break;
			}
			else if(iBfurie_out.rms_fundamental < (lLimitRms*ratedAdcAmperage/100.0f)){
				phrotation=Phase_B_fault;break;
			}
			else{}
			/* ���� -180..+180 ���� ������������ � ���� 0..360 ����*/

			if (iAfurie_out.phase_fundamental<0){iAfurie_out.phase_fundamental=iAfurie_out.phase_fundamental+360.0;}
			if (iBfurie_out.phase_fundamental<0){iBfurie_out.phase_fundamental=iBfurie_out.phase_fundamental+360.0;}

			gradf_t deltaPHI_ab = 0;
			deltaPHI_ab=iAfurie_out.phase_fundamental-iBfurie_out.phase_fundamental;
			if (deltaPHI_ab< -180.0){deltaPHI_ab=deltaPHI_ab+360.0;}
			if (deltaPHI_ab>  180.0){deltaPHI_ab=deltaPHI_ab-360.0;}

			gradf_t new_deltaPHI=FLT_MIN;
			new_deltaPHI=0;
			f_ExponentialSmoothing  (
								deltaPHI_ab,
								a_b_Phi,
				    			20.0f,
				   				10.0f,
								&new_deltaPHI
				   				);
			a_b_Phi=new_deltaPHI;

			gradf_t deltaPHI_bc = 0;
			deltaPHI_bc=iBfurie_out.phase_fundamental-iCfurie_out.phase_fundamental;
			if (deltaPHI_bc< -180.0){deltaPHI_bc=deltaPHI_bc+360.0;}
			if (deltaPHI_bc>  180.0){deltaPHI_bc=deltaPHI_bc-360.0;}

			new_deltaPHI=0;
			f_ExponentialSmoothing  (
								deltaPHI_bc,
								b_c_Phi,
				    			20.0f,
				   				10.0f,
								&new_deltaPHI
				   				);
			b_c_Phi=new_deltaPHI;


			gradf_t deltaPHI_ca = 0;
			deltaPHI_ca=iCfurie_out.phase_fundamental-iAfurie_out.phase_fundamental;
			if (deltaPHI_ca< -180.0){deltaPHI_ca=deltaPHI_ca+360.0;}
			if (deltaPHI_ca>  180.0){deltaPHI_ca=deltaPHI_ca-360.0;}

			new_deltaPHI=0;
			f_ExponentialSmoothing  (
								deltaPHI_ca,
								c_a_Phi,
				    			20.0f,
				   				10.0f,
								&new_deltaPHI
				   				);
			c_a_Phi=new_deltaPHI;

			if ((a_b_Phi>0)||(b_c_Phi>0)||(c_a_Phi>0)){
				phrotation		= PhasesDirectRotation;
			}
			else if ((a_b_Phi<0)||(b_c_Phi<0)||(c_a_Phi<0)){
				phrotation		= PhasesReverceRotation;
			}
			else{
				phrotation=PhasesSequenceFault;
			}
			hg->PhaseRotation	= phrotation;
		}

#ifdef U_I_PHI_DEBUG
				u_ph[i_phd]=uCfurie_out.u_phase;
				i_ph[i_phd] = iCfurie_out.phase_fundamental;
				ui_ph[i_phd]=deltaPHI_iCuC;
				i_phd++;


				if ((i_phd % 100)==0){
					HAL_Delay(10);
					i_phd=0;
				}
#endif



#ifdef COS_FI_CALC_PRINT0
	if(print_debug0==teCOS_FI_CALC_PRINT0){
		int8_t str[17]={0};
		snprintf((char*)str,17,  "�Uc=%03d �Ic=%03d", (int)uCfurie_out.u_phase, (int)iCfurie_out.phase_fundamental);
		tkDisplay_PutHighStr(str);
		memset(&str,0,sizeof(str));
		snprintf((char*)str,17,  "Cos=%03d Fi =%03d", (uint16_t)PowerFactor, (uint16_t)u_i_Phi);
		tkDisplay_PutLowStr(str);
	}
#endif //COS_FI_CALC_PRINT0
		grc_status=GRID_OK;
	}
	return grc_status;

}
static float ave_leek_adc=0;
float leak_ui_Phi = 0;
phase_t leakPhase = Phase_unknown;
static grid_calc_status_t f_grid_leak_furie_calc(grid_t *hg){
	memset ((void*)(&iFurieLeak),0,sizeof(iFurieLeak));

	/* ����������������� �� �� ���������� ���������� ���� �����*/
	for (uint8_t i=0;i<N_FURIE;i++){
		iFurieLeak[i]=  (iFurieA[i]-aAdc0Amperage)
				    	    +(iFurieB[i]-bAdc0Amperage)
							   +(iFurieC[i]-cAdc0Amperage);

	} //DIFFERNCIAL_CURRENT_TRANSFORMER
	/* �������� ������������������� �������� ������ ������
	 * ����� �������� */
	f_ifurie(iFurieLeak,&iLEAKfurie_out);
	float new_leak_adc=0;
	/* ��������� ����� ������ */
	f_ExponentialSmoothing  (
							iLEAKfurie_out.rms_fundamental,
							ave_leek_adc,
			    			20.0f,
			   				10.0f,
							&new_leak_adc
			   				);
	/* ��������� ����� ������ */
	ave_leek_adc=new_leak_adc;
	/* ���� �������� ����� ������, ������ �� ������ �������,
	 * ��������� ����� �� 1% �� ����������� ������� ��������������
	 * ��� ���� ������� �� �����*/
	/* ��� �� ������� ����� ����� ����, �� ����� �� �����
	 * ����� - �� ������ - ��� ���� ����?
	 * � ��� � ����, ��� ���� ����� ����� �������� �������, ���
	 * ���� �� ����� ������� ���� �������, ���� �������� �� 120 ����*� + ��
	 * ��� ����� �������� ���� ���� �� ������� ������ � ����� �*/
	if (ave_leek_adc > ratedAdcAmperage/100.0f){
		float i_leak_ui_Phi = uCfurie_out.u_phase-iLEAKfurie_out.phase_fundamental;
		if (i_leak_ui_Phi< -180.0f){i_leak_ui_Phi=i_leak_ui_Phi+360.0f;}
		if (i_leak_ui_Phi>  180.0f){i_leak_ui_Phi=i_leak_ui_Phi-360.0f;}
		float new_uleak_phi=0;
		/* �������� ���� ��� ���������� ������ ������ �� ���� �*/
		f_ExponentialSmoothing  (
							i_leak_ui_Phi,
							leak_ui_Phi,
			    			20.0f,
			   				10.0f,
							&new_uleak_phi
			   				);
		leak_ui_Phi=new_uleak_phi;
		/* ���� ���� �� ����� ������ ������ � ����� ������ ���� � - �� ���� � */
		float leakcos_shift = leak_ui_Phi-u_i_Phi;
		if (leakcos_shift< -180.0f){leakcos_shift=leakcos_shift+360.0f;}
		if (leakcos_shift>  180.0f){leakcos_shift=leakcos_shift-360.0f;}
		float leakcos_shift_m120 = leakcos_shift-120.0f;
		if (leakcos_shift_m120< -180.0f){leakcos_shift_m120=leakcos_shift_m120+360.0f;}
		if (leakcos_shift_m120>  180.0f){leakcos_shift_m120=leakcos_shift_m120-360.0f;}
		float leakcos_shift_p120 = leakcos_shift+120.0f;
		if (leakcos_shift_p120< -180.0f){leakcos_shift_p120=leakcos_shift_p120+360.0f;}
		if (leakcos_shift_p120>  180.0f){leakcos_shift_p120=leakcos_shift_p120-360.0f;}
		if (fabsf(leakcos_shift) > 30){
			leakPhase=Phase_C;
		}
		else if (fabsf(leakcos_shift_p120) > 30){
			if(phrotation		== PhasesDirectRotation){
				leakPhase=Phase_A;
			}
			else if (phrotation		== PhasesReverceRotation){
				leakPhase=Phase_B;
			}
			else{
				leakPhase=Phase_unknown;
			}
		}
		else if (fabsf(leakcos_shift_p120) > 30){
			if(phrotation		== PhasesDirectRotation){
				leakPhase=Phase_B;
			}
			else if (phrotation		== PhasesReverceRotation){
				leakPhase=Phase_A;
			}
			else{
				leakPhase=Phase_unknown;
			}
		}
		else{
			leakPhase=Phase_unknown;
		}

	}
	hg->leakFurie=ave_leek_adc*((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)/3.0f);
	hg->leakFuriePhase=leakPhase;


}
static grid_calc_status_t f_grid_unbalance_factor_calc(
		adcf_t iAsgl, adcf_t iBsgl, adcf_t iCsgl,
		grid_t *hg){
	grid_calc_status_t ubf_status=GRID_clc_UnKnown;
	while(ubf_status==GRID_clc_UnKnown){
		if(procOfRated<lLimitRms){
			UnbalanceDeratingFactor=100.0f;
			AmperageUnbalance=0;
			ubf_status=GRID_clc_Impossible;
			break;
		}

		float out_factor=0;
		/*NEMA (National Electric Manufacturers Associations of the
		USA) Std. (1993) - ���������� ����������� �������:
		�������� ��������� �� ���������� ��������
		https://wp.kntu.ac.ir/tavakoli/pdf/Journal/j.ijepes.2010.12.003.pdf*/
		float iAv = (iAsgl+iBsgl+iCsgl)/3.0;
		float idmax= 0;
		idmax=fabsf(iAsgl-iAv);
		if (fabsf(iBsgl-iAv)>idmax){
			idmax=fabsf(iBsgl-iAv);
		}
		if (fabsf(iCsgl-iAv)>idmax){
			idmax=fabsf(iCsgl-iAv);
		}
		AmperageUnbalance=(idmax/iAv)*100.0;
		hg->AmperageUnbalance=AmperageUnbalance;
		hg->UnbalanceDeratingFactor_pro=100.0f;
		/* ��� ���������� �������������� 					*/
		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			procentf_t voltage_unbalance=AmperageUnbalance/7.0;
			if (voltage_unbalance<=10.0){
				if ((voltage_unbalance >= 0) && (voltage_unbalance <= 0.5)){
					out_factor=1.0;
				}
				else if ((voltage_unbalance > 0.5) && (voltage_unbalance <= 1.0)){
					out_factor=1.0-0.02*(voltage_unbalance-0.5);
				}
				else if ((voltage_unbalance > 1.0) && (voltage_unbalance <= 2.0)){
					out_factor=0.99-0.03*(voltage_unbalance-1.0);
				}
				else if ((voltage_unbalance > 2.0) && (voltage_unbalance <=10.0)){
					out_factor=0.96-0.07*(voltage_unbalance-2.0);
				}
				else{out_factor=0;}
				UnbalanceDeratingFactor=100.0*out_factor;
				ubf_status = GRID_clc_OK;
			}
			else {
				out_factor=0.96-0.07*(voltage_unbalance-2.0);
				UnbalanceDeratingFactor = 100.0*out_factor;
			}
			hg->UnbalanceDeratingFactor_pro=UnbalanceDeratingFactor;

		}
		ubf_status=GRID_clc_OK;
	}
#ifdef	AMPERAGE_UNBALANCE_PRINT0
	if(print_debug0==teAMPERAGE_UNBALANCE_PRINT0){
		int8_t str[17]={0};
		snprintf((char*)str,17,  "%01d.%02d %02d.%02d %01d.%02dA",
			(int)(aAmperage*100.0f) /100, (int)(aAmperage*100.0f) %100,
			(int)(bAmperage*100.0f) /100, (int)(bAmperage*100.0f) %100,
			(int)(cAmperage*100.0f) /100, (int)(cAmperage*100.0f) %100);
		tkDisplay_PutHighStr(str);

		memset(&str,0,sizeof(str));
		snprintf((char*)str,17,  "A���=%02d%% Dr=%02d%%", (uint16_t)AmperageUnbalance, (uint16_t)UnbalanceDeratingFactor);
		tkDisplay_PutLowStr(str);
	}
#endif

	return ubf_status;
}

static grid_calc_status_t f_grid_harmonics_calc(grid_t *hg){
	grid_calc_status_t hfc=GRID_clc_UnKnown;
	while(hfc==GRID_clc_UnKnown){
		/*********************************************************/
		float cD2 =   (cAdcAmperage*cAdcAmperage)
									   -(cAdc1Amperage*cAdc1Amperage);
		if (cD2<=0){cD2=0;}
		cTotalHarmonicDistortion=
				   100.0f*M_SQRT1_2*sqrtf(cD2)/cAdc1Amperage;
		hg->cTotalHarmonicDistortion=cTotalHarmonicDistortion*0.85f;
	    hg->THDi_HarmonicAmperageDistortion=THDi_HarmonicAmperageDistortion*0.85f;
		/*********************************************************/
	    float THDi=0;
	    if (fs.Faznost==1){
			THDi_HarmonicAmperageDistortion=cTotalHarmonicDistortion;
			hg->THDi_HarmonicAmperageDistortion=THDi_HarmonicAmperageDistortion*0.85f;
			/* ��� ����������� �������������� */
			THDi=cTotalHarmonicDistortion;

	    }
	    else if (fs.Faznost==3){
	 	   float aD2 =   (aAdcAmperage*aAdcAmperage)
	 								   -(aAdc1Amperage*aAdc1Amperage);
	 	   /*THD -  ����������� ��������� */
	 	   if (aD2<0){aD2=0;}
	 	   aTotalHarmonicDistortion=
	 			   100.0f*M_SQRT1_2*sqrtf(aD2)/aAdc1Amperage;
	 	   /*********************************************************/
	 	   float bD2 =   (bAdcAmperage*bAdcAmperage)
	 								   -(bAdc1Amperage*bAdc1Amperage);
	 	   if (bD2<0){bD2=0;}
	 	   bTotalHarmonicDistortion=
	 			   100.0f*M_SQRT1_2*sqrtf(bD2)/bAdc1Amperage;

	 	   hg->aTotalHarmonicDistortion=aTotalHarmonicDistortion*0.85f;
	 	   hg->bTotalHarmonicDistortion=bTotalHarmonicDistortion*0.85f;
	    }
	    else{}



//		/* ��������� ����� ������ ����� �������� */
//		f_ExponentialSmoothing  (
//								iLEAKfurie_out.rms_fundamental,
//								leakAdc1Amperage,
//						    	20.0,
//						   		10.0,
//								&new1Amperage
//						   		);
//		leakAdc1Amperage=new1Amperage;



	   /* ������� �������� ������ ��� ������� �� ������ ��
	    * ����� �������� �������� ������������ ������ �
	    * ������� ��������� ������ ������� ��������*/
//	   volatile float summa_i2harm = summa_Qnf-iD2;
	   /* THD_I �� ����� �������� ������ ������� �������� � ��������� ��������� ������ */
//	   if((fs.Faznost==3)&&(us.oper_mode ==LowVoltageInductionMotorsProtection)){



	    /*The THD for the 25 lowest harmonic components of a
	   	rectangular current is: 29% */
		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			float thdi=THDi/100.0f;
			if(procOfRated>lLimitRms){
				HarmonicsDeratingFactor = 100.0f/(1.0f+thdi*thdi);
				THDi_HarmonicAmperageDistortion = THDi;
			}
			hg->THDi_HarmonicAmperageDistortion=THDi_HarmonicAmperageDistortion*0.85f;
			hg->HarmonicsDeratingFactor_pro=HarmonicsDeratingFactor;
		}
		else{
			hg->HarmonicsDeratingFactor_pro=0;
		}

	hfc=GRID_clc_OK;
	}
	return hfc;
}

static grid_calc_status_t f_grid_harmonics2_calc(grid_t *hg){
	grid_calc_status_t hfc=GRID_clc_UnKnown;
	while (hfc==GRID_clc_UnKnown){
//		if(procOfRated<lLimitRms){
//			aNegativeAmperage_adc=0;
//			bNegativeAmperage_adc=0;
//			cNegativeAmperage_adc=0;
//			sumNegativeAmperage_adc=0;
//
//			aNeutralAmperage_adc=0;
//			bNeutralAmperage_adc=0;
//			cNeutralAmperage_adc=0;
//			sumNeutralAmperage_adc=0;
//			hfc =GRID_clc_Impossible;
//			break;
//		}

//		static amperf_t aNegativeAmperage_adc=0;
//		static amperf_t bNegativeAmperage_adc=0;
//		static amperf_t cNegativeAmperage_adc=0;
//		static amperf_t sumNegativeAmperage_adc=0;

//		static amperf_t aNeutralAmperage_adc=0;
//		static amperf_t bNeutralAmperage_adc=0;
//		static amperf_t cNeutralAmperage_adc=0;
//		static amperf_t sumNeutralAmperage_adc=0;


//		/* ������, �� ��������� ������ � ���������� �������*/
		/* ��������� rms-������ ���������� ��������� �������,
		 * �� ������ ������� ������ � ���������� �������    */
		float new=FLT_MIN;
		new=FLT_MIN;
		f_ExponentialSmoothing  (
								iCfurie_out.rmsNegative,
								cNegativeAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
		cNegativeAmperage_adc=new;

		/* ������, �� ����� �������� �������� �� ����� ��� � ��������,
		 * ��������� �������� */
		/* ��������� rms-������ ����������� ��������� �������,
		 * �� ������ �������� �� ����� ��� ����  	*/
		new=FLT_MIN;
		f_ExponentialSmoothing  (
								iCfurie_out.rmsZero,
								cNeutralAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
		cNeutralAmperage_adc=new;
		hg->cNegativeAmperage_A=fs.clbr_iC*cNegativeAmperage_adc*0.85f;
		hg->cNeutralAmperage_A=fs.clbr_iC*cNeutralAmperage_adc*0.85f;

	    if (fs.Faznost==3){
	    	/* ������, �� ��������� ������ � ���������� �������*/
	    	/* ��������� rms-������ ���������� ��������� �������,
	    	* �� ������ ������� ������ � ���������� �������    */
	    	f_ExponentialSmoothing  (
								iAfurie_out.rmsNegative,
								aNegativeAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
	    	aNegativeAmperage_adc=new;
	    	new=FLT_MIN;
	    	f_ExponentialSmoothing  (
								iBfurie_out.rmsNegative,
								bNegativeAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
	    	bNegativeAmperage_adc=new;

	    	/* ��������� rms-������ ������������ ��������� �������,
	    	 * �� ������ �������� �� ����� ��� ����           */
	    	f_ExponentialSmoothing  (
								iAfurie_out.rmsZero,
								aNeutralAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
	    	aNeutralAmperage_adc=new;
	    	new=FLT_MIN;

	    	new=FLT_MIN;
	    	f_ExponentialSmoothing  (
								iBfurie_out.rmsZero,
								bNeutralAmperage_adc,
					    		20.0f,
					   			10.0f,
								&new
					   			);
	    	bNeutralAmperage_adc=new;
	    	sumNeutralAmperage_adc=cNeutralAmperage_adc+cNeutralAmperage_adc+cNeutralAmperage_adc;
	    	hg->aNegativeAmperage_A=fs.clbr_iA*aNegativeAmperage_adc*0.85f;
	    	hg->bNegativeAmperage_A=fs.clbr_iB*bNegativeAmperage_adc*0.85f;


	    	hg->aNeutralAmperage_A=fs.clbr_iA*aNeutralAmperage_adc*0.85f;
	    	hg->bNeutralAmperage_A=fs.clbr_iB*bNeutralAmperage_adc*0.85f;
	    	hg->sumNeutralAmperage_A=sumNeutralAmperage_adc*0.85f;

	    }
		hfc=GRID_clc_OK;
	}
	return hfc;

}



power_t WattSec_ElectricityConsumption = {0};
power_t kWh_ElectricityConsumption ={0};


//uint32_t daylim = 0;
//uint32_t daydur   = 0;
//uint32_t daytick  = 0;
//void f_GRID_Power_Init(void){
//	daylim=24*60*60*1000;
//	daytick=HAL_GetTick();
//
//}

static grid_calc_status_t f_grid_power_calc(grid_t *hg){

	  /* ��������� ��� ������ � ��������� �����������㳿 �������� �����������,
	   * �� ����� ��������� ��� ���������� �������, � ���� �������� 100��
	   * ������� rms ������ �� ����� �������*/



	  /* ����� ������, �� ���, ����� �� ������,
	   * ������� ������� �� ����������� �������,
	   * � �������� - �� ����������� ���������   */
	if (fs.Faznost==1){
		  ActivePower = 		cAmperage * Voltage * PowerFactor/100.0f;

		  ReactivePower = 		ActivePower *
				  	  	  	  	  	sqrtf(1.0f-(PowerFactor/100.0f)*(PowerFactor/100.0f));

		  UnbalanceLostPower =	0;

		  HarmonicLostPower = 	ActivePower * sqrtf
				                    (1.0f-(HarmonicsDeratingFactor/100.0f)
				  	  	  	  	  	  	*(HarmonicsDeratingFactor/100.0f)
									);
		  hg->ActivePower_kW=ActivePower/1000.0f;
		  hg->ReactivePower_kW=ReactivePower/1000.0f;
		  hg->HarmonicLostPower_kW=HarmonicLostPower/1000.0f;
	}
	else if(fs.Faznost==3){
	  vatt_t	full_power =
			  (aAmperage + bAmperage + cAmperage) *Voltage;

	  ActivePower = 		full_power*PowerFactor/100.0f;

	  ReactivePower = 		full_power*sqrtf
			  	  	  	  	  	(1.0f-(PowerFactor/100.0f)*(PowerFactor/100.0f));

	  UnbalanceLostPower =	ActivePower*((100.0f-UnbalanceDeratingFactor)/100.0f);

	  HarmonicLostPower = 	ActivePower*sqrtf
			                    (1.0f-(HarmonicsDeratingFactor/100.0f)
			  	  	  	  	  	  	*(HarmonicsDeratingFactor/100.0f)
								);
	  hg->ActivePower_kW=ActivePower/1000.0f;
	  hg->ReactivePower_kW=ReactivePower/1000.0f;
	  hg->UnbalanceLostPower_kW=UnbalanceLostPower/1000.0f;
	  hg->HarmonicLostPower_kW=HarmonicLostPower/1000.0f;
	}
	return GRID_clc_OK;
}
//	  float nAD=NeutralAmperageDistortion/100.0f;
//	  float nhlp = 1.0/(1.0+nAD*nAD);
//	  NeutralHarmonicLostPower=ActivePower*sqrtf(1.0-nhlp*nhlp);
//
//	  nAD=NegativeAmperageDistortion/100.0f;
//	  nhlp = 1.0/(1.0+nAD*nAD);
//	  NegativeHarmonicLostPower=ActivePower*sqrtf(1.0-nhlp*nhlp);
//
//
//	  /* C������ ���-������� */
//	  WattSec_ElectricityConsumption.Active 	+=ActivePower 		* 0.1;
//	  WattSec_ElectricityConsumption.Reactive 	+=ReactivePower 	* 0.1;
//	  WattSec_ElectricityConsumption.Unbalance 	+=UnbalanceLostPower* 0.1;
//	  WattSec_ElectricityConsumption.Harmonic 	+=HarmonicLostPower * 0.1;
//	  WattSec_ElectricityConsumption.Neutral 	+=NeutralHarmonicLostPower* 0.1;
//	  WattSec_ElectricityConsumption.Negative 	+=NegativeHarmonicLostPower* 0.1;
//
//	  /* ��� �� ���� C������ kWh */
//	  if((HAL_GetTick()-daytick)==daylim){
//		  kWh_ElectricityConsumption.Active  	+=WattSec_ElectricityConsumption.Active		/360000.0;
//		  kWh_ElectricityConsumption.Reactive	+=WattSec_ElectricityConsumption.Reactive	/360000.0;
//		  kWh_ElectricityConsumption.Unbalance	+=WattSec_ElectricityConsumption.Unbalance	/360000.0;
//		  kWh_ElectricityConsumption.Harmonic	+=WattSec_ElectricityConsumption.Harmonic	/360000.0;
//		  kWh_ElectricityConsumption.Neutral 	+=WattSec_ElectricityConsumption.Neutral	/360000.0;
//		  kWh_ElectricityConsumption.Negative 	+=WattSec_ElectricityConsumption.Negative	/360000.0;
//
//		  WattSec_ElectricityConsumption.Active   =0;
//		  WattSec_ElectricityConsumption.Reactive =0;
//		  WattSec_ElectricityConsumption.Unbalance=0;
//		  WattSec_ElectricityConsumption.Harmonic =0;
//		  WattSec_ElectricityConsumption.Neutral  =0;
//		  WattSec_ElectricityConsumption.Negative =0;
//	  }
//	  /* ���� ����� ������ �������� � �����..*/

// }
//	/* ����� ��� 24*60*60 */
////http://www.mirusinternational.com/downloads/MIRUS-TP003-A-How%20Harmonics%20have%20Contributed%20to%20Many%20Power%20Factor%20Misconceptions.pdf
//
//void f_GRID_RaportHR(uint16_t* grid16){
//	/* ����������:*/
////	 * 0		uint16_t, 0.01��		������� �� 46�� �� 54��,
//	 grid16[0]=(uint16_t)(Frequency*100);
////	 * 1		uint16_t  �						������� (���� �),
//	 grid16[1]=(uint16_t)(Voltage);
////	 * 234		uint16_t  0.1A, max 6553A �������������������� ������ ��� �BC (��������),
//	 grid16[2]=(uint16_t)(aAmperage);
//	 grid16[3]=(uint16_t)(bAmperage);
//	 grid16[4]=(uint16_t)(cAmperage);
////	 * 567		uint16_t  0.1A 		�������������������� ������ ����� �������� ��� ���.
//	 grid16[5]=(uint16_t)(aAdc1Amperage);
//	 grid16[6]=(uint16_t)(bAdc1Amperage);
//	 grid16[7]=(uint16_t)(cAdc1Amperage);
//
////	 * 	����������:
////	 * 8		uint16_t  0.1���    ������� ���������,
//	 grid16[8]=(uint16_t)(ActivePower/100.0f);
////	 * 9		uint16_t  0.1��� 	��������� ��������� ��
//	 grid16[9]=(uint16_t)(ReactivePower/100.0f);
////	 * 10		int16_t   %		    ����������� ��������� (�os fi),
//	 grid16[10]=(uint16_t)PowerFactor;
//
//	 //	 * 	12	uint8_t	  % 		���������� ������ ��������� ����� �������� ������,
////	 *  	uint8_t	  % 		�� ����� ����������� �������.
//	 grid16[11]=((uint16_t)(UnbalanceDeratingFactor))*0x100
//			 	 	 	 	 	 + (uint16_t)AmperageUnbalance;
//
////	 * 11		uint8_t   %			����������� ������ ��������� ����� �������� ������� ������,
////	 * 			uint8_t	  %         �� ����� ����������� �������,
//	 grid16[12]=((uint16_t)(HarmonicsDeratingFactor))*0x100
//			 	 	 	 	 	 + (uint16_t)THDi_HarmonicAmperageDistortion;
//
//	 /* ������� ������, �� ����� �� �����, �������� �� ���� �� �����,
//	  * ��� � ������� ����������� ������� */
//	 grid16[13]=(uint16_t)LeakAmperageDistortion * 0x100
//	 /* ������� ������, �� ����� �� �����
//	  * ����� ��������� ��                   */
//			 	 	 	 	 	 + (uint16_t)GrundAmperageDistortion;
//	 /* ������� ������, �� ����� �� �����
//	  * ����� ������� ����������� ������� */
//
//	 grid16[14]=(uint16_t)NeutralAmperageDistortion * 0x100
//	 /* ������� ������ �������, �� ����� �������������� */
//			 	 	 	 	 	 + (uint16_t)NegativeAmperageDistortion;
//	 grid16[15]=0xFF00;
//
//
////	 *HarmonicsDeratingFactor=100.0;
////UnbalanceDeratingFactor=100.0;
////	 * 	³������ �� ���� �� ������ ���:
////	 * 0x10..0x2F	uint16_t 1..31    �� �������� �����
////	 *
////	 * ���������� � ����
////	 * 0x30..0x70		4 � uint32_t 0..11(1..12) �� ����� ����
////	 *
////	 * 		������� ������� �����������㳿 (���.���),
////	 * 		������� ������� ��������� �����������㳿 (����.���),
////	 * 		������� �����������㳿, �������� ����� �������� ������� (����.���),
////	 * 		������� �����������㳿, �������� �� ��������������� ����� �������� (���.���).
////	 *
////	 * 	���������:
////	 * 		0000 0001 ������� ����� ���������,
////	 * 		0000 0010 �� ���� ���������,
////	 * 		0000 0100 ������� ����� ���������,
////	 * 		0000 1000 ������� ���� ���������,
////	 * 		0001 0000 ���������� ����,
////	 * 		0010 0000 ����������� ����������� ���,
////	 * 		0100 0000 ������ ������ �� ����� ���� �����������
////	 * 		1000 0000 ������
////	 *
////
////	 *		Read Holding Registers 03
////	 *		Read Exception status  07
////	 *		Read Coils 01
////	 * */
//}
//
//void f_GRD_Print(void){
//	uint16_t strFr[17];
//							  /*0123456789012345*/
//							  /*2019/02/15 11:55*/
//	snprintf((char*)strFr, 17, "%4d/%2d/%2d %2d:%2d",
//			p_wk64_tm->tm_year+1900,
//			p_wk64_tm->tm_mon+1,
//			p_wk64_tm->tm_mday,
//			p_wk64_tm->tm_hour,
//			p_wk64_tm->tm_min);
//
//
//	WH1602b3_WriteHighString((uint8_t*)(&strFr));
//
//	uint16_t D=(uint16_t)Voltage;
//	uint16_t F=(uint16_t)Frequency;
//	uint16_t f=(uint16_t)((Frequency-(float)F)*100);
//	snprintf((char*)strFr, 17, "U=%03dv  f=%02d.%02d", D, F, f);
//
//	WH1602b3_WriteLowString ((uint8_t*)(&strFr));
//}

static volatile TM_Cntr=0;
static volatile float ThermalModelTimeConstant_sec=700.0f;
static volatile float ThermalValue=0;
static grid_calc_status_t f_ThermalModel_calc(grid_t *hg){

	procentf_t newThermalValue = 0;
//ShareOfRated_0_1=7.2f;
	TM_Cntr++;
	if( ShareOfRated_0_1>(lLimitRms/100.0f)){
		/* ���� ����� ��������� ����� 7% �� ����������� -
		 * ������� �� ������ ����������                */
		float QAmperage=ShareOfRated_0_1*ShareOfRated_0_1;
		float QUA=QAmperage/(UnbalanceDeratingFactor/100.0f);
		float QHUA=QUA/(HarmonicsDeratingFactor/100.0f);
		f_ExponentialSmoothing  (
				            QHUA,
							ThermalValue,
							ThermalModelTimeConstant_sec,
				   			10.0f,
							&newThermalValue
				   			);
	}
	else{
		/* ���� ����� ��������� ����� 7% �� ����������� -
		 * ������� �� ������ ������������                 */
		f_ExponentialSmoothing  (
							0.0f,
							ThermalValue,
							2.0f*ThermalModelTimeConstant_sec,
				   			10.0f,
							&newThermalValue
				   			);

	}

	ThermalValue=newThermalValue;
	if(ThermalValue>1.5f){
		TM_Cntr--;
	}
	hg->MotorHeatProcentage=100.0f*ThermalValue;
	return GRID_clc_OK;
}




static int16_t f_tim7_PeriodAutoTuning(hzf_t frequency){
	//	/* ������� ����� ��� TIM7 ��� �������/������ ��� ���'�":*/
		float TIM7_Freq=N_FURIE*Frequency;
	//	/* �� Ne 		- ������� ����� �� �����, ���� 'tkConfig.h'
	//	 *    Frequency - ������� ������� ������� �����, ���� 'tkGrid.c' */
		float perf=1800000/TIM7_Freq;
	    peru16=(uint16_t)perf;
		if (peru16 != old_peru16){
			MX_TIM7_Init();
		}
		return 0;
}










#ifdef FURIE_TEST
void GreedModel(void){
	f_furie_Model(Frequency);
}
#endif
#ifdef GRID_DISTANCE_DEBUG
void GridDistanceDebug(void){
	f_GridDistanceDebug();
}
void GridDistanceDebugInit(void){
	f_GridDistanceDebugInit();
}
/* �������� ���˲������Ҳ ��� ��� ��2*/
//typedef enum{
//	PhasesUnknownTypeRotation,
//	PhasesDirectRotation,
//	PhasesReverceRotation,
//	PhasesSequenceFault,
//	Phase_A_fault,
//	Phase_B_fault
//}phases_rotation_type_t;
/* �� ��������, ���� ���������� �.�.
 * 1) �������� ������ ����� �������� TK2_LINEZERO
 * 2) ��������� ���� �� ������� ����� ������
 * (������ ��� 200 ��� 100 ����� �� �����)
 * �������� �������� ����� ����� ���� �����,
 * �������� �� ��� ����, ��� �� � ��������, � ������,
 * ����� � � ������� ��������� ���� ���� � �����..
 * 3) ����������� ����������� - ���� ��� - ������, ���� ��� - ��������..
 * �� � ���..
 */
/*1) �������� ������ ����� �������� TK2_LINEZERO*/
#define TK2_LINEZERO 2088

static phases_rotation_type_t f_tk2_phase_rotation(
		volatile adcu16_t *p200a_array,/* ������ ����� ���*/
		volatile adcu16_t *p200b_array,
		volatile adcu16_t *p200c_array,
		uint16_t Nn           /* ������� �����*/
		/* ��������, �� � ��2 ����� � ����� �� �����������,
		 * ���� �� ���� ����� ������ � ��� ������� �����,
		 * � ����, ���� ��� �.. */
		){
	/* ��������� */
	phases_rotation_type_t phR=PhasesUnknownTypeRotation;
	/* ��� �������������� �� ���� - ������ ����� ����� ������ ��, ���� � 3*/
	uint8_t iam=0;
	uint8_t ibm=0;
	uint8_t icm=0;
	/* ��� �������������� �� ���� - ������ ����� ���� ������ ��, ���� � 3*/
	uint8_t iap=0;
	uint8_t ibp=0;
	uint8_t icp=0;
	/* �����, ���� ���� ���� � ��� ����� ��, ���� ���� � ��� ���� - ������� ������� ����� ���*/
	uint8_t pha=0;
	uint8_t phb=0;
	uint8_t phc=0;
	for (uint8_t i=0;i<Nn;i++){
		if(p200a_array[i]<TK2_LINEZERO){
			iam++;
		}
		else{
			if(iam>3){
				iap++;
				if((iap>3)&&(pha==0)){
					pha=i;
				}
			}
		}
		if(p200b_array[i]<TK2_LINEZERO){
			ibm++;
		}
		else{
			if(ibm>3){
				ibp++;
				if((ibp>3)&&(phb==0)){
					phb=i;
				}
			}
		}
		if(p200c_array[i]<TK2_LINEZERO){
			icm++;
		}
		else{
			if(icm>3){
				icp++;
				if((icp>3)&&(phc==0)){
					phc=i;
				}
			}
		}
	}
	/* ���� �������� �� �� �������� - ������� ����� */
	if(pha==Nn){
		phR=Phase_A_fault;
	}

	else if(phb==Nn){
		phR=Phase_B_fault;
	}
	else if(phc==Nn){
		phR=Phase_C_fault;
	}
	else{
		/* ����������� ����������� - ���� ��� - ������, ���� ��� - ��������..*/
		if  (((pha<phb)&&(phb<phc))
			||((phc<pha)&&(pha<phb))
			||((phb<phc)&&(phc<pha))){
			phR=PhasesDirectRotation;
		}
		else if(((pha>phb)&&(phb>phc))
				||((phc>pha)&&(pha>phb))
				||((phb>phc)&&(phc>pha))){
			phR=PhasesReverceRotation;
		}
		else{
			phR=PhasesSequenceFault;
		}
	}
	return phR;
}





#endif //GRID_DISTANCE_DEBUG
#endif

