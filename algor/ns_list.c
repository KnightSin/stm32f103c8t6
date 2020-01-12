#include "ns_list.h"
/********��̬����**************/
#if (STATIC_LIST == 1)
/***********��̬��˽�б���********************/


/***********��̬��ȫ�ֱ���********************/
//��̬����� ����list_len��Ԫ�صı�L��indexλ�ã�����x��index��0��ʼ
status generic_static_list_insert(generic_static_list *L,int *list_len,int index,element x) {
	element *ptr;
	int i;
	//�ƶ�index�Լ���������ݣ��ճ�indexλ��,�����ʼ�ƶ�
	if (*list_len >= MAX_LIST_LEN)return -1;//���ˣ������Բ���
	if (index<0 || index>*list_len)return -2;//����λ�÷Ƿ�
	ptr = (element *)L;//ǿתָ������
	for (i = *list_len; i >index; i--) {
		ptr[i] = ptr[i-1];
	}
	//����
	ptr[index] = x;
	(*list_len)++;
	return 0;
}
//��̬��ɾ�� ����list_len��Ԫ�صı�L��indexλ��ɾ����index��0��ʼ
status generic_static_list_delete(generic_static_list *L, int *list_len, int index) {
	int i;
	element *ptr;
	if (index<0 || index>(*list_len - 1))return -1;//ɾ��λ�ô���
	ptr = (element *)L;//ǿתָ������
	for (i = index+1; i < *list_len; i++) {//��ɾ�����һλ��ʼ������Ųһλ,����ɾ��β��������
		ptr[i-1] = ptr[i];
	}
	(*list_len)--;//�������ȼ�����
	return 0;
}


#endif
/********��������**************/
#if ( DYNAMIC_LIST == 1)
/***********������˽�б���********************/


/***********������ȫ�ֱ���********************/
status generic_dynamic_single_list_insert(generic_dynamic_single_list *L, generic_dynamic_single_list *node) {
	L->next = node;
	node->next = NULL;//β�巨
	return 0;
}
//L��ͷ���,ɾ�������е�һ��ֵ��e�Ľ��
status generic_dynamic_single_list_delete(generic_dynamic_single_list *L,element e) {
	generic_dynamic_single_list *pre_node = L;
	generic_dynamic_single_list *cur_node = L->next;
	for (; cur_node != NULL; pre_node=cur_node,cur_node = cur_node->next) {
		if (cur_node->value == e) {
			pre_node->next = cur_node->next;
			free(cur_node);
			return 0;
		}
	}
	return -1;
}
#endif

