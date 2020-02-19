/*
 * tk0_grid_sv.c
 *
 *  Created on: 27 січ. 2020 р.
 *      Author: KozakVF
 */
#include "tk0_grid_sv.h"
#include "tk3_grid_declarations.h"
#include <vv300set_factory.h>
#include "vv300set_user.h"
#include <tk1_grid.h>
static grid_t gd={0};

static int16_t f_grid_to_modbus_regs(grid_t *g, uint16_t *m, uint16_t n);
static uint16_t dc(float a);
static uint16_t fr(float a);

int16_t GRID_Init(void){

	return f_GRD_Init(&fSet, &user_settings);
}
uint16_t GRIDRegsBlock[0x40]={0};
int16_t GRID_Loop(void){
	grid_calc_status_t gcs = f_GRD_Calc(&gd);
	f_grid_to_modbus_regs(&gd, &GRIDRegsBlock,0x40);
	return 0;
}


static int16_t f_grid_to_modbus_regs(grid_t *g, uint16_t *m, uint16_t n){

	m[0]	=0; 						m[1]=1;
	m[2]	=dc(g->Frequency_Hz);
	m[3]=fr(g->Frequency_Hz);
	/* Зглажена напруга в вольтах*/
	m[4]	=dc(g->Voltage_V);
	m[5]=fr(g->Voltage_V);
	/* Зглажені струми в амперах */
	m[6]	=dc(g->aAmperage_A);		m[7]=fr(g->aAmperage_A);
	m[8]	=dc(g->bAmperage_A);		m[9]=fr(g->bAmperage_A);
	m[0xA]	=dc(g->cAmperage_A);		m[0xB]=fr(g->cAmperage_A);
	m[0xC]	=dc(g->leakAmperage_A);		m[0xD]=fr(g->leakAmperage_A);
	/* Процент струму, що витікає на землю
 	 * через замикання на землю, обчислений по сумах відліків струму,
 	 * те саме що leakAmperage_A, але в процентах від номінального струму*/
	m[0xE]=dc(g->GrundAmperageDistortion);	m[0xF]=fr(g->GrundAmperageDistortion);
	/* Зглажені основні коєфіцієнти ефективності мережі  */
	/* Тип навантаження - індуктивние чи ємнісне */
	m[0x10]=(uint16_t)(g->LoadType);		m[0x11]=(uint16_t)(g->PhaseRotation);

	/* Косинус фі*/

	m[0x12]=dc(g->PowerFactor_pro);			m[0x16]=fr(g->PowerFactor_pro);
	/* Тільки для електродвигуна, якщо навантаження не двигун - не має сенсу
	NEMA (National Electric Manufacturers Associations of the
	USA) Std. (1993) - визначення коефіцієнта асиметрії:
	найбільше відхилення від середнього значення
	https://wp.kntu.ac.ir/tavakoli/pdf/Journal/j.ijepes.2010.12.003.pdf
	Падіння потужності - через формулу асиметрії напруги              */

	m[0x13]=dc(g->UnbalanceDeratingFactor_pro);
    /* Коефіцієнт ефективності, якщо 99%, то один процент ефективності втратили через гармоніки
     * також має сенс лише для електродвигуна  */
	m[0x14]=dc(g->HarmonicsDeratingFactor_pro);m[0x15]=fr(g->HarmonicsDeratingFactor_pro);
	/* Зглажені інтегральні показники ефективності мережі, також лише для двигуна  */

	//m[0x16]=резерв



	/* Коефiцієнт перекосу фаз,  має сенс не лише для едектродвигуна									               */

	m[0x17]=dc(g->AmperageUnbalance);

	/********************************************************************************************
	 * ПОКАЗНИКИ ПОТУжНОСТІ
	 * ******************************************************************************************/
	/* Активна потужність*/
	m[0x18]=dc(g->ActivePower_kW);			m[0x19]=fr(g->ActivePower_kW);
	/* Реактивна потужність*/
	m[0x1A]=dc(g->ReactivePower_kW);		m[0x1B]=fr(g->ReactivePower_kW);
	/* Потужність втрачена електродвигуном внаслідок асиметрії струму (лише для електродвигуна)*/
	m[0x1C]=dc(g->UnbalanceLostPower_kW);	m[0x1D]=fr(g->UnbalanceLostPower_kW);
	/* Потужність, втрачена електродвигуном внаслідок несинусоїдальності струму */
	m[0x1E]=dc(g->HarmonicLostPower_kW);	m[0x1F]=fr(g->HarmonicLostPower_kW);

	/**********************************************************************************************/

	/*Зглажені параметри ефективності мережі 					                    */
	/*Коефіцієнт нелінійних спотворень по струму у кожній фазі                      */
	m[0x20]=dc(g->aTotalHarmonicDistortion);	m[0x21]=fr(g->aTotalHarmonicDistortion);
	m[0x22]=dc(g->bTotalHarmonicDistortion);	m[0x23]=fr(g->bTotalHarmonicDistortion);
	m[0x24]=dc(g->cTotalHarmonicDistortion);	m[0x25]=fr(g->cTotalHarmonicDistortion);

	/************************************************************************
	Коефіцієнт нелінійних спотворень по струму узагальнений    				*/
	m[0x26]=dc(g->THDi_HarmonicAmperageDistortion);	m[0x27]=fr(g->THDi_HarmonicAmperageDistortion);
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
    /* 1,4,7,10 3k+1 - діє в тому ж напрямку обертання сіпає двигун, але сприяє зростанню крутного моменту*/

	/* 2, 5, 8, 11   - буде діяти проти напрямку обертання, що призводить до значних пульсацій крутного моменту*/

	/* Струми, які обертають двигун в протилежну сторону*/
	amperf_t aNegativeAmperage_A;
	m[0x28]=dc(g->aNegativeAmperage_A);	m[0x29]=fr(g->aNegativeAmperage_A);
	amperf_t bNegativeAmperage_A;
	m[0x2A]=dc(g->bNegativeAmperage_A);	m[0x2b]=fr(g->bNegativeAmperage_A);
	amperf_t cNegativeAmperage_A;
	m[0x2C]=dc(g->cNegativeAmperage_A);	m[0x2d]=fr(g->cNegativeAmperage_A);
	/* 3, 6, 9 .. 3k+3 - нічого не крутить, гріє провідники.
	 * Струм циркулюює між фазою та нейтраллю чи землею.
	 * На відміну від позитивної та негативної послідовності гармонійні
      струми третього порядку взаємно не компенсуються,
      а додадаються арифметично в нейтральній шині.*/
	/* Струми, що через гармоніки витікають на землю або в нейтраль,
	 * нагрівають нейтраль */

	m[0x2E]=dc(g->aNeutralAmperage_A);	m[0x2f]=fr(g->aNeutralAmperage_A);

	m[0x30]=dc(g->bNeutralAmperage_A);	m[0x31]=fr(g->bNeutralAmperage_A);

	m[0x32]=dc(g->cNeutralAmperage_A);	m[0x33]=fr(g->cNeutralAmperage_A);

	m[0x34]=dc(g->sumNeutralAmperage_A);m[0x35]=fr(g->sumNeutralAmperage_A);

	m[0x36]=dc(g->leakFurie);			m[0x37]=fr(g->leakFurie);

//	m[0x38]=(uint16_t)(g->leakFuriePhase);m[0x39]=0x39;



	m[0x38]=g->mPOPUGAYS;
	m[0x38]=g->kPOPUGAYS;


	m[0x3A]=dc(g->MotorHeatProcentage);	m[0x3B]=fr(g->MotorHeatProcentage);
	m[0x3C]=0x3C;	m[0x3D]=0x3D;
	m[0x3E]=0x3E;	m[0x3F]=0x3F;
	return 0;
}

static uint16_t dc(float a){
	return (uint16_t)a;
}
static uint16_t fr(float a){
	float b= a*10000.0f;
	return (uint32_t)b % 10000;
}
void f_hgt(void){
//	if (Voltage>220.0f*hLimitVoltage/100.0f){return GRID_VoltageFault;}
//	else if (Voltage<220.0f*lLimitVoltage/100.0f){return GRID_VoltageFault;}
//	else{}
}
