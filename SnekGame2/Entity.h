#pragma once
namespace sf

{
	class RenderWindow;
}

class Entity

{
public:
	virtual void draw(sf::RenderWindow& window)=0;
	virtual void preUpdate()=0;
	virtual void update()=0;
private:
};