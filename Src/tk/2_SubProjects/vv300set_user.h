/*
 * vv300set_user.h
 *
 *  Created on: 26 лист. 2019 р.
 *      Author: KozakVF
 */

#ifndef TK_2_SUBPROJECTS_VV300SET_USER_H_
#define TK_2_SUBPROJECTS_VV300SET_USER_H_

#include <stdio.h>
#include "stm32f1xx.h"


#include "stm32_rtc.h"


typedef enum {
	password_user_access,
	free_user_access
} menu_access_t;


typedef enum {
 LowVoltageInductionMotorsProtection,
 LowVoltagePowerSupply
}oper_mode_t;

typedef enum{
	//10А	2..10 сек 	102..513 сек   300сек
	IEC_60947_Class10A   = 0,
	//10	4..10 сек	204..513 сек,  400сек
	IEC_60947_Class10    = 1,
	//20	6..20 сек	308..1026 сек/* Еквівалент Тк2 687сес */
	IEC_60947_Class20	 = 2,
	//30	9..30 сек	462..1540 сек  1000сек
	IEC_60947_Class30	 = 3
}motor_protection_relay_class_t;
/*
Час реакції при запуску електродвигуна з холодного стану при кратності перевищення номінативного струму 7.2 (ДСТУ IEC 60947-4-1
Клас реле:
10А 	2..10сек
10  	4..10сек
20		6..20сек
30 	постійна часу теплової моделі – 2..20сек/0.0195=102..1540сек.
За замовчуванням: 13 сек = 700сек постійна часу
*/





typedef struct {


	oper_mode_t oper_mode;                          //0х1000

	FunctionalState motor_OverLoadProtection;       //0х1001

	float rated_amperage_set;						//0x1002
	motor_protection_relay_class_t OverLoadInstProtectionClass;
	uint16_t current_limit;							//0x040C

	FunctionalState motor_UnderLoadProtection;
	float rated_motor_power;						//0x0408..9
	uint16_t power_limit;

	FunctionalState motor_OverVoltageProtection;
	uint16_t overvoltage_limit;

	FunctionalState motor_AmperageUnbalanceProtection;
	uint16_t unbalance_limit;

	FunctionalState motor_GroundProtection;
	uint16_t ground_leak_limit;

	FunctionalState motor_StartNumberLimit;
	uint16_t start_numbe_limit;

	FunctionalState motor_JummedProtection;
	uint16_t reserve0;

	FunctionalState Phase_A_OverLoadProtection;
	float Phase_A_current_tripping_limit;
	FunctionalState Phase_B_OverLoadProtection;
	float Phase_B_current_tripping_limit;
	FunctionalState Phase_C_OverLoadProtection;
	float Phase_C_current_tripping_limit;


	uint32_t usCRC;
} user_settings_t;
extern user_settings_t user_settings;



/********************************************************************
 * ******************************************************************
 */
typedef struct {
	uint32_t user_PUK;
	uint32_t user_PIN2;
	uint32_t tk_PIN;
}menu_access_params_t;
/************************************************************************************/


typedef enum{
	tk5_OK,
	tk5_RatedAmperageErr,
	tk5_AmperageVsPowerSetErr,
	tk5_zeroPower,
	tk5_Unknown_Err
}tk5_error_t;

tk5_error_t f_set_testing(user_settings_t *t);

#endif /* TK_2_SUBPROJECTS_VV300SET_USER_H_ */
