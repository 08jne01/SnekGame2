#pragma once
#include <SFML/Network.hpp>
struct DroppedPacket

{
	DroppedPacket(const short& index,
		const bool& me,
		const sf::Packet& packet
	) : 
		m_index(index),
		m_me(me),
		m_packet(packet)

	{

	}

	short m_index;
	bool m_me;
	sf::Packet m_packet;
};