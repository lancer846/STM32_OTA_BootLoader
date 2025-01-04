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
		// ���������������
		if(qp.pos_ptr_out != qp.pos_ptr_in) {
			// out �� in ��ָ�벻��ȣ���ʾ�������д����µ�����
			// ���ն��е�ÿ��Ԫ�ص� start��end Ϊ���������д���
			// 1����ȡ�÷�����Ԫ�صĸ���
			uint16_t byte_count = qp.pos_ptr_out->end - qp.pos_ptr_out->start + 1;

			for(uint16_t i = 0; i < byte_count; i++) {
				u0_printf("%c", qp.pos_ptr_out->start[i]);
			}
			u0_printf("\n");
			qp.pos_ptr_out++;
			// ָ���б��Ѵﵽĩβ��������λ
			if(qp.pos_ptr_out == qp.pos_ptr_end) {
				qp.pos_ptr_out = &qp.pos_ptr[0];
			}
		
		}
	}

	return 0;
}
