#include "Snake.hpp"
#include <iostream>

namespace engine {

	Snake::Snake(gameDataRef data)
		: _data(data)
	{
		_snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
		_snake[0].setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		_snake[0].setOutlineColor(sf::Color::Black);
		_snake[0].setOutlineThickness(1.0f);
	}


	void Snake::update() {

		previousPos1 = _snake[0].getPosition();
		switch(_dir) {
			case Dir::Up:
			_snake[0].move(sf::Vector2f(0.0f, -_speed));
			break;
			case Dir::Down:
			_snake[0].move(sf::Vector2f(0.0f, _speed));
			break;
			case Dir::Right:
			_snake[0].move(sf::Vector2f(_speed, 0.0f));
			break;
			case Dir::Left:
			_snake[0].move(sf::Vector2f(-_speed, 0.0f));
		}
		
		//Collision//////////////////
		if ((_snake[0].getPosition().x < 0) || (_snake[0].getPosition().x > SCREEN_WIDTH - TILESIZE) ||
			(_snake[0].getPosition().y < 0) || (_snake[0].getPosition().y > SCREEN_HEIGHT - TILESIZE)) {
			_dead = true;
		}

		for (int i = 1; i < _snake.size(); i++) {
			if (_snake[0].getPosition() == _snake[i].getPosition()) {
				_dead = true;
			}
		}
		////////////////////////////

		if (_dead) {
			_snake[0].setPosition(previousPos1);
		} else {
			for(int i = 1; i < _snake.size(); i++) {
				previousPos2 = _snake[i].getPosition();
				_snake[i].setPosition(previousPos1);
				std::swap(previousPos1, previousPos2);
			}
		}

	}

	bool Snake::ateDot(sf::Vector2f dot) {
		if (_snake[0].getPosition() == dot) {
			_snake.emplace_back(sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
			_snake[_snake.size() - 1].setPosition(previousPos1);
			_snake[_snake.size() - 1].setOutlineColor(sf::Color::Black);
			_snake[_snake.size() - 1].setOutlineThickness(1.0f);
			return true;
		} else {
			return false;
		}
	}

	bool Snake::hitDot(sf::Vector2f dot) {
		for (int i = 0; i < _snake.size(); i++) {
			if (_snake[i].getPosition() == dot) {
				return true;
			}
		}
		return false;
	}

	void Snake::move(Dir dir) {

		switch(dir) {
			case Dir::Up:
			if(_dir != Dir::Down) {
				_dir = dir;
			}
			break;
			case Dir::Down:
			if(_dir != Dir::Up) {
				_dir = dir;
			}
			break;
			case Dir::Right:
			if(_dir != Dir::Left) {
				_dir = dir;
			}
			break;
			case Dir::Left:
			if(_dir != Dir::Right) {
				_dir = dir;
			}
		}

	}


	void Snake::draw() {
		for (int i = 0; i < _snake.size(); i++) {
			_data->window.draw(_snake[i]);
		}
	}



	//AI///////////////////////////////////////////////////////////////////
	Matrix<float> Snake::getInputs(sf::Vector2f dot) {
		Matrix<float> inputs(12, 1);


		//Distance to snake
		//Up
		bool found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y - i);
			for (int j = 1; j < _snake.size(); j++) {
				if (spot == _snake[j].getPosition()) {
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
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y + i);
			for(int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
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
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y);
			for (int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
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
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y);
			for(int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
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
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y - i);
			for (int j = 1; j < _snake.size(); j++) {
				if (spot == _snake[j].getPosition()) {
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
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y + i);
			for(int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if(found) {
				inputs.set(5, i / TILESIZE);
				break;
			}
		}
		if(!found) {
			inputs.set(5, 100);
		}
		//Down-Left
		found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y + i);
			for(int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if(found) {
				inputs.set(6, i / TILESIZE);
				break;
			}
		}
		if(!found) {
			inputs.set(6, 100);
		}
		//Up-Left
		found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y - i);
			for(int j = 1; j < _snake.size(); j++) {
				if(spot == _snake[j].getPosition()) {
					found = true;
					break;
				}
			}
			if(found) {
				inputs.set(7, i / TILESIZE);
				break;
			}
		}
		if(!found) {
			inputs.set(7, 100);
		}*/




		//distance to food
		//Up
		found = false;
		for (int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y - i);
			if (spot == dot) {
				inputs.set(4, 100/*(1 / (i / TILESIZE)) * 1000*/);
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
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y + i);
			if (spot == dot) {
				inputs.set(5, 100/*(1 / (i / TILESIZE)) * 1000*/);
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
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y);
			if(spot == dot) {
				inputs.set(6, 100/*(1 / (i / TILESIZE)) * 1000*/);
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
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y);
			if(spot == dot) {
				inputs.set(7, 100/*(1 / (i / TILESIZE)) * 1000*/);
				found = true;
				break;
			}
		}
		if (!found) {
			inputs.set(7, 1);
		}
		//Up-Right
		/*found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y - i);
			if(spot == dot) {
				inputs.set(12, i / TILESIZE);
				found = true;
				break;
			}
		}
		if(!found) {
			inputs.set(12, 100);
		}
		//Down-Right
		found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y + i);
			if(spot == dot) {
				inputs.set(13, i / TILESIZE);
				found = true;
				break;
			}
		}
		if(!found) {
			inputs.set(13, 100);
		}
		//Down-Left
		found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y + i);
			if(spot == dot) {
				inputs.set(14, i / TILESIZE);
				found = true;
				break;
			}
		}
		if(!found) {
			inputs.set(14, 100);
		}
		//Up-Left
		found = false;
		for(int i = TILESIZE; i < SCREEN_WIDTH; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y - i);
			if(spot == dot) {
				inputs.set(15, i / TILESIZE);
				found = true;
				break;
			}
		}
		if(!found) {
			inputs.set(15, 100);
		}*/



		//distance to wall
		//Up
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y - i);
			if(spot.y < 0) {
				inputs.set(8, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Down
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x, _snake[0].getPosition().y + i);
			if(spot.y >= SCREEN_HEIGHT) {
				inputs.set(9, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Right
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y);
			if(spot.x >= SCREEN_WIDTH) {
				inputs.set(10, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Left
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y);
			if(spot.x < 0) {
				inputs.set(11, (1 / (i / TILESIZE)) * 2);
				break;
			}
		}
		//Up-Right
		/*for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y - i);
			if(spot.y < 0 || spot.x > SCREEN_WIDTH) {
				inputs.set(20, i / TILESIZE);
				break;
			}
		}
		//Down-Right
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x + i, _snake[0].getPosition().y + i);
			if(spot.y > SCREEN_HEIGHT || spot.x > SCREEN_WIDTH) {
				inputs.set(21, i / TILESIZE);
				break;
			}
		}
		//Down-Left
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y + i);
			if(spot.y > SCREEN_HEIGHT || spot.x < 0) {
				inputs.set(22, i / TILESIZE);
				break;
			}
		}
		//Up-Left
		for(int i = TILESIZE; i <= SCREEN_WIDTH + TILESIZE; i += TILESIZE) {
			sf::Vector2f spot(_snake[0].getPosition().x - i, _snake[0].getPosition().y - i);
			if(spot.y < 0 || spot.x < 0) {
				inputs.set(23, i / TILESIZE);
				break;
			}
		}*/


		//std::cout << _snake[0].getPosition().x / TILESIZE << "\t" << _snake[0].getPosition().y / TILESIZE << std::endl;

		return inputs;
	}
	///////////////////////////////////////////////////////////////////////

}