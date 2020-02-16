/*
 * tk3_grid_declarations.h
 *
 *  Created on: 27 січ. 2020 р.
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
	/* Зглажена частота в герцах
	 * https://ua.energy/wp-content/uploads/2019/07/SOU-Zabezpechennya-kontrolyu-i-dotrymannya-pokaznykiv-yakosti-e_e-pry-peredachi.pdf
	нормально допустиме
	значення                    від 49,8 до 50,2 Гц,
	гранично допустиме значення від 49,2 до 50,8 Гц        */

	hzf_t 		Frequency_Hz;
	/* Зглажена напруга в вольтах*/
	voltf_t 	Voltage_V;
	/* Зглажені струми в амперах */
	amperf_t 	aAmperage_A;
	amperf_t 	bAmperage_A;
	amperf_t 	cAmperage_A;
	amperf_t	leakAmperage_A;
	/* Процент струму, що витікає на землю
 	 * через замикання на землю, обчислений по сумах відліків струму,
 	 * те саме що leakAmperage_A, але в процентах від номінального струму*/
	procentf_t GrundAmperageDistortion;

	/* Зглажені основні коєфіцієнти ефективності мережі  */
	/* Тип навантаження - індуктивние чи ємнісне */
	grid_load_t LoadType;
	phases_rotation_type_t PhaseRotation;
	/* Косинус фі*/
	procentf_t PowerFactor_pro;
	/* Тільки для електродвигуна, якщо навантаження не двигун - не має сенсу
	NEMA (National Electric Manufacturers Associations of the
	USA) Std. (1993) - визначення коефіцієнта асиметрії:
	найбільше відхилення від середнього значення
	https://wp.kntu.ac.ir/tavakoli/pdf/Journal/j.ijepes.2010.12.003.pdf
	Падіння потужності - через формулу асиметрії напруги              */
	procentf_t UnbalanceDeratingFactor_pro;

    /* Коефіцієнт ефективності, якщо 99%, то один процент ефективності втратили через гармоніки
     * також має сенс лише для електродвигуна  */
	procentf_t HarmonicsDeratingFactor_pro;

	/* Зглажені інтегральні показники ефективності мережі, також лише для двигуна  */
	procentf_t HmUnbDerating_pro;
	procentf_t fullDerating_pro;

	/* Коефiцієнт перекосу фаз,  має сенс не лише для едектродвигуна									               */
	procentf_t AmperageUnbalance;

	/* Показники енергоспоживання                                                  */
	kvatt_t	ActivePower_kW;
	kvatt_t	ReactivePower_kW;
	kvatt_t	UnbalanceLostPower_kW;//(лише для електродвигуна)
	kvatt_t	HarmonicLostPower_kW;

	/**********************************************************************************************/

	/*Зглажені параметри ефективності мережі 					                    */
	/*Коефіцієнт нелінійних спотворень по струму у кожній фазі                      */
	procentf_t aTotalHarmonicDistortion;
	procentf_t bTotalHarmonicDistortion;
	procentf_t cTotalHarmonicDistortion;

	/************************************************************************
	Коефіцієнт нелінійних спотворень по струму узагальнений    				*/
	procentf_t THDi_HarmonicAmperageDistortion;
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
	amperf_t bNegativeAmperage_A;
	amperf_t cNegativeAmperage_A;

	/* 3, 6, 9 .. 3k+3 - нічого не крутить, гріє провідники.
	 * Струм циркулюює між фазою та нейтраллю чи землею.
	 * На відміну від позитивної та негативної послідовності гармонійні
      струми третього порядку взаємно не компенсуються,
      а додадаються арифметично в нейтральній шині.*/
	/* Струми, що через гармоніки витікають на землю або в нейтраль,
	 * нагрівають нейтраль */
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
