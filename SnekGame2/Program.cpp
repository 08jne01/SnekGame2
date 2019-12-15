#include "Program.h"

Program::Program(
	const short& width,
	const short& height
) :
	m_width(width),
	m_height(height),
	m_tree(Vec2f(width / 2.0, height / 2.0), width, height, 1),
	m_eventHandlerThread(&Program::eventHandlerLoop, this)
{
	
}

int Program::mainLoop()
 
{
	Player player = Player(Vec2f(m_width / 2.0, m_height / 2.0), &m_tree, m_width, m_height);
	m_player = &player;
	m_entities.push_back(m_player);

	std::vector<std::shared_ptr<Ghost>> ghosts;
	m_ghosts = &ghosts;

	NetworkHandler networkHandler(m_mtx, this);
	networkHandler.connect("127.0.0.1", 5000);

	

	//std::cout << NetworkHandler::getInstance() << " " << &networkHandler << std::endl;

	

	sf::Event events;
	sf::Clock updateClock;

	float updateTime = 1.0 / 60.0;

	
	m_eventHandlerThread.launch();
	sf::sleep(sf::seconds(1));
	//window.setActive(true);
	if (!window.isOpen())
		return EXIT_FAILURE;
	while (window.isOpen())

	{
		//window.pollEvent(events);
		//eventHandler(events);

		if (updateClock.getElapsedTime().asSeconds() >= updateTime)

		{
			update();
			updateClock.restart();
		}

		window.clear();
		draw();
		window.display();
	}
	networkHandler.disconnect();
	m_eventHandlerThread.wait();
	
	//Hacky way around a dumb crash caused by SFML deleting our window twice.
	window.create(sf::VideoMode(0, 0), "");

	return EXIT_SUCCESS;
}

void Program::draw()

{
	m_mtx.lock();
	for (int i = 0; i < m_entities.size(); i++)

	{
		
		m_entities[i]->draw(window);
		
	}
	m_mtx.unlock();
}

void Program::update()

{
	getSteering();
	m_tree.clear();
	m_mtx.lock();
	for (int i = 0; i < m_entities.size(); i++)

	{
 		m_entities[i]->preUpdate();
	}
	m_mtx.unlock();
	m_mtx.lock();
	for (int i = 0; i < m_entities.size(); i++)

	{
		m_entities[i]->update();
	}
	m_mtx.unlock();
}

void Program::getSteering()

{
	short steer = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		steer--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		steer++;
	m_player->setSteering(steer);
}

short Program::getGhostIndex(const std::string& id)

{
	for (int i = 0; i < m_ghosts->size(); i++)

	{
		if ((*m_ghosts)[i]->getID() == id)
			return i;
	}

	return -1;
}

void Program::createGhost(const unsigned char& r, const unsigned char& g, const unsigned char& b, const std::string& id, const std::string& name, const bool& first)

{
	std::cout << "New player connected!" << std::endl;
	m_ghosts->push_back(std::shared_ptr<Ghost>(new Ghost(&m_tree, r, g, b, id, name)));
	m_entities.push_back(m_ghosts->back().get());
	if (first)

	{
		NetworkHandler::getInstance()->sendUpdateSneks(id, m_player->points);
	}
		
}

void Program::updateGhostActualPosition(const std::string& id, const Vec2f& position, const float& size, const bool& allowedPoints)

{
	short index = getGhostIndex(id);
	if (index != -1)

	{
		(*m_ghosts)[index]->setPosition(position);
		(*m_ghosts)[index]->setAllowedPoints(allowedPoints);
		(*m_ghosts)[index]->setSize(size);
	}
}

void Program::addGhostPoint(const std::string& id, const Vec2f& position, const float& size, const unsigned char& type)

{
	//std::cout << id << std::endl;
	short index = getGhostIndex(id);
	if (index != -1)

	{
		(*m_ghosts)[index]->points.push_back(Point(position, size, (Point::PointType)type));
	}

	//std::cout << "add" << std::endl;
}

void Program::removeGhost(const std::string& id)

{
	short index = getGhostIndex(id);
	
	//Ghost* ptrGhost = nullptr;
	if (index != -1)

	{
		Ghost* ptrGhost = (*m_ghosts)[index].get();
		m_ghosts->erase(m_ghosts->begin() + index);
		for (int i = 0; i < m_entities.size(); i++)

		{
			if (ptrGhost == m_entities[i])

			{
				m_entities.erase(m_entities.begin() + i);
				break;
			}
		}
	}
}

void Program::clearGhost(const std::string& id)

{
	short index = getGhostIndex(id);
	(*m_ghosts)[index]->points.clear();
}

void Program::eventHandlerLoop()

{
	sf::Context context;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;
	window.create(sf::VideoMode(m_width, m_height), "Sneki Boi 2", sf::Style::Default, settings);
	window.setActive(false);
	sf::Event events;
	while (window.isOpen())

	{
		m_mtxEvents.lock();
		window.pollEvent(events);
		eventHandler(events);
		m_mtxEvents.unlock();
	}
	sf::sleep(sf::seconds(0.5));
}

void Program::eventHandler(const sf::Event& events)

{
	if (events.type == sf::Event::EventType::Closed)

	{
		m_mtx.lock();
		window.close();
		m_mtx.unlock();
	}

	if (events.type == sf::Event::EventType::KeyPressed)

	{
		if (events.key.code == sf::Keyboard::Space)

		{
			reset();
		}
	}
		
}

void Program::reset()

{
	NetworkHandler::getInstance()->sendClear();
	m_mtx.lock();
	m_player->reset();
	m_mtx.unlock();
}