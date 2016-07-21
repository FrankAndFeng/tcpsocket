/*
 * tcpfunc.c,封装程序内相关的定义和函数
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "tcpfunc.h"

/* 创建客户端列表 */
client_list *clist_create(void)
{
    client_list *clist_head;
    clist_head = (client_list *)malloc(sizeof(struct _client_list));

    if (NULL == clist_head)
    {
            printf("create client list failed in memory allocate\n");
        }
    clist_head->pnode = NULL;

    return clist_head;
}

/* 在list尾部插入结点，
 * 输入参数：client_list *clist_head,头结点，
 *           client_list *list_new,待插入的结点
 * 返回值：int，插入成功返回0，否则返回错误代码，-1 */
int insertNodeTail(client_list *clist_head, client_list *clist_new)
{
    int ret = 0;
    if ((NULL == clist_head) || (NULL == clist_new))
    {
            return --ret;
        }

    client_list *cpt = clist_head;
    /* 到达最后一个非空结点 */
    while (((node *)cpt)->next)
    {
            cpt =(client_list *)(((node *)cpt)->next);
        }
    /* 将结点插入尾部 */
    if (cpt)
    {
         ((node *)cpt)->next = (node *)clist_new;
         ((node *)clist_new)->next = NULL;
         return ret;
    }
    else
    {
         return --ret;
    }
}

/* 按照sockfd删除结点
 * 输入参数：client_list *clist_head,头结点
 *           int sockfd，待删除结点的sockfd数据部分
 * 返回值：删除成功返回0，否则返回错误代码，-1表示输入有误，1表示未找到
 * 头结点保存socket server信息，不允许删除，所以从cptn开始删除*/
int delClient(client_list *clist_head, int sockfd)
{
    int ret = 0;
    if ((NULL == clist_head) || (sockfd <= 0))
    {
            return --ret;
        }

    client_list *cpt = clist_head;
    client_list *cptn = (client_list *)(((node *)clist_head)->next);

    while (cptn)
    {
            if (cptn->sock_fd == sockfd)
            {
                        break;
                    }
            cpt = cptn;
            cptn = (client_list *)(((node *)cptn)->next);
        }
    if (NULL == cptn)
    {
            return ++ret;
        }
    ((node *)cpt)->next = ((node *)cptn)->next;

    free(cptn);
    return ret;
}

/* 按sockfd信息查找客户端
 * 输入参数：client_list *clist_head,头结点
 *           int sockfd，待删除结点的sockfd信息
 * 返回值：查找到的client_list结点，若没有找到，返回NULL
 */
client_list *searchClient(client_list *clist_head, int sockfd)
{
    if ((NULL == clist_head) || (sockfd <= 0))
    {
            return NULL;
    }

    client_list *cpt = clist_head;

    while (cpt)
    {
        if (cpt->sock_fd == sockfd)
            {
                        break;
            }
        cpt = (client_list *)(((node *)cpt)->next);
     }
    if (NULL == cpt)
    {
        return NULL;
    }
    return cpt;
}

/* 打印所有客户端信息
 * 输入参数：client_list *clist_head客户端列表的头结点
 * 输出：终端打印*/
void printAllClient(client_list *clist_head)
{
    if (NULL == clist_head)
    {
            printf("empty clist_head\n");
        }
    client_list *cpt = clist_head;
    printf("this is socket server %d\n", cpt->sock_fd);
    cpt = (client_list *)(((node *)cpt)->next);
    while (cpt)
    {
          printf("this is client %d\n", cpt->sock_fd);
          cpt = (client_list *)(((node *)cpt)->next);
    }
}
/************终端交互操作函数**********/
/*主页信息显示,该主页是程序的主入口
 * 输入参数：void
 * 返回值：int，成功打印返回0，否则返回-1*/
int homePage(void)
{
    printf("这里是程序主页，请输入对应的数字来选择你想要的功能\n");
    printf("输入0，获取帮助信息\n");
    printf("输入1，显示当前在线的客户端信息\n");
    printf("输入2，向所有在线客户端广播消息\n");
    printf("输入3，选择一个客户端客户端，向它发生消息\n");
    printf("输入4，选择一个客户端，关闭和它之间的连接\n");
    printf("输入5，关闭当前服务器并退出\n");
    printf("--");
}

/* 打印帮助信息 */
int printhelp(void)
{
    printf("这里是帮助信息列表，尚未完善，敬请期待。。。\n");
    //printf("任意按钮，返回主页\n");

}

/* 打印服务器和客户端信息
 * 输入参数：client_list *head,客户列表的头结点
 * 返回值：打印成功返回0，否则返回-1*/
int printAll(client_list *head)
{
    int ret = 0;
    if (NULL == head)
        return --ret;

    printf("当前连接的客户端：\n");
    client_list *cpt = head;
    /* 打印服务器和客户端 */
    int sockfd;
    char *sin_addr;
    int port;
    int flag = 0;

    while (cpt)
    {
        sockfd = cpt->sock_fd;
        sin_addr = inet_ntoa(cpt->their_addr.sin_addr);
        port = ntohs(cpt->their_addr.sin_port);

        if (flag == 0)
        {
            printf("server with sockfd %d on %s:%d\n", sockfd, sin_addr, port);
            flag++;
        }
        else
        {
             printf("client with sockfd%d on %s:%d\n", sockfd, sin_addr, port);
        }
        cpt = (client_list *)(((node *)cpt)->next);
    }
    return ret;
}

/*向所有客户端广播消息  */
int broadcast(client_list *head, char *str_send, int len)
{
    int ret = 0;
    if ((NULL == head) || (NULL == str_send) || (len <= 0))
        return --ret;

    client_list *cpt = (client_list *)(((node *)head)->next);
    while (cpt)
    {
        if ((send(cpt->sock_fd, str_send, len, 0)) <= 0)
        {
            printf("send data to client %d failed\n", cpt->sock_fd);
            return --ret;
        }
        cpt = (client_list *)(((node *)cpt)->next);
    }
    memset(str_send, 0, len);

    return ret;
}


