/*
 * tkSGL.c
 *
 *  Created on: 2 ����. 2017 �.
 *      Author: Volodymyr.Kozak
 */
#include "../3_Libraries/tkSGL.h"

#include <stdint.h> /* int32_t style anable */
#include <stddef.h> /* NULL is anable       */
#include <stdbool.h>

uint32_t  f_btnExpSmoothing(
				uint16_t sample_value,            /* ������� �������� ����������, �� */
				uint32_t previous_smoothed_value, /* �������� �������� ��������, ��  */
				uint32_t Ntau				      /* ���Ҳ��� ����������� � ������� �����
				 	 	 	 	 	 	 	 	     ����� 	1Tou 	67%,
				 	 	 	 	 	 	 	 	     	 	2Tou 	86,5%
				 	 	 	 	 	 	 	 	     	 	3Tou 	95,0%
				 	 	 	 	 	 	 	 	     	 	4Tou 	98,2%
				 	 	 	 	 	 	 	 	     	 	5Tou    99,3% */
				){
		uint32_t in_sm16_value=0;
		 /* ������� �������� ��������, ��	   */
		in_sm16_value = sample_value/Ntau + previous_smoothed_value*(Ntau-1)/Ntau;


  return (in_sm16_value);
}



sgl_err_t f_ExponentialSmoothing(
				float sample_value,            /* ������� �������� ����������, �� */
				float previous_smoothed_value, /* �������� �������� ��������, ��  */
				float time_constant,		   /* ��������� ����, ������� 		   */
				float sampling_frequency,	   /* ������� ���������, ��		   */
				float *p_smoothed_value		   /* ������� �������� ��������, ��	   */
				){

	sgl_err_t fExpErr =                     	 SGL_UNKNOWN_ERROR;
	if (p_smoothed_value==NULL) {fExpErr=        SGL_NULL_ptr;}
	float smoothing_factor = 0;
	smoothing_factor=1.0f/(sampling_frequency * time_constant);
	if ((smoothing_factor <=0) || (smoothing_factor >= 1)){
		fExpErr									=SGL_WRONG_arg;
	}
	else {
		float in_sm_value = 0;

		in_sm_value = (smoothing_factor * sample_value) + ((1.0-smoothing_factor) * previous_smoothed_value);

		(*p_smoothed_value) =in_sm_value;
		fExpErr                              	=SGL_SUCCESS;
	}
  return (fExpErr);
}

