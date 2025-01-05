# STM32_OTA

- 在 stm32F407 中以扇区来进行擦写，该芯片主功能区存有 8 个扇区
- 将内部划分为 B 区和 A 区，B区在前
  - B 区存放 BootLoader，A 区存放程序功能代码

## BootLoader 的作用：

- 系统开机，先进入 B 区。

- 1、没有 OTA 事件时，直接跳转到 A 区执行功能代码
- 2、有 OTA 事件时，擦除 A 区代码，将新代码写入 A 区

- 暂定 BootLoader 大小为 16Kbytes，占据扇区 0（Sector 0）

![image-20250104200114846](C:\Users\G\AppData\Roaming\Typora\typora-user-images\image-20250104200114846.png)

## OTA标志位

- 在开发板上没有 EEPROM，所以将 OTA 标志位同样存入了外置 flash W25Q128 中，与固件升级包放入不同区域
  - W25Q128将16M的容量分为256个块（Block),每个块大小为64K个字节，将标志位放入最后一个 Block 中
- B 区需要在 A 区更新完后，将 OTA 标志位清除
- A 区需要在程序下载完毕之后，将 OTA 标志位进行置位

## 分区跳转任务

- 1、设置 sp，A 区起始位置 0x0800 4000，给到 sp
  - sp 表示 Stack Pointer（__initial_sp），由向量表决定
- 2、设置 pc，A 区起始位置 0x0800 4000 + 4，给到 pc
  - 这里需要让 A 区程序认为是复位后就执行的，所以需要将 A 区的 Reset_Handler 交给 pc 进行执行
  - Reset_Handler 也是在 向量表中的，紧跟 __initial_sp 后

- 3、补充，需要将在 B 区使用到的外设进行 reset 初始化，保证其在跳转 A 区时是初始状态