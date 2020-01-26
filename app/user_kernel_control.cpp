#include "user_kernel_control.hpp"
#include "user_timer.h"
#include "user_app_common.h"
#include "xprintf.h"
#include<vector>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<string>
using namespace std;
volatile uint32_t CPU_RunTime;
extern const sys_base_event_t sys_init_ok;
static EventBits_t r_event;//�¼�����ֵ
xTaskHandle Kernel_TASK_PCB;//Kernel�߳̾��
Kernel::Kernel(){
	TIM5_Int_Init(9,719);//0.1ms�ж�1��
}
Kernel::~Kernel(){
	
}
unsigned char CPU_RunInfo[400];
void Kernel::top_command(){
	memset(CPU_RunInfo,0,400);
	vTaskList((char *)&CPU_RunInfo); //��ȡ��������ʱ����Ϣ
	xprintf_s("---------------------------------------------\r\n");
	xprintf_s("������ ����״̬ ���ȼ� ʣ��ջ �������\r\n");
	xprintf_s("%s", CPU_RunInfo);
	xprintf_s("---------------------------------------------\r\n");

	memset(CPU_RunInfo,0,400); //��Ϣ����������

	vTaskGetRunTimeStats((char *)&CPU_RunInfo);

	xprintf_s("������ ���м��� ʹ����\r\n");
	xprintf_s("%s", CPU_RunInfo);
	xprintf_s("---------------------------------------------\r\n\n");
//	vTaskDelay(1000); /* ��ʱ1000 �� tick */
}
//��Ԫ��������c���ã�������������kernel��
void kernel_test_start(){
	 unsigned int save_value=0;
	//�ȴ��¼�
		r_event = xEventGroupWaitBits(sys_base_event_group,//�¼�����
											sys_init_ok,//�ȴ����¼�
											pdFALSE,//true�ȵ�������¼�λ
											pdTRUE,//true�߼���ȴ�
											portMAX_DELAY);//�ȴ�ʱ��	
		if(r_event & sys_init_ok == sys_init_ok){//sys_init_ok�����
			Kernel *kernel=new Kernel();
			for(;;){
				xTaskNotifyWait(top_command_value,top_command_value,&save_value, portMAX_DELAY);
				if(save_value==top_command_value){
					save_value=0;
					kernel->top_command();
				}
			}
		}
}
