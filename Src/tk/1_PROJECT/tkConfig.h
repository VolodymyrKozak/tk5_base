/*
 * tkConfig.h
 *
 *  Created on: 20 бер. 2018 р.
 *      Author: KozakVF
 */

#ifndef TKCONF_H_
#define TKCONF_H_
#include "stdint.h"

/*****************************************************************************
 * КОНФІГУРАЦІЯ БАЗОВОГО ПРОЕКТУ
 *****************************************************************************
 * 1. ЗАГАЛЬНІ ПОЛОЖЕННЯ
 * 1.1. В проекті, зформованому CubeMX здійснюються мінімальні правки.
 * в файлі main.c викликаються лише функції
 * 	tkInit() для виклику функцій ініціалії усіх процесів проекту,
 * 	tkLoop() для виклику функції в основному циклі.
 * 1.2.Усі робочі файли .h .c розміщені у папках:
 *  tk/1_PROJECT
 *  tk/2_SubProgects
 *  tk/3_Libraries
 * 1.3.У папці 3_PROJECT розміщені:
 * 	цей файл конфігурації проекту tConfig.h,
 * 	файли tkMain.c та .h,  де здійснюються:
 * 		диспетчеризація усіх процесів проекту, робота вочдогу
 * 	файли tkErr_Handler.c та .h	обробка прогамних помилок з записом інформації
 * 	про програмні помилкb у флеш-память.
 * 1.4.	У папці 3_Libraries розміщені файли .c та .h бібліотечних модулів, тобто функцій,
 * які призначені для використанні в самих різних проектах, наприклад - драйвери пристроїв та ін.
 * 1.5  У папці 2_SubProjects містяться складові частини базового проекту, або складові частини
 * конкретного проекту, зокрема і функції тестування модулів бібліотеки.
 * 1.6. Конкретна конфігурація при відлагодженні базового проекту і робочих проектів
 * здійснюється дефайнами у цьому файлі.
 * 1.7. У конкретних проектах ті складові частини базового проекту і бібліотек,
 * які не використовуються можуть видалятися або блокуватися з використання #ifdef
 *
*/
#define ZERO_INIT_Cplt 0x1254

//#define ENABLE_I2C
/*****************************************************************************************
 *  2. ДИСПЛЕЙ
 *  Установити тип дисплея, або або:*/
#ifdef ENABLE_I2C
#define WINSTAR_Display_WH1602B3
//#define WINSTAR_Display_WO1602G
#endif //ENABLE_I2C
/* Кнопки АЦП різні, вибрати варіант - кнопки як на роботі, або кнопки як дома.. */
#define OFFICEBUTTONS
//#define HOMEBUTTONS
/*****************************************************************************************
 *  3. МОДБАС. Конфігурація та використання
 *********************************************************************
 * Установити конігурацію Modbus для проекту . Можливі варіанти:
1) Лише один Master
2) Лише один Slave
3) Master і	 Slave
4) Master і  Master
УВАГА: Slave і Slave не підтримується
УВАГА: Один і той же UART/RS485 в одному і тому ж проекті
не може бути і MASTER, i SLAVE */

//#define MASTERRTU_HUART_4
//#define MASTERRTU_HUART_5

#define SLAVERTU_HUART_4
//#define SLAVERTU_HUART_5
#if defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
/*Якщо в проекті реалізований MODBUS RTU SLAVE, необхідно:
  1) призначити унікальну в мережі RTU адресу Modbus 01..125 */
#define SLAVE_NODE_ADDRESS      0x01
/*2)описати логіку прикладyого алгоритму обробки
 * циркулярних (функція MB_Slave_Selective_Handler())
 *  і вибіркових функція (MB_Slave_Broadcast_Handler())
 *  повідомлень, файл tkSubProjects/pr_MBRTU_SLAVE.c,
 *  використовувати класичну (файл tkModbus/mb_slave5.c як зразок */
/* очевидно може бути вибрана лише одна з альтернатив   */
//#define MODBUS_APPLICATION_SLAVE_CLASSIC
//#define MODBUS_APPLICATION_SLAVE_asTK2
#endif // defined (SLAVERTU_HUART_4) || defined (SLAVERTU_HUART_5)
/*******************************************************************/
//#define MASTERRTU_HUART_4
//#define MASTERRTU_HUART_5
#if defined (MASTERRTU_HUART_4) || defined (MASTERRTU_HUART_5)
/******************************************************************************
 * Розкоментувати дефайн, якщо TK4 управляє індикаторами ОВЕН СМИ2 Индикатор RS-485
 * При цьому обов'язково повинен бути розкоментований відповідний MASTERRTU_HUART
 * у розділі нижче */
//#define MODBUS_MASTER_for_OVEN_INDICATOR
/*закоментувати у робочому проекті, коли тестування вже не потрібне */
//#define MODBUS_MASTER_for_OVEN_INDICATOR_TESTING
//#define MODBUS_MASTER_for_OVEN_ARCHIVE_MCD200

/* Розкоментувати,якщо здыйснюэться передача модбас-повідомлень для двох ТК2*/
//#define DRIVES_REMOTE_CONTROL
#endif //defined (MASTERRTU_HUART_4) || defined (MASTERRTU_HUART_5)
/****************************************************************************************
 * ДАЛІ ЧОРНОВИКИ ПОКИ ЩО
 */

//#define THREE_PHASE_MOTOR_PROTECTION
//#define ONE_PHASE_MOTOR_PROTECTION
//#define THREExONE_PHASE_MOTOR_PROTECTION
//#define BOARD_TK4_TESTING
#define STM32_INTERNAL_CLOCK
#define DEBUGGING_ONLY
#define IT_WAS_ATTEMPT_ONLY


/* Заготовка для управління двома насосами */
//#ifdef TK2_REMOTE_CTR







/* ***************************************************************************************
 * Вимірювання параметрів мережі трьохфазного струму з використанням перетворення Фурьє  *
 * ***************************************************************************************
 * На основі вімірювань відліків щомікросекунди, тобто 20 відліків АЦП за період
 * напруги,трьох фазних струмів, та струму витоку (при наявності у схемі диф. трансформатора
 * розраховуються параметри електромережі:
 * 	діюча напруга в одній з фаз,
 * 	частота,
 * 	діючі струми у кожній з фаз,
 *  струм витоку (розрахунковий)
 *  коэффициент несинусоидальности тока(Total Harmonic Current Distortion)
 *  КОЕФІЦІЄНТИ:
 *  	косинус фі (power factor), коефіцієнт потужності електродвигуна
 *  	коефіцієнт потужності електробвигуна викликаний гармоніками
 *  	коефіцієнт потужності електродвигуна викликаний асиметрією струму
 *
 * */
/* Робота з вимірюванням і обчисленням параметрів електромережі*/
#define GRD_FURIE /**/
#ifdef GRD_FURIE
/* Тестування властивостей перетворення Фурьє для відлагодження 		*/
//#define FURIE_TEST
/* Моделювання струмів і дистанційна зміна ВХІДНИХ параметрів*/
#define GRID_DISTANCE_DEBUG
/* Кількість відліків за період струму/напруги для обчислень Фур'є      */
#define N_FURIE (uint16_t)18
#define N_FURIEBUTTON  N_FURIE/5 + 1
#define FURIE_LIB
#endif //GRD_FURIE

/* Відлагодження */
#ifdef DEBUGGING_ONLY
//#define FREQUENCY_CALC_DEBUG0
//#define FREQUENCY_CALC_PRINT0

/* Вивід на екран незглажених папуг струму фази С
 * Фурьє VC 200 відліків */
//#define TIME_CNTR_DEBUG
//#define AMPERAGE_FURIE_VS_EUGENYT0
//#define COS_FI_CALC_PRINT0
//#define AMPERAGE_UNBALANCE_PRINT0
#endif


//#define FLASH_TO_RESTART /* використати якщо флеш tk4 може бути забруднена іншими програмними системати*/



#define adcf_t float
#define adcu16_t uint16_t
#define amperf_t float
#define hzf_t  float
#define voltf_t  float
#define vatt_t float
#define kvatt_t float
#define gradf_t  float
#define procentf_t float


#define DISTANE_DEBUG

#endif //TKLIBRARIES_TKCONF_H_
