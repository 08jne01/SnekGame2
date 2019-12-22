#pragma once
#include <Maths/Maths.h>
#include <vector>
#include "Entity.h"
struct Point

{
	enum class PointType

	{
		Snek,
	};

	Point()

	{

	}

	Point(
		const Vec2f& position,
		const float& radius,
		const PointType& type
	):
		m_position(position),
		m_radius(radius),
		m_type(type)
	{

	}

	Vec2f m_position;
	float m_radius;
	PointType m_type;
};
//TODO make rectangle work
class Boundary

{
	enum class BoundaryType

	{
		Rectangle,
		Circle
	};

public:
	///Circle type boundary
	Boundary(
		const Vec2f& position,
		const float& radius
	):
		m_position(position),
		m_type(BoundaryType::Circle),
		m_radius(radius),
		m_width(0),
		m_height(0)
	{

	}

	///Rectangle Type Boundary
	Boundary(
		const Vec2f& position,
		const float& width,
		const float& height
	) :
		m_position(position),
		m_type(BoundaryType::Rectangle),
		m_width(width),
		m_height(height),
		m_radius(0)
	{

	}

	bool contains(const Point& point) const

	{
		switch (m_type)

		{
		case BoundaryType::Circle:
			return containsRadius(point);
		case BoundaryType::Rectangle:
			return containsRectangle(point);
		default:
			return false;
		}
	}

	///Only called by rectangular type
	bool intersects(const Boundary& range) const

	{
		switch (range.getType())

		{
		case BoundaryType::Circle:
			return intersectsRadius(range);
		case BoundaryType::Rectangle:
			return intersectsRectangle(range);
		default:
			return false;
		}
	}

	inline BoundaryType getType() const

	{
		return m_type;
	}

	inline Vec2f getPos() const

	{
		return m_position;
	}

	inline float getRadius() const

	{
		return m_radius;
	}

	inline float getWidth() const

	{
		return m_width;
	}

	inline float getHeight() const

	{
		return m_height;
	}

private:

	inline bool containsRadius(const Point& point) const

	{
		return (distanceSquared(point.m_position, m_position) < m_radius * m_radius);
	}

	inline bool containsRectangle(const Point& point) const

	{
		return (
			point.m_position.x >= m_position.x - m_width / 2.0 &&
			point.m_position.x <= m_position.x + m_width / 2.0 &&
			point.m_position.y >= m_position.y - m_height / 2.0 &&
			point.m_position.y <= m_position.y + m_height / 2.0
			);
	}

	inline bool intersectsRadius(const Boundary& range) const

	{
		const Vec2f dist = vecAbs(m_position - range.getPos());

		if (dist.x > (m_width / 2.0 + range.getRadius()))
			return false;

		if (dist.y > (m_height / 2.0 + range.getRadius()))
			return false;

		if (dist.x <= (m_width / 2.0))
			return true;

		if (dist.y <= (m_height / 2.0))
			return true;

		float cornerSqr = pow(dist.x - m_width / 2.0, 2) + pow(dist.y - m_height / 2.0, 2);
		return (cornerSqr <= pow(range.getRadius(), 2));
	}

	inline bool intersectsRectangle(const Boundary& range) const

	{
		return false; //Disabled for now
	}

	const Vec2f m_position;
	const BoundaryType m_type;
	const float m_radius, m_width, m_height;
};

class QuadTreeObj

{
public:
	QuadTreeObj(const Boundary boundary, const short capacity, const short level);
	~QuadTreeObj();
	void subDivide();
	void insert(const Point& point);
	void query(std::vector<Point>& output, const Boundary& range);

private:
	QuadTreeObj* m_northwest = nullptr;
	QuadTreeObj* m_northeast = nullptr;
	QuadTreeObj* m_southwest = nullptr;
	QuadTreeObj* m_southeast = nullptr;

	const short m_capacity;
	const Boundary m_boundary;
	std::vector<Point> points;
	bool divided = false;
	short m_level = 0;
};

class QuadTree

{
public:
	QuadTree(const Vec2f& position, const int& width, const int& height, const short& capacity);
	~QuadTree();
	void insert(const Point& point);
	void query(std::vector<Point>& output, const Boundary& range);
	void clear();
private:
	QuadTreeObj* m_tree;
	const Vec2f m_rootPosition;
	const int m_width;
	const int m_height;
	const short m_capacity;
};