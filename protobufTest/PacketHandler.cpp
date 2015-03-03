#include"PacketHandler.h"
#include<google/protobuf/message_lite.h>

PacketHandler::PacketHandler(){ }
std::string PacketHandler::connected_packet(bool conn)
{
	Packet::PacketHeader header;
	
	Packet::PacketHeader::connected_packet *connected=header.mutable_connected_packet();
	
	if(conn)
	{
		connected->set_connected(1);
	}
	std::string buf;
	header.SerializeToString(&buf);
	
	return buf;
}

std::string PacketHandler::login_packet(char *username,char *pwd,int version)
{
	Packet::PacketHeader header;
	
	Packet::PacketHeader::LoginPacket *login=header.mutable_login_packet;
	login->set_username(username);
	login->set_password(pwd);
	login->set_version(version);
	
	std::string buf;
	header.SerializeToString(&buf);
	
	return buf;
}

std::string PacketHandler::auth_code_packet(char *auth_code) {
    Packet::PacketHeader header;

    header.set_auth_code(auth_code);
     
    std::string buf;
    header.SerializeToString(&buf);

    return buf;
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
