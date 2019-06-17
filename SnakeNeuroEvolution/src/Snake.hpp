#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>
#include "DEFINITIONS.hpp"
#include "Matrix.hpp"

namespace Game {

	class Snake
	{
	public:
		Snake(gameDataRef data);

		void update();

		bool ateDot(sf::Vector2f dot);

		bool hitDot(sf::Vector2f dot);

		void move(Dir dir);

		void draw();

		bool isDead() { return dead; }

		//AI//////////////////////////
		Matrix<float> getInputs(sf::Vector2f dot);
		/////////////////////////////

	private:
		gameDataRef data;

		std::vector<sf::RectangleShape> snake;

		float speed = TILESIZE;

		sf::Vector2f moveVector;

		Dir dir = Dir::Up;

		sf::Vector2f previousPos1;
		sf::Vector2f previousPos2;

		bool dead = false;

	};

}

