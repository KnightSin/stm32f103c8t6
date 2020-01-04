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
#include "user_timer.h"
xTaskHandle OPEN_LED_Task_TCB;
//ʹ��freertos�Ĺ�������api
void open_led(){
	int i =0;
	TIM3_PWM_Config(10*1000,0);//��ʼƵ����10khz��ռ�ձ�Ϊ0
	while(1){
		for(i=1;i<=40;i++){
			TIM3_PWM_Set_Rate(0.01*i);//����ռ�ձ���0.4
			vTaskDelay(20); /* ��ʱ20tick��������20ms */
		}
		for(i=1;i<=40;i++){
			TIM3_PWM_Set_Rate(0.01*(40-i));//����ռ�ձ���0
			vTaskDelay(20); /* ��ʱ20tick��������20ms */
		}
	}
}