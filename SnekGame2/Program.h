#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Player.h"
#include "Entity.h"

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
	~Program();
	int mainLoop();
	void draw();
	void update();
	void reset(); //Reset all variables required to go into any other game state
	void getSteering();
	void eventHandler(const sf::Event& events);

private:
	std::vector<Entity*> m_entities;
	const short m_width, m_height;
	bool shouldQuit = false;
	sf::Mutex m_mtx;
	sf::RenderWindow window;

	Player m_player;
	QuadTree m_tree;
};