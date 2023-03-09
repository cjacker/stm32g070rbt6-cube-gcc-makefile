/***
	*************************************************************************************************
	*	@file  	main.c
	*	@version V1.0
	*  @date    2021-11-22
	*	@author  反客科技
	*	@brief   SPI驱动屏幕
   ************************************************************************************************
   *  @description
	*
	*	实验平台：反客STM32G070RBT6核心板 （型号：FK-G070M1-RBT6）+ 0.96寸液晶模块（型号：SPI096M1-160*80） 
	*	淘宝地址：https://shop212360197.taobao.com
	*	QQ交流群：536665479
	*
>>>>> 功能说明：
	*
	*	SPI驱动LCD进行简单的演示
	*
	************************************************************************************************
***/

#include "main.h"
#include "led.h"
#include "usart.h"
#include "key.h"   
#include "lcd_spi_096.h"

/********************************************** 函数声明 *******************************************/

void SystemClock_Config(void);		// 时钟初始化

// LCD测试函数，函数定义在底部
void 	LCD_Test_Clear(void);			// 清屏测试
void 	LCD_Test_Text(void);			   // 文本测试
void 	LCD_Test_Variable (void);	   // 变量显示，包括整数和小数
void 	LCD_Test_Color(void);			// 矩形填充测试
void 	LCD_Test_Grahic(void);		   // 2D图形绘制
void 	LCD_Test_Image(void);			// 图片显示
void  LCD_Test_Direction(void);	   // 更换显示方向

/***************************************************************************************************
*	函 数 名: main
*	入口参数: 无
*	返 回 值: 无
*	函数功能: 运行主程序
*	说    明: 无
****************************************************************************************************/

int main(void)
{
	HAL_Init();						// 初始化HAL库
	SystemClock_Config();		// 配置系统时钟，主频64MHz
	LED_Init();						// 初始化LED引脚
	USART1_Init();					// USART1初始化	
	KEY_Init();						// 初始化按键引脚

	SPI_LCD_Init();     			// 液晶屏以及SPI初始化  

	while (1)
	{
    printf("Hello World\r\n");
		LCD_Test_Clear(); 		// 清屏测试
		LCD_Test_Text();			// 文本测试
		LCD_Test_Variable ();	// 变量显示，包括整数和小数
		LCD_Test_Color();			// 颜色测试
		LCD_Test_Grahic();		// 2D图形绘制	
		LCD_Test_Image();			// 图片显示
		LCD_Test_Direction();	// 更换显示方向

		LED1_Toggle;		
	}
}

/****************************************************************************************************/
/**
  *   系统时钟配置：
  *
  *            System Clock source            	= PLLCLK 
  *            SYSCLK(Hz)                     	= 64000000 (CPU 主频 64MHz)
  *            HCLK(Hz)                       	= 64MHz
  *            AHB                   				= 64MHz
  *            APB             						= 64MHz
  *	
  *            HSE Frequency(Hz)              	= 25000000  (外部晶振频率 25MHz)
  *            PLL_M                          	= 5
  *            PLL_N                          	= 64
  *            PLL_R                          	= 5
  *
  *				CPU主频 = HSE Frequency / PLL_M * PLL_N / PLL_R = 25M /5*64/5 = 64M
  */   
/****************************************************************************************************/

void SystemClock_Config(void)	// 该函数由cubeMX生成，例程里所有外设时钟都放在这个函数
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV5;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Clear
*
*	函数功能:	清屏测试
*************************************************************************************************/
void LCD_Test_Clear(void)
{
	uint8_t	i = 0;			// 计数变量
	
	LCD_SetDirection(Direction_H_Flip);		
	LCD_SetTextFont(&CH_Font16);			// 设置2424中文字体,ASCII字体对应为2412
	LCD_SetColor(LCD_BLACK);				// 设置画笔颜色

	for(i=0;i<8;i++)
	{
		switch (i)		// 切换背景色
		{
			case 0: LCD_SetBackColor(LIGHT_RED); 		break;	
			case 1: LCD_SetBackColor(LIGHT_GREEN); 	break;				
			case 2: LCD_SetBackColor(LIGHT_BLUE); 		break;
			case 3: LCD_SetBackColor(LIGHT_YELLOW); 	break;
			case 4: LCD_SetBackColor(LIGHT_CYAN);		break;
			case 5: LCD_SetBackColor(LIGHT_GREY); 		break;
			case 6: LCD_SetBackColor(LIGHT_MAGENTA); 	break;
			case 7: LCD_SetBackColor(LCD_WHITE); 		break;			
			default:	break;			
		}
		LCD_Clear();		// 清屏
		LCD_DisplayText(13, 5,"STM32G0 刷屏测试");
		LCD_DisplayText(13,30,"屏幕分辨率:160*80");
		LCD_DisplayText(13,55,"控制器:ST7735");	
		HAL_Delay(1000);	// 延时
	}
}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Text
*
*	函数功能:	文本显示测试
*
*	说    明:	显示文本，包括各种字体大小的中文和ASCII字符 
*************************************************************************************************/
void LCD_Test_Text(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏

	LCD_SetColor(LCD_CYAN);   
	LCD_SetAsciiFont(&ASCII_Font12); LCD_DisplayString(0,  0,"ASCII:12*12"); 	
	LCD_SetAsciiFont(&ASCII_Font16); LCD_DisplayString(0, 12,"ASCII:16*16");																																		                                                                   					  		
	LCD_SetAsciiFont(&ASCII_Font20); LCD_DisplayString(0, 28,"ASCII:20*20");  
	LCD_SetColor(LCD_WHITE);				    		
	LCD_SetAsciiFont(&ASCII_Font24); LCD_DisplayString(0, 48,"ASCII:2424"); 
	LCD_SetColor(LCD_YELLOW);		
	LCD_SetAsciiFont(&ASCII_Font32); LCD_DisplayString(135, 25,"*");
	LCD_SetAsciiFont(&ASCII_Font32); LCD_DisplayString(128,  0,"32"); LCD_DisplayString(128,  48,"32");

	HAL_Delay(2000);	// 延时等待
	LCD_Clear(); 								// 清屏

	LCD_SetTextFont(&CH_Font12);			// 设置1212中文字体,ASCII字体对应为1206
	LCD_SetColor(0X8AC6D1);					// 设置画笔
	LCD_DisplayText(0, 0,"1212:反客科技");	
	
	LCD_SetTextFont(&CH_Font16);			// 设置1616中文字体,ASCII字体对应为1608
	LCD_SetColor(0XC5E1A5);					// 设置画笔
	LCD_DisplayText(0, 12,"1616:反客科技");		
	
	LCD_SetTextFont(&CH_Font20);			// 设置2020中文字体,ASCII字体对应为2010
	LCD_SetColor(0XFFB549);					// 设置画笔
	LCD_DisplayText(0, 28,"2020:反客科技");		

	LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
	LCD_SetColor(0XFF585D);					// 设置画笔
	LCD_DisplayText(0, 48,"2424:反客科技");		


	HAL_Delay(2000);	// 延时等待
}
/************************************************************************************************
*	函 数 名:	LCD_Test_Variable
*
*	函数功能:	变量显示，包括整数和小数
*************************************************************************************************/
void LCD_Test_Variable (void)
{
	uint16_t i;					// 计数变量
	int32_t	a = 0;			// 定义整数变量，用于测试
	int32_t	b = 0;			// 定义整数变量，用于测试
	int32_t	c = 0;			// 定义整数变量，用于测试

	double f = -1234.1234;	// 定义浮点数变量，用于测试
	
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	
   LCD_SetTextFont(&CH_Font16);     
	LCD_SetColor(LIGHT_CYAN);					// 设置画笔，蓝绿色		
	LCD_DisplayText(0, 0,"整数:");									
				
	LCD_SetColor(LIGHT_YELLOW);				// 设置画笔，亮黄色		
	LCD_DisplayText(0,20,"填充空格:");	
	LCD_DisplayText(0,40,"填充0:");	
	
	LCD_SetColor(LIGHT_RED);					// 设置画笔	，亮红色		
	LCD_DisplayText(0,60,"小数:");	

	
	for(i=0;i<100;i++)
   {
		LCD_SetColor(LIGHT_CYAN);								// 设置画笔	，蓝绿色	
		LCD_ShowNumMode(Fill_Space);							// 多余位填充空格
		LCD_DisplayNumber( 50,0, b+i*10, 4) ;				// 显示变量			
		LCD_DisplayNumber( 100,0, c-i*10, 4) ;				// 显示变量			
		
		LCD_SetColor(LIGHT_YELLOW);								// 设置画笔，亮黄色	

		LCD_ShowNumMode(Fill_Space);								// 多余位填充 空格
		LCD_DisplayNumber( 80,20, a+i*150, 8) ;				// 显示变量		

		LCD_ShowNumMode(Fill_Zero);								// 多余位填充0      
		LCD_DisplayNumber( 80,40, b+i*150, 8) ;				// 显示变量			
		
		LCD_SetColor(LIGHT_RED);									// 设置画笔，亮红色			
		LCD_ShowNumMode(Fill_Space);								// 多余位填充空格		
		LCD_DisplayDecimals( 50,60, f+i*0.01, 11,4);		// 显示小数		
		
		HAL_Delay(15);				
   }
	HAL_Delay(2500);		
}
/*************************************************************************************************
*	函 数 名:	LCD_Test_Color
*
*	函数功能:	颜色测
*************************************************************************************************/
void LCD_Test_Color(void)
{
	uint16_t i;					// 计数变量
	uint16_t y;
// 颜色测试>>>>>	
	LCD_SetBackColor(LCD_BLACK); //设置背景色
	LCD_Clear(); //清屏，刷背景色
	
	LCD_SetTextFont(&CH_Font16);			// 设置字体
	LCD_SetColor(LCD_WHITE);				// 设置画笔颜色
	LCD_DisplayText(0,0,"RGB三基色:");

	//使用画线函数绘制三基色色条
	for(i=0;i<160;i++)
	{
		LCD_SetColor( LCD_RED-(i<<16) );
      LCD_DrawLine_V(0+i,  20,10);
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_GREEN-(i<<8) );
      LCD_DrawLine_V(0+i,  30,10);
	}
	for(i=0;i<240;i++)
	{
		LCD_SetColor( LCD_BLUE-i );
      LCD_DrawLine_V(0+i,  40,10);
	}	

   y = 50;
   LCD_SetColor(LIGHT_CYAN);    LCD_FillRect(85,y+5     ,75,10);  LCD_DisplayString(0,y     ,"LIGHT_CYAN");	   
	LCD_SetColor(LIGHT_MAGENTA); LCD_FillRect(105,y+16*1+5,55,10);  LCD_DisplayString(0,y+16*1,"LIGHT_MAGENTA");	

	HAL_Delay(2000);
}

/*************************************************************************************************
*	函 数 名:	LCD_Test_Grahic
*
*	函数功能:	2D图形绘制
*************************************************************************************************/
void LCD_Test_Grahic(void)
{
	LCD_SetBackColor(LCD_BLACK); //设置背景色
	LCD_Clear(); //清屏，刷背景色	

	LCD_SetColor(LCD_WHITE);	
	LCD_DrawRect(0,0,160,80); 			//绘制矩形

	LCD_SetColor(LCD_RED);    LCD_FillCircle(50,40,30);		//填充圆形
	LCD_SetColor(LCD_GREEN);  LCD_FillCircle(80,40,30); 	
	LCD_SetColor(LCD_BLUE);   LCD_FillCircle(110,40,30);  	
	
	HAL_Delay(1000);	
	LCD_Clear(); //清屏，刷背景色	
	
	LCD_SetColor(LIGHT_CYAN);
	LCD_DrawCircle(80,40,30);			//绘制圆形
	LCD_DrawCircle(80,40,20);   

	LCD_SetColor(LIGHT_RED);	
	LCD_DrawEllipse(80,40,70,28); 	//绘制椭圆 
	LCD_DrawEllipse(80,40,60,35); 	//绘制椭圆    

	HAL_Delay(1000);		
}
/*************************************************************************************************
*	函 数 名:	LCD_Test_Image
*
*	函数功能:	图片显示测试
*************************************************************************************************/
void LCD_Test_Image(void)
{
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 10, 10, 60, 60, Image_Android_60x60) ;	   // 显示图片

	LCD_SetColor( 0xff9DD3A8);
	LCD_DrawImage( 90, 10, 60, 60, Image_Toys_60x60) ;		// 显示图片

	HAL_Delay(2000);	
}
/*************************************************************************************************
*	函 数 名:	LCD_Test_Direction
*
*	函数功能:	更换显示方向
*************************************************************************************************/
void  LCD_Test_Direction(void)
{
   LCD_SetDirection(Direction_V);		   
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	LCD_SetTextFont(&CH_Font12);  
	LCD_SetColor( 0xffDFF9FB);  	
	LCD_DisplayText(5,10,"Direction_V"); 	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
	LCD_SetTextFont(&CH_Font24);
	LCD_SetColor( 0xff9DD3A8);  
	LCD_DisplayText(16,112,"反客");
	LCD_DisplayText(16,136,"科技");
	HAL_Delay(1000);	// 延时	

   LCD_SetDirection(Direction_H);		   
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	LCD_SetTextFont(&CH_Font16);  
	LCD_SetColor( 0xffDFF9FB);  	
	LCD_DisplayText(20,0,"Direction_H"); 	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // 显示图片
	LCD_SetTextFont(&CH_Font24);
	LCD_SetColor( 0xff9DD3A8);  
	LCD_DisplayText(100,16,"反客");
	LCD_DisplayText(100,40,"科技");
	HAL_Delay(1000);	// 延时		

   LCD_SetDirection(Direction_V_Flip);		   
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	LCD_SetTextFont(&CH_Font16);  
	LCD_SetColor( 0xffDFF9FB);  	
	LCD_DisplayText(20,10,"_V_Flip"); 	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 10,50, 60, 60, Image_Android_60x60) ;	   // 显示图片
	LCD_SetTextFont(&CH_Font24);
	LCD_SetColor( 0xff9DD3A8);  
	LCD_DisplayText(16,112,"反客");
	LCD_DisplayText(16,136,"科技");
	HAL_Delay(1000);	// 延时		

   LCD_SetDirection(Direction_H_Flip);		   
	LCD_SetBackColor(LCD_BLACK); 			//	设置背景色
	LCD_Clear(); 								// 清屏
	LCD_SetTextFont(&CH_Font16);  
	LCD_SetColor( 0xffDFF9FB);  	
	LCD_DisplayText(20,0,"Direction_H_Flip"); 	
	LCD_SetColor( 0xffF6E58D);
	LCD_DrawImage( 30,20, 60, 60, Image_Android_60x60) ;	   // 显示图片
	LCD_SetTextFont(&CH_Font24);
	LCD_SetColor( 0xff9DD3A8);  
	LCD_DisplayText(100,16,"反客");
	LCD_DisplayText(100,40,"科技");
	HAL_Delay(1000);	// 延时		

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
