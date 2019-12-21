#include "Ghost.h"

Ghost::Ghost(
	QuadTree* tree,
	const unsigned char& r,
	const unsigned char& g,
	const unsigned char& b,
	const std::string& id,
	const std::string& name
) :
	Snek(tree, Type::Ghost),
	m_ID(id),
	m_name(name),
	m_position(-1),
	m_prevPosition(-1)

{
	setColor(r, g, b);
}

void Ghost::update()

{
	m_prevPoints.push_back(Point(m_position, m_size, Point::PointType::Snek));

	if (m_prevPosition.x < 0 && m_prevPosition.y < 0)

	{
		m_drawPosition = m_position;
		m_prevPosition = m_position;
	}

	else
		m_drawPosition = lerp(m_drawPosition, m_position, 0.3f);

	if (distanceSquared(m_drawPosition, m_position) > 200.0)
		m_drawPosition = m_position;

	if (distanceSquared(m_drawPosition, m_prevPosition) > m_spacingSqrd && m_allowedPoints)

	{
		float distSqr = 10000.0;
		bool set = false;
		Vec2f position(0);
		std::list<Point>::iterator bestIt;
		for (std::list<Point>::iterator it = m_prevPoints.begin(); it != m_prevPoints.end(); ++it)

		{
			float newDist = distanceSquared(it->m_position, m_drawPosition);
			if (newDist < distSqr)

			{
				distSqr = newDist;
				position = it->m_position;
				bestIt = it;
				set = true;
			}
		}

		if (!set)

		{
			points.push_back(Point(m_drawPosition, m_size, Point::PointType::Snek));
			m_prevPosition = m_drawPosition;
		}
			
		else

		{
			points.push_back(Point(position, m_size, Point::PointType::Snek));
			m_prevPosition = position;
			m_prevPoints.erase(bestIt);
		}

		
	}

	if (m_prevPoints.size() > 10)
		m_prevPoints.pop_front();
}