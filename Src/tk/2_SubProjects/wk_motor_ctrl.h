/*
 * wk_motor_ctrl.h
 *
 *  Created on: 25 лист. 2019 р.
 *      Author: KozakVF
 */

#ifndef TK_2_SUBPROJECTS_WK_MOTOR_CTRL_H_
#define TK_2_SUBPROJECTS_WK_MOTOR_CTRL_H_
#include "wk_motor_commands.h"


typedef enum {
	motor_oFF          =0x0,
	motor_oN_direct    =0xFF,
	motor_oN_reverce   =0x0F
}motor_state_t;














#endif /* TK_2_SUBPROJECTS_WK_MOTOR_CTRL_H_ */
