/*
 * tk_wo1602g.c
 *
 *  Created on: 14 ����. 2017 �.
 *      Author: Volodymyr.Kozak
 */


#ifdef WINSTAR_Display_WO1602G

#include "tk_wo1602g.h"

#include "stdint.h"
#include "i2c.h"
#include "tkDebugThings.h"
#include "tkErrLib.h"
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     P R I V A T E     D E F I N E S
 *******************************************************************************************************************
*/
#define WO1602G_CMD				0x00     /* �������� ������ ���'�� WO1602G ��� ������ ������ 		*/
#define WO1602G_DATA			0x40	 /* �������� ������ ���'�� WO1602G ��� ������ �����  		*/
#define WO1602G_SCREEN_ADDRESS 	0x80     /* �������� ������ ������� ��������� ������ 				*/
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     P R I V A T E     V A R I A B L E S
 *******************************************************************************************************************
*/
/*******************************************************************************************************************
 *******************************************************************************************************************
 *    S T A T I C    F U N C T I O N S  D E C L A R A T I O N S
 *******************************************************************************************************************
*/
static I2C_HandleTypeDef *wo1602g_i2c;  /* �������� �������� i2c, ��������� ��� �����������  	*/
static uint16_t wo1602g_addr = 0x3E; 	/* ������ slave I2C ������� wo1602g							*/

static wo1602g_status_t WO1602_WriteCmd(uint8_t cmd);
static wo1602g_status_t WO1602_WriteData(uint8_t data);
static wo1602g_status_t WO1602_WriteString16 (uint8_t *str);
static uint8_t chCyrillic(uint8_t ch_Cyrillic);
//static uint8_t f_I2C_ScanBus();
//static uint8_t chHEX (uint8_t i_hex);

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     P U B L I C   F U N C T I O N S
 *******************************************************************************************************************
*/

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     � � � � � � � � � � � � �    WO1602G
 *******************************************************************************************************************
����������� wo1602g - ����������� ���������� ������ � ���������� �� ������������
https://www.google.com.ua/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&ved=0ahUKEwjJybbOqMLXAhXDVxoKHaV5ByMQFggtMAE&url=http%3A%2F%2Fbiakom.com%2Fpdf%2Fwo1602g-yyh-ct_Winstar.pdf&usg=AOvVaw331bZtHQjKDcXgZc0cMr9E
����.12
����������� ���������� ����������� �������� ��� 3� �����. ��� 5� ����� - ���. �������� ����
������� ����������� �� ������ � ������� 5�8,
����� ����������� �� ��� �����,
��������� ������ ��������� ������ ���� ����� ��������� �����
*/

wo1602g_status_t WO1602_Init(I2C_HandleTypeDef *hi2c, uint8_t devAddr) {
	wo1602g_status_t wo1602gInit_status = WO1602G_UNKNOWN_Err;
	if (hi2c==NULL) {wo1602gInit_status = WO1602G_NULL_Ptr; return (wo1602gInit_status);}
	wo1602g_i2c = hi2c;			   /* �������� ��� ������������ �� ��������� �������� �� ������� I2C 	*/
	if (devAddr) {
		wo1602g_addr = devAddr;    /* i ������ i2c slave ������ wo1602g									*/
	}
	uint8_t idb_attempts =0;
	wo1602gInit_status=WO1602G_I2C_Err;
	/* ���������� ����������/��������� i2c slave ����		*/
	while ((idb_attempts<10)&&(wo1602gInit_status==WO1602G_I2C_Err)){
		/* ���������� ����������/��������� i2c slave ������ wo1602g		*/
		if (HAL_I2C_IsDeviceReady(wo1602g_i2c, wo1602g_addr << 1, 10, 100) == HAL_OK) {
			wo1602gInit_status=WO1602G_OK;
		}
		else {
			i2c1BUSY_Doctor ();
			if (HAL_I2C_IsDeviceReady(wo1602g_i2c, wo1602g_addr << 1, 10, 100) == HAL_OK) {
				wo1602gInit_status=WO1602G_OK;

			}
		}
		idb_attempts++;
	}
	if 	(wo1602gInit_status == WO1602G_I2C_Err){
		return (wo1602gInit_status);
	}

									/* � ��� ����������� i2c c������ �� ������ �� ������, ������� ������ ���� */

	wo1602gInit_status = WO1602_WriteCmd(0x38); /* Set: 	10 0000
									interface data length
															 1 0000 - 8-bit         0�38=������s 8���,
															 0 0000 - 4-bit
 	 	 	 	 	 	 	 	   numbers of display line     1000 - 2 ������		     ��� ������
															   0000 - 1 �������
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	100 - 5�11dots
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	000 - 5�8 dots)			 ����� 5�8
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 ��   */

	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x39);     /* ����� ��� - �� �������, ���� ��������� ��� ���� ������ �� ������ (����� ��������� �������)*/
	}
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x14);	   /* 		      			 1 0000                                               */
	}			                   /*          				   1000 -  display SHIFT    0x14=
															   0000 -  cursor moving    �������� � ��������
															    100 -  right			�� ������� ������
															    000 -  left */
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x74);     /* Set CGRAM address in address counter.*/
	}
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x54);		   /* Set CGRAM address in address counter.*/
	}
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x6F);		   /* Set CGRAM address in address counter.*/
	}
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x0C);		   /* 						1000                    0x0C =				*/
	}								   /*		    			 100	Set display (D), �������
																  10    cursor
																  00    NO cursor 			��� �������
																   1    blinking cursor
																   0    NO blinking cursor */
	if (wo1602gInit_status == WO1602G_OK){
		WO1602_WriteCmd(0x01);		   /* Write �00H� to DDRAM and set DDRAM address to �00H� from address counter*/
	}
	HAL_Delay(5);					   /* � ��� �������� ����������� ��������� ��� ���������� ����������� */
	return (wo1602gInit_status);	   /* wo1602 ������� �� ����������� ���������� � ���������� ���� �������   */
}

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     ���� ��������� �����    WO1602G
 *******************************************************************************************************************
��������� ���� ��������� ����� ������� 16 �������
���� �� ��� ������ ����� ������ ���� ���������� � ����� 16 �������
���� ������� ����� 16 - ���������� ���� 16, ������� �� ���� ������� ���������� �� 16 �������, �� �������
���� ������� ����� 16 - �������� �� �������, ��� ����� �� ����� ����������� ����� ���������� �������
����������� �������� ��������� ����������� ����� 16 �������.
*/

wo1602g_status_t  WO1602_WriteHighString (uint8_t *str){
	volatile wo1602g_status_t wo1602wsH16 =WO1602G_UNKNOWN_Err;
	if (str==NULL) { wo1602wsH16=WO1602G_NULL_Ptr; return (wo1602wsH16);}
	wo1602wsH16 = WO1602_SetCursor(0, 0);
	HAL_Delay (1);
	if (wo1602wsH16 == WO1602G_OK){
		wo1602wsH16 = WO1602_WriteString16 (str);
	}
	return (wo1602wsH16);
}
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     ���� �������� �����    WO1602G
 *******************************************************************************************************************
��������� ���� �������� ����� ������� 16 �������
���� �� ��� ������ ����� ������ ���� ���������� � ����� 16 �������
���� ������� ����� 16 - ���������� ���� 16, ������� �� ���� ������� ���������� �� 16 �������, �� �������
���� ������� ����� 16 - �������� �� �������, ��� ����� �� ����� ����������� ����� ���������� �������
����������� �������� ��������� ����������� ����� 16 �������.
*/
wo1602g_status_t  WO1602_WriteLowString (uint8_t *str){
	wo1602g_status_t wo1602wsL16 =WO1602G_UNKNOWN_Err;
	if (str==NULL) { wo1602wsL16=WO1602G_NULL_Ptr; return (wo1602wsL16);}

	wo1602wsL16 = WO1602_SetCursor(0, 1);
	HAL_Delay (1);
	if (wo1602wsL16 == WO1602G_OK){
		wo1602wsL16 = WO1602_WriteString16 (str);
	}
	return (wo1602wsL16);
}
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     �������� ������    WO1602G
 *******************************************************************************************************************
����� ���������. ������ (���������) - � ��������� ����� ���� ������
*/
wo1602g_status_t WO1602_ClearScreen (void) {
	return (WO1602_WriteCmd(0x01));
}


//void WO1602_WriteString(uint8_t *str) {
//	while (*str) {
//		HAL_I2C_Mem_Write(wo1602g_i2c, wo1602g_addr << 1, WO1602G_DATA, I2C_MEMADD_SIZE_8BIT, (str++), 1, 100);
//		HAL_Delay(1);
//	}
//}
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
wo1602g_status_t WO1602_SetCursor(uint8_t x, uint8_t y) {
	volatile wo1602g_status_t wo1602sc = WO1602G_UNKNOWN_Err;
	if (y > 1) {
		y = 1;
		wo1602sc = WO1602G_WRONG_Arg;

	}
	else if (x > 0x0F) {
		x = 0x0F;/* ����� �� 63 (0�3F, ��� � �� �� �� ����������� ������, � ����� ���� 16 ������� */
		wo1602sc = WO1602G_WRONG_Arg;
	}
	else {
		wo1602sc = WO1602G_OK;
	}
	wo1602g_status_t wo1602sc1 = WO1602_WriteCmd(WO1602G_SCREEN_ADDRESS | (y * 0x40) | x);
	if ( wo1602sc1 != WO1602G_OK){
		wo1602sc= wo1602sc1;
	}
	return (wo1602sc);
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
wo1602g_status_t WO1602_WriteChar(uint8_t ch) {
	wo1602g_status_t wo1602wch = WO1602G_UNKNOWN_Err;
	uint8_t char_wo1602g = chCyrillic (ch);
	wo1602wch=WO1602_WriteData(char_wo1602g);
	return (wo1602wch);
}
static uint8_t _displaycontrol=0x00;
// Turn the display on/off
void WO1602_noDisplay()
{
   _displaycontrol &= ~LCD_DISPLAYON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void WO1602_display()
{
   _displaycontrol |= LCD_DISPLAYON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void WO1602_noCursor()
{
   _displaycontrol &= ~LCD_CURSORON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}
void WO1602_cursor()
{
   _displaycontrol |= LCD_CURSORON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns on/off the blinking cursor
void WO1602_noBlink()
{
   _displaycontrol &= ~LCD_BLINKON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void WO1602_blink()
{
   _displaycontrol |= LCD_BLINKON;
   WO1602_WriteCmd(LCD_DISPLAYCONTROL | _displaycontrol);
}


/*******************************************************************************************************************
 *******************************************************************************************************************
 *     P R I V A T E     F U N C T I O N S
 *******************************************************************************************************************
*/
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     � � � � �    �    WO1602G   �������ֲ�
 *******************************************************************************************************************
������ �������, ��� ��������������� ��������� � ��� �������� ��������.
�� ���, �� �������� ������� HAL, �������� �� i2c ���������� ��� ����������.
���������� ��������, ����� ��������� ������ �� ��������, �����������, � ��� ����, ��������� �������������
��������� ������ �������, �� ������ ����� �������� ����������.
���� ��� ��, ���������� ��������� � ������� ����.
*/

static wo1602g_status_t WO1602_WriteCmd(uint8_t cmd) {
	wo1602g_status_t wo1602wc = WO1602G_UNKNOWN_Err;
	uint8_t writing_attempts = 0;
	volatile HAL_StatusTypeDef i2cmw = HAL_ERROR;
	while  ((writing_attempts<10) && (i2cmw!= HAL_OK)) {
		 i2cmw = HAL_I2C_Mem_Write(	wo1602g_i2c,			/* �������� �� ������� I2c										*/
									wo1602g_addr << 1,		/* ������ i2c �������, ������� �� 1 ����
									 	 	 	 	 	 	 ����, �� ���������, ���������� HAL ��� ������� ������/������	*/
									WO1602G_CMD,			/* ������, ������ ��� ������ ������ 0�00						*/
									I2C_MEMADD_SIZE_8BIT,	/* ������� ������� � ������ = 1									*/
									&cmd,					/* ������� ��������, ������ ��� ����������, ���� ����			*/
									1,						/* ������� ����� 											*/
  									100);					/* ������� 														*/
		 if (i2cmw!= HAL_OK){
			 i2c1BUSY_Doctor ();
			 i2cmw = HAL_I2C_Mem_Write(	wo1602g_i2c,			/* �������� �� ������� I2c										*/
										wo1602g_addr << 1,		/* ������ i2c �������, ������� �� 1 ����
										 	 	 	 	 	 	 ����, �� ���������, ���������� HAL ��� ������� ������/������	*/
										WO1602G_CMD,			/* ������, ������ ��� ������ ������ 0�00						*/
										I2C_MEMADD_SIZE_8BIT,	/* ������� ������� � ������ = 1									*/
										&cmd,					/* ������� ��������, ������ ��� ����������, ���� ����			*/
										1,						/* ������� ����� 											*/
	  									100);					/* ������� 														*/
			 if (i2cmw!= HAL_OK){
				 writing_attempts++;
			 }
		 }

	}
	if (i2cmw == HAL_OK) {
		wo1602wc = WO1602G_OK;
	}
	else {
			wo1602wc = WO1602G_Err;
	}
	delayUs(56);
	return (wo1602wc);
}
/*******************************************************************************************************************
 *******************************************************************************************************************
 *     � � � � �    � � � � �  �   WO1602G
 *******************************************************************************************************************

*/
wo1602g_status_t WO1602_WriteData(uint8_t data) {
	wo1602g_status_t wo1602wch = WO1602G_UNKNOWN_Err;

	HAL_StatusTypeDef i2cwc = HAL_ERROR;
	uint8_t writing_data_attempts=0;
	while  ((writing_data_attempts<10) && (i2cwc!= HAL_OK)){
		 i2cwc = HAL_I2C_Mem_Write(wo1602g_i2c, wo1602g_addr << 1, WO1602G_DATA, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
		 if (i2cwc != HAL_OK){
			 i2c1BUSY_Doctor ();
			 i2cwc = HAL_I2C_Mem_Write(	 wo1602g_i2c,
				 	 	 	 	 	 wo1602g_addr << 1,
									 WO1602G_DATA,
									 I2C_MEMADD_SIZE_8BIT,
									 &data,
									 1,
									 100);
		 }
		if (i2cwc != HAL_OK){
			writing_data_attempts++;
		}
		else  {
			wo1602wch = WO1602G_OK;
		}
	}
	delayUs(56);
	return (wo1602wch);
}




/*******************************************************************************************************************
 *******************************************************************************************************************
 *     � � � � �    �    WO1602G   ����� 16 ����, �������� �������
 *******************************************************************************************************************
����� ���������� �� ���������� ���� �������, ��������� � ��������� �����
*/
static wo1602g_status_t WO1602_WriteString16 (uint8_t *str){
	wo1602g_status_t wo1602ws16 =WO1602G_UNKNOWN_Err;
	if (str==NULL) { wo1602ws16=WO1602G_NULL_Ptr; return (wo1602ws16);}
	uint8_t iWHS=0;
	uint8_t strLength=0;
	strLength=strlen (str);
	if (strLength>17){
		strLength=17;
		wo1602ws16=WO1602G_WRONG_Arg;
	}
	else {
		wo1602ws16=WO1602G_OK;
	}

	wo1602g_status_t wo1602ws161 = WO1602G_UNKNOWN_Err;
	for (iWHS=0;iWHS<strLength;++iWHS){
		wo1602ws161 = WO1602_WriteChar(str[iWHS]);
		if (wo1602ws161 != WO1602G_OK) {
			wo1602ws16=wo1602ws161;
		}
//		HAL_I2C_Mem_Write(wo1602g_i2c, wo1602g_addr << 1, WO1602G_DATA, I2C_MEMADD_SIZE_8BIT, str[iWHS], 1, 100);
		delayUs(56);
	}
	if (strLength<17){
		for (iWHS=strLength;iWHS<17;++iWHS){
			wo1602ws161 = WO1602_WriteChar(' ');
			if (wo1602ws161 != WO1602G_OK) {
				wo1602ws16=wo1602ws161;
			}
			delayUs(56);
		}
	}
	return (wo1602ws16);
}
//https://aticleworld.com/how-to-create-and-display-custom-characters-on-16x2-alphanumeric-lcd/
//https://github.com/jenschr/Arduino-libraries/blob/master/LiquidCrystal/LCD.h

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     � � � � � � � � � �    � � � � � � � �   � � � � � � � � � � �
 *******************************************************************************************************************
// ����� ���������� �� ������ ���� �����������, ��������� �� ����� �������������
*/
wo1602g_status_t  WO1602_CreateCustomCharacter(
											   uint8_t *cMap, /* ����� � 8 ���� ��� ������� �����������
											   	   	   	   	   	   	   ���������, ��� ��������� '�' {
											    						***01110							0x0E
											    						***10001							0x11
											    						***10000							0x10
											    						***11100							0x1C
											    						***10000							0x10
											    						***10001							0x11
											    						***01110							0x0E
											    						***00000							0x00 }*/
											   uint8_t  cLocation     /*0..7 - ���������� ����� ������ � ������ ������� ����������� */
											   ) {
	wo1602g_status_t wo1602wch = WO1602G_UNKNOWN_Err;

	HAL_StatusTypeDef i2cwc = HAL_ERROR;
	//	   for (uint8_t i_ccc=0; i_ccc<8; i_ccc++) {
	//		   uint8_t volatile temp=*(cMap+(i_ccc*sizeof(uint8_t)));
	//		   if (
	//				   WO1602_WriteData(*(cMap+(i_ccc*sizeof(uint8_t))))!=WO1602G_OK
	//				   ){
	//			   break;
	//		   }
	//	   }
		   for (uint8_t i_ccc=0; i_ccc<8; i_ccc++) {

			   	uint8_t volatile temp_adr =  WO1602G_CGRAM_ADDR | (cLocation << 3);
			    uint8_t volatile temp_data=*(cMap+(i_ccc*sizeof(uint8_t)));
			    uint8_t writing_data_attempts=0;
				do {
		 	 	 	 i2cwc = HAL_I2C_Mem_Write(wo1602g_i2c, wo1602g_addr << 1, temp_adr, I2C_MEMADD_SIZE_8BIT, &temp_data, 1, 100);
		 	 	 	 ++writing_data_attempts;
    			}
				while  ((writing_data_attempts<10) && (i2cwc!= HAL_OK));
				if (i2cwc == HAL_OK){
					wo1602wch=WO1602G_OK;
				}
				else {
					wo1602wch=WO1602G_I2C_Err;
				}
				delayUs(56);
		   }
	return (wo1602wch);
//
//
//
//	   wo1602g_status_t wo1602ccc = WO1602G_UNKNOWN_Err;
//	   if (cMap==NULL) { wo1602ccc = WO1602G_NULL_Ptr; return (wo1602ccc);}
//
//
//	   cLocation &= 0x7;            // we only have 8 locations 0-7
//
//	   wo1602ccc = WO1602_WriteCmd(WO1602G_CGRAM_ADDR | (cLocation << 3));
//	   if (wo1602ccc != WO1602G_OK) {  return (wo1602ccc);}
//
//	   for (uint8_t i_ccc=0; i_ccc<8; i_ccc++) {
//		   uint8_t volatile temp=*(cMap+(i_ccc*sizeof(uint8_t)));
//		   if (
//				   WO1602_WriteData(*(cMap+(i_ccc*sizeof(uint8_t))))!=WO1602G_OK
//				   ){
//			   break;
//		   }
//	   }
//	   return (wo1602ccc);
}

/*******************************************************************************************************************
 *******************************************************************************************************************
 *     ������������� ���������� �����˲�
 *******************************************************************************************************************
//������� ����� ���� windows ��� �������� ��������� ������  �������
 * ͳ�� � �� �������� ����� �������, � ����������� ��������,
 * ������ ������� ��������� ���  ����� ��, ���� �� ���� ����������� ��,
 * � ��������� ��������� ����� �������, ��� �� - �� ������� ��� ��������
*/

static uint8_t chCyrillic(uint8_t ch_Cyrillic){
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

    default : ch_wo1602g = ch_Cyrillic;
   }
  return (ch_wo1602g);
}

// http://embedded.ifmo.ru/sdk/sdk61/components/lcd/WH1602B-YGK-CP.pdf
// http://terzo.acmesystems.it/download/datasheets/LCD-1.pdf

//static uint8_t f_I2C_ScanBus() {
//	uint8_t address_found=0;
//	uint8_t i = 0;
//	/* Scan only for 112 allowed addresses */
//	//for (i = 0x07; i < 0x78; i++) {
//		for (i = 0; i < 127; i++) {
//		if (HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 10, 100) == HAL_OK) {
//			address_found=i;
//			LCD_Printf("Ready: 0x%02x\n", i);
//		}
//		else {
//			 LCD_Printf("tested addr = 0x%X%X\n", i/0x10,i%0x10);
////			 HAL_Delay (50);
//		}
//		HAL_Delay (100);
//	}
//
//	if (address_found==0x00){
//			LCD_Printf("Slave's i2c-address haven't found :-(\n");
//
//	}
//	else {
//			LCD_Printf("Victory!\nSLAVE_ADDRESS IS 0x%X%X\n", address_found/0x10,address_found%0x10);
//
//	}
//	HAL_Delay(1000);
//	return (address_found);
//
//}
//
//
//static uint8_t chHEX (uint8_t i_hex){
//	uint8_t ch_wo1602g = 0x00;
//  switch(i_hex)
//   {
//    case 0x0: ch_wo1602g = '0'; break;
//    case 0x1: ch_wo1602g = '1';break;
//    case 0x2: ch_wo1602g = '2';break;
//    case 0x3: ch_wo1602g = '3';break;
//    case 0x4: ch_wo1602g = '4';break;
//    case 0x5: ch_wo1602g = '5';break;
//    case 0x6: ch_wo1602g = '6';break;
//    case 0x7: ch_wo1602g = '7';break;
//    case 0x8: ch_wo1602g = '8';break;
//    case 0x9: ch_wo1602g = '9';break;
//    case 0xA: ch_wo1602g = 'A';break;
//    case 0xB: ch_wo1602g = 'B';break;
//    case 0xC: ch_wo1602g = 'C';break;
//    case 0xD: ch_wo1602g = 'D';break;
//    case 0xE: ch_wo1602g = 'E';break;
//    case 0xF: ch_wo1602g = 'F';break;
//
//
//    default : ch_wo1602g = '0';
//   }
//  return (ch_wo1602g);
//}

#endif //WINSTAR_Display_WO1602G


/**I2C1 GPIO Configuration
PB6     ------> I2C1_SCL
PB7     ------> I2C1_SDA
*/

//HAL_I2C_ClearBusyFlagErrata_2_14_7(
//		&hi2c1,
//		GPIOB,
//		GPIO_PIN_6,
//		GPIO_PIN_7
//);
//HAL_I2C_MspInit(&hi2c1);
//MX_I2C1_Init();


