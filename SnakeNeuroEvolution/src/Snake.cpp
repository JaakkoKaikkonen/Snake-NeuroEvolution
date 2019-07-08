#include "Snake.hpp"
#include <iostream>

namespace Game {

	Snake::Snake(gameDataRef _data)
		: data(_data)
	{
		snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
		snake[0].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		snake[0].setFillColor(sf::Color::White);
		snake[0].setOutlineColor(sf::Color(sf::Color::Black));
		snake[0].setOutlineThickness(1.0f);

		snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
		snake[1].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TILESIZE);
		snake[1].setFillColor(sf::Color::White);
		snake[1].setOutlineColor(sf::Color(sf::Color::Black));
		snake[1].setOutlineThickness(1.0f);
	}


	void Snake::update() {

		previousPos1 = snake[0].getPosition();
		switch(dir) {
			case Dir::Up:
			snake[0].move(sf::Vector2f(0.0f, -TILESIZE));
			break;
			case Dir::Down:
			snake[0].move(sf::Vector2f(0.0f, TILESIZE));
			break;
			case Dir::Right:
			snake[0].move(sf::Vector2f(TILESIZE, 0.0f));
			break;
			case Dir::Left:
			snake[0].move(sf::Vector2f(-TILESIZE, 0.0f));
		}
		
		//Collision//////////////////
		if ((snake[0].getPosition().x < 0) || (snake[0].getPosition().x > SCREEN_WIDTH - TILESIZE) ||
			(snake[0].getPosition().y < 0) || (snake[0].getPosition().y > SCREEN_HEIGHT - TILESIZE)) {
			dead = true;
		}

		for (int i = 1; i < snake.size(); i++) {
			if (snake[0].getPosition() == snake[i].getPosition()) {
				dead = true;
			}
		}
		////////////////////////////

		if (dead) {
			snake[0].setPosition(previousPos1);
		} else {
			for (int i = 1; i < snake.size(); i++) {
				previousPos2 = snake[i].getPosition();
				snake[i].setPosition(previousPos1);
				std::swap(previousPos1, previousPos2);
			}
		}

	}

	bool Snake::ateFood(sf::Vector2f foodPos) {
		if (snake[0].getPosition() == foodPos) {
			snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
			snake[snake.size() - 1].setPosition(previousPos1);
			snake[snake.size() - 1].setFillColor(sf::Color::White);
			snake[snake.size() - 1].setOutlineColor(sf::Color(sf::Color::Black));
			snake[snake.size() - 1].setOutlineThickness(1.0f);
			return true;
		} else {
			return false;
		}
	}

	bool Snake::touchFood(sf::Vector2f foodPos) {
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].getPosition() == foodPos) {
				return true;
			}
		}
		return false;
	}

	void Snake::move(Dir dir) {

		switch(this->dir) {
			case Dir::Up:
			if (dir != Dir::Down) {
				this->dir = dir;
			}
			break;
			case Dir::Down:
			if (dir != Dir::Up) {
				this->dir = dir;
			}
			break;
			case Dir::Right:
			if (dir != Dir::Left) {
				this->dir = dir;
			}
			break;
			case Dir::Left:
			if (dir != Dir::Right) {
				this->dir = dir;
			}
		}

	}


	void Snake::draw() {
		for (int i = 0; i < snake.size(); i++) {
			data->window.draw(snake[i]);
		}
	}



	//AI///////////////////////////////////////////////////////////////////
	Matrix<float> Snake::getInputs(sf::Vector2f foodPos) {

		Matrix<float> inputs(NUM_OF_INPUTS, 1);

		//Distance to snake
		//Up
		bool found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y - i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(0, (1 / (i / TILESIZE)) * 100.0f);
				break;
			}
		}
		if (!found) {
			inputs.set(0, 1.0f);
		}
		//Down
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y + i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(1, (1 / (i / TILESIZE)) * 100.0f);
				break;
			}
		}
		if (!found) {
			inputs.set(1, 1.0f);
		}
		//Right
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(2, (1 / (i / TILESIZE)) * 100.0f);
				break;
			}
		}
		if (!found) {
			inputs.set(2, 1.0f);
		}
		//Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(3, (1 / (i / TILESIZE)) * 100.0f);
				break;
			}
		}
		if (!found) {
			inputs.set(3, 1.0f);
		}


		//distance to food
		//Up
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y - i);
			if (spot == foodPos) {
				inputs.set(4, 100.0f);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(4, 1.0f);
		}
		//Down
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y + i);
			if (spot == foodPos) {
				inputs.set(5, 100.0f);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(5, 1.0f);
		}
		//Right
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y);
			if (spot == foodPos) {
				inputs.set(6, 100.0f);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(6, 1.0f);
		}
		//Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y);
			if (spot == foodPos) {
				inputs.set(7, 100.0f);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(7, 1.0f);
		}


		//distance to wall
		//Up
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y - i);
			if (spot.y < 0) {
				inputs.set(8, (1 / (i / TILESIZE)) * 2.0f);
				break;
			}
		}
		//Down
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y + i);
			if (spot.y >= SCREEN_HEIGHT) {
				inputs.set(9, (1 / (i / TILESIZE)) * 2.0f);
				break;
			}
		}
		//Right
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y);
			if (spot.x >= SCREEN_WIDTH) {
				inputs.set(10, (1 / (i / TILESIZE)) * 2.0f);
				break;
			}
		}
		//Left
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y);
			if (spot.x < 0) {
				inputs.set(11, (1 / (i / TILESIZE)) * 2.0f);
				break;
			}
		}


		return inputs;
	}
	///////////////////////////////////////////////////////////////////////

}