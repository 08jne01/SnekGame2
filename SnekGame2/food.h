#include <vector>
#include <SFML/Graphics.hpp>

#include "QuadTree.h"
#include "Entity.h"
class Food : public Entity

{
public:

	Food(QuadTree* tree, Point::PointType type);
	void draw(sf::RenderWindow& window);
	void preUpdate();

	QuadTree* getTree();

	//Public member variables
	Point m_point;
	Vec2f m_drawPosition;

private:

	sf::CircleShape m_circle;
	sf::Color m_color = sf::Color::Green;

	QuadTree* m_tree;
	Point::PointType m_type;
};