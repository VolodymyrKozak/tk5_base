/*
 * tkFurie.c
 *
 *  Created on: 10 груд. 2018 р.
 *      Author: KozakVF
 */
#include "../../1_PROJECT/tkConfig.h"
#ifdef FURIE_LIB
#include "../../3_Libraries/tkGrid/tkFurie.h"
#include "stdint.h"
#include "math.h"
#include "float.h"
/* При старті двигуна струм - до 7.2 від номінального, АЦП не може
 * адекватно вимірювати такий струм, і дає або 0 попугаїв, або 4095
 * Таку ситуацію розпізнаємо і видаємо струм 7.2* на номінальний
 * паралельно блокуючи обчислення спотрворення грамонік
 * це - декілька секунд при пуску двигуна,
 * але вони важливі для обчислення теплової моделі і відключення
 * двигуна при заклинюванні ротора.. */
extern volatile  adcf_t ratedAdcAmperage;

static float uicosf[N_FURIE]={0};
static float uisinf[N_FURIE]={0};
static uint16_t Ne  =N_FURIE;
static float    Nef =0;

/* Обчислення на один-два порядки скорочуються, якщо не рахувати синуси/косинуси кожен раз
 * При ініціалізації в масив записується таблиця синусів 								*/
void f_uiFurie_Init(void){
	Nef=(float)Ne;
	for (uint8_t i=0;i<Ne;i++){
		float iffff = (float)i;
		float Arg=0;
		Arg=-(2.0f * M_PI*iffff)/Nef;
	      uicosf[i] = cosf(Arg);
	      uisinf[i] = sinf(Arg);
	}
}

/* Перетворення Фурьє для напруги,
 * постійна складова - ігнорується, обчислюється:
 * амплітуда напруги ( у одиницях АЦП для подальшого масштабування
 * фаза - у градусах, для читабельності
 * На вході -
 * показчик на масив відліків АЦП 18 відліків,
 * поточна частота мережі енергопостачання, 40..60Гц
 * дискрет між відліками обчислюється через частоту,
 * */
int	f_ufurie(
		volatile adcf_t *pf_array,	/* показчик на масив відліків АЦП 0..4096, перетворений у формат float 		*/
		volatile ufurie_out_t *p_ufr	/*u_rms   -амплітуда напруги ( у одиницях АЦП для подальшого масштабування	*/
		){

	float  summa_Re1=0;//изначальная сумма для к-того элемента(палки), которая будет считать действительную часть
	float  summa_Im1=0;//изначальная сумма для к-того элемента(палки),которая будет считать мнимую часть

	for (uint16_t ne=0; ne<Ne; ne++){
		/* Фурьє для першої гармоніки                                       */
//		float nef=(float)ne;
//		float Arg=0;
//		Arg=-(2.0 * M_PI*nef)/Nef;
//		float Re1 = cosf(Arg)*pf_array[ne];
//		float Im1 = sinf(Arg)*pf_array[ne];
		float Re1 = uicosf[ne]*pf_array[ne];
		float Im1 = uisinf[ne]*pf_array[ne];
		summa_Re1 = summa_Re1+Re1;
		summa_Im1 = summa_Im1+Im1;
	}

//	volatile double fundamental_ampl =2.0*sqrt(summa_Re*summa_Re+summa_Im*summa_Im)/Nef; // 2* моя корректировка ..Спросить у Андрея..
//	volatile double rms_fundamental = fundamental_ampl*M_SQRT1_2;

	volatile float fundamental_ampl1 =2.0*sqrt(summa_Re1*summa_Re1+summa_Im1*summa_Im1)/Nef; // 2* моя корректировка ..Спросить у Андрея..
	p_ufr->u1harm_ampl = fundamental_ampl1*M_SQRT1_2;

	float F=atan2f(summa_Im1,summa_Re1);
	/* Фаза першої гармоніки в градусах */
	p_ufr->u_phase=F*(180.0f/ M_PI);
	/* Приводимо фазу в діапазон 0..360 град */
	if(p_ufr->u_phase<0){p_ufr->u_phase=p_ufr->u_phase+360.0f;}


	return 0;
}


/* Функція реалізує дискретне перетворення Фурьє
 * для обчислення величин у мережі змінного струму відомої частоти 46..54Гц
 * кількість відліків - завжди 18 */
int f_ifurie(	volatile adcf_t *pf_array,
				volatile ifurie_out_t *p_ifr){
	float sumZero = 0;
	float sumNegative=0;
	volatile float spectr[10]={0};

	for (uint16_t ke=0; ke<(Ne/2+1); ke++){
		float  summa_Re1=0;//изначальная сумма для к-того элемента(палки), которая будет считать действительную часть
		float  summa_Im1=0;//изначальная сумма для к-того элемента(палки),которая будет считать мнимую часть
		for (uint16_t ne=0; ne<Ne; ne++){
			/* Фурьє для першої гармоніки                                       */
//			float nef=(float)ne;
//			float Arg=0;
//			Arg=-(2.0 * M_PI*nef*kef)/Nef;
//
//			float Re1 = cosf(Arg)*pf_array[ne];
//			float Im1 = sinf(Arg)*pf_array[ne];
			uint16_t i_icos =0;
			i_icos = (ke*ne) % Ne;
			float Re1 = (uicosf[i_icos]*pf_array[ne])/Nef;
			float Im1 = (uisinf[i_icos]*pf_array[ne])/Nef;
			summa_Re1 = summa_Re1+Re1;
			summa_Im1 = summa_Im1+Im1;
		}
		volatile float ampl_ke =2.0*sqrt(summa_Re1*summa_Re1+summa_Im1*summa_Im1); // 2* моя корректировка ..Спросить у Андрея..
		if (ke==0){spectr[ke]=ampl_ke/2.0f;}
		else if (ke==1){
			spectr[ke]=ampl_ke;
			p_ifr->rms_fundamental = ampl_ke*M_SQRT1_2;
			float F=atan2f(summa_Im1,summa_Re1);
			/* Фаза першої гармоніки в градусах */
			p_ifr->phase_fundamental=F*(180.0/ M_PI);
			/* Приводимо фазу в діапазон 0..360 град */
			if(p_ifr->phase_fundamental<0){p_ifr->phase_fundamental=p_ifr->phase_fundamental+360.0f;}
		}
		else{
			spectr[ke]=ampl_ke;
			/* Гармоніки нульової послідовності */
			/* 3, 6. 9, 12*/
			if ((ke>0)&&((ke % 3)==0)){
				sumZero+=ampl_ke*ampl_ke;
			}
			/*2, 5, 8, 11 */
			/* Гармоніки негативної послідовності */
			if ((ke>=2)&&(((ke-2) % 3)==0)){
				sumNegative+=ampl_ke*ampl_ke;
			}
		}
		//https://www.bsuir.by/m/12_101945_1_116065.pdf

	}
	/* Якщо rms першої гармоніки більша  чотирикратного номінального струму
	 * - це пуск електродвигуна, а отже штучно множимо на 7.2,
	 * пусковий струм, що понад пропускну спроможність АЦП */
//	if(p_ifr->rms_fundamental>4.0f*ratedAdcAmperage){
//		p_ifr->rms_fundamental=7.2f*ratedAdcAmperage;
//		//і блокуємо гармоніки
//		p_ifr->rmsZero = 0;
//		p_ifr->rmsNegative;
//	}
//	else{
	p_ifr->rmsZero = sqrt(sumZero)*M_SQRT1_2;
	p_ifr->rmsNegative = sqrt(sumNegative)*M_SQRT1_2;
//	}
	return(0);
}
/* Функція обчислення rms через суму квадратів елементарних струмів*/
int f_iheat(
		volatile adcu16_t *p200_array,
		float zero_line,
		uint16_t Nn,
		volatile iheat_out_t  *p_ih){

	uint16_t zero_line16=(uint16_t)zero_line;
	float Nnf = (float)Nn;
	/* rms_UINT16 */
	volatile uint32_t  summa_Zero = 0;
	volatile uint64_t  summa_Qn64 = 0;  //Теплова енергія
	for (uint16_t n=0; n<Nn; n++){
		/* zero point UINT16*/
		summa_Zero = summa_Zero + p200_array[n];
		/* сума квадратів діючих струмів для визначення тепла
		 * поділена на кількість відліків
		 * тобто в середньому за один відлік (100us) витрачено на нагрівання */
		 summa_Qn64=summa_Qn64+((p200_array[n]-zero_line16)
		 /*262144*/   	 *(p200_array[n]-zero_line16));
	}
	p_ih->permanent_component = ((double)(summa_Zero))/Nnf;

	double rmsfd=sqrt ((double)(summa_Qn64/Nnf));
	float rmsfd_f=(float)rmsfd;
//	if(rmsfd_f>(4.0f*ratedAdcAmperage)){
//		/* Якщо rms  більша  чотирикратного номінального струму
//		 * - це пуск електродвигуна, а отже штучно множимо на 7.2,
//		 * пусковий струм, що понад пропускну спроможність АЦП */
//		p_ih->rms_heat=7.2f*ratedAdcAmperage;
//	}
//	else{
		p_ih->rms_heat=(float)(rmsfd);
//	}
	return 0;

}
/* Сдвиговое дискретное преобразование Фурье (СДПФ)
 * http://aco.ifmo.ru/el_books/numerical_methods/lectures/glava4_3.html*/







#endif //FURIE_LIB
