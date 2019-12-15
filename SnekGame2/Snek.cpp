#include "Snek.h"
#define PI 3.14159
Snek::Snek(
	QuadTree* tree,
	const Type type
): 
	m_tree(tree),
	m_drawPosition(0),
	m_type(type)

{
	m_drawPoints.setPrimitiveType(sf::Quads);
}

void Snek::preUpdate()

{
	short offset = 0;
	if (m_type == Type::Player)
		offset = 10;

	for (int i = 0; i < points.size() - offset && points.size() >= offset; i++)

	{
		m_tree->insert(points[i]);
	}
}

void Snek::draw(sf::RenderWindow& window)

{
	sf::CircleShape circle(5);
	circle.setFillColor(m_color);
	m_drawPoints.clear();
	Vec2f tl, bl;
	Vec2f horizontal = Vec2f(-1, 0);
	bool wait = false;

	for (int i = 0; i < points.size(); i++)

	{
		if (i > 0)

		{
			Vec2f pos = points[i].m_position;
			Vec2f prev = points[i - 1].m_position;
			Vec2f diff = pos - prev;

			if (diff.magnitudeInPlace() < 10)

			{
				float angle = atan2(diff.y , diff.x);
				float r = points[i].m_radius;
				diff.rotateInPlace(-angle);
				
				Vec2f tr = Vec2f(diff.x, -r);
				Vec2f br = Vec2f(diff.x, r);
				tr.rotateInPlace(angle);
				br.rotateInPlace(angle);
				tr += prev;
				br += prev;

				if (i == 1 || wait)

				{
					tl = Vec2f(0, -r);
					bl = Vec2f(0, r);
					tl.rotateInPlace(angle);
					bl.rotateInPlace(angle);
					tl += prev;
					bl += prev;
				}

				m_drawPoints.append(sf::Vertex(sf::Vector2f(tl.x, tl.y), m_color));
				m_drawPoints.append(sf::Vertex(sf::Vector2f(tr.x, tr.y), m_color));
				m_drawPoints.append(sf::Vertex(sf::Vector2f(br.x, br.y), m_color));
				m_drawPoints.append(sf::Vertex(sf::Vector2f(bl.x, bl.y), m_color));

				tl = tr;
				bl = br;
				wait = false;
			}

			else

			{
				if (points.size() > 1)

				{
					wait = true;
					circle.setRadius(points[i].m_radius);
					circle.setOrigin(circle.getGlobalBounds().width / 2.0, circle.getGlobalBounds().height / 2.0);
					circle.setPosition(pos.x, pos.y);
					window.draw(circle);

					circle.setRadius(points[i - 1].m_radius);
					circle.setOrigin(circle.getGlobalBounds().width / 2.0, circle.getGlobalBounds().height / 2.0);
					circle.setPosition(prev.x, prev.y);
					window.draw(circle);
				}
			}
		}

		if (i == points.size() - 1 || i == 0 && points.size() > 0)

		{
			circle.setRadius(points[i].m_radius);
			circle.setOrigin(circle.getGlobalBounds().width / 2.0, circle.getGlobalBounds().height/2.0);
			circle.setPosition(points[i].m_position.x, points[i].m_position.y);
			window.draw(circle);
		}
	}

	window.draw(m_drawPoints);

	if (points.size() > 0)

	{
		circle.setPosition(m_drawPosition.x, m_drawPosition.y);
		circle.setRadius(points.back().m_radius);
		circle.setOrigin(circle.getGlobalBounds().width / 2.0, circle.getGlobalBounds().height / 2.0);
		circle.setFillColor(sf::Color::White);
		window.draw(circle);
	}
}

QuadTree* Snek::getTree()

{
	return m_tree;
}

