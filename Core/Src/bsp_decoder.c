#include "bsp.h"

static void HandlePowerCommand(uint8_t param1, uint8_t param2) ;
static void HandleWifiCommand(uint8_t param1, uint8_t param2);
static void HandleTemperatureCommand(uint8_t param1, uint8_t param2);
static void HandleTimerCommand(uint8_t param1, uint8_t param2);
static void HandleWorkTimeCommand(uint8_t param1, uint8_t param2);
static void HandleRemainingTimeCommand(uint8_t param1, uint8_t param2);
static void HandleBuzzerCommand(uint8_t param1, uint8_t param2);
static void HandleCommandReceive(uint8_t param1, uint8_t param2);





/**
  * @brief  解码并执行接收到的命令
  * @note   优化要点：
  *         - 使用查表法替代switch-case提高效率
  *         - 提取公共操作到单独函数
  *         - 消除重复的条件检查
  *         - 使用位操作替代数学运算
  *         - 减少不必要的延迟
  */
void handleDecode_RunCmd(void)
{
    static uint8_t cmdType,param1,param2;
	
	// 静态变量存储命令类型（避免多次访问数组）
   cmdType = inputCmd[0];
    param1 = inputCmd[1];
    param2 = inputCmd[2];
    
    // 前置条件检查（电源状态）
//    if(gpro_t.gPower_On != POWER_ON) {
//        return; // 电源关闭时直接返回
//    }

    // 命令处理函数指针类型定义
    typedef void (*CmdHandler)(uint8_t, uint8_t);
    
    // 命令处理映射表（查表法）
    static const CmdHandler cmdHandlers[] = {
        ['P'] = HandlePowerCommand,
        ['W'] = HandleWifiCommand,
        ['C'] = HandleCommandReceive,
        ['M'] = HandleTemperatureCommand,
        ['T'] = HandleTimerCommand,
        ['O'] = HandleWorkTimeCommand,
        ['R'] = HandleRemainingTimeCommand,
        ['Z'] = HandleBuzzerCommand
    };

    // 执行对应的命令处理函数
    if(cmdType < sizeof(cmdHandlers)/sizeof(cmdHandlers[0])) {
        CmdHandler handler = cmdHandlers[cmdType];
        if(handler != NULL) {
            handler(param1, param2);
        }
    }
}

/******************** 命令处理子函数 ********************/

/**
  * @brief  处理WiFi相关命令
  */
static void HandlePowerCommand(uint8_t param1, uint8_t param2) 
{
    // 参数安全校验
  //  if(param1 > POWER_CMD_MAX) return;
    
    // 执行电源操作
    switch(param1) {
    case POWER_ON:
        //HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
       // run_t.power_state = POWER_ON;
         SendData_Copy_Cmd(copy_power_on);//SendWifiData_To_Cmd(0x54); //0x54= 'R',receive order from display power on command copy a command 
         HAL_Delay(5);
         
         
         gpro_t.gPower_On = POWER_ON;
         gpro_t.g_main_power_on_step=0;
         wifi_t.wifi_open_power_on_flag =0;
         esp8266_t.esp8266_config_wifi_net_label=0;
		 gpro_t.gTimer_two_hours_counter =0; //WT.EDIT 2025.04.11
         gpro_t.g_interval_times_flag=0;//WT.EDIT 2025.04.11
         gpro_t.gTimer_counter_minutes=0;//WT.EDIT 2025.04.11
         Buzzer_KeySound();
        break;
    case POWER_OFF:
        //HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_RESET);
        //run_t.power_state = POWER_OFF;
        SendData_Copy_Cmd(copy_power_off);//SendWifiData_To_Cmd(0x53); //0x53= 'R' power off copy command from display power off
        HAL_Delay(5);

         gpro_t.gPower_On = POWER_OFF;
         gpro_t.g_main_power_off_step=0;

         wifi_t.wifi_open_power_on_flag =0;
         gpro_t.gTimer_two_hours_counter =0; //WT.EDIT 2025.04.11
         gpro_t.g_interval_times_flag=0;//WT.EDIT 2025.04.11
         gpro_t.gTimer_counter_minutes=0;//WT.EDIT 2025.04.11

         Buzzer_KeySound();
        break;
    default:
        // 异常处理
        Error_Handler();
    }
    
    // 状态更新（非阻塞式）
   // if(wifi_t.wifi_link_JPai_cloud == WIFI_CLOUD_SUCCESS) {
      ///  Publish_Power_State(run_t.power_state);
   // }
}

static void HandleWifiCommand(uint8_t param1, uint8_t param2)
{
   if(gpro_t.gPower_On==POWER_ON){
	      if(param1==1){//long press key that power key
              //fast blink led for link to tencent cloud
              SendWifiData_To_Cmd(0x52); //0x52= 'R' -> looking for wifi netware
              HAL_Delay(5);
              WIFI_IC_ENABLE();
			  Buzzer_KeySound();	
			   wifi_t.wifi_link_JPai_cloud= WIFI_CLOUD_FAIL;
		       esp8266_t.esp8266_config_wifi_net_label=wifi_start_link_net;
	           wifi_t.gTimer_5s=0;
			   
		  }
   }
}

/**
  * @brief  处理温度设置命令
  */
static void HandleTemperatureCommand(uint8_t param1, uint8_t param2)
{
   if(gpro_t.gPower_On==POWER_ON){
              
       run_t.set_temperature_value =param1;
			 if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
				Publish_Reference_Update_State();//
				HAL_Delay(300);
			 }
			    
			   
         }
}

/**
  * @brief  处理定时器设置命令
  */
static void HandleTimerCommand(uint8_t param1, uint8_t param2)
{
   if(gpro_t.gPower_On==POWER_ON){
           
			if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
				run_t.set_timer_timing_value =  param1;
				//count_total_times = run_t.set_timer_timing_value *60 ;
				 run_t.set_timer_timing_value= run_t.set_timer_timing_value *60 ;
			   run_t.time_remaining_minutes_one = (run_t.set_timer_timing_value >>8) & 0xff;
			   run_t.time_remaining_minutes_two =  run_t.set_timer_timing_value & 0xff;
			
				Publish_Reference_Update_State();//
				HAL_Delay(200);

			 }
			
			   
         }
}

/**
  * @brief  处理工作时间命令
  */
static void HandleWorkTimeCommand(uint8_t param1, uint8_t param2)
{

   if(gpro_t.gPower_On==POWER_ON){
	if(wifi_t.wifi_link_JPai_cloud == WIFI_CLOUD_SUCCESS) {
        run_t.work_time_minutes_one = param1;
        run_t.work_time_minutes_two = param2;
        Publish_Reference_Update_State();
		HAL_Delay(100);
    }
   	}
}

/**
  * @brief  处理剩余时间命令
  */
static void HandleRemainingTimeCommand(uint8_t param1, uint8_t param2)
{
     if(gpro_t.gPower_On==POWER_ON){

		  run_t.time_remaining_minutes_one = param1;
		  run_t.time_remaining_minutes_two =  param2;
		  if(wifi_t.wifi_link_JPai_cloud== WIFI_CLOUD_SUCCESS){
                Publish_Reference_Update_State();//
                HAL_Delay(100);
          }
	 }
}

/**
  * @brief  处理蜂鸣器命令
  */
static void HandleBuzzerCommand(uint8_t param1, uint8_t param2)
{  
   if(gpro_t.gPower_On==POWER_ON){
    if(param1 == 'Z') { // 关闭AI
        run_t.buzzer_sound_flag = 1;
    }
   	}
}

static void HandleCommandReceive(uint8_t param1, uint8_t param2)
{
  
	static uint8_t no_buzzer_sound_dry_off;
	
	if(gpro_t.gPower_On==POWER_ON){
	switch(param1){
	
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
				HAL_Delay(200);
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
	
	
		  break; 
	
	
		}


    	}

}


