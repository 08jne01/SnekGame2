#include "NetworkHandler.h"

NetworkHandler* NetworkHandler::m_instance = nullptr;

NetworkHandler::NetworkHandler(
	sf::Mutex& mtx_,
	Program* ptr
):
	mtx(mtx_),
	program(ptr)
{
	if (!m_instance)
		m_instance = this;
}

NetworkHandler::~NetworkHandler()

{
	//m_receiveThreadTCP->wait();
	//m_receiveThreadUDP->wait();
}

bool NetworkHandler::connect(const std::string& ip, const unsigned short& port, const std::string& name)

{
	m_ip = ip;
	m_port = port;
	m_name = name;

	bool connected = false;
	short tries = 0;

	while (!connected && tries < 5)

	{
		std::cout << "Connecting..." << std::endl;
		if (!m_tcpSocket.connect(m_ip, m_port, sf::seconds(5)))

		{
			std::cout << "Connected to TCP on " << m_ip << ":" << m_port << std::endl;
			connected = true;
		}

		else

		{
			std::cout << "Error connection timed out!" << std::endl;
		}
	}

	if (!connected)
		return false;
		

	bindUDPSocket(m_port);

	sf::Packet receivePacket;
	bool gotID = false;
	if (m_tcpSocket.receive(receivePacket) == sf::Socket::Done)

	{
		unsigned char type;
		receivePacket >> type;
		if (type == MYID)

		{
			receivePacket >> m_myID;
			std::cout << "Got my ID: " << m_myID << std::endl;
			sf::Packet sendPacket;
			MessagePacket msg(UDPJ);
			sendPacket << msg << m_myID;
			m_udpSocket.send(sendPacket, m_ip, m_port);
			gotID = true;
		}
	}
	if (!gotID)

	{
		disconnect();
		return false;
	}

	sendCreate();
	m_connected = true;


	m_receiveThreadTCP = std::shared_ptr<sf::Thread>(new sf::Thread(&NetworkHandler::receiveTCP, this));
	m_receiveThreadUDP = std::shared_ptr<sf::Thread>(new sf::Thread(&NetworkHandler::receiveUDP, this));

	m_receiveThreadTCP->launch();
	m_receiveThreadUDP->launch();

	return true;
}

void NetworkHandler::reset()

{
	m_quitTCP = false;
	m_quitUDP = false;
	m_connected = false;
	m_droppedPackets.clear();
}

void NetworkHandler::bindUDPSocket(const unsigned short& port)

{
	unsigned short code = m_udpSocket.bind(port);
	for (int i = 0; code; i++)

	{
		i++;
		code = m_udpSocket.bind(port + i);
	}

	std::cout << "UDP Socket bound to " << m_udpSocket.getLocalPort() << std::endl;
}

void NetworkHandler::receiveTCP()

{
	sf::Packet packetReceive;
	while (!m_quitTCP)

	{
		packetReceive.clear();
		sf::Socket::Status type = m_tcpSocket.receive(packetReceive);

		if (type == sf::Socket::Disconnected)

		{
			std::cout << "Disconnected from server!" << std::endl;
			m_connected = false;
		}

		else if (type == sf::Socket::Done)

		{
			unsigned char type;
			packetReceive >> type;

			switch (type)

			{
			case CREA:
				handleCreateGhost(packetReceive);
				break;
			case NPNT:
				handleNewPointUDP(packetReceive);
			case DSCT:
				handleDisconnectGhost(packetReceive);
				break;
			case MCLR:
				handleClear(packetReceive);
				break;
			default:
				break;
			}
		}
	}
}

void NetworkHandler::receiveUDP()

{
	sf::Packet packetReceive;
	while (!m_quitUDP)

	{
		packetReceive.clear();
		sf::IpAddress sender;
		unsigned short senderPort;
		if (m_udpSocket.receive(packetReceive, sender, senderPort) == sf::Socket::Done)

		{
			unsigned char type;
			packetReceive >> type;
			//std::cout << type << std::endl;
			switch (type)

			{
			case MOVE:
				handleMove(packetReceive);
				break;
			case NPNT:
				//handleNewPointUDP(packetReceive);
				break;
			default:
				break;
			}
		}
	}
}

void NetworkHandler::sendUpdateSneks(const std::string& id, const std::vector<Point>& points)

{
	CreateGhostPacket packet(255, 0, 0, "josh");
	sf::Packet sendPacket;
	packet.m_first = false;
	sendPacket << packet;
	sendPacketTCP(sendPacket);

	for (int i = 0; i < points.size(); i++)

	{
		sendPacket.clear();
		PointPacket pointPacket(points[i].m_position.x, points[i].m_position.y, (unsigned char)points[i].m_type, points[i].m_radius);
		pointPacket.m_id = id;
		sendPacket << pointPacket;
		sendPacketTCP(sendPacket);
	}
}

void NetworkHandler::sendPos(const Vec2f& position, const Vec2f& velocity, const bool& pointsAllowed, const float& bodySize)

{
	MovePacket packet(position.x, position.y, velocity.x, velocity.y, bodySize, pointsAllowed);
	sf::Packet sendPacket;
	sendPacket << packet;
	sendPacketUDP(sendPacket);
}

void NetworkHandler::sendCreate()

{
	CreateGhostPacket packet(255, 0, 0, "josh");
	sf::Packet sendPacket;
	sendPacket << packet;
	sendPacketTCP(sendPacket);
}

void NetworkHandler::sendClear()

{
	MessagePacket packet(MCLR);
	sf::Packet sendPacket;
	sendPacket << packet;
	sendPacketTCP(sendPacket);
}

void NetworkHandler::sendPacketTCP(sf::Packet packet)

{
	//m_mtxTCP.lock();
	if (m_tcpSocket.send(packet) != sf::Socket::Done)

	{
		std::cout << "Dropped packet!" << std::endl;
		m_droppedPackets.push_back(packet);
	}
	//m_mtxTCP.unlock();
}

void NetworkHandler::sendPacketUDP(sf::Packet packet)

{
	//m_mtxUDP.lock();
	m_udpSocket.send(packet, m_ip, m_port);
	//m_mtxUDP.unlock();
}

void NetworkHandler::handleCreateGhost(sf::Packet& packet)

{
	CreateGhostPacket crea;
	std::string id;
	packet >> crea;
	packet >> id;

	if (program->getGhostIndex(id) == -1)

	{
		//std::cout << crea.m_first << " " << crea.m_ghostName << std::endl;
		//std::cout << (int)crea.m_r << " " << (int)crea.m_g << " " << (int)crea.m_b << std::endl;
		program->createGhost(crea.m_r, crea.m_g, crea.m_b, id, crea.m_ghostName, crea.m_first);
	}
}

void NetworkHandler::handleDisconnectGhost(sf::Packet& packet)

{
	std::string id;
	packet >> id;
	program->removeGhost(id);
}

void NetworkHandler::handleMove(sf::Packet& packet)

{
	std::string id;
	MovePacket move;
	packet >> move;
	packet >> id;
	program->updateGhostActualPosition(id, Vec2f(move.m_x, move.m_y), move.m_bodySize, move.m_pointsAllowed);
}

void NetworkHandler::handleNewPointUDP(sf::Packet& packet)

{
	std::string id;
	PointPacket point;
	packet >> point;
	packet >> id;
	if (point.m_id == m_myID)

	{
		program->addGhostPoint(id, Vec2f(point.m_x, point.m_y), point.m_radius, point.m_type);
	}
}

void NetworkHandler::handleClear(sf::Packet& packet)

{
	std::string id;
	packet >> id;
	program->clearGhost(id);
}

void NetworkHandler::disconnect()

{
	//mtx.lock();
	m_quitTCP = true;
	m_quitUDP = true;
	//mtx.unlock();

	m_mtxUDP.lock();
	m_udpSocket.unbind();
	m_mtxUDP.unlock();

	sf::Packet sendPacket;
	MessagePacket msg(DSCT);
	sendPacket << msg;
	sendPacketTCP(sendPacket);

	m_mtxTCP.lock();
	m_tcpSocket.disconnect();
	m_mtxTCP.unlock();

	m_receiveThreadTCP->wait();
	m_receiveThreadUDP->wait();
}