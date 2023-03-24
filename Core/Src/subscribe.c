#include "subscribe.h"
#include "publish.h"

static void subscribe_data_frame_head(void)
{
    MYUSART_SendData(0x48);
}


static void subscribe_data_frame_len(uint8_t len)
{
    MYUSART_SendData(len);
}

static void subscribe_data_frame_type(uint8_t type)
{
    MYUSART_SendData(type);
}

static void subscribe_data_frame_numbers(uint8_t num)
{
    MYUSART_SendData(num);
}

static void subscribe_data_frame_oreder(uint8_t order)
{
    MYUSART_SendData(order);
}

static void subscribe_data_frame_bool(uint8_t onoff)
{
    MYUSART_SendData(onoff);
}

static void subscribe_data_frame_sum(uint8_t value)
{
    MYUSART_SendData(value);

}
static void subscribe_data_frame_value(uint8_t value)
{
    MYUSART_SendData(value);

}

static void subscribe_data_driver_mouse(uint8_t value)
{
    MYUSART_SendData(value);

}
void subscribe_data_sterilization(uint8_t ster)
{

      MYUSART_SendData(ster);
}

void subscribe_data_dry(uint8_t dry)
{

      MYUSART_SendData(dry);
}

void subscribe_data_time(uint8_t tm)
{

      MYUSART_SendData(tm);
}


/***********************************************************
 *
  * @brief  Subscribe data power on off 
  * @param  data_frame: data frame
  * @retval None
  ***********************************************************/
void Subscribe_Data_QueryDev(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x06);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0);
    subscribe_data_frame_sum(0x51);
}
/***********************************************************
 *
  * @brief  Subscribe data power on off 
  * @param  data_frame: data frame
  * @retval None
  ***********************************************************/
void Subscribe_Data_PowerOn(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x02);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x55);


}

void Subscribe_Data_PowerOff(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x02);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x54);
    

}

void Subscribe_Data_PtcOn(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x05);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x58);
}

void Subscribe_Data_PtcOff(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x05);
    subscribe_data_frame_bool(0x00);
    subscribe_data_frame_sum(0x57);
}

void Subscribe_Data_SterilizationOn(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x07);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x5A);
}
void Subscribe_Data_SterilizationOff(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x07);
    subscribe_data_frame_bool(0x00);
    subscribe_data_frame_sum(0x59);
}

void Subscribe_Data_UltrasonicOn(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x09);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x5C);
}
void Subscribe_Data_UltrasonicOff(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x09);
    subscribe_data_frame_bool(0x00);
    subscribe_data_frame_sum(0x5b);
}

void Subscribe_Data_FanSpeedHigh(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x0B);
    subscribe_data_frame_bool(0x01);
    subscribe_data_frame_sum(0x5E);
}
void Subscribe_Data_FanSpeedLow(void)
{
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x0B);
    subscribe_data_frame_bool(0x00);
    subscribe_data_frame_sum(0x5D);
}

void Subscribe_Data_AppointmentTime(uint8_t sw, uint8_t mouse, uint8_t ster, uint8_t dry,uint8_t time)
{
    uint8_t temp;
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x0A);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x0D);
    subscribe_data_frame_bool(sw);
    subscribe_data_driver_mouse(mouse);
    subscribe_data_sterilization(ster);
    subscribe_data_dry(dry);
     subscribe_data_time(time);
    temp= 0x48+0x0A+0x02+0x01+0x0d+sw+mouse+ster+dry+time;
    subscribe_data_frame_sum(temp);
}
void Subscribe_Data_SetTemperatureValue(uint8_t temp)
{
    uint8_t temp_value;
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x0e);
    subscribe_data_frame_value(temp);
  
    temp_value= 0x48+0x07+0x02+0x01+temp;
    subscribe_data_frame_sum(temp_value);
}


void Subscribe_Data_SetTimerValue(uint8_t timing)
{
    uint8_t temp_value;
    subscribe_data_frame_head();
    subscribe_data_frame_len(0x07);
    subscribe_data_frame_type(0x02);
    subscribe_data_frame_numbers(0x01);
    subscribe_data_frame_oreder(0x0f);
    subscribe_data_frame_value(timing);
    temp_value= 0x48+0x07+0x02+0x01+timing;
    subscribe_data_frame_sum(temp_value);

}



