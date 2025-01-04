#include "bsp_internal_flash.h"

/** 固件库定义的用于扇区写入到SNB寄存器位的宏(stm32f4xx_hal_flash.h文件)
 * @{
 */
// #define FLASH_SECTOR_0 ((uint32_t)0U) /*!< Sector Number 0   */
// #define FLASH_SECTOR_1 ((uint32_t)1U) /*!< Sector Number 1   */
// #define FLASH_SECTOR_2 ((uint32_t)2U) /*!< Sector Number 2   */
// #define FLASH_SECTOR_3 ((uint32_t)3U) /*!< Sector Number 3   */
// #define FLASH_SECTOR_4 ((uint32_t)4U) /*!< Sector Number 4   */
// #define FLASH_SECTOR_5 ((uint32_t)5U) /*!< Sector Number 5   */
// #define FLASH_SECTOR_6 ((uint32_t)6U) /*!< Sector Number 6   */
// #define FLASH_SECTOR_7 ((uint32_t)7U) /*!< Sector Number 7   */
/**
   * @brief  根据输入的地址给出它所在的sector
   *         例如：
            uwStartSector = GetSector(FLASH_USER_START_ADDR);
            uwEndSector = GetSector(FLASH_USER_END_ADDR);
   * @param  Address：地址
   * @retval 地址所在的sector
   */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) &&
             (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) &&
             (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) &&
             (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) &&
             (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) &&
             (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) &&
             (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else
    { /*(Address < FLASH_END_ADDR) &&
(Address >= ADDR_FLASH_SECTOR_23))*/
        sector = FLASH_SECTOR_7;
    }
    return sector;
}

/*准备写入的测试数据*/
#define DATA_32 ((uint32_t)0x87645321)

/* Exported types -----------------------------------------------------*/
/* Exported constants -------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_7
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR ADDR_FLASH_SECTOR_7

static uint32_t GetSector(uint32_t Address);

/**
 * @brief  InternalFlash_Test,对内部FLASH进行读写测试
 * @param  None
 * @retval None
 */
int InternalFlash_Test(void)
{
    /*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
    uint32_t FirstSector = 0;
    uint32_t NbOfSectors = 0;

    uint32_t SECTORError = 0;

    uint32_t Address = 0;

    __IO uint32_t Data32 = 0;
    __IO uint32_t MemoryProgramStatus = 0;
    static FLASH_EraseInitTypeDef EraseInitStruct;

    /* FLASH 解锁 ********************************/
    /* 使能访问FLASH控制寄存器 */
    HAL_FLASH_Unlock();

    FirstSector = GetSector(FLASH_USER_START_ADDR);
    NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

    /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
    /* Fill EraseInit structure*/
    // 扇区擦除
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    /*以“字”的大小进行操作， // 表示整字擦除，32bit，即 4 个字节 */
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    /* 开始擦除操作 */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        /*擦除出错，返回，实际应用中可加入处理 */
        return -1;
    }

    /* 以“字”的大小为单位写入数据 ********************************/
    Address = FLASH_USER_START_ADDR;
    // 0x20000 表示 128kb
    while (Address < (FLASH_USER_END_ADDR + 0x20000))
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                              Address, DATA_32) == HAL_OK)
        {
            Address = Address + 4;
        }
        else
        {
            /*写入出错，返回，实际应用中可加入处理 */
            return -1;
        }
    }

    /* 给FLASH上锁，防止内容被篡改*/
    HAL_FLASH_Lock();

    /* 从FLASH中读取出数据进行校验***************************************/
    /*  MemoryProgramStatus = 0: 写入的数据正确
       MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = 0;

    while (Address < (FLASH_USER_END_ADDR + 0x20000))
    {
        Data32 = *(__IO uint32_t *)Address;

        if (Data32 != DATA_32)
        {
            MemoryProgramStatus++;
        }

        Address = Address + 4;
    }
    /* 数据校验不正确 */
    if (MemoryProgramStatus)
    {
        return -1;
    }
    else
    { /*数据校验正确*/
        return 0;
    }
}

/**
 * @brief 
 * 
 * @param sector 
 *  @arg FLASH_SECTOR_0
 *  @arg FLASH_SECTOR_1
 *  @arg FLASH_SECTOR_2
 *  @arg FLASH_SECTOR_3
 *  @arg FLASH_SECTOR_4
 *  @arg FLASH_SECTOR_5
 *  @arg FLASH_SECTOR_6
 *  @arg FLASH_SECTOR_7
 * @param num 扇区连续的数量
 */
int internal_flash_erase(uint32_t sector, uint32_t num) {
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError = 0;
    /* FLASH 解锁 ********************************/
    /* 使能访问FLASH控制寄存器 */
    HAL_FLASH_Unlock();

    /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
    /* Fill EraseInit structure*/
    // 扇区擦除
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    /*以“字”的大小进行操作， // 表示整字擦除，32bit，即 4 个字节 */
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = num;
    /* 开始擦除操作 */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        /*擦除出错，返回，实际应用中可加入处理 */
        return -1;
    }
    /* 给FLASH上锁，防止内容被篡改*/
    HAL_FLASH_Lock();
    return 0;
}

/**
 * @brief 
 * 
 * @param addr 写入的 flash 的扇区地址
 * @param buf 需要写入的数据的地址
 * @param len 需要写入的数据的字节长度
 */
void internal_flash_write(uint32_t addr, uint32_t *buf, uint32_t len) {
    uint32_t i;
    /* FLASH 解锁 ********************************/
    /* 使能访问FLASH控制寄存器 */
    HAL_FLASH_Unlock();

    for (i = 0; i < len; i += 4) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *buf);
        addr += 4;
        buf += 4;
    }

    /* 给FLASH上锁，防止内容被篡改*/
    HAL_FLASH_Lock();
}
