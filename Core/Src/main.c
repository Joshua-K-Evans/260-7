/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h" // library for C strings
#include <stdbool.h> // library for bool data type
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define UART_DELAY 100 // wait max of 100 ms between frames in message
#define MAX_MESSAGE_SIZE 100 // 100 characters maximum message size
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t message[MAX_MESSAGE_SIZE] = {0}; // char array to store message received
uint8_t response[MAX_MESSAGE_SIZE] = {0}; // char array to store response message
uint8_t uart1_byte; // byte received from UART1
uint8_t uart2_byte; // byte received from UART2
uint8_t buffer_position = 0; // how many bytes received so far
bool blink0 = false;// LED blink status
bool blink1 = false; // max bit
bool blink2 = false; // mid bit
bool blink3 = false; // min bit

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // Start UART1 and UART2 to receive interrupts
  HAL_UART_Receive_IT(&huart1, &uart1_byte, 1); // put byte from UART1 in "uart1_byte"
  HAL_UART_Receive_IT(&huart2, &uart2_byte, 1); // put byte from UART2 in "uart2_byte"

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (blink0){
	   HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // Toggle LED2
	   HAL_Delay(200); // wait 200 ms
	  }
	  if (blink1){
	   	HAL_GPIO_TogglePin(MAX_BIT_GPIO_Port, MAX_BIT_Pin); // Toggle max bit
	   	HAL_Delay(200); // wait 200 ms
	  }
	  if (blink2){
	  	   	HAL_GPIO_TogglePin(MID_BIT_GPIO_Port, MID_BIT_Pin); // Toggle mid bit
	  	   	HAL_Delay(200); // wait 200 ms
	  }
	  if (blink3){
	  	   	HAL_GPIO_TogglePin(MIN_BIT_GPIO_Port, MIN_BIT_Pin); // Toggle min bit
	  	   	HAL_Delay(200); // wait 200 ms
	  }
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart1.Init.Mode = UART_MODE_TX_RX;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MAX_BIT_Pin|MID_BIT_Pin|MIN_BIT_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : MAX_BIT_Pin MID_BIT_Pin MIN_BIT_Pin LD2_Pin */
  GPIO_InitStruct.Pin = MAX_BIT_Pin|MID_BIT_Pin|MIN_BIT_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// Check if byte received was on UART2 (from laptop)
	if (huart == &huart2) {
		// If we get here, we received a byte from UART2 and it was placed in "uart2_byte"
		// Take byte received from my laptop over UART2 and send to lab partner over UART1
		HAL_UART_Transmit(&huart1, &uart2_byte, 1, UART_DELAY);
		// Restart UART2's receive interrupt to wait for next byte from laptop
		HAL_UART_Receive_IT(&huart2, &uart2_byte, 1); //start next byte receive interrupt
	}
	// Check if byte received was on UART1 (from lab partner)
	if (huart == &huart1) {
		if ((uart1_byte != '\r') && (uart1_byte != '\n')
				&& (uart1_byte != '\0')) {
			// Add uart2_byte to the message
			message[buffer_position] = uart1_byte;
			buffer_position++;
		} else {

			// If we get here, it means we received \r, \n, or \0 and the message is over
			// If we get here, we received a byte from UART1 and it was placed in "uart1_byte"
			// Take byte received from lab partner over UART1 and send to my laptop over UART2
			HAL_UART_Transmit(&huart2, &uart1_byte, 1, UART_DELAY);

			// Check message
			if (strcmp((char*) message, "LED2_ON") == 0) {

				// If "LED2_ON\n" was the message received -> Turn on LED2
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				blink0 = false;

				// Prepare response message
				strncpy((char*) response, "Task complete - LED2 turned on.\n\r",
				MAX_MESSAGE_SIZE);

			} else if (strcmp((char*) message, "LED2_OFF") == 0) {

				// If "LED2_OFF\n" was the message received -> Turn off LED2
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				blink0 = false;

				// Prepare response message
				strncpy((char*) response,
						"Task complete - LED2 turned off.\n\r",
						MAX_MESSAGE_SIZE);
			}else if (strcmp((char*) message, "MAX_ON") == 0) {

					// If "LED2_ON\n" was the message received -> Turn on LED2
					HAL_GPIO_WritePin(MAX_BIT_GPIO_Port, MAX_BIT_Pin, GPIO_PIN_SET);
					blink1 = false;

					// Prepare response message
					strncpy((char*) response, "Task complete - Max Bit turned on.\n\r",
					MAX_MESSAGE_SIZE);

				} else if (strcmp((char*) message, "MAX_OFF") == 0) {

					// If "LED2_OFF\n" was the message received -> Turn off LED2
					HAL_GPIO_WritePin(MAX_BIT_GPIO_Port, MAX_BIT_Pin, GPIO_PIN_RESET);
					blink1 = false;

					// Prepare response message
					strncpy((char*) response,
							"Task complete - Max Bit turned off.\n\r",
							MAX_MESSAGE_SIZE);
				}else if (strcmp((char*) message, "MID_ON") == 0) {

						// If "LED2_ON\n" was the message received -> Turn on LED2
						HAL_GPIO_WritePin(MID_BIT_GPIO_Port, MID_BIT_Pin, GPIO_PIN_SET);
						blink2 = false;

						// Prepare response message
						strncpy((char*) response, "Task complete - Mid Bit turned on.\n\r",
						MAX_MESSAGE_SIZE);

					} else if (strcmp((char*) message, "MID_OFF") == 0) {

						// If "LED2_OFF\n" was the message received -> Turn off LED2
						HAL_GPIO_WritePin(MID_BIT_GPIO_Port, MID_BIT_Pin, GPIO_PIN_RESET);
						blink2 = false;

						// Prepare response message
						strncpy((char*) response,
								"Task complete - Mid Bit turned off.\n\r",
								MAX_MESSAGE_SIZE);
					}else if (strcmp((char*) message, "MIN_ON") == 0) {

							// If "LED2_ON\n" was the message received -> Turn on LED2
							HAL_GPIO_WritePin(MIN_BIT_GPIO_Port, MIN_BIT_Pin, GPIO_PIN_SET);
							blink3 = false;

							// Prepare response message
							strncpy((char*) response, "Task complete - Min Pin turned on.\n\r",
							MAX_MESSAGE_SIZE);

						} else if (strcmp((char*) message, "MIN_OFF") == 0) {

							// If "LED2_OFF\n" was the message received -> Turn off LED2
							HAL_GPIO_WritePin(MIN_BIT_GPIO_Port, MIN_BIT_Pin, GPIO_PIN_RESET);
							blink3 = false;

							// Prepare response message
							strncpy((char*) response,
									"Task complete - Min Bit turned off.\n\r",
									MAX_MESSAGE_SIZE);


			} else if (strcmp((char*) message, "LED2_BLINK") == 0) {

				// If "LED2_BLINK" was the message received -> tell main() to blink LED2
				blink0 = true;

				// Prepare response message
				strncpy((char*) response, "Task complete - LED2 blinking.\n\r",
				MAX_MESSAGE_SIZE);

			} else if (strcmp((char*) message, "MAX_BLINK") == 0) {

				// If "MAX_BLINK" was the message received -> tell main() to blink PA0 LED
				blink1 = true;

				// Prepare response message
				strncpy((char*) response, "Task complete - PA0 blinking.\n\r",
				MAX_MESSAGE_SIZE);

			} else if (strcmp((char*) message, "MID_BLINK") == 0) {

				// If "MID_BLINK" was the message received -> tell main() to blink PA1 LED
				blink2 = true;

				// Prepare response message
				strncpy((char*) response, "Task complete - PA1 blinking.\n\r",
				MAX_MESSAGE_SIZE);

			} else if (strcmp((char*) message, "MIN_BLINK") == 0) {

				// If "MIN_BLINK" was the message received -> tell main() to blink PA4 LED
				blink3 = true;

				// Prepare response message
				strncpy((char*) response, "Task complete - PA4 blinking.\n\r",
				MAX_MESSAGE_SIZE);

			} else {

				// Else message was not recognized
				strncpy((char*) response, "Task not recognized.\n\r",
				MAX_MESSAGE_SIZE);
			}

			// Send the response message to laptop
			HAL_UART_Transmit(&huart2, response, strlen(response), UART_DELAY);

			// Zero out message array and response array to get ready for the next message
			memset(message, 0, sizeof(message));
			memset(response, 0, sizeof(response));
			buffer_position = 0;
		}
		// Restart UART1's receive interrupt to wait for next byte
		HAL_UART_Receive_IT(&huart1, &uart1_byte, 1); //start next byte receive interrupt
	}
}


/* USER CODE END 4 */

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
