#include "food.h"
#define PI 3.14159

Food::Food(
	QuadTree* tree,
	Point::PointType type
) :
	m_tree(tree),
	m_drawPosition(0),
	m_circle{5}

{
	m_point.m_type = type;
}

void Food::preUpdate()

{
	m_tree->insert(m_point);
}

void Food::draw(sf::RenderWindow& window)

{
	m_circle.setFillColor(m_color);
	m_circle.setPosition(m_drawPosition.x, m_drawPosition.y);
	window.draw(m_circle);
}

QuadTree* Food::getTree()

{
	return m_tree;
}

