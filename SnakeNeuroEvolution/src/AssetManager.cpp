#include "AssetManager.hpp"

namespace Game {

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			fonts[name] = font;
		}
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return fonts.at(name);
	}

}