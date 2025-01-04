#include "boot.h"

// bootloader 初始化
void boot_init(void) {
    // uint32_t temp = 0xAABB1122;
    // W25Q128_write((uint8_t*)&temp, W25Q128_LAST_BLOCK_ADDRESS, 4);
    // HAL_Delay(200);
    // 将 OTA 结构体数据清零
    memset(&st_ota_info, 0, sizeof(st_ota_info));
    // 从 W25Q128 flash 中读取数据
    W25Q128_read((uint8_t *)&st_ota_info, W25Q128_LAST_BLOCK_ADDRESS, sizeof(st_ota_info));
    // 判断是否为合法的 OTA 数据
    if (st_ota_info.ota_flag == OTA_FLAG_SET) {
        // 需要更新
        printf("need update\n");
    } else {
        // 跳转 A 区功能区
        printf("turn to A area\n");
    }
}
