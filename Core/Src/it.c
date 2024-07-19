/*
 * it.c
 *
 *  Created on: Jul 17, 2024
 *      Author: daniel
 */

#include "main.h"

extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;

void SysTick_Handler(void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM1_UP_TIM10_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim10);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim11);
}

void TIM8_BRK_TIM12_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim12);
}

