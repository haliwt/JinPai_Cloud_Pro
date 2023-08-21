#include "self_check.h"
#include "run.h"
#include "esp8266.h"
#include "publish.h"
#include "cmd_link.h"
#include "wifi_fun.h"
#include "adc.h"
#include "buzzer.h"
#include "fan.h"

void (*Self_CheckFan_Handler)(uint32_t channel,uint8_t times);



/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void Self_Check_Fain_Init(void)
{

  Self_Check_Fan_Handler(Get_Fan_Adc_Fun);//


}


/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
     static uint8_t self_power_on_flag=0, the_first_power_on =0,fan_error_flag;
	if(run_t.first_power_on_flag==0){
         run_t.gTimer_ptc_adc_times=0;
       WIFI_IC_ENABLE();
       HAL_Delay(1000);
      if(usart_wifi_t.usart_wifi_receive_read_data_flag==1){
		usart_wifi_t.usart_wifi_receive_read_data_flag=0;
       	usart_wifi_t.usart_wifi_start_receive_flag=0;
		usart_wifi_t.usart_wifi_receive_success_flag=0;
	  //  run_t.first_power_on_flag++ ;
        HAL_Delay(200);
		Publish_Data_ProdKey();	
		HAL_Delay(200);
	   
		 
	  }
	
      run_t.first_power_on_flag++;
	  run_t.open_fan_works_flag=1;
     }

    if(run_t.first_power_on_flag==1){

	
        
     switch(run_t.recoder_wifi_link_cloud_flag){

	   case 1: 
	   	  run_t.gTimer_ptc_adc_times=0;
	   	run_t.recoder_wifi_link_cloud_flag++;
		run_t.first_power_on_flag++;
		wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
	     SendWifiData_To_Cmd(0x01) ;

		

	 break;

	 case 0:
            run_t.gTimer_ptc_adc_times=0;
	    
          if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_FAIL && run_t.first_power_on_flag < 2 ){
                 wifi_t.publish_send_state_data=0;
				run_t.first_power_on_flag++;
                Read_USART2_Wifi_Data(wifi_t.usart_wifi_frame_type,wifi_t.usart_wifi_frame_len,wifi_t.usart_wifi_order);
               
           }

           if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS && run_t.first_power_on_flag == 1  ){
			run_t.first_power_on_flag++ ;
			run_t.wifi_link_JPai_cloud = 1;
			//Buzzer_KeySound();
			SendWifiData_To_Cmd(0x01) ;
           }

		
		
    
	 break;
    }

	  if(run_t.gPower_On == POWER_OFF){
	    
			 run_t.first_power_on_flag= 0x0A;
             run_t.gTimer_fan_oneselt_test=0;
			wifi_t.gTimer_wifi_send_cloud_success_times=0;
			run_t.gPower_On=POWER_OFF;
			run_t.gPower_flag = POWER_OFF;
			run_t.RunCommand_Label = POWER_OFF;

			esp8266_t.esp8266_config_wifi_net_label=0;
			
		     Buzzer_KeySound();

		 }
		  if(the_first_power_on==0)run_t.open_fan_works_flag=1;
          run_t.gTimer_ptc_adc_times=0;

   } 
}
 

/**************************************************************
	*
	*Function Name:static void Self_Checking_Function(void)
	*
	*
	*
**************************************************************/
void Self_Check_Fan_Handler(void (* fanHandler)(uint32_t channel,uint8_t times))
{
   Self_CheckFan_Handler = fanHandler;

}




