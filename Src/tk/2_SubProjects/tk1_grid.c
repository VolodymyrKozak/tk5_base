/*
 * tkFreq.c
 *  Created on: 27 груд. 2018 р.
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

#define U_KOEFF0  0.1 /* Поправка до коефіцієнта Є.Т.*/
/* Налаштування користуача */
user_settings_t us;
/* Налаштування виробника */
fSet_t fs;
/**********************************************************************************************
 * ГЛОБАЛЬНІ ЗМІННІ. ВХІДНІ ДЛЯ МОДУЛЯ
 **********************************************************************************************/
volatile uint16_t peru16 = 2000;
/* Відліки АЦП для Фур'є */
volatile adcf_t uFurieC[N_FURIE+4]={0};
volatile adcf_t iFurieA[N_FURIE+4]={0};
volatile adcf_t iFurieB[N_FURIE+4]={0};
volatile adcf_t iFurieC[N_FURIE+4]={0};
volatile adcf_t iFurieLeak[N_FURIE+4]={0};

/* Оголошені масиви для зберігання відліків струмів та напруги
 * Вводяться у файлі stm32f1_it.c (при відлагодженні - mdGRD_test.с
 * використовуються лише у цьому файлі */
volatile adcu16_t iA[240]={0};
volatile adcu16_t iB[240]={0};
volatile adcu16_t iC[240]={0};
volatile adcu16_t iDf[240]={0};
volatile adcf_t   uC[240]={0};

volatile adcu16_t iLeak[240]={0};



/**********************************************************************************************
 * ГЛОБАЛЬНІ ЗМІННІ. ВИХІДНІ ДЛЯ МОДУЛЯ
 **********************************************************************************************/


/* Зглажена частота в герцах 	  */
volatile hzf_t 	Frequency 	= 50.0;
/* Зглажена напруга в вольтах*/
volatile voltf_t 	Voltage		=220.0;
/* Зглажені струми в амперах */
volatile amperf_t 	aAmperage	=0.0;
volatile amperf_t 	bAmperage	=0.0;
volatile amperf_t 	cAmperage	=0.0;
volatile amperf_t 	leakAmperage	=0.0;
/* Зглажені основні коєфіцієнти ефективності мережі  */
procentf_t PowerFactor =100.0;
procentf_t UnbalanceDeratingFactor=100.0;
procentf_t HarmonicsDeratingFactor=100.0;

//procentf_t LeakDeratingFactor=100.0;
//procentf_t GrundDeratingFactor=100.0;
//procentf_t NeutralDeratingFactor=100.0;

/* Зглажені інтегральні показники ефективності мережі */
procentf_t HmUnbDerating =100.0;
procentf_t fullDerating =100.0;
/* Показники енергоспоживання                               */
vatt_t	ActivePower 		= 	0;
vatt_t	ReactivePower 		=   0;
vatt_t  ReactivePowerInductiv=  0;
vatt_t  ReactivePowerCapacitive=0;
vatt_t	UnbalanceLostPower 	=	0;
vatt_t	HarmonicLostPower 	= 	0;
vatt_t	NeutralHarmonicLostPower 	= 	0;
vatt_t	NegativeHarmonicLostPower 	= 	0;
/**********************************************************************************************/

/*Зглажені параметри ефективності мережі 					*/
/*Коефіцієнт нелінійних спотворень по струму у кожній фазі  */
procentf_t aTotalHarmonicDistortion = 0;
procentf_t bTotalHarmonicDistortion = 0;
procentf_t cTotalHarmonicDistortion = 0;

/************************************************************************
Коефіцієнт нелінійних спотворень по струму узагальнений    				*/
procentf_t THDi_HarmonicAmperageDistortion = 0;
/*THDі вказує на ступінь спотворення форми синусоїди струму.
THDi менше 10% вважається нормальним,
ризик несправностей обладнання відсутній, перегрівання до 6%.

THDi між 10% і 50% вказує на значне гармонійне спотворення.
Падіння коефіцієнта потужності елекродвигунів, підвищення температури,
що означає, що кабелі та установок повинні бути понадгабаритними.

THDi, що перевищує 50%, вказує на високі гармонічні спотворення.
Високий ризик виходу обладнання з ладу. Необхідний поглиблений аналіз причин
 і заходи з їх усунення, зокрема рекомендується
 система придушення несинусоїдальнусті струму.

Скорочення терміну служби обладнання:
(Дані Канадської асоціації електриків).
Коли спотворення напруги живлення знаходиться в діапазоні 10%,
термін служби обладнання значно скорочується.
Залежно від типу пристрою, скорочення терміну служби:
32,5% для однофазних електродвигунів
18% для трифазних електродвигунів
5% для трансформаторів.
***************************************************************************
3, 5, 7, 11 and 13 - найбільше небезпечні для електродвигунів
*/


/* Коефiцієнт перекосу фаз 									*/
volatile procentf_t AmperageUnbalance = 0;


/* Струми, які обертають двигун в протилежну сторону*/
static adcf_t aNegativeAmperage_adc=0;
static adcf_t bNegativeAmperage_adc=0;
static adcf_t cNegativeAmperage_adc=0;
static adcf_t sumNegativeAmperage_adc=0;
/* Струми, що через гармоніки витікають на землю або в нейтраль,
 * нагрівають нейтраль */
static adcf_t aNeutralAmperage_adc=0;
static adcf_t bNeutralAmperage_adc=0;
static adcf_t cNeutralAmperage_adc=0;
static adcf_t sumNeutralAmperage_adc=0;
/*Струм у попугаях та фаза в градусах (незглажені)
 * обчислені у кожному циклі контролера */
volatile static ifurie_out_t iAfurie_out = {0};
volatile static ifurie_out_t iBfurie_out = {0};
volatile static ifurie_out_t iCfurie_out = {0};
volatile ufurie_out_t uCfurie_out = {0};
/*Струм у попугаях, обчислений за 200 відліків в кожному циклі */
volatile iheat_out_t  iAheat_out = {0};
volatile iheat_out_t  iBheat_out = {0};
volatile iheat_out_t  iCheat_out = {0};

/* Приведений струм електродвигуна (доля від номінального)
 * буде потрібен для захисту від перевантаження по тепловій моделі
 * Обчислюється в f_GRID_Amperage_Calc()*/
float ShareOfRated_0_1=1.0f;	/* незглажений */
/* */
procentf_t procOfRated =100.0f;  	/* зглажений  */
/* За поданою командою мотор міг включитися, а міг і не включитится,
 * це реальний стан, який визначається по величині робочого струму */
extern on_off_t MotorState;

volatile static ifurie_out_t iLEAKfurie_out = {0};
volatile static iheat_out_t  iLEAKheat_out = {0};


/* Зглажені значення нулів АЦП для всіх струмів */
volatile static adcf_t aAdc0Amperage=2048;
volatile static adcf_t bAdc0Amperage=2048;
volatile static adcf_t cAdc0Amperage=2048;
volatile static adcf_t leakAdc0Amperage=2048;

/* Зглажені теплові середньоквадратичні струми фаз в попугаях */
volatile static adcf_t aAdcAmperage=0;  /* 362.0*/
volatile static adcf_t bAdcAmperage=0;
volatile static adcf_t cAdcAmperage=0;
/* середнє значення зглажених фазних струмів в попугаях       */
volatile static adcf_t aveAdcAmperage=0;
volatile static adcf_t leakAdcAmperage=0;
volatile static adcf_t grundAdcAmperage=0;
volatile static adcf_t neutralAdcAmperage=0;
volatile static adcf_t negativeAdcAmperage=0;



/* Зглажені середньоквадратичні струми основної (першої) гармоніки фаз в попугаях */
volatile static adcf_t aAdc1Amperage=0;  /* 362.0*/
volatile static adcf_t bAdc1Amperage=0;
volatile static adcf_t cAdc1Amperage=0;
volatile static adcf_t leakAdc1Amperage=0;
/*****************************************************************************/
/* зглажені кути в градусах                                                  */
static gradf_t u_i_Phi =0;
static gradf_t a_b_Phi =120.0;
static gradf_t b_c_Phi =120.0;
static gradf_t c_a_Phi =120.0;


/* УСТАВКИ, ПРИВЕДЕНІ ДО папуг АЦП */
/* Номінативний струм системи в папугах АЦП  */
volatile  adcf_t ratedAdcAmperage =  423.8f;
/* Мінімальний струм, % до номінативного, для якого обчислюємо витоки
 * Вважаємо, що двигун працює, якщо зглажений струм більше 7% номінального*/
volatile static procentf_t lLimitRms 	=   20.0f;
/* Допустимий витік з міліамперів перерахований в папуги ацп
В разі використання диференційного трансформатора (УЗО) чи без*/

volatile static procentf_t LimitLeak_adc =  7.0f;

volatile static oper_mode_t grid_oper_mode=LowVoltageInductionMotorsProtection;


volatile static adcf_t rated220AdcVoltage = 547.1f;
/* Повідомлення про граничну напругу */
volatile static procentf_t lLimitVoltage =  60.0f;
volatile static procentf_t hLimitVoltage =  150.0f;
/* Повідомлення про витік струму на землю
 * 1) В разі використання диференційного трансформатора (УЗО)*/


/*********************************************************************************************
 * Змінні допоміжні для організації обчислень
 *********************************************************************************************/

/* допоміжні змінні для обчислень */

static uint32_t fr_counter=0;
static float oldPhase = 0;

static float PhaseDiff = 0;
volatile static float currentPhaseDiff =FLT_MIN;
/* Змінна для фіксації зміни частоти мережі, а отже і частоти ТІМ7*/
 static uint16_t old_peru16=2000;
#ifdef		FREQUENCY_CALC_DEBUG0
/* Змінні для відлагоджування/тестування вимірювання частоти */
static volatile float phd[100]={0};
static volatile uint32_t i_phd=0;
static volatile float ph[100]={0};
static volatile uint32_t j_phd=0;
#endif
/* Змінні для засікання часу/тривалості при відлагоджуванні */
static uint32_t iclk=0;
static volatile uint32_t iDuration =0;

/* змінні для оцінки тривалості процедур при відлагоджуванні */
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
		/*Обнуляємо всі масиви з вдліками миттєвих значень */
	memset ((void*)&uFurieC,0,sizeof(uFurieC));
	memset ((void*)&iFurieC,0,sizeof(iFurieC));

	memset ((void*)&uC,0,sizeof(uC));
	memset ((void*)&iC,0,sizeof(iC));
	/* Коефіцієнт напруги від Тимофійовича*/
	fs.clbr_uC = 0.896f*hfs->clbr_uC;	//4.021f;
	fs.clbr_iC = 1.209f*hfs->clbr_iC;  //0.021f
	/* Номінальна напруга в попугаях */
	if((fs.clbr_uC*U_KOEFF0)>0){
		rated220AdcVoltage=220.0f/(fs.clbr_uC*U_KOEFF0);
	}

	if(fs.Faznost==1){
		/* Номінальний струм в одиницях АЦП для трифазного двигуна  */
		if(fs.clbr_iC>0){
			ratedAdcAmperage=hus->rated_amperage_set/fs.clbr_iC;
		}

	}
	else if (fs.Faznost==2){}
	else if (fs.Faznost==3){
		/*Обнуляємо всі масиви з вдліками миттєвих значень */

		memset ((void*)&iFurieA,0,sizeof(iFurieA));
		memset ((void*)&iFurieB,0,sizeof(iFurieB));
		memset ((void*)&iFurieLeak,0,sizeof(iFurieLeak));

		memset ((void*)&iA,0,sizeof(iA));
		memset ((void*)&iB,0,sizeof(iB));
		memset ((void*)&iLeak,0,sizeof(iLeak));

		fs.clbr_iA = 1.209f*hfs->clbr_iA;	//0.021f;
		fs.clbr_iB = 1.209f*hfs->clbr_iB;	//0.021f;

		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			/* Номінальний струм в одиницях АЦП для трифазного двигуна  */
			if((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)>0){
				ratedAdcAmperage=hus->rated_amperage_set/((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)/3.0f);

			}
		}

	}
	else{}

	/* Обнуляємо лічильники, що використувуються при формуванні масивів відліків
	 * функція в файлі stm32f1xx_it.c*/
	ADC_ConvInit();
	/* Ініціалізуємо таймер для відліків Фурьє */
	MX_TIM7_Init();
	/* Обчислення на один-два порядки скорочуються, якщо не рахувати синуси/косинуси кожен раз
	 * При ініціалізації в масив записується таблиця синусів 								*/
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
	    /* чистимо відліки */
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

/* За результатами Фурьє-перетворення для масиву відліків напруги (18 відліків),
 * яким є
 * .rms - умовне середньоквадратичне значення напруги  та
 * .phase - фаза у градусах основної гармоніки напруги
 * уточнюємо частоту мережі енергопостачання.
 * В разі якщо обчислена частота мережі електропостачання
 * > 54Гц або < 46Гц функція повертає аварію частоти*/

static grid_calc_status_t f_grid_frequency_calc(grid_t *hg){

	if(hg==NULL){return GRID_clc_ZERO_PTR;}
	grid_calc_status_t fc_status = GRID_clc_UnKnown;
	memset ((void*)&uCfurie_out,0,sizeof(uCfurie_out));
    /* Обчислюємо Фурє параметри */
	f_ufurie(uFurieC, &uCfurie_out);
	/* починаємо уточнювати частоту після певного часу */
	if (fr_counter>20){
		/* Форматуємо значення фази 0..360 */
		if (uCfurie_out.u_phase<0){uCfurie_out.u_phase = uCfurie_out.u_phase +360.0;}
		/*різниця фази поточної та попередньої за п'ять періодів напруги   */
		currentPhaseDiff=uCfurie_out.u_phase-oldPhase;
		/*Форматуємо різницю фази -180..+180 в 0..360  */
		if (currentPhaseDiff>180.0f){currentPhaseDiff-=360.0f;}
		if (currentPhaseDiff<-180.0f){currentPhaseDiff+=360.0f;}
#ifdef		FREQUENCY_CALC_DEBUG0
        /*******************************************************/
		/*   ВІДЛАГОДЖЕННЯ                                     */
		/*******************************************************/
			ph[i_phd]=uCfurie_out.u_phase;
			phd[i_phd]=currentPhaseDiff;
			/* Середньоквадратичне відхилення */
			i_phd++;
			j_phd++;
			if ((i_phd%100)==0){
				i_phd=0;
				if ((j_phd%200)==0){
					j_phd=0;
//					HAL_Delay(10);
					/* Розрахунок середньоквадратичного відхилення */
					float sum_phdxphd=0;
					for (uint8_t i=0;i<100;i++){
						float dDiff=PhaseDiff-phd[i_phd];
						sum_phdxphd+=dDiff*dDiff;
					} /* 0.5 ... 0.8 град.*/
					volatile float diffStandardDeviation = sqrtf(sum_phdxphd/100);
					/**/
					phd_duration=HAL_GetTick()-grd_phd;
					grd_phd = HAL_GetTick();
				}
				memset ((void*)&ph,0,sizeof(ph));
				memset ((void*)&phd,0,sizeof(phd));
			}
	    /*******************************************************/
		/*   КІНЕЦЬ ВІДЛАГОДЖЕННЯ                                     */
		/*******************************************************/
#endif //FREQUENCY_CALC_DEBUG0
		/* Якщо різниця фаз надто велика - не враховуємо, пропускаємо  	   */
		if(fabsf(currentPhaseDiff)<29.0f){
			/*(100.0+newPhaseDiff*100)/360.0) тривалість у мілісекундах п'яти періодів напруги */
			float newPhaseDiff =0;
			/* Згладжуємо..*/
			f_ExponentialSmoothing  (
							currentPhaseDiff,
							PhaseDiff,
							20.0,
						   	10.0,
							&newPhaseDiff
						   	);
			PhaseDiff=newPhaseDiff;
			float flpd=fabsf(PhaseDiff);
				if(flpd<36.0f){/* Цикл вимірювань - 100мс, тобто п'ять циклів змінного струму мережі енергопостачання,
				                  вимірювання одного циклу з пяти дозволяє оперувати в межах +- 72град.
				                  беремо половину*/
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
				/* Аварія частоти   */
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

/* Ця функція обчислює напругу в вольтах , перемножуючи показник
 * АЦП напруги з перетворення Фур'є
 * на коефіцієнт напруги та зглажує напругу в вщльтах */
static grid_calc_status_t f_grid_voltage_calc(grid_t *hg){
	/* Ми отримали амплітуду 1-ї гармоніки напруги.
	 * Вважаємо, що амплітуда 1-ї гармоніки напруги пропорційна середньоквадратичній напрузі */
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

/* Ця функція:
   1) обробляє масиви відліків АЦП струмів iAfurie_out, iВfurie_out, iСfurie_out (по 18 відліків)
		  відліки iFurieLeak обчислюються як алгебраїчні суми відліків АЦП трьох фазних струмів
		  Ці 4 масиви обробляються з вик.перетворення Фур'є, які повертають:
		  структури iAfurie_out, iBfurie_out, iCfurie_out, iLeakfurie_out,
   1.3)	  кожна структура містить:
		rms_fundamental -    			root_mean-square, діючий струм основної гармоніки, відліки АЦП
	    gradf_t phase_fundamental-   	фаза основної гармоніки,  градуси
		adcf_t	rmsZero-			 	діючий струм струми гармонік, кратних трьом
	    adcf_t  rmsNegative-            діючий струм гармонік, кратних 3*к+2
   2) Обробляє масиви відліків АЦП струмів іА, іВ, іС, іLeak (по 200..210 відліків через 100 мкс, залежно від частоти
	  обчислюємо rms через суму квадратів елементарних струмів
   3) Зглажує нулі струмів всіх АЦП
   4) Обчислює глажені rms, зглажені теплові струми, в попугаях*/
static grid_calc_status_t f_grid_amperage_calc(grid_t *hg){
	/* Обробляємо масиви відліків 18 */
//	grid_calc_status_t	ac_status=GRID_clc_UnKnown;


	/* Обробляємо масив відліків 18 для однієї фази   */
	memset ((void*)(&iCfurie_out),0,sizeof(iCfurie_out));
	f_ifurie(	iFurieC,   &iCfurie_out);

	/* Обробляємо масив відліків 200 для однієї фази  */
	/* Число значимих відліків                        */
	float Nh = 	(uint16_t)(1.0f/(Frequency*0.1f*0.001f));
	memset ((void*)(&iCheat_out),0,sizeof(iCheat_out));
	f_iheat(	iC,
				cAdc0Amperage,
				Nh,
				&iCheat_out);
	/* Зглажуємо нулі струму в попугаях для  АЦП  C */
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

	/* Обчислюємо зглажені струми основної/першої гармоніки в в папугах
	 * ВАЖЛИВО - параметри зглажування однакові для 18 і 200*/
	float new1Amperage=FLT_MIN;
	/* Зглажуємо струми першої гармоніки однієї фази в попугаях*/
	f_ExponentialSmoothing  (
							iCfurie_out.rms_fundamental,
				   			cAdc1Amperage,
				    		2.0f,
				   			10.0f,
							&new1Amperage
				   			);
	cAdc1Amperage=new1Amperage;
	/* Зглажуємо теплові струми однієї фази в попугаях */
	float newAmperage=0;
	f_ExponentialSmoothing  (
								iCheat_out.rms_heat,
					   			cAdcAmperage,
					    		2.0f,
					   			10.0f,
								&newAmperage
					   			);
	cAdcAmperage=newAmperage;

	/* Тепловий струм однієї фази в амперах */
	cAmperage	= fs.clbr_iC*cAdcAmperage;
	hg->cAmperage_A=cAmperage;
	hg->mPOPUGAYS=(uint16_t)(cAdcAmperage*1.41f);
	hg->kPOPUGAYS=(uint16_t)(cAdc1Amperage*1.41f);
	if(fs.Faznost==1){
		/* Обчислюємо частку нагрівання для однофазного електродвигуна */
		if(us.oper_mode ==LowVoltageInductionMotorsProtection){
			/* Обчислюємо приведений струм - частка від номінативного для теплової моделі */
			ShareOfRated_0_1 = iAheat_out.rms_heat / ratedAdcAmperage;
			procOfRated=(cAdcAmperage/ratedAdcAmperage)*100.0f;
		}
	}


	if(fs.Faznost==3){
		/* Обробляємо відліки струму 18 для фаз А і Б     */
		memset ((void*)(&iAfurie_out),0,sizeof(iAfurie_out));
		f_ifurie(	iFurieA,   &iAfurie_out);

		memset ((void*)(&iBfurie_out),0,sizeof(iBfurie_out));
		f_ifurie(	iFurieB,   &iBfurie_out);
		/* Обробляємо відліки струму 200 для фаз А і Б*/
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
		/* Зглажуємо нулі струму в попугаях для  АЦП  A & B*/
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
		/* Обчислюємо зглажені струми основної/першої гармоніки в в папугах
		 * ВАЖЛИВО - параметри зглажування однакові для 18 і 200*/
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
		/* Обчислюємо зглажені теплові струми в попугаях */
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
			/* Обчислюємо приведений струм - частка від номінативного для теплової моделі*/
			ShareOfRated_0_1 = ((iAheat_out.rms_heat+iBheat_out.rms_heat+iCheat_out.rms_heat) / 3.0f) / ratedAdcAmperage;

			procOfRated=(((aAdcAmperage+bAdcAmperage+cAdcAmperage)/3.0f)/ratedAdcAmperage)*100.0f;
			memset ((void*)(&iLEAKheat_out),0,sizeof(iLEAKheat_out));
			/* Обчислюємо миттєві значення теплового струму, що витікає на землю,
			 *  як суму миттєвих значень лінійних струмів фаз*/
			/* Обчислюємо миттєві значення теплового струму, що витікає на землю,
			 *  як суму миттєвих значень лінійних струмів фаз*/
			memset ((void*)(&iLeak),0,sizeof(iLeak));
			/* Формуємо масив суми відліків 200 */
			for (uint8_t i=0;i<240;i++){
				float iL=   ((float)iA[i]-aAdc0Amperage)
						    +((float)iB[i]-bAdc0Amperage)
							+((float)iC[i]-cAdc0Amperage);

				iLeak[i]= (uint16_t)iL+2048;
			}
			/* Обробляємо масив суми відліків 200, тобто струми витоку  */
			f_iheat(
				iLeak,
				2048.0f,
				Nh,
				&iLEAKheat_out);
			/* обчислюємо зглажений тепловий струм витоку */
			float newAmperage=0;
			f_ExponentialSmoothing  (
				iLEAKheat_out.rms_heat,
				leakAdcAmperage,
				2.0f,
				10.0f,
				&newAmperage
			);
			leakAdcAmperage=newAmperage;
			/* Обчислюємо струм витоку в амперах								 */
			leakAmperage= ((fs.clbr_iA+fs.clbr_iB+fs.clbr_iC)/3.0f)*leakAdcAmperage;
		}
		hg->leakAmperage_A=leakAmperage;
		if(Voltage>lLimitRms*220.0f/100.0f){;
			float ave_adc_amperage=(aAdcAmperage+bAdcAmperage+cAdcAmperage)/3.0f;
			/* струм не нижче заданого   */
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
			snprintf((char*)str,17,  "ET %03d.%03d ацп", ic/1000, ic%1000);
	    	tkDisplay_PutHighStr(str);
			memset(&str,0,sizeof(str));
			ic=(uint32_t)(iCfurie_out.rms_fundamental*1000.0f);
			snprintf((char*)str,17,  "VK %03d.%03d aцп", ic/1000, ic%1000);
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
		/* Аналіз фаз можливий, коли амплітуда напруги і струму не нижче заданих */
		bool ub=(uCfurie_out.u1harm_ampl>(lLimitRms*rated220AdcVoltage/100.0f));
		/* амплітуда першої гармоніки струму фази С не нижче заданої   */
		bool ib=(iCfurie_out.rms_fundamental>(lLimitRms*ratedAdcAmperage/100.0f));
		bool ui=ub&&ib;
		if (!ui){
			PowerFactor=100.0f;

			grc_status=GRID_clc_Impossible;

			break;
		}
		/* Фази -180..+180 град перетворюємо в фази 0..360 град*/
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


		/* Для трихфазного струму і електродвигуна обчислюємо послідовність фаз */
		if((fs.Faznost==3)&&(us.oper_mode ==LowVoltageInductionMotorsProtection)){

			if(iAfurie_out.rms_fundamental < (lLimitRms*ratedAdcAmperage/100.0f)){
				phrotation=Phase_A_fault;break;
			}
			else if(iBfurie_out.rms_fundamental < (lLimitRms*ratedAdcAmperage/100.0f)){
				phrotation=Phase_B_fault;break;
			}
			else{}
			/* Фази -180..+180 град перетворюємо в фази 0..360 град*/

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
		snprintf((char*)str,17,  "фUc=%03d фIc=%03d", (int)uCfurie_out.u_phase, (int)iCfurie_out.phase_fundamental);
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

	/* Дифтрансформатора не має обчислюємо елементарні суми відліків*/
	for (uint8_t i=0;i<N_FURIE;i++){
		iFurieLeak[i]=  (iFurieA[i]-aAdc0Amperage)
				    	    +(iFurieB[i]-bAdc0Amperage)
							   +(iFurieC[i]-cAdc0Amperage);

	} //DIFFERNCIAL_CURRENT_TRANSFORMER
	/* находимо середньоквадратичне значення струму витоку
	 * першої гармоніки */
	f_ifurie(iFurieLeak,&iLEAKfurie_out);
	float new_leak_adc=0;
	/* згладжуємо струм витоку */
	f_ExponentialSmoothing  (
							iLEAKfurie_out.rms_fundamental,
							ave_leek_adc,
			    			20.0f,
			   				10.0f,
							&new_leak_adc
			   				);
	/* згладжуємо струм витоку */
	ave_leek_adc=new_leak_adc;
	/* якщо виявлено струм витоку, більший за якийсь заданий,
	 * наприклад білший ніж 1% від номінального пробуємо ідентифікувати
	 * яка фаза пробита на землю*/
	/* Там ми виявимо струм якоїсь фази, що витікає на замлю
	 * Тепер - як знайти - якої саме фази?
	 * У нас є фаза, але вона пливе через плавання частоти, але
	 * вона не пливе відносно фази напруги, лише різниться на 120 град*і + фі
	 * Для цього знайдемо зсув фази між струмом витоку і фазою С*/
	if (ave_leek_adc > ratedAdcAmperage/100.0f){
		float i_leak_ui_Phi = uCfurie_out.u_phase-iLEAKfurie_out.phase_fundamental;
		if (i_leak_ui_Phi< -180.0f){i_leak_ui_Phi=i_leak_ui_Phi+360.0f;}
		if (i_leak_ui_Phi>  180.0f){i_leak_ui_Phi=i_leak_ui_Phi-360.0f;}
		float new_uleak_phi=0;
		/* зглажуєму зсув фаз виявленого струму витоку та фази С*/
		f_ExponentialSmoothing  (
							i_leak_ui_Phi,
							leak_ui_Phi,
			    			20.0f,
			   				10.0f,
							&new_uleak_phi
			   				);
		leak_ui_Phi=new_uleak_phi;
		/* Якщо зсув між фазою струму витоку і фазою струму фази С - це фаза С */
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
		USA) Std. (1993) - визначення коефіцієнта асиметрії:
		найбільше відхилення від середнього значення
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
		/* Для трифазного електродвигуна 					*/
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
		snprintf((char*)str,17,  "Aсим=%02d%% Dr=%02d%%", (uint16_t)AmperageUnbalance, (uint16_t)UnbalanceDeratingFactor);
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
			/* Для однофазного електродвигуна */
			THDi=cTotalHarmonicDistortion;

	    }
	    else if (fs.Faznost==3){
	 	   float aD2 =   (aAdcAmperage*aAdcAmperage)
	 								   -(aAdc1Amperage*aAdc1Amperage);
	 	   /*THD -  безрозмірних величинах */
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



//		/* зглажений струм витоку першої гармоніки */
//		f_ExponentialSmoothing  (
//								iLEAKfurie_out.rms_fundamental,
//								leakAdc1Amperage,
//						    	20.0,
//						   		10.0,
//								&new1Amperage
//						   		);
//		leakAdc1Amperage=new1Amperage;



	   /* Квадрат теплових струмів усіх гармонік як різниця між
	    * сумою квадратів реальних елементарних струмів і
	    * квадрат теплового струму основної гармоніки*/
//	   volatile float summa_i2harm = summa_Qnf-iD2;
	   /* THD_I на основі теплових струмів основної гармоніки і сумарного теплового струму */
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


//		/* Струми, які обертають двигун в протилежну сторону*/
		/* Обчислюмо rms-струми негативних складових гармонік,
		 * ці струми крутять двигун в протилежну сторону    */
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

		/* Струми, що через гармоніки витікають на землю або в нейтраль,
		 * нагрівають нейтраль */
		/* Обчислюмо rms-струми нейтральних складових гармонік,
		 * ці струми витікають на землю або нуль  	*/
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
	    	/* Струми, які обертають двигун в протилежну сторону*/
	    	/* Обчислюмо rms-струми негативних складових гармонік,
	    	* ці струми крутять двигун в протилежну сторону    */
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

	    	/* Обчислюмо rms-струми неейтральних складових гармонік,
	    	 * ці струми витікають на землю або нуль           */
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

	  /* Тимчасово для роботи з витратами електоренергії ввдедемо коефіцієнти,
	   * які пізніше поправимо для обчислення миттєвих, в сенсі протягом 100мс
	   * значень rms струмів та фазної напруги*/



	  /* Повна енергія, по суті, нікому не цікава,
	   * оскільки платять за використану активну,
	   * а подекуда - за використану реактивну   */
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
//	  /* Cчитаем ват-секунды */
//	  WattSec_ElectricityConsumption.Active 	+=ActivePower 		* 0.1;
//	  WattSec_ElectricityConsumption.Reactive 	+=ReactivePower 	* 0.1;
//	  WattSec_ElectricityConsumption.Unbalance 	+=UnbalanceLostPower* 0.1;
//	  WattSec_ElectricityConsumption.Harmonic 	+=HarmonicLostPower * 0.1;
//	  WattSec_ElectricityConsumption.Neutral 	+=NeutralHarmonicLostPower* 0.1;
//	  WattSec_ElectricityConsumption.Negative 	+=NegativeHarmonicLostPower* 0.1;
//
//	  /* Раз на добу Cчитаем kWh */
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
//	  /* Якщо кінець мысяця записуємо в архів..*/

// }
//	/* сутки это 24*60*60 */
////http://www.mirusinternational.com/downloads/MIRUS-TP003-A-How%20Harmonics%20have%20Contributed%20to%20Many%20Power%20Factor%20Misconceptions.pdf
//
//void f_GRID_RaportHR(uint16_t* grid16){
//	/* Вимірювання:*/
////	 * 0		uint16_t, 0.01Гц		частоти від 46Гц до 54Гц,
//	 grid16[0]=(uint16_t)(Frequency*100);
////	 * 1		uint16_t  В						напруги (фаза С),
//	 grid16[1]=(uint16_t)(Voltage);
////	 * 234		uint16_t  0.1A, max 6553A середньоквадратичних струмів фаз АBC (теплових),
//	 grid16[2]=(uint16_t)(aAmperage);
//	 grid16[3]=(uint16_t)(bAmperage);
//	 grid16[4]=(uint16_t)(cAmperage);
////	 * 567		uint16_t  0.1A 		середньоквадратичних струмів першої гармоніки фаз АБС.
//	 grid16[5]=(uint16_t)(aAdc1Amperage);
//	 grid16[6]=(uint16_t)(bAdc1Amperage);
//	 grid16[7]=(uint16_t)(cAdc1Amperage);
//
////	 * 	Розрахунки:
////	 * 8		uint16_t  0.1кВт    активної потужності,
//	 grid16[8]=(uint16_t)(ActivePower/100.0f);
////	 * 9		uint16_t  0.1кВт 	реактивної потужності та
//	 grid16[9]=(uint16_t)(ReactivePower/100.0f);
////	 * 10		int16_t   %		    коефіцієнта потужності (сos fi),
//	 grid16[10]=(uint16_t)PowerFactor;
//
//	 //	 * 	12	uint8_t	  % 		коефіцієнт падіння потужності через асиметрію струмів,
////	 *  	uint8_t	  % 		на основі коефіцієнта асиметрії.
//	 grid16[11]=((uint16_t)(UnbalanceDeratingFactor))*0x100
//			 	 	 	 	 	 + (uint16_t)AmperageUnbalance;
//
////	 * 11		uint8_t   %			коефіцієнта падіння потужності через наявність гармонік струму,
////	 * 			uint8_t	  %         на основі коефіцієнта гармонік,
//	 grid16[12]=((uint16_t)(HarmonicsDeratingFactor))*0x100
//			 	 	 	 	 	 + (uint16_t)THDi_HarmonicAmperageDistortion;
//
//	 /* Процент струму, що витікає на землю, ключаючи як фазу на землю,
//	  * так і нульову послідовність гармонік */
//	 grid16[13]=(uint16_t)LeakAmperageDistortion * 0x100
//	 /* Процент струму, що витікає на землю
//	  * через замикання на                   */
//			 	 	 	 	 	 + (uint16_t)GrundAmperageDistortion;
//	 /* Процент струму, що витікає на землю
//	  * через нульову послідовність гармонік */
//
//	 grid16[14]=(uint16_t)NeutralAmperageDistortion * 0x100
//	 /* Процент струму гармонік, що довбає електродвигуни */
//			 	 	 	 	 	 + (uint16_t)NegativeAmperageDistortion;
//	 grid16[15]=0xFF00;
//
//
////	 *HarmonicsDeratingFactor=100.0;
////UnbalanceDeratingFactor=100.0;
////	 * 	Відомості по днях та місяцях про:
////	 * 0x10..0x2F	uint16_t 1..31    за поточний місяць
////	 *
////	 * Зчитування з ФЛЕШ
////	 * 0x30..0x70		4 х uint32_t 0..11(1..12) за місяць року
////	 *
////	 * 		кількість спожитої електроенергії (кВт.год),
////	 * 		кількість спожитої реактивної електроенергії (кВар.год),
////	 * 		кількість електроенергії, втраченої через наявність гармонік (кВар.год),
////	 * 		кількість електроенергії, втраченої на електроприводах через асиметрію (кВт.год).
////	 *
////	 * 	Виявлення:
////	 * 		0000 0001 напруги нижче допустимої,
////	 * 		0000 0010 та вище допустимої,
////	 * 		0000 0100 частота нижче допустимої,
////	 * 		0000 1000 частота вище допустимої,
////	 * 		0001 0000 пропадання фази,
////	 * 		0010 0000 неправильної послідовності фаз,
////	 * 		0100 0000 витоку струму на землю вище допустимого
////	 * 		1000 0000 резерв
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
		/* Якщо струм становить більше 7% від номінального -
		 * вважаємо що двигун нагрівається                */
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
		/* Якщо струм становить менше 7% від номінального -
		 * вважаємо що двигун охолоджується                 */
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
	//	/* частота відліків для TIM7 АЦП напруги/струму для Фур'є":*/
		float TIM7_Freq=N_FURIE*Frequency;
	//	/* де Ne 		- кількість відліків за період, файл 'tkConfig.h'
	//	 *    Frequency - поточна виміряна частота мережі, файл 'tkGrid.c' */
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
/* КОНТРОЛЬ ПОСЛІДОВНОСТІ ФАЗ ДЛЯ ТК2*/
//typedef enum{
//	PhasesUnknownTypeRotation,
//	PhasesDirectRotation,
//	PhasesReverceRotation,
//	PhasesSequenceFault,
//	Phase_A_fault,
//	Phase_B_fault
//}phases_rotation_type_t;
/* Це алгоритм, який пропагував Є.Т.
 * 1) Вибираємо якийсь рівень сигнлалу TK2_LINEZERO
 * 2) Виявляємо коли від початку відліку періоду
 * (всього хоч 200 хоч 100 відліків на період)
 * синусоїда перетинає даний рівень зниз вверх,
 * отримуємо по суті фази, але не в градусах, а відліках,
 * можна і в градуси перевести якщо було б треба..
 * 3) контролюємо послідовності - якщо АВС - прямий, якщо СВА - зворотній..
 * Це й все..
 */
/*1) Вибираємо якийсь рівень сигнлалу TK2_LINEZERO*/
#define TK2_LINEZERO 2088

static phases_rotation_type_t f_tk2_phase_rotation(
		volatile adcu16_t *p200a_array,/* масиви відліків фаз*/
		volatile adcu16_t *p200b_array,
		volatile adcu16_t *p200c_array,
		uint16_t Nn           /* кількість відліків*/
		/* Очевидно, що в тк2 відліки в масив не записуються,
		 * тому це саме треба робити в ході обробки відліків,
		 * в циклі, який вже є.. */
		){
	/* Результат */
	phases_rotation_type_t phR=PhasesUnknownTypeRotation;
	/* Щоб застрахуватися від збоїв - рахуємо відліки нижче заданої лінії, хоча б 3*/
	uint8_t iam=0;
	uint8_t ibm=0;
	uint8_t icm=0;
	/* Щоб застрахуватися від збоїв - рахуємо відліки вище заданої лінії, хоча б 3*/
	uint8_t iap=0;
	uint8_t ibp=0;
	uint8_t icp=0;
	/* Тобто, якщо було хоча б три нижче лінії, потім хоча б три вище - фіксуємо перехід через лінію*/
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
	/* Якщо перетину лінії не виявлено - фіксуємо аварію */
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
		/* контролюємо послідовності - якщо АВС - прямий, якщо СВА - зворотній..*/
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

