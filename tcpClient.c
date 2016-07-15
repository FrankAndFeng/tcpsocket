#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netdb.h>

#include<pthread.h>

/* 服务器程序监听的端口号 */
#define PORT 8848

/* 一次所能够接收的最大字节数 */
#define MAX_BUF_SIZE 256

/* 处理终端交互的线程和入口函数 */
pthread_t pthd_io;
void *ioHandler(void *argc);

int main(int argc, char** argv)
{
	int sockfd, numbytes;
	char buf[MAX_BUF_SIZE];
	struct hostent *he;

	/* 客户端的主机信息 */
	struct sockaddr_in their_addr;

	/* 检查参数信息 */
	if (argc != 2)
	{
		fprintf(stderr, "usage: client hostname\n");
		exit(1);
	}
	
	/* 获取主机信息 */
	if ((he = gethostbyname(argv[1])) == NULL)
	{
		herror("gethostbyname");
		exit(1);
	}

	/* 获取套接字描述符 */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	/* 主机字节顺序，定义addrss地址族 */
	their_addr.sin_family = AF_INET;

	/* 将本地端口号转换为网络短整型字节顺序 */
	their_addr.sin_port = htons(PORT);
	their_addr.sin_addr = *((struct in_addr*)he->h_addr);

	/* 余下清零 */
	bzero(&(their_addr.sin_zero), 8);

	/* 连接服务器 */
	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		pthread_create(&pthd_io, NULL, ioHandler, NULL);
	}
	/* 一直接受服务器数据，默认不关心数据来源 */
	while(1)
	{
		if ((numbytes = recv(sockfd, buf, MAX_BUF_SIZE, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}
		/* 此处添加异常处理，如果捕捉到服务器异常断开连接
		 * retry connect，使用指数补偿算法，尝试重新连接N次，
		 * 如仍未成功连接，则断开连接，关闭socket*/

		buf[numbytes] = '\0';
		printf("Received: %s\n", buf);
	}
	close(sockfd);
	
	return 0;
}

/* 处理终端交互线程的入口函数
* 返回值：void*
* 传入参数：void*,根据实际传入的参数，将参数指针赋给argc，若不止一个参数，
* 则放在一个结构体内，将结构体指针传入*/
void *ioHandler(void *argc)
{
	char strin[MAX_BUF_SIZE];

	while(1)
	{
		if(fgets(strin, MAX_BUF_SIZE, stdin))
		{
			strin[strlen(strin) - 1] = '\0';
			printf("thread %d: %s\n", getpid(), strin);
			memset(strin, 0, MAX_BUF_SIZE);
		}
	}
	return ((void*)0);
}
