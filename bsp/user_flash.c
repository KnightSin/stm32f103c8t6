#include "user_flash.h"
#include "stm32f10x_flash.h"
/*��Ϊ������ֲ��fatfs��Ҫdiskio��֧�֣�����������Ҫ��flash�����������棬ʵ����Щ
diskio�ĺ������������ܽ�һ����ֲfatfs*/
//ʵ�����¼����������rtc��ȡʱ�亯��������ʹ��fatfs��
/********��ʼ��flash�豸************/
//�����ӳ����ڣ�72MHZ��Ƶ���ӳ�ʱ������Ϊ2����ʾcpu����\
flash����ָ���2���ں��ٷ���flash��״̬
DSTATUS flash_initialize(BYTE pdrv){
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);//Ҫ�رհ����ڷ���
	return RES_OK;
}

DSTATUS flash_status(BYTE pdrv){
	//��ʼ�����ɹ�
	if(flash_initialize(0))
		return STA_NOINIT;
	//��ʼ���ɹ������д����
	//if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
	//	return STA_PROTECT;
	//else 
		return RES_OK;
}

DRESULT flash_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count){
	UINT addr = FLASH_START_ADDR;
	int i = 0;
	//��μ��
	if(!buff || sector+count >= FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//�豸���
	if(flash_initialize(0))
		return RES_NOTRDY;
	//ͨ��ת����ֱַ�Ӷ�
	sector+=USER_START_SECTOR;//��������ƫ��
	addr += FLASH_SECTOR_SIZE*sector;
	count *= FLASH_SECTOR_SIZE;
	for(;i<count;i++){
		buff[i] = *(__IO BYTE *)addr;
		addr++;
	}
	return RES_OK;
}

#if (USE_FAT_FS ==0)
DRESULT flash_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	//stm32֧�ְ��ֺ�ȫ��д��
	UINT data = 0x0;
	UINT addr = FLASH_START_ADDR;
	UINT i =0;
	//��μ��
	if(!buff || sector+count > FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//�豸���
	if(flash_initialize(0))
		return RES_NOTRDY;
	//д�������
	if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
		return RES_WRPRT;
	//��ַת��
	sector+=USER_START_SECTOR;//��������ƫ��
	addr += FLASH_SECTOR_SIZE*sector;
	//����
	FLASH_Unlock();
	//��������ָ������
	for(;i<count;i++){
		if(FLASH_COMPLETE != FLASH_ErasePage(addr+i*FLASH_SECTOR_SIZE)){
			//assert_param(0);
			return RES_ERROR;
		}
	}
	//���ô�д���ֽ�����
	count *= FLASH_SECTOR_SIZE;
	//����д������
	for(i=0;i<count;i+=4,addr += 4){
		data = buff[i] | (buff[i+1]<<8) | (buff[i+2]<<16) | (buff[i+3]<<24); //С�˴洢���͵�ַ������ֽ�
		if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data)){
			//assert_param(0);
			return RES_ERROR;
		}
	}
	//����
	FLASH_Lock();
	return RES_OK;
}

#elif USE_FAT_FS ==1
static BYTE swap_buff[FLASH_SECTOR_SIZE*2];//��������  ����ŵ������ڲ��������bug������ԭ�������
/*
reason:flash_writeһ���Ǳ�task���õĺ�������task�ĺ����������ϣ�ÿ���ֲ�����
����ռ��task��ջ�ռ䡣��task��ջ�ռ�һ�㶼������С��1k����˴�����ľֲ�����
������ջ����������ⲿ���룬������ȫ�����ľ�̬��������ȫ������ȫ�ֱ������Ͷ�ջ�޹ء�
cortex-m3��msp��psp����ջָ�롣��ĳ�����üĴ���������ʹ���ĸ�ջָ����Ϊsp��
һ��osʹ��msp��Ϊos��ջָ�룬taskʹ��psp��Ϊtask��ջָ�롣
*/
DRESULT flash_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count){
	//stm32֧�ְ��ֺ�ȫ��д��
	UINT data = 0x0;
	UINT addr = FLASH_START_ADDR;
	UINT i =0,j=0;
	UINT front = 0,back=0;//0������룬1��������
	//��μ��
	if(!buff || sector+count >= FLASH_SECTOR_NUMBER)
		return RES_PARERR;
	//�豸���
	if(flash_initialize(0))
		return RES_NOTRDY;
	//д�������
	//if((FLASH_GetWriteProtectionOptionByte()&FLASH_WRProt_Pages62to127) != FLASH_WRProt_Pages62to127)
		//return RES_WRPRT;
	//��ַת��
	sector+=USER_START_SECTOR;//��������ƫ��
	addr += FLASH_SECTOR_SIZE*sector;
	front = (sector&0x1)?1:0;
	back = (sector+count-1)&0x1?0:1;
	//����
	FLASH_Unlock();
	//����ǰ�����
	if(front){
		//attention������������������\
		����������߼���������read��write�����ڴ���Ĳ�������δƫ�Ƶ�sector���˴���sector�Ѿ������ƫ�ơ�\
	֮ǰ�����bug�������޸Ĺ��������Ҽ�¼
		flash_read(pdrv,swap_buff,sector-USER_START_SECTOR-1,1);
	}
	if(back){
		flash_read(pdrv,swap_buff+FLASH_SECTOR_SIZE,sector-USER_START_SECTOR+count,1);
	}
	//������������
	for(;i<=count;i+=2){//==��ǰ�����������������Ϊǰ������ʱ�������˲�����ַ�����º󲻶����ɶ���
		if(FLASH_COMPLETE != FLASH_ErasePage(addr+i*FLASH_SECTOR_SIZE-
																																(front?FLASH_SECTOR_SIZE:0)))
			return RES_ERROR;
	}
	//���ô�д���ֽ�����
	count *= FLASH_SECTOR_SIZE;
	//�ض�λд���ַ
	addr-=front*FLASH_SECTOR_SIZE;
	if(front){//����ǰ����
		for(i=0;i<FLASH_SECTOR_SIZE;i+=4,addr+=4){
			data = swap_buff[i] | (swap_buff[i+1]<<8) | (swap_buff[i+2]<<16) | (swap_buff[i+3]<<24); //С�˴洢���͵�ַ������ֽ�
			if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
		}
	}
	//����д������
	for(i=0;i<count;i+=4,addr += 4){
		data = buff[i] | (buff[i+1]<<8) | (buff[i+2]<<16) | (buff[i+3]<<24); //С�˴洢���͵�ַ������ֽ�
		if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
	}
	if(back){//���غ����
		for(i=FLASH_SECTOR_SIZE;i<2*FLASH_SECTOR_SIZE;i+=4,addr+=4){
			data = swap_buff[i] | (swap_buff[i+1]<<8) | (swap_buff[i+2]<<16) | (swap_buff[i+3]<<24); //С�˴洢���͵�ַ������ֽ�
			if(FLASH_COMPLETE != FLASH_ProgramWord(addr,data))return RES_ERROR;
		}
	}
	//����
	FLASH_Lock();
	return RES_OK;
}




#endif
DRESULT flash_ioctl(BYTE pdrv, BYTE cmd, void* buff){
	//�豸���
	if(flash_initialize(0))
		return RES_NOTRDY;
	if(cmd == CTRL_SYNC){
	  return RES_OK;
	}
	else if(cmd == GET_SECTOR_COUNT){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_SECTOR_NUMBER;
	}
	else if(cmd == GET_SECTOR_SIZE){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_SECTOR_SIZE;
	}
	else if(cmd == GET_BLOCK_SIZE){
		if(!buff)return RES_ERROR;
		*(UINT*)buff = FLASH_BLOCK_SIZE;
	}
	else if(cmd == CTRL_TRIM){//֪ͨĳ����Χ���߼����鲻����Ҫ,���Բ���
		 return RES_OK;
	}
	else
		return RES_PARERR;
	return RES_OK;
}

/*@brief:����flash��С����λ��KB
*/
unsigned short flash_size(void){
	return ((*((unsigned short *)0x1FFFF7E0))&0xFFFF);
}
const struct flash_driver flash_driver={
	flash_initialize,
	flash_status,
	flash_read,
	flash_write,
	flash_ioctl,
};