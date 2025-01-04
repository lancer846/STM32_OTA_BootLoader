#include "gd32f4xx.h"
#include "usart.h"
#include "delay.h"

int main(void) {
	usart_init();
	
	int a = 10;
	//u0_printf("a = %d\n", a);
	
	while(1) {
		u0_printf("1234\n");
		Delay_ms(1000);
		// 处理传输过来的数据
		if(qp.pos_ptr_out != qp.pos_ptr_in) {
			// out 与 in 的指针不相等，表示队列中有存入新的数据
			// 按照队列的每个元素的 start与end 为界限来进行处理
			// 1、获取该分区内元素的个数
			uint16_t byte_count = qp.pos_ptr_out->end - qp.pos_ptr_out->start + 1;

			for(uint16_t i = 0; i < byte_count; i++) {
				u0_printf("%c", qp.pos_ptr_out->start[i]);
			}
			u0_printf("\n");
			qp.pos_ptr_out++;
			// 指针列表已达到末尾，返回首位
			if(qp.pos_ptr_out == qp.pos_ptr_end) {
				qp.pos_ptr_out = &qp.pos_ptr[0];
			}
		
		}
	}

	return 0;
}
