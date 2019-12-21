#include "Program.h"

Program::Program(
	const short& width,
	const short& height
) :
	m_width(width),
	m_height(height),
	m_tree(Vec2f(width / 2.0, height / 2.0), width, height, 1),
	m_eventHandlerThread(&Program::eventHandlerLoop, this),
	m_gameState(GameState::MENU),
	m_ip("127.0.0.1"),
	m_port(5000)
{
	
}

int Program::mainLoop()
 
{
	
	int error = 0;
	while (!shouldQuit && !error)

	{
		if (m_gameState == GameState::MENU)
			error = mainLoopMenu();
		else
			error = mainLoopGame();
	}

	

	//std::cout << NetworkHandler::getInstance() << " " << &networkHandler << std::endl;
	//window.setActive(true);
	//if (!window.isOpen())
		//return EXIT_FAILURE;
	/*m_gameState = GameState::MENU;
	mainLoopMenu();
	m_gameState = GameState::MULTI;
	std::cout << "Sleep 5" << std::endl;
	sf::sleep(sf::seconds(5));
	mainLoopGame();*/
	
	//Hacky way around a dumb crash caused by SFML deleting our window twice.
	window.create(sf::VideoMode(0, 0), "");
	return error;
}

int Program::mainLoopMenu()

{
	std::string line;
	//std::cout << "Snek Game Console> ";
	//std::getline(std::cin, line);
	
	while (m_gameState == GameState::MENU && !shouldQuit)

	{
		std::cout << "Snek Game Console> ";
		std::cin.clear();
		std::getline(std::cin, line);
		executeArguments(line);
	}
	
	return EXIT_SUCCESS;
}

int Program::mainLoopGame()

{
	static Player player = Player(Vec2f(m_width / 2.0, m_height / 2.0), &m_tree, m_width, m_height);
	m_player = &player;
	m_player->reset();
	m_entities.push_back(m_player);

	static std::vector<std::shared_ptr<Ghost>> ghosts;
	m_ghosts = &ghosts;


	static sf::Event events;
	static sf::Clock updateClock;
	static float updateTime = 1.0 / 60.0;

	static NetworkHandler networkHandler(m_mtx, this);
	GameState stateWhenStarted = m_gameState;
	if (m_gameState == GameState::MULTI)

	{
		if (!networkHandler.connect(m_ip, m_port))

		{
			m_gameState = GameState::MENU;
			gameCleanUp();
			return EXIT_SUCCESS;
		}
			
	}
		


	m_eventHandlerThread.launch();
	sf::sleep(sf::seconds(1));
	window.requestFocus();

	while (m_gameState != GameState::MENU)

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

	if (stateWhenStarted == GameState::MULTI)

	{
		NetworkHandler::getInstance()->disconnect();
		NetworkHandler::getInstance()->reset();
	}
		
	m_eventHandlerThread.wait();
	gameCleanUp();
	return EXIT_SUCCESS;
}

void Program::gameCleanUp()

{
	m_mtx.lock();
	m_entities.clear();
	m_ghosts->clear();
	m_eventQueue.clear();
	m_player->points.clear();
	m_tree.clear();
	
	m_mtx.unlock();
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

void Program::executeArguments(const std::string& line)

{
	std::vector<std::string> argv;
	for (int i = 0; i < argv.size(); i++)

	{
		std::cout << argv[i] << std::endl;
	}

	collectArguments(argv, line);
	parseArguments(argv);
}

void Program::collectArguments(std::vector<std::string>& argv, const std::string& line)

{
	std::string word = "";
	std::string linePlusNewline = line + '\n';
	for (int i = 0; i < linePlusNewline.size(); i++)

	{
		char c = linePlusNewline[i];
		if (c == ' ' || c == '\n')

		{
			if (word != "")

			{
				argv.push_back(word);
				word = "";
			}
				
		}

		else
			word += c;
	}
}

void Program::parseArguments(const std::vector<std::string>& argv)

{
	if (!argv.size())
		return;

	short size = argv.size();

	if (argv[0] == "launch")

	{
		if (size > 1 && (argv[1] == "multiplayer" || argv[1] == "m"))

		{
			if (size != 3)

			{
				std::cout << "Please supply ip:port" << std::endl;
				return;
			}

			m_gameState = GameState::MULTI;
			std::string ipport = argv[2];
			bool isPort = false;
			std::string ip;
			std::string port;
			for (int i = 0; i < ipport.size(); i++)

			{
				char c = ipport[i];
				if (c == ':')
					isPort = true;
				else

				{
					if (isPort)
						port += c;
					else
						ip += c;
				}
			}

			std::stringstream os;
			os << port;
			short numPort;
			os >> numPort;
			m_ip = ip;
			m_port = numPort;
		}
			
		else if (size > 1 && (argv[1] == "singleplayer" || argv[1] == "s"))
			m_gameState = GameState::SINGLE;
	}

	else if (argv[0] == "quit")
		shouldQuit = true;
	else if (argv[0] == "help")

	{
		std::cout << "launch <mode> (ip:port) - (singleplayer alias s or multiplayer alias m)" << std::endl;
		std::cout << "quit - quits the program" << std::endl;
		std::cout << "help - brings up this menu" << std::endl;
	}
	else
		std::cout << "Unknown command -> help for possible commands" << std::endl;
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
		m_gameState = GameState::MENU;
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