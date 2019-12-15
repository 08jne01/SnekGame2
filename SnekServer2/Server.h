#pragma once
#include <iostream>
#include <Maths/Maths.h>

#include "../SnekGame2/Packets.h"
#include "Client.h"

class Server

{
public:
	Server(const unsigned short& serverPort);
	int mainLoop();

	//Setup
	void generateID(std::string& s);
	void createID(std::string& s);
	
	//Utility
	short getUDPClient(const sf::IpAddress& ip, const unsigned short& port);
	short getClientFromID(const std::string& id);
	bool checkID(std::string& s); //check id exists

	//Broadcast
	void broadcastTCP(sf::Packet& packet, const short& index = -1, const bool& me = false);
	void broadcastUDP(sf::Packet& packet, const short& index = -1, const bool& me = false);
	void sendDroppedPackets();

	//Inlined functions
	inline void sendTCPPacket(Client& client, sf::Packet& packet, const short& index, const bool& me);
	inline void listenerReady();

	inline void udpSocketReady();
	void setUDPClientInfoFromID(const sf::IpAddress& ip, const unsigned short& port, const std::string& id);

	void checkTCPSocketsReady();
	inline void tcpSocketReady(short& index); 
	inline void disconnectClient(const short& index);

private:
	std::vector<Client> m_clients;

	sf::SocketSelector m_selector;
	sf::TcpListener m_listener;
	sf::UdpSocket m_udpSocket;

	bool m_running = true;
	const unsigned short m_serverPort;
};