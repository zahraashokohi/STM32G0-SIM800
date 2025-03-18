/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Sim800Handler.h"
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

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void UARTStream_init(UARTStream* stream, UART_HandleTypeDef* huart, DMA_HandleTypeDef* rxDma, DMA_HandleTypeDef* txDma, uint8_t* rxBuff, Stream_LenType rxBuffSize, uint8_t* txBuff, Stream_LenType txBuffLen);
void UARTStream_rxHandle(UARTStream* stream);
void UARTStream_txHandle(UARTStream* stream);
static const char CRLF[2] = "\r\n";
void UARTStream_stopReceiveDMA(UARTStream* stream);
void UARTStream_stopTransmitDMA(UARTStream* stream);

void UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len);
void UARTStream_receive(IStream* stream, uint8_t* buff, Stream_LenType len);
Stream_LenType UARTStream_checkReceive(IStream* stream);
uint8_t val = 0;
counter_t cnt;
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim14);
	
	UARTStream_init(&uartStream1, &huart1, &hdma_usart1_rx, &hdma_usart1_tx,uartStream1RxBuff, sizeof(uartStream1RxBuff), uartStream1TxBuff, sizeof(uartStream1TxBuff));
	
	IStream_receive(&uartStream1.In);
	
	OStream_writeBytes(&uartStream1.Out, (uint8_t*) "Running...\r\n", 12);
	OStream_flush(&uartStream1.Out);
	uint8_t index;
	
	//pwrkey_on(PWRKEY_GPIO_Port, PWRKEY_Pin);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(1500);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_Delay(5000);
  Sim800_config();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		 
		 Uart_dataPars_();
		/*while (IStream_available(&uartStream1.In) > 0) 
		{
			val = IStream_readUInt8(&uartStream1.In);
			
		}
		 OStream_writeBytes(&uartStream1.Out, (uint8_t*) "AT\r\n", 5);
	   OStream_flush(&uartStream1.Out);
		  HAL_Delay(500);
		// OStream_writeBytes(&uartStream1.Out, (uint8_t*) "AT\r\n", 5);
	  // OStream_flush(&uartStream1.Out);
		//  HAL_Delay(500);*/
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
}

/* USER CODE BEGIN 4 */

void UARTStream_init(UARTStream* stream, UART_HandleTypeDef* huart, DMA_HandleTypeDef* rxDma, DMA_HandleTypeDef* txDma, uint8_t* rxBuff, Stream_LenType rxBuffSize, uint8_t* txBuff, Stream_LenType txBuffSize) {
	stream->HUART = huart;
	stream->RxDMA = rxDma;
	stream->TxDMA = txDma;
	IStream_init(&stream->In, UARTStream_receive, rxBuff, rxBuffSize);
	IStream_setCheckReceive(&stream->In, UARTStream_checkReceive);
	IStream_setArgs(&stream->In, stream);
	OStream_init(&stream->Out, UARTStream_transmit, txBuff, txBuffSize);
	OStream_setArgs(&stream->Out, stream);
	//OStream_setCheckReceive(&stream->In, UARTStream_checkReceive);
}
void UARTStream_rxHandle(UARTStream* stream) {
	IStream_handle(&stream->In, IStream_incomingBytes(&stream->In));
}
void UARTStream_txHandle(UARTStream* stream) {
	OStream_handle(&stream->Out, OStream_outgoingBytes(&stream->Out));
}

void UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len) {
	UARTStream* uartStream = (UARTStream*) OStream_getArgs(stream); 
	UARTStream_stopTransmitDMA(uartStream);
	HAL_UART_Transmit_DMA(uartStream->HUART, buff, len);
}
void UARTStream_receive(IStream* stream, uint8_t* buff, Stream_LenType len) {
	UARTStream* uartStream = (UARTStream*) IStream_getArgs(stream); 
	UARTStream_stopReceiveDMA(uartStream);
	HAL_UART_Receive_DMA(uartStream->HUART, buff, len);
}
Stream_LenType UARTStream_checkReceive(IStream* stream) {
	UARTStream* uartStream = (UARTStream*) IStream_getArgs(stream); 
	return IStream_incomingBytes(stream) - uartStream->RxDMA->Instance->CNDTR;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	switch ((uint32_t) huart->Instance) {
		case USART1_BASE:
			UARTStream_rxHandle(&uartStream1);
		//  BUF.FifoRx[BUF.FifoIndex++] = 
		 
			break;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
	switch ((uint32_t) huart->Instance) {
		case USART1_BASE:
			UARTStream_txHandle(&uartStream1);
			break;
	}
}
void UARTStream_stopReceiveDMA(UARTStream* stream) {
	UART_HandleTypeDef* huart = stream->HUART;
	uint32_t dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
  if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && dmarequest)
  {
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    /* Abort the UART DMA Rx stream */
    if (huart->hdmarx != NULL)
    {
      HAL_DMA_Abort(huart->hdmarx);
    }
    /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
		CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));
		CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

		/* At end of Rx process, restore huart->RxState to Ready */
		huart->RxState = HAL_UART_STATE_READY;
  }
}
void UARTStream_stopTransmitDMA(UARTStream* stream) {
	UART_HandleTypeDef* huart = stream->HUART;
	uint32_t dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAT);
  if ((huart->gState == HAL_UART_STATE_BUSY_TX) && dmarequest)
  {
    CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAT);

    /* Abort the UART DMA Tx stream */
    if (huart->hdmatx != NULL)
    {
      HAL_DMA_Abort(huart->hdmatx);
    }
    /* Disable TXEIE and TCIE interrupts */
		CLEAR_BIT(huart->Instance->CR1, (USART_CR1_TXEIE_TXFNFIE | USART_CR1_TCIE));

		/* At end of Tx process, restore huart->gState to Ready */
		huart->gState = HAL_UART_STATE_READY;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM14)
	{
		cnt.CurrentTime++;
		 
		if(HAL_GPIO_ReadPin(NET_LIGHT_SIM800_GPIO_Port, NET_LIGHT_SIM800_Pin) == 0)
		{
			HAL_GPIO_WritePin(NET_LIGHT_GPIO_Port, NET_LIGHT_Pin, GPIO_PIN_RESET);
		}
		else{
			HAL_GPIO_WritePin(NET_LIGHT_GPIO_Port, NET_LIGHT_Pin, GPIO_PIN_SET);
		}
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

