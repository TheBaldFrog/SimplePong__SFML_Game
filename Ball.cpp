#include "Ball.h"

Ball::Ball(float startX, float startY)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(17, 17));
	m_Shape.setPosition(m_Position);
}

FloatRect Ball::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Ball::getShape()
{
	return m_Shape;
}

float Ball::getXVelocity()
{
	return m_DirectionX;
}

float Ball::getSpeed()
{
	return m_Speed;
}

void Ball::reboundSides()
{
	m_DirectionX = -m_DirectionX;
}

void Ball::reboundBatOrTop()
{
	/*if (getPosition().top < 0)
	{
		m_Position.y = 0;
	}*/
	m_DirectionY = -m_DirectionY;
}

void Ball::reboundBottom()
{
	m_Position.y = 0;
	m_Position.x = 500;
	m_DirectionY = -m_DirectionY;
}

void Ball::update(Time dt)
{
	m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
	m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

	m_Shape.setPosition(m_Position);
}

void Ball::setSpeed(float speed)
{
	m_Speed = speed;
}

void Ball::setPosition(Vector2f pos)
{
	m_Position = pos;
}

void Ball::setColor(Color col)
{
	m_Shape.setFillColor(col);
}
