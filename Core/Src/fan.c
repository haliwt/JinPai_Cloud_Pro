#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"




void FAN_CCW_RUN(void)
{
   FAN_CW_SetLow();
   FAN_CCW_SetHigh(); //PA6
  
}

void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_CW_SetLow(); //brake
}


void Fan_Slowly_Speed(void)
{
    static uint16_t fan_speed;
	//static uint16_t  fan_dec;
	fan_speed ++ ;
	
	FAN_CW_SetLow();

	if(fan_speed ==1)  FAN_CCW_SetLow(); //brake//Run fan//brake/Run fan //Run fan//brake //Run fan //brake //Run fan
	
	if(fan_speed % 2 ==0 ){
	   FAN_CCW_SetHigh(); //Run fan
    }
    if( fan_speed % 3 ==0 ){
  
	  FAN_CCW_SetLow(); //brake
	  
	}

	if(fan_speed > 10000){
		fan_speed =0;
	    
	     FAN_CCW_SetLow(); //brake
	}
	
}



void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}
//"杀毒" 
void SterIlization(uint8_t sel)
{
    if(sel==0){//open 
		 
		 PLASMA_SetHigh();
		

	}
	else{ //close

		PLASMA_SetLow();
	

	}



}

void Dry_Function(uint8_t sel)
{
   if(sel ==0){

  

		PTC_SetHigh();

   }
   else{

       PTC_SetLow();

   }

}



