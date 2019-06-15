#pragma once

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Snake.hpp"

#include "NeuralNetwork.hpp"
#include "GeneticAlgorithm.hpp"
#include "Matrix.hpp"


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
		gameDataRef _data;

		Snake* _snake;

		sf::RectangleShape _dot;

		int _moveTimer = 0;

		Dir _dir;

		sf::Text _gameOverText;

		bool _restart = false;

		sf::Clock _restartTimer;

		int timeSinceFood = 0;


		int moveDelay = 0;

		//AI//////////////////////////////////////////////////
		int generationSize = 20000;

		int brainIndex = 0;

		GeneticAlgorithm GA;

		Matrix<float> inputs;

		int generationCount = 1;

		int _score = 0;

		int _bestScore = 0;

		int _generationBestScore = 0;
		////////////////////////////////////////////////////

		char snakeFileName[255] = "";

	};

}

