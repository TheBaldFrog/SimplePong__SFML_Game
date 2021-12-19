#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bat
{
private:
	Vector2f m_Position;
	
	// A retangleShape
	RectangleShape m_Shape;
	float m_Speed = 1400.0f;
	bool m_MovingRight = false;
	bool m_MovingLeft = false;
public:
	Bat(float startX, float startY);

	FloatRect getPosition();
	RectangleShape getShape();

	void moveRight();
	void moveLeft();
	void stopLeft();
	void stopRight();

	void update(Time dt);
};