#include "AssetManager.hpp"

namespace Game {

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			this->fonts[name] = font;
		}
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return this->fonts.at(name);
	}

}