#include "run.h"
#include "wifi_fun.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "buzzer.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "flash.h"
#include "execute.h"
#include "publish.h"
#include "subscribe.h"
#include "adc.h"
#include "self_check.h"

RUN_T run_t; 

static void Display_Command_ReceiveData(uint8_t cmd);
static void Single_Command_ReceiveCmd(uint8_t cmd); 
static void Fan_ContinueRun_OneMinute_Fun(void);



uint8_t no_buzzer_sound_dry_off;
uint16_t receive_from_display_power_flag;
uint8_t receive_from_display_power_off_flag;



/**********************************************************************
*
*Function Name:void Decode_RunCmd(void)
*Function: receive usart touchkey of command 
*Input Ref:NO
*Return Ref:NO
*
**********************************************************************/
void Decode_RunCmd(void)
{
  uint16_t count_total_times;
  static uint8_t wifi_looking_for;
  uint8_t cmdType_1=inputCmd[0],cmdType_2 = inputCmd[1];

    
  switch(cmdType_1){
  
      case 'P': //power on and off
        run_t.display_cmd_link_wifi =0; //WT.edit 2023.08.16
      	Display_Command_ReceiveData(cmdType_2);  
           
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_flag==POWER_ON){
	      if(cmdType_2==1){//long press key that power key
              //fast blink led for link to tencent cloud
              SendWifiData_To_Cmd(0x52); //0x52= 'R' -> looking for wifi netware
             
              WIFI_IC_ENABLE();
              if(run_t.display_cmd_link_wifi ==0){
                run_t.display_cmd_link_wifi++;
			    Buzzer_KeySound();
              }
              SendWifiData_To_Cmd(0x52); //0x52= 'R' -> looking for wifi netware
			   wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_FAIL;
		       esp8266_t.esp8266_config_wifi_net_label=wifi_start_link_net;
	           wifi_t.gTimer_5s=0;
			   
		   }
		 }
       
	   break;
        
      case 'C':
           run_t.display_cmd_link_wifi =0; //WT.edit 2023.08.16
           if(run_t.gPower_flag==POWER_ON){
            
             Single_Command_ReceiveCmd(cmdType_2); //Single_ReceiveCmd(cmdType_2); 
              
           }
     
         
      break;

	  case 'M': //set up temperature value
	     
	  	if(run_t.gPower_flag==POWER_ON){
              
             run_t.set_temperature_value = cmdType_2;
			 if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
				Publish_Reference_Update_State();//
				HAL_Delay(300);
			 }
			    
			   
         }
	  break;

	  case 'T': //set up tiemr timing
	   
		  if(run_t.gPower_flag==POWER_ON){
           
			if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
				run_t.set_timer_timing_value =  inputCmd[1];
				count_total_times = run_t.set_timer_timing_value *60 ;
			   run_t.time_remaining_minutes_one = count_total_times >>8;
			   run_t.time_remaining_minutes_two =  count_total_times & 0xff;
			
				Publish_Reference_Update_State();//
				HAL_Delay(300);

			 }
			
			   
         }
	  break;

	  case 'O': //works how long times minute ?
          if(run_t.gPower_flag==POWER_ON){

		   
             if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			 	run_t.work_time_minutes_one =  inputCmd[1];
                run_t.work_time_minutes_two =  inputCmd[2];
                Publish_Reference_Update_State();//
                HAL_Delay(300);
		      }
        }
	  break;

	  case 'R': //remaining time minutes value ?
          if(run_t.gPower_flag==POWER_ON){

		     run_t.time_remaining_minutes_one = inputCmd[1];
			 run_t.time_remaining_minutes_two =  inputCmd[2];
		    if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
                Publish_Reference_Update_State();//
                HAL_Delay(300);
             }
	        }
      break;

	  
	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_flag==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
		        run_t.buzzer_sound_flag = 1;
			   // Buzzer_KeySound();
			}
			 
		
		}
     
	    break;
 	
}   
}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Display_Command_ReceiveData(uint8_t cmd)
{
  
 
  static uint8_t buzzer_power_on_sound,buzzer_power_Off_sound;
  static uint16_t first_power_off_flag;
    switch(cmd){


    case 0x01: // power on
         receive_from_display_power_flag++;
         receive_from_display_power_off_flag++;
         SendWifiData_To_Cmd(0x54); //0x54= 'R',receive order from display power on command copy a command 
         
         if(receive_from_display_power_flag !=buzzer_power_on_sound && first_power_off_flag !=1 ){ 
            first_power_off_flag++;
            buzzer_power_on_sound = receive_from_display_power_flag ;
         	Buzzer_KeySound();
         }
         SendWifiData_To_Cmd(0x54);
	
		
         run_t.RunCommand_Label= POWER_ON;
         wifi_t.wifi_open_power_on_flag =0;
		 esp8266_t.esp8266_config_wifi_net_label=0;

		 
	 cmd=0xff;  
     break;

    case 0x00: //power off


        SendWifiData_To_Cmd(0x53); //0x53= 'R' power off copy command from display power off
        if(first_power_off_flag==1)first_power_off_flag++;

        if(receive_from_display_power_off_flag !=buzzer_power_Off_sound){
            buzzer_power_Off_sound= receive_from_display_power_off_flag;
            Buzzer_KeySound();
        }
        SendWifiData_To_Cmd(0x53); //0x53= 'R' power off copy command from display power off
	    wifi_t.gTimer_wifi_send_cloud_success_times=0;
    
        run_t.RunCommand_Label = POWER_OFF;

        wifi_t.wifi_open_power_on_flag =0;
	
		

 

    cmd = 0xff;
    break;

   

    

     default:

     break;

    }

}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Command_ReceiveCmd(uint8_t cmd)
{

    
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	      no_buzzer_sound_dry_off=1;

       case DRY_ON:
         run_t.gDry = 1;
		 if(no_buzzer_sound_dry_off==0){
		  	    Buzzer_KeySound();
		  }
		  else no_buzzer_sound_dry_off=0;
			
		 if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
		    Publish_Reference_Update_State();//Publish_PTC_State();//Publish_PTC_ON_State();
            HAL_Delay(300);
	      }
		   
		 
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			run_t.gDry = 0;
			
			if(no_buzzer_sound_dry_off==0){
				Buzzer_KeySound();
            }
			else no_buzzer_sound_dry_off=0;
		
			
			if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
				Publish_Reference_Update_State();
			    HAL_Delay(300);
			}
			   
       break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
       	
			Buzzer_KeySound();
	   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
	        Publish_Reference_Update_State();
	        HAL_Delay(300);
	     
	   	}
	    
       break;

       case PLASMA_OFF:
           run_t.gPlasma=0;
		   Buzzer_KeySound();
	   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
	         Publish_Reference_Update_State();
            HAL_Delay(300);
	       
	      
	   	}
	   
       break;

       case ULTRASONIC_ON:
        
		  run_t.gUltrasonic =1;
		  Buzzer_KeySound();
		  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			  Publish_Reference_Update_State();
			 HAL_Delay(300);
		  }
		
		   
       break;

       case ULTRASONIC_OFF :
	   	    run_t.gUltrasonic =0;
	        Buzzer_KeySound();
		  
		   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			  Publish_Reference_Update_State();
			 HAL_Delay(300);
		   }
		 
		  
       break;

	   case WIFI_CONNECT_FAIL:

	       run_t.dp_link_wifi_fail =1;


	   break;


      default :
        cmd =0;

      break; 


    }



}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void SystemReset(void)
{
    if(run_t.gPower_flag ==POWER_ON){
		run_t.gPower_flag=0xff;
		run_t.gPower_On=POWER_ON;
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
    }

}

/**********************************************************************
	*
	*Functin Name: void  RunCommand_MainBoard_Fun(void)
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void RunCommand_MainBoard_Fun(void)
{

   static uint8_t power_just_on,power_off_fan_flag,the_first_dc_power_on;
   static uint8_t appointment_powr_onf_flag,power_off_step; 
    if(run_t.buzzer_sound_flag == 1){
	 	run_t.buzzer_sound_flag = 0;
	    Buzzer_KeySound();

	 }
  
   switch(run_t.RunCommand_Label){

	case POWER_ON: //1
	      power_off_step =0;
        if(wifi_t.wifi_open_power_on_flag ==1){
            wifi_t.wifi_open_power_on_flag =0;
            SendWifiCmd_To_Order(WIFI_POWER_ON); //display pannel power on 
            HAL_Delay(5);//HAL_Delay(20);
            SendWifiData_To_Cmd(0x54);
            HAL_Delay(5);//HAL_Delay(20);
             run_t.gTimer_run_process_times=0;
             appointment_powr_onf_flag=0;
        }
	    if(run_t.run_process_step ==0){
        run_t.run_process_step ++;  
	    SetPowerOn_ForDoing();
        run_t.gTimer_run_process_times=0;
		run_t.gPower_flag = POWER_ON;
		run_t.gPower_On = POWER_ON;
		esp8266_t.esp8266_config_wifi_net_label=0;
        run_t.gTimer_10s=0;
	
		Update_DHT11_Value(); //to message display 
		HAL_Delay(2);//HAL_Delay(20);
		run_t.gTimer_run_process_times=0;

        }
		

		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS && run_t.run_process_step==1){
			run_t.recoder_wifi_link_cloud_flag = 1;
			wifi_t.wifi_has_been_link_cloud = WIFI_CLOUD_SUCCESS;
            run_t.gTimer_run_process_times=0;
        
	 	    SendWifiData_To_Cmd(0x01) ;
			HAL_Delay(2);//HAL_Delay(100);  	
			esp8266_t.esp8266_config_wifi_net_label=wifi_publish_update_data;
			if(run_t.wifi_normal_power_on == WIFI_NORMAL_POWER_ON){
              // run_t.set_timer_timing_value =0;
               Publish_Reference_Update_State();
               run_t.gTimer_run_process_times=0;
               run_t.run_process_step =2;
			}
            else if(run_t.app_appointment_time_power_on == WIFI_TIMER_POWER_ON){
               run_t.set_timer_timing_value = wifi_app_timer[0];
                Publish_Reference_Update_State();
                run_t.gTimer_run_process_times=0;
                run_t.run_process_step =2;

            }
          
			
          
		}
        else{
         
    		run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;
            run_t.set_timer_timing_value =0;
            SendWifiData_To_PanelTime(run_t.set_timer_timing_value);

       }

       if(run_t.gTimer_run_process_times > 0 &&  run_t.run_process_step ==2){
		       
    		    run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;
                run_t.gTimer_run_process_times=0;
                appointment_powr_onf_flag=0;
			
        }

	break;

   

	case UPDATE_TO_PANEL_DATA: //3
     run_t.run_process_step =0;
     if(run_t.gTimer_senddata_panel >30 && run_t.gPower_On==POWER_ON){ //300ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();
	 }

	if((run_t.gTimer_10s>30 && run_t.gPower_flag == POWER_ON)||power_just_on < 10){
    	power_just_on ++ ;
		run_t.gTimer_10s=0;
		Update_DHT11_Value();
        HAL_Delay(5);

     }

  

	if((run_t.app_appointment_time_power_on == WIFI_TIMER_POWER_ON && appointment_powr_onf_flag==0) &&  run_t.gTimer_run_process_times > 0 ){ //
		appointment_powr_onf_flag++;
  
   
        sendData_Reference_Data(run_t.gDry,run_t.gPlasma,run_t.gUltrasonic);
	    HAL_Delay(5);
    
        run_t.gTimer_run_process_times =0;

	}

    if((run_t.app_appointment_time_power_on == WIFI_TIMER_POWER_ON &&  run_t.gTimer_run_process_times > 0) && appointment_powr_onf_flag==1){ //
	
     
        run_t.app_appointment_time_power_on = WIFI_NULL;
        

       
        SendWifiData_To_PanelTime(wifi_app_timer[0]);
  
        HAL_Delay(5);
      

	}

    if(run_t.wifi_normal_power_on == WIFI_NORMAL_POWER_ON && appointment_powr_onf_flag !=1 &&  run_t.gTimer_run_process_times > 0){
           run_t.wifi_normal_power_on = WIFI_NULL;
        

        run_t.set_timer_timing_value =0;
        SendWifiData_To_PanelTime(run_t.set_timer_timing_value);
  
        HAL_Delay(5);

    }
    
	
	 if(run_t.gTimer_ptc_adc_times > 2 ){ //3 minutes 120s
         run_t.gTimer_ptc_adc_times=0;
		 Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,20);
	     Judge_PTC_Temperature_Value();

	 }

	 if(run_t.gTimer_fan_adc_times > 1){ //2 minute 180s
	     run_t.gTimer_fan_adc_times =0;
	     Self_CheckFan_Handler(ADC_CHANNEL_0,30);
	 }
    break;

     case POWER_OFF: //2

        switch(power_off_step){

        case 0:
            run_t.wifi_normal_power_on =0;
            esp8266_t.esp8266_config_wifi_net_label=0xff;
            run_t.run_process_step=0;
            SendWifiCmd_To_Order(WIFI_POWER_OFF); 
		    HAL_Delay(5);//HAL_Delay(100);
            power_off_step =1;
            run_t.gTimer_run_process_times=0;

        break;

        case 1:
            SetPowerOff_ForDoing();
            run_t.set_timer_timing_value=0;
            Publish_Power_OFF_State();
            power_off_step =2;
            run_t.gTimer_run_process_times=0;

        break;

        case 2:
           if( run_t.gTimer_run_process_times >0){
          
            power_off_step =3;
            run_t.gTimer_run_process_times=0;

           
           }
         
       break;

       case 3:

        
          run_t.gTimer_run_process_times=0;

	  
           run_t.fan_set_level=5;
     
           run_t.gFan_counter =0;
           if(the_first_dc_power_on ==0){
              the_first_dc_power_on++;

           }
           else
		    run_t.gFan_continueRun =1;
    
	    run_t.gPower_flag =POWER_OFF;
       
          power_off_step =4;

       break;

       case 4:
	
        wifi_t.wifi_has_been_link_cloud = WIFI_CLOUD_SUCCESS;
		run_t.recoder_wifi_link_cloud_flag = 1; //recoder has been linked cloud flag
		run_t.set_timer_timing_value=0;
        if( run_t.gTimer_run_process_times >5){
           run_t.gTimer_run_process_times=0;
            run_t.set_timer_timing_value=0;
           Publish_Power_OFF_State();

       }
       

      break;
      }
	break;

	case POWER_OFF_NULL:

	break;

    }
	
   Fan_ContinueRun_OneMinute_Fun();

	

   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS  &&  run_t.wifi_link_JPai_cloud==0){
	 	    run_t.wifi_link_JPai_cloud++;
	 	    SendWifiData_To_Cmd(0x01) ;
	 }
	 
		
	
}

static void Fan_ContinueRun_OneMinute_Fun(void)
{
	
	if(run_t.gFan_continueRun ==1 && run_t.gPower_On ==POWER_OFF){
          
		if(run_t.gFan_counter < 60){

		    Fan_Run_Fun();
		}       
        else if(run_t.gFan_counter > 59){

		run_t.gFan_counter=0;

		run_t.gFan_continueRun++;
		FAN_Stop();
		}
	  }



}



