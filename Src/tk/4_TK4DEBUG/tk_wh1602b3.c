/*
 * lcd_wh1602b3.c
 *
 *  Created on: 7 ����. 2017 �.
 *      Author: KozakVF
 */
#include "tk_wh1602b3.h"
#ifdef WINSTAR_Display_WH1602B3
#include "string.h"
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "tkDebugThings.h"

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     P R I V A T E     D E F I N E S
 *******************************************************************************************************************
*/
extern I2C_HandleTypeDef hi2c1;
#define WH1602b3_ADDR  				0x3C    /* i2c ������ ������� wh1602b3								*/


#define WH1602B3_CMD				0x00     /* �������� ������ ���'�� WO1602G ��� ������ ������ 		*/
#define WH1602B3_DATA				0x40	 /* �������� ������ ���'�� WO1602G ��� ������ �����  		*/
#define WH1602B3_SCREEN_ADDRESS 	0x80     /* �������� ������ ������� ��������� ������ 				*/


#define INIT_8BIT_MODE 				0x30
#define INIT_2x16_5x8_MODE 			0x38
#define TURN_OFF_LCD 				0x08
#define CLEAN_LCD 					0x01

#define TURN_ON_CURSOR_BLINKS 		0x0F
#define TURN_ON_NO_CURSOR			0x0C

#define CURSOR_SHIFT_RIGHT			0x14
#define DISPLAY_SHIFT_LEFT			0x18

static uint8_t WH1602b3_chCyrillic(uint8_t ch_Cyrillic);
static wh1602b3_status_t WH1602b3_WriteCmd(uint8_t cmd);
static wh1602b3_status_t WH1602b3_WriteData(uint8_t data);
//
//	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, GPIO_PIN_SET);				/* 	AVARIA_ZACHYST	 */
//	  HAL_Delay(1000);
//	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2, GPIO_PIN_RESET);				/* 	AVARIA_ZACHYST	 */
//	  HAL_Delay(1000);
//	  return (0);
//}

wh1602b3_status_t WH1602b3_Init( void ){

//	 volatile uint8_t i2c_addr =0;
//	 i2c_addr=f_I2C_ScanBus();
	 volatile wh1602b3_status_t Err=0;
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (INIT_8BIT_MODE);
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (INIT_2x16_5x8_MODE);
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (TURN_OFF_LCD);
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (CLEAN_LCD);
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (TURN_ON_NO_CURSOR);
	 HAL_Delay (100);
	 Err = WH1602b3_WriteCmd (CURSOR_SHIFT_RIGHT);
//	 Err = WH1602b3_WriteCmd (DISPLAY_SHIFT_LEFT);

	 HAL_Delay (100);
	 return Err;
 }

 wh1602b3_status_t WH1602b3_WriteHighString(uint8_t *str){
// wh1602b3_status_t  W�1602b3_WriteHighString (uint8_t *str){

	 if (str==NULL) { return WH1602B3_NULL_Ptr;}
	 uint8_t sSize = strlen((char*)str);
	 if (sSize>17) {
		 sSize=17;
		 return (WH1602b3_WriteSyllable(0,0,sSize,str));
	 }
	 else {
		 uint8_t strTemp[17]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
		 snprintf((char*)strTemp, 17, "%s", str );
		 return (WH1602b3_WriteSyllable(0,0,17,strTemp));
	 }

 }

 wh1602b3_status_t WH1602b3_WriteLowString(uint8_t *str){

	 if (str==NULL) { return WH1602B3_NULL_Ptr;}
	 uint32_t sSize = strlen((char*)str);
	 if (sSize>17) {
		 sSize=17;
		 return (WH1602b3_WriteSyllable(0,1,sSize,str));
	 }
	 else {
		 char strTemp[17]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
		 snprintf(strTemp, 17, "%s",str );
		 return (WH1602b3_WriteSyllable(0,1,17,(uint8_t*)strTemp));
	 }
 }


 wh1602b3_status_t WH1602b3_WriteSyllable(	uint8_t x,       	/* ���������, � ����� ���������� ����, 0..15 	*/
		 	 	 	 	 	 	 	 	 	uint8_t y,  		/* �����, 0 - ������, 1 - �����				*/
											uint8_t size, 		/* �������� ������ ��� �����					*/
											uint8_t *syllable	/* �������� �� ����� ����, �� ����� ���������� */
											){
	 if (syllable == NULL){
		 return WH1602B3_NULL_Ptr;
	 }
	 if (x>15) {
		 return WH1602B3_WRONG_Arg;
	 }
	 if ((x+size)>16){
		 size=17-x;
	 }


	 WH1602b3_SetCursor(x, y);
	 for (uint8_t i=0;i<size;i++){
		 if ((syllable[i] != 0)
//				 ||	(syllable[i] != '\n')
				 ){
				 WH1602b3_WriteChar(syllable[i]);
		 }
	 }
	 return WH1602B3_OK;
 }
 /*******************************************************************************************************************
  *******************************************************************************************************************
  *     ���� ������ �������   WO1602G
  *******************************************************************************************************************
 ��������� ���� ������� ������ �� �������, ������ �������� WO1602_SetCursor, ��� �� �������, ������ �� ����������
  ������������� �������
 ����������� ���������������  ��� ������������ ��������� �� ���� �����, � ����
 ����� ������� �������� �����
 ���� ����� ����� � ����� ��������� ����� ���������, ������� ������ ���������� � ������� ������ ��� �������
 */
 wh1602b3_status_t WH1602b3_WriteChar(uint8_t ch) {
 	wh1602b3_status_t wh1602b3ch = WH1602B3_UNKNOWN_Err;
 	uint8_t char_wh1602b3 = WH1602b3_chCyrillic (ch);
 	wh1602b3ch=WH1602b3_WriteData(char_wh1602b3);
 	return (wh1602b3ch);
 }
 /*******************************************************************************************************************
  *******************************************************************************************************************
  *     ����̲����� ������� �� ������ ����ֲ�   WO1602G
  *******************************************************************************************************************
 ������ ����������� �� ������� � = 0 - ������ �����, �=0 - ����� ��� ���������
 ����� �� ���������
 ����������� ��������������� ������ � �������� WO1602_WriteChar ��� ������������ ��������� �� ���� �����, � ����
 ����� ������� �������� �����
 ���� ����� ����� � ����� ��������� ����� ���������, ������� ������ ���������� � ������� ������ ��� �������
 */
 wh1602b3_status_t WH1602b3_SetCursor(uint8_t x, uint8_t y) {
 	wh1602b3_status_t wh1602sc = WH1602B3_UNKNOWN_Err;
 	if (y > 1) {
 		y = 1;
 		wh1602sc = WH1602B3_WRONG_Arg;

 	}
 	else if (x > 0x0F) {
 		x = 0x0F;/* ����� �� 63 (0�3F, ��� � �� �� �� ����������� ������, � ����� ���� 16 ������� 			*/
 		wh1602sc = WH1602B3_WRONG_Arg;
 	}
 	else {
 		wh1602sc = WH1602B3_OK;
 	}
 	wh1602b3_status_t wh1602sc1 = WH1602b3_WriteCmd(WH1602B3_SCREEN_ADDRESS | (y * 0x40) | x);
 	if ( wh1602sc1 != WH1602B3_OK){
 		wh1602sc= wh1602sc1;
 	}
 	return (wh1602sc);
 }
 /*******************************************************************************************************************
  *******************************************************************************************************************
  *     �������� �����
  ******************************************************************************************************************/
 wh1602b3_status_t WH1602b3_ClearScreen (void) {
 	return (WH1602b3_WriteCmd(0x01));
 	HAL_Delay(1);
 }
 /*******************************************************************************************************************
  *******************************************************************************************************************
  *     ������������� ���������� �����˲�
  *******************************************************************************************************************
  * ������� ����� ���� windows ��� �������� ��������� ������  �������
  * ͳ�� � �� �������� ����� �������, � ����������� ��������,
  * ������ ������� ��������� ���  ����� ��, ���� �� ���� ����������� ��,
  * � ��������� ��������� ����� �������, ��� �� - �� ������� ��� ��������
 */

 static uint8_t WH1602b3_chCyrillic(uint8_t ch_Cyrillic){
 	uint8_t ch_wo1602g = 0x00;
   switch(ch_Cyrillic)
    {
     case '�': ch_wo1602g = 'A'; break;
     case '�': ch_wo1602g = '\240';break;
     case '�': ch_wo1602g = 'B';break;
     case '�': ch_wo1602g = '\241';break;
     case '�': ch_wo1602g = '\340';break;
     case '�': ch_wo1602g = 'E';break;
     case '�': ch_wo1602g = '\242';break;
     case '�': ch_wo1602g = '\243';break;
     case '�': ch_wo1602g = '\244';break;
     case '�': ch_wo1602g = '\245';break;
     case '�': ch_wo1602g = '\246';break;
     case '�': ch_wo1602g = 'K';break;
     case '�': ch_wo1602g = '\247';break;
     case '�': ch_wo1602g = 'M';break;
     case '�': ch_wo1602g = 'H';break;
     case '�': ch_wo1602g = 'O';break;
     case '�': ch_wo1602g = '\250';break;
     case '�': ch_wo1602g = 'P';break;
     case '�': ch_wo1602g = 'C';break;
     case '�': ch_wo1602g = 'T';break;
     case '�': ch_wo1602g = '\251';break;
     case '�': ch_wo1602g = '\252';break;
     case '�': ch_wo1602g = '\253';break;
     case '�': ch_wo1602g = 'X';break;
     case '�': ch_wo1602g = '\254';break;//
     case '�': ch_wo1602g = '\341';break;//
     case '�': ch_wo1602g = '\342';break;//
     case '�': ch_wo1602g = '\256';break;//
     case '�': ch_wo1602g = 'b';break;
     case '�': ch_wo1602g = 0xAD;break;
     case '�': ch_wo1602g = 0xAF;break;
     case '�': ch_wo1602g = '\260';break;
     case '�': ch_wo1602g = '\261';break;
     case '�': ch_wo1602g = 'a';break;
     case '�': ch_wo1602g = '\262';break;
     case '�': ch_wo1602g = '\263';break;
     case '�': ch_wo1602g = '\264';break;
     case '�': ch_wo1602g = '\343';break;
     case '�': ch_wo1602g = 'e';break;
     case '�': ch_wo1602g = '\265';break;
     case '�': ch_wo1602g = '\266';break;
     case '�': ch_wo1602g = '\267';break;
     case '�': ch_wo1602g = '\270';break;
     case '�': ch_wo1602g = '\271';break;
     case '�': ch_wo1602g = '\272';break;
     case '�': ch_wo1602g = '\273';break;
     case '�': ch_wo1602g = '\274';break;
     case '�': ch_wo1602g = '\275';break;
     case '�': ch_wo1602g = 'o';break;
     case '�': ch_wo1602g = '\276';break;
     case '�': ch_wo1602g = 'p';break;
     case '�': ch_wo1602g = 'c';break;
     case '�': ch_wo1602g = '\277';break;
     case '�': ch_wo1602g = 'y';break;
     case '�': ch_wo1602g = '\300';break;
     case '�': ch_wo1602g = '\344';break;
     case '�': ch_wo1602g = '\301';break;
     case '�': ch_wo1602g = '\346';break;
     case '�': ch_wo1602g = 'x';break;
     case '�': ch_wo1602g = '\345';break;
     case '�': ch_wo1602g = '\302';break;
     case '�': ch_wo1602g = '\303';break;
     case '�': ch_wo1602g = '\304';break;
     case '�': ch_wo1602g = '\305';break;
     case '�': ch_wo1602g = '\306';break;
     case '�': ch_wo1602g = '\307';break;
     case '�': ch_wo1602g = 'i';break;
     case '�': ch_wo1602g = 'I';break;
     case '�': ch_wo1602g = 0x9F;break;
     case '�': ch_wo1602g = 0x1F;break;
     case '�': ch_wo1602g = 'e';break;
     case '�': ch_wo1602g = 'E';break;
     case '\n':ch_wo1602g = ' ';break;
//     case ' ': ch_wo1602g = 0x02;break;
     default : ch_wo1602g = ch_Cyrillic;
    }
   return (ch_wo1602g);
 }


 static wh1602b3_status_t WH1602b3_WriteCmd(uint8_t cmd) {
 	wh1602b3_status_t wh1602b3wc = WH1602B3_UNKNOWN_Err;
 	uint8_t writing_attempts = 0;
 	HAL_StatusTypeDef i2cmw = HAL_ERROR;
 	while (wh1602b3wc == WH1602B3_UNKNOWN_Err){

 		while  ((writing_attempts<10) && (i2cmw!= HAL_OK)){
 			i2cmw=HAL_I2C_IsDeviceReady(&hi2c1, WH1602b3_ADDR << 1, 1, 10);
 			if (i2cmw==HAL_BUSY){
 				i2c1BUSY_Doctor ();
 			}
 		}
 		if (i2cmw != HAL_OK){
 			wh1602b3wc=WH1602B3_BUSY; break;
 		}

 		do {
 		 i2cmw = HAL_I2C_Mem_Write(	&hi2c1,					/* �������� �� ������� I2c										*/
 				 	 	 	 	 	WH1602b3_ADDR << 1,		/* ������ i2c �������, ������� �� 1 ����
 									 	 	 	 	 	 	 bit, �� ���������, ���������� HAL ��� ������� ������/������	*/
 									WH1602B3_CMD,			/* ������, ������ ��� ������ ������ 0�00						*/
 									I2C_MEMADD_SIZE_8BIT,	/* ������� ������� � ������ = 1									*/
 									&cmd,					/* ������� ��������, ������ ��� ����������, ���� ����			*/
 									1,						/* ������� ����� 											*/
   									100);					/* ������� 														*/
 		 ++writing_attempts;

 		}
 		while  ((writing_attempts<10) && (i2cmw!= HAL_OK));
 		if (i2cmw == HAL_OK) {
 			wh1602b3wc = WH1602B3_OK;}
 		else {
 			wh1602b3wc = WH1602B3_Err;
 		}
 	}
 	delayUs(24);
 	return (wh1602b3wc);
 }
 /*******************************************************************************************************************
  *******************************************************************************************************************
  *     � � � � �    � � � � �  �   WO1602G
  *******************************************************************************************************************

 */
 static wh1602b3_status_t WH1602b3_WriteData(uint8_t data) {
 	wh1602b3_status_t wh1602b3rd = WH1602B3_UNKNOWN_Err;
 	uint8_t wh_Data = data;
 	uint8_t writing_attempts = 0;
 	HAL_StatusTypeDef i2cmw = HAL_ERROR;
 	while (wh1602b3rd == WH1602B3_UNKNOWN_Err){

 		while  ((writing_attempts<10) && (i2cmw!= HAL_OK)){
 			i2cmw=HAL_I2C_IsDeviceReady(&hi2c1, WH1602b3_ADDR << 1, 1, 10);
 			if (i2cmw==HAL_BUSY){
 				i2c1BUSY_Doctor ();
 			}
 		}
 		if (i2cmw != HAL_OK){
 			wh1602b3rd=WH1602B3_BUSY; break;
 		}

 		do {
 		 i2cmw = HAL_I2C_Mem_Write(	&hi2c1,					/* �������� �� ������� I2c										*/
 				 	 	 	 	 	WH1602b3_ADDR << 1,		/* ������ i2c �������, ������� �� 1 ����
 									 	 	 	 	 	 	 bit, �� ���������, ���������� HAL ��� ������� ������/������	*/
									WH1602B3_DATA,			/* ������, ������ ��� ������ ������ 0�00						*/
 									I2C_MEMADD_SIZE_8BIT,	/* ������� ������� � ������ = 1									*/
 									&wh_Data,					/* ������� ��������, ������ ��� ����������, ���� ����			*/
 									1,						/* ������� ����� 											*/
   									100);					/* ������� 														*/
 		 ++writing_attempts;

 		}
 		while  ((writing_attempts<10) && (i2cmw!= HAL_OK));
 		if (i2cmw == HAL_OK) {
 			wh1602b3rd = WH1602B3_OK;}
 		else {
 			wh1602b3rd = WH1602B3_Err;
 		}
 	}
 	delayUs(24);
 	return (wh1602b3rd);
 }









 /**************************************************************************************************/
#endif //WINSTAR_Display_WH1602B3
