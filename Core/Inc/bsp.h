#ifndef __BSP_H_
#define __BSP_H_
#include "main.h"

#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "esp8266.h"
#include "cmd_link.h"
#include "run.h"
#include "subscribe.h"
#include "publish.h"
#include "interrupt_manager.h"
#include "wifi_fun.h"
#include "buzzer.h"
#include "dht11.h" 
#include "execute.h"
#include "adc.h"
#include "fan.h"
#include "self_check.h"


typedef enum{

  copy_power_off = 0x1,
  copy_power_on = 0x02,
  copy_wifi_power_off=3,
  copy_wifi_power_on=4,
  copy_wifi_timer_power_on=5,
  


}copy_cmd_t;
 


typedef struct main_process{

  uint8_t gPower_On;
  uint8_t g_main_process_step;
  uint8_t g_interval_times_flag;
  uint8_t gTimer_two_hours_counter;
  uint8_t gTimer_counter_minutes;


}process_t;

extern process_t gpro_t;


void receive_copy_cmd(uint8_t cmd);

void works_two_hours_handler(void);



#endif

