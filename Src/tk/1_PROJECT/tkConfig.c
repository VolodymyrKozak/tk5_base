/*
 * tkConfig.c
 *
 *  Created on: 23 ���. 2019 �.
 *      Author: KozakVF
 *  Created on: 23 ���. 2019 �.
 *      Author: Volodymyr.Kozak
 */

#include "../1_PROJECT/tkConfig.h"

#ifdef WINSTAR_Display_WH1602B3
#include "tk_wh1602b3.h"
#endif
#ifdef WINSTAR_Display_WO1602G
#include "tk_wo1602g.h"
#endif
#include "string.h"
#include "zFF.h"


