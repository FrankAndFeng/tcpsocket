#include <stdlib.h>
#include <assert.h>

#include "node.h"

/* 创建链表 */
node* create(void)
{
	node* phead;
	phead = (node *)malloc(sizeof(struct _node));
	if (NULL == phead)
	{
		printf("create node failed: memory allocate failed!\n");
		//exit(-1);
	}
	phead -> next = NULL;
	return phead;
}

/* 插入结点
 * 输入参数：node* phead,头结点，
 *			 int pos,插入位置
 *			 node* node_new,插入的结点
 * 返回值：int,插入成功返回0，插入失败返回对应错误代码，-1*/
int insertNode(node *phead, int pos, node *node_new)
{
	int RET = 0;
	assert(phead);
	node *pt = phead;

	if (pos <= 0 || NULL == node_new)
	{
		return --RET;
	}

	if (pt)
	{
		pt = pt->next;
		while (pos-- && pt)
		{
			pt = pt->next;
		}
		/*if(pos >lenoflist),return 1*/
		if (NULL == pt)
		{
			return --RET;
		}
		node_new->next = pt->next;
		pt->next = node_new;
	}
	return RET;
}
/*在头结点之后，插入结点
 * 输入参数：node *phead,头结点
 *			 node *node_new,新插入结点
 * 返回值:int,插入成功返回1，失败返回0*/
int insertNodeFront(node *phead, node *node_new)
{
	int RET = 0;
	assert(phead);
	if (NULL == node_new)
	{
		return --RET;
	}
	node *pt = phead;
	if (pt)
	{
		node_new->next = pt->next;
		pt->next = node_new;
	}
	return RET;
}

/* 删除结点
 * 输入参数：node *phead,头结点，
 *			 node *node_del，要删除的结点
 * 返回值：删除成功返回0，失败返回-1*/
int delNode(node *phead, node *node_del)
{
	int RET = 0;
	assert(phead);
	if (NULL == node_del)
	{
		return --RET;
	}
	node *pt = phead;
	node *ptnext = phead->next;

	while(ptnext)
	{
		if (ptnext == node_del)
		{
            break;
		}
        pt = ptnext;
        ptnext = ptnext->next;
	}
    if (ptnext == NULL)
    {
        return --RET;
    }
    pt->next = ptnext->next;
    return RET;
}

