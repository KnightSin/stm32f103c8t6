#ifndef ARCH_H
#define ARCH_H
#ifdef __cplusplus
extern "C"{
#endif
#define DEBUG

#ifdef C51
#define DATA data
#define  
#define  
#define XDATA xdata
#define CODE code

#else
#define DATA 
#define  
#define  
#define XDATA 
#define CODE 
#endif

#ifndef NULL
#define NULL		0x00
#endif

#define TRUE		1
#define FALSE		0
#ifndef ONEMALLOC(size)

#define ONEMALLOC(size)	one_malloc(size)
#endif
#ifndef ONEFREE(addr)

#define ONEFREE(addr,size)	one_free(addr,size)
#endif

#if defined(DEBUG)
#define DEBUG_LOG(char,int)		printf("assert failed:%s,%d",char,int)	
#define assert(expr)			if((expr)==0) DEBUG_LOG(__FILE__,__LINE__)
#endif
typedef enum{
	err_ok=0,
	err_failed=1,
}err_t;


typedef unsigned char  uchar;   
typedef unsigned int   uint; 
typedef unsigned long  ulong;
typedef bit bool;
typedef unsigned int size_t;
#define offset(TYPE,MEMBER)	((size_t)&(((TYPE *)0)->MEMBER))
//�����ṹ���ͣ���Ա���ƣ���Ա��ַ����ṹ����ʼ��ַ
#define contain_of(TYPE,MEMBER,MEMBER_PTR) ((TYPE *)(MEMBER_PTR-offset(TYPE,MEMBER)))
typedef uchar sem_t;//�������ź���
#define sem_init(sem_name,value)		sem_name = value
#define sem_wait(sem_name)				while(!sem_name);	\
										sem_name-=1	
#define sem_post(sem_name)				sem_name+=1

typedef bool mutex_t; //������
#define mutex_init(mutex_name,value)	mutex_name = value
#define mutex_lock(mutex_name)			while(!mutex_name);	\
										mutex_name=0	
#define mutex_unlock(mutex_name)		mutex_name=1
//�ٽ��� �ر�ȫ���ж�

#define critical_area_enter()				(EA=0)
#define critical_area_exit()				(EA=1)

#define ban_schedule()						(TR0=0)
#define allow_schedule()					(TR0=1)
void context_push();
void context_pop();

#ifdef __cplusplus
}
#endif
#endif