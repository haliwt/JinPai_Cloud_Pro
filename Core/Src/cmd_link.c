#include "cmd_link.h"
#include <string.h>
#include "usart.h"
#include "run.h"
#include "fan.h"
#include "esp8266.h"
#include "interrupt_manager.h"
#include "wifi_fun.h"

#define MAX_BUFFER_SIZE  30

USART_WIFI_T usart_wifi_t;


uint8_t  inputBuf[5];
uint8_t  inputCmd[5];
uint8_t  wifiInputBuf[1];
uint8_t test_counter;
uint8_t test_counter_usat1;




static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

volatile uint8_t transOngoingFlag;
volatile uint8_t usart2_transOngoingFlag;


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
 

	if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
        test_counter_usat1++;
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 'T')  //hex :54 - "T" -fixed

			    wr_flag =0;
				state=1; //=1
		break;
		case 1: //#1
             if(inputBuf[0] == 'K' || inputBuf[0]=='O' || inputBuf[0]=='R')  {//hex :4B - "K" -fixed
                if(inputBuf[0]=='O' || inputBuf[0]=='R'){
                       inputCmd[0]= inputBuf[0];
					   wr_flag=1;
					   
				}
				state=2; //=1

             }
			else{
			   wr_flag =0;
			   state =0;
			}
			break;
            
        case 2:
			 if(wr_flag == 1){
			    inputCmd[1]= inputBuf[0];

			 }
			 else
             	inputCmd[0]= inputBuf[0];
             state = 3;
        
        break;
        
        case 3:
		     if(wr_flag ==1){
	           inputCmd[2]= inputBuf[0];
		     }
			 else
			  inputCmd[1]= inputBuf[0];
	         run_t.decodeFlag =1;
			
	         state = 0;
        
        break;

	
	
		default:
			state=0;
			run_t.decodeFlag =0;
		}
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
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
/********************************************
	*
	*Function Name:void Decode_Function(void)
    *Function: receive dsipay panel of order
    *Input Ref:NO
    *Return Ref:NO

*********************************************/ 
void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{
   uint32_t temp;
   static uint8_t error_usart_flag;
  
   if(huart->Instance==USART1){
    

	  if(run_t.gTimer_usart_error >260){
	  	run_t.gTimer_usart_error=0;
	      __HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE);//UART_FLAG_NE
         __HAL_UART_GET_FLAG(&huart1,UART_FLAG_NE); //USART_ISR_FE
         __HAL_UART_GET_FLAG(&huart1,USART_ISR_FE);
         if(UART_FLAG_ORE==1 || UART_FLAG_NE==1 ||USART_ISR_FE==1  ||error_usart_flag ==1){
           __HAL_UART_CLEAR_OREFLAG(&huart1);
              __HAL_UART_CLEAR_NEFLAG(&huart1);
               __HAL_UART_CLEAR_FEFLAG(&huart1);
             error_usart_flag=0;
          
          temp=USART1->ISR;
          temp = USART1->RDR;
		 
          
	       UART_Start_Receive_IT(&huart1,inputBuf,1);
          
		  
          
         }
	  	}
         
     
        
     if(run_t.process_run_guarantee_flag ==1){
        run_t.process_run_guarantee_flag=0;
       run_t.iwdg_feed_success_flag =1;
       run_t.gTimer_check_iwdg_flag =0;
       
      }
    
      if(run_t.gTimer_iwdg > 210){
          run_t.gTimer_iwdg = 0;
        // SendWifiCmd_To_Order(0xff);
     }
     if(run_t.gTimer_check_iwdg_flag >250){
         run_t.gTimer_check_iwdg_flag=0;
         if(run_t.iwdg_feed_success_flag==1){
            run_t.iwdg_feed_success_flag=0;
            error_usart_flag=0;
         
         }
         else{
             error_usart_flag=1;
		    
         
         }

      }
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
	outputBuf[1]='A'; //41
	outputBuf[2]='R'; //
	outputBuf[3]=dry; //	// 'R' rotator motor for select filter
	outputBuf[4]=kill; // // one command parameter
	outputBuf[5]=mouse;
	
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
void SendWifiData_To_PanelTime(uint8_t dat1)
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



/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

//	if(huart== &huart2){
//
//       usart2_transOngoingFlag =0;
//
//	}

}





