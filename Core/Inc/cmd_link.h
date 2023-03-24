#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_
#include "main.h"

#define USART_CMD_NUMBERS   25
#define USART_WIFI_NUMBERS  25


typedef enum {
    
     WIFI_SUCCESS ,
     WIFI_FAIL 

}wifi_enum_state_type;

typedef enum {
    WIFI_CLOUD_FAIL,
    WIFI_CLOUD_SUCCESS

}wifi_enum_link_cloud_state;
typedef struct _usart_cmd_t {
    uint8_t usart_cmd[USART_CMD_NUMBERS];
    uint8_t usart_cmd_len;
    uint8_t cmd_data[1];

}USART_CMD_T;

extern USART_CMD_T usart_cmd_t;

typedef struct _usart_wifi_t {
    
    uint8_t usart_wifi_frame_len[1];

    uint8_t usart_wifi_sum;
    uint8_t usart_wifi_counter;
    uint8_t usart_receive_numbers;
    uint8_t usart_wifi_receive_success_flag;
	uint8_t usart_wifi_start_receive_flag;
    uint8_t usart_wifi_receive_read_data_flag;
   
    uint8_t usart_wifi_data[1];
	uint8_t usart_wifi[USART_WIFI_NUMBERS];
    


}USART_WIFI_T;

extern USART_WIFI_T usart_wifi_t;


extern uint8_t rx_wifi_data[7];

extern uint8_t inputBuf[4];

extern uint8_t  inputCmd[2];
extern uint8_t wifiInputBuf[1];

void Decode_Function(void);

void sendData_Real_TimeHum(uint8_t hum,uint8_t temp);

void SendWifiData_To_Cmd(uint8_t wdata);

void SendWifiData_To_PanelWindSpeed(uint8_t dat1);


void SendWifiData_To_PanelTime(uint8_t dat1);
void SendWifiData_To_PanelTemp(uint8_t dat1);
void SendWifiCmd_To_Order(uint8_t odata);
void SendData_Real_GMT(uint8_t hdata,uint8_t mdata,uint8_t sdata);
void Eesp8266_TxData_ToSmartPhone(void);

void SendWifiData_To_WifiSetTemp(uint8_t dat1);
void USART2_WIFI_Receive_Interrupt_Data(void);



#endif 
