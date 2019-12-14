#pragma once
#include "Snek.h"
class Player : public Snek

{
public:
	Player(const Vec2f& position, QuadTree& tree);
	void setSteering(const short& steering);
	void update();
	void checkCollision();
	inline void steer();
private:
	Vec2f m_position, m_velocity, m_prevPosition;
	short m_steering = 0;

	//Constant config values
	const float m_steerVelocity = 0.1;
	const float m_speed = 2.0;
	const float m_spacingSqrd = 16.0;

	//Non constant config values
	float m_size = 3.0;


	bool m_alive = true;
};