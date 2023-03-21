#include "wifi_fun.h"
#include "cmd_link.h"
#include "run.h"
#include "fan.h"
#include "tim.h"
#include "esp8266.h"

#include "dht11.h"
#include "usart.h"

#include "flash.h"


WIFI_FUN   wifi_t;

static void Wifi_ReceiveData_Handler(void);
void RunWifi_Command_Handler(uint8 command)
{
    
    
    switch(command){

        case wifi_start_link_net:

             //wifi gpio 13 pull down 5s 
            WIFI_AUTO_SMART_CONFIG();
            if(wifi_t.gTimer_5s > 5){
                wifi_t.gTimer_5s = 0;
                command = wifi_smartconfig_model;
                WIFI_AUTO_EXIT_SMART_CONFIG();

                

            }
               
        break;

        case wifi_smartconfig_model:

            Wifi_ReceiveData_Handler();

        break;

        case wifi_receive_data_state:


       break;






    }




}


static void Wifi_ReceiveData_Handler(void)
{
    static uint8_t usart_len;

    usart_len = sizeof(usart_wifi_t.usart_wifi)/sizeof(usart_wifi_t.usart_wifi[0])
    
    memcpy(wifi_t.wifi_dispodr_data,usart_wifi_t.usart_wifi,usart_len);

     wifi_t.usart_wifi_frame_type =wifi_t.wifi_dispodr_data[0];
	 wifi_t.usart_wifi_sequence =wifi_t.wifi_dispodr_data[1];
	 wifi_t.usart_wifi_order =wifi_t.wifi_dispodr_data[2];
	 wifi_t.usart_wifi_model =wifi_t.wifi_dispodr_data[3];
	 wifi_t.usart_wifi_state =wifi_t.wifi_dispodr_data[4];

	 wifi_t.usart_wifi_cloud_state =wifi_t.wifi_dispodr_data[5];

	 wifi_t.usart_wifi_signal_state =wifi_t.wifi_dispodr_data[6];

	 wifi_t.usart_wifi_pass_state =wifi_t.wifi_dispodr_data[8];

	

    



}




