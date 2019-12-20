#include "Player.h"

Player::Player(
	const Vec2f& position,
	QuadTree* tree,
	short width,
	short height
) :
	Snek(tree, Type::Player),
	m_position(position),
	m_prevPosition(0),
	m_velocity(2,0),
	generator(clock()),
	m_width(width),
	m_height(height)

{
	reset();
}

void Player::reset()

{
	points.clear();
	m_alive = true;
	std::uniform_int_distribution<int> distribution(0, 360);

	float angle = PI * (float)distribution(generator) / 180.0;
	float distance = 400;
	m_position.x = distance * cos(angle) + 500;
	m_position.y = distance * sin(angle) + 500;

	Vec2f direction = normalize(Vec2f(500, 500) - m_position);
	m_velocity = direction * 2.0;
}

void Player::update()

{
	if (!m_alive)
		return;

	float dt = 60.0*frameTime.getElapsedTime().asSeconds();
	frameTime.restart();
	checkCollision();

	steer();
	m_position += m_velocity*dt;

	NetworkHandler::getInstance()->sendPos(m_position, m_velocity, m_allowedPoints, m_size);

	if (distanceSquared(m_position, m_prevPosition) > m_spacingSqrd && m_allowedPoints)

	{
		points.push_back(Point(m_position, m_size, Point::PointType::Snek));
		m_prevPosition = m_position;
	}

	if (m_spacer.getElapsedTime().asSeconds() > m_randomTime)

	{
		m_spacer.restart();
		m_distHoles = 0.0;
		m_randomTime = randomf(0.6, 1.6);
		m_randomDistance = randomi(3 * m_size + 9, 3 * m_size + 25);
	}

	if (m_distHoles < m_randomDistance)

	{
		m_spacer.restart();
		m_allowedPoints = false;
	}

	else

	{
		m_allowedPoints = true;
	}

	m_distHoles += distance(m_position, m_prevPositionIncludingHoles);

	if (m_distHoles)

	m_prevPositionIncludingHoles = m_position;
	m_drawPosition = m_position;
}

void Player::checkCollision()

{
	if (!(m_position.x >= 0 && m_position.x < m_width && m_position.y >= 0 && m_position.y < m_height))

	{
		m_alive = false;
		return;
	}

	std::vector<Point> output;
	getTree()->query(output, Boundary(m_position, m_size));
	//std::cout << output.size() << std::endl;
	for (int i = 0; i < output.size(); i++)

	{
		float radiusSqr = pow(m_size + output[i].m_radius,2);
		if (distanceSquared(output[i].m_position, m_position) < radiusSqr)

		{
			switch (output[i].m_type)

			{
			case Point::PointType::Snek:
				{
					m_alive = false;
					return;
				}
			}
		}
	}
}

void Player::setSteering(const short& steering)

{
	m_steering = steering;
}

inline void Player::steer()

{
	m_velocity.rotateInPlace(m_steering * m_steerVelocity);
	m_velocity.setMagnitude(2.0);
}