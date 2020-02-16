/*
 * tkComent.h
 *
 *  Created on: 14 ��. 2019 �.
 *      Author: KozakVF
 */

#ifndef TKPROJECT_ZKCOMENT_H_
#define TKPROJECT_ZKCOMENT_H_

/*������ ����� �������������� Tconv ����������� �� �������:
 *
 Tconv = Tsmt + 12,5 ������,
 ��� Tsmt � ��� ���������� �������� ����� �������.
 ��������, ������� ADCCLK ������ ������ 12MHz
 ���� ����� ������� � 1,5 �����.
 ����� Tconv = =1,5 + 12,5 = 14 ������ * 1/12MHz= 1.17us.
 ���� ��� ������ 239.5����, ���
 Tconv=239.5+12.5=252 �����  / 12MHz = 21us. ����� 14 ������ ���� ���� ������� �� 293u�
 Tconv=71.5+12.5=84 �����  / 12MHz = 7us. ����� 14 ������ ���� ���� ������� �� 98u�
 Tconv=55.5+12.5=68 �����  / 12MHz = 5.7us. ����� 14 ������ ���� ���� ������� �� 80u�
 Tconv=41.5+12.5=68 �����  / 12MHz = 5.7us. ����� 14 ������ ���� ���� ������� �� 80u�
// ���� �������� ������� ����� ��� ��� ��� - ����� 100us (200 ����� �� 20�� ��� 50��),
 �� ������������ ��������� ����� - 55.5

 ���� ������� ����� - ����� 200us, ����� 100 ����� ��� ������ ������/�������,
 �� ����� ������� �� 71.5.

*/
//
////���� ������ � ��� � ���� �� ���
//
//#include "TK12_5_Board.h"
//#include <Time.h>
//#include "Level2_StdPeriph/L2_ADC1andDMA.h"
//#include "L3_Telemetry.h"
//#include "L2_i2c_lcdSymb.h"
//#include "L2_RTCandCalendar.h"
//#include "L2_InsideFlashDriver.h"
//#include "L2_RTCandCalendar.h"
//#include "L4_algorithm.h"
//#include "L3_LCD_Menu.h"
//#include "main.h"
//
//char sel_12=0; //����� ����� ������� ��� ������ 12
//char sel_13=0; //����� ����� ������� ��� ������ 13
//char sel_14=0; //����� ����� ������� ��� ������ 14
//char up_down=0;  // =1 ����������� �� 1 �  =2 ��������� ������� ��� ���� �����  //WAS 04 04 2017
////����������� ��������� ������ up,longup, down, longdown
//char up_d=0;   //����������� ������ ����� (up)
//char longup_d=0;    //����������� ������ ������ ������� �����(longup)
//char down_d=0;      //����������� ������ ���� (down)
//char longdown_d=0;  //����������� ������ ������ ������� ����(longdown)
//unsigned int psw_setp=0; //�������� ������ ��������� ��� �������������� �������
//unsigned int psw_setp_us=0;  // �������� ���������� ������ ������������
//unsigned int psw_setp_wrk1=0;//�������� ���������� ������ ��� ��������� ������� 1-� ������
//unsigned int psw_setp_wrk2=0;//�������� ���������� ������ ��� ��������� ������� 2-� ������
////char iPswin=0;          //������� ������� ���6���� ������
//char cmd_set_wrk=0;
//unsigned int N_rd_Arx; //����� ������������ ��������� � ������
//uint32_t sizeof_fld;
//char scr10=0;
//char Block_Dwn_UC=0; //����������� ��������� UC ��� ��������������
///********************* ��������� ��������� ��� ������ *****************************************/
//
////��������� ��������� (��)
////2-������                // 09 01 2018
//char * const StrOffOn []={"oFF ","oN  "};
////char * const Str1GBK[] = {" ��=","  ��="};
//char * const Str1GBK1[] = {"0",">"};
////1-������                                     // 09 01 2018
//char * const StrMode []  = {"��� �","���� ","P��  ","ABT  ","�����","       ",
//                            "�����  ","�����  ",};
//char * const Str1GBK[] = {" ��=","��="};
////2-������
//char * const StrZR []  ={"����-��� ","����-��� ","����-��� "};
//char * const StrLevel []  ={"H��","CP ","B�C","99%","���"};
//
////��������� - ��������� ��������
//char * const Str1PKBK = "   ���������    ";
//char * const Str2PKBK = "   ��������     ";
//
//// = = = = = = = = = = =
////��������� - ��������� ��������  WAS 14 06 2015
//char * const Str1KOPKBK = "    ��������    ";  //�������� ���. ����������, �1
//char * const Str2KOPKBK = " ��� ���������� ";
//
////��������� - ��������� ��������  WAS //�������� �����, �2   21 06 2015
////1-������
//char * const Str1PrmIBK1 = {"I1"};
//char * const Str1PrmIBK2 = {" I2"};
////2-������
//char * const Str2PrmIBK1 = {"I3"};
//char * const Str2PrmIBK2 = {"A IE="};
////1 � 2-������
//char * const Str12PrmIBK = {"A"};
//
// //���������� �����,������ ��������� �2, ���    10 01 2016
//char * const Str1PrmIBK1d = {"ia="};
//char * const Str1PrmIBK2d = {"ic="};
////2-������
//char * const Str2PrmIBK1d = {"ib="};
//char * const Str2PrmIBK2d = {" I3="};
//
//  //���������� �����,������ ��������� �2, ���
//
//
////��������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmUBK[]  = {"U=","V"," Cos=", " ","*","XXXX"};  //�������� ����������, �3
//char * const Str2PrmUBK[]  = {"f=","P=","���",">"," "};
//
////��������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmRDBK[] = {"��="," ��=","�"};    //�������� ������. ��������, �4
//char * const Str2PrmRDBK[] = {"��="," ��=","�"};    // > -> � 10 01 2018
//
////��������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmIdBK[] = {"I����=","��   "};  //�������� ��������, �5
//char * const Str2PrmIdBK[] = {"R������= ","�","-------"};
//
////��������� - ��������� ��������  WAS 19 02 2016
//char * const Str1PrmIasmm[] = {"����� ���� ","%",">"}; //�������� ����������, �43
//char * const Str2PrmIasmm[] = {"����� ��=","�"};
//
//
////��������� - ��������� ��������  WAS 24 10 2015
//char * const Str1PrmADD[]= {"����  ","��=","���","  ","�� ","%  ","%"}; //�������� ADD, �6
//char * const Str2PrmADD[]= {"      ��=","�  "};
//
////��������� �������,��������� ������ �������� ADD- ��������� �������� ��� Cos�  WAS 07 01 2017
//char * const Str1PrmCos[]= {"i3="," i2="," UI"," ","-"}; //�������� ADD, �6
//char * const Str2PrmCos[]= {"i1="," ","-","  "};
//
////��� ������� �7 � �8 ���������� ����� ������� �� �������� �� (���� ������)
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK = "����� ����      " ;  //�������� ��� ��=1 ����������� ������, �7
//char * const Str2PrmaIBK[] = {"I1="," I2=","A","         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK[] = {"I1=","   ","�","�"}; //�������� ��� ��=1 ����������� ������ ������  �8
//char * const Str2PrmaIsBK[] = {"I2="," I3","A"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK1 = "����������      " ;  //�������� ��� ��=2 ����������� ������, �7
//char * const Str2PrmaIBK1[] = {"I1="," I2=","A","         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK1[] = {"I1=","   ","�","�"}; //�������� ��� ��=2 ����������� ������ ������  �8
//char * const Str2PrmaIsBK1[] = {"I2="," I3","A"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK2 = "����������      " ;  //�������� ��� ��=3 ����������� ������, �7
//char * const Str2PrmaIBK2[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK2[] = {"�����:    ","�","�"}; //�������� ��� ��=3 ����������� ������ ������  �8
//char * const Str2PrmaIsBK2[] = {"  ���=","���  "};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK3 = "����� ��������  ";  //�������� ��� ��=4 ����������� ������, �7
//char * const Str2PrmaIBK3[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK3[] = {"�����:    ","�","�"}; //�������� ��� ��=4 ����������� ������ ������  �8
//char * const Str2PrmaIsBK3[] = {"R��������=","�"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK4 = "��� ������      " ;  //�������� ��� ��=5 ����������� ������, �7
//char * const Str2PrmaIBK4[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK4[] = {"�����:    ","�","�"}; //�������� ��� ��=5 ����������� ������ ������  �8
//char * const Str2PrmaIsBK4[] = {"   I�= ","��  "};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK5 = "����� ���       " ;  //�������� ��� ��=6 ����������� ������, �7
//char * const Str2PrmaIBK5[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK5[] = {"�����:    ","�","�"}; //�������� ��� ��=6 ����������� ������ ������  �8
//char * const Str2PrmaIsBK5[] = {"  R���� ��=","�"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK6 = "����� ��        " ;  //�������� ��� ��=7 ����������� ������, �7
//char * const Str2PrmaIBK6[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK6[] = {"�����:    ","�","�"}; //�������� ��� ��=7 ����������� ������ ������  �8
//char * const Str2PrmaIsBK6[] = {"��="," ��=","�"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK7 = "����������OE Uc " ;  //�������� ��� ��=8 ����������� ������, �7
//char * const Str2PrmaIBK7[] = {"��� ����� ��=",">"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK7[] = {"�����:    ","�","�"}; //�������� ��� ��=8 ����������� ������ ������  �8
//char * const Str2PrmaIsBK7[] = {"   Uc=","B      "};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK71 = "������������    ";  //�������� ��� ��=81  ����������� ������, �7
//char * const Str2PrmaIBK71[] = {"������ Uc ��=",">"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK71[] = {"�����:    ","�","�"}; //�������� ��� ��=81 ����������� ������ ������  �8
//char * const Str2PrmaIsBK71[] = {"   Uc=","B      "};
//
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK72 = "������������ Uc " ;  //�������� ��� ��=82  ����������� ������, �7
//char * const Str2PrmaIBK72[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK72[] = {"�����:    ","�","�"}; //�������� ��� ��=82 ����������� ������ ������  �8
//char * const Str2PrmaIsBK72[] = {"   Uc=","B      "};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK8 = "������ ���������";  //�������� ��� ��=9 ����������� ������, �7
//char * const Str2PrmaIBK8[] = {"         ��="," >"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK8[] = {"�����:    ","�","�"}; //�������� ��� ��=9 ����������� ������ ������  �8
//char * const Str2PrmaIsBK8 = "����� �������.=";
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK9 = "���������       " ;  //�������� ��� ��=21 ����������� ������, �7
//char * const Str2PrmaIBK9[] = {"��� ����� ��=",">"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK9[] = {"I1=","   ","�","�"}; //�������� ��� ��=21 ����������� ������ ������  �8
//char * const Str2PrmaIsBK9[] = {"I2="," I3=","A"};
//
////��� ���������� - ��������� ��������  WAS 25 10 2015
//char * const Str1PrmaIBK0 = "��������� ���   " ;  //�������� ��� ��=22 ����������� ������, �7
//char * const Str2PrmaIBK0[] = {"������    ��=",">"};
//
////��� ���������� - ��������� ��������  WAS 21 06 2015
//char * const Str1PrmaIsBK0[] = {"I1=","   ","�","�"}; //�������� ��� ��=22 ����������� ������ ������  �8
//char * const Str2PrmaIsBK0[] = {"I2="," I3=","A"};
//
////��� ���������� - ��� ������ �� ��=10    WAS 28 02 2016
//char * const Str1_AL_10 = "��������� �� �� ";   //�������� ��� ��=10 ����������� ������ ������  �7
//char * const Str2_AL_10[] = {"         ��="," >"};
//
////��� ���������� - ��� ������ �� ��=10    WAS 28 02 2016
//char * const Str1_sAL_s10[] = {"�����:    ","�","�"}; //�������� ��� ��=10 ����������� ������ ������  �8
//char * const Str2_sAL_s10[]    = {"   ��=","�     "};
//
////��� ���������� - ��� ������ �� ��=11    WAS 28 02 2016
//char * const Str1_AL_11 = "��������� �� �� ";     //�������� ��� ��=11 ����������� ������ ������  �7
//char * const Str2_AL_11[] = {"         ��="," >"};
//
////��� ���������� - ��� ������ �� ��=11    WAS 28 02 2016
//char * const Str1_AL_s11[] = {"�����:    ","�","�"}; //�������� ��� ��=11 ����������� ������ ������  �8
//char * const Str2_AL_s11[]   = {"   ��=","�     "};
//
////��� ���������� - ��� ������ �� ��=12    WAS 28 02 2016
//char * const Str1_AL_12 = "�������� ����.  ";     //�������� ��� ��=12 ����������� ������ ������  �7
//char * const Str2_AL_12[] = {"��������. ��=",">"};
//
////��� ���������� - ��� ������ �� ��=12  WAS 28 02 2016
//char * const Str1_AL_s12[] = {"�����:    ","�","�"}; //�������� ��� ��=12 ����������� ������ ������  �8
//char * const Str2_AL_s12   = "                ";
//
////��� ���������� - ��� ������ �� ��=13     05 02 2016
//char * const Str1_AL_13 = "��. ����������  ";     //�������� ��� ��=13 ����������� ������ ������  �7
//char * const Str2_AL_13[] = {"����� N"," ��="," >"};
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_1_13 = "������� ���     "; //�������� ��� ��=13, �����.=1 �����.������ ������ �8
//char * const Str2_AL_s_1_13 = " ������� ��     ";
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_2_13 = "������� ��� �   "; //�������� ��� ��=13, �����.=2 �����.������ ������ �8
//char * const Str2_AL_s_2_13 = "�����. ����.����";
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_3_13 = "������� ��� �   "; //�������� ��� ��=13, �����.=3 �����.������ ������ �8
//char * const Str2_AL_s_3_13 = "�����.������� ��";
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_5_13 = "����� ������    "; //�������� ��� ��=13, �����.=5 �����.������ ������ �8
//char * const Str2_AL_s_5_13 = "����� ����������";
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_6_13 = "  ��� �������   "; //�������� ��� ��=13, �����.=6 �����.������ ������ �8
//char * const Str2_AL_s_6_13 = "  �����������   ";
//
////��� ���������� - ��� ������ �� ��=13   05 02 2016
//char * const Str1_AL_s_7_13 = "��� ���������   "; //�������� ��� ��=13, �����.=7 �����.������ ������ �8
//char * const Str2_AL_s_7_13 = "Uc,Ic ����� ����";
//
//// = = = = = = = = = = =
////��������� - ��������� �������� ���. ���������� WAS 15 06 2015, 02 16
//char * const Str1KDPKBK = "     �P���      ";  //�������� ���. ����������, �9
//char * const Str2KDPKBK = "�������/�������.";
//
////��������� - ��������� �������� ���. ���������� WAS 15 06 2015
////char * const Str1KDNBK[] = {"   ��","    "};  //�������� � �������, �10
//char * const Str1KDNBK[] = {"��","     "};  //�������� � �������, �10
//char * const Str2KDNBK[] =
// {"1 N��YX���������","2 N��YX���������","3 MFJGSWDEV     ",
//  "1 N��YX����     ","2          �����",
// };
//
//// = = = = = = = = = = =
////��������� - ��������� ��������  WAS 15 06 2015
//char * const Str1NOPKBK = "   ���������    ";  //�������� �������� ����������, �11
//char * const Str2NOPKBK = "  �����������   ";
//
//// = = = = = = = = = = = ��������� �������� ����������, �������,  ���.
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Ish = "��� �������� �� ";                                // �12
////2-������                          WAS 29 07 2015
//char * const Str2N_Ish[] = {"   ��=","���"};
//char * const Str2N_A[] = {"A   ","A  ","�A  ","A "}; //���.��� ���������� ����.
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Imax= "����� ���� �� ��";  // Imax,                    �13
////2-������                          WAS 29 07 2015
//char * const Str2N_Imax[]={"���� I�=","%I�  "};
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015 -21 02 2016
//char * const Str1N_VU_ADD= "������� ���/����";  // ������ ���,              �14
////2-������                          WAS 29 07 2015 -21 02 2016
//char * const Str2N_VU_ADD[]= {"��   ��=","%   "};
//
////��������� - �������� ���������  WAS 02 03 2016
////1-������
//char * const Str1N_NU_ADD= "������� ���/����";  // ������ ���,              �49
////2-������
//char * const Str2N_NU_ADD[]= {"��   ��=","%   "};
//
////��������� - �������� ���������  WAS 02 03 2016
////1-������
//char * const Str1N_SU_ADD= "������� ���/����";  // ������� ������� ���,    �6 ���
////2-������
//char * const Str2N_SU_ADD[]= {"��   ��=","%   "};
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Idif= "I������ �� �����";  //��� ������ I����,         �15
////2-������                          WAS 29 07 2015
//char * const Str2N_Idif[]= {" I����=","��  "};
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Rizol=" ����� ���� ��  ";  //����� ����. ��(R ��������), �16
////2-������                          WAS 29 07 2015
//char * const Str2N_Rizol="R����.=";
//char * const Str2N_d[]= {"�","�   ","���"};         //���.��� ���������� ����.
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_TzdVk="�������� ���  ��";  //� �������� ��������� ����., �17
////2-������                          WAS 29 07 2015
//char * const Str2N_TzdVk="   ����� ��=";
//char * const Str2N_c="�";                     //���.��� ���������� ����.
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_TzdOt="�������� ���� ��";  //� �������� ���������� ����., �18
////2-������                          WAS 29 07 2015
//char * const Str2N_TzdOt="   ����� ��=";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Rdu = " ����� �������� ";  //R�� ������ ������������ ����., �19
////2-������                          WAS 29 07 2015
//char * const Str2N_Rdu = "    R��=";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_TVsh= "� ���������� ���";  //� ������ ��� ����. �� �����.���., �20
////2-������                          WAS 29 07 2015
//char * const Str2N_TVsh= "���/�� ��=";
//char * const Str2N_m= "���";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Uotk= "������� ���� �� ";  // ������� ���� ����.,            �21
////2-������                          WAS 29 07 2015
//char * const Str2N_Uotk= "    ��>";
//char * const Str2N_sm= "��  ";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_Uvkl= " ������� ��� �� ";  // ������� ��� ����.,             �22
////2-������                          WAS 29 07 2015
//char * const Str2N_Uvkl= "    ��<";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_PNngr="�������� �������";  //���������� �������(��),         �23
////2-������                          WAS 29 07 2015
//char * const Str2N_PNngr="        ��=";
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_RJrab="   ��� ������   ";  //����� ������ ������,            �24
////2-������                          WAS 29 07 2015
//char * const Str2N_RJrab[] = {"     ������     ","    �������     "};
//
////��������� - �������� ���������  WAS 19 07 2015
////1-������                           WAS 29 07 2015
//char * const Str1N_TpDat=  "  ��� �������   ";  //��������� ��� ������� ������,   �25
////2-������                          WAS 29 07 2015
//char * const Str2N_TpDat[]= {"   �����������  ","   ���(��)      ","     ��(��)     ","     A��        "};
//
////��������� - �������� ���������  WAS 19 07 2015
//char * const Str1N_Kolim="�������  ���� ��";  //�������.����(���.���)����.,     �26
////2-������                           WAS 30 07 2015
//char * const Str2N_Kolim="��� ���������>";
//char * const Str2N_sp=" ";        // ������
//
////��������� - �������� ���������  WAS 20 07 2015
//char * const Str1N_OgrVk="����� ��� �� 10�"; //����������� ���.��������� ����., �27
////2-������                           WAS 30 07 2015
//char * const Str2N_OgrVk="��� ���������>";
//
////��������� - �������� ���������  WAS 20 07 2015
//char * const Str1N_SamZp="   ����������   ";  //���������� ����������� ����.,   �28
////2-������                           WAS 30 07 2015
//char * const Str2N_SamZp[] = {"    ����-���    ","    ����-���    "};
//
////��������� - �������� ���������  WAS 20 07 2015
//   //���������� ������� ���� ��� 1 2 3, �29, �30, �31
//char * const Str1N_KlbI_123= "���������� ���� "; //���������� ������� ���� ��� 1 2 3, �29
////2-������                           WAS 30 07 2015
//char * const Str2N_KlbI_123[]= {"I1","I2","I3","� k=","� k=","� c="};
//// = = = = = = = = = = = ��������� �������� ����������, �������,  ���.
//
//// = = = = = = = = = = = ��������� ������. ����������, �������,  ���.
//
////��������� - ������� ��������� (��������)   WAS 15 06 2015 - 22 02 2016
//char * const Str1NDPKBK = "   ���������    ";  //����a���� �����. ������. �����. �32
//char * const Str2NDPKBK = "   ���������    ";
//
////��������� - �����. ������. ����������  WAS 23 07 2015
// char * const Str1_tzvpn = "�������� ���  ��";  //� �������� ��� ����.,  �33
// char * const Str2_tzvpn[] = {"   ����� ��=","�"};
//
////��������� - �����. ������. ����������  WAS 15 06 2015
//char * const Str1_Rj_KLB = "���.����������  ";  //�����������������, �34
//char * const Str2_Rj_KLB[]= {" ��� ���������� ","   ����������   ","���������� ��="};
////  Str1_tzvpn
////Str1_Exp_W
//
// //��������� ������
//  char * const StrVvediteParolOld = " ��A��� ������: ";  //     �37
// char * const StrVvediteParolNov = " ����� ������:  ";
//
// //����� �������� �� �������
// char * const Str1_Serial_Number = " ����� �������� ";     //   �36
// char * const Str2_Serial_Number = "N 15��������    ";
//
//char * const Str1N_In_sh= "   ��� ��� ��   ";     // I� �����    �38
// char * const Str2N_In_sh[] = {"    I�=","A   ","A    "} ;
//
// char * const Str1N_P_sh= "����� ���� �� ��";     //����� ���. P �39
// char * const Str2N_P_sh[] = {"���� �min=","%��"} ;
//
// char * const Str1N_Cnt_mod= "����� ����������";        // ������ (����������) ����������  �40
// char * const Str2N_Cnt_mod[] = {"  ��� ������    ","  ������������� ","     ������     ",
//                                 " �������������� ","    ���������   "} ;   //    01 12 2015
//
// char * const Str1N_Typ_dm= "���������� ����.";        // ������ (����������) ����������  �41
// char * const Str2N_Typ_dm[] = {"���������� = ","  "} ;   //                                  27 01 2016
//
// char * const Str1N_K_Izol= "���������� R����";        // ����������� ��������    �42
// char * const Str2N_K_Izol[] = {"R�","� ","K="};       //                        09 02 2016
//
//  //���������� ������� ���� ���������� 23 02 2016
//char * const Str1N_Klb_Id= "���������� ���� "; //���������� ������� ���� ����������  �44
////2-������
//char * const Str2N_Klb_Id[]= {"I�","�� ","k"," "};
//
//  //���������� ����������� ���������� ���� 23 02 2016        �45
//char * const Str1N_Klb_Uc= "���������� Uc   ";
////2-������                           WAS 30 07 2015
//char * const Str2N_Klb_Uc[]= {"Uc=","B ","Ku="," "};
//
////R�� ������ ������������ ����.   23 02 2016
////1-������                           WAS 29 07 2015
//char * const Str1N_Rck = " ����� �������� ";  //R�� ������ ������������ ����., �46
////2-������                          WAS 29 07 2015
//char * const Str2N_Rck[] = {"    Rck=","�   "};
//
////��������� - �����. ������. ����������  WAS 07 01 2017
//char * const Str1_Debugg = "�������� �����. ";  //�����������������, �50
//char * const Str2_Debugg=  " N ���������= ";
//
////��������� - �����. ������. ����������  WAS 07 01 2017
//char * const Str1_cosf_krp = "    cosf_���    ";  //�����������������, �51
//char * const Str2_cosf_krp[]= {" ��� = ","      "};
//
////��������� - ���������� ��������  WAS 24 01 2017
//char * const Str1_react_pow[] = {"S= "," ��     "};               // �52
//char * const Str2_react_pow[]= {"Q="," ���    ","+"};
//
////��������� - ������ �������������  WAS 24 01 2017
//char * const Str1_loss_eff = "������ ���������";                   // �53
//char * const Str2_loss_eff[] = {"            ","%"};
//
////��������� - ������� ���/�� WAS 24 01 2017
//char * const Str1_EKM_DP[]= {"  ���/��  =","�"};                  // �54
//char * const Str2_EKM_DP[] = {"      ��=","�  "};
//
////��������� - ������� ��/��  WAS 24 01 2017
//char * const Str1_RD_PP[] = {"  ��/��  =","�"};                   // �55
//char * const Str2_RD_PP[] = {"      ��=","�  "};
//
////��������� - ������� ��/��  WAS 24 01 2017 �������� �� �� �� (GOT2)
//char * const Str1_DV_TR[] = {"��="," ��=","�"};                   // �56
//char * const Str2_DV_TR   = "                ";
//
////��������� - ���������  WAS 28 01 2017
//char * const Str1_PHASIN[] = {"  ��������� I","  "};               // �57
//char * const Str2_PHASIN[] = {" I","=","  ","����"," ���","              "};
//
////��������� - ���������� ������� ���� ��                            // �59  15 02 2017
////1-������
//char * const Str1N_klb_AD = "���������� ��   ";
////2-������
//char * const Str2N_klb_AD[] = {"��=","�� ","K="};
//
//
////��������� - �������� ���������   15 02 2017
////1-������
//char * const Str1N_ADD = " ����� �������� ";  //R�� ������ ������������ ����., �59
////2-������
//char * const Str2N_ADD[] = {"   ���=","��   "};
//
////��������� - �������� ���������   26 03 2017
////1-������
//char * const Str1N_Tzv_pn ="�������� �������";  //� �������� ��������� ����., �61
////2-������
//char * const Str2N_Tzv_pn[] = {"   Tzv_pn= ","�  "};
//
////��������� - �������� ���������   26 03 2017
////1-������
//char * const Str1N_Pmin_d ="Min ��������    ";  //P min ��������, �62
////2-������
//char * const Str2N_Pmin_d[] = {"   Pmin_d=","���"};
//
////��������� - �������� ���������   26 03 2017
////1-������
//char * const Str1N_Pmax_d ="Max ��������    ";  //P min ��������, �63
////2-������
//char * const Str2N_Pmax_d[] = {"   Pmax_d=","���"};
//
////��������� - �������� ���������   26 03 2017
////1-������
//char * const Str1N_J_jok ="  �a��� �����   ";  //������� ������  ������, �64
////2-������
//char * const Str2N_J_jok[] = {"����������� "," �� ","��� ","�����������    "};
//
////��������� - �������� ���������  23 05 2017
////1-������
//char * const Str1N_Ac_Psw ="������ �������� "; //������ � �����. ��� ������, �66
////2-������
//char * const Str2N_Ac_Psw[] = {"�����������  ","�1 ","HET","�����������="};
//
////2-������                          WAS 31 05 2017
//char * const Str2_Work_Set[] = {"���"," ��"}; //�67
//
////1-������
//char * const Str1N_Parol_Wrk ="������ �������� "; // �68
////2-������
//char * const Str2N_Parol_Wrk[] = {"��������� "," R1"," R2","   "};
//
//
//  //���������� ������� ���� ���������� 23 02 2016
// //1-������
//char * const Str1N_Id_sys_err[]= {"������.Idcr=","�"};  // �72
////2-������
//char * const Str2N_Id_sys_err[]= {"I�=","-"," ","��      "};
//
//   //������ �������� �������� �������,� ������� �� Flash, ���� ��� ��� ��������� 03 01 2018
//uint16_t  const Setting[84]=
//    {  135/*�����_min*/, 136/*�����_max*/, 137/*��_min*/,138/*��_max*/,
//        //    0,          1,                2,            3,
//        10/*���/��_minut_min*/,50/*���/��_minut_max*/,139/*���.���/10_minut_min*/,140/*���.���.._max*/,
//        // 4,                   5,                     6,                          7,
//        133/*K_Rizol_min,���*/,134/*K_Rizol_max,���*/, 141/*R��_min*/,142/*R��_max*/,
//        // 8,                   9,                      10,           11,
//            //Prg_cxk-�����_������������ ����� ���������
//         143/*Prg_cxk_min*/,144/*Prg_cxk_max*/,145/*I���_min,��*/,146/*I���_max,��*/,
//        //12,                13,                14,               15,
//      /*���.�������� ��,���*/ 10/*(0,1) min*/, 11/*max*/,
//        //                    16,              17,
//         //�� 32 ��������� ��������� �������:
//      /*��� ��*/ 4/*min*/, 412/*max*/,
//        //      18,       19,
//      /*���������� ��*/ 0/*min*/, 8/*max*/, /*���������� ��*/ 1/*min*/, 5/*max*/,
//        //             20,       21,                         22,       23,
//      /*���. �������*/ 1/*min*/, 2/*max*/, /*�(����. �����*/ 0/*min*/, 1/*max*/,
//        //            24,       25,                         26,       27
//       /*����� ��(��� RS485)*/ 0/*min*/,255/*max*/,
//         //                   28,        29,
//       /*���. �� ��� ������.������*/ 1/*min*/, 2/*max*/,
//         //                         30,       31,
//           //������ ���������� 2-� ��������:
//       /*���. ������� ����. �������     */ 1/*min*/, 3/*max*/,
//        //                                32,       33,
//       /*���. ����������� �������� �� ��*/ 1/*min*/, 3/*max*/,
//        //                                34,       35,
//              //������ ��������� ������� Flash, ��� ��������� ����������� ����� � ����������� ��
//              // ����  � � ������� �� Y=0..8
//           63/*��� ���� ��412 ��� ��4 */,
//        // 36,
//             //��� � ��. Y=0..8
//               //������ max �������� ������� ��� ���� I�  �� Flash
//          100/*Y=0*/, 101/*Y=1*/, 102/*Y=2*/, 103/*Y=3*/, 104/*Y=4*/,
//        // 37,        38,          39,         40,         41,
//          105/*Y=5*/, 106/*Y=6*/, 107/*Y=7*/, 108/*Y=8*/,
//        // 42,        43,          44,         45,
//              //������ ��������� ������� Flash, ��� ��������� ����������� ����� � ����������� ��
//              // � �������� ���� �� �=1..5
//           70/*X=1*/, 75/*X=2*/, 80/*X=3*/, 85/*X=4*/, 90/*X=5*/,
//        // 46,        47,        48,        49,        50
//             //��� � ��. Y=0..8
//               //������ min �������� ������� ��� ���� I�  �� Flash
//          119/*Y=0*/, 120/*Y=1*/, 121/*Y=2*/, 122/*Y=3*/, 123/*Y=4*/,
//        // 51,        52,         53,         54,         55,
//          124/*Y=5*/, 125/*Y=6*/, 126/*Y=7*/, 127/*Y=8*/,
//        // 56,        57,          58,         59,
//        //WAS 08 04 207
//         129/*AD_��_min,��*/,130/*AD_��_max,��*/, 131/*AD_��_min,��*/,132/*AD_��_max,��*/,
//        //60,                 61,                 62,                  63,
//         147/*�����_min*/,148/*�����_max*/,149/*Tzv_pn_min*/,150/*Tzv_pn_max*/, //Tzv_pn-�������.�����. ��������.�����
//        // 64,            65,               66,              67,
//         151/*Pmin_d_min*/,152/*Pmin_d_max*/,153/*Pmax_d_min*/,154/*Pmax_d_max*/,
//        // 68,            69,                70,               71,
//        155/*TVn_Sx_min*/,156/*TVn_Sx_max*/,157/*RizolED_min,���*/,158/*RizolED_max,���*/,
//        // 72,            73,                74,                  75,
//         147/*         */,148/*         */,147/*         */,148/*         */,
//        // 76,         77,                  78,         79,
//        /*�_��*/ 0/*min*/, 3/*max*/, 0 /*���.*/, 0 /*���.*/,
//             // 80,       81,       82,         83,
//    };
//
// /*************** ��������� ����������  ****************************************/
//  static uint8_t EditValueProp=0; //���������� �������� �������� �������������� ��������
//  // ��� 0 - ������� �� ���������/���� �������
//  // ��� 1 - ����� - ������ �������
//  // ��� 2 - ������� �� ���������/���� ������� �������
//  // ��� 3 - ����� - ������ ������� �������
//
// // static int8_t SubMenu=0;              // ����� �������� �������
//  static Type_Def_Screen EditedScrren; //����� � ������� ������������� �������
//  static uint32_t EditValue;    //���������� �������� ������������� ��������
//  static uint32_t EditValue_1;  // �� �� ��� ���������� �������� EditValue
//  static uint8_t DecData [12];         //������ � ������� �������� ������������� �������� ����������
//  static uint8_t DecSine;              //������� �������������� ����� ��� ������� DecData
//  static uint8_t DecBitNum;    //����� ���������� ������������� �������� (�� 1 �� 10 ������������)
//  static uint8_t DecBit;       //����� �������������� ������� (������� �� ����)
//  static uint8_t CursorPos;    //������� ������� ������� �������������� ����� �� ������ ���
//  static uint8_t CursorPosRTC;  //������� �������������� ������� ����� �� ������ ���
//  static uint8_t UserPswEnable=0; //������ ��������� �������� ������������ �����
//  static uint8_t UsPswEnable=0; //������ �������� ������������ �����
//  static uint32_t Sett_min;    //���������� �������� �������� min ������ �������
//  static uint32_t Sett_max;    //���������� �������� �������� max ������ �������
// // ��� 0 - ������ ������������ ����� - ���������� �� ���������������� ������
//  // ��� 1 - ������ ������������ ����� - ���������� �� ��������� ������� ������
//
///***********************************************************************************/
//
//
//#include "screen_aux.c"    // ������
// #include "inoutedit_aux.c" // ����-����� ��������, ��������������
//
///**************************************************************************************************/
//// ��������������� ������� ��� ������� ������� ���������� ����
//void LoadEditValue (Type_Def_Screen CurScrren)
//{  EditValue_1=0;   //������� ��� ������ �� ������������� ����� ������� 10 02 2018
//  switch (CurScrren)
//  {  //��������� ����������� ���.
//    case settings_add_scn:    EditValue=Settings_TK_fl[175];break;   // �6   10 01 2018
//    case settings_Pn_shk_scn: EditValue=Settings_TK_fl[1];break;   //nic  �12
//    case aux_In_shk_scn:      EditValue=Settings_TK_fl[0];break;   //�38
//    case settings_imax_scn:   EditValue=Settings_TK_fl[55];break;   //nic  �13
//    case aux_Imax_shk_scn:    EditValue=Settings_TK_fl[56];break;   // �13
//    case settings_pmin_scn:   EditValue=Settings_TK_fl[57];break;   //nic  �14
//    case settings_Levl_off_scn: EditValue=Settings_TK_fl[58];break; //�14 21 02 2016
//    case aux_sett_Cnt_mod_scn: EditValue=Settings_TK_fl[23];break;  //WD    �40 01 12 2015
//    case settings_RJ_P_O_scn: EditValue=Settings_TK_fl[5];break;   //WD  01 12 2015  �24
//    case settings_Psw_usr_scn:  EditValue=Settings_TK_fl[64];break;   //  �65
//    //case Acces_set_No_Psw:      EditValue=Settings_TK_fl[68];break;   //  �64
//    case Restr_Work:            EditValue=Settings_TK_fl[95];break;   // 01 16 2017  �67
//
//
//      //��������� ����� 06 09 2015 ���.
//
//    //��������� ��������� ���.
//    case aux_param_num_scn:         EditValue=Settings_TK_fl[63];break;  //WD   �10
//    case aux_param_num_scn1:        EditValue=Settings_TK_fl[97];break;  //���1 �10
//    case aux_param_num_scn2:        EditValue=Settings_TK_fl[98];break;  //���2 �10
//    case aux_settings_tzvpn_scn:  EditValue=Settings_TK_fl[33];break;  //WD   �33
//    case settings_ton_scn:          EditValue=Settings_TK_fl[32];break;  //WD   �17
//    case settings_Tzv_pn_scn:      EditValue=Settings_TK_fl[Tzv_pn];break;  //WD   �61
//    case settings_Pmin_d_scn:       EditValue=Settings_TK_fl[Pmin_d];break;  //WD   �62
//    case settings_Pmax_d_scn:       EditValue=Settings_TK_fl[Pmax_d];break;  //WD   �63
//    case settings_J_jok_scn:    EditValue=Settings_TK_fl[J_jok_set];break;   //  �64
//    case  settings_PN_scn:      EditValue=Settings_TK_fl[4];break;   //WD   �23
//    case settings_TVn_Sx_scn:   EditValue=Settings_TK_fl[18];break; //WD   �20
//    case settings_Kol_BKL_scn:   EditValue=Settings_TK_fl[kol_VKL];break;  //WD   �27
//    case settings_Kol_Imp_scn:   EditValue=Settings_TK_fl[36];break;  //WD   �26
//    case settings_risol_scn:  EditValue=Settings_TK_fl[21];break;  //nic  �16
//    case settings_idif_scn:   EditValue=Settings_TK_fl[11];break;   //nic  �15
//    case settings_Rdu_scn:    EditValue=Settings_TK_fl[22];break;  //WD   �19
//    case settings_toff_scn:   EditValue=Settings_TK_fl[60];break;  //WD   �18
//    //case aux_serial_Numb:     EditValue=Settings_TK_fl[8];break;   //WD    �41
//    case settings_Rck_scn:    EditValue=Settings_TK_fl[59];break;  //WD    �46
//    case settings_KLB_I1_scn:    EditValue=kia;break;   //WD   �29 //Settings_TK_fl[13]
//    case settings_KLB_I2_scn:    EditValue=kib;break;   //WD   �30  Settings_TK_fl[15]
//    case settings_KLB_I3_scn:    EditValue=kic;break;   //WD   �31   Settings_TK_fl[17]
//    case settings_KLB_Id_scn:    EditValue=Settings_TK_fl[96]; break;   //WD   �44
//    case settings_KLB_Uc_scn:    EditValue=Settings_TK_fl[42]; break;   //WD   �45
//    case aux_settings_Debug_scn: EditValue=Settings_TK_fl[N_Brows];break; //WD  �50 07 01 2016
//    case aux_setts_cosf_kpr_scn: EditValue=Settings_TK_fl[62];break; //WD  �51 07 01 2016
//    case settings_phas_scn:      EditValue=Settings_TK_fl[113];break; //  �51 05 02 2017
//    case settings_KLB_AD_scn:    EditValue=Settings_TK_fl[k_klb_AD];break; //  �59 15 02 2017
//    case aux_sett_K_Izol:        EditValue=Settings_TK_fl[46];break;       // �42 09 02 2016
//    case Id_syst_error:          EditValue=Settings_TK_fl[Idcr];break;     // �72 14 03 2018
//    //��������� ��������� ���.
//
//    case settings_Levl_on_scn:  EditValue=Settings_TK_fl[34];break;   //WD   �22
//
//      //��������� ����� 06 09 2015 ���.                            //WD
//    case settings_Tup_Dt_scn:    EditValue=Settings_TK_fl[35];break;  //WD   �25
//    case settings_BKL_P_3_scn:    EditValue=Settings_TK_fl[39];  //nic ����������� ������������� �������  �28
//                                if (EditValue<1)EditValue=0;    //WD �������� EditValue<0 -> EditValue<1 (� �� ����� ������)
//                                 else if (EditValue>1)EditValue=1;
//                                 break;
//
//      //��������� ����� 06 09 2015 ���.
//    case aux_setRTC_scn:          EditRTCFuncPrepar (0x80+3);break;   //    �35
//    case Acces_set_No_Psw:                                            //    �66  24 12 2017
//    case userpsw_scn:           EditValue=0; break;                   //    �37
//  }
//}
//
////����������� ���������� ��� ��������� � ��������������;
//void EditValueToScreen (Type_Def_Screen CurScrren)
//{ switch (CurScrren)
//  { //��������� ��������� ���.
//    case settings_Pn_shk_scn:     EditFuncPrepar (EditValue,5,0xC0+6);  break;  //WD   �12
//    case aux_In_shk_scn:    EditFuncPrepar (EditValue,5,0xC0+7);  break;  //�38
//    case settings_imax_scn:   EditFuncPrepar (EditValue,3,0xC0+8);  break; //WD   �13
//    case aux_Imax_shk_scn:    EditFuncPrepar (EditValue,3,0xC0+10); break;  //�39
//    case settings_BKL_P_3_scn:   break;                             //   �28 // nic ������������� ��������
//    case settings_pmin_scn:     EditFuncPrepar (EditValue,4,0xC0+8);  break;  //WD   �14  21 02 2016
//    case settings_add_scn:      EditFuncPrepar (EditValue,4,0xC0+8);  break;  //WD   �6  10 01 2018
//    case settings_Psw_usr_scn:  EditFuncPrepar (EditValue,2,0xC0+7);  break;  //WD   �65
//
//    case settings_Levl_off_scn: EditFuncPrepar (EditValue,4,0xC0+8); break; //WD   �49
//    case aux_sett_Cnt_mod_scn:  break;                                      // �40
//    case settings_RJ_P_O_scn:   break;                            //WD   �24 nic ������������� �������� 22 10 2015
//    case settings_Tup_Dt_scn:   break;                             //WD   �25 nic �������������
//    //case Acces_set_No_Psw:      break;                            //WD   �66 nic �������������
//    case Restr_Work:            break;                             //WD   �67
//    //��������� ��������� ���.
//
//    //��������� ��������� ���
//    case aux_param_num_scn:        EditFuncPrepar (EditValue,9,0x80+2);  break;  // �10
//    case aux_param_num_scn1:       EditFuncPrepar (EditValue,5,0x80+11);  break; //��� �10
//    case aux_param_num_scn2:       EditFuncPrepar (EditValue,9,0x80+2);  break; //��� �10
//    case aux_settings_tzvpn_scn: EditFuncPrepar (EditValue,3,0xC0+12);  break;  // �33
//    case settings_ton_scn:         EditFuncPrepar (EditValue,3,0xC0+12); break;  //WD   �17
//    case settings_Tzv_pn_scn:       EditFuncPrepar (EditValue,2,0xC0+11);break;  //WD   �61
//    case settings_Pmin_d_scn:      EditFuncPrepar (EditValue,3,0xC0+10);break;  //WD   �62
//    case settings_Pmax_d_scn:      EditFuncPrepar (EditValue,3,0xC0+10);break;  //WD   �63
//    case settings_J_jok_scn:       EditFuncPrepar (EditValue,1,0xC0+15);break;  //WD   �64
//    case  settings_PN_scn:    EditFuncPrepar (EditValue,4,0xC0+11); break;   //WD   �23
//    case settings_TVn_Sx_scn:   EditFuncPrepar (EditValue,3,0xC0+10); break; //WD   �20
//    case settings_Kol_BKL_scn:   EditFuncPrepar (EditValue,2,0xC0+14); break;  //WD   �27
//    case settings_Kol_Imp_scn:   EditFuncPrepar (EditValue,1,0xC0+14); break;  //WD   �26
//    case settings_risol_scn:  EditFuncPrepar (EditValue,6,0xC0+7); break;     //WD   �16
//    case settings_idif_scn:   EditFuncPrepar (EditValue,5,0xC0+7);  break;  //WD   �15
//    case settings_Rdu_scn:    EditFuncPrepar (EditValue,4,0xC0+8);  break;  //WD   �19
//    case settings_KLB_AD_scn: EditFuncPrepar (EditValue,4,0xC0+12);  break;  //WD   �59  //15 02 2017
//    case settings_toff_scn:   EditFuncPrepar (EditValue,3,0xC0+12); break;  //WD   �18
//     case settings_Rck_scn:    EditFuncPrepar (EditValue,4,0xC0+8); break;  //WD   �46
//    case settings_KLB_I1_scn:    EditFuncPrepar (EditValue,5,0xC0+11); break;   //WD   �29
//    case settings_KLB_I2_scn:    EditFuncPrepar (EditValue,5,0xC0+11); break;   //WD   �30
//    case settings_KLB_I3_scn:    EditFuncPrepar (EditValue,5,0xC0+11); break;   //WD   �31
//    case settings_KLB_Id_scn:    EditFuncPrepar (EditValue,5,0xC0+11); break;   //WD   �44
//    case settings_KLB_Uc_scn:    EditFuncPrepar (EditValue,5,0xC0+11); break;   //WD   �45
//    case aux_sett_K_Izol:        EditFuncPrepar (EditValue,4,0xC0+12); break;   //WD   �42
//    case aux_setts_cosf_kpr_scn: EditFuncPrepar (EditValue,3,0xC0+8); break;   //WD   �50
//    case settings_phas_scn:       EditFuncPrepar (EditValue,1,0x80+12); break; //WD   �51
//    case aux_settings_Debug_scn:  EditFuncPrepar (EditValue,2,0xC0+14); break; //WD   �49
//    case Id_syst_error:           EditFuncPrepar (EditValue,3,0x80+12); break; //WD   �72
//
//    //��������� ��������� ���.
//
//    case userpsw_scn:
//              //��������� ��� ���� ������  //  WD psw 09 05 2017
//      if((psw_setp==Settings_TK_fl [65]) || (psw_setp==Settings_TK_fl [66]))
//         EditFuncPrepar (EditValue,4,0xC0+12);   // 14 12 2017
//      break;
//    case Acces_set_No_Psw: EditFuncPrepar (EditValue,4,0xC0+12); break; //  24 12 2017
//  }
//}
//
//
//void LoadParamFromEditValue (Type_Def_Screen CurScrren)
//{
//  switch (CurScrren)
//   {    //� ������� ����������� �������� �������,��������, ���� ��� ��� ������� ���� ��������� 220,��:
//        // case settings_iw_scn: if(EditValue<120)EditValue=120;else if (EditValue>220)EditValue=220;
//        //                       WriteFlash32_CRC (&Settings_TK_fl[0],EditValue);break;
//
//     //��������� ��������� ���
//    case settings_Pn_shk_scn:     //Limit_Setting(16); //��� ����� ������� ���.�������� ��,���
//                                  WriteFlash32_CRC (&Settings_TK_fl[1],EditValue);break;  // �12
//    case aux_In_shk_scn:          WriteFlash32_CRC (&Settings_TK_fl[0],EditValue); break;  //38
//
//    case settings_imax_scn:       WriteFlash32_CRC (&Settings_TK_fl[55],EditValue);break;  // �13
//    case aux_Imax_shk_scn:        WriteFlash32_CRC (&Settings_TK_fl[56],EditValue);break;   //�13
//    case settings_BKL_P_3_scn:    WriteFlash32_CRC (&Settings_TK_fl[39],EditValue);break;  // �28
//    case settings_pmin_scn:       WriteFlash32_CRC (&Settings_TK_fl[57],EditValue);break;    //�14 21 02 2016
//    case settings_add_scn:        WriteFlash32_CRC (&Settings_TK_fl[175],EditValue);break;   //�6 10 01 2018
//    case settings_Psw_usr_scn:    WriteFlash32_CRC (&Settings_TK_fl[64],EditValue);break; //�65
//    //case Acces_set_No_Psw:        WriteFlash32_CRC (&Settings_TK_fl[68],EditValue);break; //�66
//
//    case settings_Levl_off_scn:   WriteFlash32_CRC (&Settings_TK_fl[58],EditValue);break; //�14 21 02 2016
//    case aux_sett_Cnt_mod_scn:    WriteFlash32_CRC (&Settings_TK_fl[23],EditValue);break; //WD   �40
//    case settings_RJ_P_O_scn:     WriteFlash32_CRC (&Settings_TK_fl[5],EditValue);break; //WD   �24
//    case settings_Tup_Dt_scn:     WriteFlash32_CRC (&Settings_TK_fl[35],EditValue);break; //WD   �25
//    case aux_setRTC_scn:          LoadToRTCNewTime();break;                             //    �35
//    //case Restr_Work:          WriteFlash32_CRC (&Settings_TK_fl[95],EditValue);break; //WD   �67
//    case Restr_Work:          cmd_set_wrk=1;break; //WD   �67
//
//    //��������� ��������� ���
//
//      //��������� ��������� ���
//    case aux_param_num_scn:         Limit_Set_clp(1);
//                                    WriteFlash32_CRC (&Settings_TK_fl[63],EditValue);break;  //WD   �10
//    case aux_param_num_scn1:        Limit_Set_clp(2);
//                                    WriteFlash32_CRC (&Settings_TK_fl[97],EditValue);break;  //WD   �10
//    case aux_param_num_scn2:        Limit_Set_clp(3);
//                                    WriteFlash32_CRC (&Settings_TK_fl[98],EditValue);break;  //WD   �10
//    case aux_settings_tzvpn_scn:    WriteFlash32_CRC (&Settings_TK_fl[33],EditValue);break;  //WD   �33
//    case settings_ton_scn:          //Limit_Setting(0); //����������� ����� ������� �����
//                                    WriteFlash32_CRC (&Settings_TK_fl[32],EditValue);break;  //WD   �17
//    case settings_Tzv_pn_scn:      // Limit_Setting(0); //����������� ����� ������� �����
//                                    WriteFlash32_CRC (&Settings_TK_fl[Tzv_pn],EditValue);break; //WD   �61
//    case settings_Pmin_d_scn:      //Limit_Setting(0); //����������� ����� ������� �����
//                                    WriteFlash32_CRC (&Settings_TK_fl[Pmin_d],EditValue);break;  //WD   �62
//    case settings_Pmax_d_scn:      //Limit_Setting(0); //����������� ����� ������� �����
//                                    WriteFlash32_CRC (&Settings_TK_fl[Pmax_d],EditValue);break;  //WD   �63
//    case settings_J_jok_scn:       WriteFlash32_CRC (&Settings_TK_fl[J_jok_set],EditValue);break; //WD   �64
//    case settings_PN_scn:          // Limit_Setting(2);  //����������� ����� ������� ��
//                                    WriteFlash32_CRC (&Settings_TK_fl[4],EditValue);break;   //WD   �23
//    case settings_TVn_Sx_scn:      // Limit_Setting(4);  //����������� ����� ������� ���/��,���
//                                    WriteFlash32_CRC (&Settings_TK_fl[18],EditValue);break;  //WD   �20
//    case settings_Kol_BKL_scn:     // Limit_Setting(6); //����������� ����� ������� ���.��� �� 10 ���
//                                    WriteFlash32_CRC (&Settings_TK_fl[37],EditValue);break;   //WD   �27
//    case settings_Kol_Imp_scn:   WriteFlash32_CRC (&Settings_TK_fl[36],EditValue);break;      //WD   �26
//    case settings_risol_scn:  //Limit_Setting(8); //����������� ����� ������� RizolED,���
//                              WriteFlash32_CRC (&Settings_TK_fl[21],EditValue);break;         //WD  �16
//    case settings_idif_scn:   //Limit_Setting(14); //����������� ����� ������� I���,��
//                              WriteFlash32_CRC (&Settings_TK_fl[11],EditValue);break;          //WD  �15
//    case settings_Rdu_scn:    //Limit_Setting(10); //����������� ����� ������� ����� ������ R��
//                              WriteFlash32_CRC (&Settings_TK_fl[22],EditValue);break;         //WD  �19
//    case settings_KLB_AD_scn: WriteFlash32_CRC (&Settings_TK_fl[k_klb_AD],EditValue);break;  //WD  �59
//    case settings_toff_scn:   WriteFlash32_CRC (&Settings_TK_fl[60],EditValue);break;         //WD   �18
//    case settings_Rck_scn:    WriteFlash32_CRC (&Settings_TK_fl[59],EditValue);break;        //WD   �46
//    case settings_KLB_I1_scn:    WriteFlash32_CRC (&Settings_TK_fl[N_v_fl+1],EditValue);PK=1;break;     //WD   �29
//    case settings_KLB_I2_scn:    WriteFlash32_CRC (&Settings_TK_fl[N_v_fl+2],EditValue);PK=1;break;   //WD   �30
//    case settings_KLB_I3_scn:    WriteFlash32_CRC (&Settings_TK_fl[N_v_fl+3],EditValue);PK=1;break;   //WD   �31
//    case settings_KLB_Id_scn:    WriteFlash32_CRC (&Settings_TK_fl[96],EditValue);break;   //WD   �44
//    case settings_KLB_Uc_scn:    WriteFlash32_CRC (&Settings_TK_fl[42],EditValue);break;   //WD   �45
//    case aux_sett_K_Izol:       WriteFlash32_CRC (&Settings_TK_fl[46],EditValue);break;    //WD   �42
//    case aux_settings_Debug_scn:   WriteFlash32_CRC (&Settings_TK_fl[N_Brows],EditValue);  break; //WD  �49
//    case aux_setts_cosf_kpr_scn:   WriteFlash32_CRC (&Settings_TK_fl[62],EditValue);  break; //WD  �50
//    case settings_phas_scn:        WriteFlash32_CRC (&Settings_TK_fl[113],EditValue); break; //WD  �51
//    case settings_Levl_on_scn:   WriteFlash32_CRC (&Settings_TK_fl[34],EditValue);break;   //WD   �22 -34
//    case Id_syst_error:          WriteFlash32_CRC (&Settings_TK_fl[Idcr],EditValue);break;  //�72
//
//    //��������� ��������� ���
//
//                //����������/�������� ������ ��������� ��������     WD 24 12 2017
//   case userpsw_scn: if (EditValue==psw_setp)
//                        UserPswEnable|=0x1;  // ������ ������
//                      if (psw_setp==Settings_TK_fl[65]){psw_setp_wrk1=psw_setp;psw_setp=0;}
//                      if (psw_setp==Settings_TK_fl[66]){psw_setp_wrk2=psw_setp;psw_setp=0;}
//                        break;
//                //����������/�������� ������ ������������      WD 24 12 2017
//   case  Acces_set_No_Psw:
//                      if (EditValue==psw_setp_us)
//                       {WriteFlash32_CRC (&Settings_TK_fl[68],1);// ������ ������
//                        psw_setp_us=UsPswEnable=0;
//                       }
//                      if (EditValue==Settings_TK_fl[67])  //�������� ������
//                       {WriteFlash32_CRC (&Settings_TK_fl[68],0);// ����� ������
//                        psw_setp_us=UsPswEnable=0;
//                       }
//                      break;
//  }
//}
//
////������� ������ ������ ��� �������������� ������
//unsigned int psw_set_curr(Type_Def_Screen CurScrren)
// {unsigned int gop;
//   if ((CurScrren==settings_RJ_P_O_scn) || (CurScrren==settings_Tup_Dt_scn) ||  //Rj_P_O 24;  ��� ���� 25
//      (CurScrren==settings_BKL_P_3_scn) || (CurScrren==settings_Pn_shk_scn) || //BKL_P_3 28;  P� 12
//      (CurScrren == aux_In_shk_scn) || (CurScrren==settings_imax_scn) ||       //I�  38;      I� 13,
//      (CurScrren==aux_Imax_shk_scn) ||(CurScrren==settings_pmin_scn) ||         //Pmin 39;    AD_�� 14
//      (CurScrren==settings_Levl_off_scn)||(CurScrren==aux_setRTC_scn)||        //AD_�� 21;    ���� 35
//      (CurScrren==settings_add_scn)                                             //AD_C� 6;
//     )
//    gop=Settings_TK_fl [64];
//
//   if ((CurScrren==aux_settings_tzvpn_scn) || (CurScrren==settings_ton_scn) || //����� 33; ����� 17
//       (CurScrren==settings_PN_scn) || (CurScrren==settings_TVn_Sx_scn) ||       //�� 23;    TVn_Sx 20
//       (CurScrren==settings_Kol_BKL_scn)||(CurScrren==settings_risol_scn) ||     //���.��� 27; RizolED 16
//       (CurScrren==settings_Rdu_scn)||(CurScrren==settings_idif_scn) ||         //R�� 19; I��� 15
//       (CurScrren==Acces_set_No_Psw)                                       //??????
//       )
//   gop=Settings_TK_fl [65];
//
//   if ((CurScrren==settings_KLB_I1_scn) || (CurScrren==settings_KLB_I2_scn) || //KLB_I1 29; KLB_I2 30
//       (CurScrren==settings_KLB_I3_scn) || (CurScrren==settings_KLB_Id_scn) || //KLB_I3 31; KLB_Id 44
//       (CurScrren==settings_KLB_Uc_scn )|| (CurScrren==aux_sett_K_Izol) ||    //KLB_Uc 45;  K_RizolED 42
//       (CurScrren==settings_KLB_AD_scn) || (CurScrren==aux_param_num_scn) ||   //KLB_AD 34; ��  10
//       (CurScrren==Restr_Work) || (CurScrren==aux_param_num_scn1)||           //���� �������. �����.; ���1 ��  10
//       (CurScrren==aux_param_num_scn2) || aux_settings_Debug_scn ||            //���� �������. �����.; ���2 ��  10
//       (CurScrren==Id_syst_error)                                             // Idcr
//       )
//    gop=Settings_TK_fl [66];
//   return gop;
// }
//
///**������� �������� ����� ��������****/
////��� ������� ������ enter
//Type_Def_Screen ScreenEnter (Type_Def_Screen CurScrren)
//{
//   switch (CurScrren)
//   { case main_scn:           return param_scn;           // ������� �����, �0 -> �1
//       case param_scn:        return param_i_scn;         // ����� ��������� ��������, �1 -> �2
//       case param_iover_scn:  return param_iovers_scn;   // ����� ��������� �����, �7 -> �8
//       case aux_param_scn:    return Arx1_scn;           // ����� ������. �����, �9 -> �10
//       case settings_scn:     return aux_sett_Cnt_mod_scn; // ����� ������� ������. �����, �11 -> �40
//       case aux_In_shk_scn:   return settings_Pn_shk_scn;
//       case aux_settings_scn: return Acces_set_No_Psw; // ����� ������� ������. �����, �32 -> �66
//       //case Arx1_scn:         return Arx2_scn;          // ����� ������, �47 -> �48
//       case param_u_scn:      return param_react_power; // ������ �������� U -> �������� S � Q , �3 -> �52
//       case param_Iasmm_scn:  return param_loss_eff;    // ������ �������� �������� -> ������ ���., ... -> �53
//       //case param_rd_scn:     return param_tim_work;   // ������ �������� ����.�����. -> ����� ����������., ... -> �60
//
//
//     default:return CurScrren;
//  }
//}
//
////��� ������� ������ esc
//Type_Def_Screen ScreenEsc (Type_Def_Screen CurScrren)
//{
//  switch (CurScrren)
//   { case param_scn:
//     case aux_param_scn:
//     case settings_scn:
//     case aux_settings_scn:   //���������� ������� ����� ������ � ��������� �� ������� �����
//                              UserPswEnable&=~0x03;EditValueProp=0;
//                              return main_scn;
//
//       //�������� ���������  ���
//       case param_i_scn:
//       case param_u_scn:
//       case param_rd_scn:
//       case param_ia_scn:
//       //case settings_add_scn:
//       case param_Iasmm_scn:
//       case param_iover_scn:  return param_scn;
//       case param_iovers_scn: return param_scn;
//       case param_react_power: return param_scn;     //  �52
//       case param_loss_eff:    return param_scn;     //  �53
//       //case param_EKM_DP :     return param_scn;     //  �54
//       //case param_RD_PP:       return param_scn;     //  �55
//       case param_DV_TR:       return param_scn;     //  �56
//       case param_tim_work:    return param_scn;     //  �60
//
//       //�������� ���������  ���
//
//        //�����  ���
//       case Arx1_scn:                                   //  �47
//       case Arx2_scn:             return aux_param_scn;  //  �48 -> �9
//       //�����  ���
//
//        //��������� ��������� ���
//       case aux_sett_Cnt_mod_scn:  // �40  22 12 2017
//       case settings_RJ_P_O_scn:   // �24
//       case settings_BKL_P_3_scn:  // �28
//       case settings_Tup_Dt_scn:   // �25  22 02 2016
//       case settings_Pn_shk_scn:   // �12
//       case aux_In_shk_scn:        // �38    20 02 2016
//       case settings_imax_scn:     // �13
//       case aux_Imax_shk_scn:      // �39    21 02 2016
//       case settings_pmin_scn:     // �14
//       case settings_add_scn:      // �6  10 01 2018
//       //case settings_Psw_usr_scn:             // �65
//       //case Acces_set_No_Psw:                // �66
//       case settings_Levl_off_scn: // �49 (�����), ������� 21
//       //case Restr_Work:                       // �67  22 06 2017
//       case aux_setRTC_scn:        // �35
//       case No_set_Psw:
//                    return settings_scn; // �11
//        //��������� ��������� ���
//
//         //��������� ��������� ���              23 12 2017
//        case Acces_set_No_Psw:          // �66
//        case imag_UserPsw:              // �71
//        case aux_settings_tzvpn_scn:  // �33
//        case settings_ton_scn:          // �17
//        case  settings_PN_scn:          // �23
//        case settings_TVn_Sx_scn:       // �20
//        case settings_Kol_BKL_scn:      // �27
//        case settings_risol_scn:        // �16
//        case settings_Rdu_scn:          // �19
//        case settings_idif_scn:         // �15
//        case settings_KLB_I1_scn:       // �29
//        case settings_KLB_I2_scn:       // �30
//        case settings_KLB_I3_scn:       // �31
//        case settings_KLB_Id_scn:       // �44
//        case settings_KLB_Uc_scn:       // �45
//        case aux_sett_K_Izol:           // �42
//        case settings_KLB_AD_scn:       // �59
//        case settings_phas_scn:         // �51
//        case aux_param_num_scn:         // �10
//        case aux_Serial_Number:
//        case aux_param_num_scn1:        //���1 �10
//        case aux_param_num_scn2:        //���2 �10
//        //case settings_Tzv_pn_scn:     // �61
//        //case settings_Pmin_d_scn:       // �62
//        //case settings_Pmax_d_scn:       // �63
//        //case settings_J_jok_scn:       // �64
//        //case settings_Kol_Imp_scn:  // �26
//        //case settings_Rck_scn:      // �46
//        //case aux_settings_Debug_scn:  // �49
//        //case aux_setts_cosf_kpr_scn:  // �50
//        case  N_Brows_s:             // �58
//        case Id_syst_error:          // �72
//                    return aux_settings_scn;
//         //��������� ��������� ���
//
//       //case settings_Levl_on_scn:    // ????
//       //case settings_Tup_Dt_scn:
//       //case settings_Kol_Imp_scn:
//       //case settings_Kol_BKL_scn:
//       //case settings_BKL_P_3_scn:
//
//
//       //case aux_settings_tzvpn_scn:
//       //case aux_setRTC_scn:
//      // case aux_Imax_shk_scn:
//      // case aux_sett_Cnt_mod_scn:
//      //  case aux_sett_K_Izol:           //�42
//      //   return aux_settings_scn;
//
//       case  userpsw_scn: return EditedScrren; // ????
//    default:return CurScrren;
//  }
//}
//
////��� ������� ������ Down (������, "-"
//Type_Def_Screen ScreenDown (Type_Def_Screen CurScrren)
//{
//  switch (CurScrren)
//   { //����� ����� ����������,��� ������:
//    //�1, param_scn         - ���������� ���������
//    //�11, settings_scn     - �������� �����������
//    //�32, aux_settings_scn - ��������� ���������
//    //�9, aux_param_scn     - ����� ������ ��� �������
//    /*
//     //�� 03 12 2017
//    case param_scn:         return   aux_param_scn;  // �1 -> �9
//    case aux_param_scn:     return   settings_scn;      // �9 -> �11
//    case settings_scn:      return   aux_settings_scn;  // �11-> �32
//    */
//     //����� 03 12 2017
//    case param_scn:         return   settings_scn;  // �1 -> �11
//    case settings_scn:      return   aux_settings_scn;  // �11 -> �32
//    case aux_settings_scn:  return   aux_param_scn;  // �32 -> �9
//
//    //�������� �������� ����������
//    case param_i_scn:     return  param_u_scn;            // �2-> �3
//    case param_u_scn:     return  param_Iasmm_scn;        // �3 -> �43
//    case param_Iasmm_scn: return param_ia_scn;            // �43-> �5
//    case param_ia_scn:    return param_rd_scn;           // �5 -> �4
//    //case param_add_scn:   return param_rd_scn;            // �6 -> �4
//    //case param_rd_scn:    return param_EKM_DP;           //�4 -> �54
//    //case param_EKM_DP:    return param_RD_PP;            //�54 -> �55
//    //case param_rd_scn:    return param_RD_PP;              //�4  -> �55
//    //case param_RD_PP:     return param_DV_TR;            //�55 -> �56
//    case param_rd_scn:      return param_DV_TR;            //��4  -> �56
//    case param_DV_TR:     return param_iover_scn;        //�56 -> �7
//
//    //�����
//    case Arx1_scn:    return Arx2_scn;        //�47 -> �48
//    case Arx2_scn:      return Arx1_scn;        //�48 -> �47
//
//    //������� � �������� ������
//    case param_loss_eff:    return  param_ia_scn; // ������ �������� ������. -> I���� , �53 -> �5
//   //������� ����� ������� ������������� ��������� ���������
//   // case No_set_Psw:            return settings_scn; // �70
//
//    //��������� ������� �����. ��������
//    case aux_sett_Cnt_mod_scn:    return  settings_RJ_P_O_scn;      // �40 -> �24
//    case settings_RJ_P_O_scn:     return  settings_BKL_P_3_scn;     // �24 -> �28
//    case settings_BKL_P_3_scn:    return  settings_Tup_Dt_scn;      // �28 -> �25
//    case settings_Tup_Dt_scn:     return  settings_Pn_shk_scn;      // �25 -> �12
//    case settings_Pn_shk_scn:     return  aux_In_shk_scn;           // �12 -> �38
//    case aux_In_shk_scn:          return  settings_imax_scn;        // �38 -> �13
//    case settings_imax_scn:       return aux_Imax_shk_scn;          // �13 -> �39
//       //���� �������� ������� ������� ������ ������� �14 � � 49
//       //��� ������� ADD,���� ���� ������ �� ����������,
//       //�.�. Settings_TK_fl[35]!=3                     22 12 17
//    case aux_Imax_shk_scn:        if(Settings_TK_fl[35]==3)
//                                   return settings_pmin_scn;         // �39 -> �14
//                                   else return  aux_setRTC_scn;       // �39 -> �35
//    case settings_pmin_scn:       if(Settings_TK_fl[35]==3)
//                                   return  settings_add_scn;    // �14 -> �49  // �49 -> �6
//                                  else return  aux_setRTC_scn;       // �14 -> �35
//    case settings_add_scn:        return   settings_Levl_off_scn;    // �6 -> �49
//    case settings_Levl_off_scn:   return  aux_setRTC_scn;           // �49 -> �35
//    //case settings_add_scn:                                          // �6
//    //case aux_setRTC_scn:          return  settings_Psw_usr_scn;     // �35 -> �65
//    //case settings_Psw_usr_scn:    return  Restr_Work;               // �65 -> �67
//
//      //��������� ������� ���������
//    case Acces_set_No_Psw:           return imag_UserPsw;            //�66 -> �71
//    case  imag_UserPsw:             return aux_settings_tzvpn_scn;   //�71 -> �33
//    case aux_settings_tzvpn_scn:  return settings_ton_scn;         // �33-> �17
//    case settings_ton_scn:          return settings_PN_scn;          // �17-> �23
//          //case settings_Tzv_pn_scn:        return settings_Pmin_d_scn;           // �61
//          //case settings_Pmin_d_scn:       return settings_Pmax_d_scn;            // �62 �63
//          //case settings_Pmax_d_scn:       return settings_J_jok_scn;             // �63 -> �63
//          //case settings_J_jok_scn:        return settings_PN_scn;                // �64 -> �23
//    case settings_PN_scn:           return settings_TVn_Sx_scn;      // �23 -> �20
//    case settings_TVn_Sx_scn:       return settings_Kol_BKL_scn;     // �20 -> �27
//    case settings_Kol_BKL_scn:      return settings_risol_scn;       // �27 -> �16
//            //case settings_risol_scn:        return settings_idif_scn;             // �16 -> �15
//    case settings_risol_scn:         return settings_Rdu_scn;        // �16 -> �19
//    case settings_Rdu_scn:         return settings_idif_scn;         // �19 -> �15
//           // case settings_Rdu_scn:          return  settings_Rck_scn;             // �19 -> �46
//    case settings_idif_scn:         return settings_KLB_I1_scn;      // �15 -> �29
//    case settings_KLB_I1_scn:       return  settings_KLB_I2_scn;     // �29 -> �30
//    case settings_KLB_I2_scn:       return  settings_KLB_I3_scn;     // �30-> �31
//    case settings_KLB_I3_scn:       return  settings_KLB_Id_scn;     // �31-> �44
//    case settings_KLB_Id_scn:       return  settings_KLB_Uc_scn;     // �44-> �45
//    case settings_KLB_Uc_scn:       return  aux_sett_K_Izol;         // �45-> �42
//    case aux_sett_K_Izol:           return  settings_KLB_AD_scn;     // �42 -> �59
//    case settings_KLB_AD_scn:       return  Id_syst_error;          // �59 -> �72
//    case Id_syst_error:             return  settings_phas_scn;      // �72 -> �51
//    case settings_phas_scn:         scr10=1; return  aux_param_num_scn; // �51 -> �10
//    case aux_param_num_scn:         scr10=2; return  aux_param_num_scn1; // �10 -> ��� �10
//    case aux_param_num_scn1:        scr10=3; return  aux_param_num_scn2; // �10 -> ��� �10
//    case aux_param_num_scn2:        if(G_fl==1) return  aux_settings_Debug_scn; //��� �10 -> �50
//                                    if(G_fl==0) return  aux_Serial_Number;
//    case aux_settings_Debug_scn:    if(G_fl==1)  return  N_Brows_s;
//    case N_Brows_s:                 return  aux_Serial_Number;
//                   //case aux_param_num_scn:         return  aux_settings_Debug_scn; // �10 -> �49
//                  //case aux_settings_Debug_scn:    return  aux_setts_cosf_kpr_scn; // �49 -> �50
//                 //case aux_setts_cosf_kpr_scn:    return  N_Brows_s;              // �50 > �58
// default:return CurScrren;
//  }
//}
//
//
////��� ������� ������ up
//
//Type_Def_Screen ScreenUp (Type_Def_Screen CurScrren)
//{  switch (CurScrren)
//  {
// //����� ����� ����������,��� ������:
//    //�1, param_scn         - ���������� ���������
//    //�11, settings_scn     - �������� �����������
//    //�32, aux_settings_scn - ��������� ���������
//    //�9, aux_param_scn     - ����� ������ ��� �������
//
//    /*
//    //�� 03 12 2017
//    case aux_param_scn:    return param_scn;       //�9 -> �1
//    case settings_scn:     return aux_param_scn;   //�11 -> �9
//    case aux_settings_scn: return settings_scn;    //�32 -> �11
//    */
//     //����� 03 12 2017
//    case aux_param_scn:     return aux_settings_scn;  //�9 -> �32
//    case aux_settings_scn:  return settings_scn;      //�32 -> �11
//    case settings_scn:      return param_scn;        //�11 -> �1
//
//    //////////
//    //�������� �������� ����������
//    case param_u_scn:     return  param_i_scn;        // �3 -> �2
//    case param_Iasmm_scn: return  param_u_scn;        // �43 -> �3
//    case param_ia_scn:    return  param_Iasmm_scn;    // �5 -> �43
//    //case param_add_scn:   return  param_ia_scn;       // �6 -> �5
//    case param_rd_scn:    return  param_ia_scn;      // �4 -> �5
//    //case param_EKM_DP:    return  param_rd_scn;       // �54 -> �4
//    //case param_RD_PP:     return  param_EKM_DP;       // �55 -> �54
//    //case param_RD_PP:     return  param_rd_scn;       // �54 -> �4
//    //case param_DV_TR:     return  param_RD_PP;        // �56 -> �55
//    case param_DV_TR:       return  param_rd_scn;       // �56 -> �4
//    case param_iover_scn:   return  param_DV_TR;        // �7 -> �56
//    case param_iovers_scn:  return param_iover_scn;    // �8 -> �7
//    //case param_react_power: return  param_u_scn; // ������ �������� �������� S � Q -> U , �52 -> �3
//    case param_react_power: return  param_ia_scn; // ������ �������� �������� S � Q -> U , �52 -> �5
//
//    //case param_loss_eff:    return  param_Iasmm_scn; // ������ �������� ������. -> ����� , �53 -> �43
//   ////// case param_loss_eff:    return  param_ia_scn; // ������ �������� ������. -> I���� , �53 -> �5
//    //case param_tim_work:    return  param_EKM_DP; // ������ �������� ������. -> ����� , �60 -> �54
//    case param_tim_work:    return param_DV_TR;     //����� ��������� ������� -> �56
//
//
//        //������� �����. ��������� ���                  22 12 2017
//    case settings_RJ_P_O_scn:    return  aux_sett_Cnt_mod_scn;          // �24 -> �40
//    case settings_BKL_P_3_scn:   return  settings_RJ_P_O_scn;           // �28 -> �24
//    case settings_Tup_Dt_scn:    return settings_BKL_P_3_scn;           // �25 -> �28
//    case settings_Pn_shk_scn:     return settings_Tup_Dt_scn;           // �12 -> �25
//    case aux_In_shk_scn:     return settings_Pn_shk_scn;                // �38 -> �12
//    case settings_imax_scn:       return aux_In_shk_scn;                // �13 -> �38
//    case aux_Imax_shk_scn:        return settings_imax_scn;             // �39 -> �13
//    case settings_pmin_scn:       return aux_Imax_shk_scn;              // �14 -> �39
//    //case settings_Levl_off_scn:   if(Settings_TK_fl[35]==3)
//    //                               return  settings_pmin_scn;            // �49 -> �14
//    //                               else return aux_Imax_shk_scn;        //  �49-> �39
//    case settings_add_scn:         return  settings_pmin_scn;           //�6  -> �14
//    case settings_Levl_off_scn:    return  settings_add_scn;         //�49 -> �6
//    case aux_setRTC_scn:          if(Settings_TK_fl[35]==3)
//                                    return  settings_Levl_off_scn ;     //  �35-> �49
//                                   else return aux_Imax_shk_scn;        //  �35-> �39
//
//    //case settings_Psw_usr_scn:    return  aux_setRTC_scn;             //  �65-> �35
//    //case Restr_Work:              return  settings_Psw_usr_scn;       //  �67-> �66
//       //������� �����. ��������� ���
//
//      //������� ��������� ���
//    case Acces_set_No_Psw:         return aux_settings_scn;           //�66-> �32
//     case imag_UserPsw:              return  Acces_set_No_Psw;        // �71-> �66
//    case aux_settings_tzvpn_scn:  return imag_UserPsw;               // �33-> �71
//    case settings_ton_scn:          return aux_settings_tzvpn_scn;   // �17 -> �33
//         //case settings_Tzv_pn_scn:        return settings_ton_scn;                // �61->
//         //case settings_Pmin_d_scn:       return settings_Tzv_pn_scn;              // �62->
//         //case settings_Pmax_d_scn:       return settings_Pmin_d_scn;              // �63->
//         //case settings_J_jok_scn:        return settings_Pmax_d_scn;              // �64->
//    case settings_PN_scn:           return settings_ton_scn;          // �23-> �17
//    case settings_TVn_Sx_scn:       return settings_PN_scn;           // �20-> �23
//    case settings_Kol_BKL_scn:      return settings_TVn_Sx_scn;       // �27-> �20
//    case settings_risol_scn:        return settings_Kol_BKL_scn;      // �16-> �27
//        //case settings_risol_scn:         return settings_risol_scn;                // �15->
//    case settings_Rdu_scn:          return settings_risol_scn;       // �19-> �16
//         //case settings_Rck_scn:          return settings_Rdu_scn;                  // �46->
//    case settings_idif_scn:         return  settings_Rdu_scn;         // �15 ->�19
//    case settings_KLB_I1_scn:       return  settings_idif_scn;        // �29 -> �15
//    case settings_KLB_I2_scn:       return  settings_KLB_I1_scn;      // �30 -> �29
//    case settings_KLB_I3_scn:       return  settings_KLB_I2_scn;      // �31 -> �30
//    case settings_KLB_Id_scn:       return  settings_KLB_I3_scn;      // �44 -> �31
//    case settings_KLB_Uc_scn:       return  settings_KLB_Id_scn;      // �45 -> �44
//    case aux_sett_K_Izol:           return  settings_KLB_Uc_scn;      // �42-> �45
//    case settings_KLB_AD_scn:       return  aux_sett_K_Izol;          // �59 -> �42
//    case Id_syst_error:             return  settings_KLB_AD_scn;      // �72 -> �59
//    case settings_phas_scn:         return  Id_syst_error;            // �51 -> �72
//    case aux_param_num_scn:         return  settings_phas_scn;        // �10 -> �51
//    case aux_param_num_scn1:        scr10=1; return aux_param_num_scn;  //��� �10 -> �10
//    case aux_param_num_scn2:        scr10=2; return aux_param_num_scn1; // �10 ->��� �10
//    case aux_settings_Debug_scn:    scr10=3; return aux_param_num_scn2; // �36 ->��� �10
//    case  N_Brows_s:                return aux_settings_Debug_scn;
//    case  aux_Serial_Number:        if(G_fl==1) return N_Brows_s;
//                                    if(G_fl==0) return aux_param_num_scn2;
//    //case aux_setts_cosf_kpr_scn:    return  aux_settings_Debug_scn;  // �51
//   //case  N_Brows_s:                return  aux_setts_cosf_kpr_scn;  // �58->�51    settings_phas_scn;
//
//      //������� �������� ���
//   default:return CurScrren;
//  }
//}
//
//
////������� ������ ������ �� ���
//void ScreenToLCD (Type_Def_Screen Screen, Type_Def_ScreenMode Mode)
//{
//  if ((EditValueProp&0x02)&&(!(EditValueProp&0x01))) {writeLCDc(0x0C); EditValueProp&=~0x02;} //��������� ������, ���� ��������� �������
//
//  switch (Screen)
//  { case main_scn:          MainScreen(Mode);break;
//
//    // ������ �������� ���. ����������
//    case param_scn:         Contr_Bas_Param_Screen(Mode);     break; // ����� �������� ���. ���������� WAS , �1
//    case param_i_scn:
//                              Contr_Bas_Param_i_scn(Mode);      break;                  //�2
//    case param_u_scn:       Contr_Bas_Param_u_scn(Mode);      break;                  //�3
//    case param_Iasmm_scn:   Contr_Bas_Param_Iasmm_scn(Mode);    break;                //�43
//    case param_ia_scn:      Contr_Bas_Param_ia_scn(Mode);     break;                  //�5
//    case param_rd_scn:      Contr_Bas_Param_rd_scn(Mode);     break;                  //�4
//    //case param_add_scn:     Contr_Bas_Param_add_scn(Mode);    break;                  //�6
//    case param_iover_scn:   Contr_Bas_Param_iover_scn(Mode);  break;                  //�7
//    case param_iovers_scn:  Contr_Bas_Param_iovers_scn(Mode); break;                  //�8
//    case param_react_power: Cntr_Prm_react_power_scn(Mode);   break;                   //�52
//    case param_loss_eff:    Cntr_Prm_loss_eff_scn(Mode);      break;                   //�53
//    //case  param_EKM_DP:     Cntr_Prm_EKM_DP_scn(Mode);        break;                   //�54
//    //case  param_RD_PP:      Cntr_Prm_RD_PP_scn(Mode);         break;                   //�55
//    case  param_DV_TR:      Cntr_Prm_DV_TR_scn(Mode);         break;                   //�56
//    case param_tim_work:    Contr_Param_tim_work(Mode);        break;                   //�60
//
//    // ������ �������
//    case aux_param_scn:     Contr_Add_Param_Screen(Mode);       break;  // ����� ��������� ��������������//�9
//    case Arx1_scn:          Param_nz[32]=0; Arx_1_i_scn(Mode);  break;               //�47
//    case Arx2_scn:          Arx_2_i_scn(Mode);                  break;              //�46
//
//    // ������ ������� �����. ���������
//    case settings_scn:             Setup_Bas_Param_Screen(Mode);  break; // ����� ��������� �������� //�11
//    case settings_Pn_shk_scn:          Setup_Bas_iw_scn(Mode);        break;   //�12
//    case aux_In_shk_scn:           Setup_Add_In_shk_scn(Mode);    break;   //�38  02 03 2016
//    case settings_imax_scn:        Setup_Bas_imax_scn(Mode);      break;   //�13
//    case aux_Imax_shk_scn:         Setup_Add_Imax_shk_scn(Mode);  break;   //�39 02 03 2016
//    case settings_BKL_P_3_scn:     Setup_Bas_BKL_P_3_scn(Mode);   break;  //�28
//    case settings_pmin_scn:        Setup_Bas_VU_ADD_scn(Mode);    break; //�14   21 02 2016
//    case settings_Levl_off_scn:    Setup_Bas_NU_ADD_scn(Mode);    break;  //�49
//    case settings_add_scn:         Contr_Bas_Param_add_scn(Mode); break;  //�6
//    case aux_sett_Cnt_mod_scn:     Setup_Add_Cnt_mod_scn(Mode);   break;  //�40  22 02 2016
//    case settings_RJ_P_O_scn:      Setup_Bas_RJ_P_O_scn(Mode);    break;  //�24
//    case settings_Tup_Dt_scn:      Setup_Bas_Tup_Dt_scn(Mode);    break;  //�25
//    case aux_setRTC_scn:                 CalendarScreen(Mode);    break; //�35
//    case settings_Psw_usr_scn:     Stp_Psw_usr (Mode);            break; //�65
//    case Restr_Work:               Restr_Work_Sett(Mode);         break; //�67
//    case No_set_Psw:               Stp_No_set_Psw(Mode);         break; //�70
//
//
//    // ������ ��������� �������
//    case aux_settings_scn:         Setup_Add_Param_Screen(Mode); break; //  �32
//    case aux_param_num_scn:        Contr_aux_Param_num_scn(Mode); scr10=1; break;       //�10
//    case aux_param_num_scn1:       Contr_aux_Param_num_scn(Mode); scr10=2; break;       //�10
//    case aux_param_num_scn2:       Contr_aux_Param_num_scn(Mode); scr10=3; break;       //�10
//    case aux_settings_tzvpn_scn:   Setup_TZVpn_scn(Mode);  break; //�33  24 07 2015
//    case settings_ton_scn:         Setup_Bas_ton_scn(Mode);        break; //�17
//    case settings_Tzv_pn_scn:       Stp_Bas_Tzv_pn_scn(Mode);      break;  // �61
//    case settings_Pmin_d_scn:      Stp_Bas_Pmin_d_scn(Mode);       break; // �62
//    case settings_Pmax_d_scn:      Stp_Bas_Pmax_d_scn(Mode);       break; // �63
//    case settings_J_jok_scn:       Stp_Bas_J_jok_scn(Mode);       break;  // �64
//    case settings_PN_scn:          Setup_Bas_PN_scn(Mode);        break;  //�23
//    case settings_TVn_Sx_scn:      Setup_Bas_TVn_Sx_scn(Mode);    break;  //�20
//    case settings_Kol_BKL_scn:     Setup_Bas_Kol_BKL_scn(Mode);   break;  //�27
//    //case settings_Kol_Imp_scn:     Setup_Bas_Kol_Imp_scn(Mode);   break; //�26 ??????
//    case settings_risol_scn:       Setup_Bas_risol_scn(Mode);     break; //�16
//    case settings_idif_scn:        Setup_Bas_idif_scn(Mode);      break; //�15
//    case settings_Rdu_scn:         Setup_Bas_Rdu_scn(Mode);       break; //�19
//    case settings_KLB_AD_scn:      Set_Bs_klb_AD_scn(Mode);       break; //�59
//    //case settings_toff_scn:        Setup_Bas_toff_scn(Mode);      break; //�18  ??????
//    //case aux_serial_Numb:        Setup_Add_Typ_dm(Mode);        break; //�41      27 01 2016
//    case settings_Rck_scn:         Setup_Bas_Rck_scn(Mode);       break; //�46      24 02 2016
//    case aux_sett_K_Izol:          Setup_Add_K_Izol(Mode);         break;          //�42      09 02 2016
//    case settings_KLB_I1_scn:      Setup_Bas_KLB_I1_scn(Mode);    break;  //�29
//    case settings_KLB_I2_scn:      Setup_Bas_KLB_I2_scn(Mode);    break;  //�30
//    case settings_KLB_I3_scn:      Setup_Bas_KLB_I3_scn(Mode);    break;  //�31
//    case settings_KLB_Id_scn:      Setup_Bas_KLB_Id_scn(Mode);    break;  //�44,
//    case settings_KLB_Uc_scn:      Setup_Bas_KLB_Uc_scn(Mode);    break;  //�45
//    case aux_settings_Debug_scn:   Setup_Add_Debug_scn(Mode);     break;  //�50
//    case aux_setts_cosf_kpr_scn:   Setup_Bas_cosf_kpr_scn(Mode);  break;  // �51
//    case settings_phas_scn:        Cntr_phas_scn(Mode);           break;  // �57
//    case N_Brows_s:                N_Brows_scn(Mode);             break;  // �58
//    case imag_UserPsw:             imag_UserPsw_Scr(Mode);         break; //�71
//    case userpsw_scn:              UserPsw_Screen(Mode);         break;  //�37 ����� ������
//    case Acces_set_No_Psw:         Stp_Acces_set_No_Psw(Mode);   break; //�66
//    case aux_Serial_Number:        Serial_Number_Screen(Mode);   break;  //�36
//    case Id_syst_error:            Stp_Id_syst_error_scn(Mode);   break;  //�72
//    default: MainScreen(all);break;
//   }
//}
//
///********************************************************************************************/
//Type_Def_ButtonMode KeyCommand;
//
//// ������� ������� �� ������� ����
//void ScreenDispetcher (int8_t Sync)
//{
//  static Type_Def_Screen CurScrren=main_scn;
//
//  static uint32_t PswTime=0;
//  static char tmp_s=0;
//  //C�������� �����
//  KeyCommand = KeyFunction();
//
//  //��������� ����� ����� ��������� ������
//  if(KeyCommand!=none_key) PswTime=GET_SYSTIMING;
//
// // �������������� ����������� �� �������� �����, ���� ������ ��������� �� ����������
//  if(TimeRefSysTick (PswTime)>= PSW_TIME * SYSTICK_IRQ_FREQ)
//  { CurScrren=main_scn;
//    MainScreen(all);
//    PswTime=GET_SYSTIMING;
//    writeLCDc(0x0C);//��������� ������ �� ���� ������
//    psw_setp=psw_setp_wrk1=psw_setp_wrk2=0;           //WS  17 12 2017
//    UserPswEnable&=~0x03; EditValueProp=0;
//    return;
//  }
//
//  //if(CurScrren!= aux_userpswchange_scn) UserPswEnable&=~0x02; //���������� ��������� ���������� ��������� ������
//     //�� ���������� �������������� ������� R2,���� ������ ������ �� R1 ���
//  if((UserPswEnable&0x1)&&(psw_setp_wrk1)&&(psw_setp_wrk2))
//   { UserPswEnable&=~0x03;EditValueProp=0; //�������� ������
//     psw_setp_wrk1=psw_setp_wrk2=0;
//   }
//  if((CurScrren==param_scn)||(CurScrren==settings_scn)||(CurScrren==aux_param_scn))
//   { UserPswEnable&=~0x03;EditValueProp=0; //�������� ������
//     psw_setp_wrk1=psw_setp_wrk2=0;
//   }
//
//  switch (CurScrren)
//  {  default:
//          if(CurScrren!=settings_KLB_Uc_scn) Block_Dwn_UC=0;
//            switch (KeyCommand)
//            { case up:       CurScrren=ScreenUp(CurScrren); ScreenToLCD(CurScrren,all); break;
//              case down:     CurScrren=ScreenDown(CurScrren); ScreenToLCD(CurScrren,all); break;
//              case longup:   if(Sync){CurScrren=ScreenUp(CurScrren); ScreenToLCD(CurScrren,all);}; break;
//              case longdown: if(Sync){CurScrren=ScreenDown(CurScrren); ScreenToLCD(CurScrren,all);}; break;
//              case enter:    CurScrren=ScreenEnter(CurScrren); ScreenToLCD(CurScrren,all); break;
//              case esc:      CurScrren=ScreenEsc(CurScrren);   ScreenToLCD(CurScrren,all); Param_nz[15]=1;  break;
//
//              case none_key:
//              default: if(Sync)ScreenToLCD(CurScrren,values); break;
//             }
//        break;
//
//      //����� ���� �������������� �������� �������
//      case  aux_In_shk_scn:       // �38
//      case  settings_Pn_shk_scn:      // �12
//      case  settings_imax_scn:    // �13
//      case  aux_Imax_shk_scn:     // �39
//      case  settings_pmin_scn:     // �14
//      case  settings_add_scn:      // �6  10 01 2018
//      case  settings_Levl_off_scn: // �49
//      case  settings_RJ_P_O_scn:   // �24
//      case  settings_Tup_Dt_scn:   // �25
//      case  settings_idif_scn:
//      case  settings_risol_scn:
//      case  settings_ton_scn:      // �17
//      case settings_Tzv_pn_scn:     // �61
//      case settings_Pmin_d_scn:    // �62
//      case settings_Pmax_d_scn:    // �63
//      case settings_J_jok_scn:     // �64
//      case settings_Psw_usr_scn:   // �65
//      case Restr_Work:             // �67
//      case  settings_toff_scn:
//      case  settings_Rdu_scn:
//      case  settings_KLB_AD_scn:    // �59
//      case  settings_TVn_Sx_scn:
//      //case  settings_Levl_off_scn:
//      case  settings_Levl_on_scn:
//      case  settings_PN_scn:
//      //case  settings_Tup_Dt_scn:
//      case  settings_Kol_Imp_scn:
//      case  settings_Kol_BKL_scn:
//      case  settings_BKL_P_3_scn:
//      case  settings_KLB_I1_scn:
//      case  settings_KLB_I2_scn:
//      case  settings_KLB_I3_scn:
//      case  aux_settings_tzvpn_scn:
//      case  aux_settings_Debug_scn:     //WD 07 01 2017  �49 ????
//      case  aux_setts_cosf_kpr_scn:     //WD 10 01 2017  �50
//      case  settings_phas_scn:          // 02 2017    �51
//      case  aux_param_num_scn:        // �10
//      case  aux_param_num_scn1:       //���1 �10
//      case  aux_param_num_scn2:       //���2 �10
//      case  aux_sett_Cnt_mod_scn:   //WD 02 12 2015 �40
//      case  settings_Rck_scn:        // 24 02 2016   �46
//      case  settings_KLB_Id_scn:      // 24 02 2016  �44
//      case  settings_KLB_Uc_scn:      // 24 02 2016  �45,
//      case  aux_sett_K_Izol:         //WD 09 02 2016 �42
//      case  aux_setRTC_scn:
//      case  userpsw_scn:                // �37
//      case  Acces_set_No_Psw:           // �66
//      case  Id_syst_error:              // �72
//
//         if (EditValueProp&0x01)
//          {  //� ��������� �������� min � max ������� ������� �����. Sett_min � Sett_max,������
//              Sett_min=Sett_max=0;
//            if (CurScrren==settings_Pn_shk_scn)
//              Min_Max_Sett(16,17); // P�, 12
//             if(CurScrren == aux_In_shk_scn)
//              Min_Max_Sett_TK412_d(36,100,0,0);      //I�, 38
//            if (CurScrren==settings_imax_scn)
//             {Sett_min=0; Sett_max=100;}             //I�, 13  -����� ���������� I�
//            if (CurScrren==aux_Imax_shk_scn)
//              {Sett_min=0; Sett_max=100; }           //Pmin, 39 -����� ���������� P�
//            if (CurScrren==settings_pmin_scn)
//              {Sett_min=0; Sett_max=100;}           //�������� % 22 12 2017
//                //Min_Max_Sett(60,61);              //AD_��, 14 �� --min,max
//            if (CurScrren==settings_add_scn)            //AD_��,
//              {Sett_min=0; Sett_max=100;}           //�������� %
//            if (CurScrren==settings_Levl_off_scn)
//             { Sett_min=0; Sett_max=100;}           //�������� %  22 12 2017
//               // Min_Max_Sett(62,63);             //AD_��, 21  ,�� -min,max
//            if (CurScrren==aux_sett_K_Izol)
//             Min_Max_Sett(8,9);                    //K_RizolED 42, - min,max
//            if (CurScrren==settings_ton_scn)
//              Min_Max_Sett(0,1);                    //�����, 17 - min,max
//            if (CurScrren==settings_PN_scn)
//              Min_Max_Sett(2,3);                    //�� - min,max, 23
//            if (CurScrren==settings_Kol_BKL_scn)
//              Min_Max_Sett(6,7);                 //���.���, 27   /10_minut,
//            if (CurScrren==aux_settings_tzvpn_scn)
//              Min_Max_Sett(64,65);               //�����, 33
//            if (CurScrren==settings_Tzv_pn_scn)
//              Min_Max_Sett(66,67);              //Tzv_pn, 61
//            if (CurScrren==settings_Pmin_d_scn)
//              Min_Max_Sett(68,69);             //Pmin_d, 62
//            if (CurScrren==settings_Pmax_d_scn)
//              Min_Max_Sett(70,71);              //Pmax_d, 63
//            if (CurScrren==settings_TVn_Sx_scn)
//              Min_Max_Sett(72,73);              //TVn_Sx, 20
//            if (CurScrren==settings_risol_scn)
//              Min_Max_Sett(74,75);              //RizolED, 16
//            if (CurScrren==settings_Rdu_scn)
//              Min_Max_Sett(10,11);              //R��, 19
//            if (CurScrren==settings_idif_scn)
//              Min_Max_Sett(14,15);              //I���, 15
//            if (CurScrren==settings_KLB_I1_scn)
//             {Sett_min=10;Sett_max=200;}        //KLB_I1, 29
//            if (CurScrren==settings_KLB_I2_scn)
//             {Sett_min=10;Sett_max=200;}        //KLB_I2, 30
//            if (CurScrren==settings_KLB_I3_scn)
//             {Sett_min=10;Sett_max=200;}        //KLB_I3, 31
//            if (CurScrren==settings_KLB_Id_scn)
//             {Sett_min=10;Sett_max=200;}        //KLB_Id, 44
//            if (CurScrren==settings_KLB_Uc_scn)
//             {Sett_min=2900;Sett_max=6000;        // 2900->1500
//              Block_Dwn_UC=1; }                //KLB_Uc, 45
//            if (CurScrren==settings_KLB_AD_scn)  //KLB_AD
//             {Sett_min=10;Sett_max=300;}                     // 20 01 2018
//            if(CurScrren==aux_param_num_scn);     //��, 10
//            if(CurScrren==aux_param_num_scn1);    //��, 10
//            if(CurScrren==aux_param_num_scn2);     //��, 10
//            if (CurScrren==settings_KLB_AD_scn)  //KLB_AD
//            if (CurScrren==aux_settings_Debug_scn) //KLB_Uc, 45
//             {Sett_min=0;Sett_max=5; }
//            if (CurScrren==Id_syst_error) //Idcr (������. ����.������ Id), 72
//             {Sett_min=0;Sett_max=999; }
//            if(CurScrren==settings_Psw_usr_scn); //��� ????
//            //if (CurScrren==Acces_set_No_Psw)
//            // {Sett_min=0;Sett_max=1;       //��� ������ ������������
//
//            switch (KeyCommand)  //����� ��������������
//             { case longup:
//                case up:
//                      if (CurScrren==aux_setRTC_scn) EditRTCFuncUP();
//                          //��������� �������� ��������� ����� �� ������ �� 1 �����
//                      else {if ((CurScrren==aux_In_shk_scn)||(CurScrren==settings_Pn_shk_scn)||            //I�, P�
//                                 (CurScrren==settings_imax_scn) || (CurScrren==aux_Imax_shk_scn)||         //Pmin
//                                 (CurScrren==settings_pmin_scn) || (CurScrren==settings_Levl_off_scn)||     // AD_��, AD_H�
//                                 (CurScrren==aux_sett_K_Izol)||(CurScrren==settings_ton_scn)||             //K_RizolED, �����po
//                                 (CurScrren==settings_PN_scn)||(CurScrren==settings_Kol_BKL_scn)||         //��,���.���/10_minut
//                                 (CurScrren==aux_settings_tzvpn_scn)|| /*(CurScrren==settings_tzvpn_scn)|| *///Tvkl_dv,Tzv_pn,
//                                 (CurScrren==settings_Pmin_d_scn)||(CurScrren==settings_Pmax_d_scn)||        //Pmin_d,Pmax_d
//                                 (CurScrren==settings_TVn_Sx_scn)||(CurScrren==settings_risol_scn)||         //TVn_Sx,RizolED
//                                 (CurScrren==settings_Rdu_scn) || (CurScrren==settings_idif_scn)||            //R��,I���
//                                 (CurScrren==settings_KLB_I1_scn)||(CurScrren==settings_KLB_I2_scn)||        //KLB_I1,KLB_I2
//                                 (CurScrren==settings_KLB_I3_scn)||(CurScrren==settings_KLB_Id_scn)||         //KLB_I3,KLB_Id
//                                 (CurScrren==settings_KLB_Uc_scn)||(CurScrren==settings_KLB_AD_scn)||      //KLB_Uc,KLB_AD
//                                 (CurScrren==settings_add_scn)||(CurScrren==Id_syst_error)                  // AD_C�,Idcr
//                                )
//                              {up_down=1;EditFuncUP1(CurScrren);} //WD 04 04 2017
//                             else EditFuncUP(CurScrren);
//                           }
//                      ScreenToLCD (CurScrren,values);
//                       break;
//             case down: // �������� ������ �� ���� ������ ������
//             case longdown:
//                     if (CurScrren==aux_setRTC_scn)EditRTCFuncShiftRhit();
//                        //��������� �������� ��������� ����� �� ������ �� 1 ����
//                      else {if ((CurScrren==aux_In_shk_scn)||(CurScrren==settings_Pn_shk_scn)||            //I�, P�
//                                 (CurScrren==settings_imax_scn) || (CurScrren==aux_Imax_shk_scn)||         //Pmin
//                                 (CurScrren==settings_pmin_scn) || (CurScrren==settings_Levl_off_scn)||    // AD_��, AD_H�
//                                 (CurScrren==aux_sett_K_Izol)||(CurScrren==settings_ton_scn)||             //K_RizolED, �����po
//                                 (CurScrren==settings_PN_scn)||(CurScrren==settings_Kol_BKL_scn)||         //��,���.���/10_minut
//                                 (CurScrren==aux_settings_tzvpn_scn)|| /*(CurScrren==settings_tzvpn_scn)||*/ //Tvkl_dv,tzvpn,
//                                 (CurScrren==settings_Pmin_d_scn)||(CurScrren==settings_Pmax_d_scn)||      //Pmin_d,Pmax_d
//                                 (CurScrren==settings_TVn_Sx_scn)||(CurScrren==settings_risol_scn)||       //TVn_Sx,RizolED
//                                 (CurScrren==settings_Rdu_scn) || (CurScrren==settings_idif_scn)||         //R��,I���
//                                 (CurScrren==settings_KLB_I1_scn)||(CurScrren==settings_KLB_I2_scn)||      //KLB_I1,KLB_I2
//                                 (CurScrren==settings_KLB_I3_scn)||(CurScrren==settings_KLB_Id_scn)||      //KLB_I3,KLB_Id
//                                 (CurScrren==settings_KLB_Uc_scn) ||(CurScrren==settings_KLB_AD_scn)||      //KLB_Uc,KLB_AD
//                                 (CurScrren==settings_add_scn)|| (CurScrren==Id_syst_error)                 // AD_C�,Idcr
//                                )
//                              {up_down=2;EditFuncUP1(CurScrren);} //WD 04 04 2017
//                             else EditFuncShiftRhit(CurScrren);
//                           }
//                       break;
//                case esc: //����� �� ������ �������������� ��� ������
//                      EditValueProp&=~0x01;
//                       //���� ��� ����� ����� ������, �� ������������ � ������������� �����
//                      if((CurScrren==userpsw_scn)||(CurScrren==Acces_set_No_Psw))
//                      { CurScrren=EditedScrren;
//                        LoadEditValue (CurScrren);
//                        ScreenToLCD (CurScrren,all);
//                      }
//                      else
//                      { LoadEditValue (CurScrren);
//                        ScreenToLCD (CurScrren,values);
//                      }
//                      break;
//                case enter:    // ������ � ����� �� ������ ��������������
//                      EditValueProp&=~0x01;
//                      EditValue=EditFuncAssembly(CurScrren);
//                      if((up_down==1)||(up_down==2))
//                          {EditValue= EditValue_1; //WD 04 04 2017
//                            up_down=0;
//                            Sett_min=Sett_max=0;
//                          }
//                      LoadParamFromEditValue(CurScrren);
//                       //���������� ������ � �������=55,   05 12 2017 ���.
//                      if((CurScrren==Acces_set_No_Psw)&&(Settings_TK_fl [68]==1))
//                       {psw_setp=Settings_TK_fl [64];}
//
//                       //���������� ������ � �������..���.
//                      //if (CurScrren==aux_userpswchange_scn)
//                      //{EditValue=0;                         //WD  �������� ==0 -> =0 18 10 2015
//                      // ScreenToLCD(CurScrren,all);
//                      //}
//                      if (CurScrren==userpsw_scn)  //���� ����� ����� ������
//                      {CurScrren=EditedScrren;          //������������ � �������������� ������
//                       ScreenToLCD(CurScrren,all);      //�������������� ������������� ������
//                       if (UserPswEnable&1) goto EditPrepear;    //���� ������ ����� ���������, �� ������������� ��������������
//                      }
//                      else ScreenToLCD(CurScrren,values);
//                      Sett_Reset_regim_key(0); //C������� ����� ��������� ������ ��� ��������.�������
//                      break;
//                }
//          }
//          else
//          {//����� �����������
//            switch (KeyCommand)
//              { case up:
//                         CurScrren=ScreenUp (CurScrren);
//                         LoadEditValue(CurScrren);
//                         ScreenToLCD (CurScrren,all);
//                         break;
//                case esc:
//                         CurScrren=ScreenEsc(CurScrren); ScreenToLCD(CurScrren,all);
//                         break;
//                case down:
//                         CurScrren=ScreenDown(CurScrren); ScreenToLCD(CurScrren,all);
//                         break;
//                case enter:  //���������� � ��������������
//                        //����� ������ ��� ������ case Acces_set_No_Psw, 04 12 2017 ���.
//                        tmp_s=0;
//                        //if (CurScrren==Acces_set_No_Psw)
//                        // {/*psw_setp=Settings_TK_fl [64];*/ goto EditPrepear;}
//                        if ((CurScrren==aux_sett_Cnt_mod_scn)||(CurScrren==settings_RJ_P_O_scn)|| //
//                            (CurScrren==settings_BKL_P_3_scn)||(CurScrren==settings_Tup_Dt_scn)|| //
//                            (CurScrren==aux_In_shk_scn)||(CurScrren==settings_Pn_shk_scn)  ||     //
//                            (CurScrren==settings_imax_scn)||(CurScrren==aux_Imax_shk_scn)  ||     //
//                            (CurScrren==settings_pmin_scn)||(CurScrren==settings_Levl_off_scn) || //
//                            (CurScrren==aux_setRTC_scn) ||(CurScrren==settings_add_scn)
//                            )
//                           {tmp_s=1;
//                            if(Settings_TK_fl [68]==1)goto EditPrepear;}
//
//                        if (!(UsPswEnable&0x01)) //���� ������ �����. �� ����� ��������� �� ����� ����� ������
//                           if (CurScrren==Acces_set_No_Psw)
//                                 {psw_setp_us=Settings_TK_fl[64]; //WD psw 09 05 2017
//                                   EditedScrren=CurScrren;//CurScrren=Acces_set_No_Psw;
//                                      ScreenToLCD (CurScrren,all);
//                                 }
//
//                        //���� ������ R1 ��� ��������� �������� �����, �� ��������
//                        //������������� ����� ����� ������ �� ������ R2
//                        if ((UserPswEnable&0x01)&&(psw_setp_wrk1!=0))
//                          {psw_setp=psw_set_curr(CurScrren);
//                           if(psw_setp!= psw_setp_wrk1)
//                            {//����� �������� ������ R1, �.�. ������������� ���������
//                             //����������� � ������� �������� � ������� R2
//                             psw_setp=psw_setp_wrk1=UserPswEnable=0;
//                            }
//                          }
//                        //���� ������ R2 ��� ��������� �������� �����, �� ��������
//                        //������������� ����� ����� ������ �� ������ R1
//                        if ((UserPswEnable&0x01)&&(psw_setp_wrk2!=0))
//                          {psw_setp=psw_set_curr(CurScrren);
//                           if(psw_setp!= psw_setp_wrk2)
//                            {//����� �������� ������ R2, �.�. ������������� ���������
//                             //����������� � ������� �������� � ������� R1
//                             psw_setp=psw_setp_wrk2=UserPswEnable=0;
//                            }
//                          }
//
//                        if (!(UserPswEnable&0x01)&&(CurScrren!=Acces_set_No_Psw)) //���� ������ �� ����� ��������� �� ����� ����� ������
//                           if (CurScrren!=userpsw_scn)
//                                 { psw_setp=psw_set_curr(CurScrren); //WD psw 09 05 2017
//                                   EditedScrren=CurScrren;CurScrren=userpsw_scn;
//                                   if(tmp_s==1)CurScrren= No_set_Psw; //������� ������ 18 12 2017
//                                      ScreenToLCD (CurScrren,all);
//                                 }
//
//                   EditPrepear:
//                        EditValueProp|=0x01;                //���������� ������� - ��������������
//                        LoadEditValue(CurScrren);           //��������� �������� ������� ��� ��������������
//                        EditValueToScreen (CurScrren) ;     //����������� ���������� ��� ��������� � ��������������
//                        ScreenToLCD (CurScrren,values);     //������� � ������������� ������� ������
//                        Sett_Reset_regim_key(1);   //��������� ���. ������ up, down  24 10 2018
//                        break;
//                          //��������� ������� nic 16 12 2015  ���.
//                 default: if(Sync) //���������� ������� �������� �� ������� ��������, ��� ��� ����������
//                         {
//                           if (CurScrren==aux_setRTC_scn) CalendarScreen (values);
//                           else if (CurScrren==settings_KLB_I1_scn) Setup_Bas_KLB_I1_scn(values); // �29
//                           else if (CurScrren==settings_KLB_I2_scn) Setup_Bas_KLB_I2_scn(values); // �30
//                           else if (CurScrren==settings_KLB_I3_scn) Setup_Bas_KLB_I3_scn(values); // �31
//                           else if (CurScrren==settings_KLB_Id_scn) Setup_Bas_KLB_Id_scn(values); // �44
//                           else if (CurScrren==settings_KLB_Uc_scn) Setup_Bas_KLB_Uc_scn(values); // �45
//                           else if (CurScrren==aux_sett_K_Izol) Setup_Add_K_Izol(values); // �42
//                           else if (CurScrren==settings_phas_scn) Cntr_phas_scn(values); // �51
//                           else if (CurScrren== N_Brows_s) N_Brows_scn(values);            // �58
//                           else if (CurScrren== settings_KLB_AD_scn) Set_Bs_klb_AD_scn(values); // �59
//                           else if (CurScrren==Id_syst_error) Stp_Id_syst_error_scn(values); // �72
//                          }
//              }
//          }
//         break;
//  }
//}
//                          //��������� ������� nic 16 12 2015  ���.
//void Scr_bas_KZ(char KZn)
//{ if(KZn!=0)
//   { writeLCDc(0x80+14);printUnsigLong(KZn,2);
//     writeLCDc(0xC0);WriteLCDstring("oFF");
//     Param_nz[15]=1;
//   }
//  /*
//  if(KZn==1)
//       { writeLCDc(0x80);WriteLCDstring("����� ���� ��=");printUnsigLong(KZn,2);   //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("�� I1,I2 � I3  >");                       //C����� 2
//       }
//   if(KZn==2)
//       { writeLCDc(0x80);WriteLCDstring("���������� ��=");printUnsigLong(KZn,2);  //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("���� I1,I2 � I3>");                       //C����� 2
//       }
//   if(KZn==21)
//       {writeLCDc(0x80);;WriteLCDstring("���������  ��=");printUnsigLong(KZ,2);   //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("��� �����      >");                     //C����� 2
//       }
//   if(KZn==22)
//       {writeLCDc(0x80);;WriteLCDstring("���������  ��=");printUnsigLong(KZ,2);   //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("��� ������     >");                     //C����� 2
//       }
//   if(KZn==3)
//       {writeLCDc(0x80);;WriteLCDstring("���������� ��=");printUnsigLong(KZ,2);   //C����� 1
//              writeLCDc(0xC0);WriteLCDstring("P=");                              //C����� 2
//         printSignLongPiont_I2C(Param_nz[59],5,1); WriteLCDstring(" ������ >");
//       }
//  if(KZn==4)
//       {writeLCDc(0x80);;WriteLCDstring("����� �������� >");                            //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("R��="); printSignLongPiont_I2C(Param_nz[RizED],6,3); //C����� 2
//                      WriteLCDstring(" ��=");printUnsigLong(KZ,2);WriteLCDstring(">");
//       }
//   if(KZn==5)
//       {writeLCDc(0x80);;WriteLCDstring("��� ������ ��=");printUnsigLong(KZ,2);          //C����� 1
//         writeLCDc(0xC0);WriteLCDstring("I���� �������  >");                             //C����� 2
//       }
//   if(KZn==6)
//       {writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK5);                                  //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK5[0]); printUnsigLong(KZ,2);         //C����� 2
//                      WriteLCDstring(Str2PrmaIBK5[1]);
//       }
//   if(KZn==7)
//       { writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK6);                               //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK6[0]); printUnsigLong(KZ,2);      //C����� 2
//                      WriteLCDstring(Str2PrmaIBK6[1]);
//       }
//   if(KZn==8)
//       { writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK7);                                   //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK7[0]); printUnsigLong(KZ,2);         //C����� 2
//                      WriteLCDstring(Str2PrmaIBK7[1]);
//       }
//   if(KZn==81) //��==81
//       { writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK71);                              //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK71[0]);printUnsigLong(KZ,2);       //C����� 2
//                      WriteLCDstring(Str2PrmaIBK71[1]);
//       }
//   if(KZn==82) //��==82
//       { writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK72);                             //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK72[0]);printUnsigLong(KZ,2);      //C����� 2
//                      WriteLCDstring(Str2PrmaIBK72[1]);
//       }
//
//   if(KZn==9)
//       { writeLCDc(0x80);;WriteLCDstring(Str1PrmaIBK8);                              //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2PrmaIBK8[0]); printUnsigLong(KZ,2);      //C����� 2
//                      WriteLCDstring(Str2PrmaIBK6[1]);
//       }
//   if(KZn==10) //��==10
//       { writeLCDc(0x80);;WriteLCDstring(Str1_AL_10);                                 //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2_AL_10[0]);printUnsigLong(KZ,2);           //C����� 2
//                         WriteLCDstring(Str2_AL_10[1]);
//       }
//   if(KZn==11) //��==11
//       { writeLCDc(0x80);;WriteLCDstring(Str1_AL_11);                                 //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2_AL_11[0]);printUnsigLong(KZ,2);          //C����� 2
//                         WriteLCDstring(Str2_AL_11[1]);
//       }
//      if(KZn==12) //��==12
//       { writeLCDc(0x80);;WriteLCDstring(Str1_AL_12);                                  //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2_AL_12[0]);printUnsigLong(KZ,2);           //C����� 2
//                         WriteLCDstring(Str2_AL_12[1]);
//       }
//      if(KZn==13) //��==13
//       { writeLCDc(0x80);;WriteLCDstring(Str1_AL_13);                            //C����� 1
//         writeLCDc(0xC0);WriteLCDstring(Str2_AL_13[0]);printUnsigLong(Soob,1);   //C����� 2
//             WriteLCDstring(Str2_AL_13[1]);printUnsigLong(KZ,2);
//             WriteLCDstring(Str2_AL_13[2]);
//       }
//   */
//  }
//
//  //�� ������� ����������� �� min ��� max � ������������ �� Flash ���������� EditValue,
//  //���� � ��� ������� ��������, ������� �� ��������� � �������� ���������
//  //������� ��������� �������� N_Sett (� �������� �������, � ������� �������� ����������� �� �������)
//  //�� �������� � �������� ��� ������ ���������
//  //������� ���������� �������� ���������� EditValue(��������������� �������� �������);
//  //�������� ���������� �������� ���������� EditValue(��������������� �������� ������� � ������ ����������� �� ���������);
// void Limit_Setting(char N_Sett) //�� ����������� �������
//  {if(EditValue<=Setting[N_Sett]) EditValue=Setting[N_Sett];
//   if(EditValue>Setting[N_Sett+1]) EditValue=Setting[N_Sett+1];
//    ;
//   }
//
////�� ����������� �������, ��������������� ��� �� Limit_Set_clp
// uint16_t Limit_1(uint16_t Edit,char N_Sett) //�� ����������� �������
//  { uint16_t limit_t;
//    if(Edit<=Setting[N_Sett]) limit_t=Setting[N_Sett];
//   if(Edit>Setting[N_Sett+1]) limit_t=Setting[N_Sett+1];
//   return limit_t ;
//   }
//
// // 02 01 2018
////�� ���������������� ����������� �� min ��� max �����-���� �������������� �������, �������� � ������� �������
//  // � ������������ �� Flash ���������� EditValue.��� �������� ����� �������������� ���������.
//  //������� ���������� �������� ���������� EditValue(��������������� �������� �������);
//  //�������� ���������� �������� ���������� EditValue(��������������� �������� �������
//  //� ������ ����������� �� ���������);
// //�������� ������� (collapsible-���� clps)  ������� ��������  ������� �� ������� �� 412
//void Limit_Set_clp(char N_Setclp)
//  { uint16_t TK_1;
//    char Y_y1;
//    char X_x1;
//    char DfZ;   //����� �,����������  Diff_Z
//    char ZVT; //����� �,������  ��,��
//    char Tt;   //����� �, ������� �������
//    char Hkl;     //����� �,�����. �������
//     ///////
//    char BB;     //����� B,�������/�������
//    char Kod;    //����� K,���������� �� ������.�����.
//    char Adr;    //����� ���,����� �������
//     ///////
//    char M;      //����� �,������
//    char F;      //����� F,�����������
//    char J;      //����� J,�����
//    char P,S,W,D,E,V; //������
//
//   if(N_Setclp==1)
//    { //��, �������������� ������� � Flash (Settings_TK_fl[63]) ��� ������� �� 412,
//      //������� ��������������� � EditValue,
//     //��� ����������� ������� c ������������ ��412YX����:
//     // ��-������������� �������, Y- ����� �������, X- ���������� �������,
//     //�-������,�-������ ��,��; � -���. �������,
//           //1. ���������� ���������� ������� ������� -���
//     TK_1= EditValue/1000000;   //��� �� �������
//      //������ �� ���� ������� ���
//     if(TK_1<4)TK_1=4;
//     if((TK_1>4)&&(TK_1<412))TK_1=4;
//     if(TK_1>412)TK_1=412;                                            //��� �������
//      //������ �� ���� ������� ���
//     Y_y1=((EditValue/100000)-(TK_1*10));                             //� �������
//     X_x1=((EditValue/10000)-(TK_1*100)-(Y_y1*10));                   //����������
//     DfZ=((EditValue/1000)-(TK_1*1000)-(Y_y1*100)-(X_x1*10));   //����������
//     ZVT=((EditValue/100)-(TK_1*10000)-(Y_y1*1000)-(X_x1*100)-(DfZ*10));//������ ��,��
//     Tt=((EditValue/10)-(TK_1*100000)-(Y_y1*10000)-(X_x1*1000)-(DfZ*100)-(ZVT*10));                      //������� �������
//     Hkl=(EditValue-(TK_1*1000000)-(Y_y1*100000)-(X_x1*10000)-(DfZ*1000)-(ZVT*100)-(Tt*10));       //�����. �������
//      //���������� ���������� ... -���
//
//      //2. ��������� ��������� ����������� ���������� ������� ������� -���
//     TK_1=Limit_1(TK_1,18);
//     Y_y1=Limit_1(Y_y1,20);
//     X_x1=Limit_1(X_x1,22);
//     DfZ=Limit_1(DfZ,26);
//     ZVT=Limit_1(ZVT,26);
//     Tt=Limit_1(Tt,26);
//     Hkl=Limit_1(Hkl,24);
//   //2. ��������� ��������� ����������� ... -���
//
//      //3. ������������ ���������� �� ��������� ������� �������-���
//    // EditValue= CA0_1 + (D_1*100) + (N_1*1000) + (X_x1*10000) +(Y_y1*100000) + (TK_1*1000000);
//     EditValue= Hkl + (Tt*10) + (ZVT*100) + (DfZ*1000)+
//       (X_x1*10000) +(Y_y1*100000) + (TK_1*1000000);
//     //3. ������������ ���������� �� ...-���
//    }
//
//    if(N_Setclp==2)
//     { //��, �������������� ������� � Flash (Settings_TK_fl[97]) ��� ������� �� 412,
//      //������� ��������������� � EditValue,
//     //��� ����������� ������� c ������������ �����:
//     // ��-������������� �������, Y- ����� �������, X- ���������� �������,
//     //B-�������/�������, �-���. ������������ ����������; ���-����� ����������� ��
//           //1. ���������� ���������� ������� ������� -���
//       BB=EditValue/10000;                           //�������/�������
//       Kod= ((EditValue/1000) -(BB*10));            //���������� ��
//       Adr= (EditValue -(BB*10000)-(Kod*1000));  //�����
//      //��������� ��������� ����������� ���������� ������� ������� -���
//       BB= Limit_1(BB,26);
//       Kod= Limit_1(Kod,24);
//       Adr= Limit_1(Adr,28);
//        //3. ������������ ���������� �� ��������� ������� �������-���
//       EditValue= (Adr + (Kod*1000) + (BB*10000));
//     }
//    if(N_Setclp==3)
//     {//��, �������������� ������� � Flash (Settings_TK_fl[98]) ��� ������� �� 412,
//      //������� ��������������� � EditValue,
//     //��� ����������� ������� c ������������ MFJPSWDEV:
//       // �-������ ��, F- ����������� �� Flash ��� � BKP, J-�����,��������� ������
//      M=EditValue/100000000;                           //������ ��
//      F=((EditValue/10000000) -(M*10));            //�����������
//      J=((EditValue/1000000) -(M*100)-(F*10));     //�����
//      P=((EditValue/100000) -(M*1000)-(F*100)-(J*10));
//      S=((EditValue/10000) -(M*10000)-(F*1000)-(J*100)-(P*10));
//      W=((EditValue/1000) -(M*100000)-(F*10000)-(J*1000)-(P*100)-(S*10));
//      D=((EditValue/100) -(M*1000000)-(F*100000)-(J*10000)-(P*1000)-(S*100)-(W*10));
//      E=((EditValue/10) -(M*10000000)-(F*1000000)-(J*100000)-(P*10000)-(S*1000)-(W*100)-(D*10));
//      V=(EditValue -(M*100000000)-(F*10000000)-(J*1000000)-(P*100000)-(S*10000)-(W*1000)-(D*100)-(E*10));
//      //��������� ��������� ����������� ���������� ������� ������� -���
//       M= Limit_1(M,80);
//       F= Limit_1(F,26);
//       J= Limit_1(J,26);
//       P= Limit_1(P,26);
//       S= Limit_1(S,26);
//       W= Limit_1(W,26);
//       D= Limit_1(D,26);
//       E= Limit_1(E,26);
//       V= Limit_1(V,26);
//        //3. ������������ ���������� �� ��������� ������� �������-��� EditValue
//       EditValue=(V +(E*10)+(D*100)+(W*1000)+(S*10000)+(P*100000)+
//                  (J*1000000)+(F*10000000)+(M*100000000));
//     }
//   }
//
//  //�� ������� ����������� �� max � ������������ �� Flash ���������� EditValue,
//  //���� � ��� ������� ��������, ������� �� ���� �������� �������
//  //�������� ���������� ���������:
//  //   N_Sett1- ����� �������� ������� Flash, � ������� ������� ��� �������,
//  //         ���������� ��� ��, ���������� �������(�� Y=0..8) �  //���������� ������� (�� X=0..8).
//  //   N_Sett2- ����� 1-�� �������� ������ ��������� ������� Flash, � ������� ��������
//  //          �������  ����������� �� I� �� ����������� �������, ������������, ���� ��� ������� ��412
//  //   N_Sett3- ����� 1-�� �������� ������ ��������� ������� Flash, � ������� ��������
//  //          �������  ����������� �� I� �� ����������� ��������, ������������, ���� ��� ������� ��4
//  //������� ���������� �������� ���������� EditValue(��������������� �������� �������);
//  //�������� ���������� �������� ���������� EditValue(��������������� �������� ������� � ������ �����������
//
// void Limit_Sett_TK4_412(char N_Sett1,char N_Sett2,char N_Sett3) //�� ����������� ������� �� ���� I�
//  { uint16_t TK_1;
//    char X_x1;
//    char Y_y1;
//    char tmp_pl;
//
//    tmp_pl= Setting[N_Sett1];
//     TK_1= Settings_TK_fl[tmp_pl]/1000000;   //��� �� �������
//     Y_y1=((Settings_TK_fl[tmp_pl]/100000)-(TK_1*10));   //� �������
//     X_x1=((Settings_TK_fl[tmp_pl]/10000)-(TK_1*100)-(Y_y1*10));   //����������
//
//    if(TK_1==412)   //������ � ������������ �� ���� �� � ������� Y
//     { tmp_pl=Setting[N_Sett2];
//       if(EditValue>Settings_TK_fl[tmp_pl+Y_y1]) EditValue=Settings_TK_fl[tmp_pl+Y_y1];
//     }
//
//    if(TK_1==4)   //������ � ������������ �� ���� �� � ������� X
//     { tmp_pl=Setting[N_Sett3];
//       if(EditValue>Settings_TK_fl[tmp_pl+((X_x1-1)*5)]) EditValue=Settings_TK_fl[tmp_pl+((X_x1-1)*5)];
//     }
//  }
//  //�� ������� ������ max ��� min ��� ������������� ������� ��� I� � ���������� EditValue_1,
//  //���� ��� �������������� ������� �� �������� ��������� ��������� �������
//  //�������� ���������� ���������:
//  //   N_Sett1- ����� �������� ������� Flash, � ������� ������� ��� �������,
//  //         ���������� ��� ��, ���������� �������(�� Y=0..8)
//  //   N_Sett2- ����� 1-�� �������� ������ ��������� ������� Flash, � ������� �������
//  //          ������ max ������� ��� I�
//  //   N_Sett3- ����� 1-�� �������� ������ ��������� ������� Flash, � ������� �������
//  //          ������ min ������� ������� ��� I�
//
//  //������� b���������� �������� ���������� EditValue_1(��������������� �������� �������);
//  //�������� ���������� �������� ���������� EditValue_1(��������������� �������� ������� � ������ �����������
//
// void Min_Max_Sett_TK412(char N_Sett1,char N_Sett2,char N_Sett3) //�� ����������� ������� �� ���� I�  07 04 2017
//  { uint16_t TK_1;
//    char Y_y1;
//    char tmp_pl;
//
//    tmp_pl= Setting[N_Sett1];
//     TK_1= Settings_TK_fl[tmp_pl]/1000000;   //��� �� �������
//     Y_y1=((Settings_TK_fl[tmp_pl]/100000)-(TK_1*10));   //� �������
//    if(TK_1==412)   //������ � ������������ �� ���� �� � ������� Y
//     { //�������� ���������� �������� ������� max ��� �������
//       tmp_pl=Setting[N_Sett2];
//       if(EditValue>Settings_TK_fl[tmp_pl+Y_y1]) EditValue_1=Settings_TK_fl[tmp_pl+Y_y1];
//        //�������� ���������� ������� ������� max ��� �������
//       tmp_pl=Setting[N_Sett3];
//       if(EditValue<Settings_TK_fl[tmp_pl+Y_y1]) EditValue_1=Settings_TK_fl[tmp_pl+Y_y1];
//     }
//  }
//
// //�� ������ ����������� ������� �� ���� I� ��� ������� TK412 07 05 2017
// //N_Sett1,N_Sett2 � N_Sett3 ��. ����, � ���� N_Sett4=0, �� ����� ������ N_Sett2, � N_Sett3=0
//void Min_Max_Sett_TK412_d(char N_Sett1,char N_Sett2,char N_Sett3,char N_Sett4) //�� ����������� ������� �� ���� I�  07 04 2017
//  { uint16_t TK_1;
//    char Y_y1;
//    char tmp_pl;
//
//    tmp_pl= Setting[N_Sett1];
//     TK_1= Settings_TK_fl[tmp_pl]/1000000;   //��� �� �������
//     Y_y1=((Settings_TK_fl[tmp_pl]/100000)-(TK_1*10));   //� �������
//    if(TK_1==412)   //������ � ������������ �� ���� �� � ������� Y
//     { //�������� ���������� �������� ������� max ��� �������
//       Sett_max=Settings_TK_fl[N_Sett2+Y_y1];
//        //�������� ���������� ������� ������� max ��� �������
//       if(N_Sett4)
//        { tmp_pl=Setting[N_Sett3];
//         Sett_min=Settings_TK_fl[tmp_pl+Y_y1];
//        }
//       else Sett_min=0;
//     }
//  }
//
//  //�� � Flash ������� min ����������� ������� � ���������� Sett_min
//  // � max ����������� ������� � ���������� Sett_max
//  //������� ���������:
//  // N_Sett1 (� �������� ������� Setting[],������� �������� �������
//  //          Flash, � ������� �������� min ����������� �������;
//  // N_Sett2 (� �������� ������� Setting[],������� �������� �������
//  //          Flash, � ������� �������� max ����������� �������;
//  //�������� ���������:
// // Sett_min - min ����������� �������
// // Sett_m�� - min ����������� �������
// void Min_Max_Sett(char N_Sett1,char N_Sett2) //�� ���������� ����������� �������
//  { char tmp_mn;
//     //� Sett_min �������� � Flash min ����������� �������
//    tmp_mn=Setting[N_Sett1];
//    Sett_min=Settings_TK_fl[tmp_mn];
//     //� Sett_max �������� � Flash max ����������� �������
//    tmp_mn=Setting[N_Sett2];
//    Sett_max=Settings_TK_fl[tmp_mn];
//   }
//
// // ������ �������������� ��� ������ up � down
// void Sett_Reset_regim_key(char st_rs) //�� ���������� ����������� �������
//  { //���������� ������ �������������� ��� ������ up � down
//    if(st_rs==1)
//     { up_d=1; down_d=1;}
//    //�������� ������ �������������� ��� ������ up � down
//    if(st_rs==0)
//     { up_d=0; down_d=0;
//       longdown_d=longup_d=up_down=0;}
//  }
//
//
//#endif /* TKPROJECT_ZKCOMENT_H_ */