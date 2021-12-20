#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include "SFML/Graphics.hpp"
#include <chrono>
#include <thread>

int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Pong", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(90);

	int score = 0;
	int lives = 3;

	Bat bat(1920 / 2, 1080 - 20);
	Ball ball(1920 / 2, 0);
	// HUD
	Text hud;
	Font font;
	font.loadFromFile("fonts/DS-DIGIT.ttf"); // cool retro font
	hud.setFont(font);

	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	bool isDead = false;
	bool intersect = false;
	bool botVision = false; // false

	int aceleration = 350;

	Color col[15]{
		{255,0,0},
		{0,255,0},
		{0,0,255},
		{255,255,0},
		{0,255,255},
		{255,0,255},
		{192,192,192},
		{128,128,128},
		{128,0,0},
		{128,128,0},
		{0,128,0},
		{128,0,128},
		{0,128,128},
		{0,0,128},
		{255,255,255}
	};
	int colCont = 0;

	bool acceptInput = true;
	
	// Clock for timing everything
	Clock clock;
	while (window.isOpen())
	{
		// Handle the player input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyReleased)
			{
				acceptInput = true;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			bat.moveLeft();
		}
		else
		{
			bat.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			bat.moveRight();
		}
		else
		{
			bat.stopRight();
		}

		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::B))
			{
				//ball.setSpeed(0);
				if (botVision)
					botVision = false;
				else
					botVision = true;
				acceptInput = false;
			}
		}

		if (botVision)
		{
			bat.setPosition(Vector2f(ball.getPosition().left - bat.getPosition().width / 2 + ball.getPosition().width/2
				, bat.getPosition().top));
		}

		// Update the bat and the HUD
		Time dt = clock.restart(); // Update the delta time
		bat.update(dt);
		ball.update(dt);
		// Update the HUD
		std::stringstream ss;
		ss << "Score:" << score << "  Lives:" << lives << " Speed: " << ball.getSpeed();
		hud.setString(ss.str());

		// Handle ball hitting the bottom
		if (ball.getPosition().top > window.getSize().y)
		{
			// reverse the ball direction
			ball.reboundBottom();
			// Remove life
			lives--;

			// Check for zero lives
			if (lives < 1)
			{
				score = 0;
				std::chrono::seconds dura(2);
				std::this_thread::sleep_for(dura);
				window.close();
			}

			ball.setSpeed(600);
			isDead = true;
		}

		// Handle ball hitting top
		if (ball.getPosition().top < 0)
		{
			ball.setPosition(Vector2f(ball.getPosition().left, 0));
			ball.reboundBatOrTop();
			if (!isDead)
			{
				score++;
				if (colCont == 15)
				{
					colCont = 0;
				}
				ball.setColor(col[colCont]);
				colCont++;
			}
			else
			{
				isDead = false;
			}
		}

		// Handle ball hitting sides
		if (ball.getPosition().left < 0 ||
			ball.getPosition().left + ball.getPosition().width > window.getSize().x)
		{
			ball.reboundSides();
		}

		/*if (ball.getPosition().left < 0)
		{
			ball.setPosition(Vector2f(0, ball.getPosition().top));
			ball.reboundSides();
		}
		if (ball.getPosition().left + ball.getPosition().width > window.getSize().x)
		{
			ball.setPosition(Vector2f(window.getSize().x, ball.getPosition().top));
			ball.reboundSides();
		}*/

		if (ball.getPosition().intersects(bat.getPosition()))
		{
			ball.reboundBatOrTop();
			if (!intersect)
			{
				if (ball.getSpeed() > 6000)
					aceleration = 100;
				if (ball.getSpeed() > 10000)
					aceleration = 50;
				if (ball.getSpeed() > 11000)
					aceleration = 0;
				/*if (ball.getSpeed() > 15000)
					aceleration = 20;
				if (ball.getSpeed() > 18000)
					aceleration = 10;
				if (ball.getSpeed() > 20000)
					aceleration = 1;
				if (ball.getSpeed() > 21000)
					aceleration = 0;*/

				ball.setSpeed(ball.getSpeed() + aceleration);
				intersect = true;
			}
		}
		else
		{
			if (intersect)
			{
				intersect = false;
			}
		}

		// Draw the bat, the ball and the HUD
		window.clear();

		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());

		window.display();
	}
}