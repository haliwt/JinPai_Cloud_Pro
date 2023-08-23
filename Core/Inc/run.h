#ifndef __RUN_H__
#define __RUN_H__
#include "main.h"

#define WIFI_TUYA      1  //
#define AI_ENABLE      1

typedef enum {                   /* enumeration for CProcess signals */
    kill =1,notkill,dry,notdry,ai,notai
}Signal;

typedef enum
{
  AIENABLE = 0,
  AIDISABLE = !AIENABLE
} AiStatus;

typedef enum
{
  WIFIENABLE =1,
  WIFIDISABLE = !WIFIENABLE

}WifiStatus;

typedef enum{

   POWER_OFF,
   POWER_ON =0x01,
   POWER_CONNECTOR_WIFI,
   UPDATE_TO_PANEL_DATA,
   WIFI_RESTART_INIT,
   WIFI_NORMAL_POWER_ON,
   WIFI_TIMER_POWER_ON,
   POWER_OFF_NULL,
   WIFI_NULL,

   

}run_state_t;

typedef enum{

	 DRY_ON = 0x40,
	 DRY_OFF = 0x41,

	 DRY_ON_NO_BUZZER = 0x90,
	 DRY_OFF_NO_BUZZER = 0x91,

	 PLASMA_ON = 0x20,
	 PLASMA_OFF = 0x21,

	 ULTRASONIC_ON =0x10,
	 ULTRASONIC_OFF = 0x11,

	 WIFI_CONNECT_FAIL = 0x55,
     WIFI_CONNECT_SUCCESS= 0xAA



}works_t;



typedef struct _RUN_T{
    
    //power on
	uint8_t gPower_On;
    uint8_t first_dc_power_on_flag ;

	uint8_t gPower_flag;
	uint8_t gDht11_flag;
    
	

	uint8_t gDht11_humidity ;
	uint8_t gDht11_temperature;

    

	uint8_t RunCommand_Label;
	uint8_t dp_link_wifi_fail;
	//buzzer
	uint8_t buzzer_sound_flag ;
  
    uint8_t decodeFlag;
    //run step
    uint8_t run_process_step ;
    uint8_t gTimer_run_process_times;
    uint8_t gTimer_minuter_times;
  

    //display command 
    uint8_t display_cmd_link_wifi;
 

    //flash 
    uint8_t flash_write_data_error;
	uint8_t flash_write_data_flag;
    uint8_t flash_read_data;
	
	
    //ptc
    uint8_t  gDry;
    uint8_t gTimer_ptc_adc_times ;
   
    uint8_t ptc_too_heat_value;
	
	//fan 
	uint8_t  gFan;
	uint8_t gFan_pwm_duty_level;
	uint8_t fan_detect_malfuntion;
	uint8_t gFan_continueRun;
	uint8_t  gFan_counter;
	uint8_t fan_start_shut_off_flag;
	uint8_t gTimer_fan_oneselt_test ;
	uint8_t gTimer_fan_adc_times;
	uint8_t self_check_fan_power_on;
	uint8_t fan_set_level;
	uint8_t open_fan_works_flag;
	

	//adc 
	uint8_t ADC_channel_No;
      

      uint8_t  gPlasma;
     
	 
	  uint8_t  gUltrasonic;
	  uint8_t  gTimer;
      uint8_t  gTemperature ;
	  uint8_t  gFanSpeed;
	  uint8_t  gHumidity;
	//fault 
	  uint8_t  alarm_call;
	
	  

	  //usart1
	
  
  
    uint8_t gTimer_check_iwdg_fla;
    uint8_t gTimer_usart_error;
  

	//wifi 
	uint8_t recoder_wifi_link_cloud_flag;

	  
	  uint8_t  set_temperature_value;
	  uint8_t  set_timer_timing_value;
	  uint8_t  set_temperature_decade;
	  uint8_t  set_temperature_unit;
	  uint8_t  set_wind_speed_value;
  
	  uint8_t  wind_speed_decade;
	  uint8_t  wind_speed_unit;
      uint8_t wifi_config_net_lable;
	 
	  uint8_t wifi_link_JPai_cloud;
	  uint8_t first_power_on_flag;
	  uint8_t app_appointment_time_power_on;
      uint8_t wifi_normal_power_on;

	 
	  
	  
	uint8_t gTimer_1s;
	uint8_t gTimer_10s;
	uint8_t gTimer_send_cloud_state;
	uint8_t gTimer_senddata_panel;

	uint8_t  time_remaining_minutes_one;
	uint8_t  time_remaining_minutes_two;
	uint8_t  work_time_minutes_one;
	uint8_t  work_time_minutes_two;

	uint16_t fan_detect_voltage; 
	uint16_t ptc_temp_voltage;

	

	
}RUN_T;

//#define CProcessInit(me_) ((me_)->cmdCtr__ =0,(me_)->state__ = IDLE)
extern uint8_t ReceiveBuffer[1];
extern RUN_T run_t; 




void  RunCommand_MainBoard_Fun(void);

void Decode_RunCmd(void);



void Single_RunCmd(uint8_t sig);

void Single_Mode(void);

void Single_ReceiveCmd(uint8_t cmd);
void SystemReset(void);



#endif 


