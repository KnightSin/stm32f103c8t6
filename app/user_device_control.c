/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "user_device_control.h"
//���������ⲿ�豸�Ĳ���
void eeprom_test();
xTaskHandle Device_Test_Task_TCB;
void device_test(){
	eeprom_test();
	vTaskDelete(NULL);//������ɣ�ɾ������
}