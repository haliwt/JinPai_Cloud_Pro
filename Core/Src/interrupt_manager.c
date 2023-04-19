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
             usart_wifi_t.usart_wifi[usart_wifi_t.usart_wifi_counter] = usart_wifi_t.usart_wifi_data[0];
			  usart_wifi_t.usart_wifi_counter++;
			

			if(usart_wifi_t.usart_wifi[1] > 30){
			    usart_wifi_t.usart_wifi_start_receive_flag=0;
                usart_wifi_t.usart_wifi_receive_success_flag=0;
			    usart_wifi_t.usart_wifi[1]=0;

			}

			
		  if(usart_wifi_t.usart_wifi_counter == (usart_wifi_t.usart_wifi[1])){

				//usart_wifi_t.usart_wifi[usart_wifi_t.usart_wifi_counter] = usart_wifi_t.usart_wifi_data[0];
					usart_wifi_t.usart_wifi_receive_success_flag=1;
			
			        USART2_WIFI_Receive_Data();
	
      }
     
			

	}

}
/*****************************************************************************************
	*
	*Function Name:static void Wifi_ReceiveData_Handler(void)
	*Function :receive from wifi usart of data 
	*Input Ref: NO
	*Return Ref:NO
	*
*
******************************************************************************************/
void USART2_WIFI_Receive_Data(void)
{

  
   if(usart_wifi_t.usart_wifi_receive_success_flag==1){
	//	usart_wifi_t.usart_wifi_start_receive_flag=0;
	//	usart_wifi_t.usart_wifi_receive_success_flag=0;
		usart_wifi_t.usart_wifi_receive_read_data_flag = 1;
		wifi_t.usart_wifi_frame_len =usart_wifi_t.usart_wifi[1];
		wifi_t.usart_wifi_frame_type = usart_wifi_t.usart_wifi[2];
		wifi_t.usart_wifi_sequence =usart_wifi_t.usart_wifi[3];
		wifi_t.usart_wifi_order =usart_wifi_t.usart_wifi[4];
		//judge wifi if link cloud net
		wifi_t.usart_wifi_model =usart_wifi_t.usart_wifi[5];
		wifi_t.usart_wifi_state =usart_wifi_t.usart_wifi[6];
		wifi_t.usart_wifi_cloud_state =usart_wifi_t.usart_wifi[7];

		wifi_t.usart_wifi_signal_state =usart_wifi_t.usart_wifi[8];

		wifi_t.usart_wifi_pass_state =usart_wifi_t.usart_wifi[9];

		wifi_t.usart_wifi_seconds_value = usart_wifi_t.usart_wifi[10];
		
        wifi_t.usart_wifi_sum_codes = usart_wifi_t.usart_wifi[usart_wifi_t.usart_wifi_counter];

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
		run_t.gFan_counter++;

        run_t.gTimer_1s ++;
	    run_t.gTimer_10s++;
		
        wifi_t.gTimer_5s++;
	   wifi_t.gTimer_get_beijing_time++;
	   wifi_t.gTimer_beijing_time++;
	
	   run_t.gTimer_send_cloud_state++;
	 
	   wifi_t.gTimer_wifi_send_cloud_success_times++;

	   //usart 1
	    run_t.gTimer_check_iwdg_flag++;
	   run_t.gTimer_iwdg++;
	   run_t.gTimer_usart_error++;
	 
		run_t.gTimer_ptc_adc_times ++; 
	   
	 
		}
 	}
 }
