/*for sockaddr_in */
#include<netinet/in.h>
/*for socket functions */
#include<sys/socket.h>
/*for cntl */
#include<fcntl.h>

#include<event2/event.h>
#include<event2/buffer.h>
#include<event2/bufferevent.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


// 这两个函数成功返回 0 失败返回 -1
#ifdef WIN32
int evthread_use_windows_threads(void);
#define EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED
#endif
#ifdef _EVENT_HAVE_PTHREADS
int evthread_use_pthreads(void);
#define EVTHREAD_USE_PTHREADS_IMPLEMENTED
#endif


#define MAX_LINE 16384


char rot13_char(char c)
{
	/* We don't want to use isalpha here; setting the locale would change
	* which characters are considered alphabetical. */
	if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
		return c + 13;
	else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
		return c - 13;
	else
		return c;

}
void readcb(struct bufferevent *bev,void *ctx)
{
	struct evbuffer *input,*output;
	char *line;
	size_t n;
	int i;
	input=bufferevent_get_input(bev);
	output=bufferevent_get_output(bev);
	/*
	evbuffer_readln (struct evbuffer *buffer, size_t *n_read_out, enum evbuffer_eol_style eol_style)
 	Read a single line from an evbuffer. 
	*/
	while((line=evbuffer_readln(input,&n,EVBUFFER_EOL_LF)))
	{
		for(i=0;i<n;i++)
		{
		 // line[i]=rot13_char(line[i]);
		}
			
			/*
			int 	evbuffer_add (struct evbuffer *buf, const void *data, size_t datlen)
			Append data to the end of an evbuffer. 
		*/
			evbuffer_add(output,line,n);
			evbuffer_add(output,"\n",1);
			free(line);
		 
	}
	
	if(evbuffer_get_length(input)>=MAX_LINE)
	{
		/*Too long,just process what there is and go on 
		so that the buffer doesn't grow infinity long 
		*/
		char buf[1024];
		while(evbuffer_get_length(input))
		{
			/*
			int 	evbuffer_remove (struct evbuffer *buf, void *data, size_t datlen)
 	Read data from an evbuffer and drain the bytes read. 
	*/
			int n=evbuffer_remove(input,buf,sizeof(buf));
			/*for (i = 0; i < n; ++i)
				buf[i] = rot13_char(buf[i]);*/
			evbuffer_add(output,buf,n);
			
		}
		evbuffer_add(output,"\n",1);
	}
}
void errorcb(struct bufferevent *bev,short error,void *ctx)
{
	if(error & BEV_EVENT_EOF)
	{
		/* connection has been closed, do any clean up here */
	}
	else if(error & BEV_EVENT_ERROR)
	{
		/* check errno to see what error occurred */
	}
	else if(error & BEV_EVENT_TIMEOUT)
	{
		/* must be a timeout event handle, handle it */
	}
}

void do_accept(evutil_socket_t listener,short event,void *arg)
{
	struct event_base *base=(event_base*)arg;
	struct sockaddr_in cliaddr;
	socklen_t len=sizeof(cliaddr);
	int fd=accept(listener,(struct sockaddr*)&cliaddr,&len);
	if(fd<0)
		perror("accept");
	else if(fd>FD_SETSIZE)
		close(fd);
	else
	{
		struct bufferevent *bev;
		evutil_make_socket_nonblocking(fd);
		bev=bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(bev,readcb,NULL,errorcb,NULL);
		bufferevent_setwatermark(bev,EV_READ,0,MAX_LINE);
		bufferevent_enable(bev,EV_READ|EV_WRITE);
		
	}
	
}
void run()
{
	evutil_socket_t listener;
	struct sockaddr_in sin;
	struct event_base *base;
	struct event *listener_event;
	
	base=event_base_new();
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(8888);
	
	listener=socket(AF_INET,SOCK_STREAM,0);
	evutil_make_socket_nonblocking(listener);
	
	int one=1;
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	
	bind(listener,(struct sockaddr*)&sin,sizeof(sin));
	
	listen(listener,16);
	
	//event_new(base,evutil_socket_t,short, event_callback_fn,void *)
	listener_event=event_new(base,listener,EV_READ|EV_PERSIST,do_accept,(void*)base);
	/**XXX check it */
	event_add(listener_event,NULL);
	
	event_base_dispatch(base);
	
}

int main(int argc,char *argv[])
{
		//_IONBUF : no buffer 
	setvbuf(stdout,NULL,_IONBF,0);
	run();
}
 