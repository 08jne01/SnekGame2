#include "Program.h"

Program::Program(
	const short& width,
	const short& height
) :
	m_width(width),
	m_height(height),
	m_tree(Vec2f(width / 2.0, height / 2.0), width, height, 1),
	m_player(Vec2f(width/2.0, height/2.0), m_tree)
	

{
	m_entities.push_back(&m_player);
}

Program::~Program()

{

}

int Program::mainLoop()

{

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;
	window.create(sf::VideoMode(m_width, m_height), "Sneki Boi 2", sf::Style::Default, settings);

	if (!window.isOpen())
		return EXIT_FAILURE;

	sf::Event events;
	sf::Clock updateClock;

	float updateTime = 1.0 / 60.0;

	while (window.isOpen())

	{
		while (window.pollEvent(events))
			eventHandler(events);

		if (updateClock.getElapsedTime().asSeconds() >= updateTime)

		{
			update();
			updateClock.restart();
		}

		window.clear();
		draw();
		window.display();
	}

	return EXIT_SUCCESS;
}

void Program::draw()

{
	for (int i = 0; i < m_entities.size(); i++)

	{
		m_entities[i]->draw(window);
	}
}

void Program::update()

{
	getSteering();
	m_tree.clear();
	for (int i = 0; i < m_entities.size(); i++)

	{
		m_entities[i]->preUpdate();
	}

	for (int i = 0; i < m_entities.size(); i++)

	{
		m_entities[i]->update();
	}
}

void Program::getSteering()

{
	short steer = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		steer--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		steer++;
	m_player.setSteering(steer);
}

void Program::eventHandler(const sf::Event& events)

{
	if (events.type == sf::Event::EventType::Closed)
		window.close();
}

void Program::reset()

{

}