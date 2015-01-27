#include"main.h"
#include"network.h"
#include"process.h"

/* suggest user redis to store fd info */
int fd_map[MAX_FD]={0}; /*对话连接，如fd[1]=2,fd[2]=1,表示fd为1的与2进行对话*/
int ready_fd_map[MAX_FD]={0}; /* alreayd connected fd */

void io_loop(int listen_sock,int epoll_fd)
{
	int nfds;
	struct epoll_event events[MAX_EVENTS];
	
	while(1)
	{
		
		nfds=epoll_wait(epoll_fd,events,MAX_EVENTS,-1);/* epoll 和select不同，0理解返回，-1将不确定，也有说法是永久阻塞，如果返回0表示超时*/
		for(int i=0;i<nfds;i++)
		{
			if(events[i].data.fd == listen_sock) /*有新的连接*/
			{
				int client=accept_incoming(listen_sock,epoll_fd);
				ready_fd_map[client]=1; /*client has connected */
			}
			else
			{
				if(events[i].events & EPOLLERR)
				{
					#ifdef DEBUG
						printf("error condition,events:%d,fd:%d\n",events[i].events,epoll_fd);
					#endif
					close_and_clean(events[i].data.fd);
					close_and_clean(fd_map[events[i].data.fd]);
				}
				else if(events[i].events & EPOLLIN)
				{
					printf("process request,sock_fd %d\n",epoll_fd);
					process_request(events[i].data.fd,epoll_fd);
				}
				else if(events[i].events & EPOLLOUT)
				{
					  printf("EPOLLOUT sock_fd: %d write\n",epoll_fd);
				}
			}
		}
				

	}
}

void process_request(int client,int epoll_fd)
{
	ssize_t count;
	if(!fd_map[client])
	{
		
        /* proxy to server*/
        
        char buf[10];
        count=read_all(client,buf);
		
		int connect_fd=atoi(buf);/*要对话的一方fd*/
		printf("client fd:%d\n",connect_fd);
		
		if(!connect_fd)
		{
			close(client);
			return;
		}
		if(!ready_fd_map[connect_fd])
		{
			close(client);
			return;
		}
		
		fd_map[client]=connect_fd;
		fd_map[connect_fd]=client;
		
		 printf("register %d %d\n" , client , connect_fd);
		
	}
	else
	{
		 /*
           1. client read to porxy
           2. proxy read to client
        */
        char buf[1048576]; //buf 1M
		count=read_all(client,buf);
		int fd=fd_map[client];
		
		printf("send all to %d from %d\n",fd,client);
		
		if(strlen(buf)>0)
			send_all(fd,buf);
		else
		{
			printf("buffer is null %d",fd);
		}
	}
}
		
int main(int argc, char**argv)
{
	//1.listen socket on port
	int listen_sock=open_non_blocking_socket(8001);
	
	//2.fork load balance server
	fork_process(2);
	
	//3.create epoll and register events
	int epoll_fd=epoll_start(listen_sock);
	
	//4.io loop for waiting network events
	io_loop(listen_sock,epoll_fd);
	
	//no coming
	return 0;
}