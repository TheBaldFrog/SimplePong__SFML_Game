#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <random>
void createFlake(std::vector<sf::CircleShape>& arr)
{
	sf::CircleShape flake(float(rand()) / (float)(RAND_MAX / 3.0f) + 0.5f);
	flake.setPosition(float(rand() % 1920), 0.0f);
	flake.setFillColor(sf::Color::White);

	arr.emplace_back(flake);
}