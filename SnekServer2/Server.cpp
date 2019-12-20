#include "Server.h"

Server::Server(
	const unsigned short& serverPort
): 
	m_serverPort(serverPort)

{

}

Server::~Server()

{
	for (int i = 0; i < m_connectingThreads.size(); i++)

	{
		m_connectingThreads[i]->wait();
	}
}

int Server::mainLoop()

{
	int codeListener = m_listener.listen(m_serverPort);
	int codeUDPSocket = m_udpSocket.bind(m_serverPort);

	if (codeListener)

	{
		std::cout << "TCP Listener failed to bind!" << std::endl;
		return EXIT_FAILURE;
	}

	if (codeUDPSocket)

	{
		std::cout << "UDP Socket failed to bind!" << std::endl;
		return EXIT_FAILURE;
	}

	m_selector.add(m_listener);
	m_selector.add(m_udpSocket);

	while (m_running)

	{
		if (m_selector.wait())

		{
			if (m_selector.isReady(m_listener))
				listenerReady();
			else if (m_selector.isReady(m_udpSocket))
				udpSocketReady();
			else
				checkTCPSocketsReady();

			sendDroppedPackets();
		}

		for (int i = 0; i < m_clients.size(); i++)

		{
			if (!m_clients[i].m_udpConnected)

			{
				
			}
		}
	}
	return EXIT_SUCCESS;
}

inline void Server::listenerReady()

{
	m_clients.push_back(Client());
	m_clients.back().m_tcpSocket = std::shared_ptr<sf::TcpSocket>(new sf::TcpSocket());
	if (m_listener.accept(*m_clients.back().m_tcpSocket) == sf::Socket::Done)

	{
		//Setup client
		m_selector.add(*m_clients.back().m_tcpSocket);
		std::string id;
		generateID(id);
		m_clients.back().tcpSetup(id);
		//Log to console
		std::cout << "(Server) <- (Client " << 
			m_clients.back().m_id << ") : new connection on address: " << 
			m_clients.back().m_ip << ":" << m_clients.back().m_tcpPort << std::endl;

		std::cout << m_clients.size() << " client(s) connected" << std::endl;
		sf::Packet sendPacket;
		MessagePacket msg(MYID);
		sendPacket << msg << m_clients.back().m_id;
		broadcastTCP(sendPacket, m_clients.size() - 1, true);
		m_connectingThreads.push_back(std::shared_ptr<sf::Thread>(new sf::Thread(&Server::clientConnected, this)));
		m_connectingThreads.back()->launch();
	}

	else
		m_clients.pop_back();
}

inline void Server::udpSocketReady()

{
	sf::Packet receivePacket;
	sf::IpAddress ip;
	unsigned short port;

	if (m_udpSocket.receive(receivePacket, ip, port) == sf::Socket::Done)

	{
		unsigned char type;
		sf::Packet copy = receivePacket;
		copy >> type;
		short connected = getUDPClient(ip, port);

		if (type == UDPJ && connected == -1)

		{
			std::string id;
			copy >> id;
			setUDPClientInfoFromID(ip, port, id);
		}

		else if (type == NPNT)

		{
			PointPacket packet;
			copy >> packet;
			if (packet.m_id == "")

			{
				receivePacket << m_clients[connected].m_id;
				broadcastUDP(receivePacket, connected);
			}	

			else

			{
				short index = getClientFromID(packet.m_id);
				if (index != -1)

				{
					receivePacket << m_clients[connected].m_id;
					broadcastUDP(receivePacket, index, true);
				}
			}
		}

		else if (connected != -1)

		{
			receivePacket << m_clients[connected].m_id;
			broadcastUDP(receivePacket, connected);
		}
	}
}

void Server::clientConnected()

{
	if (!m_clients.size())
		return;

	const std::string id = m_clients.back().m_id;
	short index = getClientFromID(id);
	short count = 0;

	while (!m_clients[index].m_udpConnected && index != -1 && count < 20)

	{
		index = getClientFromID(id);
		sf::Packet sendPacket;
		MessagePacket msg(MYID);
		sendPacket << msg << m_clients[index].m_id;
		broadcastTCP(sendPacket, index, true);
		count++;
		sf::sleep(sf::seconds(0.25));
	}

	if (count >= 20)
		disconnectClient(index);
}

void Server::setUDPClientInfoFromID(const sf::IpAddress& ip, const unsigned short& port, const std::string& id)

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if (m_clients[i].m_id == id)

		{
			m_clients[i].m_udpPort = port;
			m_clients[i].m_udpConnected = true;
			std::cout << "(Server) <- (Client " << id << ") : connected on UDP" << std::endl << std::endl;
			return;
		}
	}
}

void Server::checkTCPSocketsReady()

{
	for (short i = 0; i < m_clients.size(); i++)
	
	{
		if (m_selector.isReady(*m_clients[i].m_tcpSocket))
			tcpSocketReady(i);
	}
}

inline void Server::tcpSocketReady(short& index)

{
	sf::Packet receivePacket;

	if (m_clients[index].m_tcpSocket->receive(receivePacket) == sf::Socket::Disconnected)

	{
		disconnectClient(index);
		index--;
	}

	else

	{
		unsigned char type;
		sf::Packet copy = receivePacket;
		copy >> type;
		switch (type)

		{
			default:
			{
				receivePacket << m_clients[index].m_id;
				broadcastTCP(receivePacket, index);
			}
		}
	}
}

inline void Server::disconnectClient(const short& index)

{
	MessagePacket msg(DSCT);
	sf::Packet sendPacket;
	sendPacket << msg << m_clients[index].m_id;
	broadcastTCP(sendPacket, index);
	std::cout << "(Server) -> (Client "
		<< m_clients[index].m_id << ") : Disconnected. "
		<< m_clients.size() - 1 << " clients(s) remaining" << std::endl << std::endl;
	m_selector.remove(*m_clients[index].m_tcpSocket);
	m_clients.erase(m_clients.begin() + index);
}

//Setup
void Server::generateID(std::string& s)

{
	bool exists = true;
	while (exists)

	{
		createID(s);
		exists = checkID(s);
	}
}

void Server::createID(std::string& s)

{
	s = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < 5; i++)

	{
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}
}

//Utility
short Server::getUDPClient(const sf::IpAddress& ip, const unsigned short& port)

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if (m_clients[i].m_ip == ip && m_clients[i].m_udpPort == port)
			return i;
	}

	return -1;
}

short Server::getClientFromID(const std::string& id)

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if (m_clients[i].m_id == id)
			return i;
	}
	return -1;
}

bool Server::checkID(std::string& s) //check id exists

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if (m_clients[i].m_id == s)
			return true;
	}

	return false;
}

//Broadcast TCP: Leave index and me default for broadcast to all. Set index to something to not broadcast to
//that client. Set index to something and me to true to broadcast to only that client.
void Server::broadcastTCP(sf::Packet& packet, const short& index, const bool& me)

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if ((!me && index != i) || (me && index == i))
			sendTCPPacket(m_clients[i], packet, index, me);
	}
}
//Broadcast UDP: Leave index and me default for broadcast to all. Set index to something to not broadcast to
//that client. Set index to something and me to true to broadcast to only that client.
void Server::broadcastUDP(sf::Packet& packet, const short& index, const bool& me)

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if ((!me && index != i) || (me && index == i))
			m_udpSocket.send(packet, m_clients[i].m_ip, m_clients[i].m_udpPort);
	}
}

void Server::sendDroppedPackets()

{
	for (int i = 0; i < m_clients.size(); i++)

	{
		if (m_clients[i].m_dropped.size() > 0)

		{
			DroppedPacket packet = m_clients[i].m_dropped.front();
			m_clients[i].m_dropped.pop_front();
			broadcastTCP(packet.m_packet, packet.m_index, packet.m_me);
		}
	}
}

//Inlined functions
inline void Server::sendTCPPacket(Client& client, sf::Packet& packet, const short& index, const bool& me)

{
	sf::Socket::Status status = client.m_tcpSocket->send(packet);
	if (status != sf::Socket::Done && status != sf::Socket::Disconnected)

	{
		std::cout << "(Server) -> (Client " << client.m_id << ") : dropped packet" << std::endl;
		client.m_dropped.push_back(DroppedPacket(index, me, packet));
	}
}

