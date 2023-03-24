#include "interrupt_manager.h"
#include "run.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "cmd_link.h"
#include "usart.h"


void USART2_WIFI_Receive_Interrupt_Data(void)
{
	 if(usart_wifi_t.usart_wifi_start_receive_flag==0){
             if(usart_wifi_t.usart_wifi_data[0]==0x48){
				usart_wifi_t.usart_wifi_counter=0;
				usart_wifi_t.usart_wifi_start_receive_flag=1;
				 
             }
			}
		
		if(usart_wifi_t.usart_wifi_start_receive_flag==1 && usart_wifi_t.usart_wifi_receive_success_flag==0){
		
			

			if(usart_wifi_t.usart_wifi[1] > 23){
			    usart_wifi_t.usart_wifi_start_receive_flag=0;


			}
		  else if(usart_wifi_t.usart_wifi_counter == (usart_wifi_t.usart_wifi[1] -1)){

				usart_wifi_t.usart_wifi[usart_wifi_t.usart_wifi_counter] = usart_wifi_t.usart_wifi_data[0];
					usart_wifi_t.usart_wifi_receive_success_flag=1;
			
			        USART2_WIFI_Receive_Data();
	
      }
      else{

        usart_wifi_t.usart_wifi[usart_wifi_t.usart_wifi_counter] = usart_wifi_t.usart_wifi_data[0];
			  usart_wifi_t.usart_wifi_counter++;
      }
			

	}

}


/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t tm0 ;
    if(htim->Instance==TIM14){
		
	   tm0 ++ ;
       run_t.gTimer_senddata_panel++;
	 if(tm0 > 99){//100ms *10 = 1000ms =1s
        tm0 =0;
        run_t.gTimer_1s ++;
	 run_t.gTimer_10s++;
     wifi_t.gTimer_5s++;
	   wifi_t.gTimer_get_beijing_time++;
	   wifi_t.gTimer_beijing_time++;
	
	   run_t.gTimer_send_cloud_state++;
	   run_t.gTimer_send_login_sucess++;
	  
	   if(run_t.gFan_continueRun ==1){
           run_t.gFan_counter++;
		 
	   }
      

	 }
 	}
 }
