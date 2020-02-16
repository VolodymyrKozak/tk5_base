/*
 * tk1flash2page_read.c
 *
 *  Created on: 18 ���. 2019 �.
 *      Author: KozakVF
 */

#include "../3_Libraries/tk1flash2page.h"

static struct {
	uint32_t 	page_addr;
	size_t 		RecSize	 ;
    uint8_t 	N 		 ;
    uint8_t 	l_start  ;
    uint8_t     l_i		 ;
} l = {0};


void f_look_next_flash_to_read_Init(void){
	memset (&l, 0, sizeof(l));
}

uint32_t f_look_next_flash_to_read(uint32_t page_addr, size_t RecSize){
	if (page_addr != l.page_addr){ return  0xFFFFFFFF;}
	/* ���� ����� ������ ����� �� ��-������� */
	if ((RecSize > 0x400) || (RecSize < 0x8)){return 0xFFFFFFFF;}
	/* ���� ����� ������ �� ������� ������ �������  */
	//if ( __builtin_popcount (RecSize) != 1 ){return 0xFFFFFFFF;};
	if ((0x800 % RecSize) !=0) {return 0xFFFFFFFF;}
	/* ���� ���������� � ����� ������� */
	if (l.l_start >= l.N){
		/* � ��������� ������ - ����� ������� ������*/
		if (l.l_start == (l.N*2-1)){
			/* ��������� ���� ����� �������*/
			if (l.l_i >l.N){l.l_i--;}
		}
		else{
			/* �������� ����� ������ �� ����, ��������� ����� � ����� ������� */
			if (l.l_i >0){l.l_i--;}
		}
	}
	/* ���� ���������� � ����� ������� */
	else if ((l.l_start < l.N)){
		/* ���� � ������������ ���� ����� ������� ��� ������� �� ����*/
		if ((l.l_i == 0)){
			/* � ���� �� ��� ���������� ������ �� ������ ����� �������,
			 * �� ����� �� ������ ��� ������� */
			if (l.l_start!=(l.N-1)){
				 /* �������� � �������� ������ ������ �� ����� �������,
				 * ����, ��� ������� ���������.. */
				l.l_i = l.N*2-1;
			}
		}
		/* ���� �� �� �������� ����� */
		else {
			/* ��������� ������� �� ���� */
			if ((l.l_i <l.N) &&(l.l_i >0)){l.l_i--;
			/* ���� ���� ���������� �� ����� ������� */
			}
			/* i ��������� ��� ����� ������� */
			else if (l.l_i >l.N) {
				l.l_i--;
			}
			else{}
		}
	}
	else {}
	return  (page_addr+l.l_i*RecSize);
}


uint32_t f_look_start_flash_to_read (uint32_t page_addr, size_t RecSize){
	/* ���� �� �� ������ ����� � ��� ������� */
	if ((page_addr % 0x1000) != 0) {return 0xFFFFFFFF;}
	/* ���� ����� ������ ����� �� ��-������� */
	if ((RecSize > 0x400) || (RecSize < 0x8)){return 0xFFFFFFFF;}
	/* ���� ����� ������ �� ������� ������ �������  */
	//if ( __builtin_popcount (RecSize) != 1 ){return 0xFFFFFFFF;};
	if ((0x800 % RecSize) !=0) {return 0xFFFFFFFF;}

	uint32_t record_addr = 0xFFFFFFFF;
	uint32_t Nrecords = 0x1000 / RecSize;
	f_look_next_flash_to_read_Init();
	l.page_addr			=page_addr;
	l.RecSize			=RecSize;
    l.N 				=0x800 / RecSize;

	while (record_addr == 0xFFFFFFFF){
		/* ������ ����� ����� ���������� ������ ����� ������� */
		uint32_t wordtest = f_read_flash_word((page_addr+(Nrecords-1)*RecSize));
		if (wordtest==0xFFFFFFFF){
			/* ���� ��� ����� �� �������� */
			for (uint8_t i = Nrecords-1; i>0;i--){
				/* ���������� ����� ����� ������� ������ ��������� � ���������� �� ������� */
//				uint32_t addr = page_addr+(i-1)*RecSize;
				uint32_t wordtest1=f_read_flash_word(page_addr+(i-1)*RecSize);
				/* ���� ������� �������� ��������� ����� (i-1) - �������� 	*/
				if (wordtest1 != 0xFFFFFFFF){
					/* ���� (i-1) - ����� ���������� ������					*/
					record_addr=page_addr+(i-1)*RecSize;
					/* ������� ����� ���������� ������ ��� ���������� ������/����� ��� ����� ������ */
					l.l_start = i-1;
					l.l_i = l.l_start;
					break;

				}
			}
		}
		/* ���� �������� ������� ����� � ������� ���� ���������� ������ ����� �������*/
		else if (wordtest!=0xFFFFFFFF){
			/* ���������� ����� ����� ���������� ������ ����� ������� */
			uint32_t wordtest0 = f_read_flash_word(page_addr+(Nrecords/2-1)*RecSize);
			/* ���� ��� ����� �� �������� */
			if (wordtest0==0xFFFFFFFF){
				/* ���������� �� ������ ����� �������, ��������� � ���������� */
				for (int8_t i = Nrecords/2-1; i>0;i--){
					wordtest0=f_read_flash_word(page_addr+(i-1)*RecSize);
					if (wordtest0 != 0xFFFFFFFF){
						/* ���� ������� ����� - ������ ������ ���������� ������ */
						record_addr=page_addr+(i-1)*RecSize;
						l.l_start = i-1;
						l.l_i = l.l_start;
						break;
					}
					else if (wordtest0 == 0xFFFFFFFF){
						if (i==1){
							/* ���� �� ������ ������� ������ �� ������� -
							 * ���������� ������� � ������� ����� ����� ������� */
							l.l_start = l.N*2-1;
							l.l_i = l.l_start;
							record_addr=page_addr+(l.l_start)*RecSize;
						}
					}
					else{}
				}
			}
		}
		else{}
	}


	return record_addr;
}
