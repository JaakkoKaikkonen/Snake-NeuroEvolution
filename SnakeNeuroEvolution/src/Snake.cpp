#include "Snake.hpp"
#include <iostream>

namespace Game {

	Snake::Snake(gameDataRef _data)
		: data(_data)
	{
		snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
		snake[0].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		snake[0].setFillColor(sf::Color::White);
		snake[0].setOutlineColor(sf::Color(6, 30, 62));
		snake[0].setOutlineThickness(1.0f);

		snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
		snake[1].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TILESIZE);
		snake[1].setFillColor(sf::Color::White);
		snake[1].setOutlineColor(sf::Color(6, 30, 62));
		snake[1].setOutlineThickness(1.0f);
	}


	void Snake::update() {

		previousPos1 = snake[0].getPosition();
		switch(dir) {
			case Dir::Up:
			snake[0].move(sf::Vector2f(0.0f, -speed));
			break;
			case Dir::Down:
			snake[0].move(sf::Vector2f(0.0f, speed));
			break;
			case Dir::Right:
			snake[0].move(sf::Vector2f(speed, 0.0f));
			break;
			case Dir::Left:
			snake[0].move(sf::Vector2f(-speed, 0.0f));
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
			for(int i = 1; i < snake.size(); i++) {
				previousPos2 = snake[i].getPosition();
				snake[i].setPosition(previousPos1);
				std::swap(previousPos1, previousPos2);
			}
		}

	}

	bool Snake::ateDot(sf::Vector2f dot) {
		if (snake[0].getPosition() == dot) {
			snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
			snake[snake.size() - 1].setPosition(previousPos1);
			snake[snake.size() - 1].setFillColor(sf::Color::White);
			snake[snake.size() - 1].setOutlineColor(sf::Color(6, 30, 62));
			snake[snake.size() - 1].setOutlineThickness(1.0f);
			return true;
		} else {
			return false;
		}
	}

	bool Snake::hitDot(sf::Vector2f dot) {
		for (int i = 0; i < snake.size(); i++) {
			if (snake[i].getPosition() == dot) {
				return true;
			}
		}
		return false;
	}

	void Snake::move(Dir _dir) {

		switch(dir) {
			case Dir::Up:
			if (_dir != Dir::Down) {
				dir = _dir;
			}
			break;
			case Dir::Down:
			if (_dir != Dir::Up) {
				dir = _dir;
			}
			break;
			case Dir::Right:
			if (_dir != Dir::Left) {
				dir = _dir;
			}
			break;
			case Dir::Left:
			if (_dir != Dir::Right) {
				dir = _dir;
			}
		}

	}


	void Snake::draw() {
		for (int i = 0; i < snake.size(); i++) {
			data->window.draw(snake[i]);
		}
	}



	//AI///////////////////////////////////////////////////////////////////
	Matrix<float> Snake::getInputs(sf::Vector2f dot) {
		Matrix<float> inputs(12, 1);


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
				inputs.set(0, (1 / (i / TILESIZE)) * 100);
				break;
			}
		}
		if (!found) {
			inputs.set(0, 1);
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
				inputs.set(1, (1 / (i / TILESIZE)) * 100);
				break;
			}
		}
		if (!found) {
			inputs.set(1, 1);
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
				inputs.set(2, (1 / (i / TILESIZE)) * 100);
				break;
			}
		}
		if (!found) {
			inputs.set(2, 1);
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
				inputs.set(3, (1 / (i / TILESIZE)) * 100);
				break;
			}
		}
		if (!found) {
			inputs.set(3, 1);
		}
		//Up-Right
		/*found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y - i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(4, i / TILESIZE);
				break;
			}
		}
		if (!found) {
			inputs.set(4, 100);
		}
		//Down-Right
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y + i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(5, i / TILESIZE);
				break;
			}
		}
		if (!found) {
			inputs.set(5, 100);
		}
		//Down-Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y + i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(6, i / TILESIZE);
				break;
			}
		}
		if (!found) {
			inputs.set(6, 100);
		}
		//Up-Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y - i);
			for (int j = 1; j < snake.size(); j++) {
				if (spot == snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if (found) {
				inputs.set(7, i / TILESIZE);
				break;
			}
		}
		if (!found) {
			inputs.set(7, 100);
		}*/




		//distance to food
		//Up
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y - i);
			if (spot == dot) {
				inputs.set(4, 100/*(1 / (i / TILESIZE)) * 100*/);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(4, 1);
		}
		//Down
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y + i);
			if (spot == dot) {
				inputs.set(5, 100/*(1 / (i / TILESIZE)) * 100*/);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(5, 1);
		}
		//Right
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y);
			if (spot == dot) {
				inputs.set(6, 100/*(1 / (i / TILESIZE)) * 100*/);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(6, 1);
		}
		//Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y);
			if (spot == dot) {
				inputs.set(7, 100/*(1 / (i / TILESIZE)) * 100*/);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(7, 1);
		}
		//Up-Right
		/*found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y - i);
			if (spot == dot) {
				inputs.set(12, i / TILESIZE);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(12, 100);
		}
		//Down-Right
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y + i);
			if (spot == dot) {
				inputs.set(13, i / TILESIZE);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(13, 100);
		}
		//Down-Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y + i);
			if (spot == dot) {
				inputs.set(14, i / TILESIZE);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(14, 100);
		}
		//Up-Left
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y - i);
			if (spot == dot) {
				inputs.set(15, i / TILESIZE);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(15, 100);
		}*/



		//distance to wall
		//Up
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y - i);
			if (spot.y < 0) {
				inputs.set(8, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Down
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x, snake[0].getPosition().y + i);
			if (spot.y >= SCREEN_HEIGHT) {
				inputs.set(9, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Right
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y);
			if (spot.x >= SCREEN_WIDTH) {
				inputs.set(10, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Left
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y);
			if (spot.x < 0) {
				inputs.set(11, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Up-Right
		/*for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y - i);
			if (spot.y < 0 || spot.x > SCREEN_WIDTH) {
				inputs.set(20, i / TILESIZE);
				break;
			}
		}
		//Down-Right
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x + i, snake[0].getPosition().y + i);
			if (spot.y > SCREEN_HEIGHT || spot.x > SCREEN_WIDTH) {
				inputs.set(21, i / TILESIZE);
				break;
			}
		}
		//Down-Left
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y + i);
			if (spot.y > SCREEN_HEIGHT || spot.x < 0) {
				inputs.set(22, i / TILESIZE);
				break;
			}
		}
		//Up-Left
		for (int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(snake[0].getPosition().x - i, snake[0].getPosition().y - i);
			if (spot.y < 0 || spot.x < 0) {
				inputs.set(23, i / TILESIZE);
				break;
			}
		}*/


		//std::cout << snake[0].getPosition().x / TILESIZE << "\t" << snake[0].getPosition().y / TILESIZE << std::endl;

		return inputs;
	}
	///////////////////////////////////////////////////////////////////////

}