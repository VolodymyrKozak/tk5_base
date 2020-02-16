/*
 * tkErrHandler.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKLIBRARIES_TKERRLIB_H_
#define TKLIBRARIES_TKERRLIB_H_

#include "stdint.h"

#define GET_tkERR() snprintf(errStr, 128,"%s %s %s %d %s %s ",__FILE__, __LINE__,__func__, errCode, __DATE__,__TIME__)

extern uint8_t errStr[128];
extern uint8_t errCode;


void woErrPrint (void);

#endif /* TKLIBRARIES_TKERRLIB_H_ */
