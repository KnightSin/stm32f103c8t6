#include "user_timer.h"

/*������ʱ����*/
#if defined(TIM6)&&defined(TIM7)
/**********************************************************
** ������: TIM6_Int_Init
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1) /TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
***********************************************************/
static void TIM6_Int_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʹ��TIM6ʱ��
	
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ARRPreloadConfig(TIM6,ENABLE);//ʹ����װ�Ĵ����Ļ��幦��,��ʱ�����¼��ڴ˴μ�������Ż���Ч
	//��������жϣ�������ʽ�ж�
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);     //ʹ��ָ����TIM6�ж�,��������ж�
	TIM_UpdateRequestConfig(TIM6,TIM_UpdateSource_Global);//���������ʱ�����жϣ����ø����¼��������ж�
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);

}

volatile char tim6_int_flag = 0;
/**********************************************************
** ������: TIM6_IRQHandler
** ��������: ��ʱ��6���жϷ������
** �������: ��
** �������: ��
***********************************************************/
void TIM6_IRQHandler(void){
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET){
		//do your task
		tim6_int_flag++;
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//��������жϱ�־λ
	}
}
/**********************************************************
** ������: TIM7_Int_Init
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1) /TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
***********************************************************/
static void TIM7_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef           NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʹ��TIM7ʱ��
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ARRPreloadConfig(TIM7,ENABLE);//ʹ����װ�Ĵ����Ļ��幦��,��ʱ�����¼��ڴ˴μ�������Ż���Ч
	//��������жϣ�������ʽ�ж�
	TIM_ITConfig(TIM7,TIM_IT_Update, ENABLE);     //ʹ��ָ����TIM7�ж�,��������ж�
	TIM_UpdateRequestConfig(TIM7,TIM_UpdateSource_Global);//���������ʱ�����жϣ����ø����¼��������ж�
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
}
volatile char tim7_int_flag = 0;
/**********************************************************
** ������: TIM7_IRQHandler
** ��������: ��ʱ��7���жϷ������
** �������: ��
** �������: ��
***********************************************************/
void TIM7_IRQHandler(void){
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		//do your task
		tim7_int_flag++;
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);//��������жϱ�־λ
	}
}
/**********************************************************
** ������: Update
** ��������:  ������ʱ������
** �������: ��
** �������: ��
** ˵������ʱʱ��=(Ԥ��Ƶ��+1��*������ֵ+1) /TIM6ʱ��(72MHz)����λ(s)
   �������ʱ��t=(7200*10000)/72000000s=1s
***********************************************************/
static void update(TIM_TypeDef *TIMx,u16 arr,u16 psc,Update_option_t option){
	//��ֹ���������¼�
	TIM_UpdateDisableConfig(TIMx,ENABLE);
	/* Set the Autoreload value */
  TIMx->ARR = arr;
 
  /* Set the Prescaler value */
  TIMx->PSC = psc;
	//������������¼�
	TIM_UpdateDisableConfig(TIMx,DISABLE);
	if(option == immediately)		//ǿ�Ƹ��£�������Ч����
		TIM_GenerateEvent(TIMx,TIM_EventSource_Update);
}
static void start(TIM_TypeDef *TIMx){	
	TIM_Cmd(TIMx, ENABLE);     //ʹ�ܶ�ʱ��
}
static void stop(TIM_TypeDef *TIMx){
	TIM_Cmd(TIMx, DISABLE);     //ʧ�ܶ�ʱ��
}
static void deinit(TIM_TypeDef *TIMx){
	TIM_DeInit(TIMx);
}
const Basic_Timer_t tim6 = {
	TIM6_Int_Init,
	update,
	start,
	stop,
	deinit,
};
const Basic_Timer_t tim7 = {
	TIM7_Int_Init,
	update,
	start,
	stop,
	deinit,
};

#endif