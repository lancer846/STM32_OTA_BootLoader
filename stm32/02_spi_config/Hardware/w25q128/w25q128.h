#ifndef __W25Q128_H__
#define __W25Q128_H__

#include "main.h"
#include "spi.h"

/**
 * @brief SPI 可以采用模式 0 、3，此处使用模式 0
 * SPI 的 cs 引脚设置为 软实现
 * 上升沿数据写入 GD25Q32C (w25q128)
 * 下降沿数据移出
 *
 * 外置 flash 芯片中一个扇区的大小是 4k，一个块的大小是 64k
 */

#define W25Q128_CS_LOW() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define W25Q128_CS_HIGH() HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)

void w25q128_init(void);
uint16_t W25Q128_readID(void);
void W25Q128_write_enable(void);
void W25Q128_wait_busy(void);
void W25Q128_erase_sector(uint32_t addr);
void W25Q128_erase_block_64k(uint32_t addr);
void W25Q128_write(uint8_t *buffer, uint32_t addr, uint16_t numbyte);
void W25Q128_read(uint8_t *buffer, uint32_t read_addr, uint16_t read_length);

#endif /* __W25Q128_H__ */
