/*
 * tk1flash2page_write.c
 *
 *  Created on: 18 ���. 2019 �.
 *      Author: KozakVF
 */

#include "../3_Libraries/tk1flash2page.h"



static uint32_t f_look_for_flash_address (uint32_t page_addr, size_t RecSize);





flash_status_t f_write2page_record (uint32_t page_addr, /* ����� ������ �������  */
									 uint32_t *pRecord,  /* �������� �� ����� � RAM*/
									 size_t RecSize){    /* ����� ������ � ������ */
	flash_status_t w300_status = FLASH_UNKNOWN_Err;
	uint32_t flashAddr = f_look_for_flash_address (page_addr, RecSize);
	if (f_write_to_flash_record (	pRecord,
									flashAddr,
									RecSize
								 ) != FLASH_OK){
		if ((flashAddr - page_addr) < 0x800){
			w300_status=VD_PRIMARY_PAGE_Err;
		}
		else {
			w300_status=VD_SECONDARY_PAGE_Err;
		}
	}
	else {
		w300_status=FLASH_OK;
	}
//	uint32_t volatile ghg[8] = {0};
//	for (uint8_t i=0;i<8;i++){
//		ghg[i] = f_read_flash_word(0x08033000+512*i);
//	}
//	volatile uint32_t flashReadAddr = f_look_start_flash_to_read(page_addr, RecSize);
//	uint32_t volatile ljmkl[11]={0};
//
//	for (uint8_t i=0;i<11;i++){
//		ljmkl[i]=f_look_next_flash_to_read(page_addr, RecSize);
//	}
//	HAL_Delay(1);

	return w300_status;
}
/*
 * 1. �������� ��� ��������� ����������� ���� ��������� ������ �� 2 �������
 * ����, ��������� � �����
 * size ������ - RecSize, ����'������ ����i�� ��i��� (1024, 512, 256, 128, 64, 32, 16..
 * ������� ������ �� ������� - 0�800 / RecSize;
 * 2 ���� ������� ������ -����� �� �������, ���� �� ������� ��������
 * ������ � ��������� ���� ��������, � ��� � ��������, ���� ��������� �� �������
 * 3 ���� ������� - ��������, ���������� ��������� ���������� ������ ����� �������
 * 3.1. ���� �� �������� - ������� ����� �������, ������ � 0
 * 3.2. ���� ������ - ����� �� �������, ���� �� ������� ��������,
 * ������ � ��������� ���� �������� ��� � ��������, ���� ��������� �� �������
 * 4. ���� ��������==3 ������� �������� �������..
 * */


static uint32_t f_look_for_flash_address (uint32_t page_addr, size_t RecSize){
	/* ���� �� �� ������ ����� � ��� ������� */
	if ((page_addr % 0x1000) != 0) {return 0xFFFFFFFF;}
	/* ���� ����� ������ ����� �� ��-������� */
	if ((RecSize > 0x400) || (RecSize < 0x8)){return 0xFFFFFFFF;}
	/* ���� ����� ������ �� ������� ������ �������  */
	//if ( __builtin_popcount (RecSize) != 1 ){return 0xFFFFFFFF;};
	if ((4096 % RecSize) !=0) {return 0xFFFFFFFF;}
	uint32_t record_addr = 0xFFFFFFFF;
	uint32_t Nrecords = 0x1000 / RecSize;
	while (record_addr == 0xFFFFFFFF){
		/* ������ ����� ����� ���������� ������ ����� ������� */
		uint32_t wordtest = f_read_flash_word((page_addr+(Nrecords-1)*RecSize));
		if (wordtest==0xFFFFFFFF){
			/* ���� ��� ����� �� �������� */
			for (uint8_t i = Nrecords-1; i>0;i--){
				/* ���������� ����� ����� ������� ������ ��������� � ���������� �� ������� */
//				uint32_t addr = page_addr+(i-1)*RecSize;
				uint32_t wordtest1=f_read_flash_word(page_addr+(i-1)*RecSize);


				/* ���� ������� �������� ��������� ����� (i-1) - �������� */
				if (wordtest1 != 0xFFFFFFFF){
					/* ���� (i) - ����� ������, ���� ����� ������ */
					record_addr=page_addr+i*RecSize;
					break;

				}
				/* ���� ����� �� ��������� ������ � �� ������ -  */
				else if (wordtest1 == 0xFFFFFFFF){
					if(i==1){
						f_flash_page_erase (page_addr);
						record_addr=page_addr;
					}
				}
				else{}
			}
		}
		/* ���� �������� ������� ����� � ������� ���� ���������� ������ ����� ������*/
		else if (wordtest!=0xFFFFFFFF){
			/* ���������� ����� ����� ���������� ������ ����� ������� */
			uint32_t wordtest0 = f_read_flash_word(page_addr+(Nrecords/2-1)*RecSize);
			/* ���� ��� ����� �� �������� */
			if (wordtest0==0xFFFFFFFF){
				/* ���������� �� ������ ����� �������, ��������� � �������� */
				for (int8_t i = Nrecords/2-1; i>0;i--){
					wordtest0=f_read_flash_word(page_addr+(i-1)*RecSize);
					if (wordtest0 != 0xFFFFFFFF){
						/* ���� ������� ����� - ������ ������ ���������� ������ */
						record_addr=page_addr+i*RecSize;break;
					}
					else if(wordtest0 == 0xFFFFFFFF){
						/* ���� ����� �� ���� � ������� ���� �� �������,
						 * ���� ������ ���� �� �������   */
						if (i==1){
							f_flash_page_erase (page_addr);
							record_addr=page_addr;
						}
					}
					else{}
				}
				/* ���� ������� ����� ���� */
			}
			/* ���� � ����� ������� ���������,
			 * ���� ������ ���� �� ������� */
			else if (wordtest0 != 0xFFFFFFFF){
				f_flash_page_erase (page_addr);
				record_addr=page_addr;
			}
			else{}
		}
		else{}
	}
		/* ���� �������� ������� ����� ����� ������� */
		if (record_addr == page_addr+(Nrecords/2-1)*RecSize){
			/* ������� �������� ������ */
			f_flash_page_erase (page_addr+0x800);
		}
		/* ���� �������� ������� ����� ����� ������� */
		if (record_addr == page_addr+(Nrecords-1)*RecSize){
			/* ������� ����� ������ */
			f_flash_page_erase (page_addr);
		}

	return record_addr;
}

