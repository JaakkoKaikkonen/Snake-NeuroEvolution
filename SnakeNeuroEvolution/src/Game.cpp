#include "Game.hpp"
#include "GameState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Game {

	Game::Game(int width, int height, std::string title) {

		sf::Image icon;
		icon.loadFromFile(TOP_ICON_FILEPATH);

		data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

		data->window.setPosition(sf::Vector2i(500, 200));

		data->window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		srand((unsigned int)time(NULL));


		data->state = new GameState(data);
		data->state->init();

		this->run();
	}

	void Game::run() {
		float newTime, frameTime;

		float currentTime = clock.getElapsedTime().asSeconds();

		float accumulator = dt;
		

		while (this->data->window.isOpen()) {

			newTime = clock.getElapsedTime().asSeconds();

			frameTime = newTime - currentTime;

			currentTime = newTime;

			if (frameTime > 0.15f) {
				frameTime = 0.15f;
			}

			accumulator += frameTime;

			while (accumulator >= dt) {

				data->state->draw(dt, &fast);

				data->state->handleInput();
				data->state->update();
			
				accumulator -= dt;
			}

			if (fast) {
				data->state->handleInput();
				data->state->update();
			}

		}

	}

}