/***
	************************************************************************************************
	*	@file  	usart.c
	*	@version V1.0
	*  @date    2021-11-12
	*	@author  反客科技	
	*	@brief   串口打印测试
   *************************************************************************************************
   *  @description
	*
	*	实验平台：反客 STM32G0系列 核心板 
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 文件说明：
	*
	*  初始化usart引脚，配置波特率等参数
	*
	************************************************************************************************
***/


#include "usart.h"



UART_HandleTypeDef huart1;  // UART_HandleTypeDef 结构体变量


/*************************************************************************************************
*	函 数 名:	HAL_UART_MspInit
*	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
*	返 回 值:	无
*	函数功能:	初始化串口引脚
*	说    明:	无		
*************************************************************************************************/

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if(huart->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();		// 开启 USART1 时钟

		GPIO_USART1_TX_CLK_ENABLE;				// 开启 USART1 TX 引脚的 GPIO 时钟
		GPIO_USART1_RX_CLK_ENABLE;				// 开启 USART1 RX 引脚的 GPIO 时钟
		
		/** USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX	*/
		
		GPIO_InitStruct.Pin 			= USART1_TX_PIN;					// TX引脚
		GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;				// 复用推挽输出
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;						// 上拉
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// 速度等级 
		GPIO_InitStruct.Alternate 	= GPIO_AF1_USART1;				// 复用为USART1
		HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin 			= USART1_RX_PIN;					// RX引脚
		HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);		
	}

}

/*************************************************************************************************
*	函 数 名:	USART1_Init
*	入口参数:	无
*	返 回 值:	无
*	函数功能:	初始化串口配置
*	说    明:	无		 
*************************************************************************************************/

void USART1_Init(void)
{
	huart1.Instance				 			= USART1;                        // 串口1
	huart1.Init.BaudRate 					= 115200;                        // 波特率
	huart1.Init.WordLength 					= UART_WORDLENGTH_8B;            // 8位数据宽度
	huart1.Init.StopBits 					= UART_STOPBITS_1;               // 停止位1
	huart1.Init.Parity 						= UART_PARITY_NONE;              // 不使用奇偶校验
	huart1.Init.Mode 							= UART_MODE_TX_RX;               // 全双工模式
	huart1.Init.HwFlowCtl 					= UART_HWCONTROL_NONE;           // 不使用硬件流控制
	huart1.Init.OverSampling 				= UART_OVERSAMPLING_16;          // 16倍过采样模式
	huart1.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE;   // 禁止单次采样，使用3次采样，确保数据更加可靠
	huart1.Init.ClockPrescaler 			= UART_PRESCALER_DIV1;           // 内核时钟分频系数，最大为64M
	huart1.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT;			// 不使用进阶功能

	HAL_UART_Init(&huart1);		// 初始化串口配置

}

/*************************************************************************************************
*	函 数 名:	fputc
*	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
*	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
*	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
*	说    明:	无		
*************************************************************************************************/

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE* f)
#endif


PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
  return (ch);
}

GETCHAR_PROTOTYPE
{
  HAL_StatusTypeDef status = HAL_BUSY;
  uint8_t data;
  while(status != HAL_OK)
    status = HAL_UART_Receive(&huart1, &data, 1, HAL_MAX_DELAY);
  return (data);
}

