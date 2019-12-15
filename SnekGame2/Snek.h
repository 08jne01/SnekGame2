#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "QuadTree.h"
#include "Entity.h"
class Snek : public Entity

{
public:

	enum class Type

	{
		Player,
		Ghost
	};

	Snek(QuadTree* tree, const Type type);
	void draw(sf::RenderWindow& window);
	void preUpdate();
	virtual void update()=0;
	
	QuadTree* getTree();

	//Public member variables
	std::vector<Point> points;
	Vec2f m_drawPosition;

	inline void setColor(const unsigned char& r, const unsigned char& g, const unsigned char& b)

	{
		m_color = sf::Color(r, g, b);
	}
private:
	sf::VertexArray m_drawPoints;
	sf::Color m_color = sf::Color::Green;
	
	QuadTree* m_tree;
	Type m_type;
};