/*
 * vv300Flash.c
 *
 *  Created on: 11 ���. 2019 �.
 *      Author: KozakVF
 */


#include "../2_SubProjects/vv300Flash.h"

#include "rtc.h"

/*
 * vd_flashTest.c
 *
 *  Created on: 14 ���. 2019 �.
 *      Author: VDShulyak
 */
#include "stdint.h"

#include "../2_SubProjects/wk_atime64.h"
#include "../3_Libraries/tk0flash_base.h"


/* ������ ������ �������
 * ������ ������� ����������� � �� � ������ pSettingD [set_arr_size/4] ��
 * FlashPage_A, ��� FlashPage_B, ���� �������� �������� �� ��������.
 ������������ ��������:
 	 FLASH_OK 				- ������ ������� �������� ���� �� �� ����� ��������
 	 VD_MENIAT_USTAVKU_Err  - ������ ������� �� �������� �� �� ����� ��������
 */
flash_status_t f_VV_Sets_Read  ( uint32_t *pSettingD,
								 uint32_t  set_arr_size,
								 uint32_t  FlashPage_A,
								 uint32_t  FlashPage_B
								){
	volatile flash_status_t sr_status = FLASH_UNKNOWN_Err;

	while (sr_status != FLASH_OK){
		/* ��������� ������ �� �������� �������� ���� � �� � Settings_TK_fl*/
		sr_status= f_read_from_flash_rec (
											 FlashPage_A,
											 pSettingD,
											 set_arr_size
										 );
		/* ���� ������ ������� �� ���������� � �������� ��������, ������ �� ��������        */
		if (sr_status != FLASH_OK){
			sr_status= f_read_from_flash_rec (
											FlashPage_B,
											pSettingD,
											set_arr_size
											  );
		}
		/* ���� ������ �� �������� �� �� ��������, �� �� �������� �������� - ��������� ������
		 * � ���������� ���������� ������� */
		if (sr_status != FLASH_OK){
			sr_status=VD_MENIAT_USTAVKU_Err;
			break;
		}
		sr_status = FLASH_OK;
	}
	return sr_status;
}

/* ��������� ������ ������� �� ����
 * ������ ������� pSettingD [set_arr_size/4] �� �� ������������ � ���� �� ��������
 * FlashPage_A � ����������� �� �������� ����  FlashPage_B,
  ������������ ��������:
 	 FLASH_OK 				- ������� ������������ � ���� ������� �� ��� ��������
 	 VD_PRIMARY_PAGE_Err	- �������� �������� �����
 	 VD_SECONDARY_PAGE_Err	- �������������� �������� �����
 	 VD_MENIAT_USTAVKU_Err  - ��� �������� �����, ���������� ����������
 */
flash_status_t f_VV_Sets_Write ( uint32_t *pSettingD,
								 uint32_t  set_arr_size,
								 uint32_t  FlashPage_A,
								 uint32_t  FlashPage_B
								){

	volatile flash_status_t sw_status = FLASH_UNKNOWN_Err;

	uint32_t vdMemoryRecordStatus=0;

	while (!((sw_status == FLASH_OK)||
			(sw_status == VD_PRIMARY_PAGE_Err)||
			(sw_status == VD_SECONDARY_PAGE_Err))){

		/* ������� �������� ��������*/
		sw_status = f_flash_page_erase(FlashPage_A);

		/* ���������� ����������������� ������ ������� �� �������� �������� ����
		 * ��� ���� ������ � ��������� ����� ������� CRC � ��������� ��������
		 * ����������� �� ���� � ����������� � �� - ������ ������� ������    */
		if (sw_status == FLASH_OK){
			sw_status= f_write_to_flash_record (
									 pSettingD,
								     FlashPage_A,
									 set_arr_size
									 );
		}
		/* e��� ������ ������ ��� ������ ������ �������� �� ���� � �� */
		if (sw_status != FLASH_OK) {
			/* ������� �������� �������� � ���������� �������� ������ */
			sw_status=f_flash_page_erase(FlashPage_A);
			if (sw_status != FLASH_OK){
				sw_status= f_write_to_flash_record (
									 pSettingD,
									 FlashPage_A,
									 set_arr_size
									);
			}
			if ((sw_status != FLASH_OK) || (vdMemoryRecordStatus!=0)){
				/* ���� � ������ ��� �� ���������� �������� */
				sw_status = VD_PRIMARY_PAGE_Err;
			}
		}
		/* ���������� ����������������� ������ ������� �� �������������� �������� ����
		* ��� ���� ������ � ��������� ����� ������� CRC � ��������� ��������
		* ����������� �� ���� � ����������� � �� - ������ ������� ������    		*/
		flash_status_t sw_status_SEC = FLASH_UNKNOWN_Err;
		/* ������� �������������� ��������*/
		sw_status_SEC = f_flash_page_erase(FlashPage_B);
		if (sw_status_SEC == FLASH_OK){
			sw_status= f_write_to_flash_record (
										pSettingD,
										FlashPage_B,
										set_arr_size
										);
		}
		/* e��� ������ ������ ��� ������ ������ �������� �� ���� � �� */
		if ((sw_status_SEC != FLASH_OK) || (vdMemoryRecordStatus!=0)){
		/* ������� ������������� �������� � ���������� �������� ������ */
			sw_status=f_flash_page_erase(FlashPage_B);
			if (sw_status_SEC == FLASH_OK){
				sw_status_SEC= f_write_to_flash_record (
										pSettingD,
										FlashPage_B,
										set_arr_size
										);
			}
		}
		if ((sw_status_SEC != FLASH_OK) || (vdMemoryRecordStatus!=0)){
		/* ���� � ������ ��� �� ���������� �������� */
			sw_status_SEC = VD_SECONDARY_PAGE_Err;
		}
		if ((sw_status==VD_PRIMARY_PAGE_Err) && (sw_status_SEC == VD_SECONDARY_PAGE_Err)){
			sw_status=VD_MENIAT_USTAVKU_Err;
		}
		else if ((sw_status==VD_PRIMARY_PAGE_Err)&& (sw_status_SEC == FLASH_OK)){
			sw_status=VD_PRIMARY_PAGE_Err;
		}
		else if ((sw_status==FLASH_OK)	&& (sw_status_SEC == VD_SECONDARY_PAGE_Err)){
			sw_status=VD_SECONDARY_PAGE_Err;
		}
		else{}
	}
	return sw_status;
}
//void f_vkFlashTest(void){
//	volatile flash_status_t vfFlashOpStatus = FLASH_UNKNOWN_Err;
///*1) ����� � ��� � ����� 200 ����� */
//int32_t ArraySettings[200]={0};
//int32_t ArrayRx[200]={0};
//uint32_t* u32 = NULL;
//u32=(uint32_t*)ArrayRx;
//	/*4) ������� �� ������ ����� ��� �������� CRC				*/
//			for (uint8_t i=0;i<128;i++){
//				u32[i]=f_read_from_flash_word(FLASH_PAGE_A_START_ADDR+4*i);;
//			}
//	/* 2) ������� ������� 120 */
//	/**************************************************************************************************************
//	* �������� ���в��� ���� stm32F105
//	*************************************************************************************************************/
//

//			f_flash_page_erase(FLASH_PAGE_A_START_ADDR); /* �������� �� ��������, � ���� �����                */
//	/*****************************************************************************************************************************
//*/
//			for (uint8_t i=0;i<128;i++){
//				ArrayRx[i]=(uint32_t)i;
//			}
//
//
///*****************************************************************************************************************************
// * ����� ������ 32-��������� ���  � ���������ί ���'�Ҳ �� ���� (������ 1 - ���� ���������)
// *****************************************************************************************************************************
//������ ���������� � ����� ������� ��������� ������ CRC
//������ ������� ���� ������ ����� � ����� ���������� ������ � ���� � ������, �� ����������� � ���������� �����
//*/
//		uint32_t vdMemoryRecordStatus=0;
//		uint32_t* u32ArrayRx = (uint32_t*)ArrayRx;
//
//		vfFlashOpStatus= f_write_to_flash_record (
//								 u32ArrayRx,
//							     (uint32_t*)FLASH_PAGE_A_START_ADDR,
//							     sizeof ArrayRx,
//								 &vdMemoryRecordStatus
//								 );
///* 5) �������� �������� ����� 									*/
//		memset (ArrayRx, 0, sizeof(ArrayRx));
///* 6) ��������� � ���� � ����� � ����� ���� ����� � ���������� CRC*/
///*****************************************************************************************************************************
//* ������� ������ 32-��������� ��� � ���� �� ���������ί ����Ҳ (������ 1 - ���� ������ ���)
//* (������ �������� CRC)
//*****************************************************************************************************************************/
////		vfFlashOpStatus= f_read_from_flash_rec (
////												 (uint32_t *)FLASH_PAGE_A_START_ADDR,
////												  ArrayRx,
////											      sizeof (ArrayRx)
////											   );
//
//		uint32_t* pSettingD =NULL;
//		pSettingD=(uint32_t*)Settings_TK_fl;
//		vfFlashOpStatus= f_read_from_flash_rec (
//												  (uint32_t *)FLASH_PAGE_A_START_ADDR,
//												  pSettingD,
//												  sizeof (Settings_TK_fl)
//												);
//
//		/* �������� ������� ����� USTAVKA ����� I_UST*/
//
//
//
////
////	/*1) ��������� ������� ������ ������� �����:*/
////		for (uint8_t i=0;i<128;i++){
////			ArraySettings[i]=i;
////		}
////	/* 2) ������� ������� 120 */
////	/**************************************************************************************************************
////	 * �������� ���в��� ���� stm32F105
////	 *************************************************************************************************************/
////
////		f_flash_page_erase(FLASH_PAGE_63); /* �������� �� ��������, � ���� �����                */
////	/*****************************************************************************************************************************
////	3) �������� ��� ����� �� ����
/////*****************************************************************************************************************************
//// * ����� ������ 32-��������� ���  � ���������ί ���'�Ҳ �� ���� (������ 1 - ���� ���������)
//// *****************************************************************************************************************************
////������ ���������� � ����� ������� ��������� ������ CRC
////������ ������� ���� ������ ����� � ����� ���������� ������ � ���� � ������, �� ����������� � ���������� �����
////*/
////		uint32_t vdMemoryRecordStatus=0;
////		vfFlashOpStatus= f_write_to_flash_record (
////								 &ArraySettings,
////							     (uint32_t*)FLASH_PAGE_63,
////							     sizeof ArraySettings,
////								 &vdMemoryRecordStatus
////								 );
////	/*4) ������� �� ������ ����� ��� �������� CRC				*/
////		for (uint8_t i=0;i<128;i++){
////			ArrayRx[i]=f_read_from_flash_word(FLASH_PAGE_63+4*i);;
////		}
////	/* 5) �������� �������� ����� 									*/
////		memset (ArrayRx, 0, sizeof(ArrayRx));
////	/* 6) ��������� � ���� � ����� � ����� ���� ����� � ���������� CRC*/
////		/*****************************************************************************************************************************
////		 * ������� ������ 32-��������� ��� � ���� �� ���������ί ����Ҳ (������ 1 - ���� ������ ���)
////		 * (������ �������� CRC)
////		*****************************************************************************************************************************/
////		vfFlashOpStatus= f_read_from_flash_rec (
////											 (uint32_t *)FLASH_PAGE_63,
////											  ArrayRx,
////										      sizeof (ArrayRx)
////										 );
////
//		f_VD_Meniat_USTAVKU (8, -999);
//
//}
//
//flash_status_t f_vkFlasOldInit(void){
//	flash_status_t vfFlashOpStatus = FLASH_UNKNOWN_Err;
//	uint32_t* pSettingD =NULL;
//	pSettingD=(uint32_t*)Settings_TK_fl;
//
//	/* ��������� ������ �� �������� �������� ���� � �� � Settings_TK_fl
// 	 ������� �� ������ ����� ��� �������� CRC							*/
//	for (uint8_t i=0;i<128;i++){
//		pSettingD[i]=f_read_from_flash_word(FLASH_PAGE_A_START_ADDR+4*i);
//	}
//	vfFlashOpStatus = f_VD_Sets_Write ();
//	return vfFlashOpStatus;
//}
//
//
