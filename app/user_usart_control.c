/* Standard includes. */
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
/* Library includes. */
#include "stm32f10x_it.h"
//bsp includes
#include "user_usart_control.h"
#include "user_rcc_control.h"
#include "user_app_common.h"

//使用freertos的绝对延时函数
xTaskHandle USART_HEART_BEAT;//发送给串口的心跳信息
/* 保存上次时间，调用后系统会自动更新它*/
static portTickType PreviousWakeTime;
static EventBits_t r_event;//事件返回值
void usart_heart_beat(){
	/* 设置延时时间，把时间转化tick节拍数 */
 const portTickType TimeIncrement = pdMS_TO_TICKS(5000);
	/* 获取当前系统时间 */
 PreviousWakeTime = xTaskGetTickCount();
		while(1){
			//等待串口空闲事件
			r_event = xEventGroupWaitBits(sys_base_event_group,//事件组句柄
											debug_serial_idle,//等待的事件
											pdTRUE,//true 等到后清除事件位
											pdTRUE,//true逻辑与等待
											portMAX_DELAY);//等待时间
			if(r_event &(debug_serial_idle)==debug_serial_idle){
				printf("系统已运行%lu秒\n",PreviousWakeTime/1000);
				//标志串口空闲事件
				xEventGroupSetBits(sys_base_event_group,debug_serial_idle);
			}
			vTaskDelayUntil(&PreviousWakeTime,TimeIncrement); /* 延时5000 tick，这里是5000ms */
		}
}
