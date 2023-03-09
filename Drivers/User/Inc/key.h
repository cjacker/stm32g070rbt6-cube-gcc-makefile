#ifndef __KEY_H
#define __KEY_H

#include "stm32g0xx_hal.h"

#define	KEY_ON	 1		//鎸夐敭鎸変笅
#define	KEY_OFF	 0		//鎸夐敭鏀惧紑

/*------------------------------------------ IO鍙ｉ厤缃畯 ----------------------------------*/


#define 	KEY_PIN           					GPIO_PIN_13        				 // KEY 寮曡剼      
#define	KEY_PORT          					GPIOC                 			 // KEY GPIO绔彛     
#define 	__HAL_RCC_KEY_CLK_ENABLE         __HAL_RCC_GPIOC_CLK_ENABLE()	 // KEY GPIO绔彛鏃堕挓

/*------------------------------------------ 鍑芥暟澹版槑 ------------------------------------*/

void 		KEY_Init(void);	//鎸夐敭IO鍙ｅ垵濮嬪寲鍑芥暟
uint8_t	KEY_Scan(void);   //鎸夐敭鎵弿

#endif //__KEY_H











