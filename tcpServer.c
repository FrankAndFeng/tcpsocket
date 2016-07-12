#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

/* 服务器要监听的本地端口号 */
#define LOCPORT 8848

/* 能够同时接受多少没有accept的连接 */
#define BACKLOG 10

int main(int argc, char** argv)
{
	/* 在sock_fd上进行监听，在new_fd上接受新的连接 */
	int sock_fd, new_fd;
	/* 本地的地址信息 */
	struct sockaddr_in my_addr;
	int	sin_size = 0;

	/* 连接者的地址信息 */
	struct sockaddr_in their_addr;

	/* 调用socket()，生成套接字描述符 */
	if ((sock_fd = socket(AF_INET,SOCK_STREAM,0)) == -1 )
	{
		perror("socket");
		exit(1);
	}

	/* 主机字节顺序 */
	my_addr.sin_family = AF_INET;

	/* 将主机需要绑定的端口号，转化为网络字节顺序的短整型 */
	my_addr.sin_port = htons(LOCPORT);

	/* 将运行程序主机的IP填充入s_addr,INADDR_ANY = 0 */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* 将此结构的其余空间清零 */
	bzero(&(my_addr.sin_zero), 8);

	/* 将套接字描述符sockfd与地址绑定my_addr */
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("perror");
		exit(1);
	}

	/* 开启监听，在指定的sockfd，最多容纳BACKLOG个未accept的消息 */
	if (listen(sock_fd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	/* 主循环,一直保持accept()状态 */
	while (1)
	{
		sin_size = sizeof(struct sockaddr_in);
		
		/* 接受远程连接，accept会一直等待接收，如果调用accept出现错误，
		   则给出错误提示，并进入下一个循环 */
		if ((new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
		if (send(new_fd, "Hello! ",10, 0) == -1)
		{
			perror("send");
			exit(1);
		}

	}		
}
