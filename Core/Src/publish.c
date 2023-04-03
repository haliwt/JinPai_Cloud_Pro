#include "publish.h"
#include "gpio.h"
#include "esp8266.h"
#include "wifi_fun.h"
#include "dht11.h"
#include "usart.h"
#include "run.h"



//static void MYUSART_SendData(uint8_t  data);
static void SendHead(void);
static void SendFrame_Len(uint8_t len);
//"0x eb 32 76 b2   d2 62 3d b3   ab 10 c2 9d   f4 f7 c5 7d";

uint32_t  ProdKey_one = 0xeb3276b2;
uint32_t ProdKey_two = 0xd2623db3;
uint32_t ProdKey_three = 0xab10c29d ;

uint32_t ProdKey_four = 0xf4f7c57d;//"0xeb3276b2d2623db3ab10c29df4f7c57d";

uint8_t send_data_numbers;



uint8_t ProdKey_sum = 0xb0 ;//0xeb+0x32+0x76+0xb2+0xd2+0x62+0x3d+0xb3+0xab+0x10+0xc2+0x9d+0xf4+0xf7+0xc5+0x7d=0x9b0;

//send one byte 
void MYUSART_SendData(uint8_t  data)
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

static void SendFrame_Power(uint8_t dat)
{
	MYUSART_SendData(dat);
}

static void SendFrame_Dry(uint8_t dry)
{
	MYUSART_SendData(dry);
}

static void SendFrame_Ster(uint8_t ster)
{
	MYUSART_SendData(ster);
}

static void SendFrame_Mouse(uint8_t mouse)
{
	MYUSART_SendData(mouse);
}

static void SendFrame_SetTemperature(uint8_t temp)
{
	MYUSART_SendData(temp);
}

static void SendFrame_SetTimer(uint8_t timing)
{
	MYUSART_SendData(timing);
}

static void SendFrame_SetFanSpeed(uint8_t speed)
{
    MYUSART_SendData(speed);
}

static void SendFrame_Read_TemperatureValue(uint8_t rtemp)
{
    MYUSART_SendData(rtemp);
}

static void SendFrame_Read_HumidityValue(uint8_t humvalue)
{
    MYUSART_SendData(humvalue);
}

static void SendFrame_Time_Remaining_One(uint8_t rtiming_one)
{
    
    MYUSART_SendData(rtiming_one);
}
static void SendFrame_Time_Remaining_Two(uint8_t rtiming_two)
{
  
   MYUSART_SendData(rtiming_two);
}

static void SendFrame_Time_Working_One(uint8_t wtiming)
{
 
	MYUSART_SendData(wtiming);
}

static void SendFrame_Time_Working_Two(uint8_t wtiming_two)
{
    
    MYUSART_SendData(wtiming_two);
}


static void SendFrame_Alarm_Infor(uint8_t inf)
{
    MYUSART_SendData(inf);
}
/*****************************************************************************
	*
	*Function Name: void Publish_Data_ProdKey(void)
	*Function : send product ID 
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
void Init_Publisher_Data_Ref(void)
{
   run_t.gPower_On = 1;
   run_t.gDry = 1;
   run_t.gPlasma = 1;
   run_t.gUltrasonic = 1;
   if(run_t.set_temperature_value < 20)run_t.set_temperature_value=20; //20~40度
   run_t.gFanSpeed = 1; //fan speed high 
   run_t.alarm_call = 0;

}


/*****************************************************************************
	*
	*Function Name: void Publish_Data_ProdKey(void)
	*Function : send product ID 
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
void Publish_Data_ProdKey(void)
{
	uint8_t product_sum;
	SendHead();
	SendFrame_Len(0x16);//frame length = 0x16
	SendFrame_Type(0xFE);
	SendFrame_Numbers(0x08);
	SendFrame_Order(0x21);
    SendProdKey_OneByte_One();
	SendProdKey_OneByte_Two();
	SendProdKey_OneByte_Three();
	SendProdKey_OneByte_Four();
	
    SendProdKey_Two();
    SendProdKey_Three();
    SendProdKey_Four();
     
    product_sum = 0x35;//0x2E//ProdKey_sum +  0x48+0x16+0xFE+0x01+0x21; //0xb0+0x48+0x16+0xFE+0x01+0x21=0x22E;
	SendFrame_Sum(product_sum);

}
/*****************************************************************************************************
 * 
 * Function Name: void Publish_Ultrasonic_OFF_State(void)
 * Function Description::send to cloud data that ultrasonic
 * Input Parameters:NO
 * Return Parameter:NO
 * 
 ****************************************************************************************************/
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


void Publish_Command_Query(void)
{
  SendHead();
  SendFrame_Len(0x07);
  SendFrame_Type(0xFE);
  SendFrame_Numbers(0x01);
  SendFrame_Order(0x01);
  SendFrame_Data(0);
  SendFrame_Sum(0x4F);


}





/*****************************************************************************
	*
	*Function Name: void Publish_Data_ProdKey(void)
	*Function : send product ID 
	*Input Ref:NO
	*Return Ref:NO
	*
*******************************************************************************/
void Publish_Return_Repeat_Data(void)
{
  
   static uint8_t numbers,data;
   numbers = usart_wifi_t.usart_wifi_counter;
   
   for(send_data_numbers =0; send_data_numbers <usart_wifi_t.usart_wifi_counter;send_data_numbers++ ){

       data =usart_wifi_t.usart_wifi[send_data_numbers];
      
	  SendFrame_Data(data);
	  


   }
    


}
/*****************************************************************************************************
 * 
 * Function Name: void Publish_Ultrasonic_OFF_State(void)
 * Function Description::send to cloud data that ultrasonic
 * Input Parameters:NO
 * Return Parameter:NO
 * 
 ****************************************************************************************************/
void Publish_Power_ON_State(void)
{

  uint8_t temp_codes;

  SendHead();
  SendFrame_Len(0x14);
  SendFrame_Type(0x01);
  SendFrame_Numbers(0x01); //4
  SendFrame_Order(0x01);
  SendFrame_Power(0x01);
  SendFrame_Dry(0x01);
  SendFrame_Ster(0x01);//8
  SendFrame_Mouse(0x01);
  if(run_t.set_temperature_value < 20)run_t.set_temperature_value=20;
  SendFrame_SetTemperature(run_t.set_temperature_value);//10
  SendFrame_SetTimer(run_t.set_timer_timing_value);
  SendFrame_SetFanSpeed(0x01);
  SendFrame_Read_TemperatureValue(run_t.gDht11_temperature);
  SendFrame_Read_HumidityValue(run_t.gDht11_humidity );
  SendFrame_Time_Remaining_One(run_t.time_remaining_minutes_one); //15
  SendFrame_Time_Remaining_Two(run_t.time_remaining_minutes_two);
  SendFrame_Time_Working_One(run_t.work_time_minutes_one);
  SendFrame_Time_Working_Two(run_t.work_time_minutes_two);
  SendFrame_Alarm_Infor(run_t.alarm_call);
  temp_codes = 0x048+0x14+0x01+0x01+0x01+0x01+0x01+0x01+0x01+run_t.set_temperature_value+run_t.set_timer_timing_value+0x01\
  	+run_t.gDht11_temperature+run_t.gDht11_humidity +run_t.time_remaining_minutes_one+run_t.time_remaining_minutes_two\
    +run_t.work_time_minutes_one+run_t.work_time_minutes_two+run_t.alarm_call;

  SendFrame_Sum(temp_codes);


}
/*****************************************************************************************************
 * 
 * Function Name: void Publish_Ultrasonic_OFF_State(void)
 * Function Description::send to cloud data that ultrasonic
 * Input Parameters:NO
 * Return Parameter:NO
 * 
 ****************************************************************************************************/
void Publish_Power_OFF_State(void)
{

  uint8_t temp_code;
  SendHead();
  SendFrame_Len(0x14);
  SendFrame_Type(0x01);
  SendFrame_Numbers(0x01); //4
  SendFrame_Order(0x01);
  SendFrame_Power(0);
  SendFrame_Dry(0);
  SendFrame_Ster(0);//8
  SendFrame_Mouse(0);
  if(run_t.set_temperature_value < 20)run_t.set_temperature_value=20;
  SendFrame_SetTemperature(run_t.set_temperature_value);//10
  SendFrame_SetTimer(run_t.set_timer_timing_value);
  SendFrame_SetFanSpeed(0);
  SendFrame_Read_TemperatureValue(run_t.gDht11_temperature);
  SendFrame_Read_HumidityValue(run_t.gDht11_humidity);
  SendFrame_Time_Remaining_One(run_t.time_remaining_minutes_one); //15
  SendFrame_Time_Remaining_Two(run_t.time_remaining_minutes_two);
  SendFrame_Time_Working_One(run_t.work_time_minutes_one);
  SendFrame_Time_Working_Two(run_t.work_time_minutes_two);
  SendFrame_Alarm_Infor(run_t.alarm_call);
  temp_code = 0x048+0x14+0x01+0x01+0x01+0x00+0x00+0x00+0x00+run_t.set_temperature_value+run_t.set_timer_timing_value\
  	+0x00+run_t.gDht11_temperature+run_t.gDht11_humidity +run_t.time_remaining_minutes_one+run_t.time_remaining_minutes_two\
    +run_t.work_time_minutes_one+run_t.work_time_minutes_two+run_t.alarm_call;

  SendFrame_Sum(temp_code);


}

/*****************************************************************************************************
 * 
 * Function Name: void Publish_Ultrasonic_OFF_State(void)
 * Function Description::send to cloud data that ultrasonic
 * Input Parameters:NO
 * Return Parameter:NO
 * 
 ****************************************************************************************************/
void Publish_Reference_Update_State(void)
{

  uint8_t temp_code,power_number;

  SendHead();
  SendFrame_Len(0x14);
  SendFrame_Type(0x01);
  SendFrame_Numbers(0x01); //4
  SendFrame_Order(0x01);
  if(run_t.gPower_On==POWER_ON){
      SendFrame_Power(0x01);
      power_number= 0x01;
  }
  else 
  {
    SendFrame_Power(0x00);
    power_number= 0;
  }
  SendFrame_Dry(run_t.gDry);
  SendFrame_Ster(run_t.gPlasma);//8
  SendFrame_Mouse(run_t.gUltrasonic);
  if(run_t.set_temperature_value < 20)run_t.set_temperature_value=20;
  SendFrame_SetTemperature(run_t.set_temperature_value);//10
  SendFrame_SetTimer(run_t.set_timer_timing_value);
  SendFrame_SetFanSpeed(run_t.set_wind_speed_value);
  SendFrame_Read_TemperatureValue(run_t.gDht11_temperature);
  SendFrame_Read_HumidityValue(run_t.gDht11_humidity);
  SendFrame_Time_Remaining_One(run_t.time_remaining_minutes_one); //15
  SendFrame_Time_Remaining_Two(run_t.time_remaining_minutes_two);
  SendFrame_Time_Working_One(run_t.work_time_minutes_one);
  SendFrame_Time_Working_Two(run_t.work_time_minutes_two);
  SendFrame_Alarm_Infor(run_t.alarm_call);
  temp_code = 0x048+0x14+0x01+0x01+0x01+power_number+run_t.gDry +run_t.gPlasma+run_t.gUltrasonic+run_t.set_temperature_value+run_t.set_timer_timing_value\
  	+run_t.set_wind_speed_value+run_t.gDht11_temperature+run_t.gDht11_humidity+run_t.time_remaining_minutes_one+run_t.time_remaining_minutes_two\
    +run_t.work_time_minutes_one+run_t.work_time_minutes_two+run_t.alarm_call;

  SendFrame_Sum(temp_code);


}


