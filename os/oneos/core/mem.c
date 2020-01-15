#include "arch/arm-cortex-m3/arch.h"
#include "core/mem.h"
#include "core/list.h"
#define USED 		1
#define UNUSED  0
static  uchar mem_heap[MEM_SIZE];
#if (BIT_MAP==1)
static  uchar bitmap[BITMAP_SIZE];
/*offset�ǻ��ڶѻ���ַ���ֽ�ƫ�ƣ�len���ֽ�����flag��used��unused*/
static void set_flag(uint offset,uint len,bool flag){
  uchar i=0;
	if(offset%MIN_ALLOC_BYTE!=0)assert(0);
  len=len/MIN_ALLOC_BYTE+(len%MIN_ALLOC_BYTE!=0?1:0);
  i= (offset/MIN_ALLOC_BYTE)%8;
  offset/=(MIN_ALLOC_BYTE*8);
  for(;len>0;len--,i++){
  	if(i==8){
	  i=0;
	  offset++;
	}
	if(flag)
    	bitmap[offset]|=(0x01<<i);
	else 
		bitmap[offset]|=(0x00<<i);
  }
}

//��ʼƫ���ֽڣ�used����unused�������ҵ���offset��len
static void get_block(uint start_offset,uint *offset,uint *len,bool flag){
   uchar  i=0, j=0;
   uint  size=0;
   i=start_offset/(8*MIN_ALLOC_BYTE);
   j=start_offset%(8*MIN_ALLOC_BYTE);
   while(flag^(bitmap[i]&(0x1<<j))){
	 j++;
	 if(j==8){
	   j=0;
	   i++;
	 }
	 if(i==BITMAP_SIZE){*offset=0;*len=0;return;}
   }
   *offset=(i*8+j)*MIN_ALLOC_BYTE;
   while(!(flag^(bitmap[i]&(0x1<<j)))){
			j++;
			size++;
			if(j==8){
				j=0;
				i++;
			}
			if(size*2>=(*len)){*len=size*2;return;}
   }
}
void *one_malloc(uint size){
	uint  start=0, offset, len;
	len=size;
	for(;;){
	 get_block(start,&offset,&len,UNUSED);
	 if(size<=len)break;
	 start+=(offset+len);
	}
	set_flag(offset,size,USED);
 	return (void *)(mem_heap+offset);
}

void one_free_size(void *addr,uint size){
   set_flag((uint)((uint)addr-(uint)mem_heap),size,UNUSED);
}
#endif
#if (LINK_MAP==1)
//ʹ�������������ڴ棬8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128 
//ÿ�����϶���˫��ѭ������ block
//����128 byte�ķŵ������������У�ÿ��chunk��������
#endif
void one_memset(void *addr,uint value,uint size){
	if(addr==NULL)return;
	for(;size>0;size--){
	  *(((uchar*)addr)+size-1)=value;
	}
}

void one_memcpy(void *dest,void *src,uint size){
   if(dest==NULL||src==NULL)return;
   for(;size>0;size--){
	 *(((uchar*)dest)+size-1)=*(((uchar*)src)+size-1);
   }
}