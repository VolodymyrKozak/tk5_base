/*
 * mdGRD_test2.c
 *
 *  Created on: 17 січ. 2020 р.
 *      Author: KozakVF
 */

#include "mdGRD_test2.h"
#ifdef GRID_DISTANCE_DEBUG
#include "tkSGL.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "vkDeclarations.h"
#include "../../3_Libraries/tkGrid/tkFurie.h"
#include <tk1_grid.h>
#include "vv300set_user.h"
#include "zFF.h"
/* Вхід симуляції від Модбас */
uint16_t AmplMode   =0; /* 0- на вхід модуля GRID подаються реальні відліки АЦП струму -
						   не нуль - симуляція, підміна відліків АЦП на змодельовані
						   в модулі TEST2:
						   1- синусоїдальна
 	 	 	 	 	 	   2- прямокутна*/


uint16_t simAmplAve =100; /* Амплітуда середня, в процентах  */
uint16_t simAmpB    =100; /* Амплітуда фази Б, для симуляції асиметрії*/
uint16_t CosinusPhi =50;  /* Косинус у процентах */

/* Відповідає номінальному струму 8.9А для ТК412Н1-0*/
float Ampl=598.0f;

extern user_settings_t user_settings;
/* Вхід з GRID*/
/* Зглажена частота в герцах 	  */
extern volatile hzf_t 		Frequency;


/* Вихід функції для GRID*/
extern adcf_t iFurieA[N_FURIE+4];
extern adcf_t iFurieB[N_FURIE+4];
extern adcf_t iFurieC[N_FURIE+4];

extern adcu16_t iA[240];
extern adcu16_t iB[240];
extern adcu16_t iC[240];

extern volatile ufurie_out_t uCfurie_out;
/*Зглажене значення фази */
static float U_PHASE=180.0f;
extern on_off_t LastCmndForMotor;
/* За поданою командою мотор міг включитися, а міг і не включитится,
 * це реальний стан, який визначається по величині робочого струму */
on_off_t LastCmndForMotorOld=oFF_;
uint32_t GRDT2_loopTick=0;
//HAL_GetTick();
uint16_t N18 = N_FURIE;


static float t2cos18[18]={0};
static float t2cos200[200]={0};




static float acosGrad=0;

static adcf_t A_amlitude = 		0;
static adcf_t B_amlitude = 		0;
static adcf_t C_amlitude = 		0;

static int f_iModel18_dist(
					gradf_t uPhase
					);

static int f_Model200_dist(
		hzf_t ui_msrfreq,
		gradf_t phaseGrad);



void f_GridDistanceDebugInit(void){
	for (uint16_t i=0;i<18;i++){
		float fi = (float)i / 18.0f;
		t2cos18[i]=cosf(2.0f*M_PI*fi);
	}
	for (uint16_t i=0;i<200;i++){
		float fi = (float)i / 200.0f;
		t2cos200[i]=cosf(2.0f*M_PI*fi);
	}
}

void f_GridDistanceDebug(void){
	if (AmplMode == 0) return;
	/* Якщо це симуляція - то стираємо отримані від АЦП відліки*/
	memset ((void*)(&iFurieA),0,sizeof(iFurieA));
	memset ((void*)(&iFurieB),0,sizeof(iFurieB));
	memset ((void*)(&iFurieC),0,sizeof(iFurieC));

	memset ((void*)(&iA),0,sizeof(iA));
	memset ((void*)(&iB),0,sizeof(iB));
	memset ((void*)(&iC),0,sizeof(iC));
	/**/
	//Якщо мотор умовно включено
	if(LastCmndForMotor==oN_){
	   A_amlitude=simAmplAve*Ampl/100.0f;
	   B_amlitude=simAmplAve*Ampl*simAmpB/10000.0f;
	   C_amlitude=simAmplAve*Ampl/100.0f;
	}

	if ((LastCmndForMotorOld==oFF_)&&(LastCmndForMotor==oFF_)){
		   A_amlitude=0;
		   B_amlitude=0;
		   C_amlitude=0;
	}
	else if((LastCmndForMotorOld==oN_)&&(LastCmndForMotor==oFF_)){
		   A_amlitude=0;
		   B_amlitude=0;
		   C_amlitude=0;
	}

	else if((LastCmndForMotorOld == oFF_)&&(LastCmndForMotor==oN_)){
		GRDT2_loopTick=HAL_GetTick();
	}
	else if((LastCmndForMotorOld == oN_)&&(LastCmndForMotor==oN_)){
		uint32_t tick=HAL_GetTick()-GRDT2_loopTick;
		if((HAL_GetTick()-GRDT2_loopTick)<7200){
			   A_amlitude=7.2f*simAmplAve*Ampl/100.0f;
			   B_amlitude=7.2f*simAmplAve*Ampl*simAmpB/10000.0f;
			   C_amlitude=7.2f*simAmplAve*Ampl/100.0f;

//			   A_amlitude=7.2f*simAmplAve*Ampl/100.0f;
//			   B_amlitude=7.2f*simAmplAve*Ampl*simAmpB/10000.0f;
//			   C_amlitude=7.2f*simAmplAve*Ampl/100.0f;
		}
		else{
			   A_amlitude=simAmplAve*Ampl/100.0f;
			   B_amlitude=simAmplAve*Ampl*simAmpB/10000.0f;
			   C_amlitude=simAmplAve*Ampl/100.0f;
		}
	}
	else{}
	   /* По заданому косинусу фі знаходимо кут в градусах
	    * для симулювання відставання струму ед від напруги */
	   float acosRad =acosf((float)CosinusPhi/100.0f);
	   acosGrad=acosRad*360.0f/(2.0f*M_PI);

		U_PHASE = uCfurie_out.u_phase;

//		int8_t str[17]={0};
//		snprintf((char*)str,17,  "Фаза %d", (uint16_t)U_PHASE);
//		tkDisplay_PutLowStr(&str);
	f_iModel18_dist(U_PHASE);


	f_Model200_dist(
			Frequency,
			U_PHASE);
	LastCmndForMotorOld=LastCmndForMotor;
}

/*Вісімнадцять відліків залежно від фази, незалежно від частоти */
static int f_iModel18_dist(
					gradf_t uPhase
					){
	        while (uPhase<0.0f){uPhase=uPhase+360.0f;}
	        while (uPhase>360.0f){uPhase=uPhase-360.0f;}
			/* Початкова фаза в градусах*/
	        /* Стум  відстає від напруги на кут acosGrad,
	         * тобто фаза струму це фаза напругb - acosGrad*/

			/* Початкова фаза в градусах*/
			float figC=(uPhase-acosGrad+9.0f);
			if(figC<0.0f){figC=figC+360.0f;}
			/* Початковий індекс синуса в масиві*/
			float figC1=(figC/360.0f)*18.0f;
			int16_t ic=(int16_t)(figC1);

			float figA=(uPhase-120.0f-acosGrad+9.0f);
			if(figA<0.0f){figA=figA+360.0f;}
			/* Початковий індекс синуса в масиві*/
			float figA1=(figA/360.0f)*18.0;
			int16_t ia=(int16_t)(figA1);


			/* Початкова фаза в градусах*/
			float figB=(uPhase-240.0f-acosGrad+9.0f);
			if(figB<0.0f){figB=figB+360.0f;}
			/* Початковий індекс синуса в масиві*/
			float figB1=(figB/360.0f)*18.0f;
			int16_t ib=(int16_t)(figB1);


	for (uint8_t n=0; n<N_FURIE; n=n+1){


		if(AmplMode==1){/* Синусиїдальний струм */
/**********************************************************************************************
 *    Фаза А
 * ********************************************************************************************
 */
			float it=0;
			uint16_t i16t = 0;
			int16_t iam=ia+n;
			if(iam>17){iam=iam-18;}
			it=2048.0+A_amlitude*t2cos18[iam];
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}

			i16t=(uint16_t)it;

			if (i16t>2048){i16t=i16t-1;}
			it = (float)i16t;
			iFurieA[n]=it;
/**********************************************************************************************
*    Фаза В
* ********************************************************************************************
*/
			int16_t ibm=ib+n;
			if(ibm>17){ibm=ibm-18;}
			it=2048.0+B_amlitude*t2cos18[ibm];
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}

			i16t = (uint16_t)it;
			if (it>2048){i16t-=1;}
			it = (float)i16t;
			iFurieB[n]=it;
/**********************************************************************************************
*    Фаза С
* ********************************************************************************************
*/
			int16_t icm=ic+n;
			if(icm>17){icm=icm-18;}
			it=2048.0+C_amlitude*t2cos18[icm];
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}

			i16t = (uint16_t)it;
			if (it>2048){i16t-=1;}
			it = (float)i16t;
			iFurieC[n]=it;
		}
		else if(AmplMode==2){
			/**********************************************************************************************
			 *    Фаза А
			 * ********************************************************************************************
			 */
			float it=0;
			uint16_t i16t = 0;
			int16_t iam=ia+n;
			if(iam>17){iam=iam-18;}
			if(t2cos18[iam]>0){
				it=2048.0+A_amlitude;
			}
			//		if (cosf(2.0* M_PI*((grid_freq/ui_msrfreq)*(nf/N18f)+uPhase/360.0))>0){
			//			it = 2048.0+A_amlitude;
			//		}
			else {
				it = 2048.0-A_amlitude;
			}
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}
			i16t=(uint16_t)it;

			if (i16t<2048){}
			else {i16t=i16t-1;}
			it = (float)i16t;
			iFurieA[n]=it;
			/**********************************************************************************************
			*    Фаза В
			* ********************************************************************************************
			*/
			int16_t ibm=ib+n;
			if(ibm>17){ibm=ibm-18;}
			if(t2cos18[ibm]>0){
				it=2048.0+B_amlitude;
			}
			else{
				it=2048.0-B_amlitude;
			}
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}
			i16t = (uint16_t)it;
			if (it<2048){i16t-=1;}
			it = (float)i16t;
			iFurieB[n]=it;

			/**********************************************************************************************
			*    Фаза С
			* ********************************************************************************************
			*/
			int16_t icm=ic+n;
			if(icm>17){icm=icm-18;}
			if(t2cos18[icm]>0){
				it=2048.0+C_amlitude;
			}
			else{
				it=2048.0-C_amlitude;
			}
			if(it>4095){it=4095.0f;}if(it<0){it=0.0f;}
			i16t = (uint16_t)it;
			if (it<2048){i16t-=1;}
			it = (float)i16t;
			iFurieC[n]=it;

		}
	}

	return 0;
}

/* Кількість результативних відліків залежить від частоти
 * при 50Гц - точно 200 відліків*/
static int f_Model200_dist(
		hzf_t ui_msrfreq,
		gradf_t uPhase){

	/* Початкова фаза в градусах*/
    float grad=0;
    grad=uPhase-acosGrad;
    if(grad<0.0f){grad=grad+360.0f;}
	float figC=grad/360.0f;
	/* Початковий індекс синуса в масиві*/
	int16_t ic=(int16_t)(figC*200.0f);
	if (ic>199){ic=ic-200;}
	if (ic<0){ic=ic-200;}

	/* Початкова фаза в градусах*/
    grad=uPhase-120.0f-acosGrad;
    if(grad<0.0f){grad=grad+360.0f;}
	float figA=grad/360.0;
	/* Початковий індекс синуса в масиві*/
	int16_t ia=(int16_t)(figA*200.0f);
	if (ia>199){ia=ia-200;}
	if (ia<0){ia=ia+200;}

	/* Початкова фаза в градусах*/
    grad=uPhase-240.0f-acosGrad;
    if(grad<0.0f){grad=grad+360.0f;}
	float figB=grad/360.0;

	/* Початковий індекс синуса в масиві*/
	int16_t ib=(int16_t)(figB*200.0f);
	if (ib>199){ib=ib-200;}
	if (ib<0){ib=ib+200;}



	/*  всего отсчетов за период 	*/

	for (uint8_t n=0; n<240; n=n+1){



		if(AmplMode==1){/* Синусиїдальний струм */
			/***************************************************************
			 * СИНУСОЇДА
			 */
			/***************************************************************/
			int16_t iam=ia+n;
			if(iam>199){iam=iam-200;}
			float iAcosf=A_amlitude*t2cos200[iam];

			float ia16=2048.0+(int16_t)iAcosf;
			if(ia16>4095){
				ia16=4095.0f;
			}if(ia16<0){ia16=0.0f;}
// 		if (cosf(2.0* M_PI*(nf/Nif+phaseGrad/360.0))>0){
// 			ia16 =2048.0+A_amlitude;
// 		}
// 		else {
// 			ia16 =2048.0-A_amlitude;
// 		}
			iA[n]=(uint16_t)ia16;
			/***************************************************************/
			/***************************************************************/
			int16_t ibm=ib+n;
			if(ibm>199){ibm=ibm-200;}
			float iBcosf=B_amlitude*t2cos200[ibm];
			float ib16 =2048+(int16_t)iBcosf;
			if(ib16>4095){ib16=4095.0f;}if(ib16<0){ib16=0.0f;}
			iB[n]=(uint16_t)ib16;
			/***************************************************************/
			/***************************************************************/
			int16_t icm=ic+n;
			if(icm>199){icm=icm-200;}
			float iCcosf=C_amlitude*t2cos200[icm];
			float ic16 =2048+(int16_t)iCcosf;
			if(ic16>4095){ic16=4095.0f;}if(ic16<0){ic16=0.0f;}
			iC[n]=(uint16_t)ic16;
			/***************************************************************/
		}
		else if(AmplMode==2){
			/***************************************************************
			 * ПРЯМОКУТНИЙ СТРУМ A
			 ***************************************************************/
			int16_t iam=ia+n;
			if(iam>199){iam=iam-200;}
			if (t2cos200[iam]>0){
				iA[n]=2048.0+A_amlitude;
			}
			else{
				iA[n]=2048.0-A_amlitude;
			}
			if(iA[n]>4095){iA[n]=4095.0f;}if(iA[n]<0){iA[n]=0.0f;}
			/***************************************************************
			 * ПРЯМОКУТНИЙ СТРУМ B
			 ***************************************************************/
			int16_t ibm=ib+n;
			if(ibm>199){ibm=ibm-200;}
			if (t2cos200[ibm]>0){
				iB[n]=2048.0+B_amlitude;
			}
			else{
				iB[n]=2048.0-B_amlitude;
			}
			if(iB[n]>4095){iB[n]=4095.0f;}if(iB[n]<0){iB[n]=0.0f;}
			/***************************************************************
			 * ПРЯМОКУТНИЙ СТРУМ C
			 ***************************************************************/
			int16_t icm=ic+n;
			if(icm>199){icm=icm-200;}
			if (t2cos200[icm]>0){
				iC[n]=2048.0+C_amlitude;
			}
			else{
				iC[n]=2048.0-C_amlitude;
			}
			if(iC[n]>4095){iC[n]=4095.0f;}if(iC[n]<0){iC[n]=0.0f;}
		}
		else{}
	}
	return 0;
}
#endif
