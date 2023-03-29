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
