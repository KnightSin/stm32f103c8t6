
/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
//user include
#include "user_timer_control.h"
#include "user_timer.h"
#include "user_app_common.h"
#include "user_led.h"
extern volatile u8  TIM5CH1_CAPTURE_STA;	//���벶��״̬		    				
extern volatile u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
xTaskHandle Basic_Timer_Test_Task_TCB;

void show_capture(){
	u32 temp=0;
	if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			xprintf_s("HIGH:%d us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA=0;//������һ�β���
		}
}

void basic_timer_test(){
	int i =10;
	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ���
	tim6.init(7199,59999);//�������6sһ�ε��ж�
	tim7.init(7199,59999);//�������6sһ�ε��ж�
	tim6.start(TIM6);
	tim6.start(TIM7);
	while(i--){
		if(i==5){
			tim6.update(TIM6,7199,9999,immediately);//�������1sһ�ε��ж�
			tim7.update(TIM7,7199,9999,immediately);
		}
		xprintf_s("tim6:%d tim7:%d\r\n",tim6_int_flag,tim7_int_flag);
		vTaskDelay(1001);//����1001ms
	}
	while(1){
		show_capture();
		vTaskDelay(100);
	}
	vTaskDelete(NULL);
}