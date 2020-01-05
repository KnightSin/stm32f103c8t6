#include "ns_eeprom.h"
#include "ns_w25qxx.h"
const u8 TEXT_Buffer[]={"iic&eeprom test:Hello World!"};
const u8 FLASH_Buffer[]={"spi&flash test:Hello World!"};
#define SIZE sizeof(TEXT_Buffer)	
#define FLASH_SIZE sizeof(FLASH_Buffer)	
u8 datatemp[SIZE];	
void eeprom_test(){
	AT24CXX_Init();
	if(AT24CXX_Check()){
		xprintf_s("check eeprom failed\r\n");
	}
	AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
	AT24CXX_Read(0,datatemp,SIZE);
	xprintf_s("%s\r\n",datatemp);
}
void w25qxx_test(){
	u32 flash_size = 128*1024*1024;	//FLASH ��СΪ16M�ֽ�
	W25QXX_Init();			//W25QXX��ʼ��
	if(W25QXX_ReadID()!=W25Q128){
		xprintf_s("check flash failed\r\n");
	}
	W25QXX_Write((u8*)FLASH_Buffer,flash_size-100,FLASH_SIZE);			//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
	W25QXX_Read(datatemp,flash_size-100,FLASH_SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
	xprintf_s("%s\r\n",datatemp);
}