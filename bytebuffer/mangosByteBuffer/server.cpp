#include"server.h"

#include<iostream>
using namespace std;

#include<string.h>

TcpServer::TcpServer()
{
	m_listenfd=socket(AF_INET,SOCK_STREAM,0);
	
	memset(&m_ServerAddr,0,sizeof(m_ServerAddr));
	m_ServerAddr.sin_family=AF_INET;
	m_ServerAddr.sin_port=htons(8001);
	m_ServerAddr.sin_addr.s_addr=INADDR_ANY;
	
	bind(m_listenfd,(SA*)&m_ServerAddr,sizeof(m_ServerAddr));
	listen(m_listenfd,10);
	
	cout << "sdf启动TCP服务器成功!" << std::endl;
}
TcpServer::~TcpServer()
{
	close(m_listenfd);
	cout<<"关闭TCP服务器成功!" << std::endl;
}

void TcpServer::run()
{
	acceptClient();
	
	int nCount=0;
	
	for(;;)
	{
		 
		//发送POD数据包
		{
			NetPacket_Test_POD msg;
			msg.nIndex = nCount;
			strncpy(msg.arrMessage, "[1]你好[2]你好[3]你好", sizeof(msg.arrMessage) );
			bool bRet = SendData(NET_TEST_POD, (const char*)&msg, sizeof(msg));
			if (bRet)
			{
				std::cout << nCount<<"->发送数据成功！" << std::endl;
			}
			else
			{
				std::cout << "发送数据失败！" << std::endl;
				break;
			}
		}
		// 发送序列化数据包
		{
			NetPacket_Test_Serialize msg;
			msg.nIndex = nCount;
			msg.strMessage = "[1]早晨[2]早晨[3]早晨";
			
			//清除缓存
			m_SendBuffer.clear();
			//序列化
			m_SendBuffer << msg;
			
			bool bRet=SendData(NET_TEST_SERIALIZE,(const char*)m_SendBuffer.contents(),
				(const unsigned int) m_SendBuffer.size());
			if (bRet)
			{
				std::cout << "发送数据成功！" << std::endl;
			}
			else
			{
				std::cout << "发送数据失败！" << std::endl;
				break;
			}
		}
		++nCount;
	}
 
}

void TcpServer::closeClient()
{
	if(m_connfd==-1) return;
	
	close(m_connfd);
	std::cout << "客户端套接字已关闭!" << std::endl;
}
void TcpServer::acceptClient()
{
	//以阻塞方式，等待客户端连接
	socklen_t clilen=sizeof(m_ClientAddr);
	m_connfd=accept(m_listenfd,(SA*)&m_ClientAddr,&clilen);
	std::cout << "接受客户端IP:" << inet_ntoa(m_ClientAddr.sin_addr)<<endl;
}

bool TcpServer::SendData(unsigned short nOpcode,const char *pDataBuffer,const unsigned int &nDataSize)
{
	NetPacketHeader* pHead = (NetPacketHeader*) m_cbSendBuf;
	pHead->wOpcode=nOpcode;
	//数据封包
	if( (nDataSize>0) && (pDataBuffer!=0))
	{
		memmove(pHead+1,pDataBuffer,nDataSize); //+1为什么
	}
	//发送消息
	const unsigned short nSendSize=nDataSize+ sizeof(NetPacketHeader);
	pHead->wDataSize=nSendSize;
	
	int ret=send(m_connfd,m_cbSendBuf,nSendSize,0);
	return (ret>0) ? true :false;
}
	
	