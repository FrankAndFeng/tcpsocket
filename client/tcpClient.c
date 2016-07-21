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

/* 重新连接服务器的最大等待时间 */
#define MAXSLEEP 64

/* 服务器断开重连 */
int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen);

/* 可移植的断开重连函数 */
int connect_retry_new(int domain, int type, int protocol,
        const struct sockaddr *sockfd, socklen_t alen);

/* 处理终端交互的线程和入口函数,以及输入参数 */
pthread_t pthd_io;
void *ioHandler(void *argc);

typedef struct _iop
{
	int sockfd;
}iop;
iop *inp;

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
	inp = (iop *)malloc(sizeof(struct _iop));
	inp->sockfd = sockfd;
	int err;
	if ((err = pthread_create(&pthd_io, NULL, ioHandler, (void *)inp)) == 0)
	{
		printf("create pthread succeed : %d\n", err);
	}
	printf("the client sockfd is : %d\n",((iop *)inp)->sockfd);
	/* 一直接受服务器数据，默认不关心数据来源 */
	while(1)
	{
		numbytes = recv(sockfd, buf, MAX_BUF_SIZE, 0);
		if (numbytes > 0)
		{
			buf[numbytes] = '\0';
			printf("Received from server: %s\n", buf);
		}
		else
		{
            //int res = connect_retry(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            int sockfd = connect_retry_new(AF_INET, SOCK_STREAM, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
            if (sockfd == -1)
            {
                close(sockfd);
			    perror("recv");
			    exit(1);
            }
		}
		/* 此处添加异常处理，如果捕捉到服务器异常断开连接
		 * retry connect，使用指数补偿算法，尝试重新连接N次，
		 * 如仍未成功连接，则断开连接，关闭socket*/
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
	int flag = 0;
    int sockfd = ((iop *)argc)->sockfd;
	//printf("the client sockfd is : %d\n",((iop *)argc)->sockfd);
	while (1)
	{
		if (fgets(strin, MAX_BUF_SIZE, stdin))
		{
			strin[strlen(strin) - 1] = '\0';
			printf("thread %d: %s\n", getpid(), strin);
			flag = send(sockfd, strin, strlen(strin), 0);
			printf("send the data? %d\n", flag);
			memset(strin, 0, strlen(strin));
		}
	}
	return ((void*)0);
}
/* 服务器断开重连，采用指数补偿算法，如果和服务器的连接意外断开
 * 进程会休眠一小段，然后进入下次循环再次尝试，
 * 每次循环休眠时间按照指数增加，直到最大延迟为一分钟左右*/
int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
    int numsec;
    /*
     *以指数增长的休眠时间重新连接
     */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        printf("trying to connect with server....\n");
        if (connect(sockfd, addr, alen) == 0)
        {
            return 0;
        }
        if (numsec <= MAXSLEEP/2)
            sleep(numsec);
    }
    return -1;
}

int connect_retry_new(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;

    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        printf("retrying connect...\n");
        if ((fd = socket(domain, type, protocol)) < 0)
            return -1;
        if (connect(fd, addr, alen) == 0)
        {
            return fd;
        }
        close(fd);
        if  (numsec <= MAXSLEEP/2)
        {
            sleep(numsec);
        }
    }
    return -1;
}
