#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <sstream>
#include "NetworkHandler.h"
#include "Player.h"
#include "Ghost.h"


class Player;

class Program

{
	enum class GameState

	{
		MENU,
		SINGLE,
		MULTI
	};

public:
	Program(const short& width, const short& height);
	int mainLoop();
	int mainLoopGame();
	void gameCleanUp();
	int mainLoopMenu();
	void draw();
	void update();
	void reset(); //Reset all variables required to go into any other game state
	void getSteering();
	void eventHandler(const sf::Event& events);
	void eventHandlerLoop();
	void executeArguments(const std::string& line);
	void collectArguments(std::vector<std::string>& argv,  const std::string& line);
	void parseArguments(const std::vector<std::string>& argv);
	short getGhostIndex(const std::string& id);
	void createGhost(const unsigned char& r, const unsigned char& g, const unsigned char& b, const std::string& id, const std::string& name, const bool& first);
	void updateGhostActualPosition(const std::string& id, const Vec2f& position, const float& size, const bool& allowedPoints);
	void addGhostPoint(const std::string& id, const Vec2f& position, const float& size, const unsigned char& type);
	void removeGhost(const std::string& id);
	void clearGhost(const std::string& id);
private:
	std::vector<Entity*> m_entities;
	const short m_width, m_height;
	bool shouldQuit = false;
	sf::Mutex m_mtx;
	sf::Mutex m_mtxEvents;

	sf::Thread m_eventHandlerThread;

	sf::RenderWindow window;
	sf::Context context;
	Player* m_player;
	std::vector<std::shared_ptr<Ghost>>* m_ghosts;
	std::list<sf::Event> m_eventQueue;
	GameState m_gameState;
	QuadTree m_tree;

	std::string m_ip;
	unsigned short m_port;
};