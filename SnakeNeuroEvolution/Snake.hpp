#pragma once

#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include <vector>
#include "DEFINITIONS.hpp"
#include "Matrix.h"

namespace engine {

	class Snake
	{
	public:
		Snake(gameDataRef data);

		void update();

		bool ateDot(sf::Vector2f dot);

		bool hitDot(sf::Vector2f dot);

		void move(Dir dir);

		void draw();

		bool dead() { return _dead; }

		//AI//////////////////////////
		Matrix<float> getInputs(sf::Vector2f dot);
		/////////////////////////////

	private:
		gameDataRef _data;

		std::vector<sf::RectangleShape> _snake;

		float _speed = TILESIZE;

		sf::Vector2f _moveVector;

		Dir _dir;

		sf::Vector2f previousPos1;
		sf::Vector2f previousPos2;

		bool _dead = false;

	};

}

