/*
server.h 
*/

#ifndef _SERVER_H
#define  _SERVER_H

#include"NetDefine.h"

#include<unistd.h>
#include <sys/socket.h>  
#include <netinet/in.h>
 #include <arpa/inet.h>
 
class TcpServer
{
public:
	TcpServer();
	virtual ~TcpServer();
	
public:
	void run();
	void acceptClient();
	void closeClient();
	
	bool  SendData( unsigned short nOpcode, const char* pDataBuffer, const unsigned int& nDataSize );
		
		
private:
	int m_listenfd;
	sockaddr_in m_ServerAddr;
	
	int m_connfd;
	sockaddr_in m_ClientAddr;
	
	char m_cbSendBuf[NET_PACKET_SIZE];	///< 发送缓存
	ByteBuffer m_SendBuffer; 	///< 发送缓存
};

 

#endif