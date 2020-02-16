/*
 * tk0flash_base.h
 *
 *  Created on: 27 ���. 2018 �.
 *      Author: Volodymyr.Kozak
 */

#ifndef TKLIBRARIES_TK1FLASH_BASE_H_
#define TKLIBRARIES_TK1FLASH_BASE_H_
/* Base address of Sector 7, 128 Kbytes */
#include <tkConfig.h>
#include "stdint.h"
#include <stdio.h>

/* �� �������� stm32f105rbtx �� 128 ����� ����, ����� ������� � 0..63
 * �� �������� - ���� 256 �����, ����� 128 �������
 * �� ������ �������������� ��������� ���� � 64 �� 127,
 * ����� ������� � ��������� ������ �������
 * ������� 64 0x08020000, �������� ������� - 127 0x0803F800
 * ������ �� ������ ������ ����������� ��
 * 0x0802000 + (FlashPageNumber - 64)* 0x800
 *
 * */

/* ��� ��� ������� */
#define FLASH_PAGE_100		0x08032000
#define FLASH_PAGE_101		0x08032800

/* ͳ��� �� ������*/
#define FLASH_PAGE_102		0x08033000
#define FLASH_PAGE_103		0x08033800
#define FLASH_PAGE_104		0x08034000
#define FLASH_PAGE_105		0x08034800
#define FLASH_PAGE_106		0x08035000
#define FLASH_PAGE_107		0x08035800
#define FLASH_PAGE_108		0x08036000
#define FLASH_PAGE_109		0x08036800
#define FLASH_PAGE_110		0x08037000
#define FLASH_PAGE_111		0x08037800
#define FLASH_PAGE_112		0x08038000

#define FLASH_PAGE_113		0x08038800
#define FLASH_PAGE_114		0x08039000
#define FLASH_PAGE_115		0x08039800
#define FLASH_PAGE_116		0x0803A000
#define FLASH_PAGE_117		0x0803A800
#define FLASH_PAGE_118		0x0803B000
#define FLASH_PAGE_119		0x0803B800
/* ������ ��� ���� ������� GRID*/
#define FLASH_PAGE_120		0x0803C000
#define FLASH_PAGE_121		0x0803C800

/*������������ ���������    */
#define FLASH_PAGE_122		0x0803D000
#define FLASH_PAGE_123		0x0803D800

/* ������������ ����������� */
#define FLASH_PAGE_124		0x0803E000
#define FLASH_PAGE_125		0x0803E800

/* ������ ������ ������.. ����� ���������� ���.. */
#define FLASH_PAGE_126		0x0803F000
#define FLASH_PAGE_127		0x0803F800


typedef enum {
	flash_condition_ok,
	flash_A_good_only,
	flash_B_good_only,
	flash_didnt_read_user,
	flash_brocken,
	flash_no_archive,
	flash_cond_unknown
}flash_condition_t;


typedef enum {
	FLASH_OK,
	FLASH_UNKNOWN_Err,
	FLASH_ZERO_Pointer,
	FLASH_WRONG_Arg,
	FLASH_READ_Err,
	FLASH_READ_CRC_Err,
	FLASH_WRITE_COMPARISON_Err,
	FLASH_WRITE_Err,
	FLASH_ERASE_Err,
	FLASH_DRIVER_Err,
	FLASH_TO_INSTALATION,
	FLASH_NO_PLASE_ON_PAGE,
	FLASH_RAM_COMPARISON_Err,
	VD_PRIMARY_PAGE_Err,
	VD_SECONDARY_PAGE_Err,
	VD_MENIAT_USTAVKU_Err,

	FLASH_Err
}flash_status_t;

void f_save (void);

#ifdef NUCLEO
/**************************************************************************************************************
 * �������� ������� ����
 *************************************************************************************************************/

flash_status_t f_flash_sector_erase (uint32_t sector_addr);
#endif

/**************************************************************************************************************
 * �������� ���в��� ���� stm32F105
 *************************************************************************************************************/
flash_status_t f_flash_page_erase (uint32_t addr_page); /* �������� �� ��������, � ���� �����                */
/*****************************************************************************************************************************
 * ����� ������ 32-��������� ���  � ���������ί ���'�Ҳ �� ���� (������ 1 - ���� ���������)
 *****************************************************************************************************************************
������ ���������� � ����� ������� ��������� ������ CRC
������ ������� ���� ������ ����� � ����� ���������� ������ � ���� � ������, �� ����������� � ���������� �����
*/
flash_status_t f_write_to_flash_record (uint32_t *u_from_RAM,
							     uint32_t u_to_FLASH,
							     size_t size_of_array
								 );

/*****************************************************************************************************************************
 * ����� ������ 32-��������� ��� � ���������ί ���'�Ҳ �� ���� (������ 2 ��������� i ������ ��� �����)
 ****************************************************************************************************************************
 �� ����� �� ������� 1, ��� �� ���������� ����� ��� ������� ����, � ���������� ������� ������ �� ����
 �� ���� ��������� ���� �������� � ���������, � ����� ������ ��� �����

 ������ ���������� � ����� ������� ��������� ������ CRC, ������ ��� ����� - ��� �RC

 ������ ������� ���� ������ ����� � ����� ���������� ������ � ���� � ������, �� ����������� � ���������� �����
 */
flash_status_t f_write_to_flash_rec_reg (uint32_t *u_rec_from_RAM,
							     	 	 uint32_t *u_rec_to_FLASH,
										 uint32_t size_of_array,
										 uint32_t reg,
										 uint32_t *u_reg_to_FLASH,
										 uint32_t *u_MemoryRecordStatus
								 );
/*****************************************************************************************************************************
 * ������� ������ 32-��������� ��� � ���� �� ���������ί ����Ҳ (������ 1 - ���� ���������)
 *****************************************************************************************************************************/
/* ������ � �������� CRC */
flash_status_t f_write_to_flash_rec_reg (uint32_t *u_rec_from_RAM,
							     	 	 uint32_t *u_rec_to_FLASH,
										 uint32_t size_of_array,
										 uint32_t reg,
										 uint32_t *u_reg_to_FLASH,
										 uint32_t *u_MemoryRecordStatus
								 );
/*****************************************************************************************************************************
 * ������� ������ 32-��������� ��� � ���� �� ���������ί ����Ҳ (������ 1 - ���� ������ ���)
 * (������ �������� CRC)
 *****************************************************************************************************************************/
flash_status_t f_read_from_flash_rec( uint32_t a_rec_from_FLASH,
							     	  uint32_t *u_rec_to_RAM,
								      size_t   size_of_array
								 );

uint32_t f_read_flash_word(uint32_t address);

/* ������� ������ ������� �� ������� ������� ������� 65..127 */
uint32_t f_flash_address(uint8_t in_flashpage_number);




//typedef struct {
//	uint8_t a0;
//	uint8_t a1;
//	uint8_t a2;
//	uint8_t a3;
//	uint8_t a4;
//	uint8_t a5;
//	uint8_t a6;
//	uint8_t a7;
//	uint32_t A8_11;
//	uint32_t A12_15;
//} str_t;
//
//typedef struct {
//	uint32_t b0_3;
//	uint32_t b4_7;
//	uint32_t b8_b11;
//	uint32_t b12_b15;
//} arr_t;
//flash_status_t f_check_write_record0 (	uint32_t *address_in_FLASH,
//										uint32_t *address_in_RAM,
//	 	 	 	 	 	 	 	 	 	uint32_t size_of_array);
//
//
//flash_status_t f_check_flash_record0 (	uint32_t *address_in_FLASH,
//										uint32_t *address_in_RAM,
//										uint32_t *p_MemoryRecordStatus,
//	 	 	 	 	 	 	 	 	 	uint32_t size_of_array);


#endif /* TKLIBRARIES_TK1FLASH_BASE_H_ */
