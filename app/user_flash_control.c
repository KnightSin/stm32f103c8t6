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
#include "user_flash_control.h"
#include "user_flash.h"
#include "user_app_common.h"
extern EventGroupHandle_t sys_base_event_group;//引用外部的事件组
xTaskHandle FLASH_SIZE_TASK_PCB;//打印系统FLASH线程句柄
static unsigned short flash_size_value=0;
extern const sys_base_event_t flash_init_ok;
void flash_read_write_test(){
	char *buff = pvPortMalloc(512);
	flash_driver.initialize(0);
	flash_driver.write(0,"flash hardware test success",50,1);
	flash_driver.read(0,buff,50,1);
	xprintf_s("%s\n",buff);
	//flash裸板测试完成，事件置位
	xEventGroupSetBits(sys_base_event_group,flash_init_ok);
	vPortFree(buff);
}
void flash_test(){
	  flash_size_value = flash_size();
		xprintf_s("FLASH:%uKB\n",flash_size_value);
		//裸板flash测试
		//flash_read_write_test();
		//flash裸板测试完成，事件置位
		xEventGroupSetBits(sys_base_event_group,flash_init_ok);
		//任务完成，删除任务 删除自身，参数是NULL
		vTaskDelete(NULL);
}

