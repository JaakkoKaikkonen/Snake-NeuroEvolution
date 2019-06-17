#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"


namespace Game {

	GameState::GameState(gameDataRef _data)
		: data(_data), dot(sf::Vector2f(TILESIZE, TILESIZE)), gameOverText("Game Over", data->assets.getFont("Font")),
		  GA(NeuralNetwork(12, 24, 4), generationSize),
		  inputs(12, 1)
	{
		//ImGui/////////////////////////////////////////////////////////////
		ImGui::SFML::Init(data->window);
		////////////////////////////////////////////////////////////////////

		dot.setFillColor(sf::Color::Red);

		gameOverText.setPosition(SCREEN_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.25);
	}

	void GameState::init() {

		dot.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
		snake = new Snake(data);
		score = 0;
		timeSinceFood = 0;

		//AI/////////////////////////////////////////////////////////////////////// Record score 138
		if (brainIndex == generationSize) {
			brainIndex = 0;

			GA.crossOver();
			GA.mutate(0.01f);


			//ImGui///////////////////////////////////////////////////////////////////////////////////////
			ImGuiLog.AddLog(("BEST SCORE: " + std::to_string(bestScore) + "\n").c_str());
			ImGuiLog.AddLog(("GENERATION: " + std::to_string(generationCount) + "\n").c_str());
			ImGuiLog.AddLog(("GENERATION BEST SCORE: " + std::to_string(generationBestScore) + "\n").c_str());
			///////////////////////////////////////////////////////////////////////////////////////////////


			generationCount++;
			generationBestScore = 0;
		}
		///////////////////////////////////////////////////////////////////////////

	}

	void GameState::handleInput() {
		sf::Event event;
		while (data->window.pollEvent(event)) {
			//ImGui///////////////////////////////////////////////
			ImGui::SFML::ProcessEvent(event);
			/////////////////////////////////////////////////////

			if (sf::Event::Closed == event.type) {
				data->window.close();
			}
		}


		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			dir = Dir::Up;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			dir = Dir::Down;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			dir = Dir::Right;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			dir = Dir::Left;
		}*/


		if (moveTimer >= moveDelay) {

			timeSinceFood++;

			//AI//////////////////////////////////////////////////////////////////
			inputs = snake->getInputs(dot.getPosition());

			//std::cout << "Inputs: " << std::endl;
			//for(int i = 0; i < 12; i++) {
			//	std::cout << inputs.get(i) << std::endl;
			//}

			int output = GA.population.at(brainIndex).predict(inputs);
			//int output = (rand() % 4) + 1;

			switch(output) {
				case 1:
				dir = Dir::Up;
				break;
				case 2:
				dir = Dir::Down;
				break;
				case 3:
				dir = Dir::Right;
				break;
				case 4:
				dir = Dir::Left;
				break;
				default:
				std::cout << "Snake gave bad output" << std::endl;
			}
			/////////////////////////////////////////////////////////////////

			snake->move(dir);
			moveTimer = 0;

		}

	}

	void GameState::update() {


		if (!snake->isDead()) {

			moveTimer++;
			if (moveTimer >= moveDelay) {
				snake->update();
			}

			if (snake->ateDot(dot.getPosition())) {
				score++;
				timeSinceFood = 0;
				do {
					dot.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
				} while (snake->hitDot(dot.getPosition()));
			}

		} else if (!restart){
			restart = true;
			restartTimer.restart();
		}

		if ((restart || timeSinceFood > 700) && restartTimer.getElapsedTime().asSeconds() > 0.0f) {
			restart = false;
			delete snake;

			//AI////////////////////////////////////////
			if (score > bestScore) {
				bestScore = score;
			}
			if (score > generationBestScore) {
				generationBestScore = score;
			}
			GA.scores.at(brainIndex) = score;
			brainIndex++;
			///////////////////////////////////////////

			this->init();
		}

	}

	void GameState::draw(float dt, bool* fast) {

		//ImGui///////////////////////////////////////////////////////////////
		ImGui::SFML::Update(data->window, sf::seconds(dt));


		//ImGui window 1
		ImGui::Begin("Settings");

		ImGui::Text("File name");

		ImGui::InputText("", snakeFileName, 255);

		if (ImGui::Button("Load")) {
			GA.loadFromFile(snakeFileName, ImGuiLog);
		}

		 ImGui::SameLine();

		if (ImGui::Button("Save")) {
			GA.population.at(0).save(snakeFileName, ImGuiLog);
		}

		ImGui::Separator();

		ImGui::Checkbox("Fast", fast);

        ImGui::End();
		/////////////////////////////////////////////////////////////////////

		//ImGui window 2
		ImGuiLog.Draw("Logs");


		data->window.clear(sf::Color(51, 51, 51));
		
		snake->draw();
		
		data->window.draw(dot);


		if (restart) {
			data->window.draw(gameOverText);
		}

		//ImGui/////////////////////////////////
		ImGui::SFML::Render(data->window);
		///////////////////////////////////////

		data->window.display();
	}


}


