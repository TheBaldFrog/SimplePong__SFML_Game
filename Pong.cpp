#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include "SFML/Graphics.hpp"
#include <chrono>
#include <thread>
#include "SFML/Audio.hpp"
#include <vector>
#include <random>
#include "FallingSnow.h"

int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Pong", Style::Fullscreen);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(120);

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

	Text activeBot;
	activeBot.setFont(font);
	activeBot.setCharacterSize(66);
	activeBot.setString("Press 'B' for Bot");
	activeBot.setPosition(Vector2f(1920 - activeBot.getGlobalBounds().width - 20, 33));
	bool botActive = true;

	bool isDead = false;
	bool intersect = false;
	bool botVision = false; // false

	int aceleration = 350;

	

	Color col[21]{
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
		{215, 38, 49},
		{162, 213, 198},
		{92, 60, 146},
		{18, 164, 217},
		{64, 104, 130},
		{81, 146, 89},
		{255,255,255}
	};
	int colCont = 0;

	bool acceptInput = true;

	SoundBuffer bufferTheme;
	bufferTheme.loadFromFile("sound/Retro-Arcade-Game-[AudioTrimmer.com].ogg");
	Sound theme(bufferTheme);
	//theme.getPlayingOffset(sf::seconds(2.f));
	theme.setPlayingOffset(sf::seconds(5.f));
	theme.setVolume(70.f);
	theme.setLoop(true);
	theme.play();

	SoundBuffer bufferHit;
	bufferHit.loadFromFile("sound/mixkit-small-hit-in-a-game-2072.wav");
	Sound soundHit(bufferHit);

	SoundBuffer bufferNotification;
	bufferNotification.loadFromFile("sound/mixkit-unlock-game-notification-253.wav");
	Sound soundNotification(bufferNotification);
	bool isSoundNotification = true;

	SoundBuffer bufferFail;
	bufferFail.loadFromFile("sound/404743__owlstorm__retro-video-game-sfx-fail.wav");
	Sound soundFail(bufferFail);

	SoundBuffer bufferLose;
	bufferLose.loadFromFile("sound/350984__cabled-mess__lose-c-03.wav");
	Sound soundLose(bufferLose);
	
	// Snow falling
	unsigned int elapsed = 0;		// Elapsed time since last snowflake spawn
	srand((unsigned int)time(0));
	unsigned int numOfSnowflakes = 300;
	unsigned int delay = 10;
	std::vector<sf::CircleShape> snowflakes;
	snowflakes.reserve(numOfSnowflakes);
	// end snow falling

	// top white shape
	RectangleShape UpShape;
	UpShape.setSize(Vector2f(window.getSize().x, 2));
	//UpShape.setPosition(Vector2f(200, 5));
	UpShape.setFillColor(Color::White);

	
	// Clock for timing everything
	Clock clock;
	while (window.isOpen())
	{
		elapsed++;

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

				// boot
				botActive = false;
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
				theme.stop();
				soundLose.play();

				std::chrono::seconds dura(2);
				std::this_thread::sleep_for(dura);
				window.close();
			}

			ball.setSpeed(600);
			isDead = true;

			soundFail.play();
		}

		// Handle ball hitting top
		if (ball.getPosition().top < 0)
		{
			ball.setPosition(Vector2f(ball.getPosition().left, 0));
			ball.reboundBatOrTop();
			if (!isDead)
			{
				score++;
				if (colCont == 21)
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
				if (ball.getSpeed() > 3650)
					aceleration = 250;
				else
					aceleration = 350;
				if (ball.getSpeed() > 6000)
					aceleration = 150;
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
				soundHit.play();
			}
		}
		else
		{
			if (intersect)
			{
				intersect = false;
			}
		}

		if (elapsed >= delay && snowflakes.size() < numOfSnowflakes)
		{
			createFlake(snowflakes);
			elapsed = 0;
		}

		// Draw the bat, the ball and the HUD
		window.clear();

		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.draw(UpShape);

		// Active bot
		if (score >= 8 && botActive)
		{
			//botActive = false;
			if (isSoundNotification)
			{
				isSoundNotification = false;
				soundNotification.play();
			}
			window.draw(activeBot);

		}

		for (unsigned int i = 0; i < snowflakes.size(); i++)
		{
			snowflakes[i].move(0.0f, snowflakes[i].getRadius() * 0.4f);
			window.draw(snowflakes[i]);

			// Don't draw snowflakes that are outside of the window
			if (snowflakes[i].getPosition().y >= window.getSize().y)
			{
				snowflakes.erase(snowflakes.begin() + i);
			}
		}

		window.display();
	}
}