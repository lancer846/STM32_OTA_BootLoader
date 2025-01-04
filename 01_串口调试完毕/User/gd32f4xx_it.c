/*!
    \file    gd32f4xx_it.c
    \brief   interrupt service routines

    \version 2024-01-15, V3.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx_it.h"
//#include "main.h"
#include "systick.h"

#include "usart.h"

void USART0_IRQHandler() {
	if(SET == usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE)) {
		// 清除空闲标志位
		usart_data_receive(USART0);
		
		u0_printf("idle %d %d\n", qp.receive_count, dma_transfer_number_get(DMA1, DMA_CH5));
		
		// 获取 DMA 传输的数量
		qp.receive_count += (SINGLE_DATA_MAX + 1) - dma_transfer_number_get(DMA1, DMA_CH5);
		// 将 out 指针指向本次传输的最后一个元素
		qp.pos_ptr_in->end = &rx_buff[qp.receive_count - 1];
		qp.pos_ptr_in++;
		// 指针列表已达到末尾，返回首位
		if(qp.pos_ptr_in == qp.pos_ptr_end) {
			qp.pos_ptr_in = &qp.pos_ptr[0];
		}
		
		
		// 如果缓存区剩余数量大于单次可能最大传输字节数量
		if( POINTER_ARR_SIZE - qp.receive_count > SINGLE_DATA_MAX) {
			qp.pos_ptr_in->start = &rx_buff[qp.receive_count];
		} else {
			qp.pos_ptr_in->start = &rx_buff[0];
			qp.receive_count = 0;
		}
		
		// 重新部分配置 DMA
		dma_channel_disable(DMA1, DMA_CH5);
		dma_memory_address_config(DMA1, DMA_CH5, DMA_MEMORY_0, (uint32_t)qp.pos_ptr_in->start);
		dma_transfer_number_config(DMA1, DMA_CH5, SINGLE_DATA_MAX + 1);
		dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_FTF);
		dma_channel_enable(DMA1, DMA_CH5);
	}
}

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
    /* if NMI exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
    /* if SVC exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
    /* if DebugMon exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
    /* if PendSV exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
//    led_spark();
//    delay_decrement();
}
