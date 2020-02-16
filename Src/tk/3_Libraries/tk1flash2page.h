/*
 * tk1flash2page.h
 *
 *  Created on: 18 ���. 2019 �.
 *      Author: KozakVF
 */

#ifndef TKLIBRARIES_TK1FLASH2PAGE_H_
#define TKLIBRARIES_TK1FLASH2PAGE_H_
#include "stdint.h"
#include "string.h"

#include "../3_Libraries/tk0flash_base.h"
/*
 * 1. �������� ��� ��������� ����������� ���� ������ �� 2 ������� ����,
 *  ��������� � ����� (����� ������� - 2��/2048����/0�800����)
 * size ������ - RecSize, ����'������ ����i�� ��i��� (1024, 512, 256, 128, 64, 32, 16..
 * ʳ������ ������ �� ������� - 0�800 / RecSize;*/

 flash_status_t f_write2page_record (uint32_t page_addr, /* ����� ������ �������  */
									 uint32_t *pRecord,  /* �������� �� ����� � RAM*/
									 size_t RecSize);    /* ����� ������ � ������ */
 /* 2. ��� ��������� ������ ��� ������ ������ ���� ����������
 * 2 ���� ������� ������ -����� �� �������, ���� �� ������� ��������
 * ������ � ��������� ���� ��������, � ��� � ��������, ���� ��������� �� �������
 * 3 ���� ������� - ��������, ���������� ��������� ���������� ������ ����� �������
 * 3.1. ���� �� �������� - ������� ����� �������, ������ � 0
 * 3.2. ���� ������ - ����� �� �������, ���� �� ������� ��������,
 * ������ � ��������� ���� �������� ��� � ��������, ���� ��������� �� �������
 * 4. ���� ��������==3 �������� �������� �������..
 * */
void f_look_next_flash_to_read_Init(void);
uint32_t f_look_start_flash_to_read(uint32_t page_addr, size_t RecSize);
uint32_t f_look_next_flash_to_read(uint32_t page_addr, size_t RecSize);

#endif /* TKLIBRARIES_TK1FLASH2PAGE_H_ */
