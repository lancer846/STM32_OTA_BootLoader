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
- B 区需要在 A 区更新完后，将 OTA 标志位清除
- A 区需要在程序下载完毕之后，将 OTA 标志位进行置位