#include "boot.h"

pFunction reset_handler;
#define TEST 1
// bootloader 初始化
void boot_init(void)
{
#if TEST
    st_ota_info.ota_flag = 0xAABB1122;
    W25Q128_write_vatiable_into_flash();
    HAL_Delay(200);
#endif
    // 将 OTA 结构体数据清零
    memset(&st_ota_info, 0, sizeof(st_ota_info));
    // 从 W25Q128 flash 中读取数据
    W25Q128_read((uint8_t *)&st_ota_info, W25Q128_LAST_BLOCK_ADDRESS, sizeof(st_ota_info));
    // 判断是否为合法的 OTA 数据
    if (st_ota_info.ota_flag == OTA_FLAG_SET)
    {
        // 需要更新
        printf("need update\n");
        // 改变全局状态
        boot_update_status |= OTA_UPDATE_A_FLAG;
        // 赋值为标志 OTA 升级区域对应的序号
        st_ota_update_package.update_package_arrive_num = 0;
    }
    else
    {
        // 跳转 A 区功能区
        printf("turn to A area\n");
        LOAD_A_Reset_Handler(STM32_A_START_ADDRESS);
    }
}

/**
 * @brief 将 A 区栈顶指针给到 sp(__initial_sp)
 * 
 * @param address RAM中的栈顶地址
 * @return __asm 
 */
__asm void MSR_MSP_set(uint32_t address) {
    /*
        C 语言转汇编，会将第一个参数 address 给到 r0，第二个参数给到 r1，...以此类推
        所以此处本质上就是将 address 给到 MSP
     */
    MSR MSP, r0;
    BX r14;
}

/**
 * @brief 
 * 将 A 区 Reset_Handler 地址给到 pc
 * 在 C 语言中，函数名() 就是将地址给到了 pc，所以此处采取同样的原理
 * @param address flash 中的 A 区起始地址
 */
void LOAD_A_Reset_Handler(uint32_t address) {
    // 判断地址是否超出 A 区 RAM 的范围 192Kbytes
    if(*(uint32_t *)address > 0x20000000 && *(uint32_t *)address < 0x20030000) {
        MSR_MSP_set(*(uint32_t *)address);
        // 复位函数
        reset_handler = (pFunction)*(uint32_t *)(address + 4);
        Reset_peripheral();
        // 使 pc 指向复位函数
        reset_handler();
    }
}

/**
 * @brief B区使用的外设进行重置
 * 
 */
void Reset_peripheral() {
    HAL_DeInit();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10);
    HAL_UART_DeInit(&huart1);
    HAL_DMA_DeInit(&hdma_usart1_rx);
    HAL_SPI_DeInit(&hspi1);
}
