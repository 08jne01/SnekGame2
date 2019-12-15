#pragma once
#include <memory>
#include <list>
#include <SFML/Network.hpp>
#include "DroppedPacket.h"
class Client

{
public:
	Client() {}
	Client(
		const sf::IpAddress& ip,
		const unsigned short& port,
		const std::string& id
	) :
		m_ip(ip),
		m_udpPort(port),
		m_id(id)
	{

	}

	~Client() {}

	void tcpSetup(const std::string& s)

	{
		m_tcpPort = m_tcpSocket->getRemotePort();
		m_ip = m_tcpSocket->getRemoteAddress();
		m_id = s;
	}
	
	std::string m_id;
	//UDP details
	sf::IpAddress m_ip;
	unsigned short m_udpPort = 0;
	//TCP details
	std::shared_ptr<sf::TcpSocket> m_tcpSocket;
	unsigned short m_tcpPort;
	
	//Dropped Packets
	std::list<DroppedPacket> m_dropped;

	//Client timer
	sf::Clock clock;
private:
};