#include"client.h"

#include <string.h> /* memove* */
 #include <arpa/inet.h>
 
#include<iostream>
using namespace std;


TcpClient::TcpClient()
{
	memset( m_cbRecvBuf, 0, sizeof(m_cbRecvBuf) );
	m_nRecvSize = 0;
	
	m_Sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	memset(&m_ServerAddr,0,sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = htons(8001);
	inet_pton(AF_INET, SERVER_IP, &m_ServerAddr.sin_addr);
	
	if(connect(m_Sockfd, (SA *) &m_ServerAddr, sizeof(m_ServerAddr))==-1)
	{
		perror("connect:");exit(-1);
	}
}
TcpClient::~TcpClient()
{
	close(m_Sockfd);
}

void TcpClient::run()
{
	int nCount=0;
	for(;;)
	{
		int nRecvSize=::recv(m_Sockfd,m_cbRecvBuf+m_nRecvSize,sizeof(m_cbRecvBuf)-m_nRecvSize,0);
		if(nRecvSize<=0)
		{
			cout<<nRecvSize<<"服务器主动断开连接!"<<endl;
			break;
		}
		//保存已经接收数据的大小
		m_nRecvSize += nRecvSize;
		
		//接收到的数据够不够一个包头的长度
		while(m_nRecvSize>=sizeof(NetPacketHeader))
		{
			//收够10个包，主动与服务器断开连接
			if(nCount >= 10)
			{
				close(m_Sockfd);
				break;
			}
			
			//读取包头
			NetPacketHeader *pHeader=(NetPacketHeader*)(m_cbRecvBuf);
			const unsigned short nPacketSize=pHeader->wDataSize;
			
			//判断是否已接收到足够一个完整包的数据
			if(m_nRecvSize < nPacketSize)
			{
				//还不够拼凑出一个完整包
				break;
			}
			//copy到数据缓存
			memcpy(m_cbDataBuf,m_cbRecvBuf,nPacketSize);
			//从接收缓存移除
			memmove(m_cbRecvBuf,m_cbRecvBuf+nPacketSize,m_nRecvSize);
			m_nRecvSize -= nPacketSize;
			
			// 解密数据，以下省略一万字
			// ...

			// 分派数据包，让应用层进行逻辑处理
			pHeader=(NetPacketHeader*)(m_cbDataBuf);
			const unsigned short nDataSize=nPacketSize - (unsigned short)sizeof(NetPacketHeader);
			
			OnNetMessage(pHeader->wOpcode,m_cbDataBuf+sizeof(NetPacketHeader),nDataSize);
			
			++nCount;
		}
	}
	cout << "已经和服务器断开连接!" <<  endl;
}
bool TcpClient::OnNetMessage(const unsigned short& nOpcode,const char* pDataBuffer,unsigned short nDataSize)
{
	switch(nOpcode)
	{
	case NET_TEST_POD:
	{
		NetPacket_Test_POD *pMsg=(NetPacket_Test_POD *)pDataBuffer;
		return OnNetPacket(pMsg);
		break;
	}
	case NET_TEST_SERIALIZE:
	{
		// 清除缓存中数据
		m_RecvBuffer.clear();
		// 将数据加入到缓存中去
		m_RecvBuffer.append((uint8*)pDataBuffer, nDataSize);
		
		//反序列化数据
		NetPacket_Test_Serialize msg;
		m_RecvBuffer >> msg;
		
		return OnNetPacket(&msg);
		
	}
	break;
	
	default:
	{
		std::cout << "收取到未知网络数据包:" << nOpcode << std::endl;
		return false;
	}
	}
}

bool TcpClient::OnNetPacket(const NetPacket_Test_POD* pMsg )
{
	cout<<"处理POD数据:"<<endl;
	cout << "索引：" << pMsg->nIndex << "  字符串：" << pMsg->arrMessage <<endl;
	return true;
}
bool TcpClient::OnNetPacket( const NetPacket_Test_Serialize* pMsg )
{
	std::cout << "处理序列化数据：" << std::endl;
	std::cout << "索引：" << pMsg->nIndex << "  字符串：" << pMsg->strMessage << std::endl;
	return true;
}

		
			
			
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

