#pragma once
#include <Maths/Maths.h>
#include <SFML/Network.hpp>

class NetworkHandler

{
public:

	enum class Type

	{
		Clear,
		Create,
		Ready
	};

	NetworkHandler();
	~NetworkHandler();

	//Setup
	void connect(const std::string& ip, const unsigned short& port);
	//Cleanup
	void disconnect();
	//Send
	void sendPacket(const sf::Packet& packet);
	//Send Packet Specific
	void sendPos(const Vec2f& position, const Vec2f& velocity, const bool& pointsAllowed, const float& bodySize);
	void sendEat(const short& index);
	void sendMessage(const Type& type);
	void sendUpdateSneks(const std::string& id);
	//Recieve
	void receiveUDP();
	void receiveTCP();

private:
	static NetworkHandler* instance;
};