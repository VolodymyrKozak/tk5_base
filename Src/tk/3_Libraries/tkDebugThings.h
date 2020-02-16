/*
 * tkDebugThings.h
 *
 *  Created on: 10 ����. 2017 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKDEBUGTHINGS_H_
#define TKDEBUGTHINGS_H_
#include <stdint.h>
#include "i2c.h"
extern uint32_t delay3_5bytes;
extern uint32_t delay1_5bytes;



/************************************************************************
̳������������
���� �� ������ ������� ���, ���� ������� ���� �������� �� ansi_c ��
��������� ����� ������� ���������� ������ ������:


timing1= getUs()-start;
			* ��������� �������� *
		  start=getUs();
� �������� ��� � ������������
������������ ��� �������� ������ - 1 ���.12 �� :-),������ �������
**************************************************************************/
void delayUs(uint32_t micros);
/***************************************************************************/
/*�������� � ������������, ������ HAL_Delay (), */
uint32_t getUs (void);
/***************************************************************************/

#ifdef ENABLE_I2C
uint8_t f_I2C_ScanBus(void);
void i2c1BUSY_Doctor (void);
HAL_StatusTypeDef HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef * hi2c, GPIO_TypeDef * I2C_Port, uint16_t SCL_Pin, uint16_t SDA_Pin);
#endif //ENABLE_I2C
#endif /* TKDEBUGTHINGS_H_ */
