#include "wifi_fun.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "esp8266.h"
#include "publish.h"
#include "dht11.h"
#include "usart.h"

#include "flash.h"


WIFI_FUN   wifi_t;
uint8_t usart_len;
uint8_t sum_codes;

uint8_t (*wifi_run_state_fun)(void);
uint8_t wifi_receive_data_state_flag;

static void Wifi_ReceiveData_Handler(void);
void RunWifi_Command_Handler(uint8_t command)
{
    
    static uint8_t recoder_times;
    switch(command){

        case wifi_start_link_net://0x02

             //wifi gpio 13 pull down 5s 
            Publish_Command_SmartCofnig();
            esp8266_t.esp8266_config_wifi_net_label = wifi_smartconfig_model;
            usart_wifi_t.usart_wifi_start_receive_flag=0; //start receive wifif usart data
          
      
        break;

        case wifi_smartconfig_model: //0x03

          //  Wifi_ReceiveData_Handler();
          if(wifi_t.gTimer_5s > 5){
              wifi_t.gTimer_5s =0;
              Publish_Data_ProdKey();
              esp8266_t.esp8266_config_wifi_net_label = wifi_receive_data;
          }

        break;

		case wifi_receive_data: //0x04
  

          if(wifi_t.gTimer_5s > 10){
            wifi_t.gTimer_5s=0;
              usart_wifi_t.usart_wifi_counter=0;
				usart_wifi_t.usart_wifi_start_receive_flag=0;
              usart_wifi_t.usart_wifi_receive_success_flag=0;
             Publish_Command_Query();
			 
          }
          

		break;

		case wifi_null:

		break;
        
        default:
            
        break;

		

      }


	  Receive_Wifi_Data(wifi_t.usart_wifi_frame_len);


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
       usart_wifi_t.usart_wifi_start_receive_flag=0;
       usart_wifi_t.usart_wifi_receive_success_flag=0;
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

	/// wifi_t.usart_wifi_sum_codes = usart_wifi_t.usart_wifi[wifi_t.usart_wifi_frame_len-1];

	
    }
}

void Receive_Wifi_Data(uint8_t cmd)
{

  
   

   if(cmd != 0x0D){
			   if( wifi_t.usart_wifi_model==1 && wifi_t.usart_wifi_state==1 &&  wifi_t.usart_wifi_cloud_state==1){

					 wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;

			   }
	   }
   

   if(cmd == 0x0D){

		wifi_t.BJ_time_hours = wifi_t.usart_wifi_signal_state;
		wifi_t.BJ_time_minutes =   wifi_t.usart_wifi_pass_state;
        wifi_t.BJ_time_seconds  =  wifi_t.usart_wifi_seconds_value;


   }

}

/**************************************************************
 * 
 * 
 * 
 * 
**************************************************************/
uint8_t Wifi_State_Special_Fun(void)
{
     switch(wifi_t.usart_wifi_state){

        case wifi_model_normal_link:
          // wifi_receive_data_state_flag=1;
            return 1;
        break;

        case wifi_model_link_fail:
           // wifi_receive_data_state_flag=2;
            return  2;
        break;

        case wifi_model_linking:
           // wifi_receive_data_state_flag=3;
            return  3;
        break;

        case wifi_model_password_error:
          //  wifi_receive_data_state_flag=4;
            return 4;
        break;

        case wifi_model_no_lookfor_router:
          //  wifi_receive_data_state_flag=5;
            return 5;
        break;

        case wifi_model_link_times_overflow:
          //  wifi_receive_data_state_flag=6;
            return 6;
        break;

        case wifi_model_no_link_router:
          //  wifi_receive_data_state_flag=7;
            return 7;
        break;



    }
                            

}











void Wifi_Model_State_Handler(uint8_t (*wifi_state_sepical_fun)(void))
{
       wifi_run_state_fun = wifi_state_sepical_fun;
       
     
}

