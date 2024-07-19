/*
 * msp.c
 *
 *  Created on: Jul 17, 2024
 *      Author: daniel
 */

#include "main.h"

void HAL_MspInit()
{
	// Perform the low level processor specific inits here using processor specific API's provided by the Cube HAL layer in Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= (0x7 << 16);

	// 3. Configure the prority for the system
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// Start GPIOC Clock
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef gpio_uart_init;
	gpio_uart_init.Pin = GPIO_PIN_6;
	gpio_uart_init.Mode = GPIO_MODE_AF_PP;
	gpio_uart_init.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &gpio_uart_init);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	/*
	 * Configure:
	 * 				- PB15->12_2(D11)
	 * 				- PF6->10_1(D3)
	 * 				- PF7->11_1(D6)
	 * 				- PH6->12_1(D9)
	 */

	/***********************************************************************
	 ***************          Configure All GPIOCLK          ***************
	 ***********************************************************************/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();


	/***********************************************************************
	 ***************          Configure PB15 for AF9          ***************
	 ***********************************************************************/

	/*
	 * Initialize and populate member of InitTypeDef structure for GPIO Peripheral
	 */
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init, 0, sizeof(GPIO_Init));
	GPIO_Init.Pin = GPIO_PIN_15;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Pull = GPIO_PULLDOWN;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_Init.Alternate = GPIO_AF9_TIM12;

	/*
	 * Inititialize GPIOF Peripheral
	 */
	HAL_GPIO_Init(GPIOB, &GPIO_Init);


	/***********************************************************************
	 ***************          Configure PF6 / PF7 for AF3          ***************
	 ***********************************************************************/

	/*
	 * Initialize and populate member of InitTypeDef structure for GPIO Peripheral
	 */
	memset(&GPIO_Init, 0, sizeof(GPIO_Init));
	GPIO_Init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Alternate = GPIO_AF3_TIM10 | GPIO_AF3_TIM11;

	/*
	 * Inititialize GPIOF Peripheral
	 */
	HAL_GPIO_Init(GPIOF, &GPIO_Init);

	/***********************************************************************
	 ***************          Configure PH6 for AF9          ***************
	 ***********************************************************************/

	/*
	 * Initialize and populate member of InitTypeDef structure for GPIO Peripheral
	 */
	memset(&GPIO_Init, 0, sizeof(GPIO_Init));
	GPIO_Init.Pin = GPIO_PIN_6;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Alternate = GPIO_AF9_TIM12;

	/*
	 * Inititialize GPIOF Peripheral
	 */
	HAL_GPIO_Init(GPIOH, &GPIO_Init);

	/***********************************************************************
	 ***************           Set and Enable IRQ's          ***************
	 ***********************************************************************/

	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);

	HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);


}
