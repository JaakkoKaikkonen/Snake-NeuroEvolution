#include "Game.hpp"
#include "GameState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Game {

	Game::Game(int width, int height, std::string title) {

		sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);

		data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		//data->window.setVerticalSyncEnabled(true);
		//data->window.setFramerateLimit(60);

		data->window.setPosition(sf::Vector2i(500, 200));

		data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand((unsigned int)time(NULL));


		//Load Resources ----------------------------------------------------------------------

		//Textures

		//Sounds

		//Fonts
		data->assets.loadFont("Font", FONT_FILEPATH);

		//-------------------------------------------------------------------------------------

		data->machine.addState(stateRef(new GameState(data)), true);

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = this->clock.getElapsedTime().asSeconds();

		float accumulator = dt;

		float accumulator2 = dt;

		float interpolation = 0.0f;

		


		while (this->data->window.isOpen()) {

			this->data->machine.processStateChanges();

			newTime = this->clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt) {

				this->data->machine.getActiveState()->draw(dt, &fast);

				this->data->machine.getActiveState()->handleInput();
				this->data->machine.getActiveState()->update();
			
				accumulator -= dt;
			}

			if (fast) {
				this->data->machine.getActiveState()->handleInput();
				this->data->machine.getActiveState()->update();
			}
			
			

		}


	}


}