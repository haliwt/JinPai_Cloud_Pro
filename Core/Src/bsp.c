#include "bsp.h"

process_t gpro_t;


void bsp_init(void)
{

	gpro_t.gPower_On = power_off;


}



/**********************************************************
*
*Function Name:void receive_copy_cmd(uint8_t cmd)
*Function : 
*
*
*
**********************************************************/
void receive_copy_cmd(uint8_t cmd)
{
  switch(cmd){

	case copy_power_off:

	break;


	case copy_power_on:


	break;

	case copy_wifi_power_on:
    
 	 
	 gpro_t.gPower_On = power_on;
     gpro_t.g_main_power_on_step=0;
	
	wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
	run_t.app_appointment_time_power_on = WIFI_NORMAL_POWER_ON;
	wifi_t.wifi_open_power_on_flag =1;
    gpro_t.gTimer_two_hours_counter =0;
    gpro_t.g_interval_times_flag=0;
	 gpro_t.gTimer_counter_minutes=0;
	break;


	case copy_wifi_power_off:
    wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;

  
    gpro_t.gPower_On=power_off;
	gpro_t.g_main_power_off_step=0;
 
    run_t.gFan_counter=0;
   
	gpro_t.gTimer_two_hours_counter =0;
     gpro_t.g_interval_times_flag=0;
	gpro_t.gTimer_counter_minutes=0;

	break;

	case copy_wifi_timer_power_on:

     gpro_t.gPower_On = power_on;
     gpro_t.g_main_power_on_step=0;
	 gpro_t.gTimer_two_hours_counter =0;
	 gpro_t.g_interval_times_flag=0;
	 gpro_t.gTimer_counter_minutes=0;
	break;

    }

}

/*******************************************************************
 *
 * Function Name:works_two_hours_handler()
 * Function:
 * Input Ref:NO
 * Return Ref:NO
 *
*******************************************************************/
void works_two_hours_handler(void)
{
  static uint8_t interval_fan_one_minute;

  if(gpro_t.gPower_On == power_on){

      if(gpro_t.gTimer_two_hours_counter > 119){//two hours
		   gpro_t.gTimer_two_hours_counter=0;

	       gpro_t.g_interval_times_flag=1;
	        interval_fan_one_minute =1;

             run_t.gFan_counter=0;


      }

	 if(gpro_t.g_interval_times_flag==1 &&  gpro_t.gTimer_two_hours_counter < 10){


	    if(interval_fan_one_minute ==1){

            if(run_t.gFan_counter <61){

			   SetLevel_Fan_PWMA(100);

			}
			else{
                 interval_fan_one_minute++;
			     FAN_Stop();
			    

			}

		}

        PTC_SetLow();
	    PLASMA_SetLow();
	
	    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

      }
	  else if(gpro_t.g_interval_times_flag==1 &&  gpro_t.gTimer_two_hours_counter > 10){

	       gpro_t.g_interval_times_flag =0;
	       gpro_t.gTimer_two_hours_counter=0;
	      

       }

	  }

}



