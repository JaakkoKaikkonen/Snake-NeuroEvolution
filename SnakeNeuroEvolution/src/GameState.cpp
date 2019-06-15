#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"


namespace Game {

	GameState::GameState(gameDataRef data)
		: _data(data), _dot(sf::Vector2f(TILESIZE, TILESIZE)), _gameOverText("Game Over", _data->assets.getFont("Font")),
		  GA(NeuralNetwork(12, 24, 4), generationSize),
		  inputs(12, 1)
	{
		//ImGui/////////////////////////////////////////////////////////////
		ImGui::SFML::Init(_data->window);
		////////////////////////////////////////////////////////////////////

		_dot.setFillColor(sf::Color::Red);

		_gameOverText.setPosition(SCREEN_WIDTH / 2 - _gameOverText.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.25);
	}

	void GameState::init() {

		//std::cout << "Game state" << std::endl;
		_dot.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
		_snake = new Snake(_data);
		_score = 0;
		timeSinceFood = 0;

		//AI/////////////////////////////////////////////////////////////////////// Record score 122
		if (brainIndex == generationSize) {
			brainIndex = 0;

			GA.crossOver();
			GA.mutate(0.01f);

			
			std::cout << "GENERATION: " << generationCount << std::endl;
			std::cout << "GENERATION BEST SCORE: " << _generationBestScore << std::endl;
			std::cout << "BEST_SCORE: " << _bestScore << std::endl;


			generationCount++;
			_generationBestScore = 0;
		}

		//std::cout << "brain_index: " << brainIndex << std::endl;
		///////////////////////////////////////////////////////////////////////////
	}

	void GameState::handleInput() {
		sf::Event event;
		while (_data->window.pollEvent(event)) {
			//ImGui///////////////////////////////////////////////
			ImGui::SFML::ProcessEvent(event);
			/////////////////////////////////////////////////////

			if (sf::Event::Closed == event.type) {
				_data->window.close();
			}
		}


		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			_dir = Dir::Up;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			_dir = Dir::Down;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			_dir = Dir::Right;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			_dir = Dir::Left;
		}*/


		if (_moveTimer >= moveDelay) {

			timeSinceFood++;

			//AI//////////////////////////////////////////////////////////////////
			inputs = _snake->getInputs(_dot.getPosition());

			/*std::cout << "Inputs: " << std::endl;
			for(int i = 0; i < 12; i++) {
				std::cout << inputs.get(i) << std::endl;
			}*/

			int output = GA.population.at(brainIndex).predict(inputs);
			//int output = (rand() % 4) + 1;

			switch(output) {
				case 1:
				_dir = Dir::Up;
				break;
				case 2:
				_dir = Dir::Down;
				break;
				case 3:
				_dir = Dir::Right;
				break;
				case 4:
				_dir = Dir::Left;
				break;
				default:
				std::cout << "Snake gave bad output" << std::endl;
			}
			/////////////////////////////////////////////////////////////////

			_snake->move(_dir);
			_moveTimer = 0;

		}

	}

	void GameState::update() {


		if (!_snake->dead()) {

			_moveTimer++;
			if (_moveTimer >= moveDelay) {
				_snake->update();
			}

			if (_snake->ateDot(_dot.getPosition())) {
				_score++;
				timeSinceFood = 0;
				do {
					_dot.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
				} while (_snake->hitDot(_dot.getPosition()));
			}

		} else if (!_restart){
			_restart = true;
			_restartTimer.restart();
		}

		if ((_restart || timeSinceFood > 700) && _restartTimer.getElapsedTime().asSeconds() > 0.0f) {
			_restart = false;
			delete _snake;

			//AI////////////////////////////////////////
			if (_score > _bestScore) {
				_bestScore = _score;
			}
			if (_score > _generationBestScore) {
				_generationBestScore = _score;
			}
			GA.scores.at(brainIndex) = _score;
			brainIndex++;
			///////////////////////////////////////////

			this->init();
		}

	}

	void GameState::draw(float dt, bool* fast) {

		//ImGui///////////////////////////////////////////////////////////////
		ImGui::SFML::Update(_data->window, sf::seconds(dt));

		ImGui::Begin("Settings");

		ImGui::Checkbox("Fast", fast);

		ImGui::Text("File name");

		ImGui::InputText("", snakeFileName, 255);

		if (ImGui::Button("Load")) {
			GA.loadFromFile(snakeFileName);
		}

		if (ImGui::Button("Save")) {
			GA.population.at(0).save(snakeFileName);
		}

        ImGui::End();
		/////////////////////////////////////////////////////////////////////


		_data->window.clear(sf::Color(51, 51, 51));
		
		_snake->draw();
		
		_data->window.draw(_dot);


		if (_restart) {
			_data->window.draw(_gameOverText);
		}

		//ImGui/////////////////////////////////
		ImGui::SFML::Render(_data->window);
		///////////////////////////////////////

		_data->window.display();
	}


}


