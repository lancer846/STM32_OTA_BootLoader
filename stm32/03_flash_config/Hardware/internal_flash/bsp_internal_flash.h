#ifndef __BSP_INTERNAL_FLASH_H__
#define __BSP_INTERNAL_FLASH_H__

#include "main.h"

/*
    STM32F407 内部 flash 只有 512kb 大小
    flash 扇区有 7 个
*/

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address
                                 of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address
                                 of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address
                                 of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address
                                 of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address
                                 of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address
                                 of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address
                                 of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address
                                 of Sector 7, 128 Kbytes  */

// 测试用
int InternalFlash_Test(void);

int internal_flash_erase(uint32_t sector, uint32_t num);
void internal_flash_write(uint32_t addr, uint32_t *buf, uint32_t len);

#endif /* __BSP_INTERNAL_FLASH_H__ */
