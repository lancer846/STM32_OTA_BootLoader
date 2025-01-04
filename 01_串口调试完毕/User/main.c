#include "gd32f4xx.h"
#include "usart.h"

int main(void) {
	usart_init();
	
	int a = 10;
	u0_printf("a = %d\n", a);
	
	uint8_t part_num = 0;
	while(1) {
		// 处理传输过来的数据
		if(qp.pos_ptr_out != qp.pos_ptr_in) {
			// out 与 in 的指针不相等，表示队列中有存入新的数据
			// 按照队列的每个元素的 start与end 为界限来进行处理
			// 1、获取该分区内元素的个数
			uint16_t byte_count = qp.pos_ptr_out->end - qp.pos_ptr_out->start + 1;
			u0_printf("该片区 %d 内有 %d 个元素：\n", part_num, byte_count);
			if(part_num >= 1) {
				u0_printf("ddd  %c\n", *(qp.pos_ptr[1].start));
			}
			
			for(uint16_t i = 0; i < byte_count; i++) {
				u0_printf("%c", qp.pos_ptr_out->start[i]);
			}
			u0_printf("\n");
			qp.pos_ptr_out++;
			// 指针列表已达到末尾，返回首位
			if(qp.pos_ptr_out == qp.pos_ptr_end) {
				qp.pos_ptr_out = &qp.pos_ptr[0];
			}
		
			part_num++;
			
		}
	}

	return 0;
}
