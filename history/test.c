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

int main()
{
	struct hostent *he;
	he = gethostbyname("127.0.0.1");
	printf("host name: %s\n", he->h_name);
	printf("host address: %lu\n", (unsigned long)he->h_addr);
	printf("size of unsigned long: %lu\n", sizeof(unsigned int));
}
