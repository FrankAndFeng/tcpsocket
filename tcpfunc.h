#ifndef __LINK_H__
#define __LINK_H__

/* 终端输入可以接受字符串的最大长度 */
#define MAX_BUF_SIZE 256

/* 链表结点 */
typedef struct _node
{
    struct _node *next;
}node;

/* 客户端列表 */
typedef struct _client_list
{
    node *pnode;
    int sock_fd;
    struct sockaddr_in their_addr;
}client_list;
/***********链表操作相关函数*************/
/* 创建客户端列表 */
client_list *clist_create(void);

/* 在尾部列表插入客户端结点 */
int insertNodeTail(client_list *clist_head, client_list *clist_new);

/* 根据sockfd删除结点 */
int delNode(client_list *clist_head, int sockfd);

/* 根据sock查找客户端 */
client_list *searchClient(client_list *clist_head, int sockfd);

/* 打印所有客户端sockfd */
void printAllClient(client_list *clist_head);

/************终端交互操作函数**********/
/* 主页信息显示 */
int homePage(void);

/* 打印帮助信息 */
int printhelp(void);
/* 打印服务器和终端信息 */
int printAll(client_list *head);

/* 向所有终端广播消息 */
int broadcast(client_list *head, char *str_send, int len);

/* 向指定终端发送消息 */
int sendToClient(client_list *head, client_list *clist_target, char *str, int len);

#endif
