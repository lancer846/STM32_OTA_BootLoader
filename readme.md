# STM32_OTA

- 在 stm32F407 中以扇区来进行擦写，该芯片主功能区存有 8 个扇区
- 将内部划分为 B 区和 A 区，B区在前
  - B 区存放 BootLoader，A 区存放程序功能代码

## BootLoader 的作用：

- 系统开机，先进入 B 区。

- 1、没有 OTA 事件时，直接跳转到 A 区执行功能代码
- 2、有 OTA 事件时，擦除 A 区代码，将新代码写入 A 区

- 暂定 BootLoader 大小为 32Kbytes，占据扇区 0、1（Sector 0、Sector 1）

![image-20250104200114846](C:\Users\G\AppData\Roaming\Typora\typora-user-images\image-20250104200114846.png)

## OTA标志位

- 在开发板上没有 EEPROM，所以将 OTA 标志位同样存入了外置 flash W25Q128 中，与固件升级包放入不同区域
  - W25Q128将16M的容量分为256个块（Block),每个块大小为64K个字节，将标志位放入最后一个 Block 中
- B 区需要在 A 区更新完后，将 OTA 标志位清除
- A 区需要在程序下载完毕之后，将 OTA 标志位进行置位

## 分区跳转任务

- 1、设置 sp，A 区起始位置 0x0800 8000，给到 sp
  - sp 表示 Stack Pointer（__initial_sp），由向量表决定
- 2、设置 pc，A 区起始位置 0x0800 8000 + 4，给到 pc
  - 这里需要让 A 区程序认为是复位后就执行的，所以需要将 A 区的 Reset_Handler 交给 pc 进行执行
  - Reset_Handler 也是在 向量表中的，紧跟 __initial_sp 后

- 3、补充，需要将在 B 区使用到的外设进行 reset 初始化，保证其在跳转 A 区时是初始状态

![image-20250105224554176](C:\Users\G\AppData\Roaming\Typora\typora-user-images\image-20250105224554176.png)

## OTA更新细节

- 1、谁将 OTA_flag 变成对勾
  - A 区

- 2、什么时候将 OTA_flag 变成对勾
  - A 区下载完毕之后

- 3、OTA 时，最新版本的程序文件下载到哪
  - 分片下载，[256]，W25Q128中

- 4、OTA 时，最新版本的程序文件如何下载？下载多少？
  - 服务器下发告诉我们上传的新版本程序的大小，字节数

- 4+、下载多少这个变量用不用保存
  - 需要保存，保存到 W25Q128 中

- 5、发生 OTA 事件时，B 区如何更新 A 区
  - 根据保存在 W25Q128 中的下载量，拿数据，写到 A 区

## 内部 flash 与 W25Q128

- STM32F407VGT6 内部 flash 有 512Kbytes，按照 sectors 来进行擦除，前面 sector0~3 是16Kbytes，sector4 是 64Kbytes，sector5~7 是 128Kbytes，故一次最少擦除 16Kbytes

- W25Q128 内部 flash 大小为 128Mbit，即16Mbytes，可以按照扇区 sector 或者块 block 进行擦除，sector 擦除一次为 4 Kbytes，block 擦除可以是 32Kbytes，或者 64Kbytes