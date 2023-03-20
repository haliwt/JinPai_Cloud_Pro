#ifndef __CMD_LINK_H_
#define __CMD_LINK_H_
#include "main.h"

#define USART_CMD_NUMBERS   50
#define USART_WIFI_NUMBERS  50

typedef struct _usart_cmd_t {
    uint8_t usart_cmd[USART_CMD_NUMBERS];
    uint8_t usart_cmd_len;
    uint8_t *data_cmd;

}USART_CMD_T;

extern USART_CMD_T usart_cmd_t;

typedef struct _usart_wifi_t {
    uint8_t usart_wifi[USART_WIFI_NUMBERS];
    uint8_t usart_wifi_len;
    uint8_t *data_wifi;


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



#endif 
