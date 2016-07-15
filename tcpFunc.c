#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 10

typedef struct _node
{
	struct _node *next;
}node;

typedef struct _cinfo_list
{
	node* cnode;
	int value;
}cinfo_list;

cinfo_list *createCinfo_list(void)
{
	cinfo_list *list1 = (cinfo_list *)malloc(sizeof(struct _cinfo_list));
	if (NULL == list1)
	{
		printf("create cinfo_list failed:memory allocate failed");
	}
	return list1;
}

cinfo_list *list_head;

int main()
{
	list_head = createCinfo_list();
	list_head->value = 99;
	int i = 0, flags = 0;
	for (;i < MAX_LEN; i++)
	{
		cinfo_list *list_new = createCinfo_list();
		list_new->value = i;
		((node *)list_new)->next = ((node *)list_head)->next;
		((node *)list_head)->next = (node *)list_new;
//		flags = insertNodeFront((node *)list_head, (node *)node_new);
		printf("insert %d succedd\n", i);
	}
	//node *ptr = ((node *)list_head)->next;
	cinfo_list *plist = list_head;
	while((node *)plist)
	{
		printf("the value in list: %d\n",plist->value);
		plist =(cinfo_list *)(((node *)plist)->next);
	}
}
