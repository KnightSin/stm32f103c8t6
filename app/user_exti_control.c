/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
//user include
#include "user_exti.h"
#include "user_exti_control.h"
#include "user_app_common.h"
#include "user_led.h"

//����:����ͬ���������ڴ�ӡ�����Ĳ�ͬ���ֻ��ۼ�
xTaskHandle Exti_Test_Task_TCB;
//�������key0������PE4
//key1 PE3
//key2 PE2
//�жϻص�
static volatile uint flag0 = 0;
static volatile uint flag1 = 0;
static volatile uint flag2 = 0;
//�˻ص��������ں�̬�ĳ��򡣰��ں�̬�����û������ܲ���ȫ��Ӧ�ý���os���Ľ�һ��
static void callback0(){
	flag0++;
}
static void callback1(){
	flag1++;

}
static void callback2(){
	flag2++;
}
extern const sys_base_event_t io_operate_ok;
static EventBits_t r_event;//�¼�����ֵ
void exti_test(){
	int i =3;
		//�ȴ��¼�
		r_event = xEventGroupWaitBits(sys_base_event_group,//�¼�����
											io_operate_ok,//�ȴ����¼�
											pdFALSE,//true �ȵ�������¼�λ
											pdTRUE,//true�߼���ȴ�
											portMAX_DELAY);//�ȴ�ʱ��	
		if(r_event & io_operate_ok == io_operate_ok){//io�Ѿ�����
					exti_obj.init(GPIOA,GPIO_Pin_3,GPIO_PortSourceGPIOA,GPIO_PinSource3,
																EXTI_Line3,EXTI_Trigger_Rising,EXTI3_IRQn,callback1);
					exti_obj.init(GPIOA,GPIO_Pin_4,GPIO_PortSourceGPIOA,GPIO_PinSource4,
																EXTI_Line4,EXTI_Trigger_Rising,EXTI4_IRQn,callback0);
					exti_obj.init(GPIOA,GPIO_Pin_2,GPIO_PortSourceGPIOA,GPIO_PinSource2,
																EXTI_Line2,EXTI_Trigger_Rising,EXTI2_IRQn,callback2);
		}
		while(i--){
			vTaskDelay(2000);//����
			xprintf_s("%u,%u,%u\r\n",flag0,flag1,flag2);
		}
		i=3;
		exti_obj.start(EXTI_Line2|EXTI_Line3|EXTI_Line4);
		while(i--){
			vTaskDelay(2000);//����
			xprintf_s("%u,%u,%u\r\n",flag0,flag1,flag2);
		}
		i=3;
		exti_obj.stop(EXTI_Line2|EXTI_Line3|EXTI_Line4);
		while(i--){
			vTaskDelay(2000);//����
			xprintf_s("%u,%u,%u\r\n",flag0,flag1,flag2);
		}
		vTaskDelete(NULL);
}
