#include "usart.h"

uint8_t rx_buff[POINTER_ARR_SIZE];
uint8_t tx_buff[POINTER_ARR_TX_SIZE];
QUEUE_POS_t qp;

void DMA_config() {
	rcu_periph_clock_enable(RCU_DMA1);
	
	dma_deinit(DMA1, DMA_CH5);
	dma_single_data_parameter_struct init_struct;
	dma_single_data_para_struct_init(&init_struct);
	
	init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
	init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	init_struct.memory0_addr = (uint32_t)rx_buff;
	init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
	init_struct.direction = DMA_PERIPH_TO_MEMORY;
	
	// 不需要 DMA 达到完成，靠 USART IDLE 中断来决定接收是否完成
	init_struct.number = SINGLE_DATA_MAX + 1;
	init_struct.priority = DMA_PRIORITY_HIGH;
	
	dma_single_data_mode_init(DMA1, DMA_CH5, &init_struct);
	dma_channel_subperipheral_select(DMA1, DMA_CH5, DMA_SUBPERI4);
	
	dma_channel_enable(DMA1, DMA_CH5);
	// 清理DMA标记
  dma_interrupt_flag_clear(DMA1, DMA_CH5, DMA_INT_FLAG_FTF);
}

// 初始化 指针队列
void QUEUE_POS_init() {
	// 指向自身指针队列的首元素地址
	qp.pos_ptr_in = &qp.pos_ptr[0];
	qp.pos_ptr_out = &qp.pos_ptr[0];
	qp.pos_ptr_in->start = &rx_buff[0];
	qp.receive_count = 0;
	qp.pos_ptr_end = &qp.pos_ptr[NUM - 1];
}

void usart_init() {
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	// 配置引脚复用
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
	
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
	
	usart_deinit(USART0);
	usart_baudrate_set(USART0, USART0_BAUDRATE);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	// 开启空闲检测中断
	usart_interrupt_enable(USART0, USART_INT_IDLE);
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable(USART0_IRQn, 2, 2);
	
	// 开启 DMA 通道
	usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
	DMA_config();
	QUEUE_POS_init();
	
	usart_enable(USART0);
}

void usart_send_byte(uint8_t byte) {
	usart_data_transmit(USART0, byte);
	while(usart_flag_get(USART0, USART_FLAG_TBE) == RESET);
}

void u0_printf(char* format, ...) {
	uint16_t i;
	va_list list_data;
	va_start(list_data, format);
	vsprintf((char* )tx_buff, format, list_data);
	va_end(list_data);
	
	for(i = 0; i < strlen((const char* )tx_buff); i++) {
		usart_send_byte(tx_buff[i]);
	}
}



