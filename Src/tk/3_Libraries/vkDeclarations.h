/*
 * vkDeclarations.h
 *
 *  Created on: 17 ñ³÷. 2020 ð.
 *      Author: KozakVF
 */

#ifndef TK_3_LIBRARIES_VKDECLARATIONS_H_
#define TK_3_LIBRARIES_VKDECLARATIONS_H_

typedef enum{
  NO_EVENT = 0,
  EVENT = !NO_EVENT
} event_t;
/* wk_motor_commands.h */
typedef enum{
	oFF_   =0x00,
	oN_    =0xFF,
	unknownOnOff=!((oFF_)||(oN_))
}on_off_t;
typedef enum{
	START,
	REVERSE,
	STOP,
	NO_MOTORST
}motor_st;
typedef enum{
	fast_measurement,
	slow_measurement
}measurement_t;

#endif /* TK_3_LIBRARIES_VKDECLARATIONS_H_ */
