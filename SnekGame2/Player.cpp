#include "Player.h"

Player::Player(
	const Vec2f& position,
	QuadTree& tree
) :
	Snek(tree),
	m_position(position),
	m_prevPosition(0),
	m_velocity(2,0)

{

}

void Player::update()

{
	if (!m_alive)
		return;

	checkCollision();

	steer();
	m_position += m_velocity;

	if (distanceSquared(m_position, m_prevPosition) > m_spacingSqrd)

	{
		points.push_back(Point(m_position, m_size, Point::PointType::Snek));
		m_prevPosition = m_position;
	}

	m_drawPosition = m_position;
}

void Player::checkCollision()

{
	std::vector<Point> output;
	getTree().query(output, Boundary(m_position, m_size));
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
	m_velocity.limit(m_speed);
}