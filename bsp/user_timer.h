#ifndef USER_TIMER_H
#define USER_TIMER_H
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
typedef enum{
	immediately=0,
}Update_option_t;

#endif