termio:termIO.o tcpfunc.o
	gcc termIO.o tcpfunc.o -o termio -lpthread
termIO.o:termIO.c tcpfunc.h
	gcc -c termIO.c -o termIO.o -lpthread
tcpfunc.o:tcpfunc.c tcpfunc.h
	gcc -c tcpfunc.c -o tcpfunc.o -lpthread
clean:
	rm termIO.o tcpfunc.o
