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
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "w25q128.h"
#include "bsp_internal_flash.h"
#include "boot.h"

/* flash �� B ����A ������ */
#define STM32_FLASH_BASE 0x08000000   // flash ��ʼ��ַ
#define STM32_SECTOR_SIZE (1024 * 16) // ������С
#define STM32_B_SECTOR_NUM 2          // B ����������
// A ����ʼ��ַ
#define STM32_A_START_ADDRESS (STM32_FLASH_BASE + STM32_SECTOR_SIZE * STM32_B_SECTOR_NUM)
/*
   �ݶ� OTA �ְ�����ʱ��ÿ�����Ĵ�СΪ 1Kbytes
  ��оƬ�ڲ� flash һ�β������� 16Kbytes��W25Q128 һ�β������� 4Kbytes��
 */
#define OTA_SUBCONTRACT_DOWNLOAD_SIZE 1024

// OTA ������־λ
#define OTA_UPDATE_A_FLAG 0x00000001
// OTA �л�������������
// ......

/* ���� */
// �ֽڽ��ջ�������С
#define POINTER_ARR_SIZE 2048
#define POINTER_ARR_TX_SIZE 2048
// ���ηְ����������ֽ���
#define SINGLE_DATA_MAX 256
#define NUM 10
  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */
  typedef struct
  {
    uint8_t *start;
    uint8_t *end;
  } POS_t;

  typedef struct
  {
    uint16_t receive_count;
    POS_t pos_ptr[NUM];
    // ��������ʱʹ�ã�ָ�������ָ��ṹ������
    POS_t *pos_ptr_in;
    // ��������ʱʹ�ã�ָ�������ָ��ṹ������
    POS_t *pos_ptr_out;
    POS_t *pos_ptr_end;
  } QUEUE_POS_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// ���� OTA ��־λ
#define OTA_FLAG_SET 0xAABB1122 // OTA ��־λ��ֵ
  typedef struct
  {
    uint32_t ota_flag; // OTA ��־λ��ֵΪ OTA_FLAG_SET ʱ���� OTA ����
    /*
      OTA ��Ҫ���ص����ݵ��ܴ�С��֮���Գ�ʼ�� 6 ����Ϊ�˺��������л���ͬ�Ĺ���
      ota_down_size[0] ���� OTA ��Ҫ���صĴ�С�������Ĳ��ֱַ𱣴治ͬ���ܵĴ����С
     */
    uint32_t ota_down_size[6];
  } OTA_Info_t;
#define OTA_INFO_SIZE sizeof(OTA_Info_t) // OTA �ṹ�����ݵĴ�С

  typedef struct
  {
    // �洢�����ְ�������
    uint8_t update_single_package[OTA_SUBCONTRACT_DOWNLOAD_SIZE];
    /*
      ��Ҫ���µ��� W25Q128 �ĵڼ��� block��
      �����жϵ�ǰ����ȡ OTA �洢���Ĵ��룬�������� block �е��������ܴ���
      Ҳ�����ڼ���������� W25Q128 �е�ƫ����
    */
    uint32_t update_package_arrive_num;
  } OTA_UpdatePackage_t;

  extern OTA_Info_t st_ota_info;                    // OTA �ṹ������
  extern OTA_UpdatePackage_t st_ota_update_package; // OTA ���µķְ�����Ϣ
  // ״̬�������� 32 λ��ÿһλ�������жϵ�ǰ�����¼�����
  extern uint32_t boot_update_status;
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
