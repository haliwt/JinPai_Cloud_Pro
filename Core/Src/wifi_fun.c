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
#include "buzzer.h"


WIFI_FUN   wifi_t;
uint8_t usart_len;
uint8_t sum_codes;

uint8_t (*wifi_run_state_fun)(void);
uint8_t wifi_receive_data_state_flag;
uint8_t receive_usart_wifi_data;
static void Wifi_ReceiveData_Handler(void);
/*********************************************************
 * 
  * @brief :void RunWifi_Command_Handler(uint8_t command)
  * @param 
  * @return 
  * @note 
  * 
  *********************************************************/
void RunWifi_Command_Handler(uint8_t command)
{
    static uint8_t repeat_times,repeat_send_times,publish_init_flag,pub_times,send_times;

	  if(run_t.first_power_on_flag== 0x0A){
    	switch(command){

        case wifi_start_link_net://0x02

             //wifi gpio 13 pull down 5s 
            Publish_Command_SmartCofnig();
            HAL_Delay(500);
            esp8266_t.esp8266_config_wifi_net_label = wifi_smartconfig_model;
           
          
      
        break;

        case wifi_smartconfig_model: //0x03

          if(repeat_times ==0){
              repeat_times++;
              Publish_Data_ProdKey();
              HAL_Delay(400);
             esp8266_t.esp8266_config_wifi_net_label=wifi_receive_data;
			 wifi_t.gTimer_5s=0;
          }
		  
          if(wifi_t.wifi_receive_data_error==1){ //is error 

             if(repeat_send_times >4){//exit smart config 
                repeat_send_times=0;
                esp8266_t.esp8266_config_wifi_net_label= wifi_receive_data;
             }
           
		  
          }


        break;

		case wifi_receive_data: //0x04
          if(wifi_t.gTimer_5s > 10){
            wifi_t.gTimer_5s=0;
            Publish_Command_Query();
            HAL_Delay(400);
			 
          }
          if(wifi_t.wifi_link_JPai_cloud==1){

			    esp8266_t.esp8266_config_wifi_net_label= wifi_publish_init_ref;
		        wifi_t.gTimer_5s=0;

		   }
		  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
		  	
		      esp8266_t.esp8266_config_wifi_net_label=  wifi_publish_update_data;
			   run_t.gTimer_send_cloud_state=0;

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
			      Publish_Reference_Update_State();
            		HAL_Delay(400);
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

		case wifi_publish_update_data://7
            if(run_t.gTimer_send_cloud_state > 119){
                run_t.gTimer_send_cloud_state=0;
                Dht11_Read_TempHumidity_Handler(&DHT11);
                Publish_Reference_Update_State();
                HAL_Delay(200);

			}
			if(wifi_t.gTimer_detect_wifi_donot > 125){
			   wifi_t.gTimer_detect_wifi_donot =0;
               Publish_Command_Query();
               HAL_Delay(200);
			}

			if(wifi_t.detect_wifi_sig_flag ==  hasnot_wifi_sig ){
                   wifi_t.detect_wifi_sig_flag=donot_detect_wifi_sig;
              wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
             SendWifiData_To_Cmd(0x00) ; //don't wifi single
		     HAL_Delay(100);
			}
			
			if(wifi_t.detect_wifi_sig_flag ==  has_wifi_sig){
			   wifi_t.detect_wifi_sig_flag=donot_detect_wifi_sig;

			    wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_SUCCESS;
				SendWifiData_To_Cmd(0x01) ; //has  wifi single
				HAL_Delay(100);


			} 
		
		break;
        
        default:
            
        break;

		

      }

    	if(usart_wifi_t.usart_wifi_receive_read_data_flag==1){
      
		usart_wifi_t.usart_wifi_receive_read_data_flag=0;
       	usart_wifi_t.usart_wifi_start_receive_flag=0;
		usart_wifi_t.usart_wifi_receive_success_flag=0;
	  
		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			if(wifi_t.usart_wifi_frame_len < 0x0d ){

					
					Publish_Return_Repeat_Data();
                    HAL_Delay(250);
                   wifi_t.publish_send_state_data=1;
		    } 
			else{

				wifi_t.publish_send_state_data=1;
			} 
         }

		
	    if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_FAIL ||  wifi_t.publish_send_state_data==1){
			  wifi_t.publish_send_state_data=0;
			
             Read_USART2_Wifi_Data(wifi_t.usart_wifi_frame_type,wifi_t.usart_wifi_frame_len,wifi_t.usart_wifi_order);
            receive_usart_wifi_data = 1;
		}

		
		

    	}
    
		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS && run_t.gPower_On==POWER_ON && send_times ==0 ){
			 send_times++;
			 SendWifiCmd_To_Order(WIFI_POWER_ON);
			 HAL_Delay(200);
			 SendWifiData_To_Cmd(0x01) ;
		     HAL_Delay(100);


		}
   
	  	}
}




/********************************************************************************
 * 
  * @brief :void Read_USART2_Wifi_Data(uint8_t type,uint8_t len,uint8_t order)
  * @param 
  * @return 
  * @note 
  * 
  *******************************************************************************/
void Read_USART2_Wifi_Data(uint8_t type,uint8_t len,uint8_t order)
{
   static uint8_t receive_data_error;
  switch(type){

    case 0xFE: //frame type

      switch(len){

	  case 0x0B:
           switch(wifi_t.usart_wifi_model){

		   case 0:

		   break;

		   case 1:
               if(wifi_t.wifi_link_JPai_cloud == WIFI_CLOUD_FAIL || wifi_t.detect_wifi_sig_flag == hasnot_wifi_sig){
	               if(wifi_t.usart_wifi_state==1 &&  wifi_t.usart_wifi_cloud_state==1){

		              wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
					  
					  wifi_t.wifi_has_been_link_cloud = WIFI_CLOUD_SUCCESS;
				      wifi_t.detect_wifi_sig_flag =  has_wifi_sig; 
					
					  wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_SUCCESS;
					   SendWifiData_To_Cmd(0x01) ;
					   HAL_Delay(100);

	              }
				  else{
						
					 wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
					 wifi_t.detect_wifi_sig_flag =  hasnot_wifi_sig;
				     run_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
				      SendWifiData_To_Cmd(0x00) ;
					   HAL_Delay(100);

				  }
              }
		   break;
          }
        
	  break;

	  case 0x0D:
     

        wifi_t.BJ_time_hours = wifi_t.usart_wifi_signal_state;
        wifi_t.BJ_time_minutes =   wifi_t.usart_wifi_pass_state;
        wifi_t.BJ_time_seconds  =  wifi_t.usart_wifi_seconds_value;
		 SendData_Real_GMT(wifi_t.BJ_time_hours,wifi_t.BJ_time_minutes, wifi_t.BJ_time_seconds );
		

	  break;
      
      }
	  wifi_t.wifi_receive_data_error = 0;
   break;
   
   case 0x02: //device answering from wifi model from command 
         switch(len){

            case 0x06:
              Publish_Reference_Update_State();
              HAL_Delay(300);
            break;

            case 0x07:
                switch(order){ //order command from
                    case 0x02: //set Power on or off

                         if(wifi_t.usart_wifi_model ==0){
							 wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;

						     wifi_t.gTimer_wifi_send_cloud_success_times=0;
					        run_t.gPower_On=POWER_OFF;
					        run_t.gPower_flag = POWER_OFF;
					        run_t.RunCommand_Label = POWER_OFF;
						 	Buzzer_KeySound();
							wifi_t.gTimer_wifi_send_cloud_success_times=0;
                            Publish_Power_OFF_State();
							HAL_Delay(300);
							SendWifiCmd_To_Order(WIFI_POWER_OFF);
							 HAL_Delay(100);
						 
						     
                         }
                         else{ //Power on 
                             wifi_t.gTimer_wifi_send_cloud_success_times=0;
                         	 run_t.gPower_flag = POWER_ON;
							 run_t.gPower_On = POWER_ON;
					         run_t.RunCommand_Label= POWER_ON;
							
							wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_SUCCESS;
							run_t.app_appointment_time_power_on = WIFI_NORMAL_POWER_ON;
                            Buzzer_KeySound();
						    Publish_Power_ON_State();
							HAL_Delay(300);
							SendWifiCmd_To_Order(WIFI_POWER_ON_NORMAL);
							 HAL_Delay(100);
							esp8266_t.esp8266_config_wifi_net_label=wifi_publish_update_data;
							 
							}

                    break;

                    case 0x05://dry on or off
                         if(wifi_t.usart_wifi_model ==0){
							run_t.gDry = 0;
                            Buzzer_KeySound();
							  Publish_Reference_Update_State();
							HAL_Delay(300);
							SendWifiCmd_To_Order(WIFI_PTC_OFF);
							 HAL_Delay(100);
                         }
                         else{
							run_t.gDry=1;
						 	Buzzer_KeySound();
							  Publish_Reference_Update_State();
							HAL_Delay(300);
							 SendWifiCmd_To_Order(WIFI_PTC_ON);
							 HAL_Delay(100);
                         }
                    break;

                    case 0x07: //sterilization on or off
                         if(wifi_t.usart_wifi_model ==0){
						 	run_t.gPlasma=0;
							Buzzer_KeySound();
                            Publish_Reference_Update_State();
						    HAL_Delay(300);
							SendWifiCmd_To_Order(WIFI_KILL_OFF);
							 HAL_Delay(100);
								
                         }
                         else{
						  run_t.gPlasma=1;
						  Buzzer_KeySound();
                          Publish_Reference_Update_State();
						  HAL_Delay(300);
						  SendWifiCmd_To_Order(WIFI_KILL_ON);
						   HAL_Delay(100);
                         }
                    break;

                    case 0x09: //ultrasonic on or off 
                        if(wifi_t.usart_wifi_model ==0){
							run_t.gUltrasonic =0;
							Buzzer_KeySound();
                           Publish_Reference_Update_State();
						   HAL_Delay(300);
							SendWifiCmd_To_Order(WIFI_ULTRASONIC_OFF);
							 HAL_Delay(100);
                         }
                         else{
						 	run_t.gUltrasonic =1;
						 	Buzzer_KeySound();
                             Publish_Reference_Update_State();
						   HAL_Delay(300);
						   SendWifiCmd_To_Order(WIFI_ULTRASONIC_ON);
						    HAL_Delay(100);
                         }

                    break;

					case 0x0B: //wind  speed control
                        
							run_t.set_wind_speed_value = wifi_t.usart_wifi_model;
							Buzzer_KeySound();
							
						    Publish_Reference_Update_State();
						    HAL_Delay(300);
						 
                         
                         

                    break;



                    case 0x0e : // set up temperature sensor value
							Buzzer_KeySound();
                        run_t.set_temperature_value = wifi_t.usart_wifi_model;
					
					    
					    Publish_Reference_Update_State();
						HAL_Delay(300);
						SendWifiData_To_PanelTemp(run_t.set_temperature_value);
						 HAL_Delay(100);
                        
                    break;

					case 0x0F : // set up timer timing value
                      	Buzzer_KeySound();
                        run_t.set_timer_timing_value = wifi_t.usart_wifi_model;
						
						Publish_Reference_Update_State();
						HAL_Delay(300);
						SendWifiData_To_PanelTime(run_t.set_timer_timing_value);
						 HAL_Delay(100);
						
                    break;

					



              }
              
            break;

            case 0x0b: // set order status from
				Publish_Reference_Update_State();
				
				run_t.gPower_On = wifi_t.usart_wifi_model;
			    run_t.gUltrasonic = wifi_t.usart_wifi_state;
				run_t.gDry = wifi_t.usart_wifi_cloud_state;
				run_t.gPlasma = wifi_t.usart_wifi_signal_state;
				run_t.set_timer_timing_value = wifi_t.usart_wifi_pass_state;
			 //   run_t.set_wind_speed_value = wifi_t.usart_wifi_fan_speed_value;
				HAL_Delay(300); 
				if(run_t.gPower_On == POWER_ON){
					Buzzer_KeySound();
					run_t.app_appointment_time_power_on = POWER_ON;
					run_t.RunCommand_Label = POWER_ON;
				}
				else   run_t.RunCommand_Label = POWER_OFF;
              
            break;

         

         }
   break;

   case 0xFF:
   	  Buzzer_KeySound();
	  HAL_Delay(200);
      Buzzer_KeySound();
	  HAL_Delay(200);
      Buzzer_KeySound();
	  HAL_Delay(200);
	 
	
       if(wifi_t.usart_wifi_order==0x02){
			wifi_t.detect_wifi_sig_flag = hasnot_wifi_sig ;
			wifi_t.wifi_receive_data_error = 1;
		}      
	  
	 

   break;

   case 0x01: //device report to APP type
        switch(wifi_t.usart_wifi_model){

		    case 0:
			    wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
		        wifi_t.detect_wifi_sig_flag =  hasnot_wifi_sig;
			   run_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
 
		    break;

		    case 1:

			    switch(wifi_t.usart_wifi_state){

				   case 0:
				      wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;
				      wifi_t.detect_wifi_sig_flag =  hasnot_wifi_sig;
				    run_t.wifi_link_JPai_cloud = WIFI_CLOUD_FAIL;

				   break;

				   case 1:
				   	  if(wifi_t.usart_wifi_cloud_state==0x01){
					  	wifi_t.detect_wifi_sig_flag =  has_wifi_sig; 
						wifi_t.wifi_link_JPai_cloud = WIFI_CLOUD_SUCCESS;
					   run_t.wifi_link_JPai_cloud = WIFI_CLOUD_SUCCESS;
    

					  }

				   break;


				}

			break;
	      
			
	   	    

        }
        
   break;
  }


}




void Wifi_Model_State_Handler(uint8_t (*wifi_state_sepical_fun)(void))
{
       wifi_run_state_fun = wifi_state_sepical_fun;
       
     
}

