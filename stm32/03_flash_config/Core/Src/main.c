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
#include <string.h>
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25q128.h"
#include "bsp_internal_flash.h"
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
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 初�?��?? 指针队列
void QUEUE_POS_init()
{
  // 指向�?�?指针队列的�?�元素地址
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


#if 0
  // ���������鶨��
  unsigned char buff[20] = {0};
  printf("\r\n========= start ========\r\n");
  // ����Flash����0
  printf("\r\n 1 erase Flash sector 0......\r\n");
  W25Q128_erase_sector(0);
  printf("Flash sector 0 erase success\r\n");
  HAL_Delay(200);

  // ��ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff
  printf("\r\n 3 read 0 addr length 10 byte data to buff......\r\n");
  W25Q128_read(buff, 0, 10);
  // �����ȡ��������
  printf("��ȡ��������= %s\r\n", buff);
  HAL_Delay(200);

  // ��0��ַд��10���ֽڵ����� �����������塱
  printf("\r\n 4 to 0 addr write 10 byte data ......\r\n");
  W25Q128_write((uint8_t *)"����������", 0, 10);
  // �ȴ�д�����
  HAL_Delay(200);
  printf("data write success\r\n");

  // ��ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff
  printf("\r\n 5 read 0 addr length 10 byte data buff......\r\n");
  W25Q128_read(buff, 0, 10);
  // �����ȡ��������
  printf("read data = %s\r\n", buff);
  HAL_Delay(1000);

  // ��Ϊ�Ǵ�0��ַд�ģ����Բ�����0������
  printf("\r\n 6 test over erase write data......\r\n");
  W25Q128_erase_sector(0);
  HAL_Delay(200);

  // ���������
  memset(buff, 0, sizeof(buff));

  printf("\r\n========= over ========\r\n");
#else
// ���������鶨��
  unsigned char buff[20] = {0};
  W25Q128_erase_sector(0);
  HAL_Delay(200);
  W25Q128_read(buff, 0, 10);
  // �����ȡ��������
  printf("��ȡ��������= %s\r\n", buff);
  HAL_Delay(200);

  // ��ȡW25Q128���豸ID
  printf("\r\n��2����ȡ�豸��id......\r\n");
  printf("�豸ID = %X\r\n", W25Q128_readID());
  HAL_Delay(200);
  W25Q128_write((uint8_t *)"qwer", 0, 5);
  HAL_Delay(200);
  W25Q128_read(buff, 0, 5);
  printf("read data = %s\r\n", buff);
  HAL_Delay(200);


  printf("==================================\r\n");
  printf("���ڽ��ж�д�ڲ�FLASHʵ�飬�����ĵȴ�\r\n");

  //  if (InternalFlash_Test()==0) {
  //        printf("��д�ڲ�FLASH���Գɹ�\r\n");
  //  } else {
  //        printf("��д�ڲ�FLASH����ʧ��\r\n");
  //  }
  uint32_t data = 0x12345678;
  internal_flash_erase(FLASH_SECTOR_7, 1);
  internal_flash_write(ADDR_FLASH_SECTOR_7, &data, 4);
  printf("read data = %#x\n", *(uint32_t *)ADDR_FLASH_SECTOR_7);
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_buff, SINGLE_DATA_MAX);
  while (1)
  {
    // 处理传输过来的数�?
    if (qp.pos_ptr_out != qp.pos_ptr_in)
    {
      // out �? in 的指针不相等，表示队列中有存入新的数�?
      // 按照队列的每�?元素�? start与end 为界限来进�?��?�理
      // 1、获取�?�分区内元素的个�?
      uint16_t byte_count = qp.pos_ptr_out->end - qp.pos_ptr_out->start + 1;
      printf("the part has %d element\n", byte_count);

      for (uint16_t i = 0; i < byte_count; i++)
      {
        printf("%c", qp.pos_ptr_out->start[i]);
      }
      printf("\n");
      qp.pos_ptr_out++;
      // 指针列表已达到末尾，返回首位
      if (qp.pos_ptr_out == qp.pos_ptr_end)
      {
        qp.pos_ptr_out = &qp.pos_ptr[0];
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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