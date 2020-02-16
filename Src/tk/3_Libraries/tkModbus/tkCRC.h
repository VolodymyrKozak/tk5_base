/*
 * tkCRC.h
 *
 *  Created on: 15 זמגע. 2017 נ.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKCRC_H_
#define TKCRC_H_

#include "tkConfig.h"
#include "mb.h"
#include "stdint.h"

#if defined (MASTERRTU_HUART_4) || defined (MASTERRTU_HUART_5)
modbus_status_t f_add_CRCm(modbus_master_tx_msg_t *tx);
modbus_status_t f_check_CRCm(modbus_master_rx_msg_t *rx);
#endif
#if defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
modbus_status_t f_add_CRCs(modbus_slave_tx_msg_t *tx);
modbus_status_t f_check_CRCs(modbus_slave_rx_msg_t *rx);
#endif
uint16_t CRC_16x (uint8_t *Buffer, uint8_t length);

#endif /* TKCRC_H_ */
