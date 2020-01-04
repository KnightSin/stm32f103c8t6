/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
/*user Library*/
#include "user_usart.h"
#include "user_led.h"
#include "user_crc.h"
#include "user_dma.h"
#include "user_timer.h"
static void hardware_init(){
  //��Ϊ��û�м���os������ʹ��xprintf_s��ʹ�����º�����ָ��out��
	extern void xfprintf(void(*func)(unsigned char),	const char*	fmt,...);
	USART1_Config(115200);
	USART2_Config(115200);
	USART3_Config(115200);
	user_led_init();
	user_crc_init();
	xfprintf(usart1_obj.u_putc,"bsp setup!\r\n");
}

int main( void ){
	hardware_init();//��ʼ��Ӳ��ƽ̨
	software_init();//��ʼ��os��app
	vTaskStartScheduler();//��ʼ����
	return 0;
}
