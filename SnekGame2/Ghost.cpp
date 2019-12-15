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
		points.push_back(Point(m_drawPosition, m_size, Point::PointType::Snek));
		m_prevPosition = m_drawPosition;
	}
}