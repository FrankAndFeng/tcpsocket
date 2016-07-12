#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netdb.h>

/* 服务器程序监听的端口号 */
#define PORT 8848

/* 一次所能够接收的最大字节数 */
#define MAX_DATA_SIZE 256

int main(int argc, char** argv)
{
	int sockfd, numbytes;
	char buf[MAX_DATA_SIZE];
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
	while(1)
	{
		if ((numbytes = recv(sockfd, buf, MAX_DATA_SIZE, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}

		buf[numbytes] = '\0';
		printf("Received: %s\n", buf);
	}
	close(sockfd);
	
	return 0;
}


