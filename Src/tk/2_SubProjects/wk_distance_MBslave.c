///*
// * wk_mb_slave1.c
// *
// *  Created on: 25 лист. 2019 р.
// *      Author: KozakVF
// */
#include "stdint.h"

#include "../3_Libraries/tkModbus/mb.h"
#include "vv300set_factory.h"
#include "vv300set_user.h"
#include "wk_distance_MBslave.h"
#include "../3_Libraries/vkDeclarations.h"


#include "../4_tk4DEBUG/d0_debug_main.h"
/**********************************************************************************************
 * ГЛОБАЛЬНІ ЗМІННІ. ВИХІДНІ ДЛЯ МОДУЛЯ
 **********************************************************************************************/
extern uint16_t GRIDRegsBlock[0x40];

/* Зглажена частота в герцах 	  */
extern volatile hzf_t 		Frequency;
/* Зглажена напруга в вольтах*/
extern volatile voltf_t 	Voltage;
/* Зглажені струми в амперах */
extern volatile amperf_t 	aAmperage;
extern volatile amperf_t 	bAmperage;
extern volatile amperf_t 	cAmperage;
extern FunctionalState CmndForMotor;
/* Коефiцієнт перекосу фаз 									*/
extern volatile procentf_t AmperageUnbalance;
extern on_off_t LastCmndForMotor;
/* Зглажені основні коєфіцієнти ефективності мережі  */
extern procentf_t PowerFactor;
extern procentf_t UnbalanceDeratingFactor;
extern procentf_t HarmonicsDeratingFactor;
extern on_off_t StartCmnd;
//procentf_t LeakDeratingFactor=100.0;
//procentf_t GrundDeratingFactor=100.0;
//procentf_t NeutralDeratingFactor=100.0;

/* Зглажені інтегральні показники ефективності мережі */
extern procentf_t HmUnbDerating ;
extern procentf_t fullDerating  ;

/* Показники енергоспоживання                               */
extern vatt_t	ActivePower 		;
extern vatt_t	ReactivePower 		;
extern vatt_t	UnbalanceLostPower 	;
extern vatt_t	HarmonicLostPower 	;
extern vatt_t	NeutralHarmonicLostPower 	;
extern vatt_t	NegativeHarmonicLostPower 	;

/**********************************************************************************************/
/*Теплова енергія як частка від повної теплової енергії,
 *необхідної для зростання температури до номінативної
*/
procentf_t ThermalModelBehavior = 0;

volatile float 	    leakAmperage_mA;
//сопротивления изоляции, в мОм:
extern float InsulationResistance_mOm;
/* Зглажені значення в кодах АЦП входів кнопок і датчиків  */
extern uint16_t SensorsAndButtonsADC[10];
/* Поточне значення тиску в одиницях, заданих користувачем */
extern float PresureValue;
extern on_off_t AutoCtrl;
 /* За поданою командою мотор міг включитися, а міг і не включитится,
  * це реальний стан, який визначається по величині робочого струму */
 extern on_off_t MotorState;
 //extern liquid_sensor_level_t LiquidSensor_Level;
 //extern motorfault_t MotorFault;

 /* Поточний стан датчика - останній сигнал */

 extern uint32_t MotorOperationalLife;
 extern GPIO_PinState dState[10];

 static uint16_t MotorFaltDrop = 0;
 #define PASSWORD1 0x1706
 static uint16_t accessFlag1=0;
 #define PASSWORD2 0x9571
 static uint16_t accessFlag2=0;
 static uint8_t ExFlag=0;

 static uint8_t f_0x0080_20m(uint16_t *res);
 static int8_t f_03_float_to_Tx(
 		float af,
 		modbus_slave_tx_msg_t *tx,
 		uint16_t i);
 static int8_t  f_float_to_2x16(float af, uint16_t *ptwo);
 //static float   f_cst(uint16_t xxxx);
 static uint8_t f_0x0090_20m(uint16_t *res);

 /* Прапор запису на флеш налаштувань виробника
  * піднімається в wk_distance_MBslave.c/h
  * командою дистанційного управління через МОДБАС*/
 extern volatile FunctionalState UserSetWriteFlashFlag;
 /* Прапор запису на флеш налаштувань користувача
  * піднімається в wk_distance_MBslave.c/h
  * командою дистанційного управління через МОДБАС*/
 extern volatile FunctionalState FactorySetWriteFlashFlag;
 extern volatile FunctionalState FactorySetWriteMemoryFlag;
 extern volatile FunctionalState UserSetWriteMemoryFlag;
 /* Вхід симуляції від Модбас */
 extern uint16_t AmplMode; /* 0- синусоїдальна
  	 	 	 	 	 	   1- прямокутна*/
 extern uint16_t simAmplAve; /* Амплітуда середня, в процентах  */
 extern uint16_t simAmpB; /* Амплітуда фази Б, для симуляції асиметрії*/
 extern uint16_t CosinusPhi;  /* Косинус у процентах */


/* Ця функція обробляє вхідні повідомлення Модбас ТК4
 * та готує відповіді згідно з прикладними алгоритмами Модбас */
void fTK5_Slave1(
		modbus_slave_rx_msg_t *rx,
		modbus_slave_tx_msg_t *tx
		){
	memset(tx,0,sizeof(modbus_slave_tx_msg_t));
        uint8_t Funk_Mod=rx->msg[1]; //Принятая функция
        tx->msg[0]=rx->msg[0];
        ExFlag=0;
        /****************************************************************/
        /****************************************************************/
        /* */

        if(Funk_Mod==0x03){
           	uint16_t rAddress=(rx->msg[2]<<8)+rx->msg[3];
           	if (rAddress==0xEEEE){
           		ExFlag =  0;
           	}
            uint16_t NumberOfRegs=(rx->msg[4]<<8)+rx->msg[5];
            ExFlag=0xFF;
            while(ExFlag==0xFF){
            	if(NumberOfRegs>TX_MAX_SLAVE_MSG_LENGTH){ExFlag=0x03;break;}

//             /* ЧИТАЄМО ОСНОВНІ ПАРАМЕТРИ                 */
            	if ((rAddress>=0x0080)&&(rAddress<0x0090)){
            		uint16_t x10Regs[0x10]={0};
            		ExFlag =  f_0x0080_20m(x10Regs);
                	int32_t shift = rAddress-0x0080;
                    if((shift+NumberOfRegs)>0x10){ExFlag=0x02;}
                    else{
                    	for(uint8_t i=0;i<NumberOfRegs;i++){
                    		tx->msg[3+2*i]=x10Regs[i+shift]   / 0x100;
                    		tx->msg[3+2*i+1]=x10Regs[i+shift] % 0x100;
                    	}
                    	tx->length=3+2*NumberOfRegs;
            		}

            	}
            	else if ((rAddress>=0x0090)&&(rAddress<0x00A0)){
            		uint16_t x10Regs[0x10]={0};
            		ExFlag =  f_0x0090_20m(x10Regs);
                	int32_t shift = rAddress-0x0090;
                    if((shift+NumberOfRegs)>0x10){ExFlag=0x02;}
                    else{
                    	for(uint8_t i=0;i<NumberOfRegs;i++){
                    		tx->msg[3+2*i]=x10Regs[i+shift]   / 0x100;
                    		tx->msg[3+2*i+1]=x10Regs[i+shift] % 0x100;
                    	}
                    	tx->length=3+2*NumberOfRegs;
                    }

            	}
            	else if(((rAddress>=0x0100)&&(rAddress<0x0140))){
            		int32_t shift = rAddress-0x0100;
            		 if((shift+NumberOfRegs)>0x40){ExFlag=0x02;}
            		 else{
            			 for(uint8_t i=0;i<NumberOfRegs;i++){
            				 tx->msg[3+2*i]=GRIDRegsBlock[i+shift]   / 0x100;
            				 tx->msg[3+2*i+1]=GRIDRegsBlock[i+shift] % 0x100;
            			 }

            		 }
            	}
            	/* Читаємо за один раз всі налаштування користувача */
        		else if((rAddress==0x2000)&&(NumberOfRegs==0x20)){
        			uint16_t value_i=0;
        			/* 0х2000..0x2001*/
        			value_i=(uint16_t)user_settings.oper_mode;
        			tx->msg[3+0x00]=value_i / 0x100;
        			tx->msg[3+0x01]=value_i % 0x100;

        		}
            	/* Читаємо за один раз всі налаштувння виробника    */
        		else if((rAddress==0x2020)&&(NumberOfRegs==0x20)){
        		    uint16_t val=0;
            	///float amperageAclbr; 	// 0х2020
            	    f_03_float_to_Tx(fSet.amperageAclbr, tx, 0x00);
            	// float clbr_iA;			// 0х2022
            	    f_03_float_to_Tx(fSet.clbr_iA,       tx, 0x04);
            	//float amperageBclbr; 		// 0х2024
            	    f_03_float_to_Tx(fSet.amperageBclbr, tx, 0x08);
            	//float clbr_iB;			// 0х2026
            	    f_03_float_to_Tx(fSet.clbr_iB,       tx, 0x0C);
            	//float amperageCclbr; 		// 0х2028
            	    f_03_float_to_Tx(fSet.amperageCclbr, tx, 0x10);
            	//float clbr_iC;			// 0х202A
            	    f_03_float_to_Tx(fSet.clbr_iC,       tx, 0x14);
            	//fSet.voltageCclbr			// 0x202C
            	    f_03_float_to_Tx(fSet.voltageCclbr,  tx, 0x18);
            	//float clbr_uC;			// 0х202E
            	    f_03_float_to_Tx(fSet.clbr_uC,       tx, 0x1C);
            	//uint16_t DeviceMode;     //0x1030 Типорозмір контролера 1...5
            	    val=  fSet.DeviceMode;
            	    tx->msg[3+0x20] = val / 0x100;
            	    tx->msg[3+0x21] = val % 0x100;
            	//uint16_t Faznost;		 //0x1031
            	    val=  fSet.Faznost;
            	    tx->msg[3+0x22] = val / 0x100;
            	    tx->msg[3+0x23] = val % 0x100;

            	}
#ifdef DEBUGGING_ONLY
            	else if((rAddress==0xE000)&&(NumberOfRegs==0x0005)){
            		tx->msg[3]=(uint16_t)print_debug0/0x100;
            		tx->msg[4]=(uint16_t)print_debug0%0x100;
            		tx->msg[5]=(uint16_t)AmplMode/0x100;
            		tx->msg[6]=(uint16_t)AmplMode%0x100;
            		tx->msg[7]=simAmplAve/0x100;
            		tx->msg[8]=simAmplAve%0x100;
            		tx->msg[9]=simAmpB/0x100;
            		tx->msg[10]=simAmpB%0x100;
            		tx->msg[11]=CosinusPhi/0x100;
            		tx->msg[12]=CosinusPhi%0x100;

            	}
#endif    //DEBUGGING_ONLY
            	else{}
            	tx->msg[2]=2*NumberOfRegs;
            	tx->length=3+2*NumberOfRegs;
            	ExFlag=0x00;
            }/* Якщо не помилка кількості запитаних регістрів*/
            if(!ExFlag){
            	tx->msg[2]=(uint8_t)(NumberOfRegs*2);
            }
            else{
            	tx->msg[2]=0x03;/* Код помилки згідно зі стандартом Модбас */
            }
        }/* конец работи по функции 0х03 = читать группу регистров
           начало работы по функции 0х06 = писать один регистр  */
       else if (Funk_Mod==0x06){
        	volatile uint16_t RegAddress=(rx->msg[2]<<8)+rx->msg[3];
        	volatile uint16_t RegValue=(rx->msg[4]<<8)+rx->msg[5];
//        	volatile static float aFloat_buff=0;
//        	volatile static float bFloat_buff=0;
        	switch(RegAddress){
        	case 0x0000:
        	// Команда ПУСК/СТОП електродвигуна чи включення пускача )
        		if(AmplMode==0){
        			if     (RegValue==0x00){CmndForMotor=DISABLE;}
        			else if(RegValue==0xFF){CmndForMotor=ENABLE;}
        			else{}
        		}
        		else if ((AmplMode==1)||(AmplMode==2)){
        			/* Якщо це симуляція - не чіпаємо пускач,
        			 * вмикаємо чи вимикаємо симуляцію відліків струму*/
        			LastCmndForMotor=(on_off_t) RegValue;
        		}
        		else{}
        	break;
        	case 0x0010:
        		/* Команда запису на флеш налаштувань користувача    */
        		UserSetWriteFlashFlag=(FunctionalState)RegValue;
        	break;
        	case 0x0011:
        		/* Команда запису на флеш налаштувань   виробника */
        		FactorySetWriteFlashFlag=(FunctionalState)RegValue;
        	break;
//        	case 0x1000:
//        		if (RegValue<2){
//        			user_settings.oper_mode=(oper_mode_t)RegValue;
//        		}
//        	break;
#ifdef DEBUGGING_ONLY
	        case 0xE000:
	        	/* Экран Тк4:*/
	        	if(RegValue<6){	print_debug0=(print_debug0_t)RegValue;}
	        break;
	        case 0xE001:
	        	/* Реальний струм чи симуляція */
	        	if(RegValue<3){AmplMode=RegValue;}
	        break;
	        case 0xE002:
	        	/* Струм всіх фаз в процентах */
//	        	uint16_t simAmplAve =100; /* Амплітуда середня, в процентах  */
	        	simAmplAve=RegValue;
	        break;
	        case 0xE003:
//	        	uint16_t     =100; /* Амплітуда фази Б, для симуляції асиметрії*/
	        	simAmpB=RegValue;
	        break;
	        case 0xE004:
//	        	uint16_t CosinusPhi =50;  /* Косинус у процентах */
	        	if((RegValue>40)&&(RegValue<=100)){CosinusPhi=RegValue;}
	        break;
#endif // DEBUGGING_ONLY
//#endif //DISTANE_DEBUG
///**************************************************************************************************************/
        	default:{
        		ExFlag=0x02;
        	}
        	}//switch(RegAddress){
        	/* Responce message 0x06*/
        	if (ExFlag == 0){
        		/* Register Number */
        		tx->msg[2]=rx->msg[2];
        		tx->msg[3]=rx->msg[3];
        		/* Register Value */
        		tx->msg[4]=rx->msg[4];
        		tx->msg[5]=rx->msg[5];
        		tx->length=6;
        	}
    }/* Кінець функції 06*/
    else if(Funk_Mod==0x10){
    	uint16_t StartingAddress     = rx->msg[2]*0x100+rx->msg[3];
    	uint16_t QuantityOfRegisters = rx->msg[4]*0x100+rx->msg[5];
    	uint16_t ByteCntr            = rx->msg[6];
    	if((QuantityOfRegisters*2)!= ByteCntr){
    		ExFlag=0x03;
    	}
    	else{
    		/*Пишемо за один раз всі налаштування користувача */
    		if((StartingAddress==0x2000)&&(QuantityOfRegisters==0x20)){
    			uint16_t value_i=0;
    			value_i=rx->msg[7+0x00]*0x100+rx->msg[7+0x01];
    			user_settings.oper_mode=(oper_mode_t)value_i;


    			UserSetWriteMemoryFlag=ENABLE;
    		}
    		/*Пишемо за один раз всі налаштування виробника */
    		else if((StartingAddress==0x2020)&&(QuantityOfRegisters==0x20)){
    			uint16_t dec=0;
    			uint16_t fra=0;
//    			float amperageAclbr; 	// 0х2000
    			dec=rx->msg[7+0x00]*0x100+rx->msg[7+ 0x01];
    			fra=rx->msg[7+0x02]*0x100+rx->msg[7+ 0x03];
    			fSet.amperageAclbr=(float)dec+(float)fra/10000.f;

//    			float clbr_iA;			// 0х2002
    			dec=rx->msg[7+0x04]*0x100+rx->msg[7+ 0x05];
    			fra=rx->msg[7+0x06]*0x100+rx->msg[7+ 0x07];
    			fSet.clbr_iA=(float)dec+(float)fra/10000.f;

//    			float amperageBclbr; 	// 0х2004
    			dec=rx->msg[7+0x08]*0x100+rx->msg[7+ 0x09];
    			fra=rx->msg[7+0x0A]*0x100+rx->msg[7+ 0x0B];
    			fSet.amperageBclbr=(float)dec+(float)fra/10000.f;

//    			float clbr_iB;			// 0х2006
    			dec=rx->msg[7+0x0C]*0x100+rx->msg[7+ 0x0D];
    			fra=rx->msg[7+0x0E]*0x100+rx->msg[7+ 0x0F];
    			fSet.clbr_iB=(float)dec+(float)fra/10000.f;

//    			float amperageCclbr; 	// 0х2008
    			dec=rx->msg[7+0x10]*0x100+rx->msg[7+ 0x11];
    			fra=rx->msg[7+0x12]*0x100+rx->msg[7+ 0x13];
    			fSet.amperageCclbr=(float)dec+(float)fra/10000.f;

//    			float clbr_iC;			// 0х200A
    			dec=rx->msg[7+0x14]*0x100+rx->msg[7+ 0x15];
    			fra=rx->msg[7+0x16]*0x100+rx->msg[7+ 0x17];
    			fSet.clbr_iC=(float)dec+(float)fra/10000.f;

//    			fSet.voltageCclbr		//0x200C
    			dec=rx->msg[7+0x18]*0x100+rx->msg[7+ 0x19];
    			fra=rx->msg[7+0x1A]*0x100+rx->msg[7+ 0x1B];
    			fSet.voltageCclbr=(float)dec+(float)fra/10000.f;

//    			float clbr_uC;			// 0х200E
    			dec=rx->msg[7+0x1C]*0x100+rx->msg[7+ 0x1D];
    			fra=rx->msg[7+0x1E]*0x100+rx->msg[7+ 0x1F];
    			fSet.clbr_iC=(float)dec+(float)fra/10000.f;

//    			uint16_t DeviceMode;     //0x1010 Типорозмір контролера 1...5
    			uint16_t value =0;
    			value=rx->msg[7+0x20]*0x100+rx->msg[7+0x21];
    			fSet.DeviceMode   =value;

//    			uint16_t Faznost;		 //0x1011
    			value=rx->msg[7+0x22]*0x100+rx->msg[7+0x23];
    			fSet.Faznost   =value;

//

//    			uint32_t xx12;       	 //0x1012
//
//    			uint32_t xx14;			 //0x1014
//    			uint32_t xx16;			 //0x1016
//
//    			uint32_t xx18;			//0x1018
//    			uint32_t xx1A;			//0x101A
//
//    			uint32_t xx1C;			//0x101C
    			FactorySetWriteMemoryFlag=ENABLE;

    		}

    		else{
    			ExFlag=0x02;
    		}
    	}
    	if (ExFlag == 0){
    		/* StartingAddress */
    		tx->msg[2]=rx->msg[2];
    		tx->msg[3]=rx->msg[3];
    		/* QuantityOfRegisters */
    		tx->msg[4]=rx->msg[4];
    		tx->msg[5]=rx->msg[5];
    		tx->length=6;
    	}
    } /* Кінець функції 0x10 (16)*/
    else if(Funk_Mod==0x11){
//        Response
//        Function code 1 Byte 0x11
    	  //tx->msg[1]=rx->msg[1];
//        Byte Count 1 Byte
    	  tx->msg[2]=6;
//        Server ID device specific
    	  tx->msg[3]='t'; tx->msg[4]='k'; tx->msg[5]='5';tx->msg[6]=' ';
    	  tx->msg[7]=(uint32_t)MotorState % 0x100;
    	  tx->length=8;
//        Run Indicator Status 1 Byte 0x00 = OFF, 0xFF = ON
//        Additional Data
       }

    else {
    	ExFlag=1;
    }
    if(ExFlag){
        tx->msg[1]=rx->msg[1]+0x80;
        tx->msg[2]=ExFlag;
        tx->length=3;
    }
    else{
        tx->msg[1]=rx->msg[1];

    }
    tx->msg[0]=rx->msg[0];
}
//

static uint8_t f_0x0080_20m(uint16_t *res){
	uint8_t Ex=0;

	uint16_t FaultsCode	=0;
	res[0]=FaultsCode;
     // Теплова енергія як частка від повної теплової енергії,
     // необхідної для зростання температури до номінативної
	res[1]=ThermalModelBehavior;


 	//Поточна активна потужність
    uint16_t rif[2]={0};
 	if(f_float_to_2x16(ActivePower,   rif)!=0){Ex=0x02;}
 	else {
 		res[2]=rif[0];
 		res[3]=rif[1];
 	}


 	//Струм фази А, А
 	if(f_float_to_2x16(aAmperage,     rif)!=0){Ex=0x02;}
 	else {
 		res[4]=rif[0];
 		res[5]=rif[1];
 	}
 	//Струм фази В, А
 	if(f_float_to_2x16(bAmperage,     rif)!=0){Ex=0x02;}
 	else{
		res[6]=rif[0];
		res[7]=rif[1];
 	}
 	//Струм фази С, Ф
 	if(f_float_to_2x16(cAmperage,     rif)!=0){Ex=0x02;}
 	else{
		res[8]=rif[0];
		res[9]=rif[1];
 	}
     res[0xA] = (uint16_t) Voltage;

     // Сосинус фі
     res[0xB] = (uint16_t)PowerFactor;
     // Коефіцієнт асиметрії

     res[0xC] = (uint16_t) UnbalanceDeratingFactor;
     res[0xD] = (uint16_t) HarmonicsDeratingFactor;
 	/*Напрацювання мотогодин, два регістри, старший і молодший слова 32 біт*/

     res[0xE] =	  MotorOperationalLife % 0x10000;
     res[0xF] =	  MotorOperationalLife % 0x10000;
     return Ex;
}


/*Зглажені параметри ефективності мережі 					*/
/*Коефіцієнт нелінійних спотворень по струму у кожній фазі  */
extern procentf_t aTotalHarmonicDistortion;
extern procentf_t bTotalHarmonicDistortion;
extern procentf_t cTotalHarmonicDistortion;

/************************************************************************
Коефіцієнт нелінійних спотворень по струму узагальнений    				*/
extern procentf_t THDi_HarmonicAmperageDistortion;
/*THDі вказує на ступінь спотворення форми синусоїди струму.
THDi менше 10% вважається нормальним,
ризик несправностей обладнання відсутній, перегрівання до 6%.

THDi між 10% і 50% вказує на значне гармонійне спотворення.
Падіння коефіцієнта потужності елекродвигунів, підвищення температури,
що означає, що кабелі та установок повинні бути понадгабаритними.

THDi, що перевищує 50%, вказує на високі гармонічні спотворення.
Високий ризик виходу обладнання з ладу. Необхідний поглиблений аналіз причин
 і заходи з їх усунення, зокрема рекомендується
 система придушення несинусоїдальнусті струму.

Скорочення терміну служби обладнання:
(Дані Канадської асоціації електриків).
Коли спотворення напруги живлення знаходиться в діапазоні 10%,
термін служби обладнання значно скорочується.
Залежно від типу пристрою, скорочення терміну служби:
32,5% для однофазних електродвигунів
18% для трифазних електродвигунів
5% для трансформаторів.
***************************************************************************
3, 5, 7, 11 and 13 - найбільше небезпечні для електродвигунів
*/
static uint8_t f_0x0090_20m(uint16_t *res){
	uint8_t Ex=0;
    uint16_t rif[2]={0};
	/* Показники енергоспоживання                               */
	//vatt_t	ActivePower 		= 	0;
 	//Поточна активна потужність
 	if(f_float_to_2x16(ActivePower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[0]=rif[0];
 		res[1]=rif[1];
 	}
	//vatt_t	ReactivePower 		=   0;
 	if(f_float_to_2x16(ReactivePower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[2]=rif[0];
 		res[3]=rif[1];
 	}
	//vatt_t	UnbalanceLostPower 	=	0;
 	if(f_float_to_2x16(UnbalanceLostPower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[4]=rif[0];
 		res[5]=rif[1];
 	}
	//vatt_t	HarmonicLostPower 	= 	0;
 	if(f_float_to_2x16(HarmonicLostPower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[6]=rif[0];
 		res[7]=rif[1];
 	}
	//vatt_t	NeutralHarmonicLostPower 	= 	0;
 	if(f_float_to_2x16(NeutralHarmonicLostPower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[8]=rif[0];
 		res[9]=rif[1];
 	}
	//vatt_t	NegativeHarmonicLostPower 	= 	0;
 	if(f_float_to_2x16(NeutralHarmonicLostPower*1000.0f,   rif)!=0){Ex=0x02;}
 	else {
 		res[0xA]=rif[0];
 		res[0xB]=rif[1];
 	}
	/**********************************************************************************************
	 * ЛОКАЛЬНІ ЗМІННІ
	 **********************************************************************************************/
	/*Зглажені параметри ефективності мережі 					*/
	/*Коефіцієнт нелінійних спотворень по струму у кожній фазі  */
	//procentf_t aTotalHarmonicDistortion = 0;
 	res[0xC]=aTotalHarmonicDistortion;
	//procentf_t bTotalHarmonicDistortion = 0;
 	res[0xD]=bTotalHarmonicDistortion;
	//procentf_t cTotalHarmonicDistortion = 0;
 	res[0xE]=cTotalHarmonicDistortion;
	/************************************************************************
	Коефіцієнт нелінійних спотворень по струму узагальнений    				*/
	res[0xF]= THDi_HarmonicAmperageDistortion;

	/*THDі вказує на ступінь спотворення форми синусоїди струму.
	THDi менше 10% вважається нормальним,
	ризик несправностей обладнання відсутній, перегрівання до 6%.

	THDi між 10% і 50% вказує на значне гармонійне спотворення.
	Падіння коефіцієнта потужності елекродвигунів, підвищення температури,
	що означає, що кабелі та установок повинні бути понадгабаритними.

	THDi, що перевищує 50%, вказує на високі гармонічні спотворення.
	Високий ризик виходу обладнання з ладу. Необхідний поглиблений аналіз причин
	 і заходи з їх усунення, зокрема рекомендується
	 система придушення несинусоїдальнусті струму.

	Скорочення терміну служби обладнання:
	(Дані Канадської асоціації електриків).
	Коли спотворення напруги живлення знаходиться в діапазоні 10%,
	термін служби обладнання значно скорочується.
	Залежно від типу пристрою, скорочення терміну служби:
	32,5% для однофазних електродвигунів
	18% для трифазних електродвигунів
	5% для трансформаторів.
	***************************************************************************
	3, 5, 7, 11 and 13 - найбільше небезпечні для електродвигунів
	*/

     return Ex;
}

/* Ця функція перетворює число з плаваючою крапкою
 * на послідовність чотирьох байт в вихідному повідомлення Модбас
 * функції 0х03
 * починаючи з заданого номера байта + 3
 * (0-адрес модбас,1-код функції, 2 - кількість байт)
 * Функція повертає:
      0, якщо все Ок
     -1, якщо не має показчика або номер байта не кратний 4    */
static int8_t f_03_float_to_Tx(
		float af,                  // вхідне число
		modbus_slave_tx_msg_t *tx, // показчик на вихідне повідомлення Модбас
		uint16_t i){               // номер байта, з якого записати число float в чотири байти
	if (tx==NULL)  {return -1;}
	if ((i%4)!= 0) {return -1;}
	uint16_t regfloat[2]={0};
	f_float_to_2x16(af, regfloat);
	tx->msg[3+i+0x00] = regfloat[0] / 0x100;
	tx->msg[3+i+0x01] = regfloat[0] % 0x100;
	tx->msg[3+i+0x02] = regfloat[1] / 0x100;
	tx->msg[3+i+0x03] = regfloat[1] % 0x100;
	return 0;
}
/* Ця функція перетворює число з плаваючою крапкою
 * на масив з двох чисел - цілою частини float
 * та дробної частини float перші чотири десятичні цифри
 * Аргументи =
 * 		вхідне число float af
 * 		показчик на масив двох числе int16_t
 * Функція повертає
 *      0, якщо все Ок
 *      -1,якщо число надто велике або немає показчика на масив*/
static int8_t f_float_to_2x16(float af, uint16_t *ptwo){
	if((af>65000.0f)||(ptwo==NULL)){	return -1;}
	else {
		ptwo[0] = (uint16_t)af;
		float r     =(float)ptwo[0];
		float q     =(af-r)*10000.0f;
		ptwo[1] = (uint16_t)(q);
		return 0;
	}
}
