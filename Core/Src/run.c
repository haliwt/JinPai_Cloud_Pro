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


RUN_T run_t; 

static void Single_Power_ReceiveCmd(uint8_t cmd);
static void Single_Command_ReceiveCmd(uint8_t cmd); 
uint8_t tencent_cloud_flag;




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

 uint8_t cmdType_1=inputCmd[0],cmdType_2 = inputCmd[1];
    
  switch(cmdType_1){
  
      case 'P': //power on and off
        
      	Single_Power_ReceiveCmd(cmdType_2);  
           
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_flag==POWER_ON){
	      if(cmdType_2==1){//long press key that power key
              //fast blink led for link to tencent cloud
              WIFI_IC_ENABLE();
			  Buzzer_KeySound();	
              // wifi_t.wifi_link_cloud =0;
			   wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_FAIL;
		       esp8266_t.esp8266_config_wifi_net_label=wifi_start_link_net;
	           wifi_t.gTimer_5s=0;
			   
		   }
		 
           
        }
       
	   break;
        
      case 'C':
           if(run_t.gPower_flag==POWER_ON){
              Single_Command_ReceiveCmd(cmdType_2); //Single_ReceiveCmd(cmdType_2); 
              
           }
     
         
      break;

	  case 'M':
	  	if(run_t.gPower_flag==POWER_ON){
              
             run_t.set_temperature_value = cmdType_2;
			 //if(wifi_t.wifi_link_cloud ==1)
			      // MqttData_Publis_SetTemp(run_t.set_temperature_value);
			   
         }
	  

	  break;

	  case 'T':
		  if(run_t.gPower_flag==POWER_ON){
             run_t.set_temperature_value = cmdType_2;
			// if(wifi_t.wifi_link_cloud ==1)
			// MqttData_Publis_SetTime(run_t.set_temperature_value);
			   
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
static void Single_Power_ReceiveCmd(uint8_t cmd)
{
  
    
    switch(cmd){

    case 0x00: //power off
        Buzzer_KeySound();
        run_t.gPower_On=POWER_OFF;
        run_t.gPower_flag = POWER_OFF;
        run_t.RunCommand_Label = POWER_OFF;
       //  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS)  
      //   MqttData_Publish_SetOpen(0x0);
           

    cmd = 0xff;
    break;

    case 0x01: // power on
         Buzzer_KeySound();
         run_t.gPower_flag = POWER_ON;
		 run_t.gPower_On = POWER_ON;
         run_t.RunCommand_Label= POWER_ON;
		 Update_DHT11_Value();
		 HAL_Delay(200);
		 if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			// MqttData_Publish_SetOpen(0x01);
	         HAL_Delay(200);
	       //  Publish_Data_ToCloud_Handler();
		 }
		 
	// cmd=0xff;  
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
    static uint8_t no_buzzer_sound_dry_off;
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	        run_t.noBuzzer_sound_dry_flag =1;

       case DRY_ON:
         run_t.gDry = 1;
	      run_t.gFan_continueRun =0;
		    Buzzer_KeySound();
		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
		  Publish_Data_AllRef();
		   HAL_Delay(30);
	      }
		   
		 
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			run_t.gDry = 0;
			 Buzzer_KeySound();
			 if(run_t.gPlasma ==0){ //plasma turn off flag
			  run_t.gFan_counter =0;
			   run_t.gFan_continueRun =1;

		     }
			if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
		        Publish_Data_AllRef();
				 HAL_Delay(30);
			}
			   
       break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
       		run_t.gUltrasonic =1;
			Buzzer_KeySound();
	   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
	        Publish_Data_AllRef();
	        HAL_Delay(30);
	     
	   	}
	    
       break;

       case PLASMA_OFF:
           run_t.gPlasma=0;
           run_t.gUltrasonic =0;
		   Buzzer_KeySound();
	   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
	         Publish_Data_AllRef();
            HAL_Delay(30);
	       
	      
	   	}
	   
       break;

       case FAN_ON:
          run_t.set_wind_speed_value=100;
		  Buzzer_KeySound();
		  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			Publish_Data_AllRef();
			 HAL_Delay(30);
		  }
		
		   
       break;

       case FAN_OFF:
	        Buzzer_KeySound();
           run_t.set_wind_speed_value = 50;
		   if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
			Publish_Data_AllRef();
			 HAL_Delay(30);
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

   static uint8_t power_just_on;
    
    if(run_t.buzzer_sound_flag == 1){
	 	run_t.buzzer_sound_flag = 0;
	    Buzzer_KeySound();

	 }
  
   switch(run_t.RunCommand_Label){

	case POWER_ON: //1
		SetPowerOn_ForDoing();
	    run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;
		power_just_on=0;
        run_t.gTimer_10s=0;
		run_t.gTheFirst_powerOn=1;
		Update_DHT11_Value();
		HAL_Delay(10);

		if(wifi_t.usart_wifi_cloud_state==WIFI_PASS){
			tencent_cloud_flag =1;
	 	    SendWifiData_To_Cmd(0x01) ;
		}
		

	break;

   case UPDATE_TO_PANEL_DATA: //4
     if(run_t.gTimer_senddata_panel >30 && run_t.gPower_On==POWER_ON){ //300ms
	   	    run_t.gTimer_senddata_panel=0;
	        ActionEvent_Handler();
	 }
	 if(wifi_t.usart_wifi_cloud_state==WIFI_PASS){
	 	   if(run_t.gTimer_send_login_sucess > 11){
	 	        SendWifiData_To_Cmd(0x01) ;
	 	   	}
	   }
    
    break;



	case POWER_OFF: //2
		SetPowerOff_ForDoing();
		if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
	 	     tencent_cloud_flag = 1;
		}
		if(run_t.gTheFirst_powerOn ==0)
         	run_t.gFan_continueRun =0;
		else{
		 run_t.gFan_continueRun =1;

		}
         run_t.gFan_counter=0;
		
        
	   run_t.gPower_flag =POWER_OFF;
		
       run_t.RunCommand_Label =0xff;
	break;

    }
	
    if((run_t.gTimer_10s>30 && run_t.gPower_flag == POWER_ON)||power_just_on < 10){
    	power_just_on ++ ;
		run_t.gTimer_10s=0;
		Update_DHT11_Value();

     }

	

	if(run_t.gFan_continueRun ==1 && run_t.gPower_flag == POWER_OFF){
          
                if(run_t.gFan_counter < 60){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }
	  }

	 if(run_t.gPlasma==0 && run_t.gDry==0 && run_t.gPower_flag ==POWER_ON && run_t.gFan_continueRun ==1){

              if(run_t.gFan_counter < 60){
          
                       FAN_CCW_RUN();
                  }       

	           if(run_t.gFan_counter > 59){
		           
				   run_t.gFan_counter=0;
				
				   run_t.gFan_continueRun++;
				   FAN_Stop();
	           }

	 }

	 if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS  &&  run_t.dp_link_wifi_fail == 1){
	 	     run_t.dp_link_wifi_fail =0;
	 	    SendWifiData_To_Cmd(0x01) ;
	 }
	 
		
	
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
   

	static uint8_t self_power_on_flag=0;
	if(self_power_on_flag==0){
        self_power_on_flag ++ ;
        Buzzer_KeySound();
      
		}
    
#if 0

	if(self_power_on_flag==0){
        self_power_on_flag ++ ;
        Buzzer_KeySound();
		run_t.flash_read_data =Flash_Read_Data();
		switch(run_t.flash_read_data){

	     case error: //wifi don't link to tencent cloud ,need manual operation
		      wifi_t.runCommand_order_lable = 0xff;
		      run_t.flash_write_data_flag = 0;
		 break;

		 case success: //wifi has been linked to tencent cloud,need auto link to tencent cloud
		 	//wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
			run_t.flash_write_data_flag = 1;
		  
         break;



		}
	

	switch(run_t.flash_write_data_flag){

      case 0:

      break;

      case 1:
        WIFI_IC_ENABLE();
      	InitWifiModule();
		//Wifi_SoftAP_Config_Handler();
		PowerOn_Self_Auto_Link_Tencent_Cloud();
		
        SmartPhone_TryToLink_TencentCloud();
 
		if(wifi_t.wifi_link_cloud ==1){
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
			//esp8266_t.gTimer_subscription_timing=0;
		}
        else wifi_t.runCommand_order_lable =0xff;

      break;


	}
   }
    
   if(wifi_t.wifi_link_cloud ==1 &&  self_power_on_flag==1){
        self_power_on_flag++;
			wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
			//esp8266_t.gTimer_subscription_timing=0;
	}

 #endif 

}



    
