#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include <fstream>

#include "imgui.h"
#include "imgui-SFML.h"


namespace Game {

	GameState::GameState(gameDataRef _data)
		: data(_data),
		  food(sf::Vector2f(TILESIZE, TILESIZE)),
		  gameOverText("Game Over", data->assets.getFont("Font")),
		  GA(NeuralNetwork(NUM_OF_INPUTS, NUM_OF_HIDDEN, NUM_OF_OUTPUT), generationSize),
		  inputs(NUM_OF_INPUTS, 1)
	{
		//ImGui/////////////////////////////////////////////////////////////
		ImGui::SFML::Init(data->window);
		////////////////////////////////////////////////////////////////////

		foodPositions = new sf::Vector2f[1000];

		//Get allTimeHighScore
		std::ifstream allTimeHighScoreInputFile("BEST-SNAKE", std::ios::binary);
		if (allTimeHighScoreInputFile.good()) {
			allTimeHighScoreInputFile.seekg(((NUM_OF_INPUTS * NUM_OF_HIDDEN) + (NUM_OF_HIDDEN) + (NUM_OF_HIDDEN * NUM_OF_OUTPUT) + (NUM_OF_OUTPUT)) * sizeof(float));
			allTimeHighScoreInputFile.read(reinterpret_cast<char*>(&allTimeHighScore), sizeof(int));
		}
		allTimeHighScoreInputFile.close();


		food.setFillColor(sf::Color::Red);

		gameOverText.setPosition(SCREEN_WIDTH / 2 - gameOverText.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.25);
	}

	void GameState::init() {

		if (playingBestRun) {
			food.setPosition(foodPositions[0]);
		} else {
			food.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
			memset(foodPositions, 0, 1000 * sizeof(sf::Vector2f));
			foodPositions[0] = food.getPosition();
		}

		snake = new Snake(data);
		score = 0;
		timeSinceFood = 0;

		//AI/////////////////////////////////////////////////////////////////////// Record score 176
		if (brainIndex == generationSize) {
			brainIndex = 0;

			GA.crossOver();
			GA.mutate(0.01f);


			//ImGui///////////////////////////////////////////////////////////////////////////////////////
			//ImGuiLog.AddLog(("BEST SCORE: " + std::to_string(highScore) + "\n").c_str());
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
			inputs = snake->getInputs(food.getPosition());

			//std::cout << "Inputs: " << std::endl;
			//for (int i = 0; i < NUM_OF_INPUTS; i++) {
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

			if (snake->ateFood(food.getPosition())) {
				score++;
				timeSinceFood = 0;
				//AI/////////////////////////////////////////////////////////
				if (playingBestRun) {
					food.setPosition(foodPositions[score]);
				} else {
				////////////////////////////////////////////////////////////
					do {
						food.setPosition((float)(rand() % (SCREEN_WIDTH / TILESIZE) * TILESIZE), (float)(rand() % (SCREEN_HEIGHT / TILESIZE) * TILESIZE));
					} while (snake->hitDot(food.getPosition()));
					//Store food positions
					foodPositions[score] = food.getPosition();
				}
			}

		} else if (!restart){
			restart = true;
			restartTimer.restart();
		}

		if ((restart || timeSinceFood > 700) && restartTimer.getElapsedTime().asSeconds() > 0.0f) {
			restart = false;
			delete snake;

			//AI/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (score > highScore) {
				highScore = score;
			}
			if (score > generationBestScore) {
				generationBestScore = score;
			}
			GA.scores.at(brainIndex) = score;
			brainIndex++;

			//Save best run
			if (score > allTimeHighScore) {
				allTimeHighScore = score;

				ImGuiLog.AddLog(("New all time highscore: " + std::to_string(allTimeHighScore) + "\n").c_str());
				GA.population.at(brainIndex).save("BEST-SNAKE", ImGuiLog);
				
				std::ofstream bestSnakeFile("BEST-SNAKE", std::ios::binary | std::ios::app);

				if (bestSnakeFile.good()) {

					bestSnakeFile.write(reinterpret_cast<char*>(&allTimeHighScore), sizeof(int));
					bestSnakeFile.write(reinterpret_cast<char*>(foodPositions), (score + 1) * sizeof(sizeof(sf::Vector2f)));

				} else {
					ImGuiLog.AddLog("Problem with \"BEST-SNAKE\" output file\n");
				}
				bestSnakeFile.close();
			}

			/*for (int i = 0; i <= allTimeHighScore; i++) {
				ImGuiLog.AddLog((std::to_string(foodPositions[i].x) + ", " + std::to_string(foodPositions[i].y) + "\n").c_str());
			}
			ImGuiLog.AddLog(std::string("------------------------------\n").c_str());*/

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			this->init();
		}

		//AI/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Start playing best run
		if (playBestRun && !playingBestRun) {
			playingBestRun = true;
			brainIndex = 0;
			GA.loadFromFile("BEST-SNAKE", ImGuiLog);

			std::ifstream f("BEST-SNAKE", std::ios::binary);
			
			//Load food positions
			f.seekg(0, f.end);
			int end = f.tellg();
			f.seekg(((NUM_OF_INPUTS * NUM_OF_HIDDEN) + (NUM_OF_HIDDEN) + (NUM_OF_HIDDEN * NUM_OF_OUTPUT) + (NUM_OF_OUTPUT)) * sizeof(float) + sizeof(int));
			int start = f.tellg();
			f.read(reinterpret_cast<char*>(foodPositions), end - start);

			this->init();
		}
		if (!playBestRun && playingBestRun) {
			playingBestRun = false;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}

	void GameState::draw(float dt, bool* fast) {

		//ImGui///////////////////////////////////////////////////////////////
		ImGui::SFML::Update(data->window, sf::seconds(dt));


		//ImGui window 1
		ImGuiLog.Draw("Logs");

		//ImGui window 2
		ImGui::Begin("Score");
		ImGui::Text(("AllTimeHighScore: " + std::to_string(allTimeHighScore)).c_str());
		ImGui::SameLine();
		ImGui::Text(("HighScore: " + std::to_string(highScore)).c_str());
		ImGui::SameLine();
		ImGui::Text(("Score: " + std::to_string(score)).c_str());
        ImGui::End();

		//ImGui window 3
		ImGui::Begin("Settings");

		ImGui::Text("File name");

		ImGui::InputText("", snakeFileName, 255);

		if (ImGui::Button("Load")) {
			GA.loadFromFile(snakeFileName, ImGuiLog);
		}

		ImGui::SameLine();

		if (ImGui::Button("Save")) {
			if (std::string(snakeFileName) == std::string("BEST-SNAKE")) {
				ImGuiLog.AddLog("Bad file name\n");			
			} else {
				GA.population.at(0).save(snakeFileName, ImGuiLog);
			}
		}

		ImGui::Separator();

		ImGui::Checkbox("Fast", fast);

		ImGui::Separator();

		ImGui::Checkbox("Play best run", &playBestRun);

        ImGui::End();
		/////////////////////////////////////////////////////////////////////

		


		data->window.clear(sf::Color(51, 51, 51));
		
		snake->draw();
		
		data->window.draw(food);


		if (restart) {
			data->window.draw(gameOverText);
		}

		//ImGui/////////////////////////////////
		ImGui::SFML::Render(data->window);
		///////////////////////////////////////

		data->window.display();
	}


}


