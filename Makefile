tcpserver:tcpfunc.o tcpserver.o
	gcc tcpfunc.o tcpserver.o -o tcpserver -lpthread
tcpfunc.o:tcpfunc.h tcpfunc.c
	gcc -c tcpfunc.c -o tcpfunc.o -lpthread
tcpserver.o:tcpServer.c tcpfunc.h
	gcc -c tcpServer.c -D DEBUG_ON -o tcpserver.o -lpthread
clean:
	rm tcpfunc.o tcpserver.o
