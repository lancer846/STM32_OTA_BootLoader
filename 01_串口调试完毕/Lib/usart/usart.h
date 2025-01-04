#ifndef __USART_H__
#define __USART_H__

#include "gd32f4xx.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

// 缓冲区的数据大小
#define POINTER_ARR_SIZE 		2048
#define POINTER_ARR_TX_SIZE 2048
// 单次分片的数据最大值
#define SINGLE_DATA_MAX	 		256
#define NUM 						 		10

// 波特率
#define USART0_BAUDRATE	115200

// 记录起始与终点位置
typedef struct {
	uint8_t* start;
	uint8_t* end;
} POS_t;

typedef struct {
	uint16_t receive_count;
	POS_t pos_ptr[NUM];
	// 控制数据接收时的 pos_ptr 的某一位的指针
	POS_t* pos_ptr_in;
	// 控制数据 输出 / 处理 时的 pos_ptr 的某一位的指针
	POS_t* pos_ptr_out;
	POS_t* pos_ptr_end;
} QUEUE_POS_t;

extern uint8_t rx_buff[POINTER_ARR_SIZE];
extern QUEUE_POS_t qp;

void usart_init();
void DMA_config();
void usart_send_byte(uint8_t byte);
void u0_printf(char* format, ...);

#endif
