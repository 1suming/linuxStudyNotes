/*
 NetDefine.h 
 */
 #ifndef _NETDEFINE_H
 #define   _NETDEFINE_H
 
 #include"TypeDefine.h"
 #include"ByteBuffer.h"
 
 ////////////////////////////////////////////////////
#define NET_PACKET_DATA_SIZE 1024 
#define NET_PACKET_SIZE (sizeof(NetPacketHeader) + NET_PACKET_DATA_SIZE) * 10


#include<stdio.h>
#include <errno.h>
#include<stdlib.h>
 
 /////////////////////////////////////////////////////////////////
 
 ///网络数据包包头
 struct NetPacketHeader
 {
	 unsigned short wDataSize; ///< 数据包大小，包含封包头和封包数据大小
	 unsigned short wOpcode; ///<操作码
 };
 
 ///网络数据包
 struct NetPacket{
	 NetPacketHeader header; //包头 
	 unsigned char Data[NET_PACKET_DATA_SIZE ];//数据
 };
 
 
 ////////////////////////////////////////////
 //网络操作码
 enum eNetOpcode
 {
	 NET_TEST_POD			= 1,	///< 测试原生数据直接拷贝
	NET_TEST_SERIALIZE		= 2,	///< 测试序列化
};


/// 测试1的网络数据包定义
struct NetPacket_Test_POD
{
	int		nIndex;
	char	arrMessage[512];
};

/// 测试2的网络数据包定义
struct NetPacket_Test_Serialize
{
	int			nIndex;
	std::string	strMessage;
};
/*
//错误处理
void diep(const char*s)
{
	perror(s);
	exit(-1);
}
*/
NET_APPEND(NetPacket_Test_Serialize)
{
	lht << rht.nIndex
		<< rht.strMessage;
	return lht;
};
NET_READ(NetPacket_Test_Serialize)
{
	lht >> rht.nIndex
		>> rht.strMessage;
	return lht;
};



 
 #endif