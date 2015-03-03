all:server client auth_server
LIBS= -lsqlite3 `pkg-config --cflags --libs protobuf` 

protoc_middleman:packet.proto
	protoc --cpp_out=. packet.proto 

auth_server:AuthServer.o AuthDatabaseHandler.o PacketHandler.o packet.pb.o
	g++ -o $@ -g $^ $(LIBS)
	
server:GameServer.o PacketHandler.o packet.pb.o  AuthDatabaseHandler.o
	g++ -o $@ -g $^ $(LIBS)

	
client:GameClient.o Packet.pb.o PacketHandler.o 
	g++ -o $@ -g $^ $(LIBS)
	
clean:
	rm AuthServer.o AuthDatabaseHandler.o PacketHandler.o packet.pb.o GameServer.o GameClient.o packet.pb.cpp packet.pb.h