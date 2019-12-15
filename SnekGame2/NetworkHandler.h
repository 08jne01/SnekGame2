#pragma once
#include <Maths/Maths.h>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include "QuadTree.h"
#include "Program.h"
#include "Packets.h"

class Program;

class NetworkHandler

{
public:

	enum class Type

	{
		Clear,
		Create,
		Ready
	};

	NetworkHandler(sf::Mutex& mtx, Program* ptr);
	~NetworkHandler();

	//Setup
	bool connect(const std::string& ip, const unsigned short& port, const std::string& name = "Unamed Sad Sneky Boi");
	void bindUDPSocket(const unsigned short& port);
	//Cleanup
	void disconnect();
	//Send
	void sendPacketTCP(sf::Packet packet);
	void sendPacketUDP(sf::Packet packet);
	//Send Packet Specific
	void sendPos(const Vec2f& position, const Vec2f& velocity, const bool& pointsAllowed, const float& bodySize);
	void sendCreate();
	void sendClear();
	//inline void sendEat(const short& index);
	//inline void sendMessage(const Type& type);
	void sendUpdateSneks(const std::string& id, const std::vector<Point>& points); //sends my current snek to 
	//Utility

	//Recieve
	void receiveUDP();
	void receiveTCP();

	//Receive Packet Functions
	void handleCreateGhost(sf::Packet& packet);
	void handleDisconnectGhost(sf::Packet& packet);
	void handleMove(sf::Packet& packet);
	void handleNewPointUDP(sf::Packet& packet);
	void handleClear(sf::Packet& packet);

	static inline NetworkHandler* getInstance()

	{
		return m_instance;
	}

private:
	//Instance of the Network Handler
	//Static so it is kinda global wherever this file is included
	static NetworkHandler* m_instance;

	//Sockets
	sf::UdpSocket m_udpSocket;
	sf::TcpSocket m_tcpSocket;

	//Thread Info
	sf::Thread m_receiveThreadTCP;
	sf::Thread m_receiveThreadUDP;
	sf::Mutex m_mtxTCP;
	sf::Mutex m_mtxUDP;
	sf::Mutex& mtx; //Mutex for the whole program
	bool m_quitTCP = false;
	bool m_quitUDP = false;

	//Connection and Client Info
	sf::IpAddress m_ip = "127.0.0.1";
	unsigned short m_port = 5000;
	std::string m_myID;
	std::string m_name;
	bool m_connected = false;

	//Dropped packets
	std::list<sf::Packet> m_droppedPackets;

	Program* program;
};