#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "StateMachine.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"

namespace engine {

	struct GameData {
		StateMachine machine;
		sf::RenderWindow window;
		AssetManager assets;
		InputManager input;
	};

	typedef std::shared_ptr<GameData> gameDataRef;

	class Game {
	public:
		Game(int width, int height, std::string title);


	private:
		sf::Clock _clock;

		const float dt = 1.0f / 60.0f;

		gameDataRef _data = std::make_shared<GameData>();

		void run();

		int speed = 30000;

	};

}
