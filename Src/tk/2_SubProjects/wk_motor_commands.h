/*
 * wk_motor_commands.h
 *
 *  Created on: 27 лист. 2019 р.
 *      Author: KozakVF
 */

#ifndef TK_2_SUBPROJECTS_WK_MOTOR_COMMANDS_H_
#define TK_2_SUBPROJECTS_WK_MOTOR_COMMANDS_H_
#include "stm32f1xx.h"
//#include "tkConfig.h"
extern FunctionalState CmndForMotor;


typedef enum{
	motor_ok                        = 0,
	motor_Amperage_Unbalance        = 1,
	motor_OverLoad                  = 2,
	motor_start_jummed   		    = 22,
	motor_operation_jummed		    = 23,
	motor_UnderLoad                 = 3,
	motor_insulation_fault          = 4,
	motor_differential_current      = 5,
	motor_dry_move					= 6,//
	motor_liquidSensor_fault        = 7,//
	motor_start_UnderVoltage        = 8,
	motor_UnderVoltage				= 81,
	motor_OverVoltage				= 82,
	motor_StartNumber_limit			= 9,
	motor_ThermalRelay_off			= 0x0A,
	motor_HumidityRelay_off			= 0x0B,
	motor_distance_cntr 			= 0x0C,
	motor_other_faults				= 0x0D,



	/* Неправильно задані параметри аналогового датчика тиску */
	motor_power_amperage_suss		 =0x80,
	no_pressure_sensor              = 0x86,
	pressure_sensor_setting_fault   = 0x87,

	motor_unknown_fault				= 0xFF
}motorfault_t;
extern motorfault_t MotorFault;
/* подаємо 1 на ногу RB5*/
void f_start(void);

/* подаємо 1 на ногу RB9*/
void f_reverce(void);

/* подаємо 1 на ногу RB8*/
void f_stop(void);

void f_motor_fault(motorfault_t motor_fault);

#endif /* TK_2_SUBPROJECTS_WK_MOTOR_COMMANDS_H_ */
