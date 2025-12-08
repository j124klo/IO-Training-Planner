#include "AssetManager.hpp"

using namespace std;

void AssetManager::loadTexture(const string& name, const string& fileName)
{
	sf::Texture texture;
	if (texture.loadFromFile(fileName))
	{
		this->_textures[name] = texture;
	}
}

sf::Texture& AssetManager::getTexture(const string& name)
{
	return this->_textures.at(name);
}

void AssetManager::loadFont(const string& name, const string& fileName)
{
	sf::Font font;
	if (font.loadFromFile(fileName))
	{
		this->_fonts[name] = font;
	}
}

sf::Font& AssetManager::getFont(const string& name)
{
	return this->_fonts.at(name);
}