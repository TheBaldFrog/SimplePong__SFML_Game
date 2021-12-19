#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Ball
{
private:
	Vector2f m_Position;
	RectangleShape m_Shape;

	float m_Speed = 600.0f;
	float m_DirectionX = .2f;
	float m_DirectionY = .2f;

public:
	Ball(float startX, float startY);

	FloatRect getPosition();
	RectangleShape getShape();
	float getXVelocity();
	float getSpeed();
	
	void reboundSides();
	void reboundBatOrTop();
	void reboundBottom();
	void update(Time dt);
	void setSpeed(float speed);
	void setPosition(Vector2f pos);
	void setColor(Color col);
};