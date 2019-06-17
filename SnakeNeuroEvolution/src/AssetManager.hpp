#pragma once

#include <map>

#include <SFML/Graphics.hpp>

namespace Game {

	class AssetManager {

	public:
		AssetManager(){}
		~AssetManager(){}

		void loadFont(std::string name, std::string fileName);
		sf::Font& getFont(std::string name);

	private:
		std::map<std::string, sf::Font> fonts;

	};

}