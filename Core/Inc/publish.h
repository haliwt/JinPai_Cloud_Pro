#ifndef __PUBLISH_H_
#define __PUBLISH_H_
#include "main.h"


void Publish_Data_Power(uint8_t cmd);
void Publish_Data_Plasma(uint8_t cmd);
void Publish_Data_PTC(uint8_t cmd);
void Publish_Data_Mouse(uint8_t cmd);
void Publish_Data_SetTiming(uint8_t timing);
void Publish_Data_RealTemp(uint8_t temp);
void Publish_Data_RealHumidity(uint8_t hum);





#endif 


