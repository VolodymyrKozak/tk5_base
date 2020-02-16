/*
 * vv300set_user.c
 *
 *  Created on: 9 лют. 2019 р.
 *      Author: KozakVF
 */

#include <vv300set_factory.h>
#include "vv300set_user.h"
#include "wk_motor_commands.h"
#include "float.h"
//#include "tkConfig.h"


user_settings_t user_settings = {
		//oper_mode_t oper_mode;
		LowVoltageInductionMotorsProtection,
		//FunctionalState motor_OverLoadProtection;
		ENABLE,
		//float rated_amperage_set;						//0x040A..B
		3.3f,
//		8.9f,			//	float rated_amperage_set,A;						//0x040A..B
		//uint16_t OverLoatInstProtectionClass;
		2,
		//uint16_t current_limit;							//0x040C
		102,			//	uint16_t current_tripping_limit, %;				//0x040C


		//FunctionalState motor_UnderLoadProtection;
		ENABLE,
		1.5f,
//		2.2f, 			//	float rated_motor_power,кВт;					//0x0408..9
		//uint16_t power_limit;
		20,				//	uint16_t power_tripping_limit;					//0x040D

		//FunctionalState motor_OverVoltageProtection;
		ENABLE,
		//uint16_t overvoltage_limit;
		160,

		//FunctionalState motor_AmperageUnbalanceProtection;
		ENABLE,
		//uint16_t unbalance_limit;
		40,

		//FunctionalState motor_GroundProtection;
		ENABLE,
		//uint16_t ground_leak_limit;
		15,
		//FunctionalState motor_StartNumberLimit;
		ENABLE,
		//uint16_t start_numbe_limit;
		4,
		//FunctionalState motor_JummedProtection;
		ENABLE,
		//uint16_t reserve0;
		0,

		//FunctionalState Phase_A_OverLoadProtection;
		ENABLE,
		//float Phase_A_current_tripping_limit;
		18.0,
		//FunctionalState Phase_B_OverLoadProtection;
		ENABLE,
		//float Phase_B_current_tripping_limit;
		18.0,
		//FunctionalState Phase_C_OverLoadProtection;
		ENABLE,
		//float Phase_C_current_tripping_limit;
		18.0,
		//CRC
		0
};



/* Синтактичний контроль уставок */
tk5_error_t f_set_testing(user_settings_t *t){
	tk5_error_t err=tk5_Unknown_Err;
	while(err==tk5_Unknown_Err){
		if(t->oper_mode==LowVoltageInductionMotorsProtection){
			t->motor_OverLoadProtection=ENABLE;
			if (!rated_amperage_test(t->rated_amperage_set)){
				err=tk5_RatedAmperageErr;
				break;
			}
			/* Якщо є підозри щодо коректності Потужності і ном.струму */
			if (t->rated_motor_power>FLT_MIN){
				if(((t->rated_amperage_set / t->rated_motor_power)>10.0f)||
					((t->rated_amperage_set / t->rated_motor_power)<3.0f)
					){
					err=tk5_AmperageVsPowerSetErr;break;
				}
				else {err=tk5_OK;}
			}
			else{
				err=tk5_zeroPower;
			}
		}
	}

	return err;
}
uint32_t  us32set[10]={0};

flash_status_t f_VV300_UserSets_Read(void){

	uint32_t size = sizeof(user_settings_t);
	uint32_t *p32uset = NULL;
	p32uset=(uint32_t*)&user_settings;
	return f_VV_Sets_Read(   p32uset,
							 size ,
							 FLASH_PAGE_USER_SETS_A,
							 FLASH_PAGE_USER_SETS_B
							);
}
flash_status_t f_VV300_UserSets_Write(void){
	uint32_t size = sizeof(user_settings_t);
	uint32_t *p32uset = NULL;
	p32uset=(uint32_t*)&user_settings;
	return f_VV_Sets_Write(
							p32uset,
							size,
							FLASH_PAGE_USER_SETS_A,
							FLASH_PAGE_USER_SETS_B
							);
}

