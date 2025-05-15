#include "bsp.h"

#define MAX_BUFFER_SIZE  10

USART_WIFI_T usart_wifi_t;


uint8_t  inputBuf[5];
uint8_t  inputCmd[5];
uint8_t  wifiInputBuf[1];


static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

volatile uint8_t transOngoingFlag;
volatile uint8_t usart2_transOngoingFlag;

uint8_t receive_displayboard_state;

/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t state,wr_flag;
    if (huart->Instance == USART1) // Motor Board receive data
    {
   
        switch (state)
        {
        case 0:
            if (inputBuf[0] == 'T') // hex: 54 - "T"
            {
                wr_flag = 0;
                state = 1;
            }
            break;

        case 1:
            if (strchr("KORY", inputBuf[0])) // Check for valid characters
            {
                if (inputBuf[0] == 'O' || inputBuf[0] == 'R')
                {
                    inputCmd[0] = inputBuf[0];
                    wr_flag = 1;
                    state = 2;
                }
                else if (inputBuf[0] == 'Y')
                {
                    state = 0x0A;
                }
                else if(inputBuf[0] == 'K')
                {
                    state = 0x10;
                }
            }
            else
            {
                wr_flag = 0;
                state = 0;
            }
            break;

        case 2:
            inputCmd[wr_flag ? 1 : 0] = inputBuf[0]; //T K W 
            state = 3;
            break;
		case 3:
            inputCmd[wr_flag ? 2 : 1] = inputBuf[0];
            run_t.decodeFlag = 1;
            state = 0;
            break;

        case 0x0A:
            receive_displayboard_state = inputBuf[0];
            receive_copy_cmd(inputBuf[0]);
            state = 0;
            break;

		case 0x10:
			 inputCmd[0] = inputBuf[0]; //T K = W , M,T,C
			 state = 0x11;
             break;
		break;

		case 0x11:
			 inputCmd[1] = inputBuf[0]; //T K W 
			 run_t.decodeFlag = 1;
             state = 0;

		break;

        default:
            state = 0;
            run_t.decodeFlag = 0;
            break;
        }

        __HAL_UART_CLEAR_OREFLAG(&huart1);
        HAL_UART_Receive_IT(&huart1, inputBuf, 1); // Restart UART receive interrupt

	
  
 }
}
/********************************************
	*
	*Function Name:void Decode_Function(void)
    *Function: receive dsipay panel of order
    *Input Ref:NO
    *Return Ref:NO

*********************************************/ 
void Decode_Function(void)
{
   if(run_t.decodeFlag==1){
   
      run_t.decodeFlag =0;
	  run_t.process_run_guarantee_flag =1;
      Decode_RunCmd();
      
     }
}

#if 0

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void SendData_Copy_Cmd(uint8_t tdata)
{

        outputBuf[0]='M'; //4D
		outputBuf[1]='Y'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
	
		
		transferSize=3;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Reference_Data(uint8_t dry,uint8_t kill,uint8_t mouse)
{

	//crc=0x55;
	outputBuf[0]='M'; //master
	outputBuf[1]='R'; //outputBuf[1]='A'; //41
	outputBuf[2]=dry; //	// 'R' rotator motor for select filter
	outputBuf[3]=kill; // // one command parameter
	outputBuf[4]=mouse;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=5;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}
/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{

	//crc=0x55;
	outputBuf[0]='M'; //master
	outputBuf[1]='A'; //41
	outputBuf[2]='D'; //44	// 'D' data
	outputBuf[3]=hum; //	// 'R' rotator motor for select filter
	outputBuf[4]=temp; // // one command parameter
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=5;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}
void SendWifiData_To_TimerValue(uint8_t dat1)
{
   
	
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='T'; //44	// 'T' time
		outputBuf[3]=dat1; //	
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}



}
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiData_To_PanelTemp(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='P'; // 'T' time
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void SendWifiData_To_WifiSetTemp(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='E'; // 'T' time
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
{
   
	//crc=0x55;
		outputBuf[0]='M'; //
		outputBuf[1]='A'; //
		outputBuf[2]='S'; // wind speed
		outputBuf[3]=dat1; //	
	
		
		//for(i=3;i<6;i++) crc ^= outputBuf[i];
		//outputBuf[i]=crc;
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}


void SendWifiData_To_Cmd(uint8_t wdata)
{
  
	//crc=0x55;
			outputBuf[0]='M'; //4D
			outputBuf[1]='A'; //41
			outputBuf[2]='W'; //44	// wifi ->infomation link wifi 	
			outputBuf[3]=wdata; //
			//for(i=3;i<6;i++) crc ^= outputBuf[i];
			//outputBuf[i]=crc;
			transferSize=4;
			if(transferSize)
			{
				while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
				transOngoingFlag=1;
				HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
			}
}
/***************************************************************
 * 
 * Function: panel power on and special function
***************************************************************/
void SendWifiCmd_To_Order(uint8_t odata)
{
  

			outputBuf[0]='M'; //mainboard
			outputBuf[1]='A'; //A ->
			outputBuf[2]='C'; //'C' ->control 
			outputBuf[3]=odata; //	
		
			transferSize=4;
			if(transferSize)
			{
				while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
				transOngoingFlag=1;
				HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
			}



}

void SendData_Real_GMT(uint8_t hdata,uint8_t mdata,uint8_t sdata)
{
   

	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='B'; //44	// 'C' ->control 
	outputBuf[3]=hdata; //	
	outputBuf[4]=mdata; //
	outputBuf[5] =sdata;
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=6;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

}
#endif 



/**
 * @brief 通用发送数据函数
 * @param cmd1 第一个命令字节
 * @param cmd2 第二个命令字节
 * @param cmd3 第三个命令字节
 * @param data 数据数组
 * @param dataSize 数据长度
 */
void SendData(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3, uint8_t *data, uint8_t dataSize)
{
    outputBuf[0] = cmd1;
    outputBuf[1] = cmd2;

    uint8_t offset = 2; // 数据起始偏移量
    
    if (cmd3 !=0)
    {
        outputBuf[2] = cmd3;
        offset = 3;
    }

    for (uint8_t i = 0; i < dataSize; i++)
    {
        outputBuf[offset + i] = data[i];
    }

    transferSize = offset + dataSize;

    if (transferSize)
    {
        while (transOngoingFlag); // Wait for ongoing transmission to complete
        transOngoingFlag = 1;
        HAL_UART_Transmit_IT(&huart1, outputBuf, transferSize);
    }
}

void SendWifiData_To_Cmd(uint8_t wdata)
{
	//crc=0x55;
	outputBuf[0]='M'; //4D
	outputBuf[1]='A'; //41
	outputBuf[2]='W'; //44	// wifi ->infomation link wifi 	
	outputBuf[3]=wdata; //
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
}



/**
 * @brief 发送实时湿度和温度数据
 * @param hum 湿度值
 * @param temp 温度值
 */
void SendData_Copy_Cmd(uint8_t tdata)
{

	uint8_t data[] = {tdata};
	SendData('M', 'Y', 0, data, sizeof(data));


}
/**
 * @brief 发送实时湿度和温度数据
 * @param hum 湿度值
 * @param temp 温度值
 */
void SendData_Real_TimeHum(uint8_t hum, uint8_t temp)
{
    uint8_t data[] = {hum, temp};
    SendData('M', 'A', 'D', data, sizeof(data));
}

/***************************************************************
 * 
 * Function: panel power on and special function
***************************************************************/
void SendWifiCmd_To_Order(uint8_t odata)
{
   uint8_t data[] = {odata};
	SendData('M', 'A', 'C', data, sizeof(data));

}
/***************************************************************
 * 
 * Function: void SendWifiData_To_Cmd(uint8_t wdata)
***************************************************************/
//void SendWifiData_To_Cmd(uint8_t wdata)
//{
//     uint8_t data[] = {wdata};
//	 SendData('M', 'A', 'w', data, sizeof(data));
//
//}

/**
 * @brief 发送参考数据
 * @param dry 干燥值
 * @param kill 杀菌值
 * @param mouse 鼠标值
 */

void sendData_Reference_Data(uint8_t dry, uint8_t kill, uint8_t mouse)
{
    uint8_t data[] = {dry, kill, mouse};
    SendData('M', 'R', 0, data, sizeof(data));
}

/**
 * @brief 发送WiFi设置温度数据
 * @param temp 温度值
 */
void SendWifiData_To_WifiSetTemp(uint8_t temp)
{
    uint8_t data[] = {temp};
    SendData('M', 'A', 'E', data, sizeof(data));
}
/**
 * @brief 发送WiFi设置定时时间值
 * @param dat1 (timer value)
 */

void SendWifiData_To_TimerValue(uint8_t dat1)
{
	//uint8_t data[] = {dat1};
	//SendData('M', 'A', 'T', data, sizeof(data));
	    outputBuf[0]='M'; //4D
		outputBuf[1]='A'; //41
		outputBuf[2]='T'; //44	// 'T' time
		outputBuf[3]=dat1; //	
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}


}
void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
{

	uint8_t data[] = {dat1};
	SendData('M', 'A', 'S', data, sizeof(data));

}
/**
 * @brief UART传输完成回调函数
 * @param huart UART句柄指针
 */

void SendData_Real_GMT(uint8_t hdata,uint8_t mdata,uint8_t sdata)
{
	    uint8_t data[] = {hdata,mdata,sdata};
		SendData('M', 'A', 'B', data, sizeof(data));

}
/**
 * @brief UART传输完成回调函数
 * @param huart UART句柄指针
 */
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{
	uint8_t data[] = {hum,temp};
	SendData('M', 'A', 'D', data, sizeof(data));

}

void SendWifiData_To_PanelTemp(uint8_t dat1)
{

	uint8_t data[] = {dat1};
	SendData('M', 'A', 'P', data, sizeof(data));


}



/**
 * @brief UART传输完成回调函数
 * @param huart UART句柄指针
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        transOngoingFlag = 0; // Clear transmission flag
    }
}



/**
  * @brief  UART错误回调函数，处理USART1通信错误
  * @param  huart: UART句柄指针
  */

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
    

	if (huart->Instance == USART1) {
        // 重新初始化或报警
        #if 0
          __HAL_UART_CLEAR_OREFLAG(&huart1);
          __HAL_UART_CLEAR_NEFLAG(&huart1);
          __HAL_UART_CLEAR_FEFLAG(&huart1);
           
          
          temp=USART1->ISR;
          temp = USART1->RDR;
		  
     
		  UART_Start_Receive_IT(&huart1,inputBuf,1);
		 #endif 
	    /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
	      /* 2. 清空寄存器（简洁写法）*/
		    (void)huart->Instance->ISR;  // 清除状态标志
		    (void)huart->Instance->RDR;  // 清空接收数据

	    /* 3. 重启接收（带错误检查）*/
	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
	        HAL_UART_Receive_IT(huart, inputBuf, 1);  // 重新启动单字节中断接收
	    } else {
	        // 可选：硬件复位USART（严重错误时）
	        __HAL_UART_DISABLE(huart);
	        __HAL_UART_ENABLE(huart);
	        HAL_UART_Receive_IT(huart, inputBuf, 1);
	    }

	    /* 4. 可选：记录错误日志或触发报警 */
	   // Error_Counter++;  // 全局错误计数器
    }
	else if (huart->Instance == USART2){

		 /* 1. 清除所有可能出现的错误标志 */
	    // 使用单条语句清除多个标志（更高效）
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);

	    /* 2. 读取状态和数据寄存器（清空残留数据）*/
	    // 使用UNUSED宏避免编译器警告（如果不需要实际值）
	    //UNUSED(uint32_t temp_isr = huart->Instance->ISR);  // 读取ISR会清除部分标志
	    //UNUSED(uint32_t temp_rdr = huart->Instance->RDR);  // 清空接收寄存器
		  /* 2. 清空寄存器（简洁写法）*/
    (void)huart->Instance->ISR;  // 清除状态标志
    (void)huart->Instance->RDR;  // 清空接收数据

//		  /* 3. 重启接收（带错误检查）*/
//	    if (HAL_UART_GetState(huart) == HAL_UART_STATE_READY) {
//	          // 重新启动单字节中断接收
//	    }

	}
}





