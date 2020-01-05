#include "user_spi.h"
#include "stm32f10x_spi.h"
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
/*********attetion����������������************************/
//�����ѯ��־λ���Ͳ�Ҫ�����жϣ����������־λ����ᵼ��SPI1_ReadWriteByte��������־λ
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData){		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}

SPI_InitTypeDef  SPI_InitStructure1;
void SPI1_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	SPI_InitStructure1.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure1.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure1.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure1.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure1.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure1.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure1.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure1.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure1.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure1);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	//NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	 //�ж�����
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE|SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
  
void SPI1_SetSpeed(u8 SpeedSet){
	SPI_InitStructure1.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure1);
	SPI_Cmd(SPI1,ENABLE);
} 

void SPI1_IRQHandler(){
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==SET){//���ջ��������գ�����������
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_RXNE);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==SET){//���ͻ�����Ϊ�գ����Է�����
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_TXE);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_OVR)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_OVR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_MODF)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_FLAG_MODF);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,SPI_FLAG_CRCERR)==SET){
		SPI_I2S_ClearFlag(SPI1,SPI_FLAG_CRCERR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,I2S_FLAG_UDR)==SET){
		SPI_I2S_ClearFlag(SPI1,I2S_FLAG_UDR);
	}
	if(SPI_I2S_GetFlagStatus(SPI1,I2S_FLAG_CHSIDE)==SET){
		SPI_I2S_ClearFlag(SPI1,I2S_FLAG_CHSIDE);
	}
}

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData){		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}


void SPI2_Init(void){
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
// //NVIC����
//  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
//	 //�ж�����
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE|SPI_I2S_IT_ERR, ENABLE);
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������			 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler){
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 
} 


/*
void SPI2_IRQHandler(){
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET){//���ջ��������գ�����������
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_RXNE);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==SET){//���ͻ�����Ϊ�գ����Է�����
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_TXE);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_OVR)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_OVR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_MODF)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_FLAG_MODF);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_CRCERR)==SET){
		SPI_I2S_ClearFlag(SPI2,SPI_FLAG_CRCERR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,I2S_FLAG_UDR)==SET){
		SPI_I2S_ClearFlag(SPI2,I2S_FLAG_UDR);
	}
	if(SPI_I2S_GetFlagStatus(SPI2,I2S_FLAG_CHSIDE)==SET){
		SPI_I2S_ClearFlag(SPI2,I2S_FLAG_CHSIDE);
	}
}
*/
