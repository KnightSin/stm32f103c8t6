#include "core/list.h"
#include "core/mem.h"

splist list_create(){
	splist  p;
    p = (splist)ONEMALLOC(sizeof(slist));
	if(p==NULL)return p;
	p->ctx=NULL;
	p->pre=p->back=p;
	return p;
}
//�ҵ�����node���Ҳ�������NULL
spnode list_find(splist sp,spnode node){
   spnode  p=sp;
   if(p==node)return p;//��ͷ���
   else p=p->back;
   while(p!=sp){
	 if(p==node)return p;
	 p=p->back;
   }
   return NULL;
}
//remove but not delete
err_t list_remove(splist sp,spnode node){
	spnode  p, ppre;
	if(sp==NULL||node==NULL)return err_failed;
	p=list_find(sp,node);
	if(p==NULL)return err_failed;
	if(p==sp){p->pre=p->back=p;return err_failed;}
	ppre=p->pre;
	ppre->back=p->back;
	p->back->pre=ppre;
	p->pre=p->back=NULL;
	return err_ok;
}

//remove and delete
err_t list_delete(splist sp,spnode *node){
	err_t  err;
	if(sp==NULL||(*node)==NULL)return err_failed;
	err=list_remove(sp,*node);
	if(err==err_ok){ONEFREE(*node,sizeof(slist));(*node)==NULL;}	 //>>>>>
	else return err;
	return err_ok;
}
err_t list_add(splist sp,spnode node){
	if(sp==NULL||node==NULL)return err_failed;
	if(list_find(sp,node)!=NULL)return err_failed;
	if(sp->back==sp&&sp->pre==sp){	//��һ�������Ԫ��
		sp->back=node;
		sp->pre=node;
		node->pre=sp;
		node->back=sp;
		return err_ok;
	}
	//ͷ�巨
	sp->back->pre=node;
	node->back=sp->back;
	node->pre=sp;
	sp->back=node;
	return err_ok;
}