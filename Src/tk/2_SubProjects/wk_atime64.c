///*
// * wk_time64.c
// *
// *  Created on: 5 ����. 2018 �.
// *      Author: Volodymyr.Kozak
// */
//
//
#include "../2_SubProjects/wk_atime64.h"

#include "rtc.h"

#include "../1_PROJECT/tkConfig.h"
#include "../1_PROJECT/tkErr_Handler.h"
#include "../3_Libraries/time64/stm32_rtc.h"
#include "../3_Libraries/time64/time64.h"


/*****************************************************************************************
It is elapsed modernized with GNU-time, with time64.h library,
without 2038 year problem.
Any transition btw files and functions
should be done with Time64_T form of time only
It's at the moment time, updatable with MCU every loop.
It's user duty to set real time at the beginning.
Defined in the file wk_atime.c, externed wk_atime.h
This value is FOR READING ONLY in any others files
*/
volatile Time64_T wk_real64_value  = 0;

/* It's base time, set into device at the facility,
 * actually it's the time of this GNU c soft compilation
 * */
volatile Time64_T wk_base64_value  = 0;

/*****************************************************************************************
It is Calenar form time.
It is used for user readability only. So, this form of time IS PROHIBITED for transition
btw file and functions
There is only ONE DEFINED value of this form*/
//struct tm  wk64_tm = {0};
struct tm *p_wk64_tm = NULL;
/* It's the functin to get human readable date_time
struct TM *localtime64   (const Time64_T *);
p_wk64_tm = localtime64   (&wk_base64_value);
*/
//static clock_installed_t clock_installed = clock_empty;
gui_status_t  f_wk64_time_Init  (void){
#ifndef STM32_INTERNAL_CLOCK
/*****************************************************************************************/
		RTClock_Init();
//****************************************************************************************
#endif //HALRTC_DEBUG_ONLY
#ifdef 	STM32_INTERNAL_CLOCK
//		HALRTC_Init();
#endif
//struct tm  wk64_tm = {0};
//struct tm *p_wk64_tm;
//	p_wk64_tm=&wk64_tm;
//	PCF85263AT_TimeTM_Get (p_wk64_tm);
//	/* Function: time_t mktime (struct tm *brokentime)
//	The mktime function converts a broken-down time structure to a simple time representation.
//	It also normalizes the contents of the broken-down time structure,
//	and fills in some components based on the values of the others.
//	The mktime function ignores the specified contents of the tm_wday, tm_yday, tm_gmtoff, and tm_zone members
//	of the broken-down time structure. It uses the values of the other components to determine the calendar time;
//	it�s permissible for these components to have unnormalized values outside their normal ranges.
//	The last thing that mktime does is adjust the components of the brokentime structure,
//	including the members that were initially ignored.
//	If the specified broken-down time cannot be represented as a simple time,
//	mktime returns a value of (time_t)(-1) and does not modify the contents of brokentime.
//
//	Calling mktime also sets the current time zone as if tzset were called;
//	mktime uses this information instead of brokentime�s initial tm_gmtoff and tm_zone members.
//	See Time Zone Functions.
//	*/
//	wk_base64_value  = mktime64 (&wk64_tm);
//		https://stm32withoutfear.blogspot.com/2016/12/RTC-stm32f1xx.html
#ifndef STM32_INTERNAL_CLOCK
/*****************************************************************************************/
	RTClock_TimeGet(&wk_base64_value);
//****************************************************************************************
#endif //HALRTC_DEBUG_ONLY
#ifdef 	STM32_INTERNAL_CLOCK
	HALRTC_TimeGet(&wk_base64_value);
#endif

	wk_real64_value  = wk_base64_value; /* It's at the moment time, updatable with MCU every tick.	 	 	 	 	 	 	 	 	       It's user function to set real time at the begining 	 */


//
//	/* Function: time_t time (time_t *result)
//	The time function returns the current calendar time as a value of type time_t.
//	If the argument result is not a null pointer, the calendar time value is also stored in *result.
//	If the current calendar time is not available, the value (time_t)(-1) is returned.
//	*/
//	//wk_time_value = time (NULL); /* set GNU-time to system-time */
//
//
//	/*Function: struct tm * localtime (const time_t *time)
//	 * The localtime function converts the simple time pointed to by time to broken-down time
//	 representation, expressed relative to the user�s specified time zone.
//	The return value is a pointer to a static broken-down time structure,
//	which might be overwritten by subsequent calls to ctime, gmtime, or localtime.
//	(But no other library function overwrites the contents of this object.)
//	The return value is the null pointer if time cannot be represented as a broken-down time;
//	typically this is because the year cannot fit into an int.
//	*/

/*	��'�	����
	int tm_sec;			������� �� ������� ������� (0,59)
	int tm_min;			������� �� ������� ������ (0,59)
	int tm_hour;		������ �� ������ (0,23)
	int tm_mday;		����� ����� (1,31)
	int tm_mon;			̳���� ���� ���� (0,11)
	int tm_year;		���� � 1900 (� Windows), � 1970 (� Unix)
	int tm_wday;		�� � ���� (0,6)
	int tm_yday;		�� � ������� ���� (0,365)
	int tm_isdst;		������ ������� ����
*/
//	f_put_basetime (&wk_base64_value);	/* It's facility set time								*/

	/* This function put the start one time timer Human Readable format:
	 * Mo,23 Feb 2018, 15:00
	 * on the screen on line 280 with GREEN letters
	 * with fastlineH 274
	 * */
//	f_put_starttime (&wk_base64_value);/*It is start of TIMER time set by user 				 */


	/* This function put the stop one time timer Human Readable format:
	 * Mo,23 Feb 2018, 15:00
	 * on the screen on line 300 with RED letters
	 * */
//	f_put_stoptime (&wk_base64_value);	/*It is stop of TIMER time, set by user */
	return gui_OK;
}

gui_status_t f_wk64_time_Loop(void){
	f_wk64_time_Get(&wk_real64_value);
	Time64_T in_time = wk_real64_value;
	p_wk64_tm=localtime64(&in_time);

	if(!f_checkNewDay(RTC_BKP_DR2)){
		// �� ���, �� �������� �� backup ������� ���� ����
		writeDateToBackup(RTC_BKP_DR2,RTC_BKP_DR3);
	}
	return 0;
}

gui_status_t  f_wk64_time_Get  (volatile Time64_T *p_Time64toGet){
	if (p_Time64toGet==NULL){
//		f_PutErrRecord( ERR, (uint32_t)gui_ZERO_ptr);
		return gui_ZERO_ptr;
	}
	Time64_T in_timeget = 0;

#ifndef STM32_INTERNAL_CLOCK
/*****************************************************************************************/
	RTClock_TimeGet(&in_timeget);
//****************************************************************************************
#endif //HALRTC_DEBUG_ONLY
#ifdef 	STM32_INTERNAL_CLOCK
	HALRTC_TimeGet(&in_timeget);
#endif
	(*p_Time64toGet)=in_timeget;
	return gui_OK;
}

gui_status_t  f_wk64_time_Set (Time64_T Time64toSet){
#ifndef STM32_INTERNAL_CLOCK
/*****************************************************************************************/
	RTClock_UserTimeSet(Time64toSet);
//****************************************************************************************
#endif //HALRTC_DEBUG_ONLY
#ifdef 	STM32_INTERNAL_CLOCK
	HALRTC_UserTimeSet(Time64toSet);
#endif

	return gui_OK;
}
