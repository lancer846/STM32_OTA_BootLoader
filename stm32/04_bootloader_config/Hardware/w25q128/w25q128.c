#include "w25q128.h"

void w25q128_init(void)
{
    // clk 拉低
}

// 读取芯片ID
// 读取设备ID
uint16_t W25Q128_readID(void)
{
    // uint16_t  temp = 0;
    uint8_t addr = 0x90;
    uint8_t base = 0x00;
    // 将CS端拉低为低电平
    W25Q128_CS_LOW();
    // 发送指令90h
    HAL_SPI_Transmit(&hspi1, &addr, 1, 1000);
    // spi_read_write_byte(0x90);//发送读取ID命令
    // 发送地址  000000H
    HAL_SPI_Transmit(&hspi1, &base, 1, 1000);
    HAL_SPI_Transmit(&hspi1, &base, 1, 1000);
    HAL_SPI_Transmit(&hspi1, &base, 1, 1000);
    // spi_read_write_byte(0x00);
    // spi_read_write_byte(0x00);
    // spi_read_write_byte(0x00);

    // 接收数据
    // 接收制造商ID
    uint16_t temp = 0;
    uint8_t temp1 = 0xFF;
    uint8_t temp2 = 0xFF;
    HAL_SPI_Receive(&hspi1, &temp1, 1, 1000);
    HAL_SPI_Receive(&hspi1, &temp2, 1, 1000);
    // temp |= spi_read_write_byte(0xFF)<<8;
    // //接收设备ID
    // temp |= spi_read_write_byte(0xFF);
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
    temp = (temp1 << 8) | temp2;
    // 返回ID
    return temp;
}

// 在进行写入操作之前需要先 发送写使能
void W25Q128_write_enable(void)
{
    uint8_t addr = 0x06;
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令06h
    HAL_SPI_Transmit(&hspi1, &addr, 1, 1000);
    // spi_read_write_byte(0x06);
    // 拉高CS端为高电平
    W25Q128_CS_HIGH();
}

/**********************************************************
 * 函 数 名 称：W25Q128_wait_busy
 * 函 数 功 能：检测线路是否繁忙
 * 传 入 参 数：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
 **********************************************************/
void W25Q128_wait_busy(void)
{
    unsigned char byte = 0Xff;
    uint8_t addr = 0x05;
    do
    {
        // 拉低CS端为低电平
        W25Q128_CS_LOW();
        // 发送指令05h
        //  spi_read_write_byte(0x05);
        HAL_SPI_Transmit(&hspi1, &addr, 1, 1000);
        // 接收状态寄存器值
        HAL_SPI_Receive(&hspi1, &byte, 1, 1000);
        // byte = spi_read_write_byte(0Xff);
        // 恢复CS端为高电平
        W25Q128_CS_HIGH();
        // 判断BUSY位是否为1 如果为1说明在忙，重新读写BUSY位直到为0
    } while ((byte & 0x01) == 1);
}

/**********************************************************
 * 函 数 名 称：W25Q128_erase_sector
 * 函 数 功 能：擦除一个扇区
 * 传 入 参 数：addr=擦除的扇区号
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：addr=擦除的扇区号，范围=0~4096。

W25Q128将16M的容量分为256个块（Block),每个块大小为64K（64000）个字节，每个块又分为16个扇区（Sector),每个扇区4K个字节。
W25Q128的最小擦除单位为一个扇区，也就是每次必须擦除4K个字节。
**********************************************************/
void W25Q128_erase_sector(uint32_t addr)
{
    // 计算扇区号，一个扇区4KB=4096
    addr *= 4096;
    uint8_t addr_origin = 0x20;
    uint8_t addr_high = (uint8_t)((addr) >> 16);
    uint8_t addr_mid = (uint8_t)((addr) >> 8);
    uint8_t addr_low = (uint8_t)addr;
    W25Q128_write_enable(); // 写使能
    W25Q128_wait_busy();    // 判断忙，如果忙则一直等待
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令20h
    HAL_SPI_Transmit(&hspi1, &addr_origin, 1, 1000);
    // spi_read_write_byte(0x20);
    // 发送24位扇区地址的高8位
    HAL_SPI_Transmit(&hspi1, &addr_high, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>16));
    // 发送24位扇区地址的中8位
    HAL_SPI_Transmit(&hspi1, &addr_mid, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>8));
    // 发送24位扇区地址的低8位
    HAL_SPI_Transmit(&hspi1, &addr_low, 1, 1000);
    // spi_read_write_byte((uint8_t)addr);
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
    // 等待擦除完成
    W25Q128_wait_busy();
}

/**********************************************************
 * 函 数 名 称：W25Q128_erase_sector
 * 函 数 功 能：擦除一个块
 * 传 入 参 数：addr=擦除的块号
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：addr=擦除的块号

W25Q128将16M的容量分为256个块（Block),每个块大小为64K（64000）个字节，每个块又分为16个扇区（Sector),每个扇区4K个字节。
W25Q128的最小擦除单位为一个扇区，也就是每次必须擦除4K个字节。
**********************************************************/
void W25Q128_erase_block_32k(uint32_t addr)
{
    // 计算扇区号，一个扇区4KB=4096
    addr *= 32 * 1024;
    uint8_t addr_origin = 0x52;
    uint8_t addr_high = (uint8_t)((addr) >> 16);
    uint8_t addr_mid = (uint8_t)((addr) >> 8);
    uint8_t addr_low = (uint8_t)addr;
    W25Q128_write_enable(); // 写使能
    W25Q128_wait_busy();    // 判断忙，如果忙则一直等待
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令D8h
    HAL_SPI_Transmit(&hspi1, &addr_origin, 1, 1000);
    // spi_read_write_byte(0x20);
    // 发送24位扇区地址的高8位
    HAL_SPI_Transmit(&hspi1, &addr_high, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>16));
    // 发送24位扇区地址的中8位
    HAL_SPI_Transmit(&hspi1, &addr_mid, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>8));
    // 发送24位扇区地址的低8位
    HAL_SPI_Transmit(&hspi1, &addr_low, 1, 1000);
    // spi_read_write_byte((uint8_t)addr);
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
    // 等待擦除完成
    W25Q128_wait_busy();
}
void W25Q128_erase_block_64k(uint32_t addr)
{
    // 计算扇区号，一个扇区4KB=4096
    addr *= 64 * 1024;
    uint8_t addr_origin = 0xD8;
    uint8_t addr_high = (uint8_t)((addr) >> 16);
    uint8_t addr_mid = (uint8_t)((addr) >> 8);
    uint8_t addr_low = (uint8_t)addr;
    W25Q128_write_enable(); // 写使能
    W25Q128_wait_busy();    // 判断忙，如果忙则一直等待
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令D8h
    HAL_SPI_Transmit(&hspi1, &addr_origin, 1, 1000);
    // spi_read_write_byte(0x20);
    // 发送24位扇区地址的高8位
    HAL_SPI_Transmit(&hspi1, &addr_high, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>16));
    // 发送24位扇区地址的中8位
    HAL_SPI_Transmit(&hspi1, &addr_mid, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>8));
    // 发送24位扇区地址的低8位
    HAL_SPI_Transmit(&hspi1, &addr_low, 1, 1000);
    // spi_read_write_byte((uint8_t)addr);
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
    // 等待擦除完成
    W25Q128_wait_busy();
}

/**********************************************************
 * 函 数 名 称：W25Q128_write
 * 函 数 功 能：写数据到W25Q128进行保存
 * 传 入 参 数：buffer=写入的数据内容  addr=写入地址  numbyte=写入数据的长度
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
 **********************************************************/
void W25Q128_write(uint8_t *buffer, uint32_t addr, uint16_t numbyte)
{
    unsigned int i = 0;
    uint8_t addr_origin = 0x02;
    uint8_t addr_high = (uint8_t)((addr) >> 16);
    uint8_t addr_mid = (uint8_t)((addr) >> 8);
    uint8_t addr_low = (uint8_t)addr;
    // 擦除扇区数据
    W25Q128_erase_sector(addr / 4096);
    // 写使能
    W25Q128_write_enable();
    // 忙检测
    W25Q128_wait_busy();
    // 写入数据
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令02h
    HAL_SPI_Transmit(&hspi1, &addr_origin, 1, 1000);
    // spi_read_write_byte(0x02);
    // 发送写入的24位地址中的高8位
    HAL_SPI_Transmit(&hspi1, &addr_high, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>16));
    // 发送写入的24位地址中的中8位
    HAL_SPI_Transmit(&hspi1, &addr_mid, 1, 1000);
    // spi_read_write_byte((uint8_t)((addr)>>8));
    // 发送写入的24位地址中的低8位
    HAL_SPI_Transmit(&hspi1, &addr_low, 1, 1000);
    // spi_read_write_byte((uint8_t)addr);
    // 根据写入的字节长度连续写入数据buffer
    for (i = 0; i < numbyte; i++)
    {
        HAL_SPI_Transmit(&hspi1, &buffer[i], 1, 1000);
        // spi_read_write_byte(buffer[i]);
    }
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
    // 忙检测
    W25Q128_wait_busy();
}

/**********************************************************
 * 函 数 名 称：W25Q128_read
 * 函 数 功 能：读取W25Q128的数据
 * 传 入 参 数：buffer=读出数据的保存地址  read_addr=读取地址   read_length=读去长度
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
 **********************************************************/
void W25Q128_read(uint8_t *buffer, uint32_t read_addr, uint16_t read_length)
{
    uint16_t i;
    uint8_t tx_buff = 0xFF;

    uint8_t addr_origin = 0x03;
    uint8_t addr_high = (uint8_t)((read_addr) >> 16);
    uint8_t addr_mid = (uint8_t)((read_addr) >> 8);
    uint8_t addr_low = (uint8_t)read_addr;
    // 拉低CS端为低电平
    W25Q128_CS_LOW();
    // 发送指令03h
    HAL_SPI_Transmit(&hspi1, &addr_origin, 1, 1000);
    // spi_read_write_byte(0x03);
    // 发送24位读取数据地址的高8位
    HAL_SPI_Transmit(&hspi1, &addr_high, 1, 1000);
    // spi_read_write_byte((uint8_t)((read_addr)>>16));
    // 发送24位读取数据地址的中8位
    HAL_SPI_Transmit(&hspi1, &addr_mid, 1, 1000);
    // spi_read_write_byte((uint8_t)((read_addr)>>8));
    // 发送24位读取数据地址的低8位
    HAL_SPI_Transmit(&hspi1, &addr_low, 1, 1000);
    // spi_read_write_byte((uint8_t)read_addr);
    // 根据读取长度读取出地址保存到buffer中
    for (i = 0; i < read_length; i++)
    {
        HAL_SPI_TransmitReceive(&hspi1, &tx_buff, &buffer[i], 1, 1000);
        // buffer[i]= spi_read_write_byte(0XFF);
    }
    // 恢复CS端为高电平
    W25Q128_CS_HIGH();
}


/**
 * @brief 向 W25Q128 中写入 OTA 标志位变量
 * 
 */
void W25Q128_write_vatiable_into_flash() {
    W25Q128_write((uint8_t *)&st_ota_info, W25Q128_LAST_BLOCK_ADDRESS, OTA_INFO_SIZE);
}
