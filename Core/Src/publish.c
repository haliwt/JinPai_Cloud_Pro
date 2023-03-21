#include "publish.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "dht11.h"
#include "usart.h"


static void MYUSART_SendData(uint8_t  data);
static void SendHead(void);
static void SendFrame_Len(uint8_t len);




//send one byte 
static void MYUSART_SendData(uint8_t  data)
{
	while((USART1->ISR&0X40)==0); 
	USART1->TDR = data;
}
//send head
static void SendHead(void)
{
	MYUSART_SendData(0x48);
}
static void SendFrame_Len(uint8_t len)
{
	MYUSART_SendData(len);
}

static void SendFrame_Type(uint8_t type)
{
	MYUSART_SendData(type);
}

static void SendFrame_Numbers(uint8_t num)
{
	MYUSART_SendData(num);
}

static void SendFrame_Vaild_Data(uint8_t dat)
{
	MYUSART_SendData(dat);
}

static void SendFrame_Data(uint8_t dat)
{
	MYUSART_SendData(dat);
}

//·¢ËÍµØÖ·
//static void SendAddr(void)
//{
//	MYUSART_SendData(AS608Addr>>24);
//	MYUSART_SendData(AS608Addr>>16);
//	MYUSART_SendData(AS608Addr>>8);
//	MYUSART_SendData(AS608Addr);
//}
//·¢ËÍ°ü±êÊ¶,

//Â¼ÈëÍ¼Ïñ PS_GetImage
//¹¦ÄÜ:Ì½²âÊÖÖ¸£¬Ì½²âµ½ºóÂ¼ÈëÖ¸ÎÆÍ¼Ïñ´æÓÚImageBuffer¡£ 
//Ä£¿é·µ»ØÈ·ÈÏ×Ö
//uint8_t  PS_GetImage(void)
//{
//  u16 temp;
//  uint8_t   ensure;
//	uint8_t   *data;
//	SendHead();
//	SendAddr();
//	SendFlag(0x01);//ÃüÁî°ü±êÊ¶
//	SendLength(0x03);
//	Sendcmd(0x01);
//  temp =  0x01+0x03+0x01;
//	SendCheck(temp);
//	data=JudgeStr(2000);
//	if(data)
//		ensure=data[9];
//	else
//		ensure=0xff;
//	return ensure;
//}



void Publish_Data_Power(uint8_t cmd)
{
    uint8_t temp;
    uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendFrame_Len(0x06);//ÃüÁî°ü±êÊ¶
	SendFrame_Vaild_Data(0x01);
	SendFrame_Data(0x01);
    temp = 0x01+0x03+0x01;
	SendFrame_Data(temp);//SendCheck(temp);
	//data=JudgeStr(2000);
	//	if(data)
	//		ensure=data[9];
	//	else
	//		ensure=0xff;
	//	return ensure; 


}




