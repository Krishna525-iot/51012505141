/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>      // Needed for printf
#include "gesture.h"  // Ensure this header is included
//#include "gesture.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* Definitions for GestureManager */
osThreadId_t GestureManagerHandle;
const osThreadAttr_t GestureManager_attributes = {
  .name = "GestureManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for InputManager */
osThreadId_t InputManagerHandle;
const osThreadAttr_t InputManager_attributes = {
  .name = "InputManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
void GestureManagerTask(void *argument);
void InputManagerTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t ges_type = 0;
void MyGestureEventHandler(InputAction event) {
	InputEvent input_event = { .source = INPUT_SOURCE_GESTURE };

	// Map gesture events to input actions
	switch (event) {
	case INPUT_MOVE_DOWN:
		input_event.action = INPUT_MOVE_DOWN;
		ges_type = 1;
		break;

	case INPUT_MOVE_UP:
		input_event.action = INPUT_MOVE_UP;
		ges_type = 0;
		break;

	case INPUT_BACK:
		input_event.action = INPUT_BACK;
		ges_type = 2;
		break;

	case INPUT_SELECT:
		input_event.action = INPUT_SELECT;
		ges_type = 3;
		break;

	default:
		return; // Ignore unknown gestures
	}

	// Enqueue the mapped input event
	enqueue_event(input_event);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	ButtonManager_HandleInterrupt(GPIO_Pin); // Forward the interrupt to the Button Manager
}

void MyButtonEventHandler(ButtonEvent *event) {
	InputEvent input_event = { .source = INPUT_SOURCE_BUTTON }; // For input manager add data to queue

	// Map button ID and action to input events
	switch (event->action) {
	case BUTTON_ACTION_SHORT_PRESS:
		input_event.action =
				(event->button_id == 1) ? INPUT_MOVE_DOWN :
				(event->button_id == 0) ? INPUT_MOVE_UP :
				(event->button_id == 2) ? INPUT_SELECT :
				(event->button_id == 3) ? INPUT_BACK : INPUT_DEPTH; // DEPTH for button_id 4
		break;

//	case BUTTON_ACTION_LONG_PRESS:
//		input_event.action =
//				(event->button_id == 0) ? INPUT_LONG_MOVE_DOWN :
//				(event->button_id == 1) ? INPUT_LONG_MOVE_UP :
//				(event->button_id == 2) ? INPUT_LONG_SELECT :
//				(event->button_id == 3) ? INPUT_LONG_BACK : INPUT_LONG_DEPTH; // Long DEPTH for button_id 4
//		break;
	}

	enqueue_event(input_event); // Enqueue the mapped input event
}


int __io_putchar(int ch) {
//    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

extern volatile uint8_t newData;

void ResetUART(void) {
	 HAL_UART_DeInit(&huart1);
	 MX_USART1_UART_Init();
	  HAL_UART_Receive_IT(&huart1, getRxBuffer(), 1);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
//  ResetUART();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of GestureManager */
  GestureManagerHandle = osThreadNew(GestureManagerTask, NULL, &GestureManager_attributes);

  /* creation of InputManager */
  InputManagerHandle = osThreadNew(InputManagerTask, NULL, &InputManager_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	ButtonManager_Init(MyButtonEventHandler);

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */

	 UART_Packet_Init(&huart1);
//	 initI2CSemaphore();  // Create the semaphore
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (isDataReady()) {
			uint8_t *rxData = getRxBuffer(); // Retrieve the buffer pointer
			process_uart_command((const char *)rxData);
			resetDataReady();
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10805D88;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin
                          |LCD_EN_Pin|LCD_RS_Pin|BUZZER_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin
                           LCD_EN_Pin LCD_RS_Pin BUZZER_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin
                          |LCD_EN_Pin|LCD_RS_Pin|BUZZER_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin PB1 */
  GPIO_InitStruct.Pin = LED1_Pin|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STANDBY_Pin S_PRV_Pin S_NEXT_Pin DEPTH_Pin
                           CHANGE_N_Pin */
  GPIO_InitStruct.Pin = STANDBY_Pin|S_PRV_Pin|S_NEXT_Pin|DEPTH_Pin
                          |CHANGE_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_N_Pin */
  GPIO_InitStruct.Pin = IR_N_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IR_N_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CHANGE_P_Pin */
  GPIO_InitStruct.Pin = CHANGE_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CHANGE_P_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GESTURE_INT_Pin */
  GPIO_InitStruct.Pin = GESTURE_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GESTURE_INT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/* USER CODE BEGIN Header_GestureManagerTask */


int8_t mylastgesture = 0;

/**
 * @brief  Function implementing the GestureManager thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_GestureManagerTask */
void GestureManagerTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
//	LCD_Init();
//	init_screen1(0);
	//----------------
	init_gesture();

//	sensor_state = 1;
	for (;;) {

		if (isDataReady()) {
			uint8_t *rxData = getRxBuffer(); // Retrieve the buffer pointer
			process_uart_command((const char *)rxData);
			resetDataReady();
		}
		process_input_events();
		//------------------------
		init_gesture();
		if (sensor_state == 1)
		{
			gestureAvailable();
		       if(newData )
		       {

		    	   mylastgesture = readGesture();
		    	   MyGestureEventHandler(mylastgesture);
		    	   newData = 0;
		       }
		}


		osDelay(pdMS_TO_TICKS(5));
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_InputManagerTask */
/**
 * @brief Function implementing the InputManager thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_InputManagerTask */
void InputManagerTask(void *argument)
{
  /* USER CODE BEGIN InputManagerTask */
	LCD_Init();
	init_screen1(0);
	/* Infinite loop */
	for (;;) {
//		if (isDataReady()) {
//			uint8_t *rxData = getRxBuffer(); // Retrieve the buffer pointer
//			process_uart_command((const char *)rxData);
//			resetDataReady();
//		}
//		process_input_events();
		osDelay(pdMS_TO_TICKS(1000)); // Task delay
	}
  /* USER CODE END InputManagerTask */
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
