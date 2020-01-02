#ifndef USER_TIMER_H
#define USER_TIMER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "user_common.h"
#include "stm32f10x.h"

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

#if defined(TIM6)&&defined(TIM7)
typedef struct{
	void(*init)(u16 arr,u16 psc);
	void(*update)(TIM_TypeDef *TIMx,u16 arr,u16 psc);
	void(*start)(TIM_TypeDef *TIMx);
	void(*stop)(TIM_TypeDef *TIMx);
	void(*deinit)(TIM_TypeDef *TIMx);
}Basic_Timer_t;

extern const Basic_Timer_t tim6;
extern const Basic_Timer_t tim7;
extern volatile char tim6_int_flag;//ÿ����һ���жϣ�ֵ��1
extern volatile char tim7_int_flag;
#endif

#ifdef __cplusplus
}
#endif
#endif