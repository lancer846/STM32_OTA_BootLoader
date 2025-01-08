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
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t rx_buff[POINTER_ARR_SIZE];
QUEUE_POS_t qp;

OTA_Info_t st_ota_info; // OTA 结构体数据
OTA_UpdatePackage_t st_ota_update_package; // OTA 更新的分包的信息

// 全局状态变量，共 32 位，每一位均用于判断当前哪种事件触发
uint32_t boot_update_status;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void QUEUE_POS_init()
{
  qp.pos_ptr_in = &qp.pos_ptr[0];
  qp.pos_ptr_out = &qp.pos_ptr[0];
  qp.pos_ptr_in->start = &rx_buff[0];
  qp.receive_count = 0;
  qp.pos_ptr_end = &qp.pos_ptr[NUM - 1];
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	QUEUE_POS_init();
  w25q128_init();
  boot_init();
  printf("boot init over\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint16_t i;
  while (1)
  {
    // OTA 标志升级标志位为 1
		if(boot_update_status & OTA_UPDATE_A_FLAG) {
      // 开始着手更新 A 区
      printf("download total size = %d\n", st_ota_info.ota_down_size[st_ota_update_package.update_package_arrive_num]);
      // 在写入时，是按照 4 个字节为单位进行处理的，所以升级的包大小必须是 4 的整数倍
      if(st_ota_info.ota_down_size[st_ota_update_package.update_package_arrive_num] % 4 == 0) {
        // printf("size\n");
        // 先进行内部 flash 擦除，此处擦除 sector2~3，共 32Kbytes
        internal_flash_erase(FLASH_SECTOR_2, 2);
        // 每次读取 1024 字节，计算总共需要读取多少次
        int read_count = st_ota_info.ota_down_size[st_ota_update_package.update_package_arrive_num] / OTA_SUBCONTRACT_DOWNLOAD_SIZE;
        for (i = 0; i < read_count; i++)
        {
          // 读取，升级包及每个单独的应用程序分配 64Kbytes
          W25Q128_read(st_ota_update_package.update_single_package, st_ota_update_package.update_package_arrive_num * 64 * 1024 + i * OTA_SUBCONTRACT_DOWNLOAD_SIZE, OTA_SUBCONTRACT_DOWNLOAD_SIZE);
          // 写入内部 flash
          internal_flash_write(STM32_A_START_ADDRESS + i * OTA_SUBCONTRACT_DOWNLOAD_SIZE, (uint32_t *)st_ota_update_package.update_single_package, OTA_SUBCONTRACT_DOWNLOAD_SIZE);
        }
        // 如果上面的大小整除不尽，就会还有剩下的等待处理
        uint32_t remain = st_ota_info.ota_down_size[st_ota_update_package.update_package_arrive_num] % OTA_SUBCONTRACT_DOWNLOAD_SIZE;
        if(remain != 0) {
          // 读取
          W25Q128_read(st_ota_update_package.update_single_package, st_ota_update_package.update_package_arrive_num * 64 * 1024 + i * OTA_SUBCONTRACT_DOWNLOAD_SIZE, remain);
          // 写入内部 flash
          internal_flash_write(STM32_A_START_ADDRESS + i * OTA_SUBCONTRACT_DOWNLOAD_SIZE, (uint32_t *)st_ota_update_package.update_single_package, remain);
        }
        
        // 写完数据，清除标志位，并且重启
        if(st_ota_update_package.update_package_arrive_num == 0) {
          // printf("ota flag\n");
          st_ota_info.ota_flag = 0;
          // 将标志位写入内存中
          W25Q128_write_vatiable_into_flash();
        }
        // printf("reset\n");
        NVIC_SystemReset();

      } else {
        printf("长度有误\n");
          boot_update_status &= ~OTA_UPDATE_A_FLAG;
      }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
