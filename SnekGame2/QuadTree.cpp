#include "QuadTree.h"

QuadTreeObj::QuadTreeObj(
	const Boundary boundary,
	const short capacity
):
	m_boundary(boundary),
	m_capacity(capacity)
{

}

QuadTreeObj::~QuadTreeObj()

{
	if (m_northwest)
		delete m_northwest;
	if (m_northeast)
		delete m_northeast;
	if (m_southwest)
		delete m_southwest;
	if (m_southeast)
		delete m_southeast;
}

void QuadTreeObj::subDivide()

{
	float x = m_boundary.getPos().x;
	float y = m_boundary.getPos().y;
	float w = m_boundary.getWidth();
	float h = m_boundary.getHeight();

	float w2 = w / 2.0;
	float h2 = h / 2.0;
	float w4 = w / 4.0;
	float h4 = h / 4.0;

	Boundary nw = Boundary(Vec2f(x - w4, y - h4), w2, h2);
	Boundary ne = Boundary(Vec2f(x + w4, y - h4), w2, h2);
	Boundary sw = Boundary(Vec2f(x - w4, y + h4), w2, h2);
	Boundary se = Boundary(Vec2f(x + w4, y + h4), w2, h2);

	m_northwest = new QuadTreeObj(nw, m_capacity);
	m_northeast = new QuadTreeObj(ne, m_capacity);
	m_southwest = new QuadTreeObj(sw, m_capacity);
	m_southeast = new QuadTreeObj(se, m_capacity);

	for (int i = 0; i < points.size(); i++)

	{
		m_northwest->insert(points[i]);
		m_northeast->insert(points[i]);
		m_southwest->insert(points[i]);
		m_southeast->insert(points[i]);
	}
	points.clear();
	divided = true;
}

void QuadTreeObj::insert(const Point& point)

{
	if (!m_boundary.contains(point))
		return;

	if (!divided)

	{
		if (points.size() < m_capacity)
			points.push_back(point);
		else
			subDivide();
	}

	if (divided)

	{
		m_northwest->insert(point);
		m_northeast->insert(point);
		m_southwest->insert(point);
		m_southeast->insert(point);
	}
}

void QuadTreeObj::query(std::vector<Point>& output, const Boundary& range)

{
	if (!m_boundary.intersects(range))
		return;
	else if (!divided)

	{
		for (int i = 0; i < points.size(); i++)

		{
			if (range.contains(points[i]))
				output.push_back(points[i]);
		}
	}

	else

	{
		m_northwest->query(output, range);
		m_northeast->query(output, range);
		m_southwest->query(output, range);
		m_southeast->query(output, range);
	}
}

QuadTree::QuadTree(
	const Vec2f& position,
	const int& width,
	const int& height,
	const short& capacity
):
	m_rootPosition(position),
	m_width(width),
	m_height(height),
	m_capacity(capacity)

{
	m_tree = new QuadTreeObj(Boundary(position, width, height), capacity);
}

QuadTree::~QuadTree()

{
	if (m_tree)
		delete m_tree;
}

void QuadTree::insert(const Point& point)

{
	m_tree->insert(point);
}

void QuadTree::query(std::vector<Point>& output, const Boundary& range)

{
	m_tree->query(output, range);
}

void QuadTree::clear()

{
	if (m_tree)

	{
		delete m_tree;
		m_tree = nullptr;
	}
		
		

	if (!m_tree)
		m_tree = new QuadTreeObj(Boundary(m_rootPosition, m_width, m_height), m_capacity);
}




