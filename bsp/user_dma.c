#include "user_dma.h"
 
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
static void user_Dma_init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar){
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����ʱ��
#if ZET6==1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//ʹ��DMA����ʱ��
#endif
  DMA_DeInit(DMA_CHx);   //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ��xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���  	

	//DMA��NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
#if ZET6 == 1
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel1_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
#endif
} 
//����һ��DMA����
static void user_Dma_Enable(DMA_Channel_TypeDef*DMA_CHx,uint transmit_counter){ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ�� 
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);	//��������ж�
	DMA_ITConfig(DMA_CHx, DMA_IT_TE, ENABLE);	//��������ж�
 	DMA_SetCurrDataCounter(DMA_CHx,transmit_counter);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  
//�ر�DMA����
static void user_Dma_Disable(DMA_Channel_TypeDef*DMA_CHx){
	DMA_Cmd(DMA_CHx, DISABLE );
}

volatile uint failed_counter[7+5]={0,0,0,0,0,0,0,0,0,0,0,0};
void DMA1_Channel1_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC1);//���ͨ��������ɱ�־
		failed_counter[0] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE1) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE1);//���ͨ����������־
		failed_counter[0] = DMA_GetCurrDataCounter(DMA1_Channel1);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL1);//���ж�
}

void DMA1_Channel2_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC2);//���ͨ��������ɱ�־
		failed_counter[1] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE2) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE2);//���ͨ����������־
		failed_counter[1] = DMA_GetCurrDataCounter(DMA1_Channel2);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL2);//���ж�	
}
void DMA1_Channel3_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��������ɱ�־
		failed_counter[2] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE3) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE3);//���ͨ����������־
		failed_counter[2] = DMA_GetCurrDataCounter(DMA1_Channel3);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL3);//���ж�	
}
void DMA1_Channel4_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��������ɱ�־
		failed_counter[3] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE4) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE4);//���ͨ����������־
		failed_counter[3] = DMA_GetCurrDataCounter(DMA1_Channel4);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL4);//���ж�	
}
void DMA1_Channel5_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC5);//���ͨ��������ɱ�־
		failed_counter[4] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE5) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE5);//���ͨ����������־
		failed_counter[4] = DMA_GetCurrDataCounter(DMA1_Channel5);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL5);//���ж�	
}
void DMA1_Channel6_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC6) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC1);//���ͨ��������ɱ�־
		failed_counter[5] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE6) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE6);//���ͨ����������־
		failed_counter[5] = DMA_GetCurrDataCounter(DMA1_Channel6);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL6);//���ж�	
}
void DMA1_Channel7_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TC7);//���ͨ��������ɱ�־
		failed_counter[6] = 0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TE7) == SET){//�������
		DMA_ClearFlag(DMA1_FLAG_TE7);//���ͨ����������־
		failed_counter[6] = DMA_GetCurrDataCounter(DMA1_Channel7);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA1_FLAG_GL7);//���ж�	
}


void DMA2_Channel1_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA2_FLAG_TC1) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TC1);//���ͨ��������ɱ�־
		failed_counter[7] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE1) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TE1);//���ͨ����������־
		failed_counter[7] = DMA_GetCurrDataCounter(DMA2_Channel1);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA2_FLAG_GL1);//���ж�	
}
void DMA2_Channel2_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA2_FLAG_TC2) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TC2);//���ͨ��������ɱ�־
		failed_counter[8] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE2) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TE2);//���ͨ����������־
		failed_counter[8] = DMA_GetCurrDataCounter(DMA2_Channel2);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA2_FLAG_GL2);//���ж�	
}
void DMA2_Channel3_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA2_FLAG_TC3) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TC3);//���ͨ��������ɱ�־
		failed_counter[9] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE3) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TE3);//���ͨ����������־
		failed_counter[9] = DMA_GetCurrDataCounter(DMA2_Channel3);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA2_FLAG_GL3);//���ж�	
}

void DMA2_Channel4_5_IRQHandler(){
	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA2_FLAG_TC4) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TC4);//���ͨ��������ɱ�־
		failed_counter[10] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE4) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TE4);//���ͨ����������־
		failed_counter[10] = DMA_GetCurrDataCounter(DMA1_Channel4);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA2_FLAG_GL4);//���ж�	

	//��ȡ��־λ���鿴dma�������
	if(DMA_GetFlagStatus(DMA2_FLAG_TC5) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TC5);//���ͨ��������ɱ�־
		failed_counter[11] = 0;
	}
	if(DMA_GetFlagStatus(DMA2_FLAG_TE5) == SET){//�������
		DMA_ClearFlag(DMA2_FLAG_TE5);//���ͨ����������־
		failed_counter[11] = DMA_GetCurrDataCounter(DMA1_Channel5);//�õ���ǰ��ʣ����ٸ�����
	}
	DMA_ClearFlag(DMA2_FLAG_GL5);//���ж�
}


const Dma_t dma1 = {
	user_Dma_init,
	user_Dma_Enable,
	user_Dma_Disable,
};
const Dma_t dma2 = {
	user_Dma_init,
	user_Dma_Enable,
	user_Dma_Disable,
};

//demo:
/*
//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����SEND_BUF_SIZE.
user_Dma_init(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //ʹ�ܴ���1��DMA����  �⺯��    
user_Dma_Enable(DMA1_Channel4);//��ʼһ��DMA���䣡	 
if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)	//�ж�ͨ��4�������
  DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
DMA_GetCurrDataCounter(DMA1_Channel4);//�õ���ǰ��ʣ����ٸ�����

*/



