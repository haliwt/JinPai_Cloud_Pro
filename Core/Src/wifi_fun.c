#include "wifi_fun.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
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
			
          Wifi_ReceiveData_Handler();
			 
          

		break;
        
        default:
            
        break;

		

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
static void Wifi_ReceiveData_Handler(void)
{
    
    uint8_t i;

	if(usart_wifi_t.usart_wifi_start_receive_flag==1){
       usart_wifi_t.usart_wifi_start_receive_flag=0;
    memcpy(wifi_t.wifi_dispose_data,usart_wifi_t.usart_wifi,  usart_wifi_t.usart_receive_numbers);
	
     wifi_t.usart_wifi_frame_type =wifi_t.wifi_dispose_data[0];
	 wifi_t.usart_wifi_sequence =wifi_t.wifi_dispose_data[1];
	 wifi_t.usart_wifi_order =wifi_t.wifi_dispose_data[2];
	 wifi_t.usart_wifi_model =wifi_t.wifi_dispose_data[3];
	 wifi_t.usart_wifi_state =wifi_t.wifi_dispose_data[4];

	 wifi_t.usart_wifi_cloud_state =wifi_t.wifi_dispose_data[5];

	 wifi_t.usart_wifi_signal_state =wifi_t.wifi_dispose_data[6];

	 wifi_t.usart_wifi_pass_state =wifi_t.wifi_dispose_data[8];

		}
    //wifi model answering signal 

	switch(wifi_t.usart_wifi_frame_type){

        case wifi_model_device_report:

        break;

        case wifi_model_device_issue:

        break;

        case wifi_model_operation:
        
              switch(wifi_t.usart_wifi_order){
                
                case wifi_model_state_query:

                    switch(wifi_t.usart_wifi_model){

                        case wifi_model_sta:

                             
                        wifi_receive_data_state_flag=wifi_run_state_fun();

                            return ;
                            
                        break;

                        case wifi_model_hekr_config:
                            wifi_receive_data_state_flag=wifi_run_state_fun();

                            return ;
                        break;

                        case wifi_model_compatible:
                          wifi_receive_data_state_flag=wifi_run_state_fun();
                    
                        return ;
                           

                        break;
                        

                    }

                break;

                case wifi_model_smartconfig_order:
                  wifi_receive_data_state_flag=wifi_run_state_fun();

                    return ;

                break;

                case wifi_model_prodkey_order:
                  wifi_receive_data_state_flag=wifi_run_state_fun();

                    return ;

                break;

            }


        break;

        case wifi_model_frame_error:

        break;

    }
    
    switch(wifi_receive_data_state_flag){

        case 1:

            wifi_t.wifi_receive_data_state =wifi_model_normal_link;
	        if(wifi_t.usart_wifi_pass_state == WIFI_PASS){
				 wifi_t.wifi_link_cloud =WIFI_SUCCESS;
				
            }
			else{
			   wifi_t.wifi_link_cloud =WIFI_FAIL;

            }

		 wifi_receive_data_state_flag=  wifi_model_exit;
        break;
        
        case 2:
            wifi_t.wifi_receive_data_state =wifi_model_link_fail;
			 wifi_receive_data_state_flag=  wifi_model_exit;
        break;

        case 3:
             wifi_t.wifi_receive_data_state = wifi_model_linking;
        break;
            
        case 4:
             wifi_t.wifi_receive_data_state =wifi_model_password_error;
			  wifi_receive_data_state_flag=  wifi_model_exit;
        break;
        
        case 5:
             wifi_t.wifi_receive_data_state =wifi_model_no_lookfor_router;
			  wifi_receive_data_state_flag=  wifi_model_exit;
        break;
        
        case 6:
             wifi_t.wifi_receive_data_state =wifi_model_link_times_overflow;
			  wifi_receive_data_state_flag=  wifi_model_exit;
        break;
        
        case 7:
             wifi_t.wifi_receive_data_state =wifi_model_no_link_router;
			  wifi_receive_data_state_flag=  wifi_model_exit;
        break;


		case   wifi_model_exit:
		    for(i=25;i>=0;i--){
		       wifi_t.wifi_dispose_data[i]=0;
			   usart_wifi_t.usart_wifi[i]=0;

			}
          wifi_t.wifi_receive_data_state =0;
		break;

        default:
            wifi_t.wifi_receive_data_state =0;
        break;
  

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

