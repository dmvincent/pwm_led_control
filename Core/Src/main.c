/*
 * main.c
 *
 *  Created on: Jul 17, 2024
 *      Author: daniel
 */

#include "main.h"

TIM_OC_InitTypeDef sConfig_nonConst;

TIM_Base_InitTypeDef Timer_Init;


// Define global HandleTypeDef structures
UART_HandleTypeDef huart6;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;

// Define global message array to be used for transmitting over UART6
char msg[200];


int main() {

	// Init Hal
	HAL_Init();
	  memset(&htim12, 0, sizeof(htim12));
	  memset(&htim10, 0, sizeof(htim10));
	  memset(&htim11, 0, sizeof(htim11));

	// Configure the System Clock
	SystemClock_Config();

	// Init UART6 TX to print messages to USER minicom session
	UART6_Init();

	memset(msg, 0, strlen(msg));
	sprintf(msg, "\e[1;1H\e[2J");
	if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	}
	// Initialize GPIO Button
	BTN_GPIO_Init();
	  while(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1) == GPIO_PIN_SET);
	  wait();
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "\nUse the PWM Mode of the Timer's Output Compare Unit to independently control the\nbrightness of 4 LED's on the following channels:\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "------------------------------------------------------------------\r\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "\tTIM12_Channel2 -> Logic_Analyzer D0\n\tTIM10_Channel1 -> Logic_Analyzer D1\n\tTIM11_Channel1 -> Logic_Analyzer D2\n\tTIM12_Channel1 -> Logic_Analyzer D3\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }

	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "==================================================================\r\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
		// Notify User Timer PWM has been initialized and is just waiting to begin
		memset(msg, 0, strlen(msg));
		sprintf(msg, "\nClick the PWR/START button to being PWM\n");
		if(HAL_UART_Transmit(&huart6, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
			Error_Handler();
		}
	 while(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1) == GPIO_PIN_SET);
   wait();




	  /***********************************************************************
	   ***************             Initialize PWM            ***************
	   ***********************************************************************/
		// Start Timer Peripheral Clock
		__HAL_RCC_TIM10_CLK_ENABLE();
		__HAL_RCC_TIM11_CLK_ENABLE();
		__HAL_RCC_TIM12_CLK_ENABLE();
	  TIM_TypeDef *timerBaseAddr;
	  uint32_t Channel;
	  memset(&Timer_Init, 0, sizeof(Timer_Init));
	  Timer_Init.Prescaler = 4;
	  //Timer_Init.Prescaler = 4999;
	  Timer_Init.Period = 10000-1;

	  /*
	   * Configure TIM Output Compare structure and clear
	   */
	  memset(&sConfig_nonConst, 0, sizeof(sConfig_nonConst));
	  sConfig_nonConst.OCMode = TIM_OCMODE_PWM1;
	  sConfig_nonConst.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfig_nonConst.Pulse = 1000;


	  for(int i = 0; i < 4; i++){
		  if(i == 0) {
			  timerBaseAddr = TIM12;
			  Channel = TIM_CHANNEL_2;
			  //sConfig_nonConst.Pulse =(Timer_Init.Prescaler*25)/100;
			  PWM_Config(timerBaseAddr, sConfig_nonConst, Channel, i);

		  }
		  else if( i == 1 ) {
			  timerBaseAddr = TIM10;
			  Channel = TIM_CHANNEL_1;
			  //sConfig_nonConst.Pulse = (Timer_Init.Prescaler*45)/100;
			  PWM_Config(timerBaseAddr, sConfig_nonConst, Channel, i);
		  }
		  else if( i == 2 ) {
			  timerBaseAddr = TIM11;
			  Channel = TIM_CHANNEL_1;
			  //sConfig_nonConst.Pulse = (Timer_Init.Prescaler*75)/100;
			  PWM_Config(timerBaseAddr, sConfig_nonConst, Channel, i);
		  }
		  else {
			  timerBaseAddr = TIM12;
			  Channel = TIM_CHANNEL_1;
			  sConfig_nonConst.Pulse = (Timer_Init.Prescaler*90)/100;
			  PWM_Config(timerBaseAddr, sConfig_nonConst, Channel, i);
		  }

	 }


	  /***********************************************************************
	   ***************               Start Timer               ***************
	   ***********************************************************************/
	  if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  uint8_t brightness12_2Delta = 100;
	  uint8_t brightness10_1Delta = 200;
	  uint8_t brightness11_1Delta = 300;
	  uint8_t brightness12_1Delta = 400;

	  uint16_t brightness12_2 = 1000 + brightness12_2Delta;
	  uint16_t brightness10_1 = 1000 + brightness10_1Delta;
	  uint16_t brightness11_1 = 1000 + brightness11_1Delta;
	  uint16_t brightness12_1 = 1000 + brightness12_1Delta;


	  while(1) {
		  while(htim12.Instance->CCR2 < 10000) {
			  brightness12_2+=brightness12_2Delta;
			  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, brightness12_2);
			  if (brightness10_1+brightness10_1Delta > 10000) {
				  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, brightness10_1-=brightness10_1Delta);
			  }
			  else
				  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, brightness10_1+=brightness10_1Delta);
			  if (brightness11_1+brightness11_1Delta > 10000) {
				  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, brightness11_1-=brightness11_1Delta);
			  }
			  else
				  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, brightness11_1+=brightness11_1Delta);
			  if (brightness12_1+brightness12_1Delta > 10000) {
				  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, brightness12_1-=brightness12_1Delta);
			  }
			  else
				  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, brightness12_1+=brightness12_1Delta);
			  HAL_Delay(1);
		  }
		  while(htim12.Instance->CCR2 > 1000) {
			  brightness12_2-=brightness12_2Delta;
			  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, brightness12_2);
			  if (brightness10_1-brightness10_1Delta < 1000) {
				  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, brightness10_1+=brightness10_1Delta);
			  }
			  else
				  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, brightness10_1-=brightness10_1Delta);
			  if (brightness11_1-brightness11_1Delta < 1000) {
				  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, brightness11_1+=brightness11_1Delta);
			  }
			  else
			  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, brightness11_1-=brightness11_1Delta);
			  if (brightness12_1-brightness12_1Delta < 1000) {
				  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, brightness12_1+=brightness12_1Delta);
			  }
			  else
			  __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, brightness12_1-=brightness12_1Delta);
			  HAL_Delay(1);
		  }

	  }


	return 0;
}

void SystemClock_Config() {

	RCC_OscInitTypeDef oscInitStruct;
	RCC_ClkInitTypeDef clockInitStruct;

	oscInitStruct.HSEState = RCC_HSE_BYPASS;
	oscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscInitStruct.PLL.PLLState = RCC_PLL_ON;
	oscInitStruct.PLL.PLLM = 19;
	oscInitStruct.PLL.PLLN = 152;
	oscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	if( HAL_RCC_OscConfig(&oscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	clockInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clockInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clockInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clockInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	clockInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if( HAL_RCC_ClockConfig(&clockInitStruct, FLASH_ACR_LATENCY_1WS) != HAL_OK) {
		Error_Handler();
	}

	// Turn of HSI to save power
	__HAL_RCC_HSI_DISABLE();

	// Reconfigure Systick now to work with the new System Clock Frequency
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void PWM_Config(TIM_TypeDef *Instance, TIM_OC_InitTypeDef sConfig_nonConst, uint32_t Channel, int i) {


	if(i == 0 || i == 3) {
		htim12.Instance = Instance;
		htim12.Init = Timer_Init;
		if(HAL_TIM_PWM_Init(&htim12) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_PWM_ConfigChannel(&htim12, &sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
	else if(i == 1) {
		htim10.Instance = Instance;
		htim10.Init = Timer_Init;
		if(HAL_TIM_PWM_Init(&htim10) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_PWM_ConfigChannel(&htim10, &sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
	else {
		htim11.Instance = Instance;
		htim11.Init = Timer_Init;
		if(HAL_TIM_PWM_Init(&htim11) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_PWM_ConfigChannel(&htim11, &sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
}

void UART6_Init(void) {
	// Start UART6 Clock
	__HAL_RCC_USART6_CLK_ENABLE();

	huart6.Instance = USART6;
	huart6.Init.Mode = UART_MODE_TX;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if ( HAL_UART_Init(&huart6) != HAL_OK ) {
		Error_Handler();
	}
}

void BTN_GPIO_Init(void) {
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	GPIO_InitTypeDef gpioJ_Init;
	memset(&gpioJ_Init, 0, sizeof(gpioJ_Init));
	gpioJ_Init.Pin = GPIO_PIN_1;
	gpioJ_Init.Mode = GPIO_MODE_INPUT;
	gpioJ_Init.Pull = GPIO_PULLUP;
	gpioJ_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOJ, &gpioJ_Init);

}

void wait(void) {
	  uint32_t waitVar =(uint32_t)0xfffff;
	  while(waitVar > 0) {
		  waitVar--;
	  }
}

void Error_Handler(void) {
	while(1);
}
