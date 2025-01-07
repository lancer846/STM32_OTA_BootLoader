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

/* flash 及 B 区、A 区分区 */
#define STM32_FLASH_BASE 0x08000000   // flash 起始地址
#define STM32_SECTOR_SIZE (1024 * 16) // 扇区大小
#define STM32_B_SECTOR_NUM 2          // B 区扇区数量
// A 区起始地址
#define STM32_A_START_ADDRESS (STM32_FLASH_BASE + STM32_SECTOR_SIZE * STM32_B_SECTOR_NUM)
/*
   暂定 OTA 分包下载时，每个包的大小为 1Kbytes
  （芯片内部 flash 一次擦除最少 16Kbytes，W25Q128 一次擦除最少 4Kbytes）
 */
#define OTA_SUBCONTRACT_DOWNLOAD_SIZE 1024

// OTA 升级标志位
#define OTA_UPDATE_A_FLAG 0x00000001
// OTA 切换到其他功能区
// ......

/* 串口 */
// 字节接收缓冲区大小
#define POINTER_ARR_SIZE 2048
#define POINTER_ARR_TX_SIZE 2048
// 单次分包传输的最大字节数
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
    // 接收数据时使用，指向自身的指针结构体数组
    POS_t *pos_ptr_in;
    // 处理数据时使用，指向自身的指针结构体数组
    POS_t *pos_ptr_out;
    POS_t *pos_ptr_end;
  } QUEUE_POS_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// 定义 OTA 标志位
#define OTA_FLAG_SET 0xAABB1122 // OTA 标志位的值
  typedef struct
  {
    uint32_t ota_flag; // OTA 标志位，值为 OTA_FLAG_SET 时进行 OTA 升级
    /*
      OTA 需要下载的数据的总大小，之所以初始化 6 个是为了后续可以切换不同的功能
      ota_down_size[0] 保存 OTA 需要下载的大小，其他的部分分别保存不同功能的代码大小
     */
    uint32_t ota_down_size[6];
  } OTA_Info_t;
#define OTA_INFO_SIZE sizeof(OTA_Info_t) // OTA 结构体数据的大小

  typedef struct
  {
    // 存储单个分包的数据
    uint8_t update_single_package[OTA_SUBCONTRACT_DOWNLOAD_SIZE];
    /*
      需要更新的是 W25Q128 的第几个 block，
      用于判断当前是拿取 OTA 存储区的代码，还是其他 block 中的其它功能代码
      也可用于计算包数据在 W25Q128 中的偏移量
    */
    uint32_t update_package_arrive_num;
  } OTA_UpdatePackage_t;

  extern OTA_Info_t st_ota_info;                    // OTA 结构体数据
  extern OTA_UpdatePackage_t st_ota_update_package; // OTA 更新的分包的信息
  // 状态变量，共 32 位，每一位均用于判断当前哪种事件触发
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
