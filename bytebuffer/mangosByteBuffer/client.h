/* 
client.h 
*/
#ifndef _CLIENT_H
#define  _CLIENT_H

#include"NetDefine.h"

#include<unistd.h>
#include <sys/socket.h>  
 #include <netinet/in.h>
class TcpClient
{
public:
	TcpClient();
	virtual ~TcpClient();
public:
	///主循环
	void run();
	///处理网络消息
	bool OnNetMessage(const unsigned short& nOpcode,const char* pDataBuffer,unsigned short nDataSize);
	
	bool OnNetPacket(const NetPacket_Test_POD* pMsg);
	bool OnNetPacket(const NetPacket_Test_Serialize* pMsg);
	
private:
	int m_Sockfd;
	sockaddr_in m_ServerAddr;
	
	char m_cbRecvBuf[NET_PACKET_SIZE];
	char m_cbDataBuf[NET_PACKET_SIZE];
	int m_nRecvSize;
	
	ByteBuffer m_RecvBuffer;
};

#endif