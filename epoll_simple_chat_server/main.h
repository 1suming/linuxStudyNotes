#ifndef _MAIN_H
#define _MAIN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<arpa/inet.h> /* htons */
#include<fcntl.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/wait.h> /* wait */
#include<sys/epoll.h>

#define MAX_EVENTS 256
#define MAX_FD 1024*1024

#define DEBUG

void io_loop(int listen_sock,int epoll_fd);
void process_request(int client,int epoll_fd);


#endif
