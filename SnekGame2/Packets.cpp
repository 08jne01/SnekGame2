#include "Packets.h"
//Generic Message Packet Operator
sf::Packet& operator<<(sf::Packet& packet, const MessagePacket& msg)

{
	return packet << msg.m_name;
}

//Move Packet
sf::Packet& operator<<(sf::Packet& packet, const MovePacket& move)

{
	return packet << move.name << move.m_x << move.m_y << move.m_velX << move.m_velY << move.m_bodySize << move.m_pointsAllowed;
}

sf::Packet& operator>>(sf::Packet& packet, MovePacket& move)

{
	return packet >> move.m_x >> move.m_y >> move.m_velX >> move.m_velY >> move.m_bodySize >> move.m_pointsAllowed;
}

//Create Ghost Packet
sf::Packet& operator<<(sf::Packet& packet, const CreateGhostPacket& create)

{
	return packet << create.name << create.m_first << create.m_r << create.m_g << create.m_b << create.m_ghostName;
}

sf::Packet& operator>>(sf::Packet& packet, CreateGhostPacket& create)

{
	return packet >> create.m_first >> create.m_r >> create.m_g >> create.m_b >> create.m_ghostName;
}

//Point Packet
sf::Packet& operator<<(sf::Packet& packet, const PointPacket& point)

{
	return packet << point.name << point.m_x << point.m_y << point.m_radius << point.m_type << point.m_id;
}

sf::Packet& operator>>(sf::Packet& packet, PointPacket& point)

{
	return packet >> point.m_x >> point.m_y >> point.m_radius >> point.m_type >> point.m_id;
}