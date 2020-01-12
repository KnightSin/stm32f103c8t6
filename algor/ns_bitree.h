#ifndef NS_BITREE_H
#define NS_BITREE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ns_algorithm.h"

#define ShowFun(fmt,...) printf(fmt,__VA_ARGS__);
#define BI_TREE   0
#define THR_TREE	0
#define AVL_TREE	0
#define HUFFMAN_TREE	0

typedef struct TreeNode {
	char ltag;
	struct TreeNode *lchild;
	element data;
	struct TreeNode *rchild;
	char rtag;
	int node_cost;//��������Ҫʹ�õĽ�����ֵ
	int balance_fact;//avl��ƽ������
}TreeNode, *Tree;//����������������������һ�������

typedef int(*visit_t)(TreeNode* node, TreeNode* OutNode, void* args);
#ifdef __cplusplus
}
#endif
#endif