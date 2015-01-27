#ifndef _NETWORK_H
#define _NETWORK_H

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

#define LISTENQ 128

typedef struct sockaddr SA;

int epoll_start(int listen_sock);
void make_socket_non_blocking(int fd);
/*在某个port打开套接字，返回listenfd */
int  open_non_blocking_socket(int port);

int accept_incoming(int listen_sock,int epoll_fd);
void close_and_clean(int fd);

int read_all(int fd,char *buf);
int send_all(int fd,char *buf);

#endif