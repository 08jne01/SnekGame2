#pragma once
#include <SFML/Network.hpp>
#define MOVE 0 //Move packet
#define CREA 1 //Create ghost on other clients
#define MCLR 2 //Clear my ghost from others
#define MYID 3 //Get my ID from server
#define UDPJ 4 //UDP join packet, make the server recognise TCP and UDP client as the same client
#define DSCT 5 //Disconnect Packet
#define NPNT 6 //New point, either for specific player (ie when they join) or for food

struct MessagePacket //Carry Generic Message Code

{
	MessagePacket(unsigned char type): m_name(type) {}
	unsigned char m_name;
};

struct MovePacket

{
	MovePacket() {}
	MovePacket(
		const float& x,
		const float& y,
		const float& velX,
		const float& velY,
		const float& bodySize,
		const bool& pointsAllowed
	):
		m_x(x),
		m_y(y),
		m_velX(velX),
		m_velY(velY),
		m_bodySize(bodySize),
		m_pointsAllowed(pointsAllowed)
	{

	}
	const unsigned char name = MOVE;
	float m_x, m_y, m_velX, m_velY, m_bodySize;
	bool m_pointsAllowed;
};

struct CreateGhostPacket //Initial Connection info like your colour, id and name

{
	CreateGhostPacket() {}
	CreateGhostPacket(
		unsigned char r,
		unsigned char g,
		unsigned char b,
		std::string ghostName
	):
		m_r(r),
		m_g(g),
		m_b(b),
		m_ghostName(ghostName)

	{

	}

	const unsigned char name = CREA;
	bool m_first = true;
	std::string m_ghostName = "unamed";
	unsigned char m_r, m_g, m_b;
};

struct PointPacket

{
	PointPacket() {}

	PointPacket(
		const float& x, 
		const float& y,
		const unsigned char& type,
		const float& radius
	):
		m_x(x),
		m_y(y),
		m_radius(radius),
		m_type(type)

	{

	}

	const unsigned char name = NPNT;
	unsigned char m_type;
	float m_x, m_y, m_radius;
	std::string m_id = "";
};

sf::Packet& operator<<(sf::Packet& packet, const MessagePacket& msg); //Only send info in since the msg is taken out of every packet to see what it is

sf::Packet& operator<<(sf::Packet& packet, const MovePacket& move);
sf::Packet& operator>>(sf::Packet& packet, MovePacket& move);

sf::Packet& operator<<(sf::Packet& packet, const CreateGhostPacket& create);
sf::Packet& operator>>(sf::Packet& packet, CreateGhostPacket& create);

sf::Packet& operator<<(sf::Packet& packet, const PointPacket& point);
sf::Packet& operator>>(sf::Packet& packet, PointPacket& point);