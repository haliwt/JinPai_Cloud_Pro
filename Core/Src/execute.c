#include "execute.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "run.h"
#include "wifi_fun.h"
#include "esp8266.h"
#include "buzzer.h"


void SetPowerOn_ForDoing(void)
{
    
    run_t.gPower_flag = POWER_ON;
    run_t.gFan_continueRun =1;
    run_t.gPower_On=POWER_ON;
	run_t.open_fan_works_flag++;
    
    if(run_t.app_appointment_time_power_on != POWER_ON){
	    run_t.gFan = 1;
		run_t.gDry = 1;
		run_t.gPlasma =1;       //"杀菌"
		run_t.gUltrasonic = 1; // "驱虫"

		//FAN_CCW_RUN();
		Fan_Run_Fun();
	    PLASMA_SetHigh(); //
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	    PTC_SetHigh();
    }
   
	
   
	
 }

void SetPowerOff_ForDoing(void)
{
   
	run_t.gPower_flag = 0;
//	run_t.gFan_continueRun =1; //the fan still run 60s
	run_t.gPower_On = POWER_OFF;

 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUltrasonic = 0; // "驱虫"
	
	
    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	if(run_t.open_fan_works_flag !=1){ //input ac power be detect fan default 
		FAN_Stop();
		run_t.gFan_continueRun=0;
	}

}



void ActionEvent_Handler(void)
{
   if(run_t.ptc_too_heat_value ==0){
	if(run_t.gDry == 1){
	
        PTC_SetHigh();
	    run_t.fan_start_shut_off_flag=1;
	}
	else{
	   PTC_SetLow();
	   run_t.fan_start_shut_off_flag=0;

	}
   }
    
	//kill
	if(run_t.gPlasma == 1){
	     PLASMA_SetHigh();
		 run_t.fan_start_shut_off_flag=1;
	}
	else{

		PLASMA_SetLow();
		run_t.fan_start_shut_off_flag=0;
	}

    if(run_t.fan_detect_malfuntion== 0){
		if(run_t.set_wind_speed_value==0){
			 run_t.fan_set_level = 1;
		     SetLevel_Fan_PWMA(98);
	    }
		else{
			 run_t.fan_set_level = 2;

		     SetLevel_Fan_PWMA(100);

		}
    }
	//driver bug
	if(run_t.gUltrasonic ==1){
	
	   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}
	
//    //shut off function and after 1 minutes shut off wind fan 
//	if(run_t.gPlasma ==0 && run_t.gDry==0 && run_t.fan_start_shut_off_flag ==0 ){
//
//        run_t.gFan_counter=0;
//		run_t.fan_start_shut_off_flag++ ;
//		run_t.gFan_continueRun=1;        
//
//	}
	
		



}

