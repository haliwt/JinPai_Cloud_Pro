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
#include "subscribe.h"
#include "flash.h"


WIFI_FUN   wifi_t;
uint8_t usart_len;
uint8_t sum_codes;

uint8_t (*wifi_run_state_fun)(void);
uint8_t wifi_receive_data_state_flag;
uint8_t receive_usart_wifi_data;
static void Wifi_ReceiveData_Handler(void);
/*********************************************************
 * 
  * @brief 
  * @param 
  * @return 
  * @note 
  *********************************************************/
void RunWifi_Command_Handler(uint8_t command)
{
    static uint8_t times;
    static uint8_t recoder_times,publish_init_flag,repeat_times,repeat_send_times,pub_times ;

	if(run_t.gPower_On == POWER_ON ){
    switch(command){

        case wifi_start_link_net://0x02

             //wifi gpio 13 pull down 5s 
            Publish_Command_SmartCofnig();
            esp8266_t.esp8266_config_wifi_net_label = wifi_smartconfig_model;
           
          
      
        break;

        case wifi_smartconfig_model: //0x03

          if(wifi_t.gTimer_5s > 5 && repeat_times ==0){
              wifi_t.gTimer_5s =0;
              repeat_times++;
              Publish_Data_ProdKey();
             
          }
          if(wifi_t.wifi_receive_data_error==1){ //is error 

             if(repeat_send_times >4){//exit smart config 
                repeat_send_times=0;
                esp8266_t.esp8266_config_wifi_net_label= wifi_null;
             }
           
		  
          }
          else{
                 esp8266_t.esp8266_config_wifi_net_label = wifi_receive_data;
                  wifi_t.gTimer_5s =0;
                 repeat_times=0;
                 repeat_send_times=0;
          }

          if(wifi_t.gTimer_5s > 10 && repeat_send_times < 5){
             wifi_t.gTimer_5s =0;
             repeat_send_times++;
             Publish_Data_ProdKey();
            
          }
          

        break;

		case wifi_receive_data: //0x04
          if(wifi_t.gTimer_5s > 20){
            wifi_t.gTimer_5s=0;
            Publish_Command_Query();
			 
          }
          if(wifi_t.wifi_link_JPai_cloud==1){

			    esp8266_t.esp8266_config_wifi_net_label= wifi_publish_init_ref;
		        wifi_t.gTimer_5s=0;

		   }
		  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
		  	
		      esp8266_t.esp8266_config_wifi_net_label=  wifi_null;

		  }

		break;

		case wifi_publish_init_ref://5
		      if(publish_init_flag ==0){
		        publish_init_flag++;
		        Init_Publisher_Data_Ref();
		        wifi_t.gTimer_5s=0;
		      }
			  if(wifi_t.gTimer_5s > 10 &&   pub_times < 3){
			  	  wifi_t.gTimer_5s=0;
			     Publish_Data_AllRef();
			     pub_times ++;

    	    }

			   if( pub_times > 2){
			   if(wifi_t.wifi_receive_data_error==1){ //is error 
			       if(pub_times >2){ //exit this publish sed data over.
			            pub_times=0;
			            esp8266_t.esp8266_config_wifi_net_label= wifi_null;
			            wifi_t.gTimer_5s=0;
			       	}

			      }
				  
			      if(wifi_t.wifi_receive_data_error==0){
			        esp8266_t.esp8266_config_wifi_net_label= wifi_subscribe_data;
			        pub_times=0;
			         publish_init_flag=0;
                wifi_t.gTimer_5s=0;
			      }

				}
		break;

    case wifi_subscribe_data://6

	

    break;

		case wifi_null://7
		  //    if(wifi_t.gTimer_5s > 30){
			//  	wifi_t.gTimer_5s =0;
			//  	Publish_Data_AllRef();
			// }
		   if(wifi_t.gTimer_5s > 60){
            wifi_t.gTimer_5s =0;
          	Publish_Command_Query();
			 }
		break;
        
        default:
            
        break;

		

      }

    if(usart_wifi_t.usart_wifi_receive_read_data_flag==1){
      

	  
		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			
            Publish_Return_Repeat_Data();
			wifi_t.publish_send_state_data=1;
             
         }

		
	    if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_FAIL ||  wifi_t.publish_send_state_data==1){
			  wifi_t.publish_send_state_data=0;
			
             Read_USART2_Wifi_Data(wifi_t.usart_wifi_frame_type,wifi_t.usart_wifi_frame_len,wifi_t.usart_wifi_order);
            receive_usart_wifi_data = 1;
		}

		usart_wifi_t.usart_wifi_receive_read_data_flag=0;
       	usart_wifi_t.usart_wifi_start_receive_flag=0;
		usart_wifi_t.usart_wifi_receive_success_flag=0;
		

    }
//	if(run_t.first_power_on_flag ==1 && receive_usart_wifi_data ==1){
//	       
//           receive_usart_wifi_data ++;
//           Publish_Data_ProdKey();
//
//	   }

//    if(wifi_t.publish_send_state_data ==1){
//		 wifi_t.publish_send_state_data=0;
//	
//         HAL_Delay(200);
//		 Example_Publish_State();
//
//	}
	

   }
}



void Read_USART2_Wifi_Data(uint8_t type,uint8_t len,uint8_t order)
{

  switch(type){

    case 0xFE: //frame type
      if(len != 0x0D){
            if( wifi_t.usart_wifi_model==1 && wifi_t.usart_wifi_state==1 &&  wifi_t.usart_wifi_cloud_state==1){

              wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
			  run_t.wifi_link_JPai_cloud = 1;
			   SendWifiData_To_Cmd(0x01) ;

            }
        }
      if(len == 0x0D){

        wifi_t.BJ_time_hours = wifi_t.usart_wifi_signal_state;
        wifi_t.BJ_time_minutes =   wifi_t.usart_wifi_pass_state;
        wifi_t.BJ_time_seconds  =  wifi_t.usart_wifi_seconds_value;
		    SendData_Real_GMT(wifi_t.BJ_time_hours,wifi_t.BJ_time_minutes, wifi_t.BJ_time_seconds );
		  }
      wifi_t.wifi_receive_data_error = 0;
   break;
   
   case 0x02: //device answering from wifi model from command 
         switch(len){

            case 0x06:
               //Subscribe_Data_QueryDev();
            break;

            case 0x07:
                switch(order){ //order command from
                    case 0x02: //set Power on or off

                         if(wifi_t.usart_wifi_model ==0){
                            Subscribe_Data_PowerOff();
                         }
                         else{
                            Subscribe_Data_PowerOn();
							Example_Publish_State();
							
                         }

                    break;

                    case 0x05://dry on or off
                         if(wifi_t.usart_wifi_model ==0){
                            Subscribe_Data_PtcOff();
                         }
                         else{
                            Subscribe_Data_PtcOn();
                         }
                    break;

                    case 0x07: //sterilization on or off
                         if(wifi_t.usart_wifi_model ==0){
                            Subscribe_Data_SterilizationOff();
                         }
                         else{
                            Subscribe_Data_SterilizationOn();
                         }
                    break;

                    case 0x09: //ultrasonic on or off 
                        if(wifi_t.usart_wifi_model ==0){
                            Subscribe_Data_UltrasonicOff();
                         }
                         else{
                          Subscribe_Data_UltrasonicOn();
                         }

                    break;

                    case 0x0b : //fan speed hihg or low
                         if(wifi_t.usart_wifi_model ==0){
                            Subscribe_Data_FanSpeedHigh();
                         }
                         else{
                            Subscribe_Data_FanSpeedLow();
                         }
                        
                    break;

                    case 0x0e: //set up temperature value
                         
                        Subscribe_Data_SetTemperatureValue(wifi_t.usart_wifi_model);
                         
                    break;

                    case 0x0f: //set timer timing
                        Subscribe_Data_SetTimerValue(wifi_t.usart_wifi_model);
                    break;



              }
              
            break;

            case 0x0b: // set order status from
               Subscribe_Data_AppointmentTime(wifi_t.usart_wifi_model, wifi_t.usart_wifi_state, wifi_t.usart_wifi_cloud_state, wifi_t.usart_wifi_signal_state,wifi_t.usart_wifi_pass_state);
            break;

         

         }
   break;

   case 0xFF:
      wifi_t.wifi_receive_data_error = 1;

   break;
  }


}




void Wifi_Model_State_Handler(uint8_t (*wifi_state_sepical_fun)(void))
{
       wifi_run_state_fun = wifi_state_sepical_fun;
       
     
}

