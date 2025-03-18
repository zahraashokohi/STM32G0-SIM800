/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "StreamBuffer.h"
#include "InputStream.h"
#include "OutputStream.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NET_LIGHT_SIM800_Pin GPIO_PIN_0
#define NET_LIGHT_SIM800_GPIO_Port GPIOA
#define NET_LIGHT_Pin GPIO_PIN_4
#define NET_LIGHT_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define PWRKEY_Pin GPIO_PIN_1
#define PWRKEY_GPIO_Port GPIOA
#define FIFO_BUFFER_SIZE   100

typedef struct{
	uint8_t ConfigPlus;
	uint32_t CurrentTime;
	uint8_t  HeaderNumber;
	uint32_t ReceiveData;
	
}counter_t;

extern counter_t cnt;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
