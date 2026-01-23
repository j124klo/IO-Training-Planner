#include "AssetManager.hpp"
#include <iostream>		//	temp, for cerr

using namespace std;

void AssetManager::loadTexture(const string& name, const string& fileName)
{
	sf::Texture texture;
	if (texture.loadFromFile(fileName))
	{
		this->_textures[name] = texture;
	}
	else	//	temp
	{
		cerr << "ERROR: Failed to load texture: " << fileName << endl;
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
	else	//	temp
	{
		cerr << "ERROR: Failed to load font: " << fileName << endl;
	}
}

sf::Font& AssetManager::getFont(const string& name)
{
	return this->_fonts.at(name);
}