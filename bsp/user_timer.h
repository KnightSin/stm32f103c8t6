#ifndef USER_TIMER_H
#define USER_TIMER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f10x_tim.h"
/*@brief
user_timerӦ���ṩ�����ʱ���ܺ�PWM�ȹ��ܣ�timer��Ϊһ�������ĵײ�֧��
�й���ʱ��Ķ�Ӧ����������ɡ�
����ҲҪע�⵽freertos�Ѿ��ṩ��һЩ�����ʱ��������freertos�Ķ�ʱ�����ǻ���
systick��ʱ�侫������tickΪ��λ�ġ�systick��ʱ��һ��AHBʱ��8��Ƶ����9MHZ��
systick�ĳ�ʼУ׼ֵ��9000����˲���1ms��ʱ��

*/

/*brief
stm32��Ӳ����ʱ��
TIM1��TIM8�߼����ƶ�ʱ��
TIM6��TIM7������ʱ��
TIM2~TIM5ͨ�ö�ʱ��
���������ͺŵ�stm32��֧�����ϵĶ�ʱ����
*/
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);
typedef enum{
	immediately=0,
	postpone=1,
}Update_option_t;
#ifdef __cplusplus
}
#endif
#endif