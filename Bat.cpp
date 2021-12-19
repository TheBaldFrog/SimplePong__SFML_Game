#include "Bat.h"

Bat::Bat(float startX, float startY)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(340, 5));
	m_Shape.setPosition(m_Position);
	m_Shape.setFillColor(Color::White);
}



FloatRect Bat::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Bat::getShape()
{
	return m_Shape;
}


void Bat::moveLeft()
{
	m_MovingLeft = true;
}

void Bat::moveRight()
{
	m_MovingRight = true;
}

void Bat::stopLeft()
{
	m_MovingLeft = false;
}

void Bat::stopRight()
{
	m_MovingRight = false;
}

void Bat::update(Time dt)
{
	if (m_MovingLeft)
	{
		m_Position.x -= m_Speed * dt.asSeconds();

		if (m_Shape.getGlobalBounds().left + m_Shape.getGlobalBounds().width < 0)
		{
			m_Position.x = 1920;
		}
	}
	

	if (m_MovingRight)
	{
		m_Position.x += m_Speed * dt.asSeconds();

		if (m_Shape.getGlobalBounds().left > 1920)
		{
			m_Position.x = 0;
		}
	}

	m_Shape.setPosition(m_Position);
}