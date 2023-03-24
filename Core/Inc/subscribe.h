#ifndef __SUBSCRIBE_H_
#define __SUBSCRIBE_H_
#include "main.h"

void Subscribe_Data_PowerOn(void);
void Subscribe_Data_PowerOff(void);

void Subscribe_Data_PtcOn(void);
void Subscribe_Data_PtcOff(void);

void Subscribe_Data_SterilizationOn(void);
void Subscribe_Data_SterilizationOff(void);

void Subscribe_Data_UltrasonicOn(void);
void Subscribe_Data_UltrasonicOff(void);

void Subscribe_Data_FanSpeedHigh(void);
void Subscribe_Data_FanSpeedLow(void);

void Subscribe_Data_AppointmentTime(uint8_t sw, uint8_t ul, uint8_t ster, uint8_t dry );

void Subscribe_Data_SetTemperatureValue(uint8_t temp);


void Subscribe_Data_SetTimerValue(uint8_t pressure);



#endif 
