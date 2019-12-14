#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "QuadTree.h"
#include "Entity.h"
class Snek : public Entity

{
public:
	Snek(QuadTree& tree);
	void draw(sf::RenderWindow& window);
	void preUpdate();
	virtual void update()=0;
	
	QuadTree& getTree();

	//Public member variables
	std::vector<Point> points;
	Vec2f m_drawPosition;
	
private:
	sf::VertexArray m_drawPoints;
	sf::Color m_color = sf::Color::Green;
	QuadTree& m_tree;
};