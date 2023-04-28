#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"
#include "adc.h"



#if 0
void FAN_CCW_RUN(void)
{
   FAN_CW_SetLow();
   FAN_CCW_SetHigh(); //PA6
  
}
#endif 
void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    HAL_TIM_PWM_Stop(&htim16,TIM_CHANNEL_1);
    
}

#if 0
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

#endif 

void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}

void Fan_Run_Fun(void)
{
	if(run_t.set_wind_speed_value==0){
		    run_t.fan_set_level = 1;
		   SetLevel_Fan_PWMA(98);
	   }
	   else{
			 run_t.fan_set_level = 2;
		    SetLevel_Fan_PWMA(100);

		}
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
/********************************************************
*
*Function Name:void SetLevel_Fan_PWMA(uint8_t levelval)
*Function: 
*
*
********************************************************/
void SetLevel_Fan_PWMA(uint8_t levelval)
{
     static uint8_t fan_default_value=0xff;
	 run_t.gFan_pwm_duty_level = levelval;
     FAN_CW_SetLow();
	 if(fan_default_value != run_t.fan_set_level){
	 	fan_default_value=run_t.fan_set_level;
	 	
	   MX_TIM16_Init();
	 }
	 HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
}


