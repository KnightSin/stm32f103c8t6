#ifndef USER_APP_COMMON_H
#define USER_APP_COMMON_H
/*@brief
���APP��Ҫ�õ�ȫ��OS����
**********/

#include "user_common.h"
#include "user_led_control.h"
#include "user_usart_control.h"
#include "user_pwr_control.h"
#include "user_crc_control.h"
#include "user_rcc_control.h"
#include "user_flash_control.h"
#include "user_fs_control.h"
#include "user_io_control.h"
#include "user_shell_control.h"
#include "user_exti_control.h"
#include "user_timer_control.h"
#include "user_device_control.h"
typedef enum{
	SYS_INIT_OK = (0x01<<0),
	FLASH_INIT_OK = (0x01<<1),
	FS_MOUNT_OK = (0x01<<2),
	FS_FILE_OPERATE_OK = (0x01<<3),
	IO_OPERATE_OK = (0x01<<4),
}sys_base_event_t;//ö������


//��Cд�ĺ��������Ҫ��C++���ã���Ҫ��extern  ��C���ķ�ʽ������hpp�ļ�������
//C�����б�
void xprintf_s(const char*	fmt,...);


//��C++д�ĺ�������Ӧ��ֱ�ӱ�Cֱ�Ӱ�����ֻ����extern�ķ�ʽ���ŵ�commonͷ�ļ��С�
//C++�����б�
extern void do_action();

#endif