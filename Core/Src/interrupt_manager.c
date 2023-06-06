#include "interrupt_manager.h"
#include "run.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "cmd_link.h"
#include "usart.h"


void USART2_Receive_Interrupt_Data(void)
{
	

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

  
  
}

/**
  * Function Name: void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  * Function: Tim3 interrupt call back function
  * Tim3 timer :timing time 10ms
  * 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t tm0,tm1,tm2 ;
    if(htim->Instance==TIM14){
		
	   tm0 ++ ;
       run_t.gTimer_senddata_panel++;
	 if(tm0 > 99){//100ms *10 = 1000ms =1s
        tm0 =0;
		tm1 ++ ;
	    tm2++;
		run_t.gFan_counter++;

        run_t.gTimer_1s ++;
	    run_t.gTimer_10s++;
		
     
	
	   run_t.gTimer_send_cloud_state++;
	 


	   //usart 1
	    run_t.gTimer_check_iwdg_flag++;
	   run_t.gTimer_iwdg++;
	   run_t.gTimer_usart_error++;
	   run_t.gTimer_fan_oneselt_test++;
		
	  if(tm1 >59){ //minutes
	  	tm1=0;
	    run_t.gTimer_ptc_adc_times ++; 
        run_t.gTimer_fan_adc_times++ ;
	  }

	  if(tm2 > 46){ //46s 
		tm2=0;


	  }
	  
	   
	 
	}
 	}
 }
