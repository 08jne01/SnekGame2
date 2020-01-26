#pragma once
#include <random>
#include "Snek.h"
#include "NetworkHandler.h"

#define PI 3.14159

class Program;

class Player : public Snek

{
public:
	Player();
	Player(const Vec2f& position, QuadTree* tree, short width, short height);
	void setSteering(const short& steering);
	void update();
	void snekEventHandler();
	void checkCollision();
	void foodEaten(const Point::PointType type);
	void resetSnek();
	void speedSnek(float multiplier);
	void checkSpeedSnek(Point::PointType type);
	void reset();
	inline Vec2f getPosition()
	{
		return m_position;
	}
	inline short getWidth()
	{
		return m_width;
	}
	inline short getHeight()
	{
		return m_height;
	}
	inline Vec2f getVelocity()
	{
		return m_velocity;
	}
	void getObstacles(const float radius, std::vector<Point>& points);
	inline void steer();
private:
	Vec2f m_position, m_velocity, m_prevPosition, m_prevPositionIncludingHoles;
	short m_steering = 0;

	//Stuff to do with gaps
	float m_distHoles = 0.0;
	float m_randomDistance = 0.0;
	float m_randomTime = 0.0;
	sf::Clock m_spacer;
	sf::Clock frameTime;

	//Constant config values
	const float m_steerVelocity = 0.08;
	const float m_speed = 2.0;
	const float m_spacingSqrd = 16.0;

	//Non constant config values
	float m_size = 3.0;
	bool m_allowedPoints = true;
	bool m_alive = true;
	short m_width, m_height;

	//Snek Attributes
	bool m_noGaps = false;
	float speedFactor;
	float speedEffectTime = 3.0;
	unsigned int m_effects[3] = { 0,0,0 }; // snek, ..., ...

	//effects clocks
	sf::Clock speedClock;


	//Random Number gen
	std::mt19937 generator;
};