/*
 * wk_motor_commands.c
 *
 *  Created on: 27 лист. 2019 р.
 *      Author: KozakVF
 */


#include "wk_motor_commands.h"
#include "gpio.h"

FunctionalState CmndForMotor = DISABLE;
//motor_control_mode_t motor_control_mode=Manual;
motorfault_t MotorFault = motor_unknown_fault;


/* подаємо 1 на ногу RB5*/
void f_start(void){
//f(MotorFault == motor_ok){
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}
/* подаємо 1 на ногу RB9*/
//void f_reverce(void){
//	if(MotorFault == motor_ok){
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
//	}
//}
/* подаємо 1 на ногу RB8*/
void f_stop(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}


void f_motor_fault(motorfault_t motor_fault){

}
