/*
 * main.h
 *
 *  Created on: Jul 17, 2024
 *      Author: daniel
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdio.h>
#include <string.h>
#include "stm32f7xx.h"

#define TRUE	1
#define FALSE	0

void SystemClock_Config(void);


void Timer_Config(TIM_TypeDef *Instance, TIM_OC_InitTypeDef *sConfig_nonConst, uint32_t Channel, int i);
void UART6_Init(void);
void BTN_GPIO_Init(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void PWM_Config(TIM_TypeDef *Instance, TIM_OC_InitTypeDef sConfig_nonConst, uint32_t Channel, int i);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
void wait(void);
void Error_Handler(void);

#endif /* INC_MAIN_H_ */
