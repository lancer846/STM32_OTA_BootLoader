#include "gd32f4xx.h"
#include "usart.h"

int main(void) {
	usart_init();
	
	int a = 10;
	u0_printf("a = %d\n", a);
	
	uint8_t part_num = 0;
	while(1) {
		// ���������������
		if(qp.pos_ptr_out != qp.pos_ptr_in) {
			// out �� in ��ָ�벻��ȣ���ʾ�������д����µ�����
			// ���ն��е�ÿ��Ԫ�ص� start��end Ϊ���������д���
			// 1����ȡ�÷�����Ԫ�صĸ���
			uint16_t byte_count = qp.pos_ptr_out->end - qp.pos_ptr_out->start + 1;
			u0_printf("��Ƭ�� %d ���� %d ��Ԫ�أ�\n", part_num, byte_count);
			if(part_num >= 1) {
				u0_printf("ddd  %c\n", *(qp.pos_ptr[1].start));
			}
			
			for(uint16_t i = 0; i < byte_count; i++) {
				u0_printf("%c", qp.pos_ptr_out->start[i]);
			}
			u0_printf("\n");
			qp.pos_ptr_out++;
			// ָ���б��Ѵﵽĩβ��������λ
			if(qp.pos_ptr_out == qp.pos_ptr_end) {
				qp.pos_ptr_out = &qp.pos_ptr[0];
			}
		
			part_num++;
			
		}
	}

	return 0;
}
