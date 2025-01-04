/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "w25q128.h"
#include "bsp_internal_flash.h"
#include "boot.h"
// �ֽڽ��ջ�������С
#define POINTER_ARR_SIZE 		2048
#define POINTER_ARR_TX_SIZE 2048
// ���ηְ����������ֽ���
#define SINGLE_DATA_MAX	 		256
#define NUM 						 		10
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
	uint8_t* start;
	uint8_t* end;
} POS_t;

typedef struct {
	uint16_t receive_count;
	POS_t pos_ptr[NUM];
	// ��������ʱʹ�ã�ָ�������ָ��ṹ������
	POS_t* pos_ptr_in;
	// ��������ʱʹ�ã�ָ�������ָ��ṹ������
	POS_t* pos_ptr_out;
	POS_t* pos_ptr_end;
} QUEUE_POS_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// ���� OTA ��־λ
#define OTA_FLAG_SET 0xAABB1122 // OTA ��־λ��ֵ
typedef struct {
  uint32_t ota_flag; // OTA ��־λ��ֵΪ OTA_FLAG_SET ʱ���� OTA ����
} OTA_Info_t;
#define OTA_INFO_SIZE sizeof(OTA_Info_t) // OTA �ṹ�����ݵĴ�С

extern OTA_Info_t st_ota_info; // OTA �ṹ������
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define SPI1_CLK_Pin GPIO_PIN_5
#define SPI1_CLK_GPIO_Port GPIOA
#define SPI1_MISO_Pin GPIO_PIN_6
#define SPI1_MISO_GPIO_Port GPIOA
#define SPI1_MOSI_Pin GPIO_PIN_7
#define SPI1_MOSI_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
