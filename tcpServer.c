#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>

/* 服务器要监听的本地端口号 */
#define LOCPORT 8848

/* 能够同时接受多少没有accept的连接 */
#define BACKLOG 10

main()
{
	/* 在sock_fd上进行监听，在new_fd上接受新的连接 */
	int sock_fd,new_fd;
	/* 本地的地址信息 */
	struct sockaddr_in my_addr;

	/* 连接者的地址信息 */

}
