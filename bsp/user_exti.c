#include "user_exti.h"
/*
stm32��19��exit������ǰ16��������GPIO�ϣ�ͨ��ѡ�񣬿��Ը������е�GPIO
����3��������rtc���ӣ�usb��������豸
���⣬exit����ѡ���źŷ��͸�NVIC���������巢������
ǰ�߻����cortex-m3�ں��жϣ����߻���������źţ������źſ������ӵ����������衣
���жϺ��¼�������
*/

/*�¼�����
������ѡ��λ�¼��ߣ����ߵ��ж����ο������¼����ιرա����ɲ����¼�
�¼������巢���������ӵ���Ƭ����������
*/

/*exit��ʼ��*/
/*
AFIO��GPIOʱ��ʹ��
GPIO��ʼ��
EXIT������
�жϴ�����
*/

static struct{
	GPIO_TypeDef* GPIOx;//ע���gpio��
	uint EXTI_Line;//ע�����
	uint Pin;
	uint GPIO_PortSourceGPIOx;
	uint GPIO_PinSourcex;
	EXTITrigger_TypeDef Trigger;
	uint EXTIx_IRQn;
	void(*callback)();//ע����жϻص�������
}regist_exti_info[MAX_EXTI_NUM];

//�Ҹ�gpio�Ƿ�ע�����û���򷵻�-1�����򷵻��±�
//EXITxֻ�ܱ�ABCDEFGHI��ĳ��GPIOע�ᡣ��һ���߲��ܱ����ע��
static int find_exti(uint EXTI_Line){
	int i =0;
	for(;i<MAX_EXTI_NUM;i++){
		if(regist_exti_info[i].EXTI_Line==EXTI_Line)return i;
	}
	if(i == MAX_EXTI_NUM)return -1;
}
//�ҿ���ĵض�����ע��ṹ��û���򷵻�-1
static int find_empty_site(){
	int i =0;
	for(;i<MAX_EXTI_NUM;i++){
		if(regist_exti_info[i].EXTI_Line==0)return i;
	}
	if(i == MAX_EXTI_NUM)return -1;
}

static int exti_init(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn,void(*cb)()){
	int i = -1;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	//GPIOʱ�ӿ���
	switch((uint)GPIOx){
		case (uint)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
		case (uint)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
		case (uint)GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);break;
		case (uint)GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);break;
		default:break;
	}
	//1.ʹ���ⲿ�ж�ʱ��AFIOʱ�ӱ���ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2.GPIO����
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//
  EXTI_Init(&EXTI_InitStructure);//
  //4.NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	//��ʼ��ע��ṹ��
	i = find_exti(EXTI_Linex);//����Ѿ������ˣ�����½ṹ��
	if(i == -1)
		i = find_empty_site();//��������ڣ����½�
	if(i!= -1){
		regist_exti_info[i].callback = cb;
		regist_exti_info[i].EXTI_Line = EXTI_Linex;
		regist_exti_info[i].GPIOx = GPIOx;
		regist_exti_info[i].EXTIx_IRQn = EXTIx_IRQn;
		regist_exti_info[i].GPIO_PinSourcex = GPIO_PinSourcex;
		regist_exti_info[i].GPIO_PortSourceGPIOx = GPIO_PortSourceGPIOx;
		regist_exti_info[i].Pin = Pin;
		regist_exti_info[i].Trigger = Trigger;
		EXTI->IMR &= ~(EXTI_Linex); //Ĭ�Ϲرյ�
		EXTI->EMR &= ~(EXTI_Linex); //Ĭ�Ϲرյ�
		return 0;
	}
	else return -1;
}

static void exti_start(uint EXTI_Line){
	//1.ʹ���ⲿ�ж�ʱ��AFIOʱ�ӱ���ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	EXTI->IMR |= EXTI_Line;//�����ж�
	EXTI->EMR &= ~(EXTI_Line); //�ر��¼�
}

static void exti_stop(uint EXTI_Line){
	EXTI->IMR &= ~(EXTI_Line); //�ر��ж�
	EXTI->EMR &= ~(EXTI_Line); //�ر��¼�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
}


static void exti_trigger(uint EXTI_Line){
	EXTI_GenerateSWInterrupt(EXTI_Line);
}

static int exti_deinit(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn
){
	int i =-1;
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  //2.GPIO����
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO???50MHz
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //?EXIT?9???PB9
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //???????
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;//?????
  EXTI_Init(&EXTI_InitStructure);//?????
	//�ҵ�ע�������
	i = find_exti(EXTI_Linex);
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = 0;
		regist_exti_info[i].GPIOx = 0;
		regist_exti_info[i].EXTIx_IRQn = 0;
		regist_exti_info[i].GPIO_PinSourcex = 0;
		regist_exti_info[i].GPIO_PortSourceGPIOx = 0;
		regist_exti_info[i].Pin = 0;
		regist_exti_info[i].Trigger = 0;
		EXTI->IMR &= ~(EXTI_Linex); //�ر��ж�
		EXTI->EMR &= ~(EXTI_Linex); //�ر��¼�
		return 0;
	}
	else return -1;
}

void EXTI0_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line0) != RESET){  //EXTI0�ж�
		 i = find_exti(EXTI_Line0);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line0);//�����־λ
		 }
	}
}
void EXTI1_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line1) != RESET){  //EXTI1�ж�
		 i = find_exti(EXTI_Line1);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line1);//�����־λ
		 }
	}	
}
void EXTI2_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line2) != RESET){  //EXTI2�ж�
		 i = find_exti(EXTI_Line2);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line2);//�����־λ
		 }
	}	
}
void EXTI3_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line3) != RESET){  //EXTI3�ж�
		 i = find_exti(EXTI_Line3);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line3);//�����־λ
		 }
	}	
}
void EXTI4_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line4) != RESET){  //EXTI4�ж�
		 i = find_exti(EXTI_Line4);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line4);//�����־λ
		 }
	}	
}
/*
void EXTI9_5_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line5) != RESET){  //EXTI5�ж�
		 i = find_exti(EXTI_Line5);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line5);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line6) != RESET){  //EXTI6�ж�
		 i = find_exti(EXTI_Line6);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line6);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line7) != RESET){  //EXTI7�ж�
		 i = find_exti(EXTI_Line7);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line7);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line8) != RESET){  //EXTI8�ж�
		 i = find_exti(EXTI_Line8);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line8);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line9) != RESET){  //EXTI9�ж�
		 i = find_exti(EXTI_Line9);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line9);//�����־λ
		 }
	}
}
*/
void EXTI15_10_IRQHandler(void){
	int i = -1;
  if(EXTI_GetITStatus(EXTI_Line10) != RESET){  //EXTI10�ж�
		 i = find_exti(EXTI_Line10);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line10);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line11) != RESET){  //EXTI11�ж�
		 i = find_exti(EXTI_Line11);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line11);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line12) != RESET){  //EXTI12�ж�
		 i = find_exti(EXTI_Line12);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line12);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line13) != RESET){  //EXTI13�ж�
		 i = find_exti(EXTI_Line13);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line13);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line14) != RESET){  //EXTI14�ж�
		 i = find_exti(EXTI_Line14);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line14);//�����־λ
		 }
	}
  if(EXTI_GetITStatus(EXTI_Line15) != RESET){  //EXTI15�ж�
		 i = find_exti(EXTI_Line15);
		 if(i != -1){
			regist_exti_info[i].callback();//ִ�лص�
			EXTI_ClearITPendingBit(EXTI_Line15);//�����־λ
		 }
	}	
}


const Exti_St exti_obj={
	exti_init,
	exti_start,
	exti_stop,
	exti_trigger,
	exti_deinit,
};



static int event_init(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn){
	int i = -1;
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	//GPIOʱ�ӿ���
	switch((uint)GPIOx){
		case (uint)GPIOA:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);break;
		case (uint)GPIOB:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);break;
		case (uint)GPIOC:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);break;
		case (uint)GPIOD:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);break;
		case (uint)GPIOE:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);break;
		case (uint)GPIOF:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);break;
		case (uint)GPIOG:RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);break;
		default:break;
	}
	//1.ʹ���ⲿ�ж�ʱ��AFIOʱ�ӱ���ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  //2.GPIO����
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//
  EXTI_Init(&EXTI_InitStructure);//
	//��ʼ��ע��ṹ��
	i = find_exti(EXTI_Linex);//����Ѿ������ˣ�����½ṹ��
	if(i == -1)
		i = find_empty_site();//��������ڣ����½�
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = EXTI_Linex;
		regist_exti_info[i].GPIOx = GPIOx;
		regist_exti_info[i].EXTIx_IRQn = EXTIx_IRQn;
		regist_exti_info[i].GPIO_PinSourcex = GPIO_PinSourcex;
		regist_exti_info[i].GPIO_PortSourceGPIOx = GPIO_PortSourceGPIOx;
		regist_exti_info[i].Pin = Pin;
		regist_exti_info[i].Trigger = Trigger;
		EXTI->IMR &= ~(EXTI_Linex); //�ر��ж�
		EXTI->EMR &= ~(EXTI_Linex); //�ر��¼�
		return 0;
	}
	else return -1;
}



static void event_start(uint EXTI_Line){
	//1.ʹ���ⲿ�ж�ʱ��AFIOʱ�ӱ���ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	EXTI->IMR &= ~(EXTI_Line); //�ر��ж�
	EXTI->EMR |= EXTI_Line; //�����¼�
}

static void event_stop(uint EXTI_Line){
	EXTI->IMR &= ~(EXTI_Line); //�ر��ж�
	EXTI->EMR &= ~(EXTI_Line); //�ر��¼�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
}
static void event_trigger(uint EXTI_Line){
	EXTI_GenerateSWInterrupt(EXTI_Line);
}
static int event_deinit(GPIO_TypeDef* GPIOx,uint Pin,
uint GPIO_PortSourceGPIOx,uint GPIO_PinSourcex,
uint EXTI_Linex,EXTITrigger_TypeDef Trigger,
uint EXTIx_IRQn
){
	int i =-1;
	GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  //2.GPIO����
  GPIO_InitStructure.GPIO_Pin = Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO???50MHz
  GPIO_Init(GPIOx,&GPIO_InitStructure);  
  //3.EXIT������
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOx,GPIO_PinSourcex);  //?EXIT?9???PB9
  EXTI_InitStructure.EXTI_Line = EXTI_Linex;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
  EXTI_InitStructure.EXTI_Trigger = Trigger;  //???????
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;//?????
  EXTI_Init(&EXTI_InitStructure);//?????
	//�ҵ�ע�������
	i = find_exti(EXTI_Linex);
	if(i!= -1){
		regist_exti_info[i].callback = 0;
		regist_exti_info[i].EXTI_Line = 0;
		regist_exti_info[i].GPIOx = 0;
		regist_exti_info[i].EXTIx_IRQn = 0;
		regist_exti_info[i].GPIO_PinSourcex = 0;
		regist_exti_info[i].GPIO_PortSourceGPIOx = 0;
		regist_exti_info[i].Pin = 0;
		regist_exti_info[i].Trigger = 0;
		EXTI->IMR &= ~(EXTI_Linex); //�ر��ж�
		EXTI->EMR &= ~(EXTI_Linex); //�ر��¼�
		return 0;
	}
	else return -1;
}


const Event_St event_obj={
	event_init,
	event_start,
	event_stop,
	event_trigger,
	event_deinit,
};
