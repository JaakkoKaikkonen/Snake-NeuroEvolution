#pragma once

#include <SFML/Graphics.hpp>
#include "DEFINITIONS.hpp"
#include "State.hpp"
#include "Game.hpp"
#include "Snake.hpp"

#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "Matrix.h"


namespace engine {

	class GameState : public State
	{
	public:
		GameState(gameDataRef data);

		void init();

		void handleInput();
		void update(float dt);
		void draw(float interpolation);

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

		int _score = 0;

		int _bestScore = 0;

		int _generationBestScore = 0;


		int moveDelay = 0;

		//AI//////////////////////////////////////////////////
		int generationSize = 20000;

		int brainIndex = 0;

		GeneticAlgorithm GA;

		Matrix<float> inputs;

		int generationCount = 1;
		////////////////////////////////////////////////////
	};

}

