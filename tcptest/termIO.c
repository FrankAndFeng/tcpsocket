#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>

#include<pthread.h>
#include "tcpfunc.h"

/*终端接受字符串最大长度*/
#define MAX_BUF_SIZE 256

/*新的线程*/
pthread_t ntid;

char strin[MAX_BUF_SIZE];

void *output_handler(void *argc);

int main(int argc, char** argv)
{
	int pid, err;
	pid = getpid();
	printf("this is the main thread %d\n", pid);
    homePage();

    int inchar;
    while (1)
    {
        if ((inchar = fgetc(stdin)) && ((inchar) != '\n'))
        {
            switch(inchar)
            {
                case '0':
                    printf("printf help\n");
                    break;
                default:
                    printf("%c\n",(char)inchar);
            }
        }
    }
	char *sockfd = "sockfd1";
	err = pthread_create(&ntid, NULL, output_handler, sockfd);
	if (err != 0)
	{
		printf("fail to create the pthread\n");
	}
	while(1)
	{
		printf("i am the main thread!\n");
		sleep(10);
	}
	return 0;

}

