#include "core/task.h"
#include "core/list.h"
#include "core/mem.h"
tasklist wait_task,block_task,extinct_task;
TASKHANDLE cur_task;
TASKHANDLE idle_task;
 void *OSSTACKPOINT;
#define slot	1 //ʱ��Ƭ
#define TIMESLICE 1
//append ���ᴴ��listnode
static err_t task_append(tasklist dest,TASKHANDLE t){
	 spnode  p=NULL;
	 if(dest==NULL||t==NULL)return err_failed;
	 p=contain_of(slist,ctx,t);
	 p->ctx=t;
	 return list_add(dest,p);
}
static err_t task_add(tasklist dest,TASKHANDLE t){
	 spnode  p=(spnode)ONEMALLOC(sizeof(slist));
	 if(p==NULL||dest==NULL||t==NULL)return err_failed;
	 p->ctx=t;
	 return list_add(dest,p);
}
//remove but not delete
static err_t task_remove(tasklist src,TASKHANDLE t){
	spnode  p=NULL;
	err_t  err;
	if(t==NULL||src==NULL)return err_failed;
	p=contain_of(slist,ctx,t);
	if(p==NULL)return err_failed;
	err=list_remove(src,p);
	return err;
}
static err_t task_change(tasklist src,tasklist dest,TASKHANDLE t){
	spnode  p=NULL;
	err_t  err;
	if(t==NULL||src==NULL||dest==NULL)return err_failed;
	if(src==dest)return err_ok;
	p=contain_of(slist,ctx,t);
	if(p==NULL)return err_failed;
	err=list_remove(src,p);
	if(err!=err_ok)return err_failed;
	err=list_add(dest,p);
	if(err!=err_ok)return err_failed;
	return err_ok;
}
static err_t task_find(tasklist src,TASKHANDLE t){
	spnode  p=NULL;
	if(t==NULL||src==NULL)return err_failed;
	p=contain_of(slist,ctx,t);
	if(p==NULL)return err_failed;
	if(p==list_find(src,p))
		return err_ok;
	else return err_failed; 
}
//����ָ��tasklist�����ȼ���ߵ�task,������NULL
static TASKHANDLE task_max_priorty(tasklist src){
   spnode  pmax, p;
   uchar  max_priorty;
   pmax=p=src;
   if(p==NULL)return NULL;
   else{
     pmax=p=p->back;
	 max_priorty=((TASKHANDLE)(p->ctx))->priorty;
   }
   while(p!=src){
     if(max_priorty<((TASKHANDLE)(p->ctx))->priorty){
	   pmax=p;
	   max_priorty=((TASKHANDLE)(p->ctx))->priorty;
	 }
	 p=p->back;
   }
   if(p==src)return pmax->ctx;
   else return NULL;
}

//���Ȳ��Ե�ʵ�֣����Ȼ��ƺ���ϵ�ṹ��أ������Ƴ��ں��ʵ�ʱ�������
//���Ը����㷨����wait_task���Һ��ʵ�task,����Ϊcur_task �κ�ʱ����һ��idle task������
err_t schedule(){
   TASKHANDLE  task;
   err_t  err=err_ok;
#if(TIMESLICE==1)
   critical_area_enter();
   err=task_append(wait_task,cur_task);
   if(err!=err_ok)return err; //������
   cur_task->status=WAIT;
   task=wait_task->pre->ctx;	//add��ͷ�壬����Ӧ��βȡ
   err = task_remove(wait_task,task);
   if(err!=err_ok)return err; //������
   task->status=RUN;
   cur_task=task;
   critical_area_exit();
   return err;
#else
   critical_area_enter();
   err=task_append(wait_task,cur_task);
   if(err!=err_ok)return err; //������
   cur_task->status=WAIT;
   task=task_max_priorty(wait_task);
   if(task==NULL)return err_failed;//������
   err = task_remove(wait_task,task);
   if(err!=err_ok)return err; //������
   task->status=RUN;
   cur_task=task;
   critical_area_exit();
   return err;
#endif		
}
TASKHANDLE task_create(uchar task_id,uchar priorty,uchar stack_size,void(*task_fun)(void)){
	TASKHANDLE  t = (TASKHANDLE)ONEMALLOC(sizeof(TASK));
	if(t==NULL)return NULL;
	t->task_id=(task_id<0?0:(task_id>255?255:task_id));		//range 0~255
	t->priorty=(priorty<0?0:(priorty>255?255:priorty));	  //range 0~255
	t->stack_size = (stack_size<=16?16:(stack_size>255?16:stack_size)); //range 16~255 out off set to 16
	if(task_fun==NULL){t->task_fun=NULL;return NULL;}
	t->task_fun=task_fun;
	t->stack_addr=ONEMALLOC(stack_size);	//����taskջram
	if(t->stack_addr==NULL)return NULL;
	t->stack_p=(void *)t->stack_addr;
	arch_initTask_stack(t->stack_p,task_fun);
	t->status=WAIT;
	if(task_add(wait_task,t)!=err_ok)return NULL;
	return t;
}

err_t task_suspend(TASKHANDLE task){
   if(task==NULL)return err_failed;
   if(task==cur_task){
     if(schedule()!=err_ok)return err_failed; 
     return task_change(wait_task,block_task,task);
   }
   if(task_find(wait_task,task)==err_ok)
     return task_change(wait_task,block_task,task);
   else return err_failed;
}
err_t task_resume(TASKHANDLE task){
   if(task==NULL)return err_failed;
   if(task==cur_task)
     return err_ok;
   if(task_find(wait_task,task)==err_ok)
     return err_ok;
   if(task_find(block_task,task)==err_ok)
   	 return task_change(block_task,wait_task,task);
   else return err_failed;
}

err_t task_delete(TASKHANDLE task){
   if(task==NULL)return err_failed;
   if(task==cur_task){
     if(schedule()!=err_ok)return err_failed;
     return task_change(wait_task,extinct_task,task);
   }
   if(task_find(wait_task,task)==err_ok)
     return task_change(wait_task,extinct_task,task);
   if(task_find(block_task,task)==err_ok)
   	 return task_change(block_task,extinct_task,task);
   else return err_failed;
}
err_t task_clean(){
	err_t  err=err_ok;
	tasklist p=extinct_task;
    if(extinct_task==NULL)
	  return err_failed;
	else p=p->back;
	while(p!=extinct_task){
	  err=list_delete(extinct_task,&p);
	  if(err!=err_ok)return err;
	}
	return err;
}


void idle_fun(){
  for(;;){
  	task_clean();
  }
}
err_t oneos_init(){
	wait_task = list_create();
	block_task = list_create();
	extinct_task = list_create();
	if(wait_task==NULL||block_task==NULL||extinct_task==NULL)return err_failed;
	idle_task=task_create(0,0,32,idle_fun);
	cur_task=idle_task;
	return err_ok;	  
}
void oneos_start(){
   arch_oneos_start();
}