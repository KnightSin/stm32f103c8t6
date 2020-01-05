// ��ģ������ GPIOB6->SCL GPIOB7->SDA      
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "user_i2c.h"
#define   uchar unsigned char
#define   uint unsigned int	

#if(SOFTWAREIIC ==1)

/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(GPIO_TypeDef *SCL_GPIOx,uint16_t SCL_PIN,GPIO_TypeDef *SDA_GPIOx,uint16_t SDA_PIN)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(SCL_GPIOx, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(SDA_GPIOx, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
		
   u8 i=5; //��������Ż��ٶ�	����������͵�5����д��
   while(i) 
   { 
     i--; 
   }  
}


/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool I2C_Start(void)
{
	SDA_H;
	SCL_H;				//ʱ�Ӻ�����������
	I2C_delay();  
	if(!SDA_read)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳� 
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	SDA_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	//���յ�8�ֽ����ݵ�һ����Ҫ����ack�ź�
	//���ͷ���Ҫ�ȴ�ack�ź�
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();//�������ӻ�����Ӧ���ź�
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	//���豸����noack���ô��豸�������ߣ��Ӷ��ر�ͨ��
	//Ȼ���ٴ�startͨ�š�������stop��startҪ��
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)//������δ������
	{
      SCL_L;
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
				SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_ReadByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_ReadByte(void)  //���ݴӸ�λ����λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}         
//���ֽ�д��*******************************************
//����������֮�������ʱ5ms���ң���ִ����������
bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address);   //���õ���ʼ��ַ      
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	
    I2C_SendByte(REG_data);
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_Stop(); 
    //delay5ms();
    return TRUE;
}

//���ֽڶ�ȡ*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){I2C_Stop();return FALSE;}
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);//�ӻ���ַΪʲô+1???��Ϊ���1λ��1���������豸�����豸
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}

		REG_data= I2C_ReadByte();
    I2C_NoAck();//�ر�ͨ��
    I2C_Stop();
    //return TRUE;
	return REG_data;
}						      


const Soft_I2c_t soft_iic = {
	I2C_GPIO_Config,
	Single_Write,
	Single_Read,
};
#endif

#if(HARDWAREIIC == 1)
#include "stm32f10x_i2c.h"
void I2C_GPIO_Config_Hard(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void I2C1_Init(){
	I2C_InitTypeDef I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_DeInit(I2C1);
	I2C_GPIO_Config_Hard();
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x77;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_Init(I2C1, &I2C_InitStructure);
	//NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		//I2C�ж�����
	I2C_ITConfig(I2C1,I2C_IT_BUF|I2C_IT_BUF|I2C_IT_ERR, ENABLE);
	I2C_Cmd(I2C1, ENABLE);
}

//�¼��ж�
void I2C1_EV_IRQHandler(){
	if(I2C_GetITStatus(I2C1,I2C_IT_RXNE) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_RXNE);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_TXE) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_TXE);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_SB) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_SB);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_ADDR) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_ADDR);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_BTF) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_BTF);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_STOPF) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_STOPF);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_ADD10) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_ADD10);
	}
}
//�����ж�
void I2C1_ER_IRQHandler(){
	if(I2C_GetITStatus(I2C1,I2C_IT_SMBALERT) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_SMBALERT);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_TIMEOUT) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_TIMEOUT);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_PECERR) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_PECERR);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_OVR) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_OVR);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_AF) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_AF);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_ARLO) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_ARLO);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_BERR) == SET){
		I2C_ClearFlag(I2C1,I2C_FLAG_BERR);
	}
}


void I2C1_Write(u8 deviceaddr,u8 addr, u8 data){
	I2C_AcknowledgeConfig(I2C1,ENABLE); 
	I2C_GenerateSTART(I2C1,ENABLE); 
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;}//EV5
	I2C_Send7bitAddress(I2C1,deviceaddr,I2C_Direction_Transmitter); 
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;} //EV6
	I2C_SendData(I2C1,addr); 
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //EV8
	I2C_SendData(I2C1,data);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} 
	I2C_GenerateSTOP(I2C1,ENABLE); 
}
 
u8 I2C1_Read(u8 deviceaddr,u8 nAddr){
	I2C_AcknowledgeConfig(I2C1,ENABLE); //
	I2C_GenerateSTART(I2C1,ENABLE); //
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //EV5
	I2C_Send7bitAddress(I2C1,deviceaddr,I2C_Direction_Transmitter); //
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//EV6
	I2C_SendData(I2C1,nAddr);//
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //EV8
	 //������Ҫ���������ڲ���ĳ����ַ����ʼ�����ڲ���ĳ����ֵַ
	I2C_GenerateSTART(I2C1,ENABLE); //
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //EV5
	I2C_Send7bitAddress(I2C1,deviceaddr,I2C_Direction_Receiver); //
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){;} //EV6
	I2C_AcknowledgeConfig(I2C1,DISABLE); //
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){;} //EV7
	I2C_GenerateSTOP(I2C1,ENABLE); //
	return I2C_ReceiveData(I2C1); //
}

const Hard_I2c_t hard_iic1 ={
	I2C1_Init,
	I2C1_Write,
	I2C1_Read,
};


#endif