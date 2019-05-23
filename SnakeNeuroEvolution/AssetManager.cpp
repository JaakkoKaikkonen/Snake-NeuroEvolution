#include "AssetManager.hpp"

namespace engine {

	void AssetManager::loadFont(std::string name, std::string fileName) {
		sf::Font font;

		if (font.loadFromFile(fileName)) {
			this->_fonts[name] = font;
		}
	}

	sf::Font& AssetManager::getFont(std::string name) {
		return this->_fonts.at(name);
	}

}