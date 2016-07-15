#ifndef __LINK_H__
#define __LINK_H__

/* 最大链表长度 */
#define MAX_LIST_LEN 100

/* 纯链表结点 */
typedef struct _node
{
	struct _node *next;
}node;

/* 创建链表结点 */
node* createNode(void);

/* 插入结点 */
int insertNode(node *phead, int pos, node *node_new);
int insertNodeFront(node *phead, node *node_new);

/* 删除结点 */
int delNode(node* phead, node* node_del);

#endif
