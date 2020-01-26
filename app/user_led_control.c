/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* Library includes. */
#include "stm32f10x_it.h"
/*user Library*/
#include "user_led.h"
#include "user_led_control.h"
xTaskHandle OPEN_LED_Task_TCB;
xTaskHandle CLOSE_LED_Task_TCB;
//ʹ��freertos�Ĺ�������api
void open_led(){
	while(1){
		user_led_on();
		vTaskSuspend(CLOSE_LED_Task_TCB);//���ƺ����ص�����
		vTaskDelay(500); /* ��ʱ500 tick��������500ms */
		vTaskResume(CLOSE_LED_Task_TCB);//500ms��ָ��ص�����
	}
}

void close_led(){
	 while(1){
		user_led_off();
		vTaskSuspend(OPEN_LED_Task_TCB);//�صƺ���𿪵�����
		vTaskDelay(500); /* ��ʱ500 tick��������500ms */
		vTaskResume(OPEN_LED_Task_TCB);//500ms��ָ���������
	}
}