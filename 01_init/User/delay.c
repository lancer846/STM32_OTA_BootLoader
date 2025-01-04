#include "delay.h"

void Delay_us(uint16_t time_us)
{
    /*
        设置滴答定时器，其计数方式为倒计时（--），系统主频为 168MHz，也就是 1/168 us
    */

    // 设置装载值
    SysTick->LOAD = time_us * 168;
    // 配置系统时钟（1），计数结束不产生中断（0），使能定时器
    SysTick->CTRL |= 0x00000005;
    // 等待计数结束
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
        ;
    // 清除计数标志位
    SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
    // 关闭定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void Delay_ms(uint16_t ms)
{
    while (ms--) {
        Delay_us(1000);
    }
}
