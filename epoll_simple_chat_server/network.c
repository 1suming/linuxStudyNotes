#include"network.h"

void perror_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void make_socket_non_blocking(int fd)
{
	int flags;
	if((flags=fcntl(fd,F_GETFL,0))<0)
		perror_exit("fcntl");
	flags |= O_NONBLOCK;
	if(fcntl(fd,F_SETFL,flags)<0)
		perror_exit("F_SETFL error");
}

int  open_non_blocking_socket(int port)
{
	int listenfd,optval=1;
	struct sockaddr_in servaddr;
	
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
		perror_exit("scoket error");
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int))<0)
		perror("setsockopt error");
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons((unsigned short)port);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(listenfd,(SA*)&servaddr,sizeof(servaddr))<0)
		perror_exit("bind error");
	
	make_socket_non_blocking(listenfd);
	
	listen(listenfd,LISTENQ);
	
	return listenfd;
}

	
/* ×¢²á·þÎñÆ÷listenfd */
int epoll_start(int listen_sock)
{
	int efd=epoll_create(1000);
	if(efd==-1)
		perror_exit("epoll create error");
	
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=listen_sock;
	epoll_ctl(efd,EPOLL_CTL_ADD,listen_sock,&ev);
	
	return efd;
}


int accept_incoming(int listen_sock,int epoll_fd)
{
	struct epoll_event ev;
	struct sockaddr_in cliaddr;
	socklen_t clilen=sizeof(cliaddr);
	
	int client=accept(listen_sock,(SA*)&cliaddr,&clilen);
	if(client<0)
		perror("accept error");
	printf("accept client is %d\n",client);
	
	make_socket_non_blocking(client);
	
	ev.events=EPOLLIN | EPOLLOUT | EPOLLET;
	ev.data.fd=client;
	
	if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client,&ev)<0)
		perror_exit("epoll_ctl error");
	return client;
}
	
void close_and_clean(int fd)
{
	close(fd);
}

int read_all(int fd,char *buf)
{
	ssize_t n=0;
	ssize_t count;
	while((count=read(fd,buf+n,sizeof buf)) >0)
	{
		n+=count;
	}
	buf[n]='\0';
	
	return n;
}


int send_all(int fd,char *buf)
{
	size_t nleft,data_size;
	nleft=data_size=strlen(buf);
	
	ssize_t nwritten;
	const char *ptr=buf;
	while(nleft>0)
	{
		if((nwritten=write(fd,ptr,nleft))<=0) /*×¢Òâ= */
		{
			if(nwritten<0 && errno==EINTR)
				break;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr+=nwritten;
	}
	return data_size;
}
		