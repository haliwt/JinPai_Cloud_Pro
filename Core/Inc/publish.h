#ifndef __PUBLISH_H_
#define __PUBLISH_H_
#include "main.h"


//send one byte 
void MYUSART_SendData(uint8_t  data);

void Publish_Data_AllRef(void);

void Publish_Data_ProdKey(void);
void Publish_Command_SmartCofnig(void);
void Publish_Command_Query(void);

#endif 


