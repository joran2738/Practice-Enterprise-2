/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define TOP_Pin GPIO_PIN_13
#define TOP_GPIO_Port GPIOC
#define TOP_EXTI_IRQn EXTI15_10_IRQn
#define RIGHT_Pin GPIO_PIN_14
#define RIGHT_GPIO_Port GPIOC
#define RIGHT_EXTI_IRQn EXTI15_10_IRQn
#define LEFT_Pin GPIO_PIN_15
#define LEFT_GPIO_Port GPIOC
#define LEFT_EXTI_IRQn EXTI15_10_IRQn
#define MIDDLE_Pin GPIO_PIN_0
#define MIDDLE_GPIO_Port GPIOA
#define MIDDLE_EXTI_IRQn EXTI0_IRQn
#define TOUCH_CS_Pin GPIO_PIN_12
#define TOUCH_CS_GPIO_Port GPIOB
#define DISPL_SCK_Pin GPIO_PIN_13
#define DISPL_SCK_GPIO_Port GPIOB
#define TOUCH_MISO_Pin GPIO_PIN_14
#define TOUCH_MISO_GPIO_Port GPIOB
#define DISPL_MOSI_Pin GPIO_PIN_15
#define DISPL_MOSI_GPIO_Port GPIOB
#define DISPL_CS_Pin GPIO_PIN_8
#define DISPL_CS_GPIO_Port GPIOA
#define DISPL_RST_Pin GPIO_PIN_9
#define DISPL_RST_GPIO_Port GPIOA
#define DISPL_DC_Pin GPIO_PIN_10
#define DISPL_DC_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
