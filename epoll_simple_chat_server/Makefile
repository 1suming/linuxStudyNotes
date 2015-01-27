CC=c99
CFLAGS=-Wall -O2
PROGS=epoll_simple_chat_server
proxy:clean main.c main.h
	$(CC) $(CFLAGS) -o $(PROGS) main.c network.c process.c -D_GNU_SOURCE
clean:
	rm  -f $(PROGS) *.o
