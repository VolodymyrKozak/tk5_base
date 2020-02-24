/*
 * wk_motor_ctrl.c
 *
 *  Created on: 25 лист. 2019 р.
 *      Author: KozakVF
 */

#include "wk_motor_ctrl.h"
#include "../1_PROJECT/tkConfig.h"
#include "../3_Libraries/vkDeclarations.h"

/* Остання команщда для мотора.
 * Якщо була натиснута кнопка START и РЕВЕРС
 * чи відпов.вигнали від датчиків  - oN_
 * Якщо була натиснута кнопка STOP
 * чи відповідні сигнали від датчиків - oFF_*/
on_off_t LastCmndForMotor  = oFF_;
/* За поданою командою мотор міг включитися, а міг і не включитится,
 * це реальний стан, який визначається по величині робочого струму */
motor_state_t MotorState  = oFF_;
uint32_t MotorOperationalLife=0;


