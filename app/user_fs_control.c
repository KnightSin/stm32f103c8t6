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
#include "user_fs_control.h"
#include "ff.h"
#include "user_app_common.h"
extern const sys_base_event_t fs_mount_ok;
extern const sys_base_event_t flash_init_ok;
extern const sys_base_event_t fs_file_operate_ok;
static EventBits_t r_event;//�¼�����ֵ
FATFS *fs,fatfs;//fs�������
const int work_buff_len = 512;
char work[work_buff_len];
int clust_size = 0;
xTaskHandle FS_TEST_TASK_PCB;//��ӡϵͳFLASH�߳̾��
/***attention!!!!**/
/*****
freertos�������ջ��С�����Ծ�����Ҫ�������ջ���涨��ϴ���������������ջ���������
*********/
static FIL fp;
char buffer[20];
int count=0;

//fatfs�ļ�ϵͳʹ��:1)mkfs	\
2) mount	\
3)�ļ�����
//��������fs����ǰ�����������flash����\
Ϊ�ˣ����ǵȵ���flash��������¼����Ż�ִ��fs_init����ع����
void fs_init(){
		//�ȴ��¼�
		r_event = xEventGroupWaitBits(sys_base_event_group,//�¼�����
											flash_init_ok,//�ȴ����¼�
											pdFALSE,//true �ȵ�������¼�λ
											pdTRUE,//true�߼���ȴ�
											portMAX_DELAY);//�ȴ�ʱ��	
		if(r_event & flash_init_ok == flash_init_ok){//flash���������
			//�����ļ�ϵͳ��û���򴴽�
//			fs = &fatfs;
//			r_event = f_mount(fs, "FLASH",1);
//			if(FR_NO_FILESYSTEM == r_event){
//				xprintf_s("not exist filesystem,create it on flash...\n");
//				r_event = f_mkfs("FLASH", 0, work, work_buff_len);
//				configASSERT(!r_event);
//				xprintf_s("   %d   ",r_event);
//				r_event = f_mount(fs, "FLASH",1);
//				configASSERT(!r_event);
//			}
//			//�������ϣ�assert
//			configASSERT(!r_event);
//			r_event = f_getfree("FLASH",&clust_size,&fs);
//			configASSERT(!r_event);
//			xprintf_s("fs mount success,size:%d clust\n",clust_size);
			
			//�����ļ�ϵͳ��û���򴴽�
			fs = &fatfs;
			r_event = f_mount(fs, "OUT_FLASH",1);
			if(FR_NO_FILESYSTEM == r_event){
				xprintf_s("not exist filesystem,create it on flash...\n");
				r_event = f_mkfs("OUT_FLASH", 0, work, work_buff_len);
				configASSERT(!r_event);
				xprintf_s("   %d   ",r_event);
				r_event = f_mount(fs, "OUT_FLASH",1);
				configASSERT(!r_event);
			}
			//�������ϣ�assert
			configASSERT(!r_event);
			r_event = f_getfree("OUT_FLASH",&clust_size,&fs);
			configASSERT(!r_event);
			xprintf_s("fs mount success,size:%d clust\n",clust_size);
		}
		//fs��ʼ����ɣ��¼���λ
		xEventGroupSetBits(sys_base_event_group,fs_mount_ok);
}

void fs_test(){
  	fs_init();
	//�ȴ��¼�
		r_event = xEventGroupWaitBits(sys_base_event_group,//�¼�����
											fs_mount_ok,//�ȴ����¼�
											pdFALSE,//true �ȵ�������¼�λ
											pdTRUE,//true�߼���ȴ�
											portMAX_DELAY);//�ȴ�ʱ��	
		if(r_event & fs_mount_ok == fs_mount_ok){//fs�Ѿ�����
//			r_event = f_open(&fp, "FLASH:hello.txt", FA_CREATE_NEW | FA_WRITE);
//			configASSERT(!r_event);
//			f_write(&fp, "Hello, World!\n�������", 30, &count);
//			configASSERT(count == 30);
//			f_close(&fp);
//			r_event = f_open(&fp, "FLASH:hello.txt", FA_READ);
//			configASSERT(!r_event);
//			f_read(&fp,buffer,30,&count);
//			f_close(&fp);
//			xprintf_s("read text:%s\n",buffer);
			//fs������д��ɣ��¼���λ
			xEventGroupSetBits(sys_base_event_group,fs_file_operate_ok);
			//������ɣ�ɾ������  ɾ������������NULL
			vTaskDelete(NULL);
		}
}