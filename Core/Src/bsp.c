#include "bsp.h"


void receive_copy_cmd(uint8_t cmd)
{
    switch(cmd){

	case copy_power_off:

	break;


	case copy_power_on:


	break;

	case copy_wifi_power_on:
     wifi_t.gTimer_wifi_send_cloud_success_times=0;
 	 run_t.gPower_flag = POWER_ON;
	 run_t.gPower_On = POWER_ON;
     run_t.RunCommand_Label= POWER_ON;
	
	wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
	run_t.app_appointment_time_power_on = WIFI_NORMAL_POWER_ON;
	wifi_t.wifi_open_power_on_flag =1;


	break;


	case copy_wifi_power_off:
    wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;

    wifi_t.gTimer_wifi_send_cloud_success_times=0;
    run_t.gPower_On=POWER_OFF;
    run_t.gPower_flag = POWER_OFF;
    run_t.RunCommand_Label = POWER_OFF;
    run_t.gFan_counter=0;
    wifi_t.gTimer_wifi_send_cloud_success_times=0;

	break;

	case copy_wifi_timer_power_on:

     run_t.RunCommand_Label = POWER_ON;
	break;







	}

}



