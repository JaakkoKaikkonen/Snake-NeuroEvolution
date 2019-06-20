#pragma once

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Snake.hpp"

#include "NeuralNetwork.hpp"
#include "GeneticAlgorithm.hpp"
#include "Matrix.hpp"

#include "ImGuiLog.h"

namespace Game {

	class GameState : public State
	{
	public:
		GameState(gameDataRef data);

		void init();

		void handleInput();
		void update();
		void draw(float dt, bool* fast);

	private:
		gameDataRef data;

		Snake* snake;

		sf::RectangleShape food;

		int moveTimer = 0;

		Dir dir;

		sf::Text gameOverText;

		bool restart = false;

		sf::Clock restartTimer;

		int timeSinceFood = 0;


		int moveDelay = 0;

		//AI//////////////////////////////////////////////////
		int generationSize = 20000;

		int brainIndex = 0;

		GeneticAlgorithm GA;

		Matrix<float> inputs;

		int generationCount = 1;

		int score = 0;

		int highScore = 0;

		int generationBestScore = 0;

		int allTimeHighScore = 0;

		sf::Vector2f* foodPositions;

		bool playBestRun = false;
		bool playingBestRun = false;
		////////////////////////////////////////////////////

		//ImGui/////////////////////////////////////////////
		char snakeFileName[255] = "";

		ImGuiLog ImGuiLog;
		///////////////////////////////////////////////////

	};

}

