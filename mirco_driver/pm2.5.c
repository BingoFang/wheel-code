#include "global_includes.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x_rcc.h"


typedef struct
{
	uint16_t Buffer_Len;
	uint16_t PM1_0_CF;
	uint16_t PM2_5_CF;
	uint16_t PM10_CF;
	uint16_t PM1_0;
	uint16_t PM2_5;
	uint16_t PM10;
	uint16_t Count0_3nm;
	uint16_t Count0_5nm;
	uint16_t Count1_0nm;
	uint16_t Count2_5nm;
	uint16_t Count5_0nm;
	uint16_t Count10nm;
}PM_Sensor_DataStruct;

typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

PM_Sensor_DataStruct	PM_Sensor_Data;
uint8_t 	PM_Sensor_RxBuffer[50];
uint16_t    PM_Sensor_RxTimeOut = 0;
uint16_t    PM_Sensor_RxCount = 0;

FlagStatus  PM_Sensor_RxFinish = RESET; 


void SysTick_Handler(void)  // SYS timer interrupt function 
{
	//OS_TimeMS ++;   //   ++1us for os timer
	//==========================================================================
	if(PM_Sensor_RxTimeOut != 0x00) // timeout for PM data receive
	{
		PM_Sensor_RxTimeOut--;
	}
	else
	{
		if((PM_Sensor_RxCount)&&(PM_Sensor_RxBuffer[0] == 'B')&&(PM_Sensor_RxBuffer[1] == 'M'))
		{
			PM_Sensor_RxCount = 0;
			PM_Sensor_RxFinish = SET;
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
		}
		else
		{
			PM_Sensor_RxCount = 0;
		}
	}
}

void USART1_IRQHandler(void)  // USART1 interrupt
{
	static uint8_t 		USART1_ByteData = 0;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART1_ByteData = USART_ReceiveData(USART1);

		if(PM_Sensor_RxFinish == RESET)
		{
			PM_Sensor_RxBuffer[PM_Sensor_RxCount++] = USART1_ByteData;
			PM_Sensor_RxTimeOut = 20;
		}

	}
	//============================================================================
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		USART1_ByteData = USART_ReceiveData(USART1); 
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}

	if(USART_GetFlagStatus(USART1, USART_FLAG_NE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_NE);
	}


	if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_FE);
	}

	if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_PE);
	}
}
//=============================================================================
//PM_USART1_Configuartion  USART1 configuration
//=============================================================================
void PM_USART1_Configuartion(void) 
{
  	USART_InitTypeDef USART_InitStructure;
  	GPIO_InitTypeDef  GPIO_InitStructure;

	 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |
							RCC_APB2Periph_GPIOC, ENABLE);

	 
  	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);

	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/* PA9 USART1_Tx */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//推挽输出-TX
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PA10 USART1_Rx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入-RX
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	USART_InitStructure.USART_BaudRate             = 9600;        					//波特率 
  	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;   		//设置数据长度为8bit 
  	USART_InitStructure.USART_StopBits             = USART_StopBits_1;      		//停止位为1 
  	USART_InitStructure.USART_Parity               = USART_Parity_No;       		//无校验位 
  	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//数据流控制为none 
  	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式都打开 

  	USART_Init(USART1, &USART_InitStructure); 						//初始化串口1

  	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 					// 接收接收中断

	USART_ITConfig(USART1, USART_IT_ERR | USART_IT_PE, ENABLE);

  	/* Enable the USART1 */ 
  	USART_Cmd(USART1, ENABLE);  									//使能USART1
}


//=============================================================================
//Check_PMSensor_DataValid    //
//=============================================================================
ErrorStatus	Check_PMSensor_DataValid(void)
{
	uint16_t 	Cal_CheckSum;
	uint16_t 	Buffer_CheckSum;
	uint16_t 	Buffer_Len;
	uint8_t 	i;
	ErrorStatus Result = ERROR;

	if((PM_Sensor_RxBuffer[0] == 'B')&&(PM_Sensor_RxBuffer[1] == 'M'))
	{
		Buffer_Len = (uint16_t)((PM_Sensor_RxBuffer[2] << 8) | PM_Sensor_RxBuffer[3]);

		Buffer_CheckSum = (uint16_t)((PM_Sensor_RxBuffer[Buffer_Len + 2] << 8) | PM_Sensor_RxBuffer[Buffer_Len + 3]);

		Cal_CheckSum = 0;
		for(i=0;i<(Buffer_Len + 2);i++)
		{
			Cal_CheckSum += PM_Sensor_RxBuffer[i];
		}

		if(Cal_CheckSum == Buffer_CheckSum)
			Result = SUCCESS;
	}
	return Result;
}
//=============================================================================
//PMSensor_DataReflash    //
//=============================================================================
void PMSensor_DataReflash(void)
{
	uint16_t Buffer_Len;

	memset(&PM_Sensor_Data,0,(sizeof(PM_Sensor_Data) - 2)); //PM_Sensor_Data.PM2_5_Old should not set to zero
	
	Buffer_Len = (uint16_t)((PM_Sensor_RxBuffer[2] << 8) | PM_Sensor_RxBuffer[3]);
	if(Buffer_Len == 28)   //PMS1003/5003
	{
		PM_Sensor_Data.Buffer_Len = 28;
		PM_Sensor_Data.PM1_0_CF = (uint16_t)((PM_Sensor_RxBuffer[4]<<8) | PM_Sensor_RxBuffer[5]);
		PM_Sensor_Data.PM2_5_CF = (uint16_t)((PM_Sensor_RxBuffer[6]<<8) | PM_Sensor_RxBuffer[7]);
		PM_Sensor_Data.PM10_CF 	= (uint16_t)((PM_Sensor_RxBuffer[8]<<8) | PM_Sensor_RxBuffer[9]);
		PM_Sensor_Data.PM1_0 	= (uint16_t)((PM_Sensor_RxBuffer[10]<<8) | PM_Sensor_RxBuffer[11]);
		PM_Sensor_Data.PM2_5 	= (uint16_t)((PM_Sensor_RxBuffer[12]<<8) | PM_Sensor_RxBuffer[13]);
		PM_Sensor_Data.PM10 	= (uint16_t)((PM_Sensor_RxBuffer[14]<<8) | PM_Sensor_RxBuffer[15]);
		PM_Sensor_Data.Count0_3nm = (uint16_t)((PM_Sensor_RxBuffer[16]<<8) | PM_Sensor_RxBuffer[17]);
		PM_Sensor_Data.Count0_5nm = (uint16_t)((PM_Sensor_RxBuffer[18]<<8) | PM_Sensor_RxBuffer[19]);
		PM_Sensor_Data.Count1_0nm = (uint16_t)((PM_Sensor_RxBuffer[20]<<8) | PM_Sensor_RxBuffer[21]);
		PM_Sensor_Data.Count2_5nm = (uint16_t)((PM_Sensor_RxBuffer[22]<<8) | PM_Sensor_RxBuffer[23]);
		PM_Sensor_Data.Count5_0nm = (uint16_t)((PM_Sensor_RxBuffer[24]<<8) | PM_Sensor_RxBuffer[25]);
		PM_Sensor_Data.Count10nm = (uint16_t)((PM_Sensor_RxBuffer[26]<<8) | PM_Sensor_RxBuffer[27]);
		
	}
	else if(Buffer_Len == 20)// PMS3003
	{
		PM_Sensor_Data.Buffer_Len = 20;
		PM_Sensor_Data.PM1_0_CF = (uint16_t)((PM_Sensor_RxBuffer[4]<<8) | PM_Sensor_RxBuffer[5]);
		PM_Sensor_Data.PM2_5_CF = (uint16_t)((PM_Sensor_RxBuffer[6]<<8) | PM_Sensor_RxBuffer[7]);
		PM_Sensor_Data.PM10_CF 	= (uint16_t)((PM_Sensor_RxBuffer[8]<<8) | PM_Sensor_RxBuffer[9]);
		PM_Sensor_Data.PM1_0 	= (uint16_t)((PM_Sensor_RxBuffer[10]<<8) | PM_Sensor_RxBuffer[11]);
		PM_Sensor_Data.PM2_5 	= (uint16_t)((PM_Sensor_RxBuffer[12]<<8) | PM_Sensor_RxBuffer[13]);
		PM_Sensor_Data.PM10 	= (uint16_t)((PM_Sensor_RxBuffer[14]<<8) | PM_Sensor_RxBuffer[15]);
		PM_Sensor_Data.Count0_3nm = 0;
		PM_Sensor_Data.Count0_5nm = 0;
		PM_Sensor_Data.Count1_0nm = 0;
		PM_Sensor_Data.Count2_5nm = 0;
		PM_Sensor_Data.Count5_0nm = 0;
		PM_Sensor_Data.Count10nm = 0;
	}
}

int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
		this is done through SystemInit() function which is called from startup
		file (startup_stm32f10x_xx.s) before to branch to application main.
		To reconfigure the default setting of SystemInit() function, refer to
		system_stm32f10x.c file
	*/ 
	SetSysClockInternal();  // sysclock for internal  RC
    /**/    
	SysTick_Init();

	/**/
	NVIC_Configuration();

	/**/
	PM_USART1_Configuartion();
	
	/**/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

	/* Init the STemWin GUI Library */
	LCD_Init();

	/*开始接收PM2.5传感器数据*/
	PM_Sensor_RxFinish = RESET;
	PM_Sensor_RxCount = 0;

	/* Infinite loop */
	while (1)
	{		         
		if(PM_Sensor_RxFinish == SET)
		{
			if(Check_PMSensor_DataValid())
			{
				PMSensor_DataReflash();
				LCD_Display(PM_Sensor_Data.PM2_5); // display PM2.5 on 	LCD
			}
			PM_Sensor_RxFinish = RESET;
			USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		}
		Delay_Ms(1000);
	}		
}




