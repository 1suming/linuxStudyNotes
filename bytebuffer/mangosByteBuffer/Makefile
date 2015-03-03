CC=g++
CFLAGS=-w

PROGS=clientMain serverMain
all:$(PROGS)


clientMain:clientMain.cpp client.cpp NetDefine.h
	$(CC) -w -o $@ $^ 
	
serverMain:serverMain.cpp server.cpp NetDefine.h
	$(CC) -w -o $@ $^ 
	
clean:
	rm  -rf *.o $(PROGS)