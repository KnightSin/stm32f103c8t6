#ifndef USER_I2C_H
#define USER_I2C_H
#ifdef __cplusplus
extern "C" {
#endif
/*@brief
IO����GPIO��AFIO���ܡ���׼��������дIO�ĳ���
��д���ģ��i2c��spi�����IO��ʹ��
**/

/*@brief
i2c����ʱһ������Ϊ1us�����ʱ���osʹ�õ�1msʱ��ҪС�Ķ�
ʹ��whileѭ��������ʱ����ʱ����ܾ�׼��
���ʹ�ö�ʱ�������������壬Ȼ��while��ѯ��
������Ȼϵͳ����ʧ1us�����ܣ����Ǳ�Ƶ�����жϴ���Ҫ�õĶ�

��������os������ȡ�������ʱ�ڼ�Ӧ�������жϣ�������ʱ�����
osʹ������͵ȼ����쳣���������Ƚ��̡��������ж��ﴦ��i2cҲ�ǿ��еģ�
���ή��ϵͳ����Ӧ�ٶ�
*/
#include "user_common.h"
#include "stm32f10x.h"	
	
#define HARDWAREIIC 1
#define SOFTWAREIIC 1
#define TRUE 1
#define FALSE 0
#if(SOFTWAREIIC ==1)
//************************************
/*ģ��IIC�˿�������붨��*/
#define SCL_PORT_H(PORT,PIN)		GPIO##PORT->BSRR = GPIO_Pin_##PIN
#define SCL_PORT_L(PORT,PIN)		GPIO##PORT->BRR = GPIO_Pin_##PIN
#define SDA_PORT_H(PORT,PIN)		GPIO##PORT->BSRR = GPIO_Pin_##PIN
#define SDA_PORT_L(PORT,PIN)		GPIO##PORT->BRR = GPIO_Pin_##PIN 
#define SCL_PORT_read(PORT,PIN)		GPIO##PORT->IDR  & GPIO_Pin_##PIN
#define SDA_PORT_read(PORT,PIN)		GPIO##PORT->IDR  & GPIO_Pin_##PIN

#define SCL_H         SCL_PORT_H(B,6)
#define SCL_L         SCL_PORT_L(B,6)
 
#define SDA_H         SDA_PORT_H(B,7)
#define SDA_L         SDA_PORT_L(B,7)

#define SCL_read      SCL_PORT_read(B,6)
#define SDA_read      SDA_PORT_read(B,7)
typedef int bool;
typedef struct{
	void(*init)(GPIO_TypeDef *SCL_GPIOx,uint16_t SCL_PIN,GPIO_TypeDef *SDA_GPIOx,uint16_t SDA_PIN);
	bool (*start)();
	void (*stop)();
	bool(*sendbyte)(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
	unsigned char (*recebyte)(unsigned char SlaveAddress,unsigned char REG_Address);
}Soft_I2c_t;
extern const Soft_I2c_t soft_iic;
#endif

#if(HARDWAREIIC ==1)

#endif
#ifdef __cplusplus
}
#endif
#endif