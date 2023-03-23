#include "publish.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "dht11.h"
#include "usart.h"


static void MYUSART_SendData(uint8_t  data);
static void SendHead(void);
static void SendFrame_Len(uint8_t len);
//"0x eb 32 76 b2   d2 62 3d b3   ab 10 c2 9d   f4 f7 c5 7d";

uint8_t  ProdKey_one = 0xeb3276b2;
uint32_t ProdKey_two = 0xd2623db3;
uint32_t ProdKey_three = 0xab10c29d ;

uint32_t ProdKey_four = 0xf4f7c57d;//"0xeb3276b2d2623db3ab10c29df4f7c57d";




uint8_t ProdKey_sum = 0xb0 ;//0xeb+0x32+0x76+0xb2+0xd2+0x62+0x3d+0xb3+0xab+0x10+0xc2+0x9d+0xf4+0xf7+0xc5+0x7d=0x9b0;

//send one byte 
static void MYUSART_SendData(uint8_t  data)
{
	while((USART2->ISR&0X40)==0); 
	USART2->TDR = data;
}

//send prodkey ID
static void SendProdKey_OneByte_One(void)
{

	MYUSART_SendData(0xeb);
}

static void SendProdKey_OneByte_Two(void)
{

	MYUSART_SendData(0x32);
}

static void SendProdKey_OneByte_Three(void)
{

	MYUSART_SendData(0x76);
}

static void SendProdKey_OneByte_Four(void)
{

	MYUSART_SendData(0xb2);
}



static void SendProdKey_Two(void){

	MYUSART_SendData(ProdKey_two>>24);
	MYUSART_SendData(ProdKey_two>>16);
	MYUSART_SendData(ProdKey_two>>8);
	MYUSART_SendData(ProdKey_two);
}

static void SendProdKey_Three(void){

	MYUSART_SendData(ProdKey_three>>24);
	MYUSART_SendData(ProdKey_three>>16);
	MYUSART_SendData(ProdKey_three>>8);
	MYUSART_SendData(ProdKey_three);
}
static void SendProdKey_Four(void)
{
	MYUSART_SendData(ProdKey_four>>24);
	MYUSART_SendData(ProdKey_four>>16);
	MYUSART_SendData(ProdKey_four>>8);
	MYUSART_SendData(ProdKey_four);



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
static void SendFrame_Order(uint8_t onum)
{
	MYUSART_SendData(onum);
}
static void SendFrame_Vaild_Data(uint8_t dat)
{
	MYUSART_SendData(dat);
}

static void SendFrame_Sum(uint8_t snum)
{
	MYUSART_SendData(snum);
}
static void SendFrame_Data(uint8_t dat)
{
	MYUSART_SendData(dat);
}


void Publish_Data_Special(uint8_t cmd,uint8_t d_type,uint8_t valid_d)
{
    uint8_t temp;
    uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendFrame_Len(cmd);//ÃüÁî°ü±êÊ¶
	SendFrame_Type(d_type);
	SendFrame_Numbers(0x00);
	SendFrame_Vaild_Data(valid_d);
    temp =0x48+d_type+cmd+valid_d;
	SendFrame_Sum(temp);



}
/****************************************************
	*
	*Function Name: void Publish_Data_ProdKey(void)
	*Function : send product ID 
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************/
void Publish_Data_ProdKey(void)
{
	uint8_t product_sum;
	SendHead();
	SendFrame_Len(0x16);//frame length = 0x16
	SendFrame_Type(0xFE);
	SendFrame_Numbers(0x01);
	SendFrame_Order(0x21);
    SendProdKey_OneByte_One();
	SendProdKey_OneByte_Two();
	SendProdKey_OneByte_Three();
	SendProdKey_OneByte_Four();
	
    SendProdKey_Two();
    SendProdKey_Three();
    SendProdKey_Four();
     
    product_sum = 0x2e; //0xb0+0x48+0x16+0xFE+0x01+0x21=0x22E;
	SendFrame_Sum(product_sum);

}


void Publish_Command_SmartCofnig(void)
{
    SendHead();
    SendFrame_Len(0x07);//frame length = 0x16
	SendFrame_Type(0xFE);
	SendFrame_Numbers(0x01);
    SendFrame_Order(0x03);
	SendFrame_Data(0);
	SendFrame_Sum(0x51);

}





