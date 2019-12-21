#pragma once
#include <list>
#include "Snek.h"
class Ghost : public Snek

{
public:
	Ghost(QuadTree* tree, const unsigned char& r, const unsigned char& g, const unsigned char& b, const std::string& id, const std::string& name);

	inline std::string getID() const

	{
		return m_ID;
	}

	inline void setPosition(const Vec2f& position)

	{
		m_position = position;
	}

	inline void setAllowedPoints(const bool& allowed)

	{
		m_allowedPoints = allowed;
	}

	inline void setSize(const float& size)

	{
		m_size = size;
	}

	void update();
private:
	std::string m_ID, m_name;
	Vec2f m_position, m_prevPosition;
	std::list<Point> m_prevPoints;
	bool m_allowedPoints = true;
	float m_size = 3;
	float m_spacingSqrd = 16.0;
};