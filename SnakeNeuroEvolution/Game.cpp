#include "Game.hpp"
#include "GameState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Game {

	Game::Game(int width, int height, std::string title) {

		/*sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);*/

		_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		//_data->window.setVerticalSyncEnabled(true);
		//_data->window.setFramerateLimit(60);

		_data->window.setPosition(sf::Vector2i(500, 200));

		//_data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand(time(NULL));


		//Load Resources ----------------------------------------------------------------------

		//Textures

		//Sounds

		//Fonts
		_data->assets.loadFont("Font", FONT_FILEPATH);

		//-------------------------------------------------------------------------------------

		_data->machine.addState(stateRef(new GameState(_data)), true);

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = this->_clock.getElapsedTime().asSeconds();

		float accumulator = dt;

		float accumulator2 = dt;

		float interpolation = 0.0f;

		


		while (this->_data->window.isOpen()) {

			this->_data->machine.processStateChanges();

			newTime = this->_clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt) {

				this->_data->machine.getActiveState()->draw(dt, &fast);

				this->_data->machine.getActiveState()->handleInput();
				this->_data->machine.getActiveState()->update();
			
				accumulator -= dt;
			}

			if (fast) {
				this->_data->machine.getActiveState()->handleInput();
				this->_data->machine.getActiveState()->update();
			}
			
			

		}


	}


}