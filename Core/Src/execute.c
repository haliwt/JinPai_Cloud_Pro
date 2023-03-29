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
    run_t.gFan_continueRun =0;
    run_t.gPower_On=POWER_ON;
	run_t.gmt_time_flag=0;
	run_t.wifi_gPower_On = 1;

    run_t.gFan = 1;
	run_t.gDry = 1;
	run_t.gPlasma =1;       //"杀菌"
	run_t.gUltrasonic = 1; // "驱虫"
	
    run_t.gFan_counter=0;
	
    FAN_CCW_RUN();
    PLASMA_SetHigh(); //
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
    PTC_SetHigh();
	
 }

void SetPowerOff_ForDoing(void)
{
   
	run_t.gPower_flag = 0;
	run_t.gFan_continueRun =1; //the fan still run 60s
	run_t.gPower_On = POWER_OFF;
	run_t.wifi_gPower_On = 0;
 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUltrasonic = 0; // "驱虫"
	
	
    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	FAN_Stop();

}



void ActionEvent_Handler(void)
{
     
	if(run_t.gDry == 1){
	
        PTC_SetHigh();
	     
	}
	else{
	   PTC_SetLow();

	}
	//kill
	if(run_t.gPlasma == 1){
	     PLASMA_SetHigh();
	}
	else{

		PLASMA_SetLow();
	}

	if(run_t.set_wind_speed_value==0){
	      Fan_Slowly_Speed();
    }
	else{

	     FAN_CCW_RUN();

	}
	//driver bug
	if(run_t.gUltrasonic ==1){
	
	   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}
    //shut off function and after 1 minutes shut off wind fan 
	if(run_t.gPlasma ==0 && run_t.gDry==0 && run_t.gUltrasonic ==0){

        run_t.gFan_counter=0;
		run_t.gFan_continueRun=1;        

	}
		



}

