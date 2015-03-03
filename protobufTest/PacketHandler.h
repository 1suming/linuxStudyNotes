#ifndef _PACKET_HANDLER_H
#define  _PACKET_HANDLER_H

#include<stdint.h>
#include<string>
#include<iostream>
#include"packet.pb.h"

//defines

#define PACKET_SIZE 32
#define INT_PACKET 3
#define MAX_BUFFER 960  //this is packet*30

enum PacketTypes{
	 PingPacketType = 1,
    // Sends one string (ping/pong) to check sockets alive
    LoginPacketType = 2,
    // Sends username (string) then password (string)
};

//packet header

typedef struct{
	uint32_t packetType;
	uint32_t packetLen;
}PacketHeader;

class PacketHandler{
public:
	PacketHandler();
	std::string login_packet(char *username,char *pwd,int version);
	std::string auth_code_packet(char *auth_code);
	std::string connected_packet(bool conn);
	
private:
	unsigned char *m_packetBuffer;
	unsigned char *m_packet;
	PacketHeader *m_packetHeader;
	
	void newPacket();
};
	
#endif